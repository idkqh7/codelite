///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "newtoolbase.h"

///////////////////////////////////////////////////////////////////////////

NewToolBase::NewToolBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	bSizer1->SetMinSize( wxSize( 700,-1 ) ); 
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("ID:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_choiceIdChoices[] = { wxT("external_tool_0"), wxT("external_tool_1"), wxT("external_tool_2"), wxT("external_tool_3"), wxT("external_tool_4"), wxT("external_tool_5"), wxT("external_tool_6"), wxT("external_tool_7"), wxT("external_tool_8"), wxT("external_tool_9") };
	int m_choiceIdNChoices = sizeof( m_choiceIdChoices ) / sizeof( wxString );
	m_choiceId = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceIdNChoices, m_choiceIdChoices, 0 );
	m_choiceId->SetSelection( 0 );
	fgSizer1->Add( m_choiceId, 0, wxALL|wxEXPAND, 5 );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer1->Add( m_panel2, 1, wxEXPAND | wxALL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Tool path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlPath, 0, wxALL|wxEXPAND, 5 );
	
	m_buttonBrowsePath = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonBrowsePath->SetDefault(); 
	fgSizer1->Add( m_buttonBrowsePath, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Working directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlWd = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlWd, 1, wxALL|wxEXPAND, 5 );
	
	m_buttonBrowseWd = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_buttonBrowseWd, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Arguments:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlArguments = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlArguments, 1, wxALL|wxEXPAND, 5 );
	
	m_buttonHelp = new wxButton( this, wxID_ANY, wxT("Help..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_buttonHelp, 0, wxALL, 5 );
	
	bSizer1->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOk->SetDefault(); 
	bSizer2->Add( m_buttonOk, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonCancel, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_choiceId->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( NewToolBase::OnIdSelected ), NULL, this );
	m_buttonBrowsePath->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonBrowsePath ), NULL, this );
	m_buttonBrowseWd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonBrowseWD ), NULL, this );
	m_buttonHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonHelp ), NULL, this );
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonOk ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonCancel ), NULL, this );
}

NewToolBase::~NewToolBase()
{
	// Disconnect Events
	m_choiceId->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( NewToolBase::OnIdSelected ), NULL, this );
	m_buttonBrowsePath->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonBrowsePath ), NULL, this );
	m_buttonBrowseWd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonBrowseWD ), NULL, this );
	m_buttonHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonHelp ), NULL, this );
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonOk ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewToolBase::OnButtonCancel ), NULL, this );
}
