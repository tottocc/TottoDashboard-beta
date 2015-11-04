#ifndef __GRAPHFRAME__
#define __GRAPHFRAME__

#include "GraphForm.h"
#include "OpenGLPane.h"

#include "../MeasureData.h"


/** Implementing GraphFrame */
class GraphFrame : public MyFrame
{
	public:
		/** Constructor */
		GraphFrame(wxWindow* parent, MeasureData *dat);
	//// end generated class members

		virtual ~GraphFrame();

	private:
		void OnExit(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();
		MeasureData *bdata;
};


#endif // __GRAPHFRAME__
