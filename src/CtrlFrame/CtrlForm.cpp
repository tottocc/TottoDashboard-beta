///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CtrlForm.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 350,250 ), wxSize( 350,600 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,40 ), wxTAB_TRAVERSAL );
	m_panel1->SetMinSize( wxSize( -1,40 ) );
	m_panel1->SetMaxSize( wxSize( -1,40 ) );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_buttonStartStop = new wxButton( m_panel1, wxID_ANY, wxT("Start"), wxPoint( -1,-1 ), wxSize( 80,-1 ), 0 );
	m_buttonStartStop->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	gSizer1->Add( m_buttonStartStop, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	m_buttonSingle = new wxButton( m_panel1, wxID_ANY, wxT("Single"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_buttonSingle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_buttonSingle->Enable( false );
	
	gSizer1->Add( m_buttonSingle, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_TOP|wxALL, 1 );
	
	
	m_panel1->SetSizer( gSizer1 );
	m_panel1->Layout();
	bSizer1->Add( m_panel1, 1, wxALL|wxEXPAND, 0 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_staticline1->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 0 );
	
	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_panel2 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_comboBoxCom = new wxComboBox( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 220,-1 ), 0, NULL, wxCB_READONLY ); 
	fgSizer1->Add( m_comboBoxCom, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	m_buttonCom = new wxButton( m_panel2, wxID_ANY, wxT("Open"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	fgSizer1->Add( m_buttonCom, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_panel2->SetSizer( fgSizer1 );
	m_panel2->Layout();
	fgSizer1->Fit( m_panel2 );
	bSizer2->Add( m_panel2, 1, wxEXPAND | wxALL, 5 );
	
	
	m_scrolledWindow1->SetSizer( bSizer2 );
	m_scrolledWindow1->Layout();
	bSizer2->Fit( m_scrolledWindow1 );
	bSizer1->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 0 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonStartStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnStartStopClick ), NULL, this );
	m_buttonSingle->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnSingleClick ), NULL, this );
	m_buttonCom->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnOpenComClick ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	m_buttonStartStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnStartStopClick ), NULL, this );
	m_buttonSingle->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnSingleClick ), NULL, this );
	m_buttonCom->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::btnOpenComClick ), NULL, this );
	
}
