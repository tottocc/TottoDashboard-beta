
#include "Main.h"
#include "MyMainFrame.h"


IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
	MyMainFrame *mainframe = new MyMainFrame(NULL);
	mainframe->SetTitle(wxT("USB Chopper"));
	mainframe->Show(true);

	return true;
}
