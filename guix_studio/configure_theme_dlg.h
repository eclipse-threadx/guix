#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// configure_theme_dlg dialog


class configure_theme_dlg : public express_dialog
{
    DECLARE_DYNAMIC(configure_theme_dlg)

public:
    configure_theme_dlg(int DisplayIndex, CWnd* pParent = NULL);   // standard constructor
    virtual ~configure_theme_dlg();
    virtual BOOL OnInitDialog();

// Dialog Data
    enum { IDD = IDD_CONFIGURE_THEMES };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnCancel();

    DECLARE_MESSAGE_MAP()

private:
    int mDisplayIndex;
    int mThemeIndex;
    int mNumThemes;
    CString mThemeNames[MAX_THEMES];
    BOOL mStaticallyDefined[MAX_THEMES];
    void UpdateThemeNameCombo();
    void ShowHidePaletteEditButton();

public:
    express_combobox mDisplayNameCombo;
    express_combobox mThemeNameCombo;
    CEdit mThemeName;
    CButton mStaticallyDefinedCheck;
    afx_msg void OnDeltaposSpinNumThemes(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinThemeIndex(NMHDR *pNMHDR, LRESULT *pResult);
    CSpinButtonCtrl mSpinNumThemes;
    CSpinButtonCtrl mSpinThemeIndex;
    afx_msg void OnCbnSelchangeDisplayName();
    afx_msg void OnBnClickedEditPalette();
    afx_msg void OnOK();
    afx_msg void OnEnKillfocusThemeName();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
};
