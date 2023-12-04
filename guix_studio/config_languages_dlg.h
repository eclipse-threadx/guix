#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

#define MAX_STRING_TABLE_STRINGS  (10 * 1024)

// config_languages_dlg dialog

class config_languages_dlg : public express_dialog
{
	DECLARE_DYNAMIC(config_languages_dlg)

public:
	config_languages_dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~config_languages_dlg();
    virtual BOOL OnInitDialog();
    static int GetLanguageNameIndex(CString &findname);
    static int GetLanguageIdIndex(CString &findid);
    static CString BuildCombinedLanguageName(char *name, char *symbol);
    static CString BuildCombinedLanguageName(int index);
    static CString GetLanguageId(CString &findname);
    static CString GetLanguageId(int index);
    static CString GetLanguageName(int index);
    static int GetStringTableIndexFromLanguageIndex(studiox_project *project, int seek_language);
    static CString ConvertOldLanguageName(CString &old_language_name);

protected:
    void AnnounceLanguageNumberChange();

// Dialog Data
	enum { IDD = IDD_CONFIG_LANGUAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
    express_combobox mLanguageNameCombo;
    CButton mStaticallyDefined;
    CButton mBidiTextSupportBox;
    CButton mBidiTextGenerateBox;
    CButton mThaiGlyphShapingSupport;
    CButton mGenAdjustedThaiString;

private:
    int mNumLanguages;
    int mStartLanguages;

    CArray<language_info> mLanguages;

public:
    CSpinButtonCtrl mSpinLanguageIndex;
    afx_msg void OnBnClickedAddLanguage();
    afx_msg void OnBnClickedDeleteLanguage();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();
    afx_msg void OnDeltaposSpinLanguageIndex(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnClickSupportBidiTextBox();
    afx_msg void OnClickedThaiGlyphShapingSupport();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
};
