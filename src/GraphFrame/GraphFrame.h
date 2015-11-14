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

		void OnExit(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();

	private:
		MeasureData *bdata;
};


#endif // __GRAPHFRAME__
