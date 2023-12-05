// PaletteLayoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "express_table_row.h"
#include "PaletteLayoutDlg.h"

extern CString target_class_name;

enum CONTROL_IDS {
    ID_REDFIELD = 4096,
    ID_GREENFIELD,
    ID_BLUEFIELD,
    ID_ALPHAFIELD,
    ID_PAL_INDEX
};

enum palette_layout_dlg_test_commands{
    TEST_SET_PREDEFINED_PALETTE_ENTRY = 1,
    TEST_SET_PALETTE_COLOR,
    TEST_SAVE_PALETTE_EDIT,
    TEST_CANCEL_PALETTE_EDIT
};

// PaletteLayoutDlg dialog
BEGIN_MESSAGE_MAP(PaletteLayoutDlg, express_dialog)
    //ON_BN_CLICKED(IDC_BROWSE_PATH, &NewProjectDlg::OnBnClickedBrowsePath)
    //ON_BN_CLICKED(ID_CANCEL, &NewProjectDlg::OnCancel)
    ON_WM_SHOWWINDOW()
    ON_WM_SYSCHAR()
    ON_WM_PAINT()
    ON_WM_VSCROLL()
    ON_COMMAND(IDOK, &PaletteLayoutDlg::OnReturnKey)

    ON_BN_CLICKED(IDOK, &PaletteLayoutDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL,  &PaletteLayoutDlg::OnCancel)
    ON_BN_CLICKED(IDCANCEL, &PaletteLayoutDlg::OnCancel)
    ON_BN_CLICKED(IDC_IMPORT_PALETTE, &PaletteLayoutDlg::OnBnClickedImportPalette)
    ON_BN_CLICKED(IDC_EXPORT_PALETTE, &PaletteLayoutDlg::OnBnClickedExportPalette)
    ON_BN_CLICKED(IDC_DEFAULT_PALETTE, &PaletteLayoutDlg::OnBnClickedDefaultPalette)

    ON_EN_KILLFOCUS(IDC_TOTAL_PALETTE_ENTRIES, &PaletteLayoutDlg::OnEnChangeTotalPaletteEntries)
    ON_EN_KILLFOCUS(IDC_USER_PALETTE_ENTRIES, &PaletteLayoutDlg::OnEnChangeUserPaletteEntries)
    ON_MESSAGE(STUDIO_TEST, &PaletteLayoutDlg::OnTestMessage)
    ON_BN_CLICKED(IDC_GEN_GRADIENT, &PaletteLayoutDlg::OnGenGradient)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
PaletteLayoutDlg::PaletteLayoutDlg(int display, int theme, CWnd* pParent /*=NULL*/)
	: express_dialog(PaletteLayoutDlg::IDD, pParent)
{
    IconId = IDB_RECENT_PROJECT;
    SetTitleText("Edit Palette");

    mDisplay = display;
    mTheme = theme;
    mpProject = GetOpenProject();
    mPaletteResetToDefault = FALSE;
    mpPaletteFrame = NULL;

    // save current settings in case user cancels after making changes:

    if (!mpProject->mDisplays[mDisplay].themes[mTheme].palette)
    {
        //create default palette
        ProjectConfigDlg::CreateDefaultPalette(mpProject, mDisplay, mTheme);
    }

    if (mpProject->mDisplays[mDisplay].themes[mTheme].palette)
    {
        theme_info* tinfo = &mpProject->mDisplays[mDisplay].themes[mTheme];
        old_pal_total_entries = tinfo->palette_total_size;
        old_pal_predefined = tinfo->palette_predefined;
        memcpy_s(OldPalette, sizeof(OldPalette), tinfo->palette, tinfo->palette_total_size * sizeof(GX_COLOR));
    }
}

///////////////////////////////////////////////////////////////////////////////
PaletteLayoutDlg::~PaletteLayoutDlg()
{
    if (mpPaletteFrame)
    {
        delete mpPaletteFrame;
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL PaletteLayoutDlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CRect client;
    CRect size;
    CRect scrollsize;

    GetClientRect(&client);

    // Get palette scroll bar size
    GetDlgItem(IDC_PALETTE_SCROLL)->GetWindowRect(&scrollsize);
    ScreenToClient(&scrollsize);

    client.left += 10;
    client.top = scrollsize.top;
    client.right = scrollsize.left - 5;
    client.bottom = scrollsize.bottom;

    // Create palette frame
    mpPaletteFrame = new palette_frame(&mpProject->mDisplays[mDisplay].themes[mTheme]);
    mpPaletteFrame->Create(target_class_name, _T("PaletteFrame"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP, client, this, 0, NULL);

    AddCancelButton();
    AddSaveButton();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    express_dialog::OnShowWindow(bShow, nStatus);
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::NotifyColorChanged(int index, GX_COLOR newcolor)
{
    if (index < mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined)
    {
        mpProject->mDisplays[mDisplay].themes[mTheme].palette[index] = newcolor;
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnPaint()
{
    express_dialog::OnPaint();
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::DoDataExchange(CDataExchange* pDX)
{
    int auto_entries;

    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_THEME_NAME, mpProject->mDisplays[mDisplay].themes[mTheme].theme_name);
    DDX_Text(pDX, IDC_TOTAL_PALETTE_ENTRIES, mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size);
    DDX_Text(pDX, IDC_USER_PALETTE_ENTRIES, mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined);

    auto_entries = mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size - mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;
    DDX_Text(pDX, IDC_AUTO_PALETTE_ENTRIES, auto_entries);
    DDX_Control(pDX, IDC_PALETTE_SCROLL, mPalIndexScroll);

    SCROLLINFO info;
    info.fMask = SIF_RANGE|SIF_POS|SIF_PAGE;
    info.nMin = 0;
    info.nMax = 255;
    info.nPage = VISIBLE_PALETTE_ROWS;
    info.nPos = 0;

    mPalIndexScroll.SetScrollInfo(&info);
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnReturnKey()
{
    CWnd *pctrl = GetFocus();
    int ctrl_id = pctrl->GetDlgCtrlID();

    switch(ctrl_id)
    {
    case IDC_TOTAL_PALETTE_ENTRIES:
        OnEnChangeTotalPaletteEntries();
        break;

    case IDC_USER_PALETTE_ENTRIES:
        OnEnChangeUserPaletteEntries();
        break;

    case IDOK:
        OnBnClickedOk();
        break;
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
/* go throught the color resources in this theme, and if any of the colors
   have an index larger than palette pre-defined size, reset that color index to
   the best match. This function is called when the user reduces the pre-defined
   palette size.
*/
void PaletteLayoutDlg::CheckResetColorIndexes(res_info *info, GX_COLOR *new_palette, USHORT new_pal_size)
{
    while(info)
    {
        if (info->type == RES_TYPE_COLOR)
        {
            if (info->colorval >= new_pal_size)
            {
                if (info->colorval < (ULONG) old_pal_predefined)
                {
                    Pixel old_color(OldPalette[info->colorval]);
                    info->colorval = image_reader::GetNearestPaletteColor(old_color, new_palette, new_pal_size);        
                }
                else
                {
                    info->colorval = 0;
                }
            }
        }
        if (info->child)
        {
            CheckResetColorIndexes(info->child, new_palette, new_pal_size);
        }
        info = info->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
// PaletteLayoutDlg message handlers
///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnBnClickedOk()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        if (mPaletteResetToDefault)
        {
            // Remap colors to the best match in the new color palette.
            ProjectConfigDlg::ColorTableConvert(mpProject->mDisplays[mDisplay].themes[mTheme].GetFirstResourceInfo(),
                                    OldPalette,
                                    old_pal_predefined,
                                    mpProject->mDisplays[mDisplay].colorformat,
                                    mpProject->mDisplays[mDisplay].themes[mTheme].palette,
                                    mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined,
                                    mpProject->mDisplays[mDisplay].colorformat);
        }
        else
        {
            /* If any of the color resources are using an index greater than the
               number of pre-defined entried, remap that entry of the color table
            */

            int new_pal_size = mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;
            if (new_pal_size < old_pal_predefined)
            {
                CheckResetColorIndexes(project->mDisplays[mDisplay].themes[mTheme].GetFirstResourceInfo(),
                                       mpProject->mDisplays[mDisplay].themes[mTheme].palette, new_pal_size);
            }
        }

        /* If we are running in palette mode, we need to remap the color resources to the 
           new palette
        */
        project->InitializeAllPixelmaps();
        GetResourceView()->OnDisplaySelect(GetProjectView()->GetActiveDisplay(), TRUE);
    }

    CDialog::OnOK();
}


///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnCancel()
{
    // TODO: Add your control notification handler code here

    // restore palette settings:

    if (mpProject->mDisplays[mDisplay].themes[mTheme].palette)
    {
        theme_info* tinfo = &mpProject->mDisplays[mDisplay].themes[mTheme];
        tinfo->palette_total_size = old_pal_total_entries;
        tinfo->palette_predefined = old_pal_predefined;
        memcpy_s(tinfo->palette, tinfo->palette_total_size * sizeof(GX_COLOR), OldPalette, sizeof(OldPalette));
        mPaletteResetToDefault = FALSE;
    }
    CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnEnChangeTotalPaletteEntries()
{
    int newval = GetDlgItemInt(IDC_TOTAL_PALETTE_ENTRIES, NULL, TRUE);

    if (newval >= 2 && newval <= 256)
    {
        mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size = newval;
        
        // check to see if we need to update pre-defined count
        if (mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined >= newval)
        {
            mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined = newval - 1;
            SetDlgItemInt(IDC_USER_PALETTE_ENTRIES, mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined);
        }

        // update the leftover count
        int autocount = newval - mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;
        SetDlgItemInt(IDC_AUTO_PALETTE_ENTRIES, autocount);
        mpProject->SetModified();
    }
    else
    {
        ErrorMsg("The palette total size must be between 2 and 256 entries.", this);
        SetDlgItemInt(IDC_TOTAL_PALETTE_ENTRIES, mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size);
    }
}


///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnEnChangeUserPaletteEntries()
{
    int totalcount = mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size;
    int usercount = GetDlgItemInt(IDC_USER_PALETTE_ENTRIES, NULL, TRUE);

    if (usercount >= 0 && usercount < totalcount)
    {
        int old_usercount = mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;

        mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined = usercount;
        
        // update the leftover count
        int autocount = totalcount - usercount;
        SetDlgItemInt(IDC_AUTO_PALETTE_ENTRIES, autocount);
        mpProject->SetModified();

        for (int index = old_usercount; index < usercount; index++)
        {
            //initiate new added entries
            mpProject->mDisplays[mDisplay].themes[mTheme].palette[index] = 0xff000000;
        }

        mpPaletteFrame->SetRowData();
    }
    else
    {
        ErrorMsg("The pre-defined palette entries must be between 1 and (total palette size - 1).", this);
        SetDlgItemInt(IDC_USER_PALETTE_ENTRIES, mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined);
    }
}


///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnBnClickedImportPalette()
{
    // TODO: Add your control notification handler code here
    CString Pathname;
    int count;
    int index;

    if (BrowseForSingleFile(_T("Select Palette File"),
        _T("Palette Files\0*.palx\0\0"), _T("palx"), Pathname, this))
    {
        xml_reader reader;

        if (!reader.ReadFile(CString(Pathname)))
        {
            ErrorMsg("Unable to read palette file", this);
            return;
        }
        
        if (!reader.EnterSection("GUIX_Palette"))
        {
            ErrorMsg("Invalid palette file format", this);
            return;
        }
        reader.ReadInt("total_entries", mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size);
        reader.ReadInt("user_entries", count);
        mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined = count;

        if (!mpProject->mDisplays[mDisplay].themes[mTheme].palette)
        {
            mpProject->mDisplays[mDisplay].themes[mTheme].palette = new GX_COLOR[256];
        }
        
        for (index = 0; index < count; index++)
        {
            reader.ReadHex("rgb", mpProject->mDisplays[mDisplay].themes[mTheme].palette[index]);
        }
        reader.CloseSection();
 
        int totalcount = mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size;
        int usercount = mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;

        SetDlgItemInt(IDC_TOTAL_PALETTE_ENTRIES, totalcount);
        SetDlgItemInt(IDC_USER_PALETTE_ENTRIES, usercount);
        int autocount = totalcount - usercount;
        SetDlgItemInt(IDC_AUTO_PALETTE_ENTRIES, autocount);
    
        mpPaletteFrame->SetRowData();
        mpProject->SetModified();
    }
}


///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnBnClickedExportPalette()
{
    // TODO: Add your control notification handler code here
    TCHAR SavePathname[MAX_PATH];
    TCHAR FileName[MAX_PATH];
    int count;
    int index;

    SavePathname[0] = 0;
    FileName[0] = 0;

    if (GetOutputFileName(SavePathname, FileName, 
            _T("Select Palette Output file"),
            _T("Palette Files\0*.palx\0\0"),
            NULL, _T("palx")), this)
    {
        xml_writer writer;

        if (!writer.OpenFile(CString(SavePathname)))
        {
            ErrorMsg("Unable to write palette file", this);
            return;
        }
        
        writer.WriteHeader("GUIX_Palette");
        writer.OpenTag("GUIX_Palette");
        writer.WriteInt("total_entries", mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size);

        count = mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;
        writer.WriteInt("user_entries", count);
        
        for (index = 0; index < count; index++)
        {
            writer.WriteHex("rgb", mpProject->mDisplays[mDisplay].themes[mTheme].palette[index]);
        }
        writer.CloseTag("GUIX_Palette");
        writer.CloseFile();
    }
}



///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnBnClickedDefaultPalette()
{
    // TODO: Add your control notification handler code here
    ProjectConfigDlg::CreateDefaultPalette(mpProject, mDisplay, mTheme);

    mPaletteResetToDefault = TRUE;

    mpProject->SetModified();

    int totalcount = mpProject->mDisplays[mDisplay].themes[mTheme].palette_total_size;
    int usercount = mpProject->mDisplays[mDisplay].themes[mTheme].palette_predefined;

    SetDlgItemInt(IDC_TOTAL_PALETTE_ENTRIES, totalcount);
    SetDlgItemInt(IDC_USER_PALETTE_ENTRIES, usercount);
    int autocount = totalcount - usercount;
    SetDlgItemInt(IDC_AUTO_PALETTE_ENTRIES, autocount);
    
    mpPaletteFrame->SetRowData();
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnGenGradient()
{
    INT start_index, end_index;
    theme_info *theme = &mpProject->mDisplays[mDisplay].themes[mTheme];

    start_index = GetDlgItemInt(IDC_START_INDEX);
    end_index = GetDlgItemInt(IDC_END_INDEX);

    if (start_index < 0)
    {
        start_index = 0;
    }

    if (end_index > theme->palette_predefined - 1)
    {
        end_index = theme->palette_predefined - 1;
    }

    if (start_index < end_index)
    {
        int bcolor, balpha, bred, bgreen, bblue;
        int fcolor, falpha, fred, fgreen, fblue;
        int ncolor, nalpha, nred, ngreen, nblue;
        int count = end_index - start_index;

        //pick start color
        fcolor = theme->palette[start_index];

        //pick end color
        bcolor = theme->palette[end_index];

        falpha = (fcolor >> 24) & 0xff;
        fred = (unsigned char)(fcolor >> 16);
        fgreen = (fcolor >> 8) & 0xff;
        fblue = fcolor & 0xff;

        balpha = (bcolor >> 24) & 0xff;
        bred = (unsigned char)(bcolor >> 16);
        bgreen = (bcolor >> 8) & 0xff;
        bblue = bcolor & 0xff;

        start_index++;

        for (int index = 1; index < count; index++)
        {
            //gen gradient color
            nalpha = (((balpha - falpha) * index) / count) + falpha;
            nred = (((bred - fred) * index) / count) + fred;
            nblue = (((bblue - fblue) * index) / count) + fblue;
            ngreen = (((bgreen - fgreen) * index) / count) + fgreen;
            ncolor = (nalpha << 24) | (nred << 16) | (ngreen << 8) | nblue;

            theme->palette[start_index++] = ncolor;
        }

        mpPaletteFrame->SetRowData();
    }
}

void PaletteLayoutDlg::Scroll(int deltaPos)
{
    int top_index = mpPaletteFrame->GetTopIndex();
  
    if (deltaPos != 0)
    {
        // Compute the new scroll position.
        top_index += deltaPos;

        if (top_index < 0)
        {
            top_index = 0;
        }
        else
        {
            if (top_index + VISIBLE_PALETTE_ROWS > 256)
            {
                top_index = 256 - VISIBLE_PALETTE_ROWS;
            }
        }
        if (top_index != mpPaletteFrame->GetTopIndex())
        {
            mpPaletteFrame->SetTopIndex(top_index);
            ::SetScrollPos(mPalIndexScroll.GetSafeHwnd(), SB_CTL, top_index, TRUE);
            mpPaletteFrame->SetRowData();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void PaletteLayoutDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *bar)
{
    int deltaPos = 0;
    int top_index = mpPaletteFrame->GetTopIndex();

    switch( nSBCode )
    {
    case SB_LINEUP:
        // Up arrow button on scrollbar was pressed.
        deltaPos = -1;
        break;

    case SB_LINEDOWN:
        // Down arrow button on scrollbar was pressed.
        deltaPos = 1;
        break;

    case SB_PAGEUP:
        // User clicked inbetween up arrow and thumb.
        deltaPos = -VISIBLE_PALETTE_ROWS;
        break;

    case SB_PAGEDOWN:
        // User clicked inbetween thumb and down arrow.
        deltaPos = VISIBLE_PALETTE_ROWS;
        break;

    case SB_THUMBTRACK:
        // Scrollbar thumb is being dragged.
        deltaPos = nPos - top_index;
        break;

    case SB_THUMBPOSITION:
        // Scrollbar thumb was released.
        deltaPos = nPos - top_index;
        break;

    default:
        // We don't process other scrollbar messages.
        return;
    }

    // Scroll the window if needed.
    Scroll(deltaPos);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT PaletteLayoutDlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CString text;
    CWnd *pWnd;
    int index;
    GX_COLOR color;
    CStringArray strlist;

    switch (wParam)
    {
    case TEST_SET_PREDEFINED_PALETTE_ENTRY:
        pWnd = GetDlgItem(IDC_USER_PALETTE_ENTRIES);

        if (pWnd)
        {
            text.Format(_T("%d"), lParam);
            pWnd->SetWindowText(text);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_USER_PALETTE_ENTRIES, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_SET_PALETTE_COLOR:
        SplitString(GetTestingParam(0), ',', &strlist);
        index = _tstoi(strlist.GetAt(0));
        color = _tstoi(strlist.GetAt(1));
        NotifyColorChanged(index, color);
        break;

    case TEST_SAVE_PALETTE_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_PALETTE_EDIT:
        OnCancel();
        break;
    }

    return 0;
}

BEGIN_MESSAGE_MAP(palette_row, express_table_row)
	ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_EN_KILLFOCUS(ID_REDFIELD, palette_row::ColorChanged)
    ON_EN_KILLFOCUS(ID_GREENFIELD, palette_row::ColorChanged)
    ON_EN_KILLFOCUS(ID_BLUEFIELD, palette_row::ColorChanged)
    ON_EN_KILLFOCUS(ID_ALPHAFIELD, palette_row::ColorChanged)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
palette_row::palette_row()
{
    mIndex = -1;
}

///////////////////////////////////////////////////////////////////////////////
int palette_row::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    express_table_row::OnCreate(lpCreateStruct);
    CRect client;
    CRect size;

    GetClientRect(&client);

    int offset = client.Width() / 8;
    size = client;
    size.top += 4;
    size.bottom -= 4;
    size.left += 5;
    size.right = offset - 10;
    mIndexPrompt.Create(_T("--"), WS_CHILD | SS_CENTER | WS_VISIBLE, size, this, ID_PAL_INDEX);

    size.left = offset * 4 + 5;
    size.right = size.left + offset - 5;
    mAlphaField.Create(WS_CHILD | SS_RIGHT | WS_VISIBLE | ES_NUMBER | WS_BORDER | WS_TABSTOP, size, this, ID_ALPHAFIELD);
    SetAccessibleDescription(mAlphaField.GetSafeHwnd(), L"Use arrow keys to navigate.");
    SetAccessibleFullDescription(mAlphaField.GetSafeHwnd(), L"Use arrow keys to navigate.");

    size.OffsetRect(offset, 0);
    mRedField.Create(WS_CHILD|SS_RIGHT|WS_VISIBLE|ES_NUMBER|WS_BORDER | WS_TABSTOP, size, this, ID_REDFIELD);
    SetAccessibleDescription(mRedField.GetSafeHwnd(), L"Use arrow keys to navigate.");
    SetAccessibleFullDescription(mRedField.GetSafeHwnd(), L"Use arrow keys to navigate.");

    size.OffsetRect(offset, 0);
    mGreenField.Create(WS_CHILD|SS_RIGHT|WS_VISIBLE|ES_NUMBER|WS_BORDER | WS_TABSTOP, size, this, ID_GREENFIELD);
    SetAccessibleDescription(mGreenField.GetSafeHwnd(), L"Use arrow keys to navigate.");
    SetAccessibleFullDescription(mGreenField.GetSafeHwnd(), L"Use arrow keys to navigate.");

    size.OffsetRect(offset, 0);
    mBlueField.Create(WS_CHILD|SS_RIGHT|WS_VISIBLE|ES_NUMBER|WS_BORDER | WS_TABSTOP, size, this, ID_BLUEFIELD);
    SetAccessibleDescription(mBlueField.GetSafeHwnd(), L"Use arrow keys to navigate.");
    SetAccessibleFullDescription(mBlueField.GetSafeHwnd(), L"Use arrow keys to navigate.");
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
void palette_row::SetData(int index, GX_COLOR color, BOOL Enabled)
{
    CEdit *edit;

    if (mIndex != index)
    {
        CString acc_name;

        // set accessible names
        acc_name.Format(_T("alpha %d"), index);
        SetControlAccessibleName(GetDlgItem(ID_ALPHAFIELD)->GetSafeHwnd(), acc_name.GetString());

        acc_name.Format(_T("red %d"), index);
        SetControlAccessibleName(GetDlgItem(ID_REDFIELD)->GetSafeHwnd(), acc_name.GetString());

        acc_name.Format(_T("green %d"), index);
        SetControlAccessibleName(GetDlgItem(ID_GREENFIELD)->GetSafeHwnd(), acc_name.GetString());

        acc_name.Format(_T("blue %d"), index);
        SetControlAccessibleName(GetDlgItem(ID_BLUEFIELD)->GetSafeHwnd(), acc_name.GetString());

        mIndex = index;
    }

    mColor = color;
    mEnabled = Enabled;

    SetDlgItemInt(ID_PAL_INDEX, index);

    if (Enabled)
    {
        mAlpha = (color >> 24) & 0xff;
        SetDlgItemInt(ID_ALPHAFIELD, mAlpha);

        mRed = (unsigned char) (color >> 16);
        SetDlgItemInt(ID_REDFIELD, mRed);

        mGreen = (color >> 8) & 0xff;
        SetDlgItemInt(ID_GREENFIELD, mGreen);
    
        mBlue = color & 0xff;
        SetDlgItemInt(ID_BLUEFIELD, mBlue);

        edit = (CEdit *)GetDlgItem(ID_ALPHAFIELD);
        edit->SetReadOnly(FALSE);
        edit = (CEdit *) GetDlgItem(ID_REDFIELD);
        edit->SetReadOnly(FALSE);
        edit = (CEdit *) GetDlgItem(ID_GREENFIELD);
        edit->SetReadOnly(FALSE);
        edit = (CEdit *) GetDlgItem(ID_BLUEFIELD);
        edit->SetReadOnly(FALSE);
    }
    else
    {
        SetDlgItemText(ID_ALPHAFIELD, _T("- - "));
        SetDlgItemText(ID_REDFIELD, _T("- - "));
        SetDlgItemText(ID_GREENFIELD, _T("- - "));
        SetDlgItemText(ID_BLUEFIELD, _T("- - "));
        edit = (CEdit *)GetDlgItem(ID_ALPHAFIELD);
        edit->SetReadOnly();
        edit = (CEdit *) GetDlgItem(ID_REDFIELD);
        edit->SetReadOnly();
        edit = (CEdit *) GetDlgItem(ID_GREENFIELD);
        edit->SetReadOnly();
        edit = (CEdit *) GetDlgItem(ID_BLUEFIELD);
        edit->SetReadOnly();
    }
    Invalidate();
}


///////////////////////////////////////////////////////////////////////////////
void palette_row::OnPaint()
{
    CBrush boxbrush;
   // CBrush *old_brush;
    CRect boxsize;
    CPaintDC dc(this);
    express_table_row::OnPaint();

    GetClientRect(&boxsize);
    int offset = boxsize.Width() / 8;
    boxsize.left  = offset + 10;
    boxsize.right = offset * 4 - 10;
    boxsize.top += 5;
    boxsize.bottom -= 5;

    if (mEnabled)
    {
        boxbrush.CreateSolidBrush(RGB(mRed, mGreen, mBlue));
    }
    else
    {
        boxbrush.CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 0));
    }
    //old_brush = dc->SelectObject(&boxbrush);

    dc.FillRect(&boxsize, &boxbrush);
}

///////////////////////////////////////////////////////////////////////////////
BOOL palette_row::OnEraseBkgnd(CDC* pDC)
{
    CBrush evenBrush(RGB(210, 210, 210));
    CBrush oddBrush(RGB(190, 190, 190));
    CBrush *pOldBrush;

    if (mIndex & 1)
    {
        pOldBrush = pDC->SelectObject(&oddBrush);
    }
    else
    {
        pOldBrush = pDC->SelectObject(&evenBrush);
    }
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed

    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
        PATCOPY);
    pDC->SelectObject(pOldBrush);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void palette_row::ColorChanged()
{
    BOOL changed = FALSE;
    int val = GetDlgItemInt(ID_ALPHAFIELD);
    if (val < 0 || val > 255)
    {
        SetDlgItemInt(ID_ALPHAFIELD, mAlpha);
    }
    else
    {
        mAlpha = val;
        changed = TRUE;
    }

    val = GetDlgItemInt(ID_REDFIELD);
    if (val < 0 || val > 255)
    {
        SetDlgItemInt(ID_REDFIELD, mRed);
    }
    else
    {
        mRed = val;
        changed = TRUE;
    }

    val = GetDlgItemInt(ID_GREENFIELD);
    if (val < 0 || val > 255)
    {
        SetDlgItemInt(ID_GREENFIELD, mGreen);
    }
    else
    {
        mGreen = val;
        changed = TRUE;
    }
    val = GetDlgItemInt(ID_BLUEFIELD);
    if (val < 0 || val > 255)
    {
        SetDlgItemInt(ID_BLUEFIELD, mBlue);
    }
    else
    {
        mBlue = val;
        changed = TRUE;
    }

    if (changed)
    {
        mColor = (mAlpha << 24) | (mRed << 16) | (mGreen << 8) | mBlue;
        
        palette_frame *win = (palette_frame*) GetParent();
        win->NotifyColorChanged(mIndex, mColor);
        Invalidate();   // force repaint color swatch
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL palette_row::PreTranslateMessage(MSG *pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        CWnd* focus_owner = GetFocus();
        CWnd* next_focus_owner = NULL;
        palette_frame *frame = (palette_frame*)GetParent();

        switch (pMsg->wParam)
        {
        case VK_RETURN:
            ColorChanged();
            break;

        case VK_LEFT:
        case VK_RIGHT:
            next_focus_owner = GetNextTabStopChild(focus_owner, pMsg->wParam);
            break;

        case VK_UP:
            next_focus_owner = GetNextTabStopChild(focus_owner, pMsg->wParam);
            if (!next_focus_owner)
            {
                if (frame->GetTopIndex() > 0)
                {
                    frame->Scroll(-1);
                }
                next_focus_owner = focus_owner;
            }
            break;

        case VK_DOWN:
            next_focus_owner = GetNextTabStopChild(focus_owner, pMsg->wParam);
            if (!next_focus_owner)
            {
                if (frame->GetTopIndex() < 256 - VISIBLE_PALETTE_ROWS)
                {
                    frame->Scroll(1);
                }
                next_focus_owner = focus_owner;
            }
            break;

        case VK_HOME:
            next_focus_owner = GetNextTabStopChild(focus_owner, pMsg->wParam);
            if (frame->GetTopIndex() > 0)
            {
                frame->Scroll(-frame->GetTopIndex());
            }
            break;

        case VK_END:
            next_focus_owner = GetNextTabStopChild(focus_owner, pMsg->wParam);
            if (frame->GetTopIndex() < 256 - VISIBLE_PALETTE_ROWS)
            {
                frame->Scroll(256 - VISIBLE_PALETTE_ROWS - frame->GetTopIndex());
            }
            break;

        case VK_PRIOR:
            next_focus_owner = focus_owner;
            frame->Scroll(-VISIBLE_PALETTE_ROWS);
            break;

        case VK_NEXT:
            next_focus_owner = focus_owner;
            frame->Scroll(VISIBLE_PALETTE_ROWS);
            break;

        default:
            return express_table_row::PreTranslateMessage(pMsg);
        }
        
        if (next_focus_owner == focus_owner)
        {
            next_focus_owner->NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, OBJID_CLIENT, CHILDID_SELF);
            CString msg(L"edit ");
            if (::GetWindowLongPtr(next_focus_owner->GetSafeHwnd(), GWL_STYLE) & ES_READONLY)
            {
                msg.Append(L"read only ");
            }
            msg.Append(L"Use arrow keys to navigate.");
            frame->UpdateStatusMsg(msg);
        }

        AssignFocus(next_focus_owner);
        return TRUE;
    }

    return express_table_row::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(palette_frame, CWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
palette_frame::palette_frame(theme_info *theme)
{
    mpTheme = theme;
    mTopIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////
palette_frame::~palette_frame()
{
};

///////////////////////////////////////////////////////////////////////////////
int palette_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    int index;
    CRect client;

    GetClientRect(&client);
    int row_height = (client.Height() + VISIBLE_PALETTE_ROWS - 1) / VISIBLE_PALETTE_ROWS;

    GetClientRect(&client);
    client.bottom = client.top + row_height;

    for (index = 0; index < VISIBLE_PALETTE_ROWS; index++)
    {
        mPaletteRows[index].Create(target_class_name, _T("PaletteRow"),
            WS_CHILD | WS_VISIBLE, client, this, NULL);
        client.OffsetRect(0, row_height);
    }

    SetRowData();

    mStatusMsg.Create(L"", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    SetLiveRegion(mStatusMsg.GetSafeHwnd());
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void palette_frame::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    //pick first row
    palette_row* parent = (palette_row*)GetWindow(GW_CHILD);
    CWnd* child;

    // set focus to the first child that has WS_TABSTOP style
    if (parent)
    {
        //pick first child of the row
        child = parent->GetWindow(GW_CHILD);

        //loop through child list until we find a child with WS_TABSTOP style,
        //and pass focus to this child
        while (child)
        {
            if (child->GetStyle() & WS_TABSTOP)
            {
                parent->AssignFocus(child);
                break;
            }

            child = child->GetWindow(GW_HWNDNEXT);
        }
    }
    // TODO: Add your message handler code here
}

///////////////////////////////////////////////////////////////////////////////
void palette_frame::SetRowData()
{
    int index;
    BOOL Enabled;
    GX_COLOR color;
    int slot;

    for (index = 0; index < VISIBLE_PALETTE_ROWS; index++)
    {
        slot = index + mTopIndex;
        if (slot < mpTheme->palette_predefined)
        {
            Enabled = TRUE;
        }
        else
        {
            Enabled = FALSE;
        }

        color = mpTheme->palette[slot];
        mPaletteRows[index].SetData(index + mTopIndex, color, Enabled);
    }
}

///////////////////////////////////////////////////////////////////////////////
void palette_frame::NotifyColorChanged(int index, GX_COLOR color)
{
    PaletteLayoutDlg* dlg = (PaletteLayoutDlg*)GetParent();

    dlg->NotifyColorChanged(index, color);
}

///////////////////////////////////////////////////////////////////////////////
void palette_frame::Scroll(int deltaPos)
{
    PaletteLayoutDlg* dlg = (PaletteLayoutDlg*)GetParent();
    dlg->Scroll(deltaPos);
}

///////////////////////////////////////////////////////////////////////////////
void palette_frame::UpdateStatusMsg(CString msg)
{
    mStatusMsg.SetWindowText(msg);
    mStatusMsg.NotifyWinEvent(
        EVENT_OBJECT_LIVEREGIONCHANGED,
        OBJID_CLIENT,
        CHILDID_SELF);
}
