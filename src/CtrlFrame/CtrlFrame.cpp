#include "CtrlFrame.h"
#include "../serial/fifo.h"

#include <stdio.h>
#include <stdlib.h>
#include <wx/colour.h>

#include <string>
using namespace std;


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// the application icon for Linux and Mac-OSX
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../res/tottoIcon.xpm"
#endif

#include <wx/gdicmn.h>

#define ON_STATE_COLOUR *wxRED
#define OFF_STATE_COLOUR wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE)
#define MEASURING_STATE_COLOUR wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)

#ifdef _WIN32
#include <windows.h>
#define msleep(X) Sleep(X)
#else
#define msleep(X) usleep((X) * 1000)
#endif

enum{
DO_TEST
};

BEGIN_EVENT_TABLE(CtrlFrame, wxFrame)
EVT_MENU(wxID_EXIT, CtrlFrame::OnExit)
EVT_IDLE(CtrlFrame::OnIdle)
EVT_WINDOW_DESTROY(CtrlFrame::ChildWindowClose)
END_EVENT_TABLE()


// ----------------------------------------------------------------------------
// Data array class for GraphFrame and MeasureData
// ----------------------------------------------------------------------------
GraphDataArray::GraphDataArray()
{
	for (int i = 0; i < GRAPH_DATA_NUM_MAX; i++)
	{	
		mdata[i] = NULL;
		wid[i] = 0;
	}
}

GraphDataArray::~GraphDataArray()
{
}

bool GraphDataArray::Add(wxWindowID id, MeasureData *md)
{
	int i = findAvilableIndex();
	if (i == NOT_AVAILABLE)
		return false;
	mdata[i] = md;
	wid[i] = id;

	return true;
}

bool GraphDataArray::IsAvailable()
{
	return findAvilableIndex() != NOT_AVAILABLE;
}

bool GraphDataArray::Delete(wxWindowID id)
{
	int i = findIndexById(id);
	delete mdata[i];
	mdata[i] = NULL;
	wid[i] = 0;

	return true;
}

bool GraphDataArray::DeleteAll()
{
	int i = 0;
	while (mdata[i]) {
		delete mdata[i];
		mdata[i] = NULL;
		wid[i] = 0;
		i++;
		if (i == GRAPH_DATA_NUM_MAX)
			break;
	};
	return true;
}

int GraphDataArray::findAvilableIndex()
{
	int i = 0;
	while (mdata[i]) {
		i++;
		if (i == NOT_AVAILABLE)   // NOT_AVAILABLE == GRAPH_DATA_NUM_MAX
			break;
	};
	return i;
}

int GraphDataArray::findIndexById(wxWindowID id)
{
	int i = 0;
	while (wid[i] != id) {
		i++;
		if (i == GRAPH_DATA_NUM_MAX)   // this codition should not be true
			break;
	};
	return i;
}


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
	SetPosition(wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)-400, 30));

	// Make a menubar
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_EXIT, wxT("E&xit"));
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	SetMenuBar(menuBar);

	// Initialize parameters
	currentOutputWindowId = 0;

	// Connect Events
	m_comboBoxCom->Connect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(CtrlFrame::comboBoxComDropDown), NULL, this);

	// Update com port list
	updateChoiceComList();
}

CtrlFrame::~CtrlFrame()
{
	// Disconnect Events
	m_comboBoxCom->Disconnect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(CtrlFrame::comboBoxComDropDown), NULL, this);

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

	gDataArray.DeleteAll();
}

void CtrlFrame::updateChoiceComList()
{
	unsigned int num = tottoCmd.UpdateComPortList();
	m_comboBoxCom->Clear();
	if (num > 0) {
		for (int i = 0; i<num; i++) {
			m_comboBoxCom->Append(tottoCmd.GetComPortDesc(i));
		}
		m_comboBoxCom->SetSelection(tottoCmd.FindComPortListIndexWithName("USB Totto"));
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
	updateChoiceComList();
	// Need to fix a bug when opening it without COM Port
}

void CtrlFrame::btnOpenComClick(wxCommandEvent& event)
{
	if (m_buttonCom->GetLabel() == "Open") {
		if (!tottoCmd.ConnectCom(m_comboBoxCom->GetSelection(), 115200))
		{
			fprintf(stderr, "Fail to open COM port\r\n");
			return;
		}
		m_comboBoxCom->Enable(false);
//		m_buttonOnOff->Enable(true);
		m_buttonStartStop->Enable(true);
		m_buttonSingle->Enable(true);
		m_buttonCom->SetLabel(wxT("Close"));
	}
	else {
		tottoCmd.DisconnectCom();
		m_buttonStartStop->Enable(false);
		m_buttonSingle->Enable(false);
//		m_buttonOnOff->Enable(false);
		m_comboBoxCom->Enable(true);
		m_buttonCom->SetLabel(wxT("Open"));
	}
}

/*
void CtrlFrame::btnOnOffClick(wxCommandEvent& event)
{
	if (m_buttonOnOff->GetBackgroundColour() == OFF_STATE_COLOUR) {
		//		tottoCmd.CtrlPort(3, true);
		m_buttonOnOff->SetBackgroundColour(ON_STATE_COLOUR);
		m_buttonOnOff->SetLabel(wxT("OFF"));
	}
	else {
		//		tottoCmd.CtrlPort(3, false);
		m_buttonOnOff->SetBackgroundColour(OFF_STATE_COLOUR);
		m_buttonOnOff->SetLabel(wxT("ON"));
	}
}
*/

void CtrlFrame::btnStartStopClick(wxCommandEvent& event)
{
	if (m_buttonStartStop->GetBackgroundColour() == OFF_STATE_COLOUR)
	{
		// Start click
		m_buttonStartStop->SetBackgroundColour(MEASURING_STATE_COLOUR);
		m_buttonStartStop->SetLabel(wxT("Stop"));

		if (!gDataArray.IsAvailable())
		{
			wxLogError(wxT("Too many graph windows opened"));
			return;
		}

		// Create sampling data and the Graph frame objects
		MeasureData *mdata = new MeasureData();
		GraphFrame *gframe = new GraphFrame(this, mdata);
		gframe->SetTitle(wxT("Graph"));

		wxPoint pos = this->GetScreenPosition() - wxPoint(500, 0);
		gframe->SetPosition(pos);

		gframe->Show(true);
		currentOutputWindowId = gframe->GetId();
		gDataArray.Add(currentOutputWindowId, mdata);

		// Create thread to make dummy data
		m_shuttingDown = false;
		MyThread *thread = new MyThread(this, mdata);

		if (thread->Create() != wxTHREAD_NO_ERROR)
		{
			wxLogError(wxT("Can't create thread!"));
			return;
		}

		wxCriticalSectionLocker enter(this->m_critsect);
		this->m_threads.Add(thread);

		if (thread->Run() != wxTHREAD_NO_ERROR)
		{
			wxLogError(wxT("Can't start thread!"));
			return;
		}
	}
	else {
		// Stop click
		m_buttonStartStop->SetBackgroundColour(OFF_STATE_COLOUR);
		m_buttonStartStop->SetLabel(wxT("Start"));
		currentOutputWindowId = 0;
		
		// sample to delte one thread
		wxThread* toDelete = NULL;
		{
			wxCriticalSectionLocker enter(this->m_critsect);

			// stop the last thread
			if (this->m_threads.IsEmpty())
			{
				wxLogError(wxT("No thread to stop!"));
				return;
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
	}
}

// Intercept menu commands
void CtrlFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void CtrlFrame::OnIdle(wxIdleEvent& event)
{
}

void CtrlFrame::ChildWindowClose(wxWindowDestroyEvent& event)
{
	wxWindow *p = event.GetWindow();
	wxWindowID id = p->GetId();

	if (id==currentOutputWindowId)
	{
		// Stop click
		m_buttonStartStop->SetBackgroundColour(OFF_STATE_COLOUR);
		m_buttonStartStop->SetLabel(wxT("START"));

		// sample to delte one thread
		wxThread* toDelete = NULL;
		{
			wxCriticalSectionLocker enter(this->m_critsect);

			// stop the last thread
			if (this->m_threads.IsEmpty())
			{
				wxLogError(wxT("No thread to stop!"));
				return;
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

	}

	gDataArray.Delete(id);
}
