#include "MyMainFrame.h"
#include "serial/fifo.h"

#include <stdio.h>
#include <stdlib.h>
#include <wx/colour.h>

#include <string>
using namespace std;


// the application icon for Linux and Mac-OSX
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "res/tottoIcon.xpm"
#endif

#define ON_STATE_COLOUR  wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)
#define OFF_STATE_COLOUR wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE)

#ifdef _WIN32
#include <windows.h>
#define msleep(X) Sleep(X)
#else
#define msleep(X) usleep((X) * 1000)
#endif

MyMainFrame::MyMainFrame(wxWindow* parent)
	:
	MainFrame(parent)
{
	// set the frame icon
	SetIcon(wxICON(tottoIcon));

	// Connect Events
	m_comboBoxCom->Connect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(MyMainFrame::comboBoxComDropDown), NULL, this);

	// Update com port list
	UpdateChoiceComList();
}

MyMainFrame::~MyMainFrame()
{
	// Disconnect Events
	m_comboBoxCom->Disconnect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(MyMainFrame::comboBoxComDropDown), NULL, this);
}

void MyMainFrame::UpdateChoiceComList()
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
void MyMainFrame::comboBoxComDropDown(wxCommandEvent& event)
{
	// Update com port list
	UpdateChoiceComList();
}

void MyMainFrame::btnOpenComClick(wxCommandEvent& event)
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

void MyMainFrame::btn1Click(wxCommandEvent& event)
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

void MyMainFrame::btn2Click(wxCommandEvent& event)
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

void MyMainFrame::btn3Click(wxCommandEvent& event)
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

