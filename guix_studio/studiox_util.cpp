#include "studiox_includes.h"
#include "direct.h"
#include "wait_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHARMAP_SIZE_DEFAULT  8192
#define CHARMAP_SIZE_EXTENDED 262144


CBrush WinBackBrush(WIN_BACKGROUND_COLOR);
CBrush BlackBrush(RGB(0, 0, 0));

wait_dialog *pWaitDialog = NULL;
int wait_nesting = 0;
BOOL is_guix_canvas_refresh_blocked = FALSE;

INI_INFO StudioXIni;
CString   gOpenProjectDir;

studiox_project *gpOpenProject = NULL;

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
    return _iob;
}



///////////////////////////////////////////////////////////////////////////////
void SetOpenProject(studiox_project *open_project)
{
    gpOpenProject = open_project;
}

///////////////////////////////////////////////////////////////////////////////
studiox_project *GetOpenProject(void)
{
    return gpOpenProject;
}

///////////////////////////////////////////////////////////////////////////////
int project_lib_version(void)
{
    if (gpOpenProject)
    {
        return gpOpenProject->mHeader.guix_version;
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    // If the BFFM_INITIALIZED message is received
    // set the path to the start path.
    switch (uMsg)
    {
        case BFFM_INITIALIZED:
        {
            if (NULL != lpData)
            {
                SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
            }
        }
    }

    return 0; // The function should always return 0.
}

///////////////////////////////////////////////////////////////////////////////
// HWND is the parent window.
// szCurrent is an optional start folder. Can be NULL.
// szPath receives the selected path on success. Must be MAX_PATH characters in length.

BOOL BrowseForFolder(HWND hwnd, LPCTSTR caption, LPCTSTR szCurrent, LPTSTR szPath)
{
    BROWSEINFO   bi = { 0 };
    LPITEMIDLIST pidl;
    TCHAR        szDisplay[MAX_PATH];
    BOOL         retval;

    if (caption == NULL)
    {
        caption = _T("Please choose a folder...");
    }
    bi.hwndOwner      = hwnd;
    bi.pszDisplayName = szDisplay;
    bi.lpszTitle      = caption;
    bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn           = BrowseCallbackProc;
    bi.lParam         = (LPARAM) szCurrent;

    pidl = SHBrowseForFolder(&bi);

    if (NULL != pidl)
    {
        retval = SHGetPathFromIDList(pidl, szPath);
        CoTaskMemFree(pidl);
    }
    else
    {
        retval = FALSE;
    }

    if (!retval)
    {
        szPath[0] = TEXT('\0');
    }

    //CoUninitialize();
    return retval;
}

///////////////////////////////////////////////////////////////////////////////
BOOL MakeRelativePath(const CString &projpath, CString &filepath)
{
    int index = 0;
    int lastdir = 0;
    BOOL dirfound = FALSE;
    CString temp;

    // make sure path start with a drive letter:
    if (filepath.IsEmpty() || filepath[1] != ':') 
    {
        return FALSE;
    }

    // check to make sure drive letters are the same:
    if (toupper(projpath[0]) != toupper(filepath[0]))
    {
        return FALSE;
    }

    while(1)
    {
        // are we at the end of either path?
        if (!projpath[index] || !filepath[index])
        {
            break;
        }
        
        // do the path strrings still match?
        if (toupper(projpath[index]) != toupper(filepath[index]))
        {
            break;
        }

        // if we are not at the end, keep track of the start
        // of the last directory name:
        if (filepath[index])
        {
            if (filepath[index] == ':' || filepath[index] == '\\')
            {
                dirfound = TRUE;
                lastdir = index;
            }
            index++;
        }
    }

    if (filepath[index] || projpath[index])
    {
        // did we find any matching directories?
        if (!dirfound)
        {
            return FALSE;
        }

        // here if we found matching directories. If the project
        // path is longer, then we need to back up:

        if (projpath[index])
        {
            temp = "..\\";
            if (lastdir == index - 1)
            {    
                index++;
            }
        }
        else
        {
            if (filepath[index] == '\\')
            {
                lastdir = index;
                temp = "";
            }
        }

        // count directories remainin on project path, back up that
        // many times
        while(projpath[index])
        {
            if (projpath[index] == '\\' && projpath[index + 1] != 0)
            {
                temp += "..\\";
            }
            index++;
        }

        // skip past the delimiter
        if (filepath[lastdir] == '\\')
        {
            lastdir++;
        }
        temp += filepath.Right(filepath.GetLength() - lastdir);
        filepath = temp;
    }
    else
    {
        // if we get to here, then the project path and the requested
        // path are exactly the same
        filepath = ".\\";
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL ConvertToProjectRelativePath(CString &path)
{
    if (MakeRelativePath(gOpenProjectDir, path))
    {
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void SaveToPathInfo(PATHINFO &info, CString &path)
{
    info.pathname = path;

    switch(info.pathtype)
    {
    case PATH_TYPE_PROJECT_RELATIVE:
        if (!ConvertToProjectRelativePath(info.pathname))
        {
            info.pathtype = PATH_TYPE_ABSOLUTE;
        }
        break;

    case PATH_TYPE_INSTALL_RELATIVE:
        if (!MakeRelativePath(GetSamplesDir(), info.pathname))
        {
            info.pathtype = PATH_TYPE_ABSOLUTE;
        }
        break;

    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
//C:\Renesas\guix_full_source_eval_for_rx_ylcdrx63n_iar\threadx\rx_ylcdrx63n\iar\demo_guix_medical
CString ConcatPathNames(CString &root, CString &extra)
{
    CString base = root;
    CString addition = extra;

    // trim leading ./ if it exists

    if (addition[0] == '.' &&
        addition[1] == '\\')
    {
        addition = addition.Right(addition.GetLength() - 2);
    }

    // for every ../ trim one directory from root
    
    while(addition.Find(_T("..\\")) == 0)
    {
        addition = addition.Right(addition.GetLength() - 3);
        base = base.Left(base.ReverseFind('\\'));
    }

    if (!addition.IsEmpty())
    {
        if (addition[0] != '\\')
        {
            base += '\\';
        }
        base += addition;
    }
    return base;
}

///////////////////////////////////////////////////////////////////////////////
CString MakeAbsolutePathname(PATHINFO &info)
{
    CString pathname;

    switch(info.pathtype)
    {
    case PATH_TYPE_ABSOLUTE:
        pathname = info.pathname;
        break;

    case PATH_TYPE_PROJECT_RELATIVE:
        pathname = ConcatPathNames(gOpenProjectDir, info.pathname);
        break;

    case PATH_TYPE_INSTALL_RELATIVE:
        pathname = ConcatPathNames(GetSamplesDir(), info.pathname);
        break;
    }
    return pathname;
}

///////////////////////////////////////////////////////////////////////////////
void GotoProjectDirectory()
{
    _tchdir(gOpenProjectDir.GetString());
}

///////////////////////////////////////////////////////////////////////////////
void SetProjectDirectory(CString &path)
{
    if (!PathIsRelative(path))
    {
        gOpenProjectDir = path;
    }
    else
    {
        TCHAR current_dir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, current_dir);

        if ((path.GetAt(0) == '.') && (path.GetAt(1) != '.'))
        {
            path = path.Mid(1);
        }

        gOpenProjectDir.Format(_T("%s\\%s"), current_dir, path);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL CheckOutputFileSecurity(CString &pathname, BOOL binary_mode)
{
    int length = pathname.GetLength();
    int index;
    CString extension = L"";

    // Check for blacklisted directory in pathname 

    if (pathname.Find(_T("Start Menu\\Programs")) >= 0)
    {
        CString error_msg = _T("Pathname is not allowed: ");
        error_msg += pathname;
        ErrorMsg(error_msg);
        return FALSE;
    }

    index = pathname.ReverseFind(L'.');

    if(index >= 0)
    {
        extension = pathname.Mid(index + 1);
    }
    else
    {
        index = pathname.GetLength();
    }

    if (binary_mode)
    {
        if (extension != L"bin")
        {
            pathname = pathname.Left(index);
            pathname += L".bin";
        }
    }
    else
    {
        if (extension != L"c" && extension != L"h")
        {
            // strip the user-supplied extension and force the extension to .c
            pathname = pathname.Left(index);
            pathname += L".c";
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void WriteToDosFile(CFile &outfile, CString &out)
{
    CString dos;
    int start = 0;

    while(1)
    {
        int newline = out.Find('\n', start);

        if (newline < 0)
        {
            dos += out.Mid(start);
            break;
        }
        if (newline > start)
        {
            dos += out.Mid(start, (newline - start));
        }
        start = newline + 1;
        dos += "\r\n";
    }
            
    outfile.Write(dos.GetBuffer(), dos.GetLength());
}

///////////////////////////////////////////////////////////////////////////////
wchar_t *CStringToWideChar(CString &text)
{
#ifdef _UNICODE
    return text.GetBuffer();
#else if
    wchar_t *uni_text;
    int len = text.GetLength();
    
    if (len)
    {
        len++;
        uni_text = new wchar_t[len];
        MultiByteToWideChar(CP_UTF8, 0, text.GetBuffer(), -1, uni_text, len);
        return uni_text;
    }
    return NULL;
#endif
}

///////////////////////////////////////////////////////////////////////////////
void SelectDropListItem(CComboBox *box, long val)
{
    int box_index;

    for (box_index = 0; box_index < box->GetCount(); box_index++)
    {
        if (box->GetItemData(box_index) == val)
        {
            box->SetCurSel(box_index);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
CString RemoveFileExtension(CString &filename)
{
    int pos = filename.ReverseFind('.');
    if (pos > 0)
    {
        return filename.Left(pos);
    }
    return filename;
}

///////////////////////////////////////////////////////////////////////////////
void ErrorMsg(const char *msg, CWnd *parent)
{
    /*Pick up command info. */
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if (!parent)
    {
        parent = AfxGetApp()->m_pMainWnd;
    }

    if(!pCmdInfo->IsNoGui())
    {
        message_dialog dlg("Error", msg, false, parent);
        dlg.DoModal();
    }
    else
    {
        if(pCmdInfo ->GetLogFile())
        {
            fprintf(pCmdInfo ->GetLogFile(), "%s\n",msg);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ErrorMsg(const CString &msg, CWnd *parent)
{
    /*Pick up command info. */
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if (!parent)
    {
        parent = AfxGetApp()->m_pMainWnd;
    }

    if(!pCmdInfo->IsNoGui())
    {
        message_dialog dlg("Error", CW2A(msg), false, parent);
        dlg.DoModal();
    }
    else
    {
        if(pCmdInfo ->GetLogFile())
        {
            fprintf(pCmdInfo->GetLogFile(), "%s\n", (char *)CW2A(msg));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void Notify(const char *msg, CWnd *parent)
{
    /*Pick up command info. */
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if (!parent)
    {
        parent = AfxGetApp()->m_pMainWnd;
    }

    if(!pCmdInfo->IsNoGui())
    {
        message_dialog dlg("Notification", msg, false, parent);
        dlg.DoModal();
    }
    else if(pCmdInfo ->GetLogFile())
    {
        fprintf(pCmdInfo ->GetLogFile(), "%s\n",msg);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL StartWorkThread(LPTHREAD_START_ROUTINE routine, LPVOID param, const char *msg, BOOL block_canvas_refresh)
{
    CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CCommandInfo* pCmdInfo = pMain->GetCmdInfo();
    BOOL result = TRUE;

    if (pCmdInfo->IsNoGui())
    {
        Notify(msg);
        result = routine(param);
    }
    else
    {
        HANDLE hthread = CreateThread(NULL, GX_WIN32_STACK_SIZE, (LPTHREAD_START_ROUTINE)routine, (LPVOID)param, CREATE_SUSPENDED, 0);

        if ((hthread != INVALID_HANDLE_VALUE) && hthread)
        {
            is_guix_canvas_refresh_blocked = block_canvas_refresh;

            BusyMsg(msg, hthread);

            GetExitCodeThread(hthread, (LPDWORD)&result);

            CloseHandle(hthread);
        }
        else
        {
            result = FALSE;
            ErrorMsg(L"Internal Error!");
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
BOOL IsWaitDialogRunning()
{
    if (pWaitDialog)
    {
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL IsGuixCanvasRefreshBlocked()
{
    return is_guix_canvas_refresh_blocked;
}

///////////////////////////////////////////////////////////////////////////////
void BusyMsg(const char *msg, HANDLE thread)
{
    /*Pick up command info. */
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if(!pCmdInfo->IsNoGui())
    {
        wait_nesting++;

        if (wait_nesting == 1)
        {
            pWaitDialog = new wait_dialog(300, 120, msg, pMain);
            pWaitDialog->SetWorkThread(thread);
            pWaitDialog->DoModal();
            delete pWaitDialog;
            pWaitDialog = NULL;
            is_guix_canvas_refresh_blocked = FALSE;
        }
        else
        {
            ResumeThread(thread);
        }
    }
    else if(pCmdInfo ->GetLogFile())
    {
        fprintf(pCmdInfo ->GetLogFile(), "%s\n", msg);
        ResumeThread(thread);
    }
}

///////////////////////////////////////////////////////////////////////////////
void EndBusyMsg(void)
{

    /*Pick up command info. */
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if(!pCmdInfo->IsNoGui())
    {
        if (wait_nesting > 0)
        {
            wait_nesting--;
        }

        if (!wait_nesting)
        {
            pWaitDialog->SendMessage(WM_CLOSE, 0, 0);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL AskUser(const char *msg, CWnd *parent)
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if(!pCmdInfo->IsNoGui())
    {
        if (parent == NULL)
        {
            parent = AfxGetApp()->m_pMainWnd;
        }

        message_dialog dlg("Please Confirm", msg, true, parent);

        if (dlg.DoModal() == IDOK)
        {
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
BOOL BrowseForSingleFile(LPCTSTR caption, LPCTSTR filter,
    LPCTSTR def_extension, CString &szPath, CWnd *parent)
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();

    OPENFILENAME OpenInfo;

    TCHAR SavePathname[MAX_PATH];
    SavePathname[0] = 0;
    szPath = _T("");

    OpenInfo.lStructSize = sizeof(OPENFILENAME);
    OpenInfo.hwndOwner = parent->m_hWnd;
    OpenInfo.hInstance = NULL;
    OpenInfo.lpstrFilter = filter;
    OpenInfo.lpstrCustomFilter = NULL;
    OpenInfo.lpstrFile = SavePathname;
    OpenInfo.nMaxFile = MAX_PATH;
    OpenInfo.lpstrFileTitle = NULL;

    if (pMain->IsTestMode())
    {
        OpenInfo.lpstrInitialDir = gOpenProjectDir;
    }
    else
    {
        OpenInfo.lpstrInitialDir = NULL;
    }
    
    OpenInfo.lpstrTitle = caption;
    OpenInfo.lpstrDefExt = def_extension;

    OpenInfo.Flags = (OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER);

    if (GetOpenFileName(&OpenInfo))
    {
        szPath = SavePathname;
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
int BrowseForMultipleFiles(TCHAR *caption, TCHAR * filter, TCHAR *def_extension,
                           TCHAR **pathptr, TCHAR **nameptr, CWnd *parent)
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();

    OPENFILENAME OpenInfo;
    int NumFiles = 0;
    int NameLen;
    TCHAR *pGet;


    TCHAR *pSavePath = new TCHAR[MAX_PATH * MAX_OPEN_FILES + 1];
    pSavePath[0] = 0;
    //szPath = _T("");
    

    OpenInfo.lStructSize = sizeof(OPENFILENAME);
    OpenInfo.hwndOwner = parent->m_hWnd;
    OpenInfo.hInstance = NULL;
    OpenInfo.lpstrFilter = filter;
    OpenInfo.lpstrCustomFilter = NULL;
    OpenInfo.lpstrFile = pSavePath;
    OpenInfo.nMaxFile = MAX_PATH * MAX_OPEN_FILES;
    OpenInfo.lpstrFileTitle = NULL;

    if (pMain->IsTestMode())
    {
        OpenInfo.lpstrInitialDir = gOpenProjectDir;
    }
    else
    {
        OpenInfo.lpstrInitialDir = NULL;
    }

    OpenInfo.lpstrTitle = caption;
    OpenInfo.lpstrDefExt = def_extension;

    OpenInfo.Flags = (OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|
        OFN_EXPLORER|OFN_ALLOWMULTISELECT);

    if (GetOpenFileName(&OpenInfo))
    {
        pGet = pSavePath;
        while(*pGet)
        {
            NumFiles++;
            NameLen = (int) _tcslen(pGet) + 1;
            pGet += NameLen;
        }
    }
    if (NumFiles > 0)
    {
        *pathptr = pSavePath;
        *nameptr = pSavePath + OpenInfo.nFileOffset;
    }
    else
    {
        *pathptr = NULL;
        *nameptr = NULL;
        delete [] pSavePath;
    }
    return NumFiles;
}

///////////////////////////////////////////////////////////////////////////////
BOOL GetOutputFileName(TCHAR *SavePathname, TCHAR *SaveFilename,
                       TCHAR *title, TCHAR *filter, TCHAR *initial_dir,
                       TCHAR *def_extension, CWnd *parent)
{

    OPENFILENAME OpenInfo;

    if (!parent)
    {
        parent = AfxGetMainWnd();
    }

    OpenInfo.lStructSize = sizeof(OPENFILENAME);
    OpenInfo.hwndOwner = parent->m_hWnd;
    OpenInfo.hInstance = NULL;
    OpenInfo.lpstrFilter = filter;
    OpenInfo.lpstrCustomFilter = NULL;
    OpenInfo.lpstrFile = SavePathname;
    OpenInfo.nMaxFile = MAX_PATH;
    OpenInfo.lpstrFileTitle = SaveFilename;
    OpenInfo.nMaxFileTitle = MAX_PATH;
    OpenInfo.lpstrInitialDir = initial_dir;
    OpenInfo.lpstrTitle = title;
    OpenInfo.lpstrDefExt = def_extension;

    OpenInfo.Flags = (OFN_PATHMUSTEXIST|OFN_EXPLORER);

    if (GetOpenFileName(&OpenInfo))
    {
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void MakePath(CString path)
{
    int index = 0;

    if (_tchdir(path) != 0)
    {
        CString Token = path.Tokenize(_T("\\"), index);

        while (!Token.IsEmpty())
        {
            if (Token.Find(_T(":")) != -1)
            {
                Token += "\\";
            }
            if (_tchdir(Token) == 0)
            {
                Token = path.Tokenize(_T("\\"), index);
                if (!Token.IsEmpty())
                {
                    _wmkdir(Token);
                }
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
CString GetAppDataPath()
{
    CString path = CString(getenv("APPDATA"));
    path += "\\Eclipse Foundation\\Eclipse_ThreadX\\GUIX_Studio";
    MakePath(path);
    return path;
}

///////////////////////////////////////////////////////////////////////////////
int CalculateStudioVersion()
{
    int studio_version = GUIX_MAJOR_VERSION * 1000000 + GUIX_MINOR_VERSION * 10000 + GUIX_PATCH_VERSION * 100 + STUDIOX_VERSION_NUMBER;
    return studio_version;
}


///////////////////////////////////////////////////////////////////////////////
BOOL ReadIniInfo(void)
{
    int loop;
    BOOL FoundIni = FALSE;
  
    /* Configure defaults in case this is first time use */

    StudioXIni.ini_version = STUDIO_INI_VERSION;
    StudioXIni.studio_version = CalculateStudioVersion();
    StudioXIni.first_run = 1;

    StudioXIni.xpos = 20;
    StudioXIni.ypos = 20;
    StudioXIni.width = 1100;
    StudioXIni.height = 640;
    StudioXIni.proj_view_width = 200;
    StudioXIni.resource_view_width = 200;
    StudioXIni.proj_view_height = 240;
    StudioXIni.samples_dir = "";

    for (loop = 0; loop < MAX_RECENT_PROJECTS; loop++)
    {
        StudioXIni.recent_project_paths[loop] = "";
    }
    
    CString inipath = GetAppDataPath() + "\\" + INI_FILE_NAME;
    xml_reader reader;
    
    if (reader.ReadFile(inipath))
    {
        FoundIni = TRUE;

        if (reader.EnterSection("information"))
        {
            reader.ReadInt("ini_version", StudioXIni.ini_version, 1);
            reader.ReadInt("studio_version", StudioXIni.studio_version, 0);
            reader.ReadString("samples_dir", StudioXIni.samples_dir);
            reader.CloseSection(TRUE, TRUE);
        }

        // KGM we can add logic here to detect older Studio version and older
        // INI version, if and when we need to. Then, reset version information
        // to current version.

        if (StudioXIni.ini_version == STUDIO_INI_VERSION &&
            StudioXIni.studio_version == CalculateStudioVersion())
        {
            StudioXIni.first_run = 0;
        }

        StudioXIni.ini_version = STUDIO_INI_VERSION;
        StudioXIni.studio_version = CalculateStudioVersion();

        if (reader.EnterSection("dimensions"))
        {
            reader.ReadInt("xpos", StudioXIni.xpos, 20);
            reader.ReadInt("ypos", StudioXIni.ypos, 20);
            reader.ReadInt("width", StudioXIni.width, 1100);
            reader.ReadInt("height", StudioXIni.height, 640);
            reader.ReadInt("project_width", StudioXIni.proj_view_width, 200);
            reader.ReadInt("resource_width", StudioXIni.resource_view_width, 200);
            reader.ReadInt("project_height", StudioXIni.proj_view_height, 240);
            reader.CloseSection(TRUE, TRUE);
        }
        if (StudioXIni.width < 0)
        {
            StudioXIni.width = 1100;
        }
        if (StudioXIni.height < 0)
        {
            StudioXIni.height = 640;
        }
        if (StudioXIni.proj_view_width < 0)
        {
            StudioXIni.proj_view_width = 200;
        }
        if (StudioXIni.resource_view_width < 0)
        {
            StudioXIni.resource_view_width = 200;
        }
        if (StudioXIni.proj_view_height < 0)
        {
            StudioXIni.proj_view_height = 240;
        }

        if (reader.EnterSection("recent_projects"))
        {
            for (loop = 0; loop < MAX_RECENT_PROJECTS; loop++)
            {
                reader.ReadString("pathname", StudioXIni.recent_project_paths[loop]);
            }
            reader.CloseSection();
        }
    }

    // Check to make sure the initial position is on the screen:
    int size = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    if (StudioXIni.xpos < 0 || StudioXIni.xpos >= size - 10)
    {
        StudioXIni.xpos = 20;
    }

    size = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (StudioXIni.ypos < 0 || StudioXIni.ypos >= size - 10)
    {
        StudioXIni.ypos = 20;
    }
    return FoundIni;
}

///////////////////////////////////////////////////////////////////////////////
void WriteIniInfo(void)
{
    int loop;
    xml_writer writer;

    // Let's try to read the config file:

    CString inipath = GetAppDataPath() + "\\" + INI_FILE_NAME;

    StudioXIni.ini_version = STUDIO_INI_VERSION;
    StudioXIni.studio_version = CalculateStudioVersion();
    
    if (writer.OpenFile(inipath))
    {
        writer.OpenTag("information");
        writer.WriteInt("ini_version", StudioXIni.ini_version);
        writer.WriteInt("studio_version", StudioXIni.studio_version);
        writer.WriteString("samples_dir", StudioXIni.samples_dir);
        writer.CloseTag("information");

        writer.OpenTag("dimensions");
        writer.WriteInt("xpos", StudioXIni.xpos);
        writer.WriteInt("ypos", StudioXIni.ypos);
        writer.WriteInt("width", StudioXIni.width);
        writer.WriteInt("height", StudioXIni.height);
        writer.WriteInt("project_width", StudioXIni.proj_view_width);
        writer.WriteInt("resource_width", StudioXIni.resource_view_width);
        writer.WriteInt("project_height", StudioXIni.proj_view_height);
        writer.CloseTag("dimensions");

        writer.OpenTag("recent_projects");

        for (loop = 0; loop < MAX_RECENT_PROJECTS; loop++)
        {
            if (StudioXIni.recent_project_paths[loop].IsEmpty())
            {
                break;
            }
            else
            {
                writer.WriteString("pathname", StudioXIni.recent_project_paths[loop]);
            }
        }
        writer.CloseTag("recent_projects");
        writer.CloseFile();
    }
}

#include "winerror.h"

///////////////////////////////////////////////////////////////////////////////
CString GetSamplesDir()
{
    TCHAR module_path[MAX_PATH];
    CString pathname;

    if (StudioXIni.samples_dir.IsEmpty())
    {
        // test to see if we are running a local test instance:
        if (GetModuleFileName(NULL, module_path, MAX_PATH))
        {
            pathname = module_path;
            if (pathname.Find(_T("WindowsApps"), 0) == -1)
            {
                // Not installed from App Store, so running local test.
                // Added this for testing, version test might needed here.
                return _T("C:\\Eclipse_ThreadX\\GUIX_Studio_test");
            }
        }
    }
    return StudioXIni.samples_dir;
}

///////////////////////////////////////////////////////////////////////////////
CString GetStudioInstallFolder()
{
    CString root_path;
    TCHAR module_path[MAX_PATH];

    if (GetModuleFileName(NULL, module_path, MAX_PATH))
    {
        root_path = module_path;
        #if defined(_DEBUG)
        root_path = root_path.Left(root_path.Find(_T("\\Debug"), 0));
        #else
        if (root_path.Find(_T("\\WindowsApps") > 0))
        {
            // here if installed from App Store
            root_path = root_path.Left(root_path.Find(_T("\\studiox"), 0));
        }
        else
        {
            // here if running local release mode build
            root_path = root_path.Left(root_path.Find(_T("\\Release"), 0));
        }
        #endif
    }    
    return root_path;
}

///////////////////////////////////////////////////////////////////////////////
CString GetTestingParam(int index)
{
    static char *fparam0 = "c:\\temp\\guix_param0.txt";
    static char *fparam1 = "c:\\temp\\guix_param1.txt";

    CString val("");
    size_t size;
    char param[MAX_PATH];
    char *filename;
    FILE *file;
    
    memset(param, 0, MAX_PATH);

    if (index)
    {
        filename = fparam1;
    }
    else
    {
        filename = fparam0;
    }
 
    file = fopen(filename, "r");

    if (file)
    {
        size = fread(param, 1, MAX_PATH, file);

        if (size)
        {
            val = CString(param);
        }
        fclose(file);
    }
    return val;
}

///////////////////////////////////////////////////////////////////////////////
CString GetMallocName()
{
    CString val("");

    if (gpOpenProject)
    {
        val = gpOpenProject->mHeader.malloc_name;
    }
    return val;
}

///////////////////////////////////////////////////////////////////////////////
void MoveRecentPathToFront(int index)
{
    int loop;
    CString temp;

    temp = StudioXIni.recent_project_paths[index];

    for (loop = index; loop > 0; loop--)
    {
        StudioXIni.recent_project_paths[loop] = 
            StudioXIni.recent_project_paths[loop - 1];
    }
    StudioXIni.recent_project_paths[0] = temp;
}


///////////////////////////////////////////////////////////////////////////////
void AddRecentProject(CString &pathname)
{
    int loop;

    BOOL path_exist = FALSE;

    // if this is already in my list, just move it to the front:

    for (loop = 0; loop < MAX_RECENT_PROJECTS; loop++)
    {
        if (StudioXIni.recent_project_paths[loop] == pathname)
        {
            if (loop == 0)
            {
                return;
            }
            path_exist = TRUE;
            MoveRecentPathToFront(loop);
            break;
        }
    }

    // the path doesn't exist, move everything down by 1 and put in front:
    if (!path_exist)
    {
        for (loop = MAX_RECENT_PROJECTS - 1; loop > 0; loop--)
        {
            StudioXIni.recent_project_paths[loop] =
                StudioXIni.recent_project_paths[loop - 1];
        }
        StudioXIni.recent_project_paths[0] = pathname;
    }

    // update recent projects menu
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    pMain->UpdateRecentProjectsMenu();
}

///////////////////////////////////////////////////////////////////////////////
project_view *GetProjectView()
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    return pMain->GetProjectView();
}

///////////////////////////////////////////////////////////////////////////////
target_view *GetTargetView()
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    return pMain->GetTargetView();
}

///////////////////////////////////////////////////////////////////////////////
target_screen *GetTargetScreen()
{
    return (GetTargetView()->GetTargetScreen());
}

///////////////////////////////////////////////////////////////////////////////
resource_view *GetResourceView()
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    return pMain->GetResourceView();
}

///////////////////////////////////////////////////////////////////////////////
properties_win *GetPropsWin()
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    return pMain->GetPropsWin();
}

///////////////////////////////////////////////////////////////////////////////
CCommandInfo *GetCmdInfo()
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    return pMain->GetCmdInfo();
}

///////////////////////////////////////////////////////////////////////////////
undo_manager *UndoManager()
{
    CMainFrame *pMain = (CMainFrame *)AfxGetApp()->GetMainWnd();
    return pMain->GetUndoManager();
}

///////////////////////////////////////////////////////////////////////////////
string_table *GetActiveStringTable()
{
    string_table *table = NULL;

    if (gpOpenProject)
    {
        int display = GetProjectView()->GetActiveDisplay();

        if (display >= 0 && display < gpOpenProject->mHeader.max_displays)
        {
            table = gpOpenProject->mDisplays[display].stable;
        }
    }
    return table;
}

///////////////////////////////////////////////////////////////////////////////
FontCharMap *GetActiveCharacterMap(BOOL reset)
{
    string_table *table = GetActiveStringTable();
    if (table)
    {
        if (reset)
        {
            table->GenerateCleanCharacterMap();
        }
        return table->GetCharacterMap();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
GX_COLOR ColorRefToGxColor(COLORREF cr)
{
    GX_COLOR val;
    BYTE red = GetRValue(cr);
    BYTE green = GetGValue(cr);
    BYTE blue = GetBValue(cr);

    val = red;
    val <<= 8;
    val |= green;
    val <<= 8;
    val |= blue;
    return val;
}

 
///////////////////////////////////////////////////////////////////////////////
COLORREF GxColorToColorRef(GX_COLOR gx)
{
    BYTE red =   (BYTE) ((gx >> 16) & 0xff);
    BYTE green = (BYTE) ((gx >> 8) & 0xff);
    BYTE blue =  (BYTE) (gx & 0xff);

    return RGB(red, green, blue);
}

/*****************************************************************************/
BOOL IsAlphaFormat(int color_format)
{
    if (color_format == GX_COLOR_FORMAT_4444ARGB ||
        color_format == GX_COLOR_FORMAT_32ARGB ||
        color_format == GX_COLOR_FORMAT_8BIT_ALPHAMAP)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
BOOL IsCpuWithDave2D(int cpu_type) // does the CPU support Dave2D?
{
    if (cpu_type == CPU_SYNERGY ||
        cpu_type == CPU_RA ||
        cpu_type == CPU_RX)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
BOOL IsCpuWithDave2D(studiox_project *project)                        // does the CPU support Dave2D?
{
    return IsCpuWithDave2D(project->mHeader.target_cpu);
}

///////////////////////////////////////////////////////////////////////////////
BOOL IsRenesasDave2D(studiox_project *project)      // Is the project configured with Dave2D enabled?
{
    if (project)
    {
        if (IsCpuWithDave2D(project))
        {
            if (project->mHeader.dave2d_graph_accelerator)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL IsDave2dFontFormat(studiox_project* project, int display)
{
    if (project && IsRenesasDave2D(project))
    {
        switch (project->mDisplays[display].colorformat)
        {
        case GX_COLOR_FORMAT_8BIT_PALETTE:
            if (project_lib_version() >= GX_VERSION_SYNERGY_GLYPH_GEN_CHANGE)
            {
                // Generate GUIX version glyph data for 8bpp palette mode.
                return FALSE;
            }
            break;
        }

        return TRUE;
    }

    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
BOOL IsSTChromeArt(studiox_project *project)
{
    if (project && (project->mHeader.target_cpu == CPU_ST_CHROMEART))
    {
        return TRUE;
    }

    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
BOOL IsRenesasHwJpeg(studiox_project *project)
{
    if (project->mHeader.target_cpu == CPU_SYNERGY ||
        project->mHeader.target_cpu == CPU_RA)
    {
        if (project->mHeader.renesas_jpeg_decoder == DECODER_TYPE_HW)
        {
            return TRUE;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void pixelmap_destroy(GX_PIXELMAP *map)
{
    if (map->gx_pixelmap_data)
    {
        delete [] map->gx_pixelmap_data;
        map->gx_pixelmap_data = NULL;
    }
    if (map->gx_pixelmap_aux_data)
    {
        // The pixelmap aux data can point at a shared palette, but in that case
        // we give each pixelmap it's own copy of the shared palette, so we don't
        // run into issues of deleting the same color array multiple times.
        delete map->gx_pixelmap_aux_data;
        map->gx_pixelmap_aux_data = NULL;
    }
    delete map;
}

///////////////////////////////////////////////////////////////////////////////
void pixelmap_list_destroy(CArray<GX_PIXELMAP *> &pixelmap_list)
{
    GX_PIXELMAP *map;

    for (int index = 0; index < pixelmap_list.GetCount(); index++)
    {
        map = pixelmap_list.GetAt(index);

        if (map)
        {
            pixelmap_destroy(map);
        }
    }

    pixelmap_list.RemoveAll();
}


///////////////////////////////////////////////////////////////////////////////
void PaintBmp(CDC *dc, int x, int y, int icon_id)
{
    CBitmap map;
    BITMAP  bmp;
    
    if (map.LoadBitmap(icon_id))
    {
        map.GetBitmap(&bmp);
    
        CDC dcMemory;
        dcMemory.CreateCompatibleDC(dc);
        dcMemory.SelectObject(&map);

        int dpi = GetSystemDPI();
        int width = MulDiv(bmp.bmWidth, dpi, DEFAULT_DPI_96);
        int height = MulDiv(bmp.bmHeight, dpi, DEFAULT_DPI_96);
        dc->StretchBlt(x, y, width, height, &dcMemory,0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    }
}

///////////////////////////////////////////////////////////////////////////////
FontCharMap::FontCharMap()
{
    map_size = 8192;
    map = new unsigned char[8192];
    Clear();
}


///////////////////////////////////////////////////////////////////////////////
FontCharMap::~FontCharMap()
{
    delete [] map;
}

///////////////////////////////////////////////////////////////////////////////
BOOL FontCharMap::Test(DWORD val)
{
    DWORD index = val >> 3;
    unsigned char mask = 0x80 >> (val & 7);

    if (index < map_size)
    {
        if (map[index] & mask)
        {
            return TRUE;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL FontCharMap::Test(DWORD start, DWORD end)
{
    while(start <= end)
    {
        if (Test(start))
        {
            return TRUE;
        }
        start++;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void FontCharMap::Set(DWORD val)
{
    DWORD index = val >> 3;
    if (index < map_size)
    {
        unsigned char mask = 0x80 >> (val & 7);
        map[index] |= mask;
    }
}

///////////////////////////////////////////////////////////////////////////////
void FontCharMap::Set(DWORD start, DWORD end)
{
    while(start <= end)
    {
        Set(start);
        start++;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL FontCharMap::TestAndSet(DWORD val)
{
    if (Test(val))
    {
        return FALSE;
    }
    Set(val);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void FontCharMap::SetMapSize(ULONG size)
{
    if (map_size != size)
    {
        if (map)
        {
            delete[] map;
        }

        map_size = size;
        map = new unsigned char[map_size];
        Clear();
    }
}

///////////////////////////////////////////////////////////////////////////////
void FontCharMap::Clear()
{
    memset(map, 0, map_size);
}

///////////////////////////////////////////////////////////////////////////////
BOOL FontCharMap::AddToMap(CString &val)
{
    BOOL new_char = FALSE;
    if (val.IsEmpty())
    {
        return FALSE;
    }

#ifdef _UNICODE
    wchar_t *wide_text = (wchar_t *)val.GetString();
#else
    wchar_t *wide_text = CStringToWideChar(val);
#endif
    
    if (wide_text)
    {
        wchar_t *get = wide_text;
        DWORD unicode;

        while(*get)
        {
            if (((*get) >= 0xd800) && ((*get) <= 0xdbff))
            {
                //Unicode from 0x10000 to 0x10ffff are encoded as two 16-bit code units called surrogate pairs
                //recombine the surrogate pairs to get a 4 byte unicode
                unicode = (*get) & (~0xd800);
                unicode <<= 10;
                unicode += 0x10000;

                get++;
                unicode += (*get) & (~0xdc00);

                if (map_size != CHARMAP_SIZE_EXTENDED)
                {
                    unsigned char *new_map = new unsigned char[CHARMAP_SIZE_EXTENDED];
                    memset(new_map, 0, CHARMAP_SIZE_EXTENDED);
                    memcpy_s(new_map, CHARMAP_SIZE_EXTENDED, map, CHARMAP_SIZE_DEFAULT);
                    delete[]map;
                    map = new_map;
                    map_size = CHARMAP_SIZE_EXTENDED;
                }
            }
            else
            {
                unicode = *get;
            }

            if ((unicode >= 0x20) && TestAndSet(unicode))
            {
                new_char = TRUE;
            }
            get++;   
        }
#ifndef _UNICODE
        delete [] wide_text;
#endif
    }
    return new_char;
}

///////////////////////////////////////////////////////////////////////////////
void FontCharMap::Overlay(FontCharMap *other)
{
    DWORD val;
    DWORD maxval = GetMapSize() << 3;

    if (!other)
    {
        return;
    }

    for (val = 0; val < maxval; val++)
    {
        if (other->Test(val))
        {
            Set(val);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL FontCharMap::IsEmpty()
{
    DWORD val;
    DWORD maxval = GetMapSize() << 3;

    for (val = 0; val < maxval; val++)
    {
        if (Test(val))
        {
            return FALSE;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
CString NumberFormatWithCommas(long value)
{
    // Format a number with commas.
    CString str;
    int insert_pos;

    str.Format(_T("%d"), value);

    insert_pos = str.GetLength() - 3;
    value /= 1000;

    while (value)
    {
        str.Insert(insert_pos, _T(","));
        value /= 1000;
        insert_pos -= 4;
    }

    return str;
}

///////////////////////////////////////////////////////////////////////////////
VOID FormatPath(CString &path)
{

    path.Replace('/', '\\');
    path = path.TrimLeft(_T("\\"));
}

///////////////////////////////////////////////////////////////////////////////
/*Check file exists or not.*/
BOOL FileExists(INT pathtype, CString pathname)
{
    CString file = pathname;
    PATHINFO path_info;
    path_info.pathname = pathname;

    switch (pathtype)
    {
    case PATH_TYPE_INSTALL_RELATIVE:
    case PATH_TYPE_PROJECT_RELATIVE:
        path_info.pathtype = pathtype;
        file = MakeAbsolutePathname(path_info);
        break;

    case PATH_TYPE_ABSOLUTE:
    default:
        break;
    }

    if (GetFileAttributes(file) == INVALID_FILE_ATTRIBUTES)
    {
        /*file does not exists*/
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
BOOL FileExists(INT display_index, INT resource_type, CString pathname)
{
    /*We should check the name in whole group, not just in current folder.
    So we must find the group forst.*/
    INT group_id;
    res_info* group_info = NULL;

    switch (resource_type)
    {
    case RES_TYPE_COLOR:
        group_id = COLOR_GROUP;
        break;

    case RES_TYPE_FONT:
        group_id = FONT_GROUP;
        break;

    case RES_TYPE_PIXELMAP:
        group_id = PIXELMAP_GROUP;
        break;

    case RES_TYPE_STRING:
        group_id = STRING_GROUP;
        break;

    default:
        return FALSE;
    }

    studiox_project *project = GetOpenProject();
    int active_theme = project->mDisplays[display_index].active_theme;
    group_info = project->FindResource(display_index, active_theme, RES_TYPE_GROUP, group_id);

    // Detemin whether the specified file exist 
    CString str;
    res_info *child_folder = group_info->child;
    res_info* child = child_folder->child;

    FormatPath(pathname);
    ConvertToProjectRelativePath(pathname);
    while (child_folder && (child_folder->type == RES_TYPE_FOLDER))
    {
        child = child_folder->child;
        while (child)
        {
            if (child->type == resource_type)
            {
                str = MakeAbsolutePathname(child->pathinfo);
                ConvertToProjectRelativePath(str);
                FormatPath(str);

                if (_tcscmp(str, pathname) == 0)
                {
                    return TRUE;
                }
            }

            child = child->next;
        }
        child_folder = child_folder->next;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL NameExists(INT display_index, INT resource_type, CString name)
{
    /*We should check the name in whole group, not just in current folder.
    So we must find the group forst.*/
    /*We want use this function for all name check. But font doesn't be added to its parent group while
    do this check. So we need to find the group.*/
    INT group_id;
    res_info* group_info = NULL;

    switch (resource_type)
    {
    case RES_TYPE_COLOR:
        group_id = COLOR_GROUP;
        break;

    case RES_TYPE_FONT:
        group_id = FONT_GROUP;
        break;

    case RES_TYPE_PIXELMAP:
        group_id = PIXELMAP_GROUP;
        break;

    case RES_TYPE_STRING:
        group_id = STRING_GROUP;
        break;

    default:
        return FALSE;
    }

    studiox_project *project = GetOpenProject();
    int active_theme = project->mDisplays[display_index].active_theme;
    group_info = project->FindResource(display_index, active_theme, RES_TYPE_GROUP, group_id);

    if (group_info == NULL)
    {
        return FALSE;
    }

    res_info *child_folder = group_info -> child;
    res_info* child = child_folder -> child;
    CString child_name;
    CString name_suffix;
    int size;

    name.MakeUpper();

    /*Traverse all folder to compare the name.*/
    while (child_folder && (child_folder->type == RES_TYPE_FOLDER))
    {
        child = child_folder->child;
        while (child)
        {

            if (child->type == resource_type)
            {
                if ((resource_type == RES_TYPE_PIXELMAP) && (child->map_list.GetCount() > 1))
                {
                    // [resource name][_F][numbers] name pattern is reserved for frame names of multi-frame image.

                    child_name.Format(_T("%s_F"), child->name);
                    child_name.MakeUpper();
                    size = child_name.GetLength();
                    if (name.GetLength() > child_name.GetLength())
                    {
                        name_suffix = name.Mid(size);

                        if ((name.Left(size) == child_name) && (name_suffix.SpanIncluding(L"0123456789") == name_suffix))
                        {
                            return TRUE;
                        }
                    }
                }

                child_name = child->name;
                child_name.MakeUpper();

                if (_tcscmp(child_name, name) == 0)
                {
                    return TRUE;
                }
            }

            child = child->next;
        }
        child_folder = child_folder->next;
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
INT GetSubMenuIndex(CMenu *pMenu, CString string)
{
    // find a submenu according to string
    CString get_string;

    for (int index = 0; index < pMenu->GetMenuItemCount(); index++)
    {
        pMenu->GetMenuString(index, get_string, MF_BYPOSITION);
        if (!get_string.Compare(string))
        {
            return index;
        }
    }

    return -1;
}

///////////////////////////////////////////////////////////////////////////////
INT GetSubMenuIndex(CMenu *pMenu, INT id)
{
    for (int index = 0; index < pMenu->GetMenuItemCount(); index++)
    {
        if (pMenu->GetMenuItemID(index) == id)
        {
            return index;
        }
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////////////
void SplitString(CString str, CHAR splitter, CStringArray *list)
{
    int index = str.Find(splitter);

    while (index >= 0)
    {
        list->Add(str.Left(index));

        str = str.Mid(index + 1);
        index = str.Find(splitter);
    }

    list->Add(str);
}

/////////////////////////////////////////////////////////////////////////////
int GetSystemDPI()
{
    // get DPI for the system
    HDC hdc = ::GetDC(NULL);

    int dpi = GetDeviceCaps(hdc, LOGPIXELSX);

    ::ReleaseDC(NULL, hdc);

    return dpi;
}

//////////////////////////////////////////////////////////////////////////////
int GuixVersionFieldsToVersionNumber(int major, int minor, int service_pack)
{
    int version_number = (major * 10000) + (minor * 100) + service_pack;
    return version_number;
}

void GuixVersionNumberToVersionFields(int version_number, int &major, int &minor, int &service_pack)
{
    major = version_number / 10000;
    version_number -= major * 10000;
    minor = version_number / 100;
    version_number -= minor * 100;
    service_pack = version_number;
}

//////////////////////////////////////////////////////////////////////////////
/*Return false when the check doesn't pass.*/
BOOL TestInputName(CString &input_string, char *field_name, CWnd *parent, BOOL show_error_message)
{
    INT index = 1;
    TCHAR temp_char;
    char msg[256];

    if (input_string.IsEmpty())
    {
        if (show_error_message)
        {
            sprintf_s(msg, sizeof(msg), "The \"%s\" field cannot be empty.", field_name);
            Notify(msg, parent);
        }
        return false;
    }

    BOOL meet_naming_rule = TRUE;

    temp_char = input_string[0];

    /*Input name must start with alpha or underline.*/

    if (!(isalpha(temp_char)) && (temp_char != '_'))
    {
        meet_naming_rule = FALSE;
    }
    else
    {
        /*Component ID must be consisted of liters, number and '_'.*/
        while (input_string[index])
        {
            temp_char = input_string.GetAt(index);

            /*Is this character a num? liter? or '_'? */
            if ((temp_char >= 0) && (temp_char <= 255) && (isalnum(temp_char) || (temp_char == '_')))
            {
                index++;
            }
            else
            {
                meet_naming_rule = FALSE;
                break;;
            }
        }
    }

    if (!meet_naming_rule)
    {
        if (show_error_message)
        {
            sprintf_s(msg, sizeof(msg), "The \"%s\" field must meet ANSI C variable naming rules.", field_name);
            Notify(msg, parent);
        }
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////
/*Return false when the check doesn't pass.*/
BOOL TestInputName(CEdit *input_field, char *field_name, CString &original_string, CWnd *parent)
{
    CString input_string;

    input_field -> GetWindowText(input_string);

    if(TestInputName(input_string, field_name, parent))
    {
        original_string = input_string;
        return TRUE;   
    }
    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
/* This function just check the import pixelmap name for now. */
BOOL SetImportIdName(INT display_index, CString &input_name)
{
    TCHAR temp_char;
    INT   index = 1;
    CString output_name = input_name;
    CHAR temp_str[10];
    BOOL status = TRUE;

    if (output_name.IsEmpty())
    {
        Notify("The name is empty, please check.");
        return FALSE;
    }

    temp_char = input_name.GetAt(0);
    if (!isalpha(temp_char) && (temp_char != '_'))
    {
        /*If filename isn't started with alpha or underline. Force the first character to be underline.*/
        input_name.SetAt(0, '_');
        status = FALSE;
    }

    while (input_name[index])
    {
        temp_char = input_name.GetAt(index);
        /*Is this character a num? liter? or '_'? */
        if (!isalnum(temp_char) && (temp_char != '_'))
        {
            input_name.SetAt(index, '_');
            status = FALSE;
        }
        index++;
    }

    /*User may add same name file for different purpose, or different files have the same name.
      So set them different Id name.*/
    index = 1;
    output_name = input_name;
    while (NameExists(display_index, RES_TYPE_PIXELMAP, output_name))
    {
        sprintf_s(temp_str, sizeof(temp_str), "_%d", index++);
        output_name = input_name + CString(temp_str);
    }

    if (input_name != output_name)
    {
        input_name = output_name;
        /*I don't know whether the message should be shown or not. So comment it now.*/
        /*
        CString temp_msg("Image ID already exists. It will be set as ");
        temp_msg += input_name;
        Notify(CW2A(temp_msg) + ".");
        */
        status = FALSE;
    }

    return status;
}


//////////////////////////////////////////////////////////////////////////////
/* Check file name .*/
BOOL IsFileNameFormat(CString filename)
{
    int index = 0;
    TCHAR temp_char;

    if (filename.IsEmpty())
    {
        return TRUE;
    }

    /*Component ID must be consisted of liters, number and '_'.*/
    while (filename[index])
    {
        temp_char = filename.GetAt(index);
        /*Is this character a num? liter? blank space? or '_'? */
        if (isalnum(temp_char) || (temp_char == '_') || (temp_char == ' ') || (temp_char == '.'))
        {
            index++;
        }
        else
        {
            Notify("The output name should match file naming conventions.");
            return FALSE;
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CString MakePixelmapName(res_info* info, int res_index)
{
    CString name;

    if (info->map_list.GetCount() > 1)
    {
        name.Format(_T("%s_F%d"), info->name, res_index);
    }
    else
    {
        name = info->name;
    }

    return name;
}

////////////////////////////////////////////////////////////////////////////////////////////////
ULONG GetTextScaler()
{
    DWORD type, size;
    ULONG scaler = DEFAULT_TEXT_SCALER;

    HKEY key;
    RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Accessibility", 0, READ_CONTROL | KEY_QUERY_VALUE, &key);

    if (key)
    {
        size = sizeof(ULONG);
        RegQueryValueEx(key, L"TextScaleFactor", 0, &type, (BYTE*)&scaler, &size);
        RegCloseKey(key);
    }

    return scaler;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
int GetScaledValue(int value, int dpi, int text_scaler)
{
    // Scale the input value according to dpi and text scaler.
    value = MulDiv(value, dpi, DEFAULT_DPI_96);
    value = MulDiv(value, text_scaler, DEFAULT_TEXT_SCALER);

    return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////
GX_UBYTE ToUByte(INT value)
{
    if (value > 255)
    {
        return 255;
    }
    else if (value < 0)
    {
        return 0;
    }

    return value;
}

///////////////////////////////////////////////////////////////////////////////
int FindPairVal(STRING_VAL_PAIR *entry, CString name)
{
    while (!entry->name.IsEmpty())
    {
        if (entry->name == name)
        {
            return entry->val;
        }

        entry++;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
CString FindPairString(STRING_VAL_PAIR *entry, int val)
{
    while (!entry->name.IsEmpty())
    {
        if (entry->val == val)
        {
            return entry->name;
        }
        entry++;
    }

    return L"";
}