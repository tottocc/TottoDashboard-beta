///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CTRLFORM_H__
#define __CTRLFORM_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/combobox.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxButton* m_buttonStartStop;
		wxButton* m_buttonSingle;
		wxStaticLine* m_staticline1;
		wxScrolledWindow* m_scrolledWindow1;
		wxPanel* m_panel2;
		wxComboBox* m_comboBoxCom;
		wxButton* m_buttonCom;
		
		// Virtual event handlers, overide them in your derived class
		virtual void btnStartStopClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void btnSingleClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void btnOpenComClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 350,250 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //__CTRLFORM_H__
