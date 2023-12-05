#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MacroRecordDlg, express_dialog)
    ON_BN_CLICKED(IDC_BROWSE_PATH, &MacroRecordDlg::OnBnClickedBrowsePath)
    ON_BN_CLICKED(IDCANCEL, &MacroRecordDlg::OnCancel)
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

MacroRecordDlg::MacroRecordDlg(CWnd* pParent)
	: express_dialog(MacroRecordDlg::IDD, pParent)
{
    IconId = IDB_MACRO_RECORD_ICON;
    SetTitleText("Record Macro");
}

MacroRecordDlg::~MacroRecordDlg()
{
}

void MacroRecordDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CRect size;
    GetWindowRect(&size);

    size.OffsetRect(0, -120);
    MoveWindow(size);
    CDialog::OnShowWindow(bShow, nStatus);
}

BOOL MacroRecordDlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    AddCancelButton();
    AddSaveButton(_T("Record"));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void MacroRecordDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

void MacroRecordDlg::OnOK()
{
    GetDlgItemText(IDC_EDIT_NAME, mmacro_name);
    GetDlgItemText(IDC_EDIT_PATH, mmacro_path);

    if (mmacro_name.IsEmpty() || mmacro_path.IsEmpty())
    {
        return;
    }
    CDialog::OnOK();
}

void MacroRecordDlg::OnCancel()
{
    CDialog::OnCancel();
}

void MacroRecordDlg::OnBnClickedBrowsePath()
{
    TCHAR path[MAX_PATH];
    if (BrowseForFolder(m_hWnd, NULL, NULL, path))
    {
        SetDlgItemText(IDC_EDIT_PATH, path);
    }
}
