#include "studiox_includes.h"
#include "MacroPlaybackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MacroPlaybackDlg, express_dialog)
    ON_BN_CLICKED(IDC_BROWSE_PATH, &MacroPlaybackDlg::OnBnClickedBrowsePath)
    ON_BN_CLICKED(IDCANCEL, &MacroPlaybackDlg::OnCancel)
    ON_WM_SHOWWINDOW()
    ON_WM_PAINT()
END_MESSAGE_MAP()

MacroPlaybackDlg::MacroPlaybackDlg(CWnd* pParent)
	: express_dialog(MacroPlaybackDlg::IDD, pParent)
{
    IconId = IDB_MACRO_PLAYBACK_ICON;
    SetTitleText("Playback Macro");
}

MacroPlaybackDlg::~MacroPlaybackDlg()
{
}

void MacroPlaybackDlg::OnPaint()
{
    express_dialog::OnPaint();
}

void MacroPlaybackDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CRect size;
    GetWindowRect(&size);

    size.OffsetRect(0, -120);
    MoveWindow(size);
    CDialog::OnShowWindow(bShow, nStatus);
}

BOOL MacroPlaybackDlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    AddCancelButton();
    AddSaveButton(_T("Playback"));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void MacroPlaybackDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

void MacroPlaybackDlg::OnOK()
{
    if (AskUser("Malicious macro file can harm your computuer, please make sure the macro file is trusted!", this))
    {
        GetDlgItemText(IDC_EDIT_NAME, mmacro_pathname);

        if (mmacro_pathname.IsEmpty())
        {
            return;
        }
        CDialog::OnOK();
    }
}

void MacroPlaybackDlg::OnCancel()
{
    CDialog::OnCancel();
}

void MacroPlaybackDlg::OnBnClickedBrowsePath()
{
    CString pathname;

    if (BrowseForSingleFile(_T("Select Macro File"), NULL, _T("Macro File"), pathname, this))
    {
        SetDlgItemText(IDC_EDIT_NAME, pathname);
    }
}