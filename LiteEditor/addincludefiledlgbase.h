//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: AddIncludeFile.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef ADDINCLUDEFILE_BASE_CLASSES_H
#define ADDINCLUDEFILE_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/pen.h>
#include <wx/aui/auibar.h>
#include <wx/toolbar.h>
#include <wx/stc/stc.h>
#include <wx/button.h>

class AddIncludeFileDlgBase : public wxDialog
{
protected:
    enum {
        ID_LINEDOWN = 1001,
        ID_LINEUP = 1002,
    };
protected:
    wxPanel* m_mainPanel;
    wxStaticText* m_staticText4;
    wxTextCtrl* m_textCtrlLineToAdd;
    wxStaticText* m_staticText5;
    wxTextCtrl* m_textCtrlFullPath;
    wxStaticText* m_staticText14;
    wxAuiToolBar* m_auibar6;
    wxStyledTextCtrl* m_textCtrlPreview;
    wxButton* m_buttonOK;
    wxButton* m_buttonCancel;

protected:
    virtual void OnTextUpdated(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonUp(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonDown(wxCommandEvent& event) { event.Skip(); }
    virtual void OnClearCachedPaths(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonOK(wxCommandEvent& event) { event.Skip(); }

public:
    AddIncludeFileDlgBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add a missing header file"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~AddIncludeFileDlgBase();
};

#endif
