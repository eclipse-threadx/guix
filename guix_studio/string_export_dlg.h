#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

// xliff_export_dlg dialog

class string_export_dlg : public express_dialog
{
public:
    string_export_dlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~string_export_dlg();

    int GetSrcLanguage();
    int GetTargetLanguage();
    int GetXliffVersion();
    CString GetPathname();

    static int GetStringExportType(CString type_name);
    static CString GetStringExportTypeName(int type);

    enum { IDD = IDD_STRING_EXPORT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedBrowse();
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelectXLIFF();
    afx_msg void OnSelectCSV();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

private:
    void ShowHideXliffVersion();

private:
    express_combobox mSrcLanguageCombo;
    express_combobox mTargetLanguageCombo;
    express_combobox xliff_version_combo;
};
