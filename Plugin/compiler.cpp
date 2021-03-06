//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : compiler.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "compiler.h"
#include "xmlutils.h"
#include <wx/log.h>
#include "macros.h"
#include "wx_xml_compatibility.h"
#include "build_system.h"
#include "build_settings_config.h"

Compiler::Compiler(wxXmlNode *node)
    : m_objectNameIdenticalToFileName(false)
{
    // ensure all relevant entries exist in switches map (makes sure they show up in build settings dlg)
    SetSwitch("Include",       "");
    SetSwitch("Debug",         "");
    SetSwitch("Preprocessor",  "");
    SetSwitch("Library",       "");
    SetSwitch("LibraryPath",   "");
    SetSwitch("Source",        "");
    SetSwitch("Output",        "");
    SetSwitch("Object",        "");
    SetSwitch("ArchiveOutput", "");
    SetSwitch("PreprocessOnly","");

    // ensure all relevant entries exist in tools map (makes sure they show up in build settings dlg)
    SetTool("LinkerName",             "");
    SetTool("SharedObjectLinkerName", "");
    SetTool("CXX",                    "");
    SetTool("CC",                     "");
    SetTool("AR",                     "");
    SetTool("ResourceCompiler",       "");
    SetTool("MAKE",                   "");
    SetTool("AS",                     "as"); // Assembler, default to as

    m_fileTypes.clear();
    if (node) {
        m_name = XmlUtils::ReadString(node, wxT("Name"));
        if (!node->HasProp(wxT("GenerateDependenciesFiles"))) {
            if (m_name == wxT("gnu g++") || m_name == wxT("gnu gcc")) {
                m_generateDependeciesFile = true;
            } else
                m_generateDependeciesFile = false;
        } else {
            m_generateDependeciesFile = XmlUtils::ReadBool(node, wxT("GenerateDependenciesFiles"));

        }

        if (!node->HasProp(wxT("ReadObjectsListFromFile"))) {
            m_readObjectFilesFromList = true;
        } else {
            m_readObjectFilesFromList = XmlUtils::ReadBool(node, wxT("ReadObjectsListFromFile"));
        }

        m_objectNameIdenticalToFileName = XmlUtils::ReadBool(node, wxT("ObjectNameIdenticalToFileName"));

        wxXmlNode *child = node->GetChildren();
        while (child) {
            if (child->GetName() == wxT("Switch")) {
                m_switches[XmlUtils::ReadString(child, wxT("Name"))] = XmlUtils::ReadString(child, wxT("Value"));
            }

            else if (child->GetName() == wxT("Tool")) {
                m_tools[XmlUtils::ReadString(child, wxT("Name"))] = XmlUtils::ReadString(child, wxT("Value"));
            }

            else if (child->GetName() == wxT("Option")) {
                wxString name = XmlUtils::ReadString(child, wxT("Name"));
                if (name == wxT("ObjectSuffix")) {
                    m_objectSuffix = XmlUtils::ReadString(child, wxT("Value"));
                } else if (name == wxT("DependSuffix")) {
                    m_dependSuffix = XmlUtils::ReadString(child, wxT("Value"));
                } else if (name == wxT("PreprocessSuffix")) {
                    m_preprocessSuffix = XmlUtils::ReadString(child, wxT("Value"));
                }
            } else if (child->GetName() == wxT("File")) {
                Compiler::CmpFileTypeInfo ft;
                ft.compilation_line = XmlUtils::ReadString(child, wxT("CompilationLine"));
                ft.extension = XmlUtils::ReadString(child, wxT("Extension")).Lower();

                long kind = (long)CmpFileKindSource;
                if (XmlUtils::ReadLong(child, wxT("Kind"), kind) == CmpFileKindSource) {
                    ft.kind = CmpFileKindSource;
                } else {
                    ft.kind = CmpFileKindResource;
                }
                m_fileTypes[ft.extension] = ft;
            }

            else if (child->GetName() == wxT("Pattern")) {
                if (XmlUtils::ReadString(child, wxT("Name")) == wxT("Error")) {
                    //found an error description
                    CmpInfoPattern errPattern;
                    errPattern.fileNameIndex = XmlUtils::ReadString(child, wxT("FileNameIndex"));
                    errPattern.lineNumberIndex = XmlUtils::ReadString(child, wxT("LineNumberIndex"));
                    errPattern.pattern = child->GetNodeContent();
                    m_errorPatterns.push_back(errPattern);
                } else if (XmlUtils::ReadString(child, wxT("Name")) == wxT("Warning")) {
                    //found a warning description
                    CmpInfoPattern warnPattern;
                    warnPattern.fileNameIndex = XmlUtils::ReadString(child, wxT("FileNameIndex"));
                    warnPattern.lineNumberIndex = XmlUtils::ReadString(child, wxT("LineNumberIndex"));
                    warnPattern.pattern = child->GetNodeContent();
                    m_warningPatterns.push_back(warnPattern);
                }
            }

            else if (child->GetName() == wxT("GlobalIncludePath")) {
                m_globalIncludePath = child->GetNodeContent();
            }

            else if (child->GetName() == wxT("GlobalLibPath")) {
                m_globalLibPath = child->GetNodeContent();
            }

            else if (child->GetName() == wxT("PathVariable")) {
                m_pathVariable = child->GetNodeContent();
            }

            else if (child->GetName() == wxT("CompilerOption")) {
                CmpCmdLineOption cmpOption;
                cmpOption.name = XmlUtils::ReadString(child, wxT("Name"));
                cmpOption.help = child->GetNodeContent();
                m_compilerOptions[cmpOption.name] = cmpOption;
            }

            else if (child->GetName() == wxT("LinkerOption")) {
                CmpCmdLineOption cmpOption;
                cmpOption.name = XmlUtils::ReadString(child, wxT("Name"));
                cmpOption.help = child->GetNodeContent();
                m_linkerOptions[cmpOption.name] = cmpOption;
            }

            child = child->GetNext();
        }
        
        if ( GetTool("MAKE").IsEmpty() ) {
            BuilderConfigPtr bldr = BuildSettingsConfigST::Get()->GetBuilderConfig("");
            if ( bldr ) {
                SetTool("MAKE", wxString() << bldr->GetToolPath() << " -j " << bldr->GetToolJobs());
            }
        }
        
        // Default values for the assembler
        if ( GetTool("AS").IsEmpty() && GetName().CmpNoCase("vc++") == 0 ) {
            SetTool("AS", "nasm");
            
        } else if ( GetTool("AS").IsEmpty() ) {
            SetTool("AS", "as");
        }
    } else {
        // Create a default compiler: g++
        m_name = "gnu g++";
        SetSwitch("Include",        "-I");
        SetSwitch("Debug",          "-g ");
        SetSwitch("Preprocessor",   "-D");
        SetSwitch("Library",        "-l");
        SetSwitch("LibraryPath",    "-L");
        SetSwitch("Source",         "-c ");
        SetSwitch("Output",         "-o ");
        SetSwitch("Object",         "-o ");
        SetSwitch("ArchiveOutput",  " ");
        SetSwitch("PreprocessOnly", "-E");
        
        m_objectSuffix = ".o";
        m_preprocessSuffix = ".i";
        
        AddPattern(eErrorPattern, "^([^ ][a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([0-9]*)([:0-9]*)(: )((fatal error)|(error)|(undefined reference))", 1, 3);
        AddPattern(eErrorPattern, "^([^ ][a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([^ ][a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)(\\(\\.text\\+[0-9a-fx]*\\))", 3, 1);
        AddPattern(eErrorPattern, "^([^ ][a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([^ ][a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([0-9]+)(:)", 3, 1);
        AddPattern(eErrorPattern, "undefined reference to", -1, -1);
        AddPattern(eErrorPattern, "\\*\\*\\* \\[[a-zA-Z\\-_0-9 ]+\\] (Error)", -1, -1);
        
        AddPattern(eWarningPattern, "([a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([0-9]+ *)(:)([0-9:]*)?( warning)", 1, 3);
        AddPattern(eWarningPattern, "([a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([0-9]+ *)(:)([0-9:]*)?( note)", 1, 3);
        AddPattern(eWarningPattern, "([a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([0-9]+ *)(:)([0-9:]*)?([ ]+instantiated)", 1, 3);
        AddPattern(eWarningPattern, "(In file included from *)([a-zA-Z:]{0,2}[ a-zA-Z\\.0-9_/\\+\\-]+ *)(:)([0-9]+ *)(:)([0-9:]*)?", 2, 4);

        SetTool("LinkerName",             "g++");
        SetTool("SharedObjectLinkerName", "g++ -shared -fPIC");
        SetTool("CXX",                    "g++");
        SetTool("CC",                     "gcc");
        SetTool("AR",                     "ar rcu");
        SetTool("ResourceCompiler",       "windres");
        SetTool("AS",                     "as");

#ifdef __WXMSW__
        SetTool("MAKE",                   "mingw32-make");
#else
        SetTool("MAKE",                   "make");
#endif

        m_globalIncludePath                    = wxEmptyString;
        m_globalLibPath                        = wxEmptyString;
        m_pathVariable                         = wxEmptyString;
        m_generateDependeciesFile              = false;
        m_readObjectFilesFromList              = true;
        m_objectNameIdenticalToFileName        = false;
    }
    
    if (m_generateDependeciesFile && m_dependSuffix.IsEmpty()) {
        m_dependSuffix = m_objectSuffix + wxT(".d");
    }
    
    if (!m_switches[wxT("PreprocessOnly")].IsEmpty() && m_preprocessSuffix.IsEmpty()) {
        m_preprocessSuffix = m_objectSuffix + wxT(".i");
    }

    if (m_fileTypes.empty()) {
        AddCmpFileType("cpp", CmpFileKindSource,   "$(CXX) $(SourceSwitch) \"$(FileFullPath)\" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("cxx", CmpFileKindSource,   "$(CXX) $(SourceSwitch) \"$(FileFullPath)\" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("c++", CmpFileKindSource,   "$(CXX) $(SourceSwitch) \"$(FileFullPath)\" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("c",   CmpFileKindSource,   "$(CC) $(SourceSwitch) \"$(FileFullPath)\" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("cc",  CmpFileKindSource,   "$(CXX) $(SourceSwitch) \"$(FileFullPath)\" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("m",   CmpFileKindSource,   "$(CXX) -x objective-c $(SourceSwitch) \"$(FileFullPath)\" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("mm",  CmpFileKindSource,   "$(CXX) -x objective-c++ $(SourceSwitch) \"$(FileFullPath)\" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(IncludePath)");
        AddCmpFileType("s",   CmpFileKindSource,   "$(AS) \"$(FileFullPath)\" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) -I$(IncludePath)");
        AddCmpFileType("rc",  CmpFileKindResource, "$(RcCompilerName) -i \"$(FileFullPath)\" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) $(RcIncludePath)");
    }
    
    // Add support for assembler file
    if ( m_fileTypes.count("s") == 0 ) {
        AddCmpFileType("s", 
                       CmpFileKindSource, 
                       "$(AS) \"$(FileFullPath)\" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/$(ObjectName)$(ObjectSuffix) -I$(IncludePath)");
    }
}

Compiler::~Compiler()
{
}

wxXmlNode *Compiler::ToXml() const
{
    wxXmlNode *node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Compiler"));
    node->AddProperty(wxT("Name"), m_name);
    node->AddProperty(wxT("GenerateDependenciesFiles"),     BoolToString(m_generateDependeciesFile));
    node->AddProperty(wxT("ReadObjectsListFromFile"),       BoolToString(m_readObjectFilesFromList));
    node->AddProperty(wxT("ObjectNameIdenticalToFileName"), BoolToString(m_objectNameIdenticalToFileName));

    std::map<wxString, wxString>::const_iterator iter = m_switches.begin();
    for (; iter != m_switches.end(); iter++) {
        wxXmlNode *child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Switch"));
        child->AddProperty(wxT("Name"), iter->first);
        child->AddProperty(wxT("Value"), iter->second);
        node->AddChild(child);
    }

    iter = m_tools.begin();
    for (; iter != m_tools.end(); iter++) {
        wxXmlNode *child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Tool"));
        child->AddProperty(wxT("Name"), iter->first);
        child->AddProperty(wxT("Value"), iter->second);
        node->AddChild(child);
    }

    std::map<wxString, Compiler::CmpFileTypeInfo>::const_iterator it = m_fileTypes.begin();
    for (; it != m_fileTypes.end(); it++) {
        wxXmlNode *child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("File"));
        Compiler::CmpFileTypeInfo ft = it->second;
        child->AddProperty(wxT("Extension"), ft.extension);
        child->AddProperty(wxT("CompilationLine"), ft.compilation_line);

        wxString strKind;
        strKind << (long)ft.kind;
        child->AddProperty(wxT("Kind"), strKind);

        node->AddChild(child);
    }

    wxXmlNode *options = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Option"));
    options->AddProperty(wxT("Name"),  wxT("ObjectSuffix"));
    options->AddProperty(wxT("Value"), m_objectSuffix);
    node->AddChild(options);

    options = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Option"));
    options->AddProperty(wxT("Name"),  wxT("DependSuffix"));
    options->AddProperty(wxT("Value"), m_dependSuffix);
    node->AddChild(options);

    options = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Option"));
    options->AddProperty(wxT("Name"),  wxT("PreprocessSuffix"));
    options->AddProperty(wxT("Value"), m_preprocessSuffix);
    node->AddChild(options);

    //add patterns
    CmpListInfoPattern::const_iterator itPattern;
    for (itPattern = m_errorPatterns.begin(); itPattern != m_errorPatterns.end(); ++itPattern) {
        wxXmlNode *error = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Pattern"));
        error->AddProperty(wxT("Name"), wxT("Error"));
        error->AddProperty(wxT("FileNameIndex"), itPattern->fileNameIndex);
        error->AddProperty(wxT("LineNumberIndex"), itPattern->lineNumberIndex);
        XmlUtils::SetNodeContent(error, itPattern->pattern);
        node->AddChild(error);
    }

    for (itPattern = m_warningPatterns.begin(); itPattern != m_warningPatterns.end(); ++itPattern) {
        wxXmlNode *warning = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Pattern"));
        warning->AddProperty(wxT("Name"), wxT("Warning"));
        warning->AddProperty(wxT("FileNameIndex"), itPattern->fileNameIndex);
        warning->AddProperty(wxT("LineNumberIndex"), itPattern->lineNumberIndex);
        XmlUtils::SetNodeContent(warning, itPattern->pattern);
        node->AddChild(warning);
    }

    wxXmlNode *globalIncludePath = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("GlobalIncludePath"));
    XmlUtils::SetNodeContent(globalIncludePath, m_globalIncludePath);
    node->AddChild(globalIncludePath);

    wxXmlNode *globalLibPath = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("GlobalLibPath"));
    XmlUtils::SetNodeContent(globalLibPath, m_globalLibPath);
    node->AddChild(globalLibPath);

    wxXmlNode *pathVariable = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("PathVariable"));
    XmlUtils::SetNodeContent(pathVariable, m_pathVariable);
    node->AddChild(pathVariable);

    // Add compiler options
    CmpCmdLineOptions::const_iterator itCmpOption = m_compilerOptions.begin();
    for ( ; itCmpOption != m_compilerOptions.end(); ++itCmpOption) {
        const CmpCmdLineOption& cmpOption = itCmpOption->second;
        wxXmlNode* pCmpOptionNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("CompilerOption"));
        pCmpOptionNode->AddProperty(wxT("Name"), cmpOption.name);
        XmlUtils::SetNodeContent(pCmpOptionNode, cmpOption.help);
        node->AddChild(pCmpOptionNode);
    }

    // Add linker options
    CmpCmdLineOptions::const_iterator itLnkOption = m_linkerOptions.begin();
    for ( ; itLnkOption != m_linkerOptions.end(); ++itLnkOption) {
        const CmpCmdLineOption& lnkOption = itLnkOption->second;
        wxXmlNode* pLnkOptionNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("LinkerOption"));
        pLnkOptionNode->AddProperty(wxT("Name"), lnkOption.name);
        XmlUtils::SetNodeContent(pLnkOptionNode, lnkOption.help);
        node->AddChild(pLnkOptionNode);
    }

    return node;
}

wxString Compiler::GetSwitch(const wxString &name) const
{
    std::map<wxString, wxString>::const_iterator iter = m_switches.find(name);
    if (iter == m_switches.end()) {
        return wxEmptyString;
    }
    return iter->second;
}

wxString Compiler::GetTool(const wxString &name) const
{
    std::map<wxString, wxString>::const_iterator iter = m_tools.find(name);
    if (iter == m_tools.end()) {
        if(name == wxT("CC")) {
            // an upgrade, return the CXX
            return GetTool(wxT("CXX"));
        }
        return wxEmptyString;
    }
    if(name == wxT("CC") && iter->second.empty()) {
        return GetTool(wxT("CXX"));
    }
    return iter->second;
}

bool Compiler::GetCmpFileType(const wxString& extension, Compiler::CmpFileTypeInfo &ft)
{
    std::map<wxString, Compiler::CmpFileTypeInfo>::iterator iter = m_fileTypes.find(extension.Lower());
    if (iter == m_fileTypes.end()) {
        return false;
    }
    ft = iter->second;
    return true;
}

void Compiler::AddCmpFileType(const wxString &extension, CmpFileKind type, const wxString &compile_line)
{
    Compiler::CmpFileTypeInfo ft;
    ft.extension = extension.Lower();
    if ( m_fileTypes.count(ft.extension) ) {
        m_fileTypes.erase(ft.extension);
    }
    
    ft.compilation_line = compile_line;
    ft.kind = type;
    m_fileTypes[extension] = ft;
}

void Compiler::SetSwitch(const wxString& switchName, const wxString& switchValue)
{
    if ( m_switches.count(switchName) ) {
        m_switches.erase(switchName);
    }
    m_switches.insert(std::make_pair(switchName, switchValue));
}

void Compiler::AddPattern(int type, const wxString& pattern, int fileNameIndex, int lineNumberIndex)
{
    CmpInfoPattern pt;
    pt.pattern         = pattern;
    pt.fileNameIndex   = wxString::Format("%d", (int)fileNameIndex);
    pt.lineNumberIndex = wxString::Format("%d", (int)lineNumberIndex);
    if ( type == eErrorPattern ) {
        m_errorPatterns.push_back(pt);
        
    } else {
        m_warningPatterns.push_back(pt);
    }
}

void Compiler::SetTool(const wxString& toolname, const wxString& cmd)
{
    if ( m_tools.count(toolname) ) {
        m_tools.erase(toolname);
    }
    m_tools.insert(std::make_pair(toolname, cmd));
}
