// color_edit_dialog.cpp : implementation file
//

#include "studiox_includes.h"
#include "install_dir_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR default_samples_path[] = _T("C:\\Azure_RTOS\\GUIX-Studio-6.1");

BEGIN_MESSAGE_MAP(install_dir_dialog, express_dialog)
    ON_BN_CLICKED(IDC_BROWSE, OnSetDirectory)
    ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(install_dir_dialog, express_dialog)

extern INI_INFO StudioXIni;

///////////////////////////////////////////////////////////////////////////////
install_dir_dialog::install_dir_dialog(CWnd *parent)
    : express_dialog(install_dir_dialog::IDD, parent)
{
    IconId = IDB_WARNING;
    SetTitleText(CString("Select GUIX Studio Example Location"));

    StudioXIni.samples_dir = default_samples_path;

    /* Configure an initial, default "recent project" */
}

///////////////////////////////////////////////////////////////////////////////
BOOL install_dir_dialog::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    //AddCancelButton();
    AddSaveButton(CString("Continue"));

    CStatic* path_view = (CStatic*)GetDlgItem(IDC_INSTALL_PATH);

    if (path_view)
    {
        path_view->SetWindowText(default_samples_path);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
void install_dir_dialog::OnSetDirectory()
{
    TCHAR path[MAX_PATH];
    CString test_project;

    if (BrowseForFolder(this->GetSafeHwnd(), _T("Select install directory.."), default_samples_path, path))
    {
        CStatic *path_view = (CStatic*)GetDlgItem(IDC_INSTALL_PATH);

        if (path_view)
        {
            path_view->SetWindowText(path);
        }
        StudioXIni.samples_dir = path;
        StudioXIni.samples_dir.MakeLower();
    }
}

///////////////////////////////////////////////////////////////////////////////
void install_dir_dialog::CopyInstalledObjects()
{
    // if no .ini file, this is our first run. Copy resources and sample project from package
    // to installation directory

    WCHAR CopyFrom[255] = _T("Hello");
    CString install_folder = GetStudioInstallFolder();
    TCHAR CopyTo[255];
    SHFILEOPSTRUCT shfile;
    int iRes;

    // copy default project to app data folder
    wcscpy(CopyFrom, install_folder);

#if defined(_DEBUG)
    wcscat(CopyFrom, _T("\\studio_package_project\\samples\\demo_guix_simple\\*.*|"));
#else
    wcscat(CopyFrom, _T("\\samples\\demo_guix_simple\\*.*|"));
#endif

    // Replace the '|' sign with null to get double-terminated string

    while (TCHAR* ptr = _tcsrchr(CopyFrom, _T('|'))) {
        *ptr = _T('\0');
    }

    wcscpy(CopyTo, GetSamplesDir());
    wcscat(CopyTo, _T("\\sample\\demo_guix_simple"));

    // Fill the SHFILEOPSTRUCT struct
    shfile.hwnd = NULL;
    shfile.wFunc = FO_COPY;
    shfile.pFrom = CopyFrom;
    shfile.pTo = CopyTo;
    shfile.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

    // Copy the eample project
    iRes = SHFileOperation(&shfile);
}

///////////////////////////////////////////////////////////////////////////////
void install_dir_dialog::ConfigureDefaultSamplesDir(void)
{
    // Copy package resources to install dir

    CString samples_dir = GetSamplesDir();

    // Make sure the directory exists
    MakePath(samples_dir);
    CopyInstalledObjects();

    samples_dir = GetSamplesDir();
    samples_dir += "\\sample";
    samples_dir.MakeLower();
    samples_dir += _T("\\demo_guix_simple\\guix_simple.gxp");
    AddRecentProject(samples_dir);
}


///////////////////////////////////////////////////////////////////////////////
void install_dir_dialog::OnOK()
{
    if (!GetSamplesDir().IsEmpty())
    {
        ConfigureDefaultSamplesDir();
    }
    CDialog::OnOK();
}