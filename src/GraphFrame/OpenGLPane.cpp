#include "OpenGLPane.h"

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "DrawText.h"


BEGIN_EVENT_TABLE(OpenGLPane, wxGLCanvas)
EVT_MOTION(OpenGLPane::mouseMoved)
EVT_LEFT_DOWN(OpenGLPane::mouseDown)
EVT_LEFT_UP(OpenGLPane::mouseReleased)
EVT_RIGHT_DOWN(OpenGLPane::rightClick)
EVT_LEAVE_WINDOW(OpenGLPane::mouseLeftWindow)
EVT_SIZE(OpenGLPane::resized)
EVT_KEY_DOWN(OpenGLPane::keyPressed)
EVT_KEY_UP(OpenGLPane::keyReleased)
EVT_MOUSEWHEEL(OpenGLPane::mouseWheelMoved)
EVT_PAINT(OpenGLPane::render)
END_EVENT_TABLE()


bool selectRegionFlag;
bool showHorizontalScrollbarFlag;
bool selectHorizontalScrollbarFlag;
int mouseDownPtX;
int mouseDownPtY;
int mouseCurrentPtX;
int mouseCurrentPtY;
int mouseReleasedPtX;
int mouseReleasedPtY;


//---------------------------------------------------------------------------
// OpenGLPane
//---------------------------------------------------------------------------

OpenGLPane::OpenGLPane(wxFrame* parent, int* args) :
wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);

	// Initialize parameters
	UpdateCanvasWidth();
	UpdateCanvasHeight();
	leftSpaceWidth = 80;
	rightSpaceWidth = 40;
	topSpaceHeight = 20;
	bottomSpaceHeight = 50;

	selectRegionFlag = false;
	mouseDownPtX = 0;
	mouseCurrentPtX = 0;
	mouseReleasedPtX = 0;

	// Initialize Draw Object
	plotobj = NULL;

	// Initialize font
	if (!InitFont())
		fprintf(stderr, "Fail to initialize font datar\n");

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

OpenGLPane::~OpenGLPane()
{
	delete m_context;
	if (plotobj)
		delete plotobj;
}

bool OpenGLPane::AddPlotData(MeasureData *dat)
{
	if (!plotobj) {
		plotobj = new PlotObject(dat);
		plotobj->AutoScale();
	}

	return true;
}


int OpenGLPane::GetGraphWidth()
{
	return canvasWidth - leftSpaceWidth - rightSpaceWidth;
}

int OpenGLPane::GetGraphHeight()
{
	return canvasHeight - bottomSpaceHeight - topSpaceHeight;
}

void OpenGLPane::resized(wxSizeEvent& evt)
{
	//wxGLCanvas::OnSize(evt);

	UpdateCanvasWidth();
	UpdateCanvasHeight();

	Refresh();
}

void OpenGLPane::UpdateCanvasWidth()
{
	canvasWidth = GetSize().x;
}

void OpenGLPane::UpdateCanvasHeight()
{
	canvasHeight = GetSize().y;
}

bool OpenGLPane::IsGraphArea(int x, int y)
{
	return x > leftSpaceWidth && x < canvasWidth - rightSpaceWidth && y > bottomSpaceHeight;
}

bool OpenGLPane::IsXAxisArea(int x, int y)
{
	return x > leftSpaceWidth && x < canvasWidth - rightSpaceWidth && y < bottomSpaceHeight;
}

//---------------------------------------------------------------------------
// OpenGLPane Draw Procedure
//---------------------------------------------------------------------------

// Initalize the OpenGL viewport for drawing in 2D
void OpenGLPane::Prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	glEnable(GL_TEXTURE_2D);   // textures
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(topleft_x, bottomrigth_x, topleft_y, bottomrigth_y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpenGLPane::DrawAxis()
{
	// white background for x-axis
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2d(0, bottomSpaceHeight);
	glVertex2d(0, 0);
	glVertex2d(canvasWidth, 0);
	glVertex2d(canvasWidth, bottomSpaceHeight);
	glEnd();

	// white background for y-axis
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2d(0, canvasHeight);
	glVertex2d(0, 0);
	glVertex2d(leftSpaceWidth, 0);
	glVertex2d(leftSpaceWidth, canvasHeight);
	glEnd();

	// Draw x-axis
	// x-axis
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2d(leftSpaceWidth, bottomSpaceHeight);
	glVertex2d(canvasWidth - rightSpaceWidth, bottomSpaceHeight);
	// x scale
	for (int i = 0; i <= 10; i++) {
		glVertex2d(GetGraphWidth() / 10.0 * i + leftSpaceWidth, bottomSpaceHeight - 5);
		glVertex2d(GetGraphWidth() / 10.0 * i + leftSpaceWidth, bottomSpaceHeight);
	}
	glEnd();

	// Draw y-axis
	// y-axis
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2d(leftSpaceWidth, canvasHeight - topSpaceHeight);
	glVertex2d(leftSpaceWidth, bottomSpaceHeight);
	// y scale
	for (int i = 0; i <= 10; i++) {
		glVertex2d(leftSpaceWidth, GetGraphHeight() / 10.0 * i + bottomSpaceHeight);
		glVertex2d(leftSpaceWidth + 5, GetGraphHeight() / 10.0 * i + bottomSpaceHeight);
	}
	glEnd();

	// Draw x-scale
	glColor3d(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(leftSpaceWidth, bottomSpaceHeight - 6, 0);
	DrawText(plotobj->GetLeft(), FONT_SCALE_SMALL, CENTER);
	glPopMatrix();

	glPushMatrix();
	glTranslated(canvasWidth - rightSpaceWidth, bottomSpaceHeight - 6, 0);
	DrawText(plotobj->GetRight(), FONT_SCALE_SMALL, CENTER);
	glPopMatrix();

	// Draw y-scale
	glColor3d(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(leftSpaceWidth - 6, bottomSpaceHeight + 6, 0);
	DrawText(plotobj->GetBottom(), FONT_SCALE_SMALL, RIGHT);
	glPopMatrix();

	glPushMatrix();
	glTranslated(leftSpaceWidth - 6, canvasHeight - topSpaceHeight + 6, 0);
	DrawText(plotobj->GetTop(), FONT_SCALE_SMALL, RIGHT);
	glPopMatrix();

	// Draw xy-axis name
	glPushMatrix();
	glTranslated(GetGraphWidth() / 2.0 + leftSpaceWidth, bottomSpaceHeight - 15, 0);
	plotobj->DrawXname(FONT_SCALE_MIDDLE);
	glPopMatrix();

	glPushMatrix();
	glTranslated(5, GetGraphHeight() / 2.0 + bottomSpaceHeight, 0);
	glRotatef(90, 0, 0, 1);
	plotobj->DrawYname(FONT_SCALE_MIDDLE);
	glPopMatrix();
}

void OpenGLPane::RenderText(char *text) {

}

void OpenGLPane::render(wxPaintEvent& evt)
{
	if (!IsShown()) return;

	wxGLCanvas::SetCurrent(*m_context);
	wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// ------------- draw some 2D ----------------
	Prepare2DViewport(0, 0, canvasWidth, canvasHeight);
	glLoadIdentity();

	// white background
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2d(0, 0);
	glVertex2d(canvasWidth, 0);
	glVertex2d(canvasWidth, canvasHeight);
	glVertex2d(0, canvasHeight);
	glEnd();

	// Draw plot data
	if (plotobj) {
		glLoadIdentity();
		glTranslated(leftSpaceWidth, bottomSpaceHeight, 0);
		glScalef(GetGraphWidth(), GetGraphHeight(), 1);
		plotobj->DrawGraph();
	}

	// Draw selected region
	glLoadIdentity();
	if (selectRegionFlag) {
		glColor4f(0.5, 0.5, 0.5, 0.5);
		glBegin(GL_QUADS);
		glVertex2d(mouseDownPtX, bottomSpaceHeight);
		glVertex2d(mouseReleasedPtX, bottomSpaceHeight);
		glVertex2d(mouseReleasedPtX, canvasHeight);
		glVertex2d(mouseDownPtX, canvasHeight);
		glEnd();
	}

	DrawAxis();

	// Draw horizontal scroolbar
	if (showHorizontalScrollbarFlag) {
		glLoadIdentity();
		glTranslated(leftSpaceWidth, bottomSpaceHeight, 0);
		glScalef(GetGraphWidth(), 1, 1);
		plotobj->DrawHorizontalScrollbar();
	}

	/*
	// Draw sample text
	string str = "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < 1; i++) {
		glColor3d(0.0, 0.0, 0.0);
		glLoadIdentity();
		glTranslated(leftSpaceWidth + 16, bottomSpaceHeight + 32 + i * 16, 0);
		DrawText(str, FONT_SCALE_SMALL - 0.01 * i, LEFT);
	}
	glLoadIdentity();
	glTranslated(10, 0, 0);
	glRotatef(90, 0, 0, 1);
	str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
	DrawText(str, FONT_SCALE_SMALL, LEFT);
	*/

	glFlush();
	SwapBuffers();
}


//---------------------------------------------------------------------------
// OpenGLPane Event Handler
//---------------------------------------------------------------------------

void OpenGLPane::mouseMoved(wxMouseEvent& event)
{
	const wxPoint pt = wxGetMousePosition();
	int mouseCurrentPtX = pt.x - this->GetScreenPosition().x;
	int mouseCurrentPtY = canvasHeight - (pt.y - this->GetScreenPosition().y);

	if (selectRegionFlag) {
		if (mouseReleasedPtX < leftSpaceWidth) {
			mouseReleasedPtX = leftSpaceWidth;
		}
		else if (mouseReleasedPtX > canvasWidth - rightSpaceWidth)
		{
			mouseReleasedPtX = canvasWidth - rightSpaceWidth;
		} else {
			mouseReleasedPtX = mouseCurrentPtX;
		}
		Refresh();
	}
	else if (selectHorizontalScrollbarFlag) {
		plotobj->MovePlotAreaX((float)(mouseCurrentPtX - mouseDownPtX) / (float)(GetGraphWidth()));
		mouseDownPtX = mouseCurrentPtX;
		Refresh();
	}
	else if (IsXAxisArea(mouseCurrentPtX, mouseCurrentPtY)) {
		showHorizontalScrollbarFlag = true;
		Refresh();
	}
	else {
		showHorizontalScrollbarFlag = false;
		Refresh();
	}

}

void OpenGLPane::mouseWheelMoved(wxMouseEvent& event) {}

void OpenGLPane::mouseDown(wxMouseEvent& event) {
	const wxPoint pt = wxGetMousePosition();
	mouseDownPtX = pt.x - this->GetScreenPosition().x;
	mouseDownPtY = canvasHeight - (pt.y - this->GetScreenPosition().y);

	if (IsGraphArea(mouseDownPtX, mouseDownPtY)) {
		mouseReleasedPtX = mouseDownPtX;
		selectRegionFlag = true;
		Refresh();
	}
	else if (IsXAxisArea(mouseDownPtX, mouseDownPtY)) {
		selectHorizontalScrollbarFlag = true;
		Refresh();
	}
}

void OpenGLPane::mouseReleased(wxMouseEvent& event)
{
	if (selectRegionFlag) {
		const wxPoint pt = wxGetMousePosition();
		mouseReleasedPtX = pt.x - this->GetScreenPosition().x;

		if (mouseReleasedPtX <= leftSpaceWidth) {
			mouseReleasedPtX = leftSpaceWidth;
		}
		else if (mouseReleasedPtX >= canvasWidth - rightSpaceWidth)
		{
			mouseReleasedPtX = canvasWidth - rightSpaceWidth;
		}
		
		if (mouseReleasedPtX < mouseDownPtX) {
			int temp = mouseReleasedPtX;
			mouseReleasedPtX = mouseDownPtX;
			mouseDownPtX = temp;
		}

		float l = (mouseDownPtX - leftSpaceWidth) / (float)(GetGraphWidth())
			* (plotobj->GetRight() - plotobj->GetLeft()) + plotobj->GetLeft();
		float r = (mouseReleasedPtX - leftSpaceWidth) / (float)(GetGraphWidth())
			* (plotobj->GetRight() - plotobj->GetLeft()) + plotobj->GetLeft();
		plotobj->SetPlotAreaX(l, r);

		selectRegionFlag = false;
		Refresh();
	}
	else if (selectHorizontalScrollbarFlag) {
		selectHorizontalScrollbarFlag = false;
		Refresh();
	}
}

void OpenGLPane::rightClick(wxMouseEvent& event)
{
	plotobj->AutoScale();
	Refresh();
}

void OpenGLPane::mouseLeftWindow(wxMouseEvent& event) 
{
	showHorizontalScrollbarFlag = false;
	selectHorizontalScrollbarFlag = false;
	Refresh();
}

void OpenGLPane::keyPressed(wxKeyEvent& event) {}
void OpenGLPane::keyReleased(wxKeyEvent& event) {}


//---------------------------------------------------------------------------
// PlotObject
//---------------------------------------------------------------------------

PlotObject::PlotObject(MeasureData* dat) {
	plotdat = dat;
}

void PlotObject::SetPlotAreaX(float l, float r)
{
	int plotnum = (r - l) / plotdat->xdelta;
	if (plotnum > 10) {
		left = l;
		right = r;
	}
}

void PlotObject::MovePlotAreaX(float x)
{
	float move = (plotdat->xmax - plotdat->xmin) * x;

	if (move < 0) {
		if (left + move < plotdat->xmin) {
			right = right + plotdat->xmin - left;
			left = plotdat->xmin;
		}
		else {
			left = left + move;
			right = right + move;
		}
	}
	else {
		if (right + move > plotdat->xmax) {
			left = left + plotdat->xmax - right;
			right = plotdat->xmax;
		}
		else {
			right = right + move;
			left = left + move;
		}
	}
}

void PlotObject::AutoScale()
{
	left = 0.0;
	if (plotdat->num > 1)
		right = plotdat->xdelta * (plotdat->num - 1);
	else
		right = plotdat->xdelta;
	bottom = 0.0;
	top = 0.0;
	for (int i = 0; i < plotdat->num; i++) {
		if (top < plotdat->ydat[i])
			top = plotdat->ydat[i];
	}
}

float PlotObject::GetLeft()
{
	return left;
}

float PlotObject::GetRight()
{
	return right;
}

float PlotObject::GetBottom()
{
	return bottom;
}

float PlotObject::GetTop()
{
	return top;
}

void PlotObject::DrawGraph()
{
	float w = right - left;
	float h = top - bottom;

	int i = (int)(left / plotdat->xdelta);
	int i_max = (int)(right / plotdat->xdelta);
	if (i_max > plotdat->num) i_max = plotdat->num;

	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	while (i < i_max) {
		glVertex2d((plotdat->xdelta * i - left) / w, (plotdat->ydat[i] - bottom) / h);
		i++;
	}
	glEnd();
}

void PlotObject::DrawHorizontalScrollbar()
{
	float w = plotdat->xmax - plotdat->xmin;
	glColor4d(0.5, 0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	if ((right - left) / w > 0.005) {
		glVertex2d(left / w, 0);
		glVertex2d(left / w, -15);
		glVertex2d(right / w, -15);
		glVertex2d(right / w, 0);
	}
	else {
		glVertex2d(left / w -0.001 , 0);
		glVertex2d(left / w -0.001, -15);
		glVertex2d(right / w + 0.001, -15);
		glVertex2d(right / w + 0.001, 0);
	}
	glEnd();
}

void PlotObject::DrawXname(float scale)
{
	int len = plotdat->xname.size();
	DrawText(plotdat->xname, scale, CENTER);
}

void PlotObject::DrawYname(float scale)
{
	int len = plotdat->yname.size();
	DrawText(plotdat->yname, scale, CENTER);
}





