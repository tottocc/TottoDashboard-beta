#include "CtrlFrame.h"
#include "../serial/fifo.h"

#include <stdio.h>
#include <stdlib.h>
#include <wx/colour.h>

#include <string>
using namespace std;


// the application icon for Linux and Mac-OSX
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../res/tottoIcon.xpm"
#endif

#if !wxUSE_THREADS
#error "This sample requires thread support!"
#endif // wxUSE_THREADS

#include <wx/thread.h>

#define ON_STATE_COLOUR  wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)
#define OFF_STATE_COLOUR wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE)

#ifdef _WIN32
#include <windows.h>
#define msleep(X) Sleep(X)
#else
#define msleep(X) usleep((X) * 1000)
#endif

wxBEGIN_EVENT_TABLE(GraphFrame, wxFrame)
EVT_MENU(wxID_EXIT, GraphFrame::OnExit)
wxEND_EVENT_TABLE()


BEGIN_EVENT_TABLE(CtrlFrame, wxFrame)
EVT_IDLE(CtrlFrame::OnIdle)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// ID for the menu commands
enum
{
	THREAD_QUIT = wxID_EXIT,
	THREAD_ABOUT = wxID_ABOUT,
	THREAD_TEXT = 101,
	THREAD_CLEAR,
	THREAD_START_THREAD = 201,
	THREAD_START_THREADS,
	THREAD_STOP_THREAD,
	THREAD_PAUSE_THREAD,
	THREAD_RESUME_THREAD,

	THREAD_START_WORKER,
	THREAD_EXEC_MAIN,
	THREAD_START_GUI_THREAD,

	THREAD_SHOWCPUS,

	WORKER_EVENT = wxID_HIGHEST + 1,   // this one gets sent from MyWorkerThread
	GUITHREAD_EVENT                  // this one gets sent from MyGUIThread
};



// ----------------------------------------------------------------------------
// a worker thread
// ----------------------------------------------------------------------------

class MyWorkerThread : public wxThread
{
public:
	MyWorkerThread(CtrlFrame *frame);

	// thread execution starts here
	virtual void *Entry();

	// called when the thread exits - whether it terminates normally or is
	// stopped with Delete() (but not when it is Kill()ed!)
	virtual void OnExit();

public:
	CtrlFrame *m_frame;
	unsigned m_count;
};

MyWorkerThread::MyWorkerThread(CtrlFrame *frame)
	: wxThread()
{
	m_frame = frame;
	m_count = 0;
}

void MyWorkerThread::OnExit()
{
}

wxThread::ExitCode MyWorkerThread::Entry()
{
	for (m_count = 0; !m_frame->Cancelled() && (m_count < 100); m_count++)
	{
		// check if we were asked to exit
		if (TestDestroy())
			break;

		// create any type of command event here
		wxThreadEvent event(wxEVT_THREAD, WORKER_EVENT);
		event.SetInt(m_count);

		// send in a thread-safe way
		wxQueueEvent(m_frame, event.Clone());

		wxMilliSleep(200);
	}

	wxThreadEvent event(wxEVT_THREAD, WORKER_EVENT);
	event.SetInt(-1); // that's all
	wxQueueEvent(m_frame, event.Clone());

	return NULL;
}

// ----------------------------------------------------------------------------
// CtrlFrame class
// ----------------------------------------------------------------------------

CtrlFrame::CtrlFrame(wxWindow* parent)
	:
	MainFrame(parent)
{
	// Set the frame icon
	SetIcon(wxICON(tottoIcon));

	// Set window position
	SetPosition(wxPoint(30, 30));

	// Connect Events
	m_comboBoxCom->Connect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(CtrlFrame::comboBoxComDropDown), NULL, this);

	// Update com port list
	UpdateChoiceComList();

	// For sampling data
	mdata = new MeasureData();

	// Create the Graph frame window
	gframe = new GraphFrame(NULL, mdata);
	gframe->SetTitle(wxT("Totto Dashboard"));
	gframe->Show(true);
}

CtrlFrame::~CtrlFrame()
{
	// Disconnect Events
	m_comboBoxCom->Disconnect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(CtrlFrame::comboBoxComDropDown), NULL, this);
	// Need to confirm frame is still live or not belore delete it
	//
	// delete gframe;
}

void CtrlFrame::UpdateChoiceComList()
{
	unsigned int num = _tottoCmd.UpdateComPortList();
	m_comboBoxCom->Clear();
	if (num > 0) {
		for (int i = 0; i<num; i++) {
			m_comboBoxCom->Append(_tottoCmd.GetComPortDesc(i));
		}
		m_comboBoxCom->SetSelection(_tottoCmd.FindComPortListIndexWithName("USB Totto"));
		m_comboBoxCom->SetStringSelection(m_comboBoxCom->GetStringSelection());
	}
	else {
		m_comboBoxCom->Append(wxT("No available COM port"));
		m_comboBoxCom->SetSelection(0);
	}
}

bool CtrlFrame::Cancelled()
{
	wxCriticalSectionLocker lock(m_csCancelled);

	return m_cancelled;
}

//
// Event handlers
//
void CtrlFrame::comboBoxComDropDown(wxCommandEvent& event)
{
	// Update com port list
	UpdateChoiceComList();
}

void CtrlFrame::btnOpenComClick(wxCommandEvent& event)
{
	// TODO: Implement btnOpenComClick
	if (m_buttonCom->GetLabel() == "Open") {
		if (!_tottoCmd.ConnectCom(m_comboBoxCom->GetSelection(), 115200))
		{
			fprintf(stderr, "Fail to open COM port\r\n");
			return;
		}
		m_comboBoxCom->Enable(false);
		m_button1->Enable(true);
		m_button2->Enable(true);
		m_button3->Enable(true);
		_tottoCmd.CtrlPort(1, true);
		_tottoCmd.CtrlPort(2, true);
		_tottoCmd.CtrlPort(3, true);
		m_button1->SetBackgroundColour(ON_STATE_COLOUR);
		m_button2->SetBackgroundColour(ON_STATE_COLOUR);
		m_button3->SetBackgroundColour(ON_STATE_COLOUR);
		m_buttonCom->SetLabel(wxT("Close"));
	}
	else {
		_tottoCmd.DisconnectCom();
		m_button1->Enable(false);
		m_button2->Enable(false);
		m_button3->Enable(false);
		m_comboBoxCom->Enable(true);
		m_buttonCom->SetLabel(wxT("Open"));
	}
}

void CtrlFrame::btn1Click(wxCommandEvent& event)
{
	// TODO: Implement btn1Click
	if (m_button1->GetBackgroundColour() == OFF_STATE_COLOUR) {
		_tottoCmd.CtrlPort(1, true);
		m_button1->SetBackgroundColour(ON_STATE_COLOUR);
	}
	else {
		_tottoCmd.CtrlPort(1, false);
		m_button1->SetBackgroundColour(OFF_STATE_COLOUR);
	}
}

void CtrlFrame::btn2Click(wxCommandEvent& event)
{
	// TODO: Implement btn2Click
	if (m_button2->GetBackgroundColour() == OFF_STATE_COLOUR) {
		_tottoCmd.CtrlPort(2, true);
		m_button2->SetBackgroundColour(ON_STATE_COLOUR);
	}
	else {
		_tottoCmd.CtrlPort(2, false);
		m_button2->SetBackgroundColour(OFF_STATE_COLOUR);
	}
}

void CtrlFrame::btn3Click(wxCommandEvent& event)
{
	// TODO: Implement btn3Click
	if (m_button3->GetBackgroundColour() == OFF_STATE_COLOUR) {
		_tottoCmd.CtrlPort(3, true);
		m_button3->SetBackgroundColour(ON_STATE_COLOUR);
	}
	else {
		_tottoCmd.CtrlPort(3, false);
		m_button3->SetBackgroundColour(OFF_STATE_COLOUR);
	}
}

void CtrlFrame::OnIdle(wxIdleEvent& event)
{
	mdata->AddSample(0.0);
}