#pragma once

// clone repo dialog
#include "studiox_includes.h"
#include "afxwin.h"
#include "afxcmn.h"

extern "C" {
#include "git2.h"
}

// used for testing, so we don't have to keep deleting the .ini file
#ifdef _DEBUG
//#define FORCE_CLONE_DIALOG
#endif

void CloneGUIXRepo(char *local_path, HANDLE write_pipe_handle);

class git_progress_screen : public CDialog
{
public:
    git_progress_screen(CWnd *parent, CString clone_message);
    virtual BOOL OnInitDialog();

    void UpdateGitMessage(const char *str);
	void UpdateIndexerProgress(const git_indexer_progress *stats);
	void UpdateCheckoutProgress(size_t completed, size_t total);

	enum { IDD = IDD_GIT_PROGRESS};

    // Generated message map functions
	DECLARE_MESSAGE_MAP()

protected:

	CString mCloneMessage;
};

class clone_repo_dialog : public express_dialog
{
	DECLARE_DYNAMIC(clone_repo_dialog)
public:
	clone_repo_dialog(CWnd *parent);
    virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_CLONE_REPO };

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	afx_msg void OnCancel();
	afx_msg void OnCloneRepo();
	afx_msg void OnSetDirectory();
	afx_msg LRESULT OnRepoCloneMsgUpdate(WPARAM wParam, LPARAM lParam);

public:
	HANDLE GetReadPipeHandle() {return mReadPipeHandle;}

protected:
	void CompleteGUIXRepoClone(CString msg);
	void ShowHideChildren(BOOL show);
	void PopulateRecentProjects(CString clone_dir);

protected:
	git_progress_screen *mpProgressScreen;
	PROCESS_INFORMATION mProcessInfo;
	CString mLocalPath;
	HANDLE mWritePipeHandle;
	HANDLE mReadPipeHandle;
	HANDLE mReadCloneInfoThreadHandle;
};
