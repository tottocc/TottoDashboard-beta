
#include "Main.h"
#include "MyMainFrame.h"


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	MyMainFrame *simple = new MyMainFrame(NULL);
	simple->Show(true);

	return true;
}



