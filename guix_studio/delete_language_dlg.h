#pragma once
#include "afxwin.h"


// delete_language_dlg dialog

class delete_language_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(delete_language_dlg)

public:
	delete_language_dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~delete_language_dlg();

// Dialog Data
	enum { IDD = IDD_DELETE_LANGUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CComboBox mLangNameCombo;
};
