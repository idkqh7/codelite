#include "ps_custom_makefile_rules_page.h"

PSCustomMakefileRulesPage::PSCustomMakefileRulesPage( wxWindow* parent, ProjectSettingsDlg *dlg )
    : PSCustomMakefileBasePage( parent )
    , m_dlg(dlg)
{
}

void PSCustomMakefileRulesPage::OnProjectCustumBuildUI( wxUpdateUIEvent& event )
{
    event.Enable( !m_dlg->IsCustomBuildEnabled() );
}

void PSCustomMakefileRulesPage::OnCmdEvtVModified( wxCommandEvent& event )
{
    wxUnusedVar(event);
    m_dlg->SetIsDirty(true);
}

void PSCustomMakefileRulesPage::Load(BuildConfigPtr buildConf)
{
    //set the custom pre-prebuild step
    wxString customPreBuild = buildConf->GetPreBuildCustom();

    //extract the dependencies
    wxString deps, rules;
    deps = customPreBuild.BeforeFirst(wxT('\n'));
    rules = customPreBuild.AfterFirst(wxT('\n'));

    rules = rules.Trim();
    rules = rules.Trim(false);

    deps = deps.Trim();
    deps = deps.Trim(false);

    m_textDeps->SetValue(deps);
    m_textPreBuildRule->SetValue(rules);
}

void PSCustomMakefileRulesPage::Save(BuildConfigPtr buildConf, ProjectSettingsPtr projSettingsPtr)
{
    //set the pre-build step
    wxString rules = m_textPreBuildRule->GetValue();
    wxString deps = m_textDeps->GetValue();

    rules = rules.Trim();
    rules = rules.Trim(false);
    deps = deps.Trim();
    deps = deps.Trim(false);

    wxString prebuilstep;
    prebuilstep << deps << wxT("\n");
    prebuilstep << rules;
    prebuilstep << wxT("\n");

    // Set the content only if there is real content to add
    wxString tmpPreBuildStep(prebuilstep);
    tmpPreBuildStep.Trim().Trim(false);
    if(tmpPreBuildStep.IsEmpty() == false) {
        buildConf->SetPreBuildCustom(prebuilstep);
    } else {
        buildConf->SetPreBuildCustom(wxT(""));
    }
}

void PSCustomMakefileRulesPage::Clear()
{
    m_textPreBuildRule->Clear();
    m_textDeps->Clear();
}
void PSCustomMakefileRulesPage::OnProjectEnabledUI(wxUpdateUIEvent& event)
{
    event.Enable( m_dlg->IsProjectEnabled() );
}
