#pragma once


// RecentProjectDlg dialog

class RecentProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(RecentProjectDlg)

public:
	RecentProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecentProjectDlg();

// Dialog Data
	enum { IDD = IDD_RECENT_PROJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
public:
    afx_msg void OnStnClickedRecent1();
    afx_msg void OnStnClickedRecent2();
    afx_msg void OnStnClickedRecent3();
    afx_msg void OnStnClickedRecent4();
    afx_msg void OnStnClickedRecent5();
    afx_msg void OnBnClickedCreateProject();
};
