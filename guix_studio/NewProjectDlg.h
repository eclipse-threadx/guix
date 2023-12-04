#pragma once
#include "afxwin.h"
#include <string>


// NewProjectDlg dialog

class NewProjectDlg : public express_dialog
{
	//DECLARE_DYNAMIC(NewProjectDlg)

public:
	NewProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewProjectDlg();
    CString &Getproject_name();
    CString &Getproject_path();

// Dialog Data
	enum { IDD = IDD_NEW_PROJECT_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL DestroyWindow();
    virtual void OnOK();
    afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
    CString mproject_name;
    CString mproject_path;

public:
    afx_msg void OnBnClickedBrowsePath();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnCancel();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();

};
