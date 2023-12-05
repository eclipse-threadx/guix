// edit_pixelmap_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "studiox.h"
#include "edit_pixelmap_dlg.h"

static int _32bpp_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_8BIT_PALETTE,
    GX_COLOR_FORMAT_565RGB,
    GX_COLOR_FORMAT_4444ARGB,
    GX_COLOR_FORMAT_24XRGB,
    GX_COLOR_FORMAT_32ARGB,
    0,
};

static int _332rgb_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_8BIT_PACKED_PIXEL,
    0,
};

static int _4444argb_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_4444ARGB,
    0,
};

static int _565rgb_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_8BIT_PALETTE,
    GX_COLOR_FORMAT_565RGB,
    GX_COLOR_FORMAT_4444ARGB,
    0,
};

static int _565rgb_synergy_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_8BIT_PALETTE,
    GX_COLOR_FORMAT_565RGB,
    GX_COLOR_FORMAT_4444ARGB,
    GX_COLOR_FORMAT_32ARGB,
    0
};

static int _565rgb_st_chromeart_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_8BIT_PALETTE,
    GX_COLOR_FORMAT_565RGB,
    GX_COLOR_FORMAT_4444ARGB,
    GX_COLOR_FORMAT_32ARGB,
    0
};

static int _1555xrgb_allowed_formats[] = {
    GX_COLOR_FORMAT_8BIT_ALPHAMAP,
    GX_COLOR_FORMAT_1555XRGB,
    0
};

STRING_VAL_PAIR PaletteTypes[] = {
    { _T("None"), PALETTE_TYPE_NONE },
    { _T("Local/Private Palette"), PALETTE_TYPE_PRIVATE },
    { _T("Global/Shared Palette"), PALETTE_TYPE_SHARED },
    { _T(""), 0 }
};

enum edit_pixelmap_dlg_test_commands{
    TEST_SET_IMAGE_PATH = 1,
    TEST_SET_IMAGE_NAME,
    TEST_CHECK_CUSTOM_OUTPUT,
    TEST_SET_CUSTOM_OUTPUT_FILE,
    TEST_CHECK_RAW_FORMAT,
    TEST_CHECK_COMPRESS,
    TEST_CHECK_ALPHA,
    TEST_CHECK_DITHER,
    TEST_SET_OUTPUT_FORMAT,
    TEST_SET_PALETTE_TYPE,
    TEST_SAVE_PIXELMAP_EDIT,
    TEST_CANCEL_PIXELMAP_EDIT
};

// edit_pixelmap_dlg dialog

IMPLEMENT_DYNAMIC(edit_pixelmap_dlg, express_dialog)
BEGIN_MESSAGE_MAP(edit_pixelmap_dlg, express_dialog)
    ON_BN_CLICKED(IDC_BROWSE, &edit_pixelmap_dlg::OnBnClickedBrowse)
    ON_BN_CLICKED(IDC_CUSTOM_OUTPUT, &edit_pixelmap_dlg::OnBnClickedCustomOutput)
    ON_BN_CLICKED(IDB_PATH_TYPE, &edit_pixelmap_dlg::OnBnClickedPathType)
    ON_BN_CLICKED(IDCANCEL, &edit_pixelmap_dlg::OnCancel)
    ON_BN_CLICKED(IDOK,&edit_pixelmap_dlg::OnBnClickedOk)
    ON_COMMAND(ID_PATHTYPE_PROJECTRELATIVE, SetPathProjectRelative)
    ON_COMMAND(ID_PATHTYPE_STUDIORELATIVE, SetPathStudioRelative)
    ON_COMMAND(ID_PATHTYPE_ABSOLUTE, SetPathAbsolute)
    ON_BN_CLICKED(IDC_RAW_FORMAT, &edit_pixelmap_dlg::OnBnClickedRawFormat)
    ON_BN_CLICKED(IDC_ALPHA, &edit_pixelmap_dlg::OnBnClickedAlpha)
    ON_CBN_SELCHANGE(IDC_OUTPUT_FORMAT, &edit_pixelmap_dlg::OnCbnSelchangeOutputFormat)
    ON_CBN_SELCHANGE(IDC_PALETTE_DEF_LIST, &edit_pixelmap_dlg::OnCbangePaletteDef)
    //ON_EN_CHANGE(IDC_ID_NAME, &edit_pixelmap_dlg::OnEnChangeIdName)
    ON_MESSAGE(STUDIO_TEST, &edit_pixelmap_dlg::OnTestMessage)
    ON_EN_KILLFOCUS(IDC_IMAGE_PATH, &edit_pixelmap_dlg::OnEditImagePath)
    ON_WM_PAINT()
END_MESSAGE_MAP()

#define PIXINFO_NOT_EQUAL -1
#define PIXINFO_NOT_SET   -2

/*****************************************************************************/
edit_pixelmap_dlg::edit_pixelmap_dlg(res_info *info, CWnd* pParent /*=NULL*/)
    : express_dialog(edit_pixelmap_dlg::IDD, pParent)
{
    IconId = IDB_PIXELMAPS;
    SetTitleText("Edit Pixelmap(s)");
    studiox_project *project = GetOpenProject();

    if (project)
    {
        mDisplay = project->GetDisplayIndex(info);
        mDisplayColorFormat = project->mDisplays[mDisplay].colorformat;
    }
    mpInfo = info;
    m_pathinfo = info->pathinfo;
}


/*****************************************************************************/
edit_pixelmap_dlg::~edit_pixelmap_dlg()
{
}

/*****************************************************************************/
BOOL edit_pixelmap_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    AddCancelButton();
    AddSaveButton();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/*****************************************************************************/
void edit_pixelmap_dlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call express_dialog::OnPaint() for painting messages

    if (!mpInfo->is_default && !IsPixelmapFolder())
    {
        PaintRequiredAsterisk(IDC_ID_NAME);
        PaintRequiredAsterisk(IDC_REQUIRED_FIELD_LEGEND);
    }

    int xpos, ypos;
    CRect rect;
    GetDlgItem(IDC_COMPRESS_OPTION_NOTE)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    int width = GetSystemMetrics(SM_CXSMICON);
    int height = GetSystemMetrics(SM_CYSMICON);

    xpos = rect.left - (width * 4 / 3);
    ypos = (rect.top + rect.bottom - height) / 2;

    HMODULE hMod = GetModuleHandle(L"user32.dll");
    HICON hIconExclamation = (HICON)LoadImage(hMod, MAKEINTRESOURCE(104), IMAGE_ICON, width, height, LR_DEFAULTCOLOR);

    DrawIconEx(dc, xpos, ypos, hIconExclamation, width, height, 0, NULL, DI_NORMAL);
}

/*****************************************************************************/
void edit_pixelmap_dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    
    DDX_Control(pDX, IDC_OUTPUT_FILE, mCustomFile);
    DDX_Control(pDX, IDC_BINARY_MODE, mBinaryMode);
    DDX_Control(pDX, IDC_PALETTE_LABEL, mPaletteLabel);
    DDX_Control(pDX, IDC_PALETTE_DEF_LIST, mPaletteDefList);
    DDX_Control(pDX, IDC_OUTPUT_FORMAT, mOutputFormatCombo);
    DDX_Control(pDX, IDB_PATH_TYPE, mPathTypeButton);

    CEdit* edit = (CEdit*)GetDlgItem(IDC_ID_NAME);
    if (mpInfo->is_default)
    {
        // they cannot change the name of system pixelmaps
        
        edit->SetReadOnly(TRUE);
        GetDlgItem(IDC_REQUIRED_FIELD_LEGEND)->ShowWindow(FALSE);
    }
    else if (IsPixelmapFolder())
    {
        GetDlgItem(IDB_PATH_TYPE)->EnableWindow(FALSE);
        GetDlgItem(IDC_IMAGE_PATH)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
        edit->EnableWindow(FALSE);
        GetDlgItem(IDC_REQUIRED_FIELD_LEGEND)->ShowWindow(FALSE);
    }
    else
    {
        SetAccessibleFullDescription(edit->GetSafeHwnd(), L"Required");
        SetAccessibleDescription(edit->GetSafeHwnd(), L"Required");
    }

    if (pDX->m_bSaveAndValidate)
    {
        SavePixelmapDlgConfig();
    }
    else
    {
        InitPixelmapDlgConfig();
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::SetCheck(int control_id, int check)
{
    CButton* button = (CButton*)GetDlgItem(control_id);

    if (!button)
    {
        return;
    }

    if (check == PIXINFO_NOT_EQUAL)
    {
        button->SetButtonStyle(BS_AUTO3STATE);
        button->SetCheck(BST_INDETERMINATE);
    }
    else
    {
        button->SetCheck(check);
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::GetCheck(int control_id, int &check)
{
    CButton* button = (CButton*)GetDlgItem(control_id);

    if (!button)
    {
        return;
    }

    switch (button->GetCheck())
    {
    case BST_CHECKED:
        check = 1;
        break;

    case BST_UNCHECKED:
        check = 0;
        break;

    case BST_INDETERMINATE:
        check = PIXINFO_NOT_EQUAL;
        break;
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::InitPixelmapDlgConfig()
{
    CDataExchange nx(this, FALSE);

    SetControlAccessibleName(GetDlgItem(IDC_OUTPUT_FILE)->GetSafeHwnd(), L"Custom Output File");

    mPixelmapInfo.compress = PIXINFO_NOT_SET;
    mPixelmapInfo.dither = PIXINFO_NOT_SET;
    mPixelmapInfo.keep_alpha = PIXINFO_NOT_SET;
    mPixelmapInfo.raw = PIXINFO_NOT_SET;
    mPixelmapInfo.output_color_format = PIXINFO_NOT_SET;
    mPixelmapInfo.palette_type = PIXINFO_NOT_SET;
    mPixelmapInfo.output_file_enabled = PIXINFO_NOT_SET;
    mPixelmapInfo.output_file = "";
    mPixelmapInfo.binary_mode = PIXINFO_NOT_SET;

    GetPixelmapInfo(mpInfo);

    SetCheck(IDC_COMPRESS, mPixelmapInfo.compress);

    if (mPixelmapInfo.keep_alpha != PIXINFO_NOT_SET)
    {
        SetCheck(IDC_ALPHA, mPixelmapInfo.keep_alpha);
    }

    SetCheck(IDC_DITHER, mPixelmapInfo.dither);
    SetCheck(IDC_RAW_FORMAT, mPixelmapInfo.raw);
    SetCheck(IDC_CUSTOM_OUTPUT, mPixelmapInfo.output_file_enabled);
    SetCheck(IDC_BINARY_MODE, mPixelmapInfo.binary_mode);

    if (!IsPixelmapFolder())
    {
        DDX_Text(&nx, IDC_IMAGE_PATH, m_pathinfo.pathname);
        DDX_Text(&nx, IDC_ID_NAME, mpInfo->name);
    }

    mPathBitmap.LoadBitmap(IDB_DOWN_ARROW);
    mPathTypeButton.SetBitmap(mPathBitmap);
    SetPathPrefix();

    studiox_project* project = GetOpenProject();
    CRect size;
    int table_index;
    int combo_index;
    int selected_index;

    // Set the drop-down list size.
    // There doesn't seem to be any way to do it in resource editor.

    if (mPixelmapInfo.output_file_enabled > 0)
    {
        mCustomFile.ShowWindow(TRUE);
        mCustomFile.SetWindowText(mPixelmapInfo.output_file);

        mBinaryMode.ShowWindow(TRUE);
    }

    mOutputFormatCombo.GetWindowRect(&size);
    size.bottom = size.top + 200;
    ScreenToClient(&size);
    mOutputFormatCombo.MoveWindow(&size);

    mOutputFormatCombo.AddString(_T("Use display default"));
    mOutputFormatCombo.SetItemData(0, 0);

    SetControlAccessibleName(mOutputFormatCombo.GetSafeHwnd(), _T("Output Format"));

    table_index = 0;
    selected_index = 0;
    int* allowed_color_formats = GetAllowedFormats(project->mDisplays[mDisplay].colorformat);

    if (allowed_color_formats != NULL)
    {
        while (allowed_color_formats[table_index])
        {
            if (allowed_color_formats[table_index] != project->mDisplays[mDisplay].colorformat)
            {
                CString name = resource_gen::GetColorFormatName(allowed_color_formats[table_index]);
                combo_index = mOutputFormatCombo.AddString(name);
                mOutputFormatCombo.SetItemData(combo_index, allowed_color_formats[table_index]);
            }

            table_index++;
        }

        if (mPixelmapInfo.output_color_format == PIXINFO_NOT_EQUAL)
        {
            combo_index = mOutputFormatCombo.AddString(L"<different color formats>");
            mOutputFormatCombo.SetItemData(combo_index, mPixelmapInfo.output_color_format);
        }
        else if (mPixelmapInfo.output_color_format == mDisplayColorFormat)
        {
            // The pixelmap color format is the same as display color format,
            // set the pixelmap color format to default.
            mPixelmapInfo.output_color_format = 0;
        }
        SelectDropListItem(&mOutputFormatCombo, mPixelmapInfo.output_color_format);
    }

    table_index = 0;
    selected_index = 0;

    while (!PaletteTypes[table_index].name.IsEmpty())
    {
        combo_index = mPaletteDefList.AddString(PaletteTypes[table_index].name);
        mPaletteDefList.SetItemData(combo_index, PaletteTypes[table_index].val);
        table_index++;
    }

    if (mPixelmapInfo.palette_type == PIXINFO_NOT_EQUAL)
    {
        combo_index = mPaletteDefList.AddString(L"<different palette types>");
        mPaletteDefList.SetItemData(combo_index, mPixelmapInfo.palette_type);
    }
    SelectDropListItem(&mPaletteDefList, mPixelmapInfo.palette_type);
    if (mDisplayColorFormat <= GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        mOutputFormatCombo.ShowWindow(FALSE);
    }
    EnableDisableRawFields();
    ShowHidePaletteType(mpInfo->output_color_format);
}

/*****************************************************************************/
void edit_pixelmap_dlg::SavePixelmapDlgConfig()
{
    CDataExchange nx(this, TRUE);

    GetCheck(IDC_COMPRESS, mPixelmapInfo.compress);
    GetCheck(IDC_ALPHA, mPixelmapInfo.keep_alpha);
    GetCheck(IDC_DITHER, mPixelmapInfo.dither);
    GetCheck(IDC_RAW_FORMAT, mPixelmapInfo.raw);
    GetCheck(IDC_CUSTOM_OUTPUT, mPixelmapInfo.output_file_enabled);

    if (!IsPixelmapFolder())
    {
        DDX_Text(&nx, IDC_ID_NAME, mpInfo->name);

        mpInfo->pathinfo = m_pathinfo;
    }

    int combo_index;
    studiox_project* project = GetOpenProject();

    if (mOutputFormatCombo.IsWindowVisible())
    {
        combo_index = mOutputFormatCombo.GetCurSel();
        mPixelmapInfo.output_color_format = mOutputFormatCombo.GetItemData(combo_index);

        if (mPixelmapInfo.output_color_format != 0)
        {
            project->mDisplays[mDisplay].default_map_format = FALSE;
        }
    }
    else
    {
        project->mDisplays[mDisplay].default_map_format = TRUE;
    }

    if (mPaletteDefList.IsWindowVisible())
    {
        combo_index = mPaletteDefList.GetCurSel();
        mPixelmapInfo.palette_type = mPaletteDefList.GetItemData(combo_index);
    }

    mCustomFile.GetWindowText(mPixelmapInfo.output_file);

    if (mPixelmapInfo.output_file.IsEmpty())
    {
        mPixelmapInfo.output_file_enabled = FALSE;
    }

    GetCheck(IDC_BINARY_MODE, mPixelmapInfo.binary_mode);

    SavePixelmapInfo(mpInfo);
}

/*****************************************************************************/
BOOL edit_pixelmap_dlg::IsPixelmapFolder()
{
    
    if ((mpInfo->type == RES_TYPE_FOLDER) ||
        (mpInfo->type == RES_TYPE_GROUP))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
void edit_pixelmap_dlg::GetPixelmapInfo(int &current_val, int input_val)
{
    if (current_val == PIXINFO_NOT_SET)
    {
        current_val = input_val;
    }
    else if (current_val != input_val)
    {
        current_val = PIXINFO_NOT_EQUAL;
    }
}

/*****************************************************************************/
BOOL edit_pixelmap_dlg::SavePixelmapInfo(int& val, int input_val)
{
    if ((input_val != PIXINFO_NOT_EQUAL) && (val != input_val))
    {
        val = input_val;
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
void edit_pixelmap_dlg::SavePixelmapInfo(res_info* info)
{
    res_info* child;
    int update_pixelmap = 0;

    switch (info->type)
    {

    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        child = info->child;
        while (child)
        {
            SavePixelmapInfo(child);
            child = child->next;
        }
        break;

    case RES_TYPE_PIXELMAP:
        update_pixelmap = SavePixelmapInfo(info->compress, mPixelmapInfo.compress);

        if (HasAlpha(info))
        {
            update_pixelmap += SavePixelmapInfo(info->keep_alpha, mPixelmapInfo.keep_alpha);
        }
        update_pixelmap += SavePixelmapInfo(info->dither, mPixelmapInfo.dither);
        update_pixelmap += SavePixelmapInfo(info->raw, mPixelmapInfo.raw);
        update_pixelmap += SavePixelmapInfo(info->output_color_format, mPixelmapInfo.output_color_format);
        update_pixelmap += SavePixelmapInfo(info->palette_type, mPixelmapInfo.palette_type);
        update_pixelmap += SavePixelmapInfo(info->output_file_enabled, mPixelmapInfo.output_file_enabled);
        update_pixelmap += SavePixelmapInfo(info->binary_mode, mPixelmapInfo.binary_mode);

        if (mPixelmapInfo.output_file != "<different output file>")
        {
            info->output_file = mPixelmapInfo.output_file;
        }

        if (!IsPixelmapFolder() || update_pixelmap)
        {
            info->is_modified = TRUE;
        }
        break;
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::GetPixelmapInfo(res_info *info)
{
    res_info* child;
    switch (info->type)
    {

    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        child = info->child;
        while (child)
        {
            GetPixelmapInfo(child);
            child = child->next;
        }
        break;

    case RES_TYPE_PIXELMAP:
        GetPixelmapInfo(mPixelmapInfo.compress, info->compress);
        if (HasAlpha(info))
        {
            GetPixelmapInfo(mPixelmapInfo.keep_alpha, info->keep_alpha);
        }
        GetPixelmapInfo(mPixelmapInfo.dither, info->dither);
        GetPixelmapInfo(mPixelmapInfo.raw, info->raw);
        GetPixelmapInfo(mPixelmapInfo.output_color_format, info->output_color_format);
        GetPixelmapInfo(mPixelmapInfo.palette_type, info->palette_type);

        if (mPixelmapInfo.output_file_enabled == PIXINFO_NOT_SET)
        {
            mPixelmapInfo.output_file = info->output_file;
        }
        else
        {
            if (mPixelmapInfo.output_file != info->output_file)
            {
                mPixelmapInfo.output_file = "<different output file>";
            }
        }

        GetPixelmapInfo(mPixelmapInfo.output_file_enabled, info->output_file_enabled);
        GetPixelmapInfo(mPixelmapInfo.binary_mode, info->binary_mode);
        break;
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::SetPathPrefix()
{
    CWnd *pathtype = GetDlgItem(IDC_PATH_PREFIX);
    HWND hWndPathType= 0;

    if (pathtype)
    {
        hWndPathType = pathtype->GetSafeHwnd();
    } 

    switch (m_pathinfo.pathtype)
    {
    
    case PATH_TYPE_PROJECT_RELATIVE:
        SetDlgItemText(IDC_PATH_PREFIX, _T("<Project Relative Path>"));
        SetControlAccessibleName(hWndPathType, _T("Project Relative Path"));
        break;

    case PATH_TYPE_INSTALL_RELATIVE:
        SetDlgItemText(IDC_PATH_PREFIX, _T("<GUIX Repo Relative Path>"));
        SetControlAccessibleName(hWndPathType, _T("GUIX Repo Relative Path"));
        break;

    case PATH_TYPE_ABSOLUTE:
    default:
        SetDlgItemText(IDC_PATH_PREFIX, _T("<Absolute Path>"));
        SetControlAccessibleName(hWndPathType, _T("Absolute Path"));
        break;
    }
}

/*****************************************************************************/
BOOL edit_pixelmap_dlg::CheckPathname(CString pathname)
{
    CString oldpath, newpath;

    oldpath = MakeAbsolutePathname(m_pathinfo);
    ConvertToProjectRelativePath(oldpath);
    FormatPath(oldpath);

    newpath = pathname;
    ConvertToProjectRelativePath(newpath);
    FormatPath(newpath);

    if (_tcscmp(newpath, oldpath))
    {
        if (FileExists(mDisplay, RES_TYPE_PIXELMAP, pathname))
        {
            ErrorMsg("Pixelmap Already Exists.", this);
        }

        studiox_project* project = GetOpenProject();

        if (project)
        {
            if ((project->mDisplays[mDisplay].num_themes > 1) &&
                (image_reader::GetFrameCount(newpath) != mpInfo->GetPixelmapFrameCount()))
            {
                return AskUser("The selected pixelmap has different frame count, the change will apply to all other theme(s), continue?", this);
            }
        }
    }

    return TRUE;
}

/*****************************************************************************/
void edit_pixelmap_dlg::OnBnClickedBrowse()
{
    CString pathname;
    TCHAR *filter;

    if (mpInfo->is_default)
    {
        filter = _T("Image Files\0*.png;*.jpg\0");
    }
    else
    {
        filter = _T("Image Files\0*.png;*.jpg;*.gif\0");
    }

    if (BrowseForSingleFile(_T("Select pixelmap source file"),
        filter, NULL, pathname, this))
    {
        if (CheckPathname(pathname))
        {
            SaveToPathInfo(m_pathinfo, pathname);
            SetPathPrefix();
            CheckAlphaStatus(TRUE);
            SetDlgItemText(IDC_IMAGE_PATH, m_pathinfo.pathname);
        }
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::OnEditImagePath()
{
    // TODO: Add your control notification handler code here
    GetDlgItemText(IDC_IMAGE_PATH, m_pathinfo.pathname);

    CheckPathname(m_pathinfo.pathname);
}


/*****************************************************************************/
void edit_pixelmap_dlg::OnBnClickedCustomOutput()
{
    // TODO: Add your control notification handler code here
    CButton *pb = (CButton *) GetDlgItem(IDC_CUSTOM_OUTPUT);

    if (pb->GetCheck() == BST_CHECKED)
    {
        mCustomFile.ShowWindow(SW_SHOW);
        mBinaryMode.ShowWindow(SW_SHOW);
    }
    else
    {
        mCustomFile.ShowWindow(SW_HIDE);
        mBinaryMode.ShowWindow(SW_HIDE);
    } 
}

/*****************************************************************************/
void edit_pixelmap_dlg::OnBnClickedPathType()
{
    // TODO: Add your control notification handler code here
    CMenu menu;
    CMenu *pPopup;
    CPoint pos;
    CRect  buttonsize;
    mPathTypeButton.GetWindowRect(&buttonsize);

    pos.x = buttonsize.left;
    pos.y = buttonsize.bottom + 2;

    menu.LoadMenu(IDR_PATHTYPE);
      pPopup = menu.GetSubMenu(0);
    //ClientToScreen(&pos);
    pPopup->TrackPopupMenu( TPM_LEFTALIGN, pos.x, pos.y, this);
}

/*****************************************************************************/
void  edit_pixelmap_dlg::SetPathProjectRelative()
{
    if (!m_pathinfo.pathname.IsEmpty())
    {
        CString abspath = MakeAbsolutePathname(m_pathinfo);
        m_pathinfo.pathtype = PATH_TYPE_PROJECT_RELATIVE;
        SaveToPathInfo(m_pathinfo, abspath);
        SetDlgItemText(IDC_IMAGE_PATH, m_pathinfo.pathname);
        SetPathPrefix();
    }
}

/*****************************************************************************/
void  edit_pixelmap_dlg::SetPathStudioRelative()
{
    if (!m_pathinfo.pathname.IsEmpty())
    {
        CString abspath = MakeAbsolutePathname(m_pathinfo);
        m_pathinfo.pathtype = PATH_TYPE_INSTALL_RELATIVE;
        SaveToPathInfo(m_pathinfo, abspath);
        SetDlgItemText(IDC_IMAGE_PATH, m_pathinfo.pathname);
        SetPathPrefix();
    }
}

/*****************************************************************************/
void  edit_pixelmap_dlg::SetPathAbsolute()
{
    if (!m_pathinfo.pathname.IsEmpty())
    {
        CString abspath = MakeAbsolutePathname(m_pathinfo);
        m_pathinfo.pathtype = PATH_TYPE_ABSOLUTE;
        SaveToPathInfo(m_pathinfo, abspath);
        SetDlgItemText(IDC_IMAGE_PATH, m_pathinfo.pathname);
        SetPathPrefix();
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::ShowHidePaletteType(int selected_format)
{
    BOOL show = FALSE;

    if (mDisplayColorFormat > GX_COLOR_FORMAT_8BIT_PACKED_PIXEL)
    {
        if (selected_format == GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            show = TRUE;
        }
    }
    mPaletteLabel.ShowWindow(show);
    mPaletteDefList.ShowWindow(show);
}

/*****************************************************************************/
BOOL edit_pixelmap_dlg::HasAlpha(res_info* info)
{
    BOOL result = FALSE;

    if (mDisplayColorFormat > GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        CString abspath = MakeAbsolutePathname(info->pathinfo);
        image_reader* pReader = image_reader::CreateProperReader(abspath);

        if (pReader)
        {
            if (pReader->CheckImageHasAlphaChannel(abspath))
            {
                result = TRUE;
            }
            delete pReader;
        }
    }

    return result;
}

/*****************************************************************************/
void edit_pixelmap_dlg::CheckAlphaStatus(BOOL Initialize)
{
    if (IsPixelmapFolder())
    {
        if (mPixelmapInfo.keep_alpha == PIXINFO_NOT_SET)
        {
            GetDlgItem(IDC_ALPHA)->EnableWindow(FALSE);
        }
        else
        {
            GetDlgItem(IDC_ALPHA)->EnableWindow(TRUE);
        }
    }
    else
    {
        // if the source has an alpha channel, default to keeping it
        if (mDisplayColorFormat <= GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            CButton* pb = (CButton*)GetDlgItem(IDC_ALPHA);
            GetDlgItem(IDC_ALPHA)->EnableWindow(FALSE);
            pb->SetCheck(FALSE);
        }
        else
        {
            CString abspath = MakeAbsolutePathname(m_pathinfo);
            image_reader* pReader = image_reader::CreateProperReader(abspath);

            if (pReader)
            {
                if (pReader->CheckImageHasAlphaChannel(abspath))
                {
                    CButton* pb = (CButton*)GetDlgItem(IDC_ALPHA);
                    pb->EnableWindow(TRUE);

                    if (Initialize)
                    {
                        pb->SetCheck(TRUE);
                        mPixelmapInfo.keep_alpha = TRUE;
                    }
                }
                else
                {
                    mPixelmapInfo.keep_alpha = FALSE;
                    GetDlgItem(IDC_ALPHA)->EnableWindow(FALSE);
                }
                delete pReader;
            }
        }
    }
}


/*****************************************************************************/
void edit_pixelmap_dlg::EnableDisableRawFields()
{
    CButton *pb = (CButton *)GetDlgItem(IDC_RAW_FORMAT);


    if (pb->GetCheck())
    {
        mOutputFormatCombo.EnableWindow(FALSE);
        GetDlgItem(IDC_ALPHA)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMPRESS)->EnableWindow(FALSE);
        GetDlgItem(IDC_DITHER)->EnableWindow(FALSE);

        if (mPaletteDefList.IsWindowVisible())
        {
            mPaletteDefList.EnableWindow(FALSE);
            mPaletteLabel.EnableWindow(FALSE);
        }
    }
    else
    {
        mOutputFormatCombo.EnableWindow(TRUE);
        CheckAlphaStatus();
        GetDlgItem(IDC_COMPRESS)->EnableWindow(TRUE);
        GetDlgItem(IDC_DITHER)->EnableWindow(TRUE);

        if (mPaletteDefList.IsWindowVisible())
        {
            mPaletteDefList.EnableWindow(TRUE);
            mPaletteLabel.EnableWindow(TRUE);
        }
    }
}

/*****************************************************************************/
void edit_pixelmap_dlg::OnBnClickedRawFormat()
{
    // TODO: Add your control notification handler code here
    CButton *pb = (CButton *) GetDlgItem(IDC_RAW_FORMAT);
    studiox_project *project = GetOpenProject();

    if (pb->GetCheck())
    {
        if (IsRenesasHwJpeg(project) ||
            project->mHeader.target_cpu == CPU_RZ)
        {
            CString abspath = MakeAbsolutePathname(m_pathinfo);
            int image_type = image_reader::GetImageType(abspath);

            if (image_type != IMAGE_TYPE_JPG)
            {
                ErrorMsg("This image type is not compatible with the target hardware decoder.", this);
                pb->SetCheck(FALSE);
            }
            else
            {
                image_reader* pReader = image_reader::CreateProperReader(abspath);

                if (pReader)
                {
                    pReader->SetOutputColorFormat(GX_COLOR_FORMAT_24XRGB, GX_COLOR_FORMAT_24XRGB);
                    pReader->SetSaveAlphaVal(FALSE);

                    if (pReader->ReadImage(abspath))
                    {
                        GX_PIXELMAP* pmap = pReader->GetPixelmap();
                        if ((pmap->gx_pixelmap_height & 0x07) != 0 ||
                            (pmap->gx_pixelmap_width & 0x07) != 0)
                        {
                            ErrorMsg("Image size is not compatible with target hardware decoder.", this);
                            pb->SetCheck(FALSE);
                            pReader->DestroyImage();
                        }
                    }
                    delete pReader;
                }
            }
        }
    }
    EnableDisableRawFields();
}

/*****************************************************************************/
void edit_pixelmap_dlg::OnBnClickedAlpha()
{
    int SelectedFormat;

    CButton *alpha_button = (CButton *) GetDlgItem(IDC_ALPHA);
    studiox_project *project = GetOpenProject();

    // If the target CPU is Synergy in 565 mode, force the output format to compatible format
    // if they turn on using alpha
    if (IsRenesasDave2D(project) &&
        alpha_button->GetCheck())
    {
        SelectedFormat = mOutputFormatCombo.GetItemData(mOutputFormatCombo.GetCurSel());
        if (SelectedFormat == 0)
        {
            SelectedFormat = mDisplayColorFormat;
        }
        if (!IsAlphaFormat(SelectedFormat))
        {
            ErrorMsg("Selected output format does not support alpha channel. Forcing output format to 32 bpp argb", this);
            SelectDropListItem(&mOutputFormatCombo, GX_COLOR_FORMAT_32ARGB);
        }
    }
}


/*****************************************************************************/
void edit_pixelmap_dlg::OnCbnSelchangeOutputFormat()
{
    int SelectedFormat;
    CButton *alpha_button;

    SelectedFormat = mOutputFormatCombo.GetItemData(mOutputFormatCombo.GetCurSel());
    studiox_project *project = GetOpenProject();

    // if target CPU is Synergy using D2D in 565 mode and the selected output format does
    // not support alpha channel, disable alpha channel
    if (IsRenesasDave2D(project))
    {
        if (!IsAlphaFormat(SelectedFormat))
        {
            alpha_button = (CButton *) GetDlgItem(IDC_ALPHA);
            if (alpha_button->GetCheck())
            {
                ErrorMsg("Alpha channel data is not supported using the selected output format. Alpha channel will be disabled", this);
                alpha_button->SetCheck(FALSE);
            }
        }
    }

    ShowHidePaletteType(SelectedFormat);
}


/*****************************************************************************/
void edit_pixelmap_dlg::OnCbangePaletteDef()
{
    // TODO: Add your control notification handler code here
}

/*
void edit_pixelmap_dlg::OnEnChangeIdName()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the express_dialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}
*/

void edit_pixelmap_dlg::OnBnClickedOk()
{
    if (!IsPixelmapFolder())
    {
        CString pixelmap_name = mpInfo->name;
        CEdit* edit = (CEdit*)GetDlgItem(IDC_ID_NAME);

        GetDlgItemText(IDC_ID_NAME, pixelmap_name);
        if (_tcscmp(pixelmap_name, mpInfo->name))
        {
            /*Check ID name in whole pixelmap folder.*/
            if (NameExists(mDisplay, RES_TYPE_PIXELMAP, pixelmap_name))
            {
                ErrorMsg("Pixelmap Name ID already Exists.", this);
                SetDlgItemText(IDC_ID_NAME, mpInfo->name);
                return;
            }
        }

        if (TestInputName(edit, "Pixelmap ID", mpInfo->name, this))
        {
            express_dialog::OnOK();
        }
    }
    else
    {
        express_dialog::OnOK();
    }
}

LRESULT edit_pixelmap_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;

    switch (wParam)
    {
    case TEST_SET_IMAGE_PATH:
        pWnd = GetDlgItem(IDC_IMAGE_PATH);
        ((CEdit *)pWnd)->SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_IMAGE_PATH, EN_KILLFOCUS), (LPARAM)((CEdit *)pWnd)->m_hWnd);
        break;

    case TEST_SET_IMAGE_NAME:
        pWnd = GetDlgItem(IDC_ID_NAME);
        ((CEdit *)pWnd)->SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ID_NAME, EN_KILLFOCUS), (LPARAM)((CEdit *)pWnd)->m_hWnd);
        break;
        
    case TEST_CHECK_CUSTOM_OUTPUT:
        pWnd = GetDlgItem(IDC_CUSTOM_OUTPUT);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CUSTOM_OUTPUT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_CUSTOM_OUTPUT_FILE:
        pWnd = GetDlgItem(IDC_OUTPUT_FILE);
        ((CEdit *)pWnd)->SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_OUTPUT_FILE, EN_KILLFOCUS), (LPARAM)((CEdit *)pWnd)->m_hWnd);
        break;

    case TEST_CHECK_RAW_FORMAT:
        pWnd = GetDlgItem(IDC_RAW_FORMAT);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_RAW_FORMAT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CHECK_COMPRESS:
        pWnd = GetDlgItem(IDC_COMPRESS);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COMPRESS, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CHECK_ALPHA:
        pWnd = GetDlgItem(IDC_ALPHA);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ALPHA, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CHECK_DITHER:
        pWnd = GetDlgItem(IDC_DITHER);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_DITHER, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_OUTPUT_FORMAT:
        for (int index = 0; index < mOutputFormatCombo.GetCount(); index++)
        {
            if (mOutputFormatCombo.GetItemData(index) == lParam)
            {
                mOutputFormatCombo.SetCurSel(index);
                pWnd = GetDlgItem(IDC_OUTPUT_FORMAT);
                SendMessage(WM_COMMAND, MAKEWPARAM(IDC_OUTPUT_FORMAT, CBN_SELCHANGE), (LPARAM)pWnd->m_hWnd);
            }
        }
        break;

    case TEST_SET_PALETTE_TYPE:
        for (int index = 0; index < mPaletteDefList.GetCount(); index++)
        {
            if (mPaletteDefList.GetItemData(index) == lParam)
            {
                mPaletteDefList.SetCurSel(index);
                pWnd = GetDlgItem(IDC_PALETTE_DEF_LIST);
                SendMessage(WM_COMMAND, MAKEWPARAM(IDC_PALETTE_DEF_LIST, CBN_SELCHANGE), (LPARAM)pWnd->m_hWnd);
            }
        }
        break;

    case TEST_SAVE_PIXELMAP_EDIT:
        OnBnClickedOk();
        break;

    case TEST_CANCEL_PIXELMAP_EDIT:
        OnCancel();
        break;
    }

    return 0;
}

INT* edit_pixelmap_dlg::GetAllowedFormats(int display_format)
{
    INT *allowed_color_formats = NULL;
    studiox_project *project = GetOpenProject();

    switch (display_format)
    {
    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        allowed_color_formats = _332rgb_allowed_formats;
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        allowed_color_formats = _4444argb_allowed_formats;
        break;

    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
        if (IsRenesasDave2D(project))
        {
            /* Synergy CPU supports 32argb in 565 driver. */
            allowed_color_formats = _565rgb_synergy_allowed_formats;
        }
        else if (IsSTChromeArt(project))
        {
            allowed_color_formats = _565rgb_st_chromeart_allowed_formats;
        }
        else
        {
            allowed_color_formats = _565rgb_allowed_formats;
        }
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        allowed_color_formats = _1555xrgb_allowed_formats;
        break;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32ARGB:
        allowed_color_formats = _32bpp_allowed_formats;
        break;

    default:
        break;
    }

    return allowed_color_formats;
}

//Check if the ouput pixelmap format is supported
BOOL edit_pixelmap_dlg::IsOutputFormatSupported(int display_format, int output_format)
{
    INT *allowed_formats = GetAllowedFormats(display_format);
    INT index = 0;

    if (!allowed_formats)
    {
        return FALSE;
    }

    while (allowed_formats[index])
    {
        if (allowed_formats[index] == output_format)
        {
            return TRUE;
        }

        index++;
    }

    return FALSE;
}
