#include "workspacesvnsettings.h"
#include "cl_standard_paths.h"
#include "workspace.h"
#include "cl_config.h"

WorkspaceSvnSettings::WorkspaceSvnSettings()
    : clConfigItem("svn-settings")
{
}

WorkspaceSvnSettings::WorkspaceSvnSettings(const wxFileName& fn)
    : clConfigItem("svn-settings")
    , m_workspaceFileName(fn)
{
}

WorkspaceSvnSettings::~WorkspaceSvnSettings()
{
}

void WorkspaceSvnSettings::FromJSON(const JSONElement& json)
{
    m_repoPath = json.namedObject("m_repoPath").toString();
}

JSONElement WorkspaceSvnSettings::ToJSON() const
{
    JSONElement json = JSONElement::createObject(GetName());
    json.addProperty("m_repoPath", m_repoPath);
    return json;
}

WorkspaceSvnSettings& WorkspaceSvnSettings::Load()
{
    clConfig conf(GetLocalConfigFile().GetFullPath());
    conf.ReadItem( this );
    return *this;
}

void WorkspaceSvnSettings::Save()
{
    clConfig conf(GetLocalConfigFile().GetFullPath());
    conf.WriteItem( this );
}

wxFileName WorkspaceSvnSettings::GetLocalConfigFile() const
{
    if ( m_workspaceFileName.IsOk() && m_workspaceFileName.Exists() ) {
        wxFileName fn(m_workspaceFileName.GetPath(), "subversion.conf");
        fn.AppendDir(".codelite");
        return fn;

    } else {
        wxFileName fn(clStandardPaths::Get().GetUserDataDir(), "subversion.conf");
        fn.AppendDir("config");
        return fn;
        
    }
}


