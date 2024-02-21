// clone_repo_dialog.cpp : implementation file
//

#include "studiox_includes.h"
#include "clone_repo_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char guix_repo_url[] = "https://github.com/eclipse-threadx/guix.git";

BEGIN_MESSAGE_MAP(clone_repo_dialog, express_dialog)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDB_EXIT_CLONE_DIALOG, OnCancel)
    ON_BN_CLICKED(IDB_CLONE_REPO, OnCloneRepo)
    ON_BN_CLICKED(IDB_SELECT_DIRECTORY, OnSetDirectory)
    ON_MESSAGE(USR_MSG_REPO_CLONE_MSG_UPDATE, OnRepoCloneMsgUpdate)
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(clone_repo_dialog, express_dialog)

#define GIT_CLONE_MSG_UPDATE_PERIOD 100 /* ms */
#define MSG_TYPE_CLONE    0
#define MSG_TYPE_COMPLETE 1

struct git_clone_info {
    char msg[MAX_PATH];
    int  msg_type;
    BOOL msg_updated;
    git_indexer_progress stats;
    BOOL stats_updated;
    int checkout_completed;
    int checkout_total;
    BOOL checkout_updated;
};

extern INI_INFO StudioXIni;
extern CString target_class_name;

static git_clone_info CloneInfo;

///////////////////////////////////////////////////////////////////////////////
static BOOL WriteCloneInfoToPipe(HANDLE writePipe, git_clone_info *info)
{
    if (writePipe)
    {
        DWORD writtenSize;
        BOOL result;

        result = WriteFile(writePipe, info, sizeof(git_clone_info), &writtenSize, NULL);
        if (writtenSize == sizeof(git_clone_info) && result)
        {
            info->checkout_updated = FALSE;
            info->msg_updated = FALSE;
            info->stats_updated = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
static BOOL ReadCloneInfoFromPipe(HANDLE readPipe, git_clone_info *info)
{
    DWORD readSize;

    if (readPipe)
    {
        BOOL result;
        result = ReadFile(readPipe, info, sizeof(git_clone_info), &readSize, NULL);

        if (readSize == sizeof(git_clone_info) && result)
        {
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
clone_repo_dialog::clone_repo_dialog(CWnd *parent)
    : express_dialog(clone_repo_dialog::IDD, parent)
{
    IconId = IDB_WARNING;
    SetTitleText(CString("Clone GUIX Repository"));

    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
    mpProgressScreen = NULL;
    mWritePipeHandle = 0;
    mReadPipeHandle = 0;
}

///////////////////////////////////////////////////////////////////////////////
BOOL clone_repo_dialog::OnInitDialog()
{
    express_dialog::OnInitDialog();
    AddCancelButton();
    return TRUE; // return TRUE unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::OnClose()
{
    CDialog::OnClose();
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::OnCancel()
{
    if (mpProgressScreen)
    {
        CompleteGUIXRepoClone(L"GUIX repository clone aborted!");
    }
    else
    {
        express_dialog::OnCancel();
    }
}

///////////////////////////////////////////////////////////////////////////////
int guix_transport_message_cb(const char *str, int len, void *payload)
{
    if (len >= MAX_PATH)
    {
        len = MAX_PATH - 1;
    }

    memcpy_s(CloneInfo.msg, MAX_PATH, str, len);
    CloneInfo.msg[len] = 0;

    CloneInfo.msg_type = MSG_TYPE_CLONE;
    CloneInfo.msg_updated = TRUE;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int guix_indexer_progress_cb(const git_indexer_progress *stats, void *payload)
{
    CloneInfo.stats = *stats;
    CloneInfo.stats_updated = TRUE;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void guix_checkout_progress_cb(const char *path, size_t completed_steps, size_t total_steps, void *payload)
{
    CloneInfo.checkout_completed = completed_steps;
    CloneInfo.checkout_total = total_steps;
    CloneInfo.checkout_updated = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void CALLBACK OnWriteCloneInfoTimer(void* lpParametar, BOOLEAN TimerOrWaitFired)
{
    HANDLE write_pipe_handle = (HANDLE)lpParametar;

    if (CloneInfo.msg_updated ||
        CloneInfo.checkout_updated ||
        CloneInfo.stats_updated)
    {
        WriteCloneInfoToPipe(write_pipe_handle, &CloneInfo);
    }
}

///////////////////////////////////////////////////////////////////////////////
void CloneGUIXRepo(char *local_path, HANDLE write_pipe_handle)
{
    BOOL clone_success = FALSE;
    git_repository* clone_out;
    git_clone_options options;
    git_remote_callbacks remote_callbacks;
    git_checkout_options checkout_opts;
    git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;

    git_libgit2_init();
    git_clone_options_init(&options, GIT_CLONE_OPTIONS_VERSION);

    remote_callbacks = fetch_options.callbacks;
    remote_callbacks.sideband_progress = guix_transport_message_cb;
    remote_callbacks.transfer_progress = guix_indexer_progress_cb;
    fetch_options.callbacks = remote_callbacks;
    options.fetch_opts = fetch_options;

    checkout_opts = options.checkout_opts;
    checkout_opts.progress_cb = guix_checkout_progress_cb;
    options.checkout_opts = checkout_opts;

    CloneInfo.checkout_updated = FALSE;
    CloneInfo.msg_updated = FALSE;
    CloneInfo.stats_updated = FALSE;

    // Create a timer to write git clone information to named pipe memory.
    HANDLE hTimer;
    BOOL success = ::CreateTimerQueueTimer(
        &hTimer,
        NULL,
        OnWriteCloneInfoTimer,
        (PVOID)write_pipe_handle,
        GIT_CLONE_MSG_UPDATE_PERIOD,
        GIT_CLONE_MSG_UPDATE_PERIOD,
        WT_EXECUTEINTIMERTHREAD);


    int clone_status = git_clone(&clone_out, guix_repo_url, local_path, &options);

    // Delete timer
    DeleteTimerQueueTimer(NULL, hTimer, NULL);

    const git_error* error = NULL;
    if (clone_status)
    {
        error = git_error_last();
    }
    else
    {
        git_repository_free(clone_out);
    }

    if(error)
    {
        memcpy_s(CloneInfo.msg, MAX_PATH, error->message, strnlen(error->message, MAX_PATH - 1) + 1);
    }
    else
    {
        CloneInfo.msg[0] = '\0';
    }

    git_libgit2_shutdown();

    // Nofity the calling process that git clone is completed.
    CloneInfo.msg_type = MSG_TYPE_COMPLETE;
    CloneInfo.msg_updated = TRUE;
    WriteCloneInfoToPipe(write_pipe_handle , &CloneInfo);
}

///////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI ReadCloneInfoThreadEntry(LPVOID thread_input)
{

    clone_repo_dialog *parent = (clone_repo_dialog *)thread_input;
    HANDLE read_pipe_handle = parent->GetReadPipeHandle();

    git_clone_info info;

    while (1)
    {
        if (ReadCloneInfoFromPipe(read_pipe_handle, &info))
        {
            parent->SendMessage(USR_MSG_REPO_CLONE_MSG_UPDATE, (WPARAM)&info, NULL);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::OnCloneRepo()
{
    TCHAR folder_path[MAX_PATH];
    char *repo_url = guix_repo_url;

    if (BrowseForFolder(this->GetSafeHwnd(), _T("Select root for GUIX git repository clone..."), _T("C:\\Eclipse_ThreadX"), folder_path))
    {
        //"git clone --depth 1 aka.ms/azrtos-guix-repo --branch master --single-branch"

        CString CloneMessage(_T("Cloning from source URL "));
        CloneMessage += repo_url;
        CloneMessage += " to\n";
        CloneMessage += "local folder ";
        CloneMessage += folder_path;
        CloneMessage += "\\guix";

        mpProgressScreen = new git_progress_screen(this, CloneMessage);
        mpProgressScreen->Create(IDD_GIT_PROGRESS, this);
        mpProgressScreen->ShowWindow(SW_SHOW);

        ShowHideChildren(SW_HIDE);

        mLocalPath.Format(L"%s\\guix", folder_path);

        // Run a subprocess to execute guix repo clone
        TCHAR exename[MAX_PATH];

        GetModuleFileName(NULL, exename, MAX_PATH);

        SECURITY_ATTRIBUTES security_attri;

        security_attri.nLength = sizeof(SECURITY_ATTRIBUTES);
        security_attri.lpSecurityDescriptor = NULL;
        security_attri.bInheritHandle = TRUE;

        if(!CreatePipe(&mReadPipeHandle, &mWritePipeHandle, &security_attri, 0))
        {
            ErrorMsg("Create git clone named pipe memory failed!", this);
            return;
        }

        // Start a work thread to read git clone message from pipe.
        mReadCloneInfoThreadHandle = CreateThread(NULL, GX_WIN32_STACK_SIZE, (LPTHREAD_START_ROUTINE)ReadCloneInfoThreadEntry, (LPVOID)this, 0, 0);

        if (mReadCloneInfoThreadHandle == INVALID_HANDLE_VALUE)
        {
            ErrorMsg("Create thread failed!", this);
            mReadCloneInfoThreadHandle = 0;
            return;
        }

        CString szCmdLine;
        szCmdLine.Format(L"%s -n --clone_guix_repo \"%s\" --write_pipe_handle %d", exename, mLocalPath, mWritePipeHandle);

        STARTUPINFO siStartInfo;

        // Set up members of the PROCESS_INFORMATION structure. 
        ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));

        // Set up members of the STARTUPINFO structure. 
        ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
        siStartInfo.cb = sizeof(STARTUPINFO);

        // Create the child process. 
        if(!CreateProcess(NULL,
            szCmdLine.GetBuffer(),     // command line 
            NULL,          // process security attributes 
            NULL,          // primary thread security attributes 
            TRUE,          // handles are inherited 
            0,             // creation flags 
            NULL,          // use parent's environment 
            NULL,          // use parent's current directory 
            &siStartInfo,  // STARTUPINFO pointer 
            &mProcessInfo  // receives PROCESS_INFORMATION 
            ))
        {
            ErrorMsg("Create git clone process failed!", this);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::CompleteGUIXRepoClone(CString msg)
{
    // Terminate git clone process
    TerminateProcess(mProcessInfo.hProcess, 0);

    // Close handles to the child process and its primary thread.
    CloseHandle(mProcessInfo.hProcess);
    CloseHandle(mProcessInfo.hThread);
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));

    // Close read and write pipe handles.
    if (mReadPipeHandle)
    {
        CloseHandle(mReadPipeHandle);
        mReadPipeHandle = 0;
    }

    if (mWritePipeHandle)
    {
        CloseHandle(mWritePipeHandle);
        mWritePipeHandle = 0;
    }

    // Terminate clone information read thread.
    if (mReadCloneInfoThreadHandle)
    {
        TerminateThread(mReadCloneInfoThreadHandle, 0);
        CloseHandle(mReadCloneInfoThreadHandle);
        mReadCloneInfoThreadHandle = 0;
    }

    BOOL clone_succeed = FALSE;

    if (!msg.IsEmpty())
    {
        ErrorMsg(CW2A(msg), this);
    }
    else
    {
        StudioXIni.samples_dir = mLocalPath;
        WriteIniInfo();
        clone_repo_dialog::PopulateRecentProjects(StudioXIni.samples_dir);
        Notify("The GUIX sample projects are now installed.", this);
        clone_succeed = TRUE;
    }

    mpProgressScreen->DestroyWindow();
    delete mpProgressScreen;
    mpProgressScreen = NULL;

    if (clone_succeed)
    {
        express_dialog::OnOK();
    }
    else
    {
        ShowHideChildren(SW_SHOW);
    }
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::PopulateRecentProjects(CString clone_dir)
{
    if (!clone_dir.IsEmpty())
    {
        clone_dir.MakeLower();
        StudioXIni.recent_project_paths[0] = clone_dir + _T("\\samples\\demo_guix_washing_machine\\demo_guix_washing_machine.gxp");
        StudioXIni.recent_project_paths[1] = clone_dir + _T("\\samples\\demo_guix_home_automation\\demo_guix_home_automation.gxp");
        StudioXIni.recent_project_paths[2] = clone_dir + _T("\\samples\\demo_guix_car_infotainment\\demo_guix_car_infotainment.gxp");
        StudioXIni.recent_project_paths[3] = clone_dir + _T("\\samples\\demo_guix_shapes\\guix_shapes.gxp");
        StudioXIni.recent_project_paths[4] = clone_dir + _T("\\samples\\demo_guix_widget_types\\guix_widget_types.gxp");

        // update recent projects menu
        CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
        pMain->UpdateRecentProjectsMenu();

        target_view *target = GetTargetView();
        if (target)
        {
            recent_project_win *dlg = (recent_project_win *) target->GetRecentDialog();
            if (dlg)
            {
                dlg->UpdateRecentList();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::OnSetDirectory()
{
    TCHAR path[MAX_PATH];
    char ascii_path[MAX_PATH];
    CString test_project;
    git_repository *repo;
    size_t converted_chars;
    BOOL ValidRepo = FALSE;

    git_libgit2_init();

    if (BrowseForFolder(this->GetSafeHwnd(), _T("Select local GUIX root directory.."), _T("C:\\"), path))
    {
        // Test to see if this is a git repository:

        wcstombs_s(&converted_chars, ascii_path, (wcslen(path) + 1) * 2, path, MAX_PATH);

        if (git_repository_open(&repo, ascii_path) != 0)
        {
            ErrorMsg("The selected directory does not appear to be a valid GUIX repository.", this);
        }
        else
        {
            git_repository_free(repo);

            test_project = path;
            test_project += "\\samples\\demo_guix_calculator\\guix_calculator.gxp";
    
            StudioXIni.samples_dir = path;
            WriteIniInfo();

            if (FileExists(PATH_TYPE_ABSOLUTE, test_project))
            {
                PopulateRecentProjects(path);
                ValidRepo = TRUE;
            }
            else
            {
                ErrorMsg("The selected directory doesn't contain the expected GUIX Studio sample projects. Please update your local GUIX repository.", this);
            }
        }
    }
    git_libgit2_shutdown();

    if (ValidRepo)
    {
       CDialog::OnOK();
    }

}

///////////////////////////////////////////////////////////////////////////////
LRESULT clone_repo_dialog::OnRepoCloneMsgUpdate(WPARAM wParam, LPARAM lParam)
{
    git_clone_info *info = (git_clone_info *)wParam;

    if (info->stats_updated)
    {
        mpProgressScreen->UpdateIndexerProgress(&info->stats);
    }

    if (info->checkout_updated)
    {
        mpProgressScreen->UpdateCheckoutProgress(info->checkout_completed, info->checkout_total);
    }

    if (info->msg_updated)
    {
        if (info->msg_type == MSG_TYPE_CLONE)
        {
            mpProgressScreen->UpdateGitMessage(info->msg);
        }
        else if (info->msg_type == MSG_TYPE_COMPLETE)
        {
            CString msg("");

            if (strnlen(info->msg, sizeof(info->msg)))
            {
                msg = _T("Unable to clone the GUIX repository to your local directory. The git reported error is: ");
                msg += info->msg;
            }
            CompleteGUIXRepoClone(msg);
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void clone_repo_dialog::ShowHideChildren(BOOL show)
{
    GetDlgItem(IDC_CLONE_PROMPT)->ShowWindow(show);
    GetDlgItem(IDB_CLONE_REPO)->ShowWindow(show);
    GetDlgItem(IDB_SELECT_DIRECTORY)->ShowWindow(show);
    GetDlgItem(IDB_EXIT_CLONE_DIALOG)->ShowWindow(show);
}

BEGIN_MESSAGE_MAP(git_progress_screen, CDialog)
    ON_WM_CREATE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
git_progress_screen::git_progress_screen(CWnd *parent, CString clone_message):
    CDialog(git_progress_screen::IDD, parent)
{
    mCloneMessage = clone_message;
}


///////////////////////////////////////////////////////////////////////////////
BOOL git_progress_screen::OnInitDialog()
{
    CDialog::OnInitDialog();
    CenterWindow();

    CStatic *CloneMessage = (CStatic *) GetDlgItem(IDC_CLONE_OPERATION);
    if (CloneMessage)
    {
        CloneMessage->SetWindowText(mCloneMessage);
    }

    CStatic* prog_field = (CStatic*)GetDlgItem(IDC_GIT_MESSAGE);
    if (prog_field)
    {
        prog_field->SetWindowText(NULL);
    }

    prog_field = (CStatic *) GetDlgItem(IDC_OBJECT_COUNT);
    if (prog_field)
    {
        prog_field->SetWindowText(NULL);
    }
    prog_field = (CStatic *) GetDlgItem(IDC_GIT_BYTES);
    if (prog_field)
    {
        prog_field->SetWindowText(NULL);
    }
    prog_field = (CStatic *) GetDlgItem(IDC_GIT_ERROR);
    if (prog_field)
    {
        prog_field->SetWindowText(NULL);
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void git_progress_screen::UpdateGitMessage(const char *str)
{
    CStatic *git_message = (CStatic *) GetDlgItem(IDC_GIT_MESSAGE);
    if (git_message)
    {
        CString msg(str);
        git_message->SetWindowText(msg);
    }
}

///////////////////////////////////////////////////////////////////////////////
void git_progress_screen::UpdateIndexerProgress(const git_indexer_progress *stats)
{
    CString stat_message;
    CStatic *stat_field;

    stat_field = (CStatic *) GetDlgItem(IDC_OBJECT_COUNT);

    if (stat_field)
    {
        if (stats->received_objects != stats->total_objects)
        {
            if (stats->received_objects)
            {
                stat_message.Format(_T("Received %d of %d objects"), stats->received_objects, stats->total_objects);
            }
            else
            {
                stat_message.Format(_T("Indexed %d of %d objects"), stats->indexed_objects, stats->total_objects);
            }
        }
        else
        {
            stat_message.Format(_T("Indexed %d of %d deltas"), stats->indexed_deltas, stats->total_deltas);
        }
        stat_field->SetWindowText(stat_message);
    }

    stat_field = (CStatic *) GetDlgItem(IDC_GIT_BYTES);
    if (stat_field)
    {
        stat_message.Format(_T("%d Total Bytes Received"), stats->received_bytes);
        stat_field->SetWindowTextW(stat_message);
    }
}

///////////////////////////////////////////////////////////////////////////////
void git_progress_screen::UpdateCheckoutProgress(size_t completed, size_t total)
{
    CString stat_message;
    CStatic *stat_field;

    stat_field = (CStatic *)GetDlgItem(IDC_GIT_ERROR);
    if (stat_field)
    {
        stat_message.Format(_T("Checking out %d of %d files"), completed, total);
        stat_field->SetWindowTextW(stat_message);
    }
}