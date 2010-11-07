#ifndef __ps_custom_build_page__
#define __ps_custom_build_page__

/**
@file
Subclass of PSCustomBuildBasePage, which is generated by wxFormBuilder.
*/

#include "project_settings_base_dlg.h"
#include "project_settings_dlg.h"

//// end generated include
class ProjectSettingsDlg;

/** Implementing PSCustomBuildBasePage */
class PSCustomBuildPage : public PSCustomBuildBasePage, public IProjectSettingsPage
{
	wxString            m_projectName;
	long                m_selecteCustomTaregt;
	ProjectSettingsDlg *m_dlg;

protected:
	// Handlers for PSCustomBuildBasePage events.
	void OnCustomBuildEnabled( wxCommandEvent& event );
	void OnCustomBuildEnabledUI( wxUpdateUIEvent& event );
	void OnCmdEvtVModified( wxCommandEvent& event );
	void OnBrowseCustomBuildWD( wxCommandEvent& event );
	void OnItemActivated( wxListEvent& event );
	void OnItemSelected( wxListEvent& event );
	void OnNewTarget( wxCommandEvent& event );
	void OnEditTarget( wxCommandEvent& event );
	void OnEditTargetUI( wxUpdateUIEvent& event );
	void OnDeleteTarget( wxCommandEvent& event );
	void OnDeleteTargetUI( wxUpdateUIEvent& event );
	void OnChoiceMakefileTool( wxCommandEvent& event );
	void OnCustomBuildCBEnabledUI(wxUpdateUIEvent& event);

public:
	/** Constructor */
	PSCustomBuildPage( wxWindow* parent, const wxString &projectName, ProjectSettingsDlg *dlg );
	//// end generated class members

	virtual void Load(BuildConfigPtr buildConf);
	virtual void Save(BuildConfigPtr buildConf, ProjectSettingsPtr projSettingsPtr);
	virtual void Clear();

protected:
	void                DoEditTarget(long item);
	void                DoUpdateTarget(long item, const wxString &target, const wxString &cmd);
	wxString            GetTargetCommand(const wxString& target);
	ProjectSettingsDlg* GetDlg();
};

#endif // __ps_custom_build_page__