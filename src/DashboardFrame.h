#ifndef __DASHBOARDFRAME_H__
#define __DASHBOARDFRAME_H__

#include "wxForm.h"
#include "wx/glcanvas.h"


// we need OpenGL headers for GLfloat/GLint types used below
#if defined(__WXMAC__) || defined(__WXCOCOA__)
#   ifdef __DARWIN__
#       include <OpenGL/gl.h>
#       include <OpenGL/glu.h>
#   else
#       include <gl.h>
#       include <glu.h>
#   endif
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

// The frame containing the GL canvas
class DashboardFrame : public wxFrame
{
public:
	DashboardFrame(wxFrame *frame,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE);

	virtual ~DashboardFrame();

private:
	void OnExit(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};


class BasicGLPane : public wxGLCanvas
{
	wxGLContext*	m_context;

public:
	BasicGLPane(wxFrame* parent, int* args);
	virtual ~BasicGLPane();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	DECLARE_EVENT_TABLE()
};


#endif // __DASHBOARDFRAME_H__
