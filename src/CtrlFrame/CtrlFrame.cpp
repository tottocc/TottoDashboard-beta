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
// thread to make dummy data
// ----------------------------------------------------------------------------
class MyThread : public wxThread
{
public:
	MyThread(CtrlFrame *frame, MeasureData *dat);
	virtual ~MyThread();

	virtual void *Entry(); // thread execution starts here

public:
	CtrlFrame *m_frame;
	MeasureData *m_data;
};

MyThread::MyThread(CtrlFrame *frame, MeasureData *dat)
	: wxThread()
{
	m_frame = frame;
	m_data  = dat;
}

MyThread::~MyThread()
{
	wxCriticalSectionLocker locker(m_frame->m_critsect);

	wxArrayThread& threads = m_frame->m_threads;
	threads.Remove(this);

	if (threads.IsEmpty())
	{
		// signal the main thread that there are no more threads left if it is
		// waiting for us
		if (m_frame->m_shuttingDown)
		{
			m_frame->m_shuttingDown = false;

			m_frame->m_semAllDone.Post();
		}
	}
}

wxThread::ExitCode MyThread::Entry()
{
	while (true)
	{
		// check if the application is shutting down: in this case all threads
		// should stop a.s.a.p.
		{
			wxCriticalSectionLocker locker(m_frame->m_critsect);
			if (m_frame->m_shuttingDown)
				return NULL;
		}

		// check if just this thread was asked to exit
		if (TestDestroy())
			break;

		if (!m_data->AddSample(0.0))
			return NULL;

		wxThread::Sleep(10);   // Call this in the thread instead of wxSleep()
	}
}


// ----------------------------------------------------------------------------
// CtrlFrame
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

	// Create thread to make dummy data
	m_shuttingDown = false;
	MyThread *thread = new MyThread(this, mdata);

	if (thread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can't create thread!"));
	}

	wxCriticalSectionLocker enter(this->m_critsect);
	this->m_threads.Add(thread);

	if (thread->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can't start thread!"));
	}
}

CtrlFrame::~CtrlFrame()
{
	// Disconnect Events
	m_comboBoxCom->Disconnect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(CtrlFrame::comboBoxComDropDown), NULL, this);
	// Need to confirm frame is still live or not belore delete it
	//
	// delete gframe;

	// tell all the threads to terminate: note that they can't terminate while
	// we're deleting them because they will block in their OnExit() -- this is
	// important as otherwise we might access invalid array elements
	{
		wxCriticalSectionLocker locker(this->m_critsect);

		// check if we have any threads running first
		const wxArrayThread& threads = this->m_threads;
		size_t count = threads.GetCount();

		if (!count)
			return;

		// set the flag indicating that all threads should exit
		this->m_shuttingDown = true;
	}

	// now wait for them to really terminate
	this->m_semAllDone.Wait();

	/*
	// sample to delte one thread
	wxThread* toDelete = NULL;
	{
	wxCriticalSectionLocker enter(this->m_critsect);

	// stop the last thread
	if (this->m_threads.IsEmpty())
	{
	wxLogError(wxT("No thread to stop!"));
	}
	else
	{
	toDelete = this->m_threads.Last();
	}
	}

	if (toDelete)
	{
	// This can still crash if the thread gets to delete itself
	// in the mean time.
	toDelete->Delete();

	}
	*/
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
}
