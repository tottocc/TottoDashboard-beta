#include "MyMainFrame.h"

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

	// Update com port list
	unsigned int num = _serial.UpdateComPortList();
	for (int i = 0; i<num; i++) {
		m_comboBoxCom->Append(_serial.GetComPortDesc(i));
	}
	if (num > 0) {
		m_comboBoxCom->SetSelection(0);
		m_comboBoxCom->SetStringSelection(m_comboBoxCom->GetStringSelection());
	}
}

void MyMainFrame::comboBoxComDropDown(wxCommandEvent& event)
{
	// Update com port list
	m_comboBoxCom->Clear();
	unsigned int num = _serial.UpdateComPortList();
	for (int i = 0; i<num; i++) {
		m_comboBoxCom->Append(_serial.GetComPortDesc(i));
	}
}

void MyMainFrame::btnOpenComClick(wxCommandEvent& event)
{
	// TODO: Implement btnOpenComClick
	if (m_buttonCom->GetLabel() == "Open") {
		if (!_serial.Create(m_comboBoxCom->GetSelection(), 115200))
		{
			fprintf(stderr, "Fail to open COM port");
			fprintf(stdout, "Fail to open COM port");
			return;
		}
		m_comboBoxCom->Enable(false);
		m_button1->Enable(true);
		m_button2->Enable(true);
		m_button3->Enable(true);
		ChopperCtrlPort(1, true);
		ChopperCtrlPort(2, true);
		ChopperCtrlPort(3, true);
		m_button1->SetBackgroundColour(ON_STATE_COLOUR);
		m_button2->SetBackgroundColour(ON_STATE_COLOUR);
		m_button3->SetBackgroundColour(ON_STATE_COLOUR);
		m_buttonCom->SetLabel(wxT("Close"));
	}
	else {
		_serial.Delete();
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
		ChopperCtrlPort(1, true);
		m_button1->SetBackgroundColour(ON_STATE_COLOUR);
	}
	else {
		ChopperCtrlPort(1, false);
		m_button1->SetBackgroundColour(OFF_STATE_COLOUR);
	}
}

void MyMainFrame::btn2Click(wxCommandEvent& event)
{
	// TODO: Implement btn2Click
	if (m_button2->GetBackgroundColour() == OFF_STATE_COLOUR) {
		ChopperCtrlPort(2, true);
		m_button2->SetBackgroundColour(ON_STATE_COLOUR);
	}
	else {
		ChopperCtrlPort(2, false);
		m_button2->SetBackgroundColour(OFF_STATE_COLOUR);
	}
}

void MyMainFrame::btn3Click(wxCommandEvent& event)
{
	// TODO: Implement btn3Click
	if (m_button3->GetBackgroundColour() == OFF_STATE_COLOUR) {
		ChopperCtrlPort(3, true);
		m_button3->SetBackgroundColour(ON_STATE_COLOUR);
	}
	else {
		ChopperCtrlPort(3, false);
		m_button3->SetBackgroundColour(OFF_STATE_COLOUR);
	}
}

//
// Control USB Chopper Port
//
void MyMainFrame::ChopperCtrlPort(unsigned int ch, bool on)
{
	char buf[256];

	if (on == true) {
		sprintf(buf, "usb en %d\r\n", ch);
	}
	else {
		sprintf(buf, "usb dis %d\r\n", ch);
	}
	_serial.Send((BYTE *)buf, strlen(buf));
	msleep(10);
	while (_serial.RecvLength())
	{
		_serial.Recv((BYTE *)buf, _serial.RecvLength());
	}
}
