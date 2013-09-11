#include "SSHAccountManagerDlg.h"
#include "AddSSHAcountDlg.h"
#include "ssh_account_info.h"
#include "sftp_settings.h"
#include "windowattrmanager.h"

SSHAccountManagerDlg::SSHAccountManagerDlg(wxWindow* parent)
    : SSHAccountManagerDlgBase(parent)
{
    SFTPSettings settings;
    SFTPSettings::Load( settings );
    
    const SSHAccountInfo::List_t& accounts = settings.GetAccounts();
    SSHAccountInfo::List_t::const_iterator iter = accounts.begin();
    for(; iter != accounts.end(); ++iter) {
        DoAddAccount( *iter );
    }
    WindowAttrManager::Load(this, "SSHAccountManagerDlg", NULL);
}

SSHAccountManagerDlg::~SSHAccountManagerDlg()
{
    for(int i=0; i<m_dvListCtrlAccounts->GetItemCount(); ++i) {
        wxDataViewItem item = m_dvListCtrlAccounts->RowToItem(i);
        SSHAccountInfo* pAccount = (SSHAccountInfo*)(m_dvListCtrlAccounts->GetItemData( item ));
        delete pAccount;
        m_dvListCtrlAccounts->SetItemData(item, (wxUIntPtr)NULL);
    }
    m_dvListCtrlAccounts->DeleteAllItems();
    WindowAttrManager::Save(this, "SSHAccountManagerDlg", NULL);
}

void SSHAccountManagerDlg::OnAddAccount(wxCommandEvent& event)
{
    AddSSHAcountDlg dlg(this);
    if ( dlg.ShowModal() == wxID_OK ) {
        SSHAccountInfo account;
        dlg.GetAccountInfo( account );
        
        DoAddAccount( account );
    }
}

void SSHAccountManagerDlg::OnDeleteAccount(wxCommandEvent& event)
{
}

void SSHAccountManagerDlg::OnEditAccount(wxDataViewEvent& event)
{
    SSHAccountInfo *account = (SSHAccountInfo*)(m_dvListCtrlAccounts->GetItemData( event.GetItem() ));
    AddSSHAcountDlg dlg(this, *account);
    if ( dlg.ShowModal() == wxID_OK ) {
        // update the user info
        dlg.GetAccountInfo(*account);
        
        // update the UI
        m_dvListCtrlAccounts->GetStore()->SetValue(account->GetHost(), event.GetItem(), 0);
        m_dvListCtrlAccounts->GetStore()->SetValue(account->GetUsername(), event.GetItem(), 1);
    }
}

void SSHAccountManagerDlg::DoAddAccount(const SSHAccountInfo& account)
{
    wxVector<wxVariant> cols;
    cols.push_back( account.GetHost() );
    cols.push_back( account.GetUsername() );
    m_dvListCtrlAccounts->AppendItem( cols, (wxUIntPtr)(new SSHAccountInfo(account)));
}

SSHAccountInfo::List_t SSHAccountManagerDlg::GetAccounts() const
{
    SSHAccountInfo::List_t accounts;
    for(int i=0; i<m_dvListCtrlAccounts->GetItemCount(); ++i) {
        accounts.push_back( *(SSHAccountInfo*)(m_dvListCtrlAccounts->GetItemData( m_dvListCtrlAccounts->RowToItem(i))) );
    }
    return accounts;
}
