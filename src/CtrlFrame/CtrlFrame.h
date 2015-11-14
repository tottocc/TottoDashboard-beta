#ifndef __CTRLFRAME_H__
#define __CTRLFRAME_H__

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include "CtrlForm.h"
#include "../serial/TottoCmd.h"
#include "../GraphFrame/GraphFrame.h"


/** Implementing MainFrame */
class CtrlFrame : public MainFrame
{
public:
	/** Constructor */
	CtrlFrame(wxWindow* parent);

	~CtrlFrame();

protected:
	// Handlers for MainFrame events.
	void comboBoxComDropDown(wxCommandEvent& event);
	void btnOpenComClick(wxCommandEvent& event);
	void btn1Click(wxCommandEvent& event);
	void btn2Click(wxCommandEvent& event);
	void btn3Click(wxCommandEvent& event);

	void OnIdle(wxIdleEvent& event);
	DECLARE_EVENT_TABLE();

private:
	void UpdateChoiceComList();
	TottoCmd _tottoCmd;

	MeasureData *mdata;
	GraphFrame *gframe;
};

#endif // __CTRLFRAME_H__
