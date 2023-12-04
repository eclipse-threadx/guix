#include "studiox_includes.h"
#include "grid_setting_dlg.h"

enum grid_snap_test_commands{
    TEST_CHECK_SHOW_GRID = 1,
    TEST_CHECK_SNAP,
    TEST_SET_GRID_SPACING,
    TEST_SET_SNAP_SPACING,
    TEST_SAVE,
    TEST_CANCEL
};

IMPLEMENT_DYNAMIC(grid_setting_dlg, express_dialog)

BEGIN_MESSAGE_MAP(grid_setting_dlg, express_dialog)
    ON_EN_KILLFOCUS(IDC_EDIT_GRID_SPACING, &OnEditGridSpacing)
    ON_EN_KILLFOCUS(IDC_EDIT_SNAP_SPACING, &OnEditSnapSpacing)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
grid_setting_dlg::grid_setting_dlg(CWnd* pParent)
    : express_dialog(grid_setting_dlg::IDD, pParent)
{
    IconId = IDB_GRID;
    SetTitleText("Grid and Snap Settings");
}

///////////////////////////////////////////////////////////////////////////////
grid_setting_dlg::~grid_setting_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
BOOL grid_setting_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    studiox_project *project = GetOpenProject();

    if (project)
    {
        // Set grid spacing spin control
        mGridSpacingVal = project->mHeader.grid_spacing;
        CStatic *ps = (CStatic *)GetDlgItem(IDC_EDIT_GRID_SPACING);

        mSpinGridSpacing.SetBuddy(ps);
        mSpinGridSpacing.SetRange(MIN_GRID_SNAP_SPACE, MAX_GRID_SNAP_SPACE);
        mSpinGridSpacing.SetPos(mGridSpacingVal);

        // Set snap spacing spin cotnrol
        mSnapSpacingVal = project->mHeader.snap_spacing;
        ps = (CStatic *) GetDlgItem(IDC_EDIT_SNAP_SPACING);

        mSpinSnapSpacing.SetBuddy(ps);
        mSpinSnapSpacing.SetRange(MIN_GRID_SNAP_SPACE, MAX_GRID_SNAP_SPACE);
        mSpinSnapSpacing.SetPos(mSnapSpacingVal);
    }

    AddCancelButton();
    AddSaveButton();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
void grid_setting_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_SPIN_GRID_SPACING, mSpinGridSpacing);
    DDX_Control(pDX, IDC_SPIN_SNAP_SPACING, mSpinSnapSpacing);

    studiox_project *project = GetOpenProject();
    if (!project)
    {
        return;
    }
    DDX_Check(pDX, IDC_SHOW_GRID, project->mHeader.grid_enabled);
    DDX_Check(pDX, IDC_SNAP_TO_GRID, project->mHeader.snap_enabled);
    DDX_Check(pDX, IDC_SNAP_TO_WIDGET, project->mHeader.snap_to_widget_enabled);

    if (pDX->m_bSaveAndValidate)
    {
        project->mHeader.grid_spacing = mSpinGridSpacing.GetPos();
        project->mHeader.snap_spacing = mSpinSnapSpacing.GetPos();

        if (GetTargetScreen())
        {
            GetTargetScreen()->Invalidate();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL grid_setting_dlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    CWnd* focus_owner = GetFocus();

    if (focus_owner)
    {
        int ctrl_id = focus_owner->GetDlgCtrlID();

        switch (ctrl_id)
        {
        case IDC_EDIT_GRID_SPACING:
        case IDC_EDIT_SNAP_SPACING:
            if ((pMsg->message == WM_CHAR) && isprint(pMsg->wParam))
            {
                if (pMsg->wParam < '0' || pMsg->wParam > '9')
                {
                    ErrorMsg("Unaccetable Character. You can only type a number here.", this);
                    return TRUE;
                }
            }
            break;
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void grid_setting_dlg::OnEditGridSpacing()
{
    int val = GetDlgItemInt(IDC_EDIT_GRID_SPACING);

    if ((val >= MIN_GRID_SNAP_SPACE) && (val <= MAX_GRID_SNAP_SPACE))
    {
        mGridSpacingVal = val;

    }
    else
    {
        char msg[255];
        sprintf_s(msg, sizeof(msg), "Invalid value for grid spacing. Valid range is from %d to %d.", MIN_GRID_SNAP_SPACE, MAX_GRID_SNAP_SPACE);
        Notify(msg, this);
        SetDlgItemInt(IDC_EDIT_GRID_SPACING, mGridSpacingVal);
    }
}

///////////////////////////////////////////////////////////////////////////////
void grid_setting_dlg::OnEditSnapSpacing()
{
    int val = GetDlgItemInt(IDC_EDIT_SNAP_SPACING);

    if ((val >= MIN_GRID_SNAP_SPACE) && (val <= MAX_GRID_SNAP_SPACE))
    {
        mSnapSpacingVal = val;

    }
    else
    {
        char msg[255];
        sprintf_s(msg, sizeof(msg), "Invalid value for snap spacing. Valid range is from %d to %d.", MIN_GRID_SNAP_SPACE, MAX_GRID_SNAP_SPACE);
        Notify(msg, this);
        SetDlgItemInt(IDC_EDIT_SNAP_SPACING, mSnapSpacingVal);
    }
}

///////////////////////////////////////////////////////////////////////////////
LRESULT grid_setting_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd    *pWnd;
    NMUPDOWN NMUpDown;
    NMHDR    hdr;

    switch (wParam)
    {
    case TEST_CHECK_SHOW_GRID:
        pWnd = (CButton *)GetDlgItem(IDC_SHOW_GRID);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SHOW_GRID, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CHECK_SNAP:
        pWnd = (CButton *)GetDlgItem(IDC_SNAP_TO_GRID);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SNAP_TO_GRID, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_GRID_SPACING:
        hdr.hwndFrom = mSpinGridSpacing.m_hWnd;
        hdr.idFrom = IDC_EDIT_GRID_SPACING;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = mSpinGridSpacing.GetPos();
        NMUpDown.iDelta = lParam - NMUpDown.iPos;

        mSpinGridSpacing.SetPos(lParam);
        SetDlgItemInt(IDC_EDIT_GRID_SPACING, lParam);

        SendMessage(WM_NOTIFY, IDC_SPIN_GRID_SPACING, (LPARAM)&NMUpDown);
        break;

    case TEST_SET_SNAP_SPACING:
        hdr.hwndFrom = mSpinSnapSpacing.m_hWnd;
        hdr.idFrom = IDC_EDIT_SNAP_SPACING;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = mSpinSnapSpacing.GetPos();
        NMUpDown.iDelta = lParam - NMUpDown.iPos;

        mSpinSnapSpacing.SetPos(lParam);
        SetDlgItemInt(IDC_EDIT_SNAP_SPACING, lParam);

        SendMessage(WM_NOTIFY, IDC_SPIN_SNAP_SPACING, (LPARAM)&NMUpDown);
        break;

    case TEST_SAVE:
        OnOK();
        break;

    case TEST_CANCEL:
        OnCancel();
        break;
    }

    return 0;
}
