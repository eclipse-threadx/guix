// string_export_dlg.cpp : implementation file
//

#include "stdafx.h"

#include "studiox_includes.h"
#include "string_export_dlg.h"
#include "config_languages_dlg.h"
#include "afxdialogex.h"

enum xliff_export_dlg_test_commands{
    TEST_SET_STRING_EXPORT_SRC_LANGUAGE = 1,
    TEST_SET_STRING_EXPORT_TARGET_LANGUAGE,
    TEST_SET_XLIFF_VERSION,
    TEST_SET_STRING_EXPORT_FILENAME,
    TEST_SET_STRING_EXPORT_PATH,
    TEST_SELECT_STRING_EXPORT_FORMAT,
    TEST_SAVE_STRING_EXPORT,
    TEST_CANCEL_STRING_EXPORT
};

STRING_VAL_PAIR string_export_type_list[]={
    { _T("STRING_EXPORT_TYPE_XLIFF"), STRING_EXPORT_TYPE_XLIFF },
    { _T("STRING_EXPORT_TYPE_CSV"), STRING_EXPORT_TYPE_CSV },
    { _T(""), 0 }
};

BEGIN_MESSAGE_MAP(string_export_dlg, express_dialog)
    ON_BN_CLICKED(IDC_BROWSE, &string_export_dlg::OnBnClickedBrowse)
    ON_BN_CLICKED(IDOK, &string_export_dlg::OnBnClickedOk)
    ON_MESSAGE(STUDIO_TEST, &string_export_dlg::OnTestMessage)
    ON_BN_CLICKED(IDC_XLIFF_FORMAT, &string_export_dlg::OnSelectXLIFF)
    ON_BN_CLICKED(IDC_CSV_FORMAT, &string_export_dlg::OnSelectCSV)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
string_export_dlg::string_export_dlg(CWnd* pParent /*=NULL*/)
    : express_dialog(string_export_dlg::IDD, pParent)
{
    IconId = IDB_EXPORT;
    SetTitleText("XLIFF/CSV Export Control");
}

///////////////////////////////////////////////////////////////////////////////
string_export_dlg::~string_export_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
void string_export_dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SRC_LANGUAGE, mSrcLanguageCombo);
    DDX_Control(pDX, IDC_TARGET_LANGUAGE, mTargetLanguageCombo);
    DDX_Control(pDX, IDC_XLIFF_VERSION, xliff_version_combo);

    int index = 0;

    while(1)
    {
        CString lang_name;
        lang_name = config_languages_dlg::BuildCombinedLanguageName(index);
        if (lang_name.IsEmpty())
        {
            break;
        }
        mSrcLanguageCombo.AddString(lang_name);
        mTargetLanguageCombo.AddString(lang_name);
        index++;
    }
    xliff_version_combo.AddString(_T("version 1.2"));
    xliff_version_combo.AddString(_T("version 2.0"));

    CRect size;

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    // Set the drop-down list size for each of the combo-boxes.
    // There doesn't seem to be any way to do it in resource editor.
    mTargetLanguageCombo.GetWindowRect(&size);
    size.bottom = size.top + 200;
    ScreenToClient(&size);
    mTargetLanguageCombo.MoveWindow(&size);
    mTargetLanguageCombo.SetCurSel(project->mHeader.string_export_target);

    mSrcLanguageCombo.GetWindowRect(&size);
    size.bottom = size.top + 200;
    ScreenToClient(&size);
    mSrcLanguageCombo.MoveWindow(&size);
    mSrcLanguageCombo.SetCurSel(project->mHeader.string_export_src);

    xliff_version_combo.GetWindowRect(&size);
    size.bottom = size.top + 60;
    ScreenToClient(&size);
    xliff_version_combo.MoveWindow(&size);

    if (project->mHeader.string_export_version == 2)
    {
        xliff_version_combo.SetCurSel(1);
    }
    else
    {
        xliff_version_combo.SetCurSel(0);
    }

    if (!pDX->m_bSaveAndValidate)
    {
        SetDlgItemText(IDC_FILENAME, project->mHeader.string_export_filename);
        SetDlgItemText(IDC_PATH, project->mHeader.string_export_path);

        if (project->mHeader.string_export_filetype == STRING_EXPORT_TYPE_XLIFF)
        {
            ((CButton *)GetDlgItem(IDC_XLIFF_FORMAT))->SetCheck(TRUE);
        }
        else
        {
            ((CButton *)GetDlgItem(IDC_CSV_FORMAT))->SetCheck(TRUE);
        }

        ShowHideXliffVersion();
    }
    else
    {
        int check = ((CButton *)GetDlgItem(IDC_XLIFF_FORMAT))->GetCheck();
        if (check)
        {
            project->mHeader.string_export_filetype = STRING_EXPORT_TYPE_XLIFF;
        }
        else
        {
            project->mHeader.string_export_filetype = STRING_EXPORT_TYPE_CSV;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_export_dlg::OnBnClickedBrowse()
{
    TCHAR path[MAX_PATH];
    CString current_path;
    GetDlgItemText(IDC_PATH, current_path);

    // TODO: Add your control notification handler code here
    if (BrowseForFolder(m_hWnd, _T("Select output folder..."), current_path, path) == TRUE)
    {
        current_path = path;
        ConvertToProjectRelativePath(current_path);
        SetDlgItemText(IDC_PATH, current_path);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_export_dlg::OnBnClickedOk()
{
    int src_index = mSrcLanguageCombo.GetCurSel();
    int target_index = mTargetLanguageCombo.GetCurSel();
    CString test_name;
    studiox_project *pProject = GetOpenProject();

    if (!pProject)
    {
        CDialog::OnOK();
        return;
    }

    if (src_index == target_index)
    {
        ErrorMsg("Source and Target languages cannot be the same", this);
        return;
    }
    
    int project_index = config_languages_dlg::GetStringTableIndexFromLanguageIndex(GetOpenProject(), src_index);

    if (project_index == -1)
    {
        ErrorMsg("The specified source language is not included in this project.", this);
        return;
    }

    GetDlgItemText(IDC_FILENAME, test_name);

    if (test_name.IsEmpty())
    {
        ErrorMsg("Please enter a valid filename", this);
        return;
    }

    GetDlgItemText(IDC_PATH, test_name);

    if (test_name.IsEmpty())
    {
        SetDlgItemText(IDC_PATH, _T(".\\"));
    }

    // save the information to the project header:

    pProject->mHeader.string_export_src = src_index;
    pProject->mHeader.string_export_target = target_index;
    if (xliff_version_combo.GetCurSel() == 1)
    {
        pProject->mHeader.string_export_version = 2;
    }
    else
    {
        pProject->mHeader.string_export_version = 1;
    }
    GetDlgItemText(IDC_FILENAME, pProject->mHeader.string_export_filename);
    GetDlgItemText(IDC_PATH, pProject->mHeader.string_export_path);
    pProject->SetModified();

    // TODO: Add your control notification handler code here
    CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
int string_export_dlg::GetSrcLanguage()
{
    return mSrcLanguageCombo.GetCurSel();
}

///////////////////////////////////////////////////////////////////////////////
int string_export_dlg::GetTargetLanguage()
{
    return mTargetLanguageCombo.GetCurSel();
}

///////////////////////////////////////////////////////////////////////////////
int string_export_dlg::GetXliffVersion()
{
    if (xliff_version_combo.GetCurSel())
    {
        return 2;
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
CString string_export_dlg::GetPathname()
{
    CString pathname;
    CString filename;
    GetDlgItemText(IDC_PATH, pathname);
    GetDlgItemText(IDC_FILENAME, filename);
    pathname += "\\";
    pathname += filename;
    return pathname;
}

///////////////////////////////////////////////////////////////////////////////
int string_export_dlg::GetStringExportType(CString type_name)
{
	return FindPairVal(string_export_type_list, type_name);
}

///////////////////////////////////////////////////////////////////////////////
CString string_export_dlg::GetStringExportTypeName(int type)
{
	return FindPairString(string_export_type_list, type);
}

///////////////////////////////////////////////////////////////////////////////
BOOL string_export_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    AddCancelButton();
    AddSaveButton();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
void string_export_dlg::OnSelectXLIFF()
{
    ShowHideXliffVersion();
}

///////////////////////////////////////////////////////////////////////////////
void string_export_dlg::OnSelectCSV()
{
    ShowHideXliffVersion();
}

///////////////////////////////////////////////////////////////////////////////
void string_export_dlg::ShowHideXliffVersion()
{
    int check = ((CButton *)GetDlgItem(IDC_XLIFF_FORMAT))->GetCheck();

    GetDlgItem(IDC_XLIFF_VERSION_LABEL)->ShowWindow(check);
    GetDlgItem(IDC_XLIFF_VERSION)->ShowWindow(check);
}

///////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT string_export_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    int language_index;
    int ctrl_id;

    switch (wParam)
    {
    case TEST_SET_STRING_EXPORT_SRC_LANGUAGE:
        language_index = config_languages_dlg::GetLanguageNameIndex(GetTestingParam(0));
        mSrcLanguageCombo.SetCurSel(language_index);
        break;

    case TEST_SET_STRING_EXPORT_TARGET_LANGUAGE:
        language_index = config_languages_dlg::GetLanguageNameIndex(GetTestingParam(0));
        mTargetLanguageCombo.SetCurSel(language_index);
        break;

    case TEST_SET_XLIFF_VERSION:
        xliff_version_combo.SetCurSel(lParam);
        break;

    case TEST_SET_STRING_EXPORT_FILENAME:
        SetDlgItemText(IDC_FILENAME, GetTestingParam(0));
        break;

    case TEST_SET_STRING_EXPORT_PATH:
        SetDlgItemText(IDC_PATH, GetTestingParam(0));
        break;

    case TEST_SELECT_STRING_EXPORT_FORMAT:
        if (lParam == STRING_EXPORT_TYPE_CSV)
        {
            ctrl_id = IDC_CSV_FORMAT;

        }
        else
        {
            ctrl_id = IDC_XLIFF_FORMAT;
        }

        CheckRadioButton(IDC_XLIFF_FORMAT, IDC_CSV_FORMAT, ctrl_id);
        SendMessage(WM_COMMAND, MAKEWPARAM(ctrl_id, BN_CLICKED), 0);
        break;

    case TEST_SAVE_STRING_EXPORT:
        OnBnClickedOk();
        break;

    case TEST_CANCEL_STRING_EXPORT:
        OnCancel();
        break;
    }

    return 0;
}
