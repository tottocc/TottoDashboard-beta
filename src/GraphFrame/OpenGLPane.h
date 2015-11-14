#ifndef __OPENGLPANE__
#define __OPENGLPANE__

#include <wx/glcanvas.h>
#include "../MeasureData.h"

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


#define PLOTOBJECT_NUM_MAX 10

class PlotObject
{
public:
	PlotObject(MeasureData* dat);

	bool IsDataUpdate();

	void SetPlotAreaX(float l, float r);
	bool MovePlotAreaX(float x);   // Return true if right value of plot area reaches xmax
	void UpatePlotArea();
	void AutoScale();

	float GetLeft();
	float GetRight();
	float GetBottom();
	float GetTop();

	void DrawGraph(int width, int height);
	void DrawHorizontalScrollbar();
	void DrawAxis(int width, int height);
	void DrawXname(float scale);
	void DrawYname(float scale);
	
private:
	MeasureData *plotdat;
	int previousNum;

	float left;
	float right;
	float bottom;
	float top;

	bool showAllPlotAreaFlag;
};


class OpenGLPane : public wxGLCanvas
{
	wxGLContext*	m_context;

public:
	OpenGLPane(wxFrame* parent, int* args);
	virtual ~OpenGLPane();

	bool AddPlotData(MeasureData *dat);

	void resized(wxSizeEvent& evt);

	int GetGraphWidth();	
	int GetGraphHeight();
	void UpdateCanvasWidth();
	void UpdateCanvasHeight();
	bool IsGraphArea(int x, int y);
	bool IsXAxisArea(int x, int y);
	bool IsRightSpace(int x);

	// draw
	void Prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void render(wxPaintEvent& evt);

	// other events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	void OnIdle(wxIdleEvent& event);
	DECLARE_EVENT_TABLE();

private:
	PlotObject *plotobj;

	int canvasWidth;
	int canvasHeight;

	int leftSpaceWidth;
	int rightSpaceWidth;
	int topSpaceHeight;
	int bottomSpaceHeight;
};


#endif // __OPENGLPANE__
