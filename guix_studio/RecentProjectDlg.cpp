// RecentProjectDlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "RecentProjectDlg.h"

extern CFont TitleFont;
extern CFont LinkFont;
extern CBrush WinBackBrush;
extern CBrush BlackBrush;
extern INI_INFO StudioXIni;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RecentProjectDlg dialog

IMPLEMENT_DYNAMIC(RecentProjectDlg, CDialog)

BEGIN_MESSAGE_MAP(RecentProjectDlg, CDialog)
 ON_WM_CTLCOLOR()
 ON_WM_SHOWWINDOW()
 ON_STN_CLICKED(IDC_RECENT_1, &RecentProjectDlg::OnStnClickedRecent1)
 ON_STN_CLICKED(IDC_RECENT_2, &RecentProjectDlg::OnStnClickedRecent2)
 ON_STN_CLICKED(IDC_RECENT_3, &RecentProjectDlg::OnStnClickedRecent3)
 ON_STN_CLICKED(IDC_RECENT_4, &RecentProjectDlg::OnStnClickedRecent4)
 ON_STN_CLICKED(IDC_RECENT_5, &RecentProjectDlg::OnStnClickedRecent5)
 ON_BN_CLICKED(IDC_CREATE_PROJECT, &RecentProjectDlg::OnBnClickedCreateProject)
END_MESSAGE_MAP()

RecentProjectDlg::RecentProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RecentProjectDlg::IDD, pParent)
{

}

RecentProjectDlg::~RecentProjectDlg()
{
}

BOOL RecentProjectDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    GetDlgItem(IDC_TITLE)->SetFont(&TitleFont);
    GetDlgItem(IDC_RECENT_1)->SetFont(&LinkFont);
    GetDlgItem(IDC_RECENT_2)->SetFont(&LinkFont);
    GetDlgItem(IDC_RECENT_3)->SetFont(&LinkFont);
    GetDlgItem(IDC_RECENT_4)->SetFont(&LinkFont);
    GetDlgItem(IDC_RECENT_5)->SetFont(&LinkFont);
    return TRUE;
}

void RecentProjectDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (bShow)
    {
        GetDlgItem(IDC_RECENT_1)->SetWindowText(StudioXIni.recent_project_paths[0]);
        GetDlgItem(IDC_RECENT_2)->SetWindowText(StudioXIni.recent_project_paths[1]);
        GetDlgItem(IDC_RECENT_3)->SetWindowText(StudioXIni.recent_project_paths[2]);
        GetDlgItem(IDC_RECENT_4)->SetWindowText(StudioXIni.recent_project_paths[3]);
        GetDlgItem(IDC_RECENT_5)->SetWindowText(StudioXIni.recent_project_paths[4]);
    }
    CWnd::OnShowWindow(bShow, nStatus);
}

void RecentProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

HBRUSH RecentProjectDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (nCtlColor)
    {
//    case CTLCOLOR_DLG:
//        return HBRUSH(BlackBrush);

    case CTLCOLOR_STATIC:
        if (pWnd->GetDlgCtrlID() != IDC_TITLE)
        {
            pDC->SetTextColor(HIGH_TEXT_COLOR);
        }
//        return HBRUSH(
//        pDC->SetBkColor(RGB(0, 0, 0));
//        return HBRUSH(BlackBrush);
        break;
    default:
        break;
    }
    return hbr;
}
// RecentProjectDlg message handlers

void RecentProjectDlg::OnStnClickedRecent1()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->OpenHistoryProject(0);
}

void RecentProjectDlg::OnStnClickedRecent2()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->OpenHistoryProject(1);
}

void RecentProjectDlg::OnStnClickedRecent3()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->OpenHistoryProject(2);
}

void RecentProjectDlg::OnStnClickedRecent4()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->OpenHistoryProject(3);
}

void RecentProjectDlg::OnStnClickedRecent5()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->OpenHistoryProject(4);
}

void RecentProjectDlg::OnBnClickedCreateProject()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->NewProject();
}
