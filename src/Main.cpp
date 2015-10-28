
#include "Main.h"
#include "MyMainFrame.h"
#include "DashboardFrame.h"



IMPLEMENT_APP(MyApp)

wxBEGIN_EVENT_TABLE(DashboardFrame, wxFrame)
EVT_MENU(wxID_EXIT, DashboardFrame::OnExit)
wxEND_EVENT_TABLE()

bool MyApp::OnInit()
{
    /*
    MyMainFrame *mainframe = new MyMainFrame(NULL);
    mainframe->SetTitle(wxT("Totto Dashboard"));
    mainframe->Show(true);
    */

	// Create the dashboard frame window

	DashboardFrame *dbdframe = new DashboardFrame(NULL, wxT("Totto Dashboard"));

	return true;
}
