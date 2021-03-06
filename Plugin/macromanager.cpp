#include "macromanager.h"
#include "environmentconfig.h"
#include "build_config.h"
#include "project.h"
#include "workspace.h"
#include "imanager.h"
#include <wx/regex.h>

MacroManager::MacroManager()
{
}

MacroManager::~MacroManager()
{
}

MacroManager* MacroManager::Instance()
{
    static MacroManager ms_instance;
    return &ms_instance;
}

wxString MacroManager::Expand(const wxString& expression, IManager* manager, const wxString& project, const wxString &confToBuild)
{
    wxString   errMsg;
    wxString   expandedString(expression);
    Workspace *workspace = WorkspaceST::Get();
    
    DollarEscaper de(expandedString);
    
    if ( workspace ) {
        expandedString.Replace(wxT("$(WorkspaceName)"), workspace->GetName());
        ProjectPtr proj = workspace->FindProjectByName(project, errMsg);
        if (proj) {
            wxString prjBuildWd;
            wxString prjRunWd;

            wxString project_name(proj->GetName());

            //make sure that the project name does not contain any spaces
            project_name.Replace(wxT(" "), wxT("_"));

            BuildConfigPtr bldConf = workspace->GetProjBuildConf(proj->GetName(), confToBuild);
            if (bldConf) {
                bool isCustom = bldConf->IsCustomBuild();
                expandedString.Replace(wxT("$(ProjectOutputFile)"), bldConf->GetOutputFileName());

                // When custom build project, use the working directory set in the
                // custom build tab, otherwise use the project file's path
                prjBuildWd = isCustom ? bldConf->GetCustomBuildWorkingDir() : proj->GetFileName().GetPath();
                prjRunWd   = bldConf->GetWorkingDirectory();
            }

            expandedString.Replace(wxT("$(ProjectWorkingDirectory)"),    prjBuildWd);
            expandedString.Replace(wxT("$(ProjectRunWorkingDirectory)"), prjRunWd);
            expandedString.Replace(wxT("$(ProjectPath)"), proj->GetFileName().GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR));
            expandedString.Replace(wxT("$(WorkspacePath)"), workspace->GetWorkspaceFileName().GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR));
            expandedString.Replace(wxT("$(ProjectName)"), project_name);

            if (bldConf) {
                expandedString.Replace(wxT("$(IntermediateDirectory)"), bldConf->GetIntermediateDirectory());
                expandedString.Replace(wxT("$(ConfigurationName)"), bldConf->GetName());
                expandedString.Replace(wxT("$(OutDir)"), bldConf->GetIntermediateDirectory());
            }

            if(expandedString.Find(wxT("$(ProjectFiles)")) != wxNOT_FOUND)
                expandedString.Replace(wxT("$(ProjectFiles)"),   proj->GetFiles());

            if(expandedString.Find(wxT("$(ProjectFilesAbs)")) != wxNOT_FOUND)
                expandedString.Replace(wxT("$(ProjectFilesAbs)"),proj->GetFiles(true));


        }
    }
    
    if ( manager ) {
        IEditor *editor = manager->GetActiveEditor();
        if (editor) {
            wxFileName fn(editor->GetFileName());

            expandedString.Replace(wxT("$(CurrentFileName)"), fn.GetName());

            wxString fpath(fn.GetPath());
            fpath.Replace(wxT("\\"), wxT("/"));
            expandedString.Replace(wxT("$(CurrentFilePath)"), fpath);
            expandedString.Replace(wxT("$(CurrentFileExt)"), fn.GetExt());

            wxString ffullpath(fn.GetFullPath());
            ffullpath.Replace(wxT("\\"), wxT("/"));
            expandedString.Replace(wxT("$(CurrentFileFullPath)"), ffullpath);
            expandedString.Replace(wxT("$(CurrentSelection)"), editor->GetSelection());
            if(expandedString.Find(wxT("$(CurrentSelectionRange)")) != wxNOT_FOUND) {
                int start=editor->GetSelectionStart(),
                    end  =editor->GetSelectionEnd();

                wxString output=wxString::Format(wxT("%i:%i"),start,end);
                expandedString.Replace(wxT("$(CurrentSelectionRange)"), output);
            }
        }
        
    }

    //exapand common macros
    wxDateTime now = wxDateTime::Now();
    expandedString.Replace(wxT("$(User)"), wxGetUserName());
    expandedString.Replace(wxT("$(Date)"), now.FormatDate());

    if (manager) {
        expandedString.Replace(wxT("$(CodeLitePath)"), manager->GetInstallDirectory());
        expandedString = manager->GetEnv()->ExpandVariables(expandedString, true);
    }
    return expandedString;
}

wxString MacroManager::Replace(const wxString& inString, const wxString& variableName, const wxString& replaceWith, bool bIgnoreCase)
{
    size_t flags = wxRE_DEFAULT;
    if ( bIgnoreCase ) flags |= wxRE_ICASE;

    wxString strRe1;
    wxString strRe2;
    wxString strRe3;
    wxString strRe4;

    strRe1 << wxT("\\$\\((") << variableName << wxT(")\\)");
    strRe2 << wxT("\\$\\{(") << variableName << wxT(")\\}");
    strRe3 << wxT("\\$(")    << variableName << wxT(")");
    strRe4 << wxT("%(")   << variableName << wxT(")%");

    wxRegEx reOne  (strRe1, flags); // $(variable)
    wxRegEx reTwo  (strRe2, flags); // ${variable}
    wxRegEx reThree(strRe3, flags); // $variable
    wxRegEx reFour (strRe4, flags); // %variable%

    wxString result = inString;
    if ( reOne.Matches(result) ) {
        reOne.ReplaceAll(&result, replaceWith);
    }

    if ( reTwo.Matches(result) ) {
        reTwo.ReplaceAll(&result, replaceWith);
    }

    if ( reThree.Matches(result) ) {
        reThree.ReplaceAll(&result, replaceWith);
    }

    if ( reFour.Matches(result) ) {
        reFour.ReplaceAll(&result, replaceWith);
    }
    return result;
}

bool MacroManager::FindVariable(const wxString& inString, wxString& name, wxString& fullname)
{
    size_t flags = wxRE_DEFAULT | wxRE_ICASE;

    wxString strRe1;
    wxString strRe2;
    wxString strRe3;
    wxString strRe4;

    strRe1 << wxT("\\$\\((") << wxT("[a-z_0-9]+") << wxT(")\\)");
    strRe2 << wxT("\\$\\{(") << wxT("[a-z_0-9]+") << wxT(")\\}");
    strRe3 << wxT("\\$(")    << wxT("[a-z_0-9]+") << wxT(")");
    strRe4 << wxT("%(")   << wxT("[a-z_0-9]+") << wxT(")%");

    static wxRegEx reOne  (strRe1, flags); // $(variable)
    static wxRegEx reTwo  (strRe2, flags); // ${variable}
    static wxRegEx reThree(strRe3, flags); // $variable
    static wxRegEx reFour (strRe4, flags); // %variable%

    if ( reOne.Matches(inString) ) {
        name = reOne.GetMatch(inString, 1);
        fullname = reOne.GetMatch(inString);
        return true;
    }

    if ( reTwo.Matches(inString) ) {
        name = reTwo.GetMatch(inString, 1);
        fullname = reTwo.GetMatch(inString);
        return true;
    }

    if ( reThree.Matches(inString) ) {
        name = reThree.GetMatch(inString, 1);
        fullname = reThree.GetMatch(inString);
        return true;
    }

    if ( reFour.Matches(inString) ) {
        name = reFour.GetMatch(inString, 1);
        fullname = reFour.GetMatch(inString);
        return true;
    }
    return false;
}
