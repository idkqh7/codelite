#include "AddSSHAcountDlg.h"
#include "cl_ssh.h"
#include "cl_exception.h"
#include <wx/msgdlg.h>
#include "windowattrmanager.h"

AddSSHAcountDlg::AddSSHAcountDlg(wxWindow* parent)
    : AddSSHAcountDlgBase(parent)
{
    WindowAttrManager::Load(this, "AddSSHAcountDlg", NULL);
}

AddSSHAcountDlg::AddSSHAcountDlg(wxWindow* parent, const SSHAccountInfo& account)
    : AddSSHAcountDlgBase(parent)
{
    m_textCtrlHost->ChangeValue( account.GetHost() );
    m_textCtrlPassword->ChangeValue( account.GetPassword() );
    m_textCtrlPort->ChangeValue( wxString() << account.GetPort() );
    m_textCtrlUsername->ChangeValue( account.GetUsername() );
    
    WindowAttrManager::Load(this, "AddSSHAcountDlg", NULL);
}

AddSSHAcountDlg::~AddSSHAcountDlg()
{
    WindowAttrManager::Save(this, "AddSSHAcountDlg", NULL);
}

void AddSSHAcountDlg::OnOKUI(wxUpdateUIEvent& event)
{
    event.Enable( !m_textCtrlHost->IsEmpty() && !m_textCtrlPassword->IsEmpty() && !m_textCtrlPort->IsEmpty() && !m_textCtrlUsername->IsEmpty());
}

void AddSSHAcountDlg::GetAccountInfo(SSHAccountInfo& info)
{
    info.SetHost( m_textCtrlHost->GetValue() );
    info.SetPassword( m_textCtrlPassword->GetValue() );

    long port = 22;
    m_textCtrlPort->GetValue().ToCLong( &port );
    info.SetPort( port );
    info.SetUsername( m_textCtrlUsername->GetValue() );
}

void AddSSHAcountDlg::OnTestConnection(wxCommandEvent& event)
{
    SSHAccountInfo account;
    GetAccountInfo(account);
    clSSH ssh(account.GetHost(), account.GetUsername(), account.GetPassword(), account.GetPort());

    try {
        wxString message;
        ssh.Connect();
        if ( !ssh.AuthenticateServer( message ) ) {
            if ( ::wxMessageBox(message, "SSH", wxYES_NO|wxCENTER|wxICON_QUESTION, this) == wxYES ) {
                ssh.AcceptServerAuthentication();
            }
        }
        ssh.Login();
        ::wxMessageBox("Successfully connected to host!");

    } catch (clException &e) {
        ::wxMessageBox(e.What(), "SSH", wxICON_WARNING|wxOK, this);
    }
}
void AddSSHAcountDlg::OnTestConnectionUI(wxUpdateUIEvent& event)
{
    event.Enable( !m_textCtrlHost->IsEmpty() && !m_textCtrlPort->IsEmpty() );
}
