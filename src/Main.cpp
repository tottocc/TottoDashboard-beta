
#include "Main.h"
#include "MyMainFrame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	MyMainFrame *mainframe = new MyMainFrame(NULL);
	mainframe->SetTitle(wxT("USB Chopper"));
	mainframe->SetIcon(wxICON(tottoIcon));
	mainframe->Show(true);

	return true;
}



