// NewProjectDlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "NewProjectDlg.h"
#include "express_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NEW_PROJECT_WIDTH 600
#define NEW_PROJECT_HEIGHT 320

// NewProjectDlg dialog

//IMPLEMENT_DYNAMIC(NewProjectDlg, CDialog)
enum new_project_dlg_test_commands{
    TEST_SET_NEW_PROJECT_PATH = 1,
    TEST_SET_NEW_PROJECT_NAME,
    TEST_SAVE_NEW_PROJECT_CREATE,
    TEST_CANCEL_NEW_PROJECT_CREATE,
};

BEGIN_MESSAGE_MAP(NewProjectDlg, express_dialog)
    ON_BN_CLICKED(IDC_BROWSE_PATH, &NewProjectDlg::OnBnClickedBrowsePath)
    ON_BN_CLICKED(IDCANCEL, &NewProjectDlg::OnCancel)
    ON_WM_SHOWWINDOW()
    ON_WM_PAINT()
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

NewProjectDlg::NewProjectDlg(CWnd* pParent /*=NULL*/)
	: express_dialog(NewProjectDlg::IDD, pParent)
{
    IconId = IDB_NEW_PROJECT_ICON;
    SetTitleText("Create New Project");
}

NewProjectDlg::~NewProjectDlg()
{
}

void NewProjectDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CRect size;
    GetWindowRect(&size);

    size.OffsetRect(0, -120);
    MoveWindow(size);
    express_dialog::OnShowWindow(bShow, nStatus);
}

BOOL NewProjectDlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    AddCancelButton();
    AddSaveButton();

    CString description(L"Required");
    SetAccessibleFullDescription(GetDlgItem(IDC_EDIT_NAME)->GetSafeHwnd(), description);
    SetAccessibleFullDescription(GetDlgItem(IDC_EDIT_PATH)->GetSafeHwnd(), description);
    SetAccessibleDescription(GetDlgItem(IDC_EDIT_NAME)->GetSafeHwnd(), description);
    SetAccessibleDescription(GetDlgItem(IDC_EDIT_PATH)->GetSafeHwnd(), description);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void NewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BOOL NewProjectDlg::DestroyWindow()
{
    GetDlgItemText(IDC_EDIT_NAME, mproject_name);
    GetDlgItemText(IDC_EDIT_PATH, mproject_path);
    return CDialog::DestroyWindow();
}

CString &NewProjectDlg::Getproject_name()
{
    return mproject_name;
}
CString &NewProjectDlg::Getproject_path()
{
    return mproject_path;
}
void NewProjectDlg::OnOK()
{
    GetDlgItemText(IDC_EDIT_NAME, mproject_name);
    GetDlgItemText(IDC_EDIT_PATH, mproject_path);

    if (mproject_name.IsEmpty())
    {
        ErrorMsg(L"The project name cannot be empty.", this);
        return;
    }

    if (mproject_path.IsEmpty())
    {
        ErrorMsg(L"The project path cannot be empty.", this);
        return;
    }

    CDialog::OnOK();
}

void NewProjectDlg::OnCancel()
{
    CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
void NewProjectDlg::OnPaint()
{
    CDialog::OnPaint();

    PaintRequiredAsterisk(IDC_EDIT_NAME);
    PaintRequiredAsterisk(IDC_EDIT_PATH);
    PaintRequiredAsterisk(IDC_REQUIRED_FIELD_LEGEND);
}



// NewProjectDlg message handlers

void NewProjectDlg::OnBnClickedBrowsePath()
{
    TCHAR path[MAX_PATH];
    if (BrowseForFolder(m_hWnd, NULL, NULL, path))
    {
        SetDlgItemText(IDC_EDIT_PATH, path);
    }
}

afx_msg LRESULT NewProjectDlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_SET_NEW_PROJECT_PATH:
        SetDlgItemText(IDC_EDIT_PATH, GetTestingParam(0));
        break;

    case TEST_SET_NEW_PROJECT_NAME:
        SetDlgItemText(IDC_EDIT_NAME, GetTestingParam(0));
        break;

    case TEST_SAVE_NEW_PROJECT_CREATE:
        OnOK();
        break;

    case TEST_CANCEL_NEW_PROJECT_CREATE:
        OnCancel();
    }

    return 0;
}