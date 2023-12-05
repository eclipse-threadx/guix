// ProjectConfigDlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "afxdd_.h"
#include "ProjectConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#else
#define STUDIOX_EVAL_BUILD
#endif

#define MIN_PROJECT_CONFIGURE_DLG_WIDTH  400
#define MIN_PROJECT_CONFIGURE_DLG_HEIGHT 400

extern COLOR_RECORD DEFAULT_COLOR_TABLE[];
extern COLOR_RECORD DEFAULT_COLOR_TABLE_GRAYSCALE[];
extern COLOR_RECORD DEFAULT_COLOR_TABLE_MONOCHROME[];


STRING_VAL_PAIR cpu_list[] = {
    { _T("Generic"), CPU_GENERIC },
    { _T("Renesas Synergy"), CPU_SYNERGY },
    { _T("Renesas RA"),      CPU_RA      },
    { _T("Renesas RX"),      CPU_RX      },
    { _T("Renesas RZ"),      CPU_RZ      },
    { _T("ST ChromeArt"),    CPU_ST_CHROMEART },
    { _T(""), 0 }
};

STRING_VAL_PAIR compiler_list[] = {
    { _T("Generic"), TOOLS_GENERIC },
    { _T("IAR"), TOOLS_IAR },
    { _T("GNU"), TOOLS_GNU },
    { _T("CCRX"), TOOLS_CCRX},
     { _T(""), 0 }
};

STRING_VAL_PAIR screen_rotation_list[] = {
    { _T("None"), GX_SCREEN_ROTATION_NONE },
    { _T("CW"), GX_SCREEN_ROTATION_CW },
    { _T("CCW"), GX_SCREEN_ROTATION_CCW },
    { _T("FLIP"), GX_SCREEN_ROTATION_FLIP},
    // Backward compatibility
    { _T("0"), GX_SCREEN_ROTATION_NONE },
    { _T("90"), GX_SCREEN_ROTATION_CW },
    { _T("270"), GX_SCREEN_ROTATION_CCW },
    { _T("180"), GX_SCREEN_ROTATION_FLIP},
     { _T(""), 0 }
};

enum project_configure_dlg_test_commands{
    TEST_CONFIGURE_SOURCE_PATH = 1,
    TEST_CONFIGURE_HEADER_PATH,
    TEST_CONFIGURE_RESOURCE_PATH,
    TEST_CONFIGURE_X_RES,
    TEST_CONFIGURE_Y_RES,
    TEST_CONFIGURE_DISPLAY_NUM,
    TEST_SELECT_DISPLAY_INDEX,
    TEST_CONFIGURE_DISPLAY_NAME,
    TEST_CONFIGURE_DISPLAY_COLOR_FORMAT,
    TEST_CONFIGURE_DISPLAY_COLOR_DEPTH,
    TEST_CONFIGURE_DISPLAY_RGB_BITS,
    TEST_CONFIGURE_MINOR_VERSION,
    TEST_CONFIGURE_SERVICE_PACK,
    TEST_CONFIGURE_CANVAS_ALLOCATE,
    TEST_CONFIGURE_CPU_TYPE,
    TEST_CONFIGURE_BIG_ENDIAN,
    TEST_SAVE_PROJECT_CONFIGURE,
    TEST_CANCEL_PROJECT_CONFIGURE,
    TEST_IS_DEFAULT_MAP_FORMAT,
    TEST_GET_CPU_TYPE,
    TEST_GET_DISPLAY_COLOR_FORMAT,
    TEST_GET_DISPLAY_COLOR_DEPTH,
    TEST_IS_1555_FORMAT,
    TEST_IS_4444_FORAMT,
    TEST_IS_332_FORMAT,
    TEST_IS_NEW_PROJECT,
    TEST_OPEN_ADVANCED_SETTINGS,
    TEST_SET_AA_TEXT_COLORS
};

// ProjectConfigWin dialog

IMPLEMENT_DYNAMIC(ProjectConfigWin, CDialog)

BEGIN_MESSAGE_MAP(ProjectConfigWin, CDialog)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUM_SCREENS, &ProjectConfigWin::OnDeltaposSpinNumScreens)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCREEN_SELECT, &ProjectConfigWin::OnDeltaposSpinScreenSelect)
    ON_EN_KILLFOCUS(IDC_NUM_DISPLAYS, &ProjectConfigWin::OnEnEditNumScreens)
    ON_EN_KILLFOCUS(IDC_SELECTED_DISPLAY, &ProjectConfigWin::OnEnEditSelectedDisplay)
    ON_EN_KILLFOCUS(IDC_MAJOR_VERSION, &ProjectConfigWin::OnEnEditMajorVersion)
    ON_EN_KILLFOCUS(IDC_MINOR_VERSION, &ProjectConfigWin::OnEnEditMinorVersion)
    ON_EN_KILLFOCUS(IDC_SERVICE_PACK, &ProjectConfigWin::OnEnEditServicePack)
    ON_BN_CLICKED(IDC_BROWSE_SOURCE, &ProjectConfigWin::OnBnClickedBrowseSource)
    ON_BN_CLICKED(IDC_BROWSE_HEADER, &ProjectConfigWin::OnBnClickedBrowseHeader)
    ON_BN_CLICKED(IDC_BROWSE_RESOURCE, &ProjectConfigWin::OnBnClickedBrowseResource)
    ON_BN_CLICKED(IDC_RADIO_1BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_RADIO_2BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_RADIO_4BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_RADIO_8BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_RADIO_16BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_RADIO_24BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_RADIO_32BPP, &ProjectConfigWin::BitDepthChanged)
    ON_BN_CLICKED(IDC_CHECK_555FORMAT, &ProjectConfigWin::Format1555Clicked)
    ON_BN_CLICKED(IDC_CHECK_4444FORMAT, &ProjectConfigWin::Format4444Clicked)
    ON_BN_CLICKED(IDC_CHECK_332FORMAT, &ProjectConfigWin::Format332Clicked)
    ON_BN_CLICKED(IDC_ADVANCED_SETTINGS, &ProjectConfigWin::OnAdvancedSettings)
    ON_CBN_SELCHANGE(IDC_CPU_COMBO, &ProjectConfigWin::OnChangeSelectedCPU)
    ON_EN_KILLFOCUS(IDC_X_RESOLUTION, &ProjectConfigWin::OnEditXRes)
    ON_EN_KILLFOCUS(IDC_Y_RESOLUTION, &ProjectConfigWin::OnEditYRes)
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_SIZE()
    ON_WM_MOUSEHWHEEL()
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
ProjectConfigWin::ProjectConfigWin(CWnd* pParent, int current_display)
	: CDialog(ProjectConfigWin::IDD, pParent)
{
    mpProject = GetOpenProject();

    if (current_display >= 0)
    {
        m_current_display = current_display;
    }
    else
    {
        m_current_display = 0;
    }

    mpParent = pParent;

    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);

    // Change dialog font size according to text scaller
    // Create the modeless dialog box from a dialol-box template
    CDialogTemplate dlt;

    if (!dlt.Load(MAKEINTRESOURCE(IDD_CONFIGURE_PROJECT_WIN)))
    {
        return;
    }

    int m_text_scaler = GetTextScaler();

    int point_size = DEFAULT_DLG_FONT_POINT_SIZE * m_text_scaler / DEFAULT_TEXT_SCALER;

    dlt.SetFont(L"MS Shell Dlg", point_size);
    LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
    m_lpszTemplateName = NULL;
    CDialog::CreateIndirect(pdata, pParent);
    GlobalUnlock(dlt.m_hTemplate);
}

///////////////////////////////////////////////////////////////////////////////
ProjectConfigWin::~ProjectConfigWin()
{
    delete m_scroll_helper;
}

///////////////////////////////////////////////////////////////////////////////
BOOL ProjectConfigWin::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set unique accessible name for browse button
    SetControlAccessibleName(GetDlgItem(IDC_BROWSE_SOURCE)->GetSafeHwnd(), _T("browse source path"));
    SetControlAccessibleName(GetDlgItem(IDC_BROWSE_HEADER)->GetSafeHwnd(), _T("browse header path"));
    SetControlAccessibleName(GetDlgItem(IDC_BROWSE_RESOURCE)->GetSafeHwnd(), _T("browse reosurce path"));

    CString description(L"pixels");
    SetAccessibleFullDescription(GetDlgItem(IDC_X_RESOLUTION)->GetSafeHwnd(), description);
    SetAccessibleFullDescription(GetDlgItem(IDC_Y_RESOLUTION)->GetSafeHwnd(), description);
    SetAccessibleDescription(GetDlgItem(IDC_X_RESOLUTION)->GetSafeHwnd(), description);
    SetAccessibleDescription(GetDlgItem(IDC_Y_RESOLUTION)->GetSafeHwnd(), description);

    if (mpProject)
    {
        m_spin_num_screens.SetRange(1, 4);
        m_spin_num_screens.SetBuddy(GetDlgItem(IDC_NUM_DISPLAYS));

        m_spin_current_screen.SetRange(1, mpProject->mHeader.num_displays);
        m_spin_current_screen.SetBuddy(GetDlgItem(IDC_SELECTED_DISPLAY));
        m_spin_current_screen.SetPos(m_current_display + 1);

        mSpinMajorVersion.SetRange(5, GUIX_MAJOR_VERSION + 1);
        mSpinMajorVersion.SetBuddy(GetDlgItem(IDC_MAJOR_VERSION));

        mSpinMinorVersion.SetRange(0, 99);
        mSpinMinorVersion.SetBuddy(GetDlgItem(IDC_MINOR_VERSION));

        mSpinServicePack.SetRange(0, 99);
        mSpinServicePack.SetBuddy(GetDlgItem(IDC_SERVICE_PACK));

        int major, minor, service_pack;
        GuixVersionNumberToVersionFields(mpProject->mHeader.guix_version, major, minor, service_pack);

        mSpinMajorVersion.SetPos(major);
        mSpinMinorVersion.SetPos(minor);
        mSpinServicePack.SetPos(service_pack);
    }

    CRect client;
    GetClientRect(&client);

    m_scroll_helper->SetDisplaySize(client.Width(), client.Height());
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::DoDataExchange(CDataExchange* pDX)
{
    int index;

    if (!mpProject)
    {
        return;
    }

    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_NUM_DISPLAYS, mpProject->mHeader.num_displays);
    DDV_MinMaxInt(pDX, mpProject->mHeader.num_displays, 1, 4);
    DDX_Text(pDX, IDC_SOURCE_PATH, mpProject->mHeader.source_path);
    DDX_Text(pDX, IDC_HEADER_PATH, mpProject->mHeader.header_path);
    DDX_Text(pDX, IDC_RESOURCE_PATH, mpProject->mHeader.resource_path);
    DDX_Text(pDX, IDC_ADDITIONAL_HEADERS, mpProject->mHeader.additional_headers);

    DDX_Control(pDX, IDC_SPIN_SCREEN_SELECT, m_spin_current_screen);
    DDX_Control(pDX, IDC_SPIN_NUM_SCREENS, m_spin_num_screens);
    DDX_Control(pDX, IDC_SPIN_MAJOR_VERSION, mSpinMajorVersion);
    DDX_Control(pDX, IDC_SPIN_MINOR_VERSION, mSpinMinorVersion);
    DDX_Control(pDX, IDC_SPIN_SERVICE_PACK, mSpinServicePack);
    DDX_Control(pDX, IDC_CPU_COMBO, mCpuCombo);
    DDX_Control(pDX, IDC_TOOLS_COMBO, mToolsCombo);
    DDX_Control(pDX, IDC_AA_TEXT_COLORS_COMBO, mAATextColorsCombo);

    DDX_Check(pDX, IDC_BIG_ENDIAN, mpProject->mHeader.big_endian);
    DDX_Check(pDX, IDC_INSERT_BEFORE, mpProject->mHeader.insert_headers_before);

    EnableDisableAdvancedSettings(mpProject->mHeader.target_cpu);

    if (pDX->m_bSaveAndValidate)
    {
        DDX_Text(pDX, IDC_SELECTED_DISPLAY, m_current_display);
        m_current_display -= 1;
        SaveDisplayConfig(m_current_display);

        int major = GetDlgItemInt(IDC_MAJOR_VERSION);
        int minor = GetDlgItemInt(IDC_MINOR_VERSION);
        int service = GetDlgItemInt(IDC_SERVICE_PACK);
        
        mGuixVersion = GuixVersionFieldsToVersionNumber(major, minor, service);
    }
    else
    {
        mOldHeader = mpProject->mHeader;

        // retrieve values from project and init dialog
        int major, minor, service_pack;
        GuixVersionNumberToVersionFields(mpProject->mHeader.guix_version, major, minor, service_pack);

        SetDlgItemInt(IDC_MAJOR_VERSION, major);
        SetDlgItemInt(IDC_MINOR_VERSION, minor);
        SetDlgItemInt(IDC_SERVICE_PACK, service_pack);

        STRING_VAL_PAIR*pEntry = cpu_list;

        while(!pEntry->name.IsEmpty())
        {
            index = mCpuCombo.AddString(pEntry->name);
            mCpuCombo.SetItemData(index, pEntry->val);
            pEntry++;
        }

        pEntry = compiler_list;

        while(!pEntry->name.IsEmpty())
        {
            index = mToolsCombo.AddString(pEntry->name);
            mToolsCombo.SetItemData(index, pEntry->val);
            pEntry++;
        }
        SelectDropListItem(&mCpuCombo, mpProject->mHeader.target_cpu);
        SelectDropListItem(&mToolsCombo, mpProject->mHeader.target_tools);

        InitDisplayConfig(m_current_display);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::InitDisplayConfig(int display_index)
{
    int radio_index = 0;
    int display_num = display_index + 1;

    CDataExchange nx(this, FALSE);

    DDX_Text(&nx, IDC_SELECTED_DISPLAY, display_num);
    DDX_Text(&nx, IDC_SCREEN_NAME, mpProject->mDisplays[display_index].name);
    DDX_Text(&nx, IDC_X_RESOLUTION, mpProject->mDisplays[display_index].xres);
    DDX_Text(&nx, IDC_Y_RESOLUTION, mpProject->mDisplays[display_index].yres);

    DDX_Check(&nx, IDC_CHECK_GRAYSCALE, mpProject->mDisplays[display_index].grayscale);
    DDX_Check(&nx, IDC_CHECK_555FORMAT, mpProject->mDisplays[display_index].format_555);
    DDX_Check(&nx, IDC_CHECK_4444FORMAT, mpProject->mDisplays[display_index].format_4444);
    DDX_Check(&nx, IDC_CHECK_332FORMAT, mpProject->mDisplays[display_index].format_332);
    DDX_Check(&nx, IDC_CHECK_REVERSE_ORDER, mpProject->mDisplays[display_index].reverse_order);
    DDX_Check(&nx, IDC_CHECK_PACKED, mpProject->mDisplays[display_index].packed_format);
    DDX_Check(&nx, IDC_ALLOCATE_CANVAS, mpProject->mDisplays[display_index].allocate_canvas);

    switch(mpProject->mDisplays[display_index].bits_per_pix)
    {
    case 1:
        radio_index = 0;
        break;

    case 2:
        radio_index = 1;
        break;

    case 4:
        radio_index = 2;
        break;

    case 8:
        radio_index = 3;
        break;

    case 16:
        radio_index = 4;
        break;

    case 24:
        radio_index = 5;
        break;

    case 32:
        radio_index = 6;
        break;
    }

    DDX_Radio(&nx, IDC_RADIO_1BPP, radio_index);

    EnableDisable(mpProject->mDisplays[display_index].bits_per_pix, mpProject);
    InitAATextColorsCombobox();
    InitRotationAnglesCombobox(display_index);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::SaveDisplayConfig(int display_index)
{
    int theme;
    int radio_index = 0;
    int display_num = display_index + 1;

    CDataExchange nx(this, TRUE);

    DDX_Text(&nx, IDC_SCREEN_NAME,  mpProject->mDisplays[display_index].name);
    DDX_Text(&nx, IDC_X_RESOLUTION, mpProject->mDisplays[display_index].xres);
    DDX_Text(&nx, IDC_Y_RESOLUTION, mpProject->mDisplays[display_index].yres);

    DDX_Check(&nx, IDC_CHECK_GRAYSCALE, mpProject->mDisplays[display_index].grayscale);
    DDX_Check(&nx, IDC_CHECK_555FORMAT, mpProject->mDisplays[display_index].format_555);
    DDX_Check(&nx, IDC_CHECK_4444FORMAT, mpProject->mDisplays[display_index].format_4444);
    DDX_Check(&nx, IDC_CHECK_332FORMAT, mpProject->mDisplays[display_index].format_332);
    DDX_Check(&nx, IDC_CHECK_REVERSE_ORDER, mpProject->mDisplays[display_index].reverse_order);

    #if defined(_DEBUG)
    DDX_Check(&nx, IDC_CHECK_PACKED, mpProject->mDisplays[display_index].packed_format);
    #else
    mpProject->mDisplays[display_index].packed_format = FALSE;
    #endif

    DDX_Check(&nx, IDC_ALLOCATE_CANVAS, mpProject->mDisplays[display_index].allocate_canvas);
    DDX_Radio(&nx, IDC_RADIO_1BPP, radio_index);

    int old_cpu_type = mpProject->mHeader.target_cpu;
    int old_color_format = mpProject->mDisplays[display_index].colorformat;

    mpProject->mHeader.target_cpu = mCpuCombo.GetItemData(mCpuCombo.GetCurSel());
    mpProject->mHeader.target_tools = mToolsCombo.GetItemData(mToolsCombo.GetCurSel());

    #if defined(STUDIOX_EVAL_BUILD)
    // force 16 or 24 bit unpackedcolor depth for evaluation build
    if (radio_index == 1)
    {
        radio_index = 3;
    }
    #endif
    switch(radio_index)
    {
    case 0:
        mpProject->mDisplays[display_index].bits_per_pix = 1;
        mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_MONOCHROME;
        break;

    case 1:
        mpProject->mDisplays[display_index].bits_per_pix = 2;
        mpProject->mDisplays[display_index].colorformat =  GX_COLOR_FORMAT_2BIT_GRAY;
        break;

    case 2:
        mpProject->mDisplays[display_index].bits_per_pix = 4;
        mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_4BIT_GRAY;
        break;

    case 3:
        mpProject->mDisplays[display_index].bits_per_pix = 8;
        if (mpProject->mDisplays[display_index].format_332)
        {
            mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_8BIT_PACKED_PIXEL;
        }
        else
        {
            mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_8BIT_PALETTE;

            CComboBox *box = (CComboBox *)GetDlgItem(IDC_AA_TEXT_COLORS_COMBO);
            int sel = box->GetCurSel();
            mpProject->mHeader.palette_mode_aa_text_colors = box->GetItemData(sel);
        }
        break;

    case 4:
        mpProject->mDisplays[display_index].bits_per_pix = 16;
        if (mpProject->mDisplays[display_index].format_4444 == TRUE)
        {
            if (mpProject->mDisplays[display_index].reverse_order)
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_4444BGRA;
            }
            else
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_4444ARGB;
            }
        }
        else if (mpProject->mDisplays[display_index].format_555 == TRUE)
        {
            if (mpProject->mDisplays[display_index].reverse_order)
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_5551BGRX;
            }
            else
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_1555XRGB;
            }
        }
        else
        {
            if (mpProject->mDisplays[display_index].reverse_order)
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_565BGR;
            }
            else
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_565RGB;
            }
        }
        break;

    case 5:
        mpProject->mDisplays[display_index].bits_per_pix = 24;

        #if defined(_DEBUG)
        if (mpProject->mDisplays[display_index].packed_format)
        {
            if (mpProject->mDisplays[display_index].reverse_order)
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_24BGR;
            }
            else
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_24RGB;
            }
        }
        else
        #endif

        {
            // 24-bit unpacked format
            if (mpProject->mDisplays[display_index].reverse_order)
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_24BGRX;
            }
            else
            {
                mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_24XRGB;
            }
        }
        break;

    case 6:
        mpProject->mDisplays[display_index].bits_per_pix = 32;
        if (mpProject->mDisplays[display_index].reverse_order)
        {
            mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_32BGRA;
        }
        else
        {
            mpProject->mDisplays[display_index].colorformat = GX_COLOR_FORMAT_32ARGB;
        }
        break;
    }

    GX_BOOL color_format_changed = FALSE;

    if (old_color_format != mpProject->mDisplays[display_index].colorformat)
    {
        color_format_changed = TRUE;
    }

    if (color_format_changed)
    {
        GX_BOOL  ChangeColorTable = FALSE;
        GX_BOOL  ConvertCustomColorGroup = FALSE;

        if (!mpProject->mHeader.b_new_project)
        {
            Notify("The output color format has changed. The system color table will be remapped to the nearest match colors.", mpParent);
        }

        for (theme = 0; theme < MAX_THEMES; theme++)
        {
            GX_COLOR *old_pal = NULL;
            int old_palsize = 0;

            if(mpProject->mDisplays[display_index].themes[theme].palette)
            {
                /* Save old palette info. */
                old_pal = mpProject->mDisplays[display_index].themes[theme].palette;
                old_palsize = mpProject->mDisplays[display_index].themes[theme].palette_predefined;

                mpProject->mDisplays[display_index].themes[theme].palette = NULL;
                mpProject->mDisplays[display_index].themes[theme].palette_total_size = 0;
            }

            res_info *info = mpProject->mDisplays[display_index].themes[theme].GetFirstResourceInfo();


            if (info)
            {
                // If we are running at 8 bit palette mode, then we need to create a default palette
                if (mpProject->mDisplays[display_index].colorformat == GX_COLOR_FORMAT_8BIT_PALETTE)
                {
                    ProjectConfigDlg::CreateDefaultPalette(mpProject, display_index, theme);
                }

                // Convert the custom colors to the best match in the new output format.
                ProjectConfigDlg:: ColorTableConvert(info,
                                   old_pal,
                                   old_palsize,
                                   old_color_format,
                                   mpProject->mDisplays[display_index].themes[theme].palette,
                                   mpProject->mDisplays[display_index].themes[theme].palette_predefined,
                                   mpProject->mDisplays[display_index].colorformat);
            }

            if(old_pal)
            {
                delete [] old_pal;
                old_pal = NULL;
            }
        }
    }

    GX_BOOL cpu_type_changed = FALSE;
    GX_BOOL accelerator_disabled = FALSE;
    if (old_cpu_type != mpProject->mHeader.target_cpu)
    {
        cpu_type_changed = TRUE;
    }
    else
    {
        if ((IsCpuWithDave2D(mpProject->mHeader.target_cpu)) &&
            (mOldHeader.dave2d_graph_accelerator == TRUE) &&
            (mpProject->mHeader.dave2d_graph_accelerator == FALSE))
        {
            accelerator_disabled = TRUE;
        }
    }

    CComboBox* box = (CComboBox*)GetDlgItem(IDC_ROTATION_ANGLE_COMBO);
    if (box)
    {
        int sel = box->GetCurSel();
        mpProject->mDisplays[display_index].rotation_angle = box->GetItemData(sel);

        // This is just a double-safety check, we really should ever get here
        // but just to make sure we don't set flip rotation unless cpu_type is Renesas/Dave2D

        if (!IsCpuWithDave2D(mpProject))
        {
            if (mpProject->mDisplays[display_index].rotation_angle == GX_SCREEN_ROTATION_FLIP)
            {
                mpProject->mDisplays[display_index].rotation_angle = GX_SCREEN_ROTATION_NONE;
            }
        }
    }
    
    if (cpu_type_changed || color_format_changed || accelerator_disabled)
    {
        if (!mpProject->mDisplays[display_index].default_map_format)
        {
            // check to see if we have any pixelmaps that cannot be supported in new format:
            if (color_format_changed)
            {
                Notify("The output color format has been changed, resetting all the pixelmap formats to the default.", mpParent);
            }
            else if (cpu_type_changed)
            {
                Notify("The cpu type has been changed, resetting all the pixelmap formats to the default.", mpParent);
            }
            else
            {
                Notify("Graphic accelerator has been disabled, resetting all the pixelmap formats to the default.", mpParent);
            }

            for (theme = 0; theme < MAX_THEMES; theme++)
            {
                ProjectConfigDlg::ResetPixelmapFormat(mpProject->mDisplays[display_index].themes[theme].GetFirstResourceInfo(), display_index);
            }
        }
    }

    if (cpu_type_changed ||
        (color_format_changed && (mpProject->mDisplays[display_index].bits_per_pix <= 8)))
    {
        // check to see if we have any fonts that cannot be supported in new format:
        for (theme = 0; theme < MAX_THEMES; theme++)
        {
            ProjectConfigDlg::CheckFontFormat(mpProject->mDisplays[display_index].themes[theme].GetFirstResourceInfo(), display_index);
        }
    }

    if (color_format_changed && (mpProject->mDisplays[display_index].colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE))
    {
        // Text scroll wheel widget does not support rounding transform style when color depth is less than or equal to 8
        // Disable rounding style for text scroll wheel widgets
        folder_info *folder = mpProject->mDisplays[display_index].GetFirstChildFolder();

        while (folder)
        {
            ProjectConfigDlg::DisableTextScrollWheelRoundStyle(folder->GetFirstChildWidget());
            folder = folder->GetNextFolder();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int ProjectConfigWin::GetBitsSelected()
{
    int bits = 0;

    if (IsDlgButtonChecked(IDC_RADIO_1BPP))
    {
        bits = 1;
    }
    else if (IsDlgButtonChecked(IDC_RADIO_2BPP))
    {
        bits = 2;
    }
    else if (IsDlgButtonChecked(IDC_RADIO_4BPP))
    {
        bits = 4;
    }
    else if (IsDlgButtonChecked(IDC_RADIO_8BPP))
    {
        bits = 8;
    }
    else if (IsDlgButtonChecked(IDC_RADIO_16BPP))
    {
        bits = 16;
    }
    else if (IsDlgButtonChecked(IDC_RADIO_24BPP))
    {
        bits = 24;
    }
    else if (IsDlgButtonChecked(IDC_RADIO_32BPP))
    {
        bits = 32;
    }
    return bits;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::Format4444Clicked()
{
    if (GetBitsSelected() == 16)
    {
        if (((CButton *)GetDlgItem(IDC_CHECK_4444FORMAT))->GetCheck() == BST_CHECKED)
        {
            CheckDlgButton(IDC_CHECK_555FORMAT, FALSE);
        }

        EnableDisable(GetBitsSelected(), mpProject);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::Format1555Clicked()
{
    if (GetBitsSelected() == 16)
    {
        if (((CButton *)GetDlgItem(IDC_CHECK_555FORMAT))->GetCheck() == BST_CHECKED)
        {
            CheckDlgButton(IDC_CHECK_4444FORMAT, FALSE);
        }

        EnableDisable(GetBitsSelected(), mpProject);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::Format332Clicked()
{
    EnableDisableAATextColorsSetting();
}


///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::BitDepthChanged()
{
    EnableDisable(GetBitsSelected(), mpProject);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OffAndDisable(int id)
{
    GetDlgItem(id)->EnableWindow(FALSE);
    CheckDlgButton(id, FALSE);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::EnableDisableCCRX(int cpu_type)
{
    int index;

    if (cpu_type == CPU_RX)
    {
        // make sure CCRX is in the tools list:
        for (index = 0; index < mToolsCombo.GetCount(); index++)
        {
            if (mToolsCombo.GetItemData(index) == TOOLS_CCRX)
            {
                return;
            }
        }

        // Did not find CCRX, append it:
        index = mToolsCombo.InsertString(-1, _T("CCRX"));
        mToolsCombo.SetItemData(index, TOOLS_CCRX);
    }
    else
    {
        // If ccrx was selected, reset to generic
        if (mToolsCombo.GetItemData(mToolsCombo.GetCurSel()) == TOOLS_CCRX)
        {
            mpProject->mHeader.target_tools = TOOLS_GENERIC;
            SelectDropListItem(&mToolsCombo, mpProject->mHeader.target_tools);
        }
        
        // delete CCRX from the drop-down list:
        for (index = 0; index < mToolsCombo.GetCount(); index++)
        {
            if (mToolsCombo.GetItemData(index) == TOOLS_CCRX)
            {
                mToolsCombo.DeleteString(index);
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::EnableDisable(int bits, studiox_project *proj)
{
    int box_index = mCpuCombo.GetCurSel();
    int cpu_type = mCpuCombo.GetItemData(box_index);

    /* Disable 2bpp radio button. Because we don't support this for now.
       Diable it to avoid unforeseeing bug. */
    OffAndDisable(IDC_RADIO_2BPP);

    EnableDisableCCRX(cpu_type);

    if (cpu_type == CPU_SYNERGY)
    {
        OffAndDisable(IDC_RADIO_1BPP);
        OffAndDisable(IDC_RADIO_2BPP);
        OffAndDisable(IDC_RADIO_4BPP);
        OffAndDisable(IDC_CHECK_INVERTED);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_555FORMAT);
        OffAndDisable(IDC_CHECK_4444FORMAT);
        OffAndDisable(IDC_CHECK_332FORMAT);
    }
    else
    {
        GetDlgItem(IDC_RADIO_1BPP)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_4BPP)->EnableWindow(TRUE);
    }

    BOOL disable_reverse_order = TRUE;

    switch(bits)
    {
    case 1:
    case 2:
        GetDlgItem(IDC_CHECK_GRAYSCALE)->EnableWindow(FALSE);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_555FORMAT);
        OffAndDisable(IDC_CHECK_4444FORMAT);
        OffAndDisable(IDC_CHECK_332FORMAT);
        CheckDlgButton(IDC_CHECK_GRAYSCALE, TRUE);
        if (cpu_type != CPU_SYNERGY)
        {
            GetDlgItem(IDC_CHECK_INVERTED)->EnableWindow(TRUE);
        }
        break;

    case 4:
        /* Disable grayscale and inverted button.
           4bpp only support grayscale for now. */
        CheckDlgButton(IDC_CHECK_INVERTED, FALSE);
        CheckDlgButton(IDC_CHECK_GRAYSCALE, TRUE);
        GetDlgItem(IDC_CHECK_GRAYSCALE)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_INVERTED)->EnableWindow(FALSE);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_555FORMAT);
        OffAndDisable(IDC_CHECK_4444FORMAT);
        OffAndDisable(IDC_CHECK_332FORMAT);
        break;

    case 8:
        GetDlgItem(IDC_CHECK_INVERTED)->EnableWindow(FALSE);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_555FORMAT);
        OffAndDisable(IDC_CHECK_4444FORMAT);
        OffAndDisable(IDC_CHECK_GRAYSCALE);

        if (cpu_type != CPU_SYNERGY)
        {
            GetDlgItem(IDC_CHECK_332FORMAT)->EnableWindow(TRUE);
        }
        break;

    case 16:
        OffAndDisable(IDC_CHECK_GRAYSCALE);
        OffAndDisable(IDC_CHECK_INVERTED);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_332FORMAT);

        if (cpu_type != CPU_SYNERGY)
        {
            if ((!IsDlgButtonChecked(IDC_CHECK_4444FORMAT)) &&
                (!IsDlgButtonChecked(IDC_CHECK_555FORMAT)))
            {
                // Only 565rgb color format support reverse byte order feature
                GetDlgItem(IDC_CHECK_REVERSE_ORDER)->EnableWindow(TRUE);
                disable_reverse_order = FALSE;
            }

            GetDlgItem(IDC_CHECK_555FORMAT)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHECK_4444FORMAT)->EnableWindow(TRUE);
        }
        break;

    case 24:
        OffAndDisable(IDC_CHECK_GRAYSCALE);
        OffAndDisable(IDC_CHECK_INVERTED);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_555FORMAT);
        OffAndDisable(IDC_CHECK_4444FORMAT);
        OffAndDisable(IDC_CHECK_332FORMAT);
        #if defined(_DEBUG)
        if (cpu_type != CPU_SYNERGY)
        {
            GetDlgItem(IDC_CHECK_REVERSE_ORDER)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHECK_PACKED)->EnableWindow(TRUE);
        }
        #endif
        break;

    case 32:
        OffAndDisable(IDC_CHECK_GRAYSCALE);
        OffAndDisable(IDC_CHECK_INVERTED);
        OffAndDisable(IDC_CHECK_PACKED);
        OffAndDisable(IDC_CHECK_555FORMAT);
        OffAndDisable(IDC_CHECK_4444FORMAT);
        OffAndDisable(IDC_CHECK_332FORMAT);
        break;
    }

    if (disable_reverse_order)
    {
        OffAndDisable(IDC_CHECK_REVERSE_ORDER);
    }

    /* override the above logic for now, if we are not building in 
       Debug mode, disable the IDC_CHECK_PACKED because this format
       is not supported today. Remove this override once we add this
       support.

       Same is true for REVERSE_ORDER, this option is not truly supported by the
       resource generation logic today. 16/24/32 bit display data needs to be
       reversed, and we don't do that today, even though logic is in studiox_display_driver
       to instantiate reverse order display driver.
    */

    #if !defined(_DEBUG)
    OffAndDisable(IDC_CHECK_PACKED);
    #endif

    EnableDisableAATextColorsSetting();
}


// ProjectConfigWin message handlers

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEnEditNumScreens()
{
    int val = GetDlgItemInt(IDC_NUM_DISPLAYS);

    if (val > 0 && val < 5)
    {
        int SelectedDisplay;

        SelectedDisplay = GetDlgItemInt(IDC_SELECTED_DISPLAY);
        if (SelectedDisplay > val)
        {
            m_current_display = val - 1;
            InitDisplayConfig(m_current_display);
        }
        m_spin_current_screen.SetRange(1, val);
        m_spin_current_screen.Invalidate();

    }
    else
    {
        Notify("Invalid value for Number of Displays. Valid range is from 1 to 4.", mpParent);
        SetDlgItemInt(IDC_NUM_DISPLAYS, mpProject->mHeader.num_displays);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEnEditSelectedDisplay()
{
    //Check name when switch display.
    if (!TestInputName((CEdit*)GetDlgItem(IDC_SCREEN_NAME), "Name", mpProject->mDisplays[m_current_display].name, mpParent))
    {
        SetDlgItemInt(IDC_SELECTED_DISPLAY, m_current_display);
        return;
    }

    SaveDisplayConfig(m_current_display);

    int new_display = GetDlgItemInt(IDC_SELECTED_DISPLAY);
    int num_displays = GetDlgItemInt(IDC_NUM_DISPLAYS);

    if (new_display >= 1 && new_display <= num_displays)
    {
        m_current_display = new_display - 1;
        InitDisplayConfig(m_current_display);
    }
    else
    {
        ErrorMsg("Selected display must be between 1 and the total number of displays", mpParent);
        SetDlgItemInt(IDC_SELECTED_DISPLAY, m_current_display + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEnEditMajorVersion()
{
    int major_version = GetDlgItemInt(IDC_MAJOR_VERSION);
    int minor_version;
    int service_pack;

    if (major_version != 5 && major_version != 6)
    {
        ErrorMsg("Invalid major version number. Must be 5 or 6", mpParent);
        GuixVersionNumberToVersionFields(mpProject->mHeader.guix_version, major_version, minor_version, service_pack);
        SetDlgItemInt(IDC_MAJOR_VERSION, major_version);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEnEditMinorVersion()
{
    int minor_version = GetDlgItemInt(IDC_MINOR_VERSION);
    int major_version;
    int service_pack;

    if (minor_version < 0 || minor_version > 99)
    {
        ErrorMsg("Invalid minor version number. Must be a value between 0 and 99", mpParent);
        GuixVersionNumberToVersionFields(mpProject->mHeader.guix_version, major_version, minor_version, service_pack);

        SetDlgItemInt(IDC_MINOR_VERSION, minor_version);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEnEditServicePack()
{
    int service_pack = GetDlgItemInt(IDC_SERVICE_PACK);
    int major_version;
    int minor_version;

    if (service_pack < 0 || service_pack > 99)
    {
        ErrorMsg("Invalid service pack number. Must be a value between 0 and 99", mpParent);
        GuixVersionNumberToVersionFields(mpProject->mHeader.guix_version, major_version, minor_version, service_pack);

        SetDlgItemInt(IDC_SERVICE_PACK, service_pack);
    }

}

int bad_range = 0;

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar)
    {
        // The message is not sent by a standard scrollbar, return directly.
        return;
    }

    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar)
    {
        // The message is not sent by a standard scrollbar, return directly.
        return;
    }

    m_scroll_helper->OnHScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    m_scroll_helper->OnSize(nType, cx, cy);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnDeltaposSpinNumScreens(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here

    int upper;
    int lower;

    m_spin_num_screens.GetRange(lower, upper);

    int iVal = pNMUpDown ->iPos + pNMUpDown->iDelta;
    if (iVal >= lower && iVal <= upper)
    {
        m_spin_current_screen.SetRange(1, iVal);

        m_spin_current_screen.Invalidate();

        int SelectedDisplay;

        SelectedDisplay = GetDlgItemInt(IDC_SELECTED_DISPLAY);
        if (SelectedDisplay > iVal)
        {
            m_current_display = iVal - 1;
            InitDisplayConfig(m_current_display);
        }
        *pResult = 0;
    }
    else
    {
        *pResult = 1;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnDeltaposSpinScreenSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
    int upper;
    int lower;
    
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    m_spin_current_screen.GetRange(lower, upper);
    int iVal = pNMUpDown ->iPos;

    if (iVal <= upper && iVal >= lower)
    {
        //Check name when switch display.
        if (!TestInputName((CEdit *)GetDlgItem(IDC_SCREEN_NAME), "Name", mpProject->mDisplays[iVal - 1].name, mpParent))
        {
            *pResult = 1;
            return;
        }

        SaveDisplayConfig(iVal - 1);
    
        iVal += pNMUpDown->iDelta;
   
        if (iVal <= upper && iVal >= lower)
        {
            m_current_display = iVal - 1;
            InitDisplayConfig(m_current_display);
            *pResult = 0;
        }
        else
        {
            *pResult = 1;
        }
    }
    else
    {
        *pResult = 1;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnBnClickedBrowseSource()
{
    // TODO: Add your control notification handler code here
    TCHAR path[MAX_PATH];
    CString relativepath;
    if (BrowseForFolder(m_hWnd, NULL, NULL, path))
    {
        relativepath = CString(path);
        ConvertToProjectRelativePath(relativepath);
        SetDlgItemText(IDC_SOURCE_PATH, relativepath);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnBnClickedBrowseHeader()
{
    TCHAR path[MAX_PATH];
    CString relativepath;
    if (BrowseForFolder(m_hWnd, NULL, NULL, path))
    {
        relativepath = CString(path);
        ConvertToProjectRelativePath(relativepath);
        SetDlgItemText(IDC_HEADER_PATH, relativepath);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnBnClickedBrowseResource()
{
    TCHAR path[MAX_PATH];
    CString relativepath;

    if (BrowseForFolder(m_hWnd, NULL, NULL, path))
    {
        relativepath = CString(path);
        ConvertToProjectRelativePath(relativepath);
        SetDlgItemText(IDC_RESOURCE_PATH, relativepath);
    }
}


///////////////////////////////////////////////////////////////////////////////
int ProjectConfigWin::GetGuixVersion()
{
    return mGuixVersion;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnAdvancedSettings()
{
    int box_index = mCpuCombo.GetCurSel();
    int cpu_type = mCpuCombo.GetItemData(box_index);

    if (IsCpuWithDave2D(cpu_type))
    {
        synergy_settings_dlg dlg(mpParent, m_current_display);

        dlg.SetOwner(mpParent);

        if (dlg.DoModal() == IDOK)
        {
            Invalidate();
            mpProject->SetModified();
        }
    }

    EnableDisableAATextColorsSetting();
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnChangeSelectedCPU()
{
    int box_index = mCpuCombo.GetCurSel();
    int cpu_type = mCpuCombo.GetItemData(box_index);

    EnableDisableAdvancedSettings(cpu_type);
    EnableDisableFlipRotation(cpu_type);

    EnableDisableCCRX(cpu_type);

    if (IsCpuWithDave2D(cpu_type))
    {
        switch(GetBitsSelected())
        {
        case 8:
        case 16:
        case 24:
        case 32:
            break;

        default:
            CheckDlgButton(IDC_RADIO_16BPP, TRUE);
            break;
        }
        mpProject->mHeader.dave2d_graph_accelerator = TRUE;
        mpProject->mHeader.renesas_jpeg_decoder = DECODER_TYPE_HW;
        mpProject->mHeader.renesas_png_decoder = DECODER_TYPE_NONE;

    }
    else
    {
        mpProject->mHeader.dave2d_graph_accelerator = FALSE;
        mpProject->mHeader.renesas_jpeg_decoder = DECODER_TYPE_NONE;
        mpProject->mHeader.renesas_png_decoder = DECODER_TYPE_NONE;
    }

    // run this to enable/disable fields that CPU does or does not support
    BitDepthChanged();
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEditXRes()
{
    int xres;

    xres = GetDlgItemInt(IDC_X_RESOLUTION);

    if (xres > GX_MAX_DISPLAY_RESOLUTION)
    {
        Notify("X resolution is out of range, revert to previous value.", mpParent);
        SetDlgItemInt(IDC_X_RESOLUTION, mpProject->mDisplays[m_current_display].xres);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::OnEditYRes()
{
    int yres;

    yres = GetDlgItemInt(IDC_Y_RESOLUTION);

    if (yres > GX_MAX_DISPLAY_RESOLUTION)
    {
        Notify("Y resolution is out of range, revert to previous value.", mpParent);
        SetDlgItemInt(IDC_Y_RESOLUTION, mpProject->mDisplays[m_current_display].yres);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::EnableDisableAdvancedSettings(int cpu_type)
{
    if (IsCpuWithDave2D(cpu_type))
    {
        GetDlgItem(IDC_ADVANCED_SETTINGS)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_ADVANCED_SETTINGS)->EnableWindow(FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::EnableDisableAATextColorsSetting()
{
    int bits = GetBitsSelected();
    int box_index = mCpuCombo.GetCurSel();
    int cpu_type = mCpuCombo.GetItemData(box_index);
    int enable = FALSE;

    if ((bits == 8) && (!IsDlgButtonChecked(IDC_CHECK_332FORMAT)))
    {
        if ((!IsDave2dFontFormat(mpProject, m_current_display)) ||
            (!mpProject->mHeader.dave2d_graph_accelerator))
        {
            enable = TRUE;
        }
    }

    GetDlgItem(IDC_AA_TEXT_COLORS_COMBO)->EnableWindow(enable);
    GetDlgItem(IDC_AA_TEXT_COLORS_LABEL)->EnableWindow(enable);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::InitAATextColorsCombobox()
{
    CComboBox *box = (CComboBox *)GetDlgItem(IDC_AA_TEXT_COLORS_COMBO);

    if (!box->GetCount())
    {
        box->InsertString(0, _T("16"));
        box->SetItemData(0, 16);

        box->InsertString(1, _T("8"));
        box->SetItemData(1, 8);
    }

    CString text;
    text.Format(_T("%d"), mpProject->mHeader.palette_mode_aa_text_colors);
    box->SelectString(0, text);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::EnableDisableFlipRotation(int cpu_type)
{
    CComboBox* box = (CComboBox*)GetDlgItem(IDC_ROTATION_ANGLE_COMBO);

    if (box)
    {
        if (IsCpuWithDave2D(cpu_type))
        {
            if (box->GetCount() < 4)
            {
                box->InsertString(3, _T("FLIP"));
                box->SetItemData(3, GX_SCREEN_ROTATION_FLIP);
            }
        }
        else
        {   
            if (box->GetCount() > 3)
            {
                box->DeleteString(3);
                box->SetCurSel(0);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigWin::InitRotationAnglesCombobox(int display)
{
    int box_index = mCpuCombo.GetCurSel();
    int cpu_type = mCpuCombo.GetItemData(box_index);

    CComboBox* box = (CComboBox*)GetDlgItem(IDC_ROTATION_ANGLE_COMBO);

    if (box)
    {
        if (!box->GetCount())
        {
            box->InsertString(0, ProjectConfigDlg::FindScreenRotationName(GX_SCREEN_ROTATION_NONE));
            box->SetItemData(0, 0);

            box->InsertString(1, ProjectConfigDlg::FindScreenRotationName(GX_SCREEN_ROTATION_CW));
            box->SetItemData(1, GX_SCREEN_ROTATION_CW);

            box->InsertString(2, ProjectConfigDlg::FindScreenRotationName(GX_SCREEN_ROTATION_CCW));
            box->SetItemData(2, GX_SCREEN_ROTATION_CCW);
        }

        if (IsCpuWithDave2D(cpu_type))
        {
            if (box->GetCount() < 4)
            {
                box->InsertString(3, ProjectConfigDlg::FindScreenRotationName(GX_SCREEN_ROTATION_FLIP));
                box->SetItemData(3, GX_SCREEN_ROTATION_FLIP);
            }
        }
        else
        {   
            if (box->GetCount() > 3)
            {
                box->DeleteString(3);
                if (mpProject->mDisplays[display].rotation_angle == GX_SCREEN_ROTATION_FLIP)
                {
                    mpProject->mDisplays[display].rotation_angle = GX_SCREEN_ROTATION_NONE;
                }
            }
        }

        switch (mpProject->mDisplays[display].rotation_angle)
        {
        case GX_SCREEN_ROTATION_CW:
            box->SetCurSel(1);
            break;

        case GX_SCREEN_ROTATION_CCW:
            box->SetCurSel(2);
            break;

        case GX_SCREEN_ROTATION_FLIP:
            box->SetCurSel(3);
            break;

        default:
            box->SetCurSel(0);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL ProjectConfigWin::Save()
{
    CWnd* child = GetFocus();
    if (child)
    {
        // if an edit field has input focus, move focus so that
        // value validation runs
        int id = child->GetDlgCtrlID();
        switch (id)
        {
        case IDC_NUM_DISPLAYS:
        case IDC_SELECTED_DISPLAY:
        case IDC_MAJOR_VERSION:
        case IDC_MINOR_VERSION:
        case IDC_SERVICE_PACK:
            SetFocus();
            return FALSE;

        default:
            break;
        }
    }

    CEdit* edit = (CEdit *)GetDlgItem(IDC_SCREEN_NAME);
    INT display_index = GetDlgItemInt(IDC_SELECTED_DISPLAY, NULL, GX_TRUE) - 1;

    if (TestInputName(edit, "Name", mpProject->mDisplays[display_index].name, mpParent))
    {
        CDialog::OnOK();
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL ProjectConfigWin::Cancel()
{
    mpProject->mHeader = mOldHeader;
    CDialog::OnCancel();
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

BOOL ProjectConfigWin::PreTranslateMessage(MSG *pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    CWnd *focus_owner = GetFocus();

    if (focus_owner)
    {
        int ctrl_id = focus_owner->GetDlgCtrlID();

        switch (ctrl_id)
        {
        case IDC_NUM_DISPLAYS:
        case IDC_SELECTED_DISPLAY:
        case IDC_MAJOR_VERSION:
        case IDC_MINOR_VERSION:
        case IDC_SERVICE_PACK:
        case IDC_X_RESOLUTION:
        case IDC_Y_RESOLUTION:
            if (pMsg->message == WM_CHAR)
            {
                if (isprint(pMsg->wParam))
                {
                    if (pMsg->wParam < '0' || pMsg->wParam > '9')
                    {
                        ErrorMsg("Unacceptable Character. You can only type a number here.", mpParent);
                        return TRUE;
                    }
                }
            }
            break;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT ProjectConfigWin::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;
    INT ctrl_id;
    studiox_project *project;
    NMUPDOWN NMUpDown;
    NMHDR hdr;

    switch (wParam)
    {
    case TEST_CONFIGURE_DISPLAY_NUM:
        hdr.hwndFrom = m_spin_num_screens.m_hWnd;
        hdr.idFrom = IDC_SPIN_NUM_SCREENS;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = m_spin_num_screens.GetPos();
        NMUpDown.iDelta = lParam - NMUpDown.iPos;

        m_spin_num_screens.SetPos(lParam);
        SetDlgItemInt(IDC_LANGUAGE_INDEX, lParam);
        SendMessage(WM_NOTIFY, IDC_SPIN_NUM_SCREENS, (LPARAM)&NMUpDown);
        break;

    case TEST_SELECT_DISPLAY_INDEX:
        hdr.hwndFrom = m_spin_current_screen.m_hWnd;
        hdr.idFrom = IDC_SPIN_NUM_SCREENS;
        hdr.code = UDN_DELTAPOS;

        NMUpDown.hdr = hdr;
        NMUpDown.iPos = m_spin_current_screen.GetPos();
        NMUpDown.iDelta = lParam - NMUpDown.iPos;

        m_spin_current_screen.SetPos(lParam);
        SetDlgItemInt(IDC_LANGUAGE_INDEX, lParam);
        SendMessage(WM_NOTIFY, IDC_SPIN_NUM_SCREENS, (LPARAM)&NMUpDown);
        break;

    case TEST_CONFIGURE_SOURCE_PATH:
        SetDlgItemText(IDC_SOURCE_PATH, GetTestingParam(0));
        break;

    case TEST_CONFIGURE_HEADER_PATH:
        SetDlgItemText(IDC_HEADER_PATH, GetTestingParam(0));
        break;

    case TEST_CONFIGURE_RESOURCE_PATH:
        SetDlgItemText(IDC_RESOURCE_PATH, GetTestingParam(0));
        break;

    case TEST_CONFIGURE_X_RES:
        SetDlgItemText(IDC_X_RESOLUTION, GetTestingParam(0));
        break;

    case TEST_CONFIGURE_Y_RES:
        SetDlgItemText(IDC_Y_RESOLUTION, GetTestingParam(0));
        break;

    case TEST_CONFIGURE_DISPLAY_NAME:
        SetDlgItemText(IDC_SCREEN_NAME, GetTestingParam(0));
        break;

    case TEST_CONFIGURE_DISPLAY_COLOR_FORMAT:
        switch (lParam)
        {
        case GX_COLOR_FORMAT_MONOCHROME:
            ctrl_id = IDC_RADIO_1BPP;
            break;

        case GX_COLOR_FORMAT_4BIT_GRAY:
            ctrl_id = IDC_RADIO_4BPP;
            break;

        case GX_COLOR_FORMAT_8BIT_PALETTE:
            ctrl_id = IDC_RADIO_8BPP;
            break;

        case GX_COLOR_FORMAT_565RGB:
        case GX_COLOR_FORMAT_4444ARGB:
            ctrl_id = IDC_RADIO_16BPP;
            break;

        case GX_COLOR_FORMAT_24XRGB:
            ctrl_id = IDC_RADIO_24BPP;
            break;

        case GX_COLOR_FORMAT_32ARGB:
            ctrl_id = IDC_RADIO_32BPP;
            break;
        }
        CheckRadioButton(IDC_RADIO_1BPP, IDC_RADIO_32BPP, ctrl_id);

        if (lParam == GX_COLOR_FORMAT_4444ARGB)
        {
            pWnd = (CButton *)GetDlgItem(IDC_CHECK_4444FORMAT);
            ((CButton *)pWnd)->SetCheck(TRUE);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ALLOCATE_CANVAS, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_CONFIGURE_DISPLAY_COLOR_DEPTH:
    {
        ULONG id_control = 0;
        switch (lParam)
        {
        case 1:
            id_control = IDC_RADIO_1BPP;
            break;

        case 2:
            id_control = IDC_RADIO_2BPP;
            break;

        case 4:
            id_control = IDC_RADIO_4BPP;
            break;

        case 8:
            id_control = IDC_RADIO_8BPP;
            break;

        case 16:
            id_control = IDC_RADIO_16BPP;
            break;

        case 24:
            id_control = IDC_RADIO_24BPP;
            break;

        case 32:
            id_control = IDC_RADIO_32BPP;
            break;
        }
        if (id_control)
        {
            CheckRadioButton(IDC_RADIO_1BPP, IDC_RADIO_32BPP, id_control);
            SendMessage(WM_COMMAND, MAKEWPARAM(id_control, BN_CLICKED), 0);
        }

        int id = GetCheckedRadioButton(IDC_RADIO_1BPP, IDC_RADIO_32BPP);
        if (id != id_control)
        {
            return GX_FAILURE;
        }
    }
        break;

    case TEST_CONFIGURE_DISPLAY_RGB_BITS:
    {
        ULONG id_control = 0;
        switch (lParam)
        {
        case 4444:
            id_control = IDC_CHECK_4444FORMAT;
            break;

        case 1555:
            id_control = IDC_CHECK_555FORMAT;
            break;

        case 332:
            id_control = IDC_CHECK_332FORMAT;
            break;

        default:
            break;
        }
        if (id_control)
        {
            pWnd = (CButton *)GetDlgItem(id_control);
            if (((CButton *)pWnd)->IsWindowEnabled())
            {
                if (((CButton *)pWnd)->GetCheck() == BST_CHECKED)
                {
                    ((CButton *)pWnd)->SetCheck(BST_UNCHECKED);
                }
                else
                {
                    ((CButton *)pWnd)->SetCheck(BST_CHECKED);
                }
                SendMessage(WM_COMMAND, MAKEWPARAM(id_control, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
            }
            else
            {
                return GX_FAILURE;
            }
        }
    }
        break;

    case TEST_CONFIGURE_MINOR_VERSION:
        mSpinMinorVersion.SetPos(lParam);
        SetDlgItemInt(IDC_MINOR_VERSION, lParam);
        break;

    case TEST_CONFIGURE_SERVICE_PACK:
        mSpinServicePack.SetPos(lParam);
        SetDlgItemInt(IDC_SERVICE_PACK, lParam);
        break;

    case TEST_CONFIGURE_CANVAS_ALLOCATE:
        pWnd = (CButton *)GetDlgItem(IDC_ALLOCATE_CANVAS);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ALLOCATE_CANVAS, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CONFIGURE_CPU_TYPE:
    {
        pWnd = GetDlgItem(IDC_CPU_COMBO);
        STRING_VAL_PAIR*pEntry = cpu_list;

        while (!pEntry->name.IsEmpty())
        {
            if (pEntry->val == lParam)
            {
                ((CComboBox *)pWnd)->SelectString(0, pEntry->name);
                SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CPU_COMBO, CBN_SELCHANGE), (LPARAM)((CComboBox *)pWnd)->m_hWnd);
            }
            pEntry++;
        }
        break;
    }

    case TEST_CONFIGURE_BIG_ENDIAN:
        pWnd = (CButton *)GetDlgItem(IDC_BIG_ENDIAN);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BIG_ENDIAN, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_IS_DEFAULT_MAP_FORMAT:
        project = GetOpenProject();
        return project->mDisplays[m_current_display].default_map_format;

    case TEST_GET_CPU_TYPE:
        project = GetOpenProject();
        return project->mHeader.target_cpu;

    case TEST_GET_DISPLAY_COLOR_FORMAT:
        project = GetOpenProject();
        return project->mDisplays[m_current_display].colorformat;

    case TEST_GET_DISPLAY_COLOR_DEPTH:
        project = GetOpenProject();
        return project->mDisplays[m_current_display].bits_per_pix;

    case TEST_IS_1555_FORMAT:
        project = GetOpenProject();
        return  project->mDisplays[m_current_display].format_555;
        break;

    case TEST_IS_4444_FORAMT:
        project = GetOpenProject();
        return  project->mDisplays[m_current_display].format_4444;
        break;

    case TEST_IS_332_FORMAT:
        project = GetOpenProject();
        return  project->mDisplays[m_current_display].format_332;
        break;

    case TEST_IS_NEW_PROJECT:
        project = GetOpenProject();
        return project->mHeader.b_new_project;

    case TEST_OPEN_ADVANCED_SETTINGS:
        OnAdvancedSettings();
        break;

    case TEST_SET_AA_TEXT_COLORS:
        pWnd = GetDlgItem(IDC_AA_TEXT_COLORS_COMBO);
        ((CComboBox *)pWnd)->SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_AA_TEXT_COLORS_COMBO, CBN_SELCHANGE), (LPARAM)pWnd->m_hWnd);
        break;
    }

    return 0;
}

// ProjectConfigDlg dialog

IMPLEMENT_DYNAMIC(ProjectConfigDlg, express_dialog)

BEGIN_MESSAGE_MAP(ProjectConfigDlg, express_dialog)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
ProjectConfigDlg::ProjectConfigDlg(CWnd* pParent, int current_display)
    : express_dialog(ProjectConfigDlg::IDD, pParent)
{
    IconId = IDB_CONFIGURE_PROJECTS;
    SetTitleText("Configure Project");

    if (current_display >= 0)
    {
        mCurrentDisplay = current_display;
    }
    else
    {
        mCurrentDisplay = 0;
    }

    mpProjectConfigWin = NULL;
}

///////////////////////////////////////////////////////////////////////////////
ProjectConfigDlg::~ProjectConfigDlg()
{
    if (mpProjectConfigWin)
    {
        delete mpProjectConfigWin;
    }
}


/////////////////////////////////////////////////////////////////////////////
COLOR_RECORD* ProjectConfigDlg::GetDefaultColorTable(int color_format)
{
    COLOR_RECORD* default_record = GX_NULL;


    if (color_format <= GX_COLOR_FORMAT_4BIT_VGA)
    {
        if (color_format == GX_COLOR_FORMAT_MONOCHROME)
        {
            default_record = DEFAULT_COLOR_TABLE_MONOCHROME;
        }
        else
        {
            default_record = DEFAULT_COLOR_TABLE_GRAYSCALE;
        }
    }
    else
    {
        default_record = DEFAULT_COLOR_TABLE;
    }

    return default_record;
}

/////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::ColorTableConvert(res_info* info, GX_COLOR* old_pal, int old_palsize, int old_color_format, GX_COLOR* new_pal, int new_palsize, int new_color_format)
{
    // find the color resource with a system id, set them to our default
    // system color rgb values
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    INT res_id;
    INT display_index = project->GetDisplayIndex(info);
    COLOR_RECORD* new_default_record = GetDefaultColorTable(new_color_format);
    COLOR_RECORD* old_default_record = GetDefaultColorTable(old_color_format);
    COLOR_RECORD* new_record;
    COLOR_RECORD* old_record;

    if (!new_default_record || !old_default_record)
    {
        return;
    }

    GX_COLOR rgb_color = 0;
    GX_COLOR colorval = 0;

    while (info)
    {
        if (info->type == RES_TYPE_COLOR)
        {
            res_id = project->GetResourceId(display_index, info);

            if (res_id < GX_MAX_DEFAULT_COLORS)
            {
                new_record = new_default_record;
                old_record = old_default_record;

                while (new_record->name)
                {
                    if (res_id == new_record->color_id)
                    {
                        rgb_color = new_record->rgb_val;

                        // Get system rgb color.
                        if (new_default_record == old_default_record)
                        {
                            colorval = GetColorVal(old_record->rgb_val, old_pal, old_palsize, old_color_format);

                            if (colorval != info->colorval)
                            {
                                // If system color is modified.
                                rgb_color = GetRGBColor(info->colorval, old_pal, old_palsize, old_color_format);
                            }
                        }
                        break;
                    }

                    new_record++;
                    old_record++;
                }

            }
            else
            {
                // Get custom rgb color.
                rgb_color = GetRGBColor(info->colorval, old_pal, old_palsize, old_color_format);
            }

            // Convert rgb color to colorval.
            info->colorval = GetColorVal(rgb_color, new_pal, new_palsize, new_color_format);
        }
        if (info->child)
        {
            ColorTableConvert(info->child, old_pal, old_palsize, old_color_format, new_pal, new_palsize, new_color_format);
        }
        info = info->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::CreateDefault1BppPalette(palette_info& info)
{
    INT pixel = 0;

    info.palette = new GX_COLOR[2];
    info.palette[0] = 0xff000000;
    info.palette[1] = 0xffffffff;

    info.total_size = 2;
    info.used_size = 2;
}


///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::CreateDefault4BppPalette(palette_info& info)
{
    INT pixel = 0;
    GX_COLOR val;

    info.palette = new GX_COLOR[16];
    GX_COLOR* put = info.palette;
    memset(put, 0, 16 * sizeof(GX_COLOR));

    for (pixel = 0; pixel <= 0xff; pixel += 0x11)
    {
        val = ((0xff000000) | (pixel << 16) | (pixel << 8) | pixel);
        *put++ = val;
    }

    info.total_size = 16;
    info.used_size = 16;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::CreateDefault8BppPalette(palette_info& info)
{
    ULONG gray;
    GX_COLOR val;
    int pre_def_size = 0;
    COLOR_RECORD* def_record = DEFAULT_COLOR_TABLE;

    info.palette = new GX_COLOR[256];
    GX_COLOR* put = info.palette;
    memset(put, 0, 256 * sizeof(GX_COLOR));

    // 16 shades of gray:
    for (gray = 0; gray <= 0xff; gray += 0x11)
    {
        val = ((0xff000000) | (gray << 16) | (gray << 8) | gray);
        *put++ = val;
        pre_def_size++;
    }

    while (def_record->name)
    {
        *put++ = def_record->rgb_val;
        pre_def_size++;
        def_record++;
    }

    info.total_size = 256;
    info.used_size = pre_def_size;
}


///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::CreateDefault332RGBPalette(palette_info& info)
{
    INT                           r = 0;
    INT                           g = 0;
    INT                           b = 0;
    INT                           red;
    INT                           green;
    INT                           blue;

    info.palette = new GX_COLOR[256];
    GX_COLOR* put = info.palette;
    memset(put, 0, 256 * sizeof(GX_COLOR));

    for (r = 0; r < 8; r++)
    {
        red = r << 21;
        if (red & 0x200000) { red |= 0x1f0000; }
        for (g = 0; g < 8; g++)
        {
            green = g << 13;
            if (green & 0x2000) { green |= 0x1f00; }
            for (b = 0; b < 4; b++)
            {
                blue = b << 6;
                if (blue & 0x40) { blue |= 0x3f; }
                *put++ = red | green | blue | 0xff000000;
            }
        }
    }

    info.total_size = 256;
    info.used_size = 256;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::CreateDefaultPalette(studiox_project* project,
    int display, int theme)
{
    palette_info info;

    if (project->mDisplays[display].themes[theme].palette)
    {
        delete[] project->mDisplays[display].themes[theme].palette;
        project->mDisplays[display].themes[theme].palette = NULL;
        project->mDisplays[display].themes[theme].palette_total_size = 0;
        project->mDisplays[display].themes[theme].palette_predefined = 0;
    }

    switch (project->mDisplays[display].colorformat)
    {
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32RGBA:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32BGRA:
        if (project->mDisplays[display].themes[theme].palette)
        {
            delete[] project->mDisplays[display].themes[theme].palette;
            project->mDisplays[display].themes[theme].palette = GX_NULL;
        }

        project->mDisplays[display].themes[theme].palette = new GX_COLOR[255];
        memset(project->mDisplays[display].themes[theme].palette, 0, 255 * sizeof(GX_COLOR));
        project->mDisplays[display].themes[theme].palette_total_size = 255;
        project->mDisplays[display].themes[theme].palette_predefined = 0;
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        CreateDefault8BppPalette(info);
        project->mDisplays[display].themes[theme].palette = info.palette;
        project->mDisplays[display].themes[theme].palette_total_size = info.total_size;
        project->mDisplays[display].themes[theme].palette_predefined = info.used_size;
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_4BIT_GRAY:
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
GX_COLOR ProjectConfigDlg::GetRGBColor(GX_COLOR colorval, GX_COLOR* old_pal, int old_palsize, int old_color_format)
{
    GX_COLOR rgb_color = 0xff000000;

    switch (old_color_format)
    {
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (old_pal && ((INT)colorval < old_palsize))
        {
            rgb_color |= old_pal[colorval];
        }
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_4BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
        rgb_color |= colorval;
        break;

    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32RGBA:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32BGRA:
        rgb_color = colorval;
        break;
    }

    return rgb_color;
}

///////////////////////////////////////////////////////////////////////////////////
GX_COLOR ProjectConfigDlg::GetColorVal(GX_COLOR rgb_color, GX_COLOR* new_pal, int new_palsize, int new_color_format)
{
    GX_COLOR colorval = 0;

    switch (new_color_format)
    {
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (new_pal)
        {
            colorval = image_reader::GetNearestPaletteColor(rgb_color, new_pal, new_palsize);
        }
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        image_reader::ConvertRGBToGray(rgb_color, (GX_UBYTE*)&colorval);
        if ((colorval & 0xff) < 128)
        {
            colorval = 0x00;
        }
        else
        {
            colorval = 0xffffff;
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        image_reader::ConvertRGBToGray(rgb_color, (GX_UBYTE*)&colorval);
        colorval |= colorval << 4;
        colorval = (colorval << 16) | (colorval << 8) | colorval;
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        colorval = rgb_color;

        if (colorval & 0x200000) { colorval |= 0x1f0000; } // extend red value, extend last bit of red of 332 mode to the rest red-bits.
        if (colorval & 0x2000) { colorval |= 0x1f00; } // set green value;
        if (colorval & 0x40) { colorval |= 0x3f; } // set blue value;
        break;

    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32RGBA:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32BGRA:
    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
        colorval = rgb_color;
        break;
    }
    return colorval;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::CheckFontFormat(res_info* start, int m_display)
{
    studiox_project* project = GetOpenProject();

    int display_bits_per_pix = project->mDisplays[m_display].bits_per_pix;

    while (start)
    {
        if (start->child)
        {
            CheckFontFormat(start->child, m_display);
        }

        if (start->type == RES_TYPE_FONT)
        {
            if ((!IsRenesasDave2D(project)) ||
                (project->mDisplays[m_display].colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE))
            {
                if (start->compress)
                {
                    start->compress = FALSE;
                }
            }

            switch (start->font_bits)
            {
            case 8:
                if (display_bits_per_pix < 16)
                {
                    start->font_bits = 1;
                }
                break;

            case 4:
                if (display_bits_per_pix < 8)
                {
                    start->font_bits = 1;
                }
                break;

            default:
                break;
            }
        }
        start = start->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::DisableTextScrollWheelRoundStyle(widget_info* start)
{
    widget_info* child;

    while (start)
    {
        child = start->GetChildWidgetInfo();

        if (child)
        {
            DisableTextScrollWheelRoundStyle(child);
        }

        if (start->basetype == GX_TYPE_STRING_SCROLL_WHEEL ||
            start->basetype == GX_TYPE_NUMERIC_SCROLL_WHEEL)
        {
            start->style &= (~GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        }

        start = start->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
int ProjectConfigDlg::GetGuixVersion()
{
    return mpProjectConfigWin->GetGuixVersion();
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::ResetPixelmapFormat(res_info* start, int m_display)
{
    studiox_project* project = GetOpenProject();
    if (project)
    {
        while (start)
        {
            if (start->child)
            {
                ResetPixelmapFormat(start->child, m_display);
            }

            if (start->type == RES_TYPE_PIXELMAP)
            {
                start->output_color_format = 0;
                if (project->mDisplays[m_display].colorformat == GX_COLOR_FORMAT_8BIT_PALETTE)
                {
                    start->palette_type = PALETTE_TYPE_SHARED;
                }
                else
                {
                    start->palette_type = PALETTE_TYPE_NONE;
                }
            }
            start = start->next;
        }
        project->mDisplays[m_display].default_map_format = TRUE;
    }
}

///////////////////////////////////////////////////////////////////////////////
CString ProjectConfigDlg::FindTargetCPUName(int val)
{
    return FindPairString(cpu_list, val);
}

int ProjectConfigDlg::FindTargetCPUVal(CString name)
{
    return FindPairVal(cpu_list, name);
}

///////////////////////////////////////////////////////////////////////////////
CString ProjectConfigDlg::FindTargetCompilerName(int val)
{
    return FindPairString(compiler_list, val);
}

///////////////////////////////////////////////////////////////////////////////
int ProjectConfigDlg::FindTargetCompilerVal(CString name)
{
    return FindPairVal(compiler_list, name);
}

///////////////////////////////////////////////////////////////////////////////
CString ProjectConfigDlg::FindScreenRotationName(int val)
{
    return FindPairString(screen_rotation_list, val);
}

///////////////////////////////////////////////////////////////////////////////
int ProjectConfigDlg::FindScreenRotationVal(CString name)
{
    return FindPairVal(screen_rotation_list, name);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::PositionChildren()
{
    if (mpProjectConfigWin)
    {
        CRect size;
        GetClientRect(size);

        size.top += m_title_bar_height;
        size.bottom -= m_status_bar_height;

        mpProjectConfigWin->MoveWindow(&size);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL ProjectConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    mpProjectConfigWin = new ProjectConfigWin(this, mCurrentDisplay);

    AddCancelButton();
    AddSaveButton();

    PositionChildren();
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::OnOK()
{
    if (mpProjectConfigWin->Save())
    {

        express_dialog::OnOK();
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::OnCancel()
{
    if (mpProjectConfigWin->Cancel())
    {

        express_dialog::OnCancel();
    }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::OnSize(UINT nType, int cx, int cy)
{
    express_dialog::OnSize(nType, cx, cy);

    PositionChildren();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void ProjectConfigDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    express_dialog::OnGetMinMaxInfo(lpMMI);
    CRect client;
    GetClientRect(&client);

    lpMMI->ptMinTrackSize.x = MIN_PROJECT_CONFIGURE_DLG_WIDTH;
    lpMMI->ptMinTrackSize.y = MIN_PROJECT_CONFIGURE_DLG_HEIGHT;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT ProjectConfigDlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_SAVE_PROJECT_CONFIGURE:
        OnOK();
        break;

    case TEST_CANCEL_PROJECT_CONFIGURE:
        OnCancel();
        break;

    default:
        return mpProjectConfigWin->OnTestMessage(wParam, lParam);
    }

    return 0;
}
