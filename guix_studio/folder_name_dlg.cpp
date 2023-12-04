// folder_name_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "studiox.h"
#include "studiox_includes.h"
#include "folder_name_dlg.h"


enum folder_name_test_commands{
    TEST_SET_FOLDER_NAME = 1,
    TEST_SAVE_FOLDER_NAME_EDIT,
    TEST_CHECK_SPECIFY_OUTPUT_FILE,
    TEST_SET_FOLDER_OUTPUT_FILE_NAME
};
// folder_name_dlg dialog

enum ItemType {
    TYPE_RESOURCE_FOLDER = 1,
    TYPE_PROJECT_VIEW_FOLDER,
};

BEGIN_MESSAGE_MAP(folder_name_dlg, express_dialog)
    ON_EN_CHANGE(IDC_FOLDER_NAME, &folder_name_dlg::OnEnChangeFolderName)
    ON_BN_CLICKED(IDC_CUSTOM_FOLDER_OUTPUT, &folder_name_dlg::OnBnClickedCustomOutput)
    ON_MESSAGE(STUDIO_TEST, &folder_name_dlg::OnTestMessage)
    ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(folder_name_dlg, express_dialog)

folder_name_dlg::folder_name_dlg(res_info *info, CWnd* pParent /*=NULL*/)
    : express_dialog(folder_name_dlg::IDD, pParent)
{
    folder_name = info->name;
    output_filename.Empty();
    input_item_type = TYPE_RESOURCE_FOLDER;
    IconId = IDB_FOLDER_RENAME;
    SetTitleText("Edit Folder Name");
}

folder_name_dlg::folder_name_dlg(folder_info *info, CWnd* pParent /*=NULL*/)
    : express_dialog(folder_name_dlg::IDD, pParent)
{
    folder_name = info->folder_name;
    output_filename = info->output_filename;
    input_item_type = TYPE_PROJECT_VIEW_FOLDER;
    IconId = IDB_FOLDER_RENAME;
    SetTitleText("Edit Folder Name");
}

folder_name_dlg::~folder_name_dlg()
{
}

void folder_name_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FOLDER_NAME, folder_name);
    DDX_Text(pDX, IDC_FOLDER_OUTPUT_FILENAME, output_filename);
}

void folder_name_dlg::OnEnChangeFolderName()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


BOOL folder_name_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    if (output_filename.IsEmpty())
    {
        /* output filename is empty means it hasn't been specified. So disable checkbox and edit control. */
        ((CButton *)GetDlgItem(IDC_CUSTOM_FOLDER_OUTPUT))->SetCheck(FALSE);
        GetDlgItem(IDC_FOLDER_OUTPUT_FILENAME)->EnableWindow(FALSE);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CUSTOM_FOLDER_OUTPUT))->SetCheck(TRUE);
        GetDlgItem(IDC_FOLDER_OUTPUT_FILENAME)->EnableWindow(TRUE);
    }

    /* Hide ouput file name edit Item and resize current window. */
    if (input_item_type == TYPE_RESOURCE_FOLDER)
    {
        /* Hide item. */
        GetDlgItem(IDC_CUSTOM_FOLDER_OUTPUT)->ShowWindow(FALSE);
        GetDlgItem(IDC_FOLDER_OUTPUT_FILENAME)->ShowWindow(FALSE);

        /* Resize window */
        CRect window_size;
        GetWindowRect(window_size);

        /* Hard coded window size here. */
        window_size.bottom -= 50;
        MoveWindow(window_size);
    }

    SetControlAccessibleName(GetDlgItem(IDC_FOLDER_OUTPUT_FILENAME)->GetSafeHwnd(), _T("folder_output_filename"));

    AddCancelButton();
    AddSaveButton();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void folder_name_dlg::GetEditFolderName(CString &new_folder_name) const
{
    new_folder_name = folder_name;
}

void folder_name_dlg::GetSpecifiedOutputFileName(CString &output) const
{
    output = output_filename;
}

void folder_name_dlg::OnBnClickedCustomOutput()
{
    // TODO: Add your control notification handler code here
    CButton *pb = (CButton *)GetDlgItem(IDC_CUSTOM_FOLDER_OUTPUT);

    if (pb->GetCheck())
    {
        GetDlgItem(IDC_FOLDER_OUTPUT_FILENAME)->EnableWindow(TRUE);
    }
    else
    {
        SetDlgItemText(IDC_FOLDER_OUTPUT_FILENAME, _T(""));
        output_filename.Empty();
        GetDlgItem(IDC_FOLDER_OUTPUT_FILENAME)->EnableWindow(FALSE);
    }
}

void folder_name_dlg::OnOK()
{
    CString name;
    GetDlgItemText(IDC_FOLDER_NAME, name);
    
    if (name.IsEmpty())
    {
        Notify("Folder name could not be empty.", this);
        return;
    }

    GetDlgItemText(IDC_FOLDER_OUTPUT_FILENAME, name);

    if (IsFileNameFormat(name))
    {
        express_dialog::OnOK();
    }
}

LRESULT folder_name_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;

    switch (wParam)
    {
    case TEST_SET_FOLDER_NAME:
        SetDlgItemText(IDC_FOLDER_NAME, GetTestingParam(0));
        break;

    case TEST_SAVE_FOLDER_NAME_EDIT:
        OnOK();
        break;

    case TEST_CHECK_SPECIFY_OUTPUT_FILE:
        pWnd = GetDlgItem(IDC_CUSTOM_FOLDER_OUTPUT);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CUSTOM_FOLDER_OUTPUT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_FOLDER_OUTPUT_FILE_NAME:
        SetDlgItemText(IDC_FOLDER_OUTPUT_FILENAME, GetTestingParam(0));
        break;
    }

    return 0;
}