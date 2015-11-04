#include "GraphFrame.h"


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// the application icon for Linux and Mac-OSX
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../res/tottoIcon.xpm"
#endif

GraphFrame::GraphFrame(wxWindow* parent, MeasureData *dat)
:
MyFrame(parent)
{
	SetIcon(wxICON(tottoIcon));

	// Set Sampling Data
	bdata = dat;

	// Make a menubar
	wxMenu *fileMenu = new wxMenu;

	fileMenu->Append(wxID_EXIT, wxT("E&xit"));
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	SetMenuBar(menuBar);

	// Make a GraphGLPane

	// JACS
#ifdef __WXMSW__
	int *gl_attrib = NULL;
#else
	int gl_attrib[20] =
	{ WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
	WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
	WX_GL_DOUBLEBUFFER,
#  if defined(__WXMAC__) || defined(__WXCOCOA__)
	GL_NONE };
#  else
	None
};
#  endif
#endif

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

	OpenGLPane* oglPane = new OpenGLPane(this, args);
	sizer->Add(oglPane, 1, wxEXPAND);
	oglPane->AddPlotData(bdata);

	SetSizer(sizer);
	SetAutoLayout(true);
}

GraphFrame::~GraphFrame()
{
	//delete m_canvas;
}

// Intercept menu commands
void GraphFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}
