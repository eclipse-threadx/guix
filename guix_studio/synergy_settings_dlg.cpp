// synergy_settings_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "studiox.h"
#include "studiox_includes.h"
#include "synergy_settings_dlg.h"


// synergy_settings_dlg dialog

STRING_VAL_PAIR jpeg_decoder_types[] = {
    { _T("None"), DECODER_TYPE_NONE },
    { _T("Software JPEG Decoder"), DECODER_TYPE_SW },
    { _T("Hardware JPEG Decoder"), DECODER_TYPE_HW },
    { _T(""), 0 }
};

STRING_VAL_PAIR png_decoder_types[] = {
    { _T("None"), DECODER_TYPE_NONE },
    { _T("Software PNG Decoder"), DECODER_TYPE_SW },
    { _T(""), 0 }
};

enum synergy_settings_dlg_test_commands{
    TEST_CHECK_2D_DRAWING_ENGINE = 1,
    TEST_SAVE_SYNERGY_SETTING,
    TEST_CANCE_SYNERGY_SETTING
};

IMPLEMENT_DYNAMIC(synergy_settings_dlg, express_dialog)
BEGIN_MESSAGE_MAP(synergy_settings_dlg, express_dialog)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
synergy_settings_dlg::synergy_settings_dlg(CWnd* pParent, int current_display)
: express_dialog(synergy_settings_dlg::IDD, pParent)
{
    IconId = IDB_CONFIGURE_PROJECTS;
    SetTitleText("Renesas Graphics Advanced Settings");

    m_active_display = current_display;
}

///////////////////////////////////////////////////////////////////////////////
synergy_settings_dlg::~synergy_settings_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
void synergy_settings_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_ENABLE_GRAPH_ACCELERATOR, mD2DCheck);
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    if (pDX->m_bSaveAndValidate)
    {
        BOOL check = mD2DCheck.GetCheck();

        project->mHeader.dave2d_graph_accelerator = check;

        CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_JPEG_DECODER);
        int index = pBox->GetCurSel();
        project->mHeader.renesas_jpeg_decoder = pBox->GetItemData(index);

        pBox = (CComboBox*)GetDlgItem(IDC_PNG_DECODER);
        index = pBox->GetCurSel();
        project->mHeader.renesas_png_decoder = pBox->GetItemData(index);
    }
    DDX_Control(pDX, IDC_JPEG_DECODER, mJpegDecoder);
    DDX_Control(pDX, IDC_PNG_DECODER, mPngDecoder);
}

///////////////////////////////////////////////////////////////////////////////
void synergy_settings_dlg::AddComboData(int control_id, STRING_VAL_PAIR *types, int current_val)
{
    STRING_VAL_PAIR *current = types;
    CComboBox *pBox = (CComboBox *)GetDlgItem(control_id);
    int index;
    int sel_index = -1;

    while (current->name.GetLength())
    {
        index = pBox->AddString(current->name);
        pBox->SetItemData(index, current->val);

        if (current->val == current_val)
        {
            sel_index = index;
        }
        current++;
    }
    pBox->SetCurSel(sel_index);
}

///////////////////////////////////////////////////////////////////////////////
BOOL synergy_settings_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();
    studiox_project *project = GetOpenProject();

    //SetWindowText(_T("synergy_settings_dlg"));

    AddCancelButton();
    AddSaveButton();

    if (!project)
    {
        return TRUE;
    }

    /* Initiate Synergy Graph Accelerator checkbox.  */
    if (project->mHeader.dave2d_graph_accelerator)
    {
        mD2DCheck.SetCheck(BST_CHECKED);
    }
    else
    {
        mD2DCheck.SetCheck(BST_UNCHECKED);
    }

    AddComboData(IDC_JPEG_DECODER, jpeg_decoder_types, project->mHeader.renesas_jpeg_decoder);
    AddComboData(IDC_PNG_DECODER, png_decoder_types, project->mHeader.renesas_png_decoder);

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
LRESULT synergy_settings_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_CHECK_2D_DRAWING_ENGINE:
        mD2DCheck.SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ENABLE_GRAPH_ACCELERATOR, BN_CLICKED), (LPARAM)mD2DCheck.m_hWnd);
        break;

    case TEST_SAVE_SYNERGY_SETTING:
        OnOK();
        break;

    case TEST_CANCE_SYNERGY_SETTING:
        OnCancel();
        break;
    }

    return 0;
}
