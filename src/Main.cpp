
#include "Main.h"

#include "MeasureData.h"
#include "CtrlFrame/CtrlFrame.h"

IMPLEMENT_APP(MyApp)

// ----------------------------------------------------------------------------
// Application
// ----------------------------------------------------------------------------
bool MyApp::OnInit()
{
	CtrlFrame *ctrlframe = new CtrlFrame(NULL);
	ctrlframe->SetTitle(wxT("Totto Dashboard"));
    ctrlframe->Show(true);

	return true;
}
