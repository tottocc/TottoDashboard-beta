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
	this->SetSizeHints( wxSize( 336,155 ), wxSize( 336,155 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 3, 0, 0 );
	
	m_comboBoxCom = new wxComboBox( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0, NULL, wxCB_READONLY ); 
	gSizer1->Add( m_comboBoxCom, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_buttonCom = new wxButton( m_panel1, wxID_ANY, wxT("Open"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	gSizer1->Add( m_buttonCom, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_panel1->SetSizer( gSizer1 );
	m_panel1->Layout();
	gSizer1->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxALL|wxEXPAND, 0 );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 3, 0, 0 );
	
	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, wxT("Port 1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	gSizer2->Add( m_staticText1, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("Port 2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gSizer2->Add( m_staticText2, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText3 = new wxStaticText( m_panel2, wxID_ANY, wxT("Port 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	gSizer2->Add( m_staticText3, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	m_panel2->SetSizer( gSizer2 );
	m_panel2->Layout();
	gSizer2->Fit( m_panel2 );
	bSizer1->Add( m_panel2, 1, wxALL|wxEXPAND, 0 );
	
	m_panel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 0, 3, 0, 0 );
	
	m_button1 = new wxButton( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_button1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_button1->Enable( false );
	
	gSizer3->Add( m_button1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP|wxALL, 1 );
	
	m_button2 = new wxButton( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_button2->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_button2->Enable( false );
	
	gSizer3->Add( m_button2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP|wxALL, 1 );
	
	m_button3 = new wxButton( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_button3->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_button3->Enable( false );
	
	gSizer3->Add( m_button3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1 );
	
	
	m_panel3->SetSizer( gSizer3 );
	m_panel3->Layout();
	gSizer3->Fit( m_panel3 );
	bSizer1->Add( m_panel3, 1, wxALL|wxEXPAND, 0 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonCom->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnOpenComClick ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn1Click ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn2Click ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn3Click ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	m_buttonCom->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnOpenComClick ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn1Click ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn2Click ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btn3Click ), NULL, this );
	
}
