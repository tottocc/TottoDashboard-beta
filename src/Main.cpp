
#include "Main.h"

#include "MeasureData.h"
#include "CtrlFrame/CtrlFrame.h"
#include "GraphFrame/GraphFrame.h"


IMPLEMENT_APP(MyApp)

wxBEGIN_EVENT_TABLE(GraphFrame, wxFrame)
EVT_MENU(wxID_EXIT, GraphFrame::OnExit)
wxEND_EVENT_TABLE()


bool MyApp::OnInit()
{
	// For sampling data
	MeasureData *bdata = new MeasureData();

    //CtrlFrame *ctrlframe = new CtrlFrame(NULL);
    //ctrlframe->SetTitle(wxT("Totto Dashboard"));
    //ctrlframe->Show(true);

	// Create the Graph frame window
	GraphFrame *gframe = new GraphFrame(NULL, bdata);
	gframe->SetTitle(wxT("Totto Dashboard"));
	gframe->Show(true);

	return true;
}
