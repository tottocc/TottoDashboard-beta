///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxForm.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 293,186 ), wxSize( 293,186 ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 3, 0, 0 );

	m_comboBoxCom = new wxComboBox( m_panel1, wxID_ANY, wxT("No COM Port"), wxPoint( -1,-1 ), wxSize( 170,-1 ), 0, NULL, 0 );
	m_comboBoxCom->SetMaxSize( wxSize( 170,-1 ) );

	gSizer1->Add( m_comboBoxCom, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonCom = new wxButton( m_panel1, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_buttonCom, 0, wxALIGN_CENTER|wxALL, 5 );


	m_panel1->SetSizer( gSizer1 );
	m_panel1->Layout();
	gSizer1->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxALL|wxEXPAND, 0 );

	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 3, 0, 0 );

	m_button1 = new wxButton( m_panel2, wxID_ANY, wxT("Port 1"), wxDefaultPosition, wxSize( 80,80 ), 0 );
	m_button1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_button1->Enable( false );

	gSizer2->Add( m_button1, 0, wxALIGN_CENTER|wxALL, 5 );

	m_button2 = new wxButton( m_panel2, wxID_ANY, wxT("Port 2"), wxDefaultPosition, wxSize( 80,80 ), 0 );
	m_button2->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_button2->Enable( false );

	gSizer2->Add( m_button2, 0, wxALIGN_CENTER|wxALL, 5 );

	m_button3 = new wxButton( m_panel2, wxID_ANY, wxT("Port 3"), wxDefaultPosition, wxSize( 80,80 ), 0 );
	m_button3->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_button3->Enable( false );

	gSizer2->Add( m_button3, 0, wxALIGN_CENTER|wxALL, 5 );


	m_panel2->SetSizer( gSizer2 );
	m_panel2->Layout();
	gSizer2->Fit( m_panel2 );
	bSizer1->Add( m_panel2, 1, wxALL|wxEXPAND, 0 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_comboBoxCom->Connect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(MainFrame::comboBoxComDropDown), NULL, this);
	m_buttonCom->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnOpenComClick ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn1Click ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn2Click ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn3Click ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	m_comboBoxCom->Disconnect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(MainFrame::comboBoxComDropDown), NULL, this);
	m_buttonCom->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnOpenComClick ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn1Click ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn2Click ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn3Click ), NULL, this );

}
