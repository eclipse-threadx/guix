// configure_theme_dlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "afxdialogex.h"
#include "configure_theme_dlg.h"
#include "PaletteLayoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum configure_theme_dlg_test_commands{
    TEST_SELECT_DISPLAY_NAME = 1,
    TEST_ADD_THEME,
    TEST_DELETE_THEME,
    TEST_SET_ACTIVE_THEME,
    TEST_SELECT_THEME_INDEX,
    TEST_SET_THEME_NAME,
    TEST_EDIT_PALETTE,
    TEST_CANCEL_THEME_CONFIGURE,
    TEST_SAVE_THEME_CONFIGURE
};

// configure_theme_dlg dialog

IMPLEMENT_DYNAMIC(configure_theme_dlg, express_dialog)

BEGIN_MESSAGE_MAP(configure_theme_dlg, express_dialog)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THEME_INDEX, &configure_theme_dlg::OnDeltaposSpinThemeIndex)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUM_THEMES, &configure_theme_dlg::OnDeltaposSpinNumThemes)
    ON_CBN_SELCHANGE(IDC_DISPLAY_NAME, &configure_theme_dlg::OnCbnSelchangeDisplayName)
    ON_BN_CLICKED(IDB_EDIT_PALETTE, &configure_theme_dlg::OnBnClickedEditPalette)
    ON_BN_CLICKED(IDOK, &configure_theme_dlg::OnOK)
    ON_EN_KILLFOCUS(IDC_THEME_NAME, &configure_theme_dlg::OnEnKillfocusThemeName)
    ON_MESSAGE(STUDIO_TEST, &configure_theme_dlg::OnTestMessage)
END_MESSAGE_MAP()

configure_theme_dlg::configure_theme_dlg(int DisplayIndex, CWnd* pParent /*=NULL*/)
    : express_dialog(configure_theme_dlg::IDD, pParent)
{
    IconId = IDB_CONFIGURE_THEMES;
    SetTitleText("Configure Themes");

    mDisplayIndex = DisplayIndex;
    mThemeIndex = 0;
    mNumThemes = 0;

    if (mDisplayIndex < 0 || mDisplayIndex >= MAX_DISPLAYS)
    {
        mDisplayIndex = 0;
    }
}

configure_theme_dlg::~configure_theme_dlg()
{
}

BOOL configure_theme_dlg::OnInitDialog()
{
    int index;
    express_dialog::OnInitDialog();

    ShowHidePaletteEditButton();

    CSpinButtonCtrl *pb = (CSpinButtonCtrl *) GetDlgItem(IDC_SPIN_NUM_THEMES);
    CEdit *pe = (CEdit *) GetDlgItem(IDC_NUM_THEMES);
    pb->SetBuddy(pe);
    pb->SetRange(1, MAX_THEMES);

    pb = (CSpinButtonCtrl *) GetDlgItem(IDC_SPIN_THEME_INDEX);
    pe = (CEdit *) GetDlgItem(IDC_THEME_INDEX);
    SetDlgItemText(IDC_THEME_INDEX, _T("1"));
    pb->SetBuddy(pe);

    studiox_project* project = GetOpenProject();
    mNumThemes = project->mDisplays[mDisplayIndex].num_themes;

    if (project)
    {
        pb->SetRange(1, mNumThemes);
    }

    for (index = 0; index < project->mHeader.num_displays; index++)
    {
        mDisplayNameCombo.AddString(project->mDisplays[index].name);
    }
    mDisplayNameCombo.SetCurSel(mDisplayIndex);

    // Initiate theme name combo
    for (index = 0; index < mNumThemes; index++)
    {
        mThemeNames[index] = project->mDisplays[mDisplayIndex].themes[index].theme_name;
        mStaticallyDefined[index] = project->mDisplays[mDisplayIndex].themes[index].statically_defined;
        mThemeNameCombo.AddString(mThemeNames[index]);
    }
    mStaticallyDefinedCheck.SetCheck(project->mDisplays[mDisplayIndex].themes[0].statically_defined);
    mThemeNameCombo.SetCurSel(project->mDisplays[mDisplayIndex].active_theme);

    CRect size;
    mDisplayNameCombo.GetWindowRect(&size);
    size.bottom = size.top + 100;
    ScreenToClient(&size);
    mDisplayNameCombo.MoveWindow(size);

    SetDlgItemText(IDC_THEME_NAME, project->mDisplays[mDisplayIndex].themes[DEFAULT_THEME].theme_name);

    AddCancelButton();
    AddSaveButton();

    //SetWindowText(_T("configure_themes"));

    return TRUE; // return TRUE unless you set the focus to a control
}

static DWORD WINAPI InitializeThemeResourcesThreadEntry(LPVOID thread_input)
{
    studiox_project* project = GetOpenProject();

    if (project)
    {
        INT* param_array = (INT*)thread_input;

        int old_num_themes = param_array[0];
        int new_num_themes = param_array[1];
        int display = param_array[2];

        for (int theme = old_num_themes; theme < new_num_themes; theme++)
        {

            // create default theme resources
            res_info* theme_0 = project->mDisplays[display].themes[0].GetFirstResourceInfo();
            res_info* pInfo = new res_info(NULL, *theme_0, TRUE);

            project->mDisplays[display].themes[theme].SetFirstResourceInfo(pInfo);

            project->InitializeThemeResources(display, theme, pInfo);
        }
    }
    EndBusyMsg();

    return TRUE;
}

void configure_theme_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_DISPLAY_NAME, mDisplayNameCombo);
    DDX_Control(pDX, IDC_THEME_NAME_COMBO, mThemeNameCombo);
    DDX_Control(pDX, IDC_THEME_NAME, mThemeName);
    DDX_Control(pDX, IDC_SPIN_NUM_THEMES, mSpinNumThemes);
    DDX_Control(pDX, IDC_SPIN_THEME_INDEX, mSpinThemeIndex);
    DDX_Control(pDX, IDC_STATICALLY_DEFINED, mStaticallyDefinedCheck);

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    INT old_num_themes = project->mDisplays[mDisplayIndex].num_themes;

    DDX_Text(pDX, IDC_NUM_THEMES, project->mDisplays[mDisplayIndex].num_themes);

    if (pDX->m_bSaveAndValidate)
    {
        int index = mSpinThemeIndex.GetPos();
        mStaticallyDefined[index - 1] = mStaticallyDefinedCheck.GetCheck();

        for (int index = 0; index < mNumThemes; index++)
        {
            project->mDisplays[mDisplayIndex].themes[index].theme_name = mThemeNames[index];
            project->mDisplays[mDisplayIndex].themes[index].statically_defined = mStaticallyDefined[index];
        }

        for (int theme = mNumThemes; theme < old_num_themes; theme++)
        {
            // delete theme resources
            project->CleanupThemeResources(&project->mDisplays[mDisplayIndex], theme);
        }

        // create default theme resources
        INT param_array[3] = { old_num_themes, mNumThemes, mDisplayIndex };

        StartWorkThread(InitializeThemeResourcesThreadEntry, param_array, "Initializing Theme Resources.", TRUE);

        for (int index = 0; index < mNumThemes; index++)
        {
            res_info *pInfo = project->mDisplays[mDisplayIndex].themes[index].GetFirstResourceInfo();
            res_info *pThemeHeader = project->FindResourceFolder(pInfo, RES_TYPE_HEADER, THEME_HEADER);

            if (pThemeHeader)
            {
                pThemeHeader->name = project->mDisplays[mDisplayIndex].themes[index].theme_name;
            }
        }

        int active_theme = mThemeNameCombo.GetCurSel();

        if (active_theme != project->mDisplays[mDisplayIndex].active_theme)
        {
            project->mDisplays[mDisplayIndex].active_theme = active_theme;

            // install active theme resources
            GetResourceView()->InstallResources(mDisplayIndex);
        }
    }
}

void configure_theme_dlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class

    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    for (int theme = project->mDisplays[mDisplayIndex].num_themes; theme < MAX_THEMES; theme++)
    {
        // delete theme resources
        project->CleanupThemeResources(&project->mDisplays[mDisplayIndex], theme);
    }

    express_dialog::OnCancel();
}

// configure_theme_dlg message handlers

void configure_theme_dlg::OnDeltaposSpinThemeIndex(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here

    int upper;
    int lower;

    mSpinThemeIndex.GetRange(lower, upper);
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    int oldval = pNMUpDown ->iPos;
    int newval = oldval + pNMUpDown->iDelta;

    // save the current name, update to new name, if the value is within 
    // limits
    if (oldval <= upper && oldval >= lower &&
        newval <= upper && newval >= lower)
    {
        if (!TestInputName(&mThemeName, "Theme Name", mThemeNames[oldval - 1], this))
        {
            *pResult = -1;
            return;
        }
        SetDlgItemText(IDC_THEME_NAME, mThemeNames[newval - 1]);

        mStaticallyDefined[oldval - 1] = mStaticallyDefinedCheck.GetCheck();
        mStaticallyDefinedCheck.SetCheck(mStaticallyDefined[newval - 1]);

        mThemeIndex = newval - 1;

        UpdateThemeNameCombo();
    }

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::OnDeltaposSpinNumThemes(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR); 
    // TODO: Add your control notification handler code here

    int oldval = pNMUpDown ->iPos;
    int newval = oldval + pNMUpDown->iDelta;
    studiox_project *project = GetOpenProject();

    // save the current name, update to new name, if the value is within 
    // limits
    if (oldval <= MAX_THEMES && oldval >= 1 &&
        newval <= MAX_THEMES && newval >= 1)
    {
        if (newval < oldval)
        {
            // decreased theme number
            CString msg;
            msg.Format(_T("The theme \"%s\" will be deleted. Are you sure?"), mThemeNames[oldval - 1].GetString());

            if (!AskUser(CT2A(msg.GetString()), this))
            {
                mSpinNumThemes.SetPos(oldval + oldval - newval);
                return;
            }

            if (mSpinThemeIndex.GetPos() > newval)
            {
                mSpinThemeIndex.SetPos(newval);
                SetDlgItemText(IDC_THEME_NAME, mThemeNames[newval - 1]);
                mStaticallyDefinedCheck.SetCheck(mStaticallyDefined[newval - 1]);
            }
        }
        else
        {
            // increased theme number
            mThemeNames[newval - 1].Format(_T("theme_%d"), newval);
            mStaticallyDefined[newval - 1] = TRUE;
        }

        mSpinThemeIndex.SetRange(1, newval);
        mSpinThemeIndex.Invalidate();
        mNumThemes = newval;
        UpdateThemeNameCombo();
    }

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::OnCbnSelchangeDisplayName()
{
    // TODO: Add your control notification handler code here
    studiox_project *project = GetOpenProject();
    int new_display_index = mDisplayNameCombo.GetCurSel();
    if (project && (mDisplayIndex != new_display_index))
    {
        mDisplayIndex = new_display_index;
        mNumThemes = project->mDisplays[mDisplayIndex].num_themes;

        UpdateThemeNameCombo();

        mThemeIndex = mThemeNameCombo.GetCurSel();

        mSpinThemeIndex.SetPos(1);
        mSpinThemeIndex.SetRange(1, mNumThemes);
        mSpinNumThemes.SetPos(mNumThemes);
        SetDlgItemText(IDC_THEME_NAME, project->mDisplays[mDisplayIndex].themes[0].theme_name);

        ShowHidePaletteEditButton();
    }
}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::OnBnClickedEditPalette()
{
    // TODO: Add your control notification handler code here
    PaletteLayoutDlg dlg(mDisplayIndex, mThemeIndex, this);
    dlg.DoModal();
    GetDlgItem(IDB_EDIT_PALETTE)->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::OnOK()
{
    int theme = GetDlgItemInt(IDC_THEME_INDEX) - 1;

    if (TestInputName(&mThemeName, "Theme Name", mThemeNames[theme], this))
    {
        express_dialog::OnOK();
    }

}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::UpdateThemeNameCombo()
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    int active_theme = mThemeNameCombo.GetCurSel();

    mThemeNameCombo.ResetContent();
    for (int index = 0; index < mNumThemes; index++)
    {
        mThemeNameCombo.AddString(mThemeNames[index]);
    }

    int last = mThemeNameCombo.GetCount() - 1;

    if (active_theme > last)
    {
        active_theme = last;
    }

    mThemeNameCombo.SetCurSel(active_theme);
}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::ShowHidePaletteEditButton()
{
    studiox_project* project = GetOpenProject();
    BOOL ShowEditDlgBtn = FALSE;

    if (project)
    {
        if ((project->mDisplays[mDisplayIndex].bits_per_pix == 8) &&
            (project->mDisplays[mDisplayIndex].packed_format == FALSE) &&
            (project->mDisplays[mDisplayIndex].format_332 == FALSE))
        {
            ShowEditDlgBtn = TRUE;
        }
    }

    GetDlgItem(IDB_EDIT_PALETTE)->ShowWindow(ShowEditDlgBtn);
}

/////////////////////////////////////////////////////////////////////////////
void configure_theme_dlg::OnEnKillfocusThemeName()
{
    // TODO: Add your control notification handler code here
    int theme = GetDlgItemInt(IDC_THEME_INDEX) - 1;
    GetDlgItemText(IDC_THEME_NAME, mThemeNames[theme]);
    UpdateThemeNameCombo();
}


/////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT configure_theme_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    NMUPDOWN NMUpDown;
    NMHDR hdr;

    switch (wParam)
    {
    case TEST_SELECT_DISPLAY_NAME:
        mDisplayNameCombo.SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_DISPLAY_NAME, CBN_SELCHANGE), (LPARAM)mDisplayNameCombo.m_hWnd);
        break;

    case TEST_ADD_THEME:
        hdr.hwndFrom = mSpinNumThemes.m_hWnd;
        hdr.idFrom = IDC_SPIN_NUM_THEMES;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = mSpinNumThemes.GetPos();
        NMUpDown.iDelta = 1;

        mSpinNumThemes.SetPos(NMUpDown.iPos + 1);
        SetDlgItemInt(IDC_NUM_THEMES, NMUpDown.iPos + 1);

        SendMessage(WM_NOTIFY, IDC_SPIN_NUM_THEMES, (LPARAM)&NMUpDown);
        break;

    case TEST_DELETE_THEME:
        hdr.hwndFrom = mSpinNumThemes.m_hWnd;
        hdr.idFrom = IDC_SPIN_NUM_THEMES;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = mSpinNumThemes.GetPos();
        NMUpDown.iDelta = -1;

        mSpinNumThemes.SetPos(NMUpDown.iPos - 1);
        SetDlgItemInt(IDC_NUM_THEMES, NMUpDown.iPos - 1);

        SendMessage(WM_NOTIFY, IDC_SPIN_NUM_THEMES, (LPARAM)&NMUpDown);
        break;

    case TEST_SET_ACTIVE_THEME:
        mThemeNameCombo.SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_THEME_NAME_COMBO, CBN_SELCHANGE), (LPARAM)mThemeNameCombo.m_hWnd);
        break;

    case TEST_SELECT_THEME_INDEX:
        hdr.hwndFrom = mSpinThemeIndex.m_hWnd;
        hdr.idFrom = IDC_SPIN_THEME_INDEX;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = mSpinThemeIndex.GetPos();
        NMUpDown.iDelta = lParam - NMUpDown.iPos;

        mSpinThemeIndex.SetPos(lParam);
        SetDlgItemInt(IDC_LANGUAGE_INDEX, lParam);

        SendMessage(WM_NOTIFY, IDC_SPIN_THEME_INDEX, (LPARAM)&NMUpDown);
        break;

    case TEST_SET_THEME_NAME:
        mThemeName.SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_THEME_NAME, EN_KILLFOCUS), (LPARAM)mThemeName.m_hWnd);
        break;

    case TEST_EDIT_PALETTE:
        OnBnClickedEditPalette();
        break;

    case TEST_CANCEL_THEME_CONFIGURE:
        OnCancel();
        break;

    case TEST_SAVE_THEME_CONFIGURE:
        OnOK();
        break;
    }

    return 0;
}
