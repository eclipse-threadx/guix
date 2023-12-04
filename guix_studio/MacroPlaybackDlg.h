#pragma once
#include "afxwin.h"
#include <string>

class MacroPlaybackDlg : public express_dialog
{
public:
    MacroPlaybackDlg(CWnd *pParent = NULL);
    ~MacroPlaybackDlg(void);

public:
    CString &Getmacro_pathname(){ return mmacro_pathname;};

// Dialog Data
	enum { IDD = IDD_MACRO_PLAYBACK_DLG};
    DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK();

private:
    CString mmacro_pathname;

public:
    afx_msg void OnBnClickedBrowsePath();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnCancel();
    afx_msg void OnPaint();
    virtual BOOL OnInitDialog();
};

