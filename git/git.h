//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2011 by René Kraus (aka upCASE)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __git__
#define __git__

#include <wx/progdlg.h>

#include "plugin.h"
#include "asyncprocess.h"
#include "processreaderthread.h"
#include <queue>

typedef struct gitAction {
	int action;
	wxString arguments;
} gitAction;

class GitPlugin : public IPlugin
{
	enum {
		ID_SETGITREPO = wxID_HIGHEST + 1,
		ID_SETTINGS,
		ID_ADD_FILE,
		ID_DELETE_FILE,
		ID_DIFF_FILE,
		ID_RESET_FILE,
		ID_RESET_REPO,
		ID_CREATE_BRANCH,
		ID_SWITCH_BRANCH,
		ID_SWITCH_BRANCH_REMOTE,
		ID_PULL,
		ID_PUSH,
		ID_COMMIT,
		ID_COMMIT_LIST,
		ID_GITK,
		ID_LIST_MODIFIED,
		ID_REFRESH,
		ID_GARBAGE_COLLETION,
	};

	enum {
		gitNone = 0,
		gitListAll,
		gitListModified,
		gitListRemotes,
		gitAddFile,
		gitDeleteFile,
		gitDiffFile,
		gitDiffRepoCommit,
		gitResetFile,
		gitResetRepo,
		gitPull,
		gitPush,
		gitCommit,
		gitBranchCreate,
		gitBranchCurrent,
		gitBranchList,
		gitBranchListRemote,
		gitBranchSwitch,
		gitBranchSwitchRemote,
		gitCommitList,
		gitRebase,
		gitGarbageCollection,
	};

	wxArrayString m_localBranchList;
	wxArrayString m_remoteBranchList;
	wxArrayString m_trackedFiles;
	wxArrayString m_modifiedFiles;
	bool m_addedFiles;
	std::map<wxString, wxTreeItemId> m_modifiedIDs;
	wxArrayString m_remotes;

	wxColour m_colourTrackedFile;
	wxColour m_colourDiffFile;
	wxString m_pathGITExecutable;
	wxString m_pathGITKExecutable;
	wxString m_repositoryDirectory;
	wxString m_currentBranch;
	std::queue<gitAction> m_gitActionQueue;

	wxTimer m_progressTimer;
	wxProgressDialog* m_progressDialog;
	wxString m_progressMessage;
	wxString m_commandOutput;
	bool m_bActionRequiresTreUpdate;
	IProcess * m_process;
	wxEvtHandler *m_topWindow;

	clToolBar* m_pluginToolbar;
	wxMenu* m_pluginMenu;

private:
	void InitDefaults();
	void EnableMenuAndToolBar(bool enable);
	void AddDefaultActions();
	void ProcessGitActionQueue();
	void ColourFileTree(wxTreeCtrl *tree, const wxArrayString& files,
	                    const wxColour& colour,
	                    std::map<wxString, wxTreeItemId>& cIDs,
	                    bool storeIds);

	void FinishGitListAction(const gitAction& ga);
	void ListBranchAction(const gitAction& ga);
	void GetCurrentBranchAction(const gitAction& ga);
	void UpdateFileTree();

	void ShowProgress(const wxString& message, bool pulse = true);
	void HideProgress();

	DECLARE_EVENT_TABLE();
	// Event handlers
	void OnProgressTimer(wxTimerEvent& Event);
	void OnProcessTerminated(wxCommandEvent &event);
	void OnProcessOutput(wxCommandEvent &event);

	void OnFileSaved(wxCommandEvent& e);
	void OnFilesAddedToProject(wxCommandEvent& e);
	void OnWorkspaceLoaded(wxCommandEvent& e);

	void OnSetGitRepoPath(wxCommandEvent &e);
	void OnSettings(wxCommandEvent &e);
	void OnFileDiffSelected(wxCommandEvent &e);
	void OnFileResetSelected(wxCommandEvent &e);
	void OnFileAddSelected(wxCommandEvent &e);
	void OnFileDeleteSelected(wxCommandEvent &e);
	void OnSwitchLocalBranch(wxCommandEvent &e);
	void OnSwitchRemoteBranch(wxCommandEvent &e);
	void OnCreateBranch(wxCommandEvent &e);
	void OnCommit(wxCommandEvent &e);
	void OnCommitList(wxCommandEvent& e);
	void OnPush(wxCommandEvent &e);
	void OnPull(wxCommandEvent &e);
	void OnResetRepository(wxCommandEvent &e);
	void OnStartGitk(wxCommandEvent& e);
	void OnListModified(wxCommandEvent& e);
	void OnRefresh(wxCommandEvent& e);
	void OnGarbageColletion(wxCommandEvent& e);
public:
	GitPlugin(IManager *manager);
	~GitPlugin();

	//--------------------------------------------
	//Abstract methods
	//--------------------------------------------
	virtual clToolBar *CreateToolBar(wxWindow *parent);
	virtual void CreatePluginMenu(wxMenu *pluginsMenu);
	virtual void HookPopupMenu(wxMenu *menu, MenuType type);
	virtual void UnPlug();
};

#endif //git
