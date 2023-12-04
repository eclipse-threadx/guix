// color_edit_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "color_edit_dialog.h"
#include "afxdialogex.h"

#define SWATCH_SIZE 4
#define PALETTE_BOX_SIZE 14

enum color_edit_dlg_test_commands{
    TEST_SET_COLOR_NAME = 1,
    TEST_SET_COLOR_RED,
    TEST_SET_COLOR_GREEN,
    TEST_SET_COLOR_BLUE,
    TEST_SET_COLOR_HUE,
    TEST_SET_COLOR_SAT,
    TEST_SET_COLOR_LUM,
    TEST_SET_PALETTE_COLOR_INDEX,
    TEST_SAVE_COLOR_EDIT,
    TEST_CANCEL_COLOR_EDIT
};

extern CString target_class_name;

BEGIN_MESSAGE_MAP(color_gradient_win, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

color_gradient_win::color_gradient_win(CWnd *pParent)
{
    int dpi = GetSystemDPI();

    mpParent = (color_edit_dialog *)pParent;

    if (mpParent->GetPalette()->palette)
    {
        m_swatch_size = MulDiv(PALETTE_BOX_SIZE, dpi, DEFAULT_DPI_96);
    }
    else
    {
        m_swatch_size = MulDiv(SWATCH_SIZE, dpi, DEFAULT_DPI_96);
    }

    mpProject = GetOpenProject();
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::OnPaint()
{
    CWnd::OnPaint();

    CRect client;
    GetClientRect(&client);

    CDC *pDC = GetDC();

    /* Create off-screen DC for drawing. */
    CDC        dcMem;
    HBITMAP    hbmMem;
    HANDLE     hOld;

    dcMem.CreateCompatibleDC(pDC);
    hbmMem = CreateCompatibleBitmap(pDC->GetSafeHdc(), client.Width(), client.Height());
    hOld = SelectObject(dcMem, hbmMem);

    dcMem.FillSolidRect(client, GetSysColor(COLOR_3DFACE));

    if (mpParent->GetPalette()->palette)
    {
        PaintColorPalette(&dcMem);
        PaintColorPaletteHighlight(&dcMem);
    }
    else
    {
        PaintColorGradient(&dcMem);
        PaintColorGradientHighlight(&dcMem);
    }

    /* Transfer the off-screen DC to the current DC. */
    pDC->BitBlt(0, 0, client.Width(), client.Height(), &dcMem, 0, 0, SRCCOPY);

    /* Free-up the off-screen DC.  */
    SelectObject(dcMem, hOld);
    DeleteObject(hbmMem);
    DeleteDC(dcMem);

    ReleaseDC(pDC);
}

///////////////////////////////////////////////////////////////////////////////
BOOL color_gradient_win::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::PaintColorPalette(CDC *pDC)
{
    CRect boxrect;
    CRect swatch;
    int row, column;
    int index = 0;

    GetClientRect(boxrect);

    int rows, columns;

    GetColorGradientDimension(&rows, &columns);

    swatch.left = boxrect.left;
    swatch.top = boxrect.top;
    swatch.right = swatch.left + m_swatch_size;
    swatch.bottom = swatch.top + m_swatch_size;

    palette_info *palinfo = mpParent->GetPalette();
    GX_COLOR *color = palinfo->palette;

    for (row = 1; row <= rows; row++)
    {
        swatch.left = boxrect.left;
        swatch.right = swatch.left + m_swatch_size;

        for (column = 0; column < columns; column++, index++)
        {
            if (index >= palinfo->used_size)
            {
               return;
            }

            COLORREF cr = GxColorToColorRef(*color);
            color++;
            CBrush swatchbrush(cr);
            CPen swatchpen(PS_SOLID, 0, cr);
            pDC->SelectObject(swatchbrush);
            pDC->SelectObject(swatchpen);
            pDC->Rectangle(swatch);
            swatch.OffsetRect(m_swatch_size, 0);
        }
        swatch.OffsetRect(0, m_swatch_size);
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::PaintColorPaletteHighlight(CDC* pDC)
{
    // Calculate selected box
    CRect selected_box;
    GetClientRect(selected_box);

    selected_box.right = selected_box.left + m_swatch_size;
    selected_box.bottom = selected_box.top + m_swatch_size;

    int select_row, select_column;

    GetSelectedPos(&select_row, &select_column);

    selected_box.OffsetRect(m_swatch_size * select_column, m_swatch_size * select_row);

    // Highlith selected color box.
    CPen outline_pen;
    CPen fill_pen;

    if (GetFocus() == this)
    {
        outline_pen.CreatePen(PS_SOLID, 1, RGB_COLOR_BLACK);
        fill_pen.CreatePen(PS_SOLID, 1, RGB_COLOR_WHITE);
    }
    else
    {
        outline_pen.CreatePen(PS_SOLID, 1, RGB_COLOR_BLACK);
        fill_pen.CreatePen(PS_SOLID, 1, RGB_COLOR_GRAY);
    }

    pDC->SelectObject(GetStockObject(NULL_BRUSH));
    pDC->SelectObject(outline_pen);
    pDC->Rectangle(selected_box);

    selected_box.DeflateRect(1, 1);
    pDC->SelectObject(fill_pen);
    pDC->Rectangle(selected_box);

    selected_box.DeflateRect(1, 1);
    pDC->SelectObject(outline_pen);
    pDC->Rectangle(selected_box);
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::PaintColorGradient(CDC *pDC)
{
    CRect boxrect;
    CRect swatch;
    int row, column;

    GetClientRect(boxrect);

    int rows, columns;

    GetColorGradientDimension(&rows, &columns);

    swatch.left = boxrect.left;
    swatch.top = boxrect.top;
    swatch.right = swatch.left + m_swatch_size;
    swatch.bottom = swatch.top + m_swatch_size;

    CBrush tempbrush(RGB(0, 0, 0));
    CPen temppen(PS_SOLID, 0, RGB(0, 0, 0));

    int red, green, blue;
    double Saturation, Hue, Luminance;
    int lum;

    mpParent->GetHSLColor(NULL, NULL, &lum);
    Luminance = lum / 100.0;

    for (row = 0; row < rows; row++)
    {
        Saturation = (double)row / (double)(rows - 1);
        swatch.left = boxrect.left;
        swatch.right = swatch.left + m_swatch_size;

        for (column = 0; column < columns; column++)
        {
            Hue = (double)column / (double)(columns - 1);
            mpParent->HslToRgb(Hue, Saturation, Luminance, red, green, blue);
            CBrush swatchbrush(RGB(red, green, blue));
            CPen swatchpen(PS_SOLID, 0, RGB(red, green, blue));
            pDC->SelectObject(swatchbrush);
            pDC->SelectObject(swatchpen);
            pDC->Rectangle(swatch);
            swatch.OffsetRect(m_swatch_size, 0);
        }
        swatch.OffsetRect(0, m_swatch_size);
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::PaintColorGradientHighlight(CDC* pDC)
{

    // Calculate selected box
    int select_row, select_column;
    GetSelectedPos(&select_row, &select_column);

    CRect selected_box;
    GetClientRect(selected_box);

    selected_box.right = selected_box.left + m_swatch_size;
    selected_box.bottom = selected_box.top + m_swatch_size;

    selected_box.OffsetRect(m_swatch_size * select_column, m_swatch_size * select_row);

    CPen highlight_pen;

    if (GetFocus() == this)
    {
        highlight_pen.CreatePen(PS_SOLID, 0, RGB_COLOR_WHITE);
    }
    else
    {
        highlight_pen.CreatePen(PS_SOLID, 0, RGB_COLOR_BLACK);
    }

    int cx = (selected_box.left + selected_box.right) >> 1;
    int cy = (selected_box.top + selected_box.bottom) >> 1;

    // Highlight selected box
    pDC->SelectObject(&highlight_pen);
    pDC->MoveTo(selected_box.left - m_swatch_size, cy);
    pDC->LineTo(selected_box.left, cy);
    pDC->MoveTo(selected_box.right + m_swatch_size, cy);
    pDC->LineTo(selected_box.right, cy);
    pDC->MoveTo(cx, selected_box.top - m_swatch_size);
    pDC->LineTo(cx, selected_box.top);
    pDC->MoveTo(cx, selected_box.bottom + m_swatch_size);
    pDC->LineTo(cx, selected_box.bottom);
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::GetColorGradientDimension(int *rows, int *cols)
{
    CRect boxrect;

    GetClientRect(boxrect);

    if (rows)
    {
        (*rows) = (boxrect.Height() / m_swatch_size);
    }

    if (cols)
    {
        (*cols) = (boxrect.Width() / m_swatch_size);
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::InvalidateSelection(int row, int col)
{
    CRect swatch;
    GetClientRect(swatch);

    swatch.right = swatch.left + m_swatch_size;
    swatch.bottom = swatch.top + m_swatch_size;

    if (!mpParent->GetPalette()->palette)
    {
        swatch.InflateRect(m_swatch_size, m_swatch_size);
    }

    swatch.OffsetRect(m_swatch_size * col, m_swatch_size * row);

    // Invalidate selection area
    InvalidateRect(&swatch);
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::GetSelectedPos(int *row, int *col)
{
    if (mpParent->GetPalette()->palette)
    {
        int color_index = mpParent->GetPaletteColor();
        int columns;

        GetColorGradientDimension(NULL, &columns);

        if (row)
        {
            (*row) = color_index / columns;
        }

        if (col)
        {
            (*col) = color_index % columns;
        }
    }
    else
    {
        int sat, hue, lum;
        int rows, columns;

        GetColorGradientDimension(&rows, &columns);

        mpParent->GetHSLColor(&hue, &sat, &lum);

        if (row)
        {
            (*row) = (sat * (rows - 1) + 50) / 100;
        }

        if (col)
        {
            (*col) = (hue * (columns - 1) + 180) / 360;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::SetSelectedPos(int row, int col)
{
    int rows, columns;
    GetColorGradientDimension(&rows, &columns);

    if ((row < 0) || (row >= rows) || (col < 0) || (col >= columns))
    {
        return;
    }
    palette_info* palinfo = mpParent->GetPalette();

    int old_row, old_col;
    GetSelectedPos(&old_row, &old_col);

    GX_BOOL update = FALSE;

    if (palinfo->palette)
    {
        int index = row * columns + col;

        if ((index >= 0) && (index < palinfo->used_size))
        {
            mpParent->SetPaletteColor(index);
            update = TRUE;;
        }
    }
    else
    {
        double satfloat = (double)row / (double)(rows - 1);
        double huefloat = (double)col / (double)(columns - 1);
        satfloat *= 100;
        int sat = (int)(satfloat + 0.5);

        huefloat *= 360;
        int hue = (int)(huefloat + 0.5);

        color_edit_dialog* dlg = (color_edit_dialog*)GetParent();
        dlg->SetHSLColor(hue, sat);

        update = TRUE;
    }

    if (update)
    {
        InvalidateSelection(old_row, old_col);
        InvalidateSelection(row, col);
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus() != this)
    {
        SetFocus();
    }

    CRect boxrect;
    GetClientRect(boxrect);

    if (boxrect.PtInRect(point))
    {
        int row, column;
        column = (point.x - boxrect.left) / m_swatch_size;
        row = (point.y - boxrect.top) / m_swatch_size;

        SetSelectedPos(row, column);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL color_gradient_win::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        BOOL processed = TRUE;
        int selected_row, selected_col;

        GetSelectedPos(&selected_row, &selected_col);

        switch (pMsg->wParam)
        {
        case VK_UP:
            selected_row--;
            break;

        case VK_DOWN:
            selected_row++;
            break;

        case VK_LEFT:
            selected_col--;
            break;

        case VK_RIGHT:
            selected_col++;
            break;

        default:
            processed = FALSE;
            break;
        }

        if (processed)
        {
            SetSelectedPos(selected_row, selected_col);
            return TRUE;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void color_gradient_win::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    Invalidate();
}

BEGIN_MESSAGE_MAP(color_edit_dialog, express_dialog)
    ON_EN_KILLFOCUS(IDC_EDIT_RED, &color_edit_dialog::OnEnChangeEditRed)
    ON_EN_KILLFOCUS(IDC_EDIT_GREEN, &color_edit_dialog::OnEnChangeEditGreen)
    ON_EN_KILLFOCUS(IDC_EDIT_BLUE, &color_edit_dialog::OnEnChangeEditBlue)
    ON_EN_KILLFOCUS(IDC_EDIT_ALPHA, &color_edit_dialog::OnEnChangeEditAlpha)
    ON_EN_KILLFOCUS(IDC_EDIT_HUE, &color_edit_dialog::OnEnChangeEditHue)
    ON_EN_KILLFOCUS(IDC_EDIT_SATURATION, &color_edit_dialog::OnEnChangeEditSaturation)
    ON_EN_KILLFOCUS(IDC_EDIT_LUMINANCE, &color_edit_dialog::OnEnChangeEditLuminance)
    ON_MESSAGE(STUDIO_TEST, &color_edit_dialog::OnTestMessage)
    ON_WM_HSCROLL()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    ON_BN_CLICKED(IDOK, OnOK)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// color_edit_dialog dialog

///////////////////////////////////////////////////////////////////////////////
color_edit_dialog::color_edit_dialog(int display_index, CWnd* pParent /*=NULL*/)
    : express_dialog(color_edit_dialog::IDD, pParent)
{
    IconId = IDB_COLORS;
    SetTitleText("Edit Color");

    mpRes = NULL;
    mName = "NEW_COLOR";
    mRed = mGreen = mBlue = 0;
    mAlpha = 255;
    mDisplayIndex = display_index;
    mResId = GX_MAX_DEFAULT_COLORS;
    mColorIndex = 0;

    mpColorGradientWin = NULL;
    mpProject = GetOpenProject();

    if (mpProject)
    {
        INT active_theme = mpProject->mDisplays[mDisplayIndex].active_theme;
        GX_COLOR *palette = mpProject->mDisplays[mDisplayIndex].themes[active_theme].palette;

        /* Create dynamic palette for some formats. */
        /* default mPalette. */
        mPalette.palette = NULL;
        mPalette.total_size = 0;
        mPalette.used_size = 0;

        switch (mpProject->mDisplays[display_index].colorformat)
        {
        case GX_COLOR_FORMAT_8BIT_PALETTE:
            if (palette)
            {
                mPalette.palette = palette;
                mPalette.total_size = mpProject->mDisplays[display_index].themes[active_theme].palette_total_size;
                mPalette.used_size = mpProject->mDisplays[display_index].themes[active_theme].palette_predefined;
            }
            break;

        case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
            if (palette == GX_NULL)
            {
                ProjectConfigDlg::CreateDefault332RGBPalette(mPalette);
            }
            break;

        case GX_COLOR_FORMAT_4BIT_GRAY:
            if (palette == GX_NULL)
            {
                ProjectConfigDlg::CreateDefault4BppPalette(mPalette);
            }
            break;

        case GX_COLOR_FORMAT_MONOCHROME:
            if (palette == GX_NULL)
            {
                ProjectConfigDlg::CreateDefault1BppPalette(mPalette);
            }
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
color_edit_dialog::color_edit_dialog(resource_item *res, int display_index, CWnd* pParent /*=NULL*/)
    : express_dialog(color_edit_dialog::IDD, pParent)
{
    IconId = IDB_COLORS;
    SetTitleText("Edit Color");

    mpRes = res->mpRes;
    mName = res->mpRes->name;
    GX_COLOR color = res->GetRGBColor(display_index);

    mAlpha = (color >> 24) & 0xff;
    mRed = (color >> 16) & 0xff;;
    mGreen = (color >> 8) & 0xff;
    mBlue = color & 0xff;
    mDisplayIndex = display_index;
    mpColorGradientWin = NULL;

    mpProject = GetOpenProject();
    mResId = mpProject->GetResourceId(display_index, res->mpRes);

    INT active_theme = mpProject->mDisplays[mDisplayIndex].active_theme;
    GX_COLOR *palette = mpProject->mDisplays[mDisplayIndex].themes[active_theme].palette;

    int color_format = mpProject->mDisplays[display_index].colorformat;

    if (!IsAlphaFormat(color_format))
    {
        mAlpha = 0xff;
    }

    /* Create dynamic palette for some formats. */
    /* default mPalette. */
    mPalette.palette = NULL;
    mPalette.total_size = 0;
    mPalette.used_size = 0;

    switch (color_format)
    {
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (palette)
        {
            mPalette.palette = palette;
            mPalette.total_size = mpProject->mDisplays[display_index].themes[active_theme].palette_total_size;
            mPalette.used_size = mpProject->mDisplays[display_index].themes[active_theme].palette_predefined;
            mColorIndex = res->mpRes->colorval;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        if (palette == GX_NULL)
        {
            ProjectConfigDlg::CreateDefault332RGBPalette(mPalette);
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (palette == GX_NULL)
        {
            ProjectConfigDlg::CreateDefault4BppPalette(mPalette);
        }
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        if (palette == GX_NULL)
        {
            ProjectConfigDlg::CreateDefault1BppPalette(mPalette);
        }
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
color_edit_dialog::~color_edit_dialog()
{
    switch (mpProject->mDisplays[mDisplayIndex].colorformat)
    {
    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
    case GX_COLOR_FORMAT_4BIT_GRAY:
    case GX_COLOR_FORMAT_MONOCHROME:

        if (mPalette.palette != GX_NULL)
        {
            delete[] mPalette.palette;
        }
        break;

    default:
        break;
    }

    if (mpColorGradientWin)
    {
        delete mpColorGradientWin;
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnOK()
{
    CWnd *child = GetFocus();
    if (child)
    {
        int id = child->GetDlgCtrlID();
        switch (id)
        {
        case IDC_EDIT_RED:
        case IDC_EDIT_GREEN:
        case IDC_EDIT_BLUE:
        case IDC_EDIT_ALPHA:
        case IDC_EDIT_HUE:
        case IDC_EDIT_SATURATION:
        case IDC_EDIT_LUMINANCE:
            SetFocus();
            return;

        default:
            break;
        }
    }

    CEdit* edit = (CEdit *)GetDlgItem(IDC_COLOR_NAME);
    CString text;
    GetDlgItemText(IDC_COLOR_NAME, text);

    if (mpRes == NULL)
    {
        if (NameExists(mDisplayIndex, RES_TYPE_COLOR, text))
        {
            Notify("Color name already exits.", this);
            SetDlgItemText(IDC_COLOR_NAME, NULL);
            return;
        }
    }
    else
    {
        if (_tcscmp(text, mName))
        {
            if (NameExists(mDisplayIndex, RES_TYPE_COLOR, text))
            {
                Notify("Color name already exits.", this);
                SetDlgItemText(IDC_COLOR_NAME, mName);
                return;
            }
        }
    }

    if (TestInputName(edit, "Color Name", mName, this))
    {
        return express_dialog::OnOK();
    }
}

///////////////////////////////////////////////////////////////////////////////
int color_edit_dialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (express_dialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    mColorUpdateMsg.Create(L"", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    SetLiveRegion(mColorUpdateMsg.GetSafeHwnd());

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
BOOL color_edit_dialog::OnInitDialog()
{
    express_dialog::OnInitDialog();

    CRect color_lable_size;
    CRect boxrect;
    GetClientRect(&boxrect);

    //Get rectangle box of color name edit control
    GetDlgItem(IDC_COLORS)->GetWindowRect(&color_lable_size);
    ScreenToClient(&color_lable_size);

    boxrect.top = color_lable_size.bottom + 10;
    boxrect.bottom -= (m_status_bar_height + 10);
    boxrect.left = color_lable_size.left;
    boxrect.right = color_lable_size.right;
    mpColorGradientWin = new color_gradient_win(this);
    mpColorGradientWin->Create(target_class_name, _T("Colors"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, boxrect, this, 0);
    mpColorGradientWin->SetWindowPos(GetDlgItem(IDC_COLORS), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    AddCancelButton();
    AddSaveButton();

    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_RED)->GetSafeHwnd(), _T("Red value must be between 0 and 255"));
    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_GREEN)->GetSafeHwnd(), _T("Green value must be between 0 and 255"));
    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_BLUE)->GetSafeHwnd(), _T("Blue value must be between 0 and 255"));
    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_ALPHA)->GetSafeHwnd(), _T("Alpha value must be between 0 and 255"));
    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_HUE)->GetSafeHwnd(), _T("Hue value must be between 0 and 360"));
    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_SATURATION)->GetSafeHwnd(), _T("Saturation value must be between 0 and 100"));
    SetAccessibleHelpString(GetDlgItem(IDC_EDIT_LUMINANCE)->GetSafeHwnd(), _T("Luminance value must be between 0 and 100"));

    SetControlAccessibleName(GetDlgItem(IDC_RED_SLIDER)->GetSafeHwnd(), _T("Red"));
    SetControlAccessibleName(GetDlgItem(IDC_GREEN_SLIDER)->GetSafeHwnd(), _T("Green"));
    SetControlAccessibleName(GetDlgItem(IDC_BLUE_SLIDER)->GetSafeHwnd(), _T("Blue"));
    SetControlAccessibleName(GetDlgItem(IDC_ALPHA_SLIDER)->GetSafeHwnd(), _T("Alpha"));
    SetControlAccessibleName(GetDlgItem(IDC_HUE_SLIDER)->GetSafeHwnd(), _T("Hue"));
    SetControlAccessibleName(GetDlgItem(IDC_SAT_SLIDER)->GetSafeHwnd(), _T("Sat"));
    SetControlAccessibleName(GetDlgItem(IDC_LUM_SLIDER)->GetSafeHwnd(), _T("Lum"));
 
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_COLOR_NAME, mName);
    DDX_Control(pDX, IDC_EDIT_RED, mRedEdit);
    DDX_Control(pDX, IDC_EDIT_GREEN, mGreenEdit);
    DDX_Control(pDX, IDC_EDIT_BLUE, mBlueEdit);
    DDX_Control(pDX, IDC_EDIT_ALPHA, mAlphaEdit);
    DDX_Control(pDX, IDC_EDIT_HUE, mHueEdit);
    DDX_Control(pDX, IDC_EDIT_SATURATION, mSaturationEdit);
    DDX_Control(pDX, IDC_EDIT_LUMINANCE, mLuminanceEdit);
    DDX_Control(pDX, IDC_RED_SLIDER, mRedSlider);
    DDX_Control(pDX, IDC_GREEN_SLIDER, mGreenSlider);
    DDX_Control(pDX, IDC_BLUE_SLIDER, mBlueSlider);
    DDX_Control(pDX, IDC_HUE_SLIDER, mHueSlider);
    DDX_Control(pDX, IDC_SAT_SLIDER, mSaturationSlider);
    DDX_Control(pDX, IDC_LUM_SLIDER, mLuminanceSlider);
    DDX_Control(pDX, IDC_ALPHA_SLIDER, mAlphaSlider);

    mRedSlider.SetRange(0, 255);
    mGreenSlider.SetRange(0, 255);
    mBlueSlider.SetRange(0, 255);
    mAlphaSlider.SetRange(0, 255);

    mHueSlider.SetRange(0, 360);
    mSaturationSlider.SetRange(0, 100);
    mLuminanceSlider.SetRange(0, 100);

    DDX_Text(pDX, IDC_EDIT_RED, mRed);
    DDX_Text(pDX, IDC_EDIT_GREEN, mGreen);
    DDX_Text(pDX, IDC_EDIT_BLUE, mBlue);
    DDX_Text(pDX, IDC_EDIT_ALPHA, mAlpha);

    DDX_Slider(pDX, IDC_RED_SLIDER, mRed);
    DDX_Slider(pDX, IDC_GREEN_SLIDER, mGreen);
    DDX_Slider(pDX, IDC_BLUE_SLIDER, mBlue);
    DDX_Slider(pDX, IDC_ALPHA_SLIDER, mAlpha);

    if (!pDX->m_bSaveAndValidate)
    {
        UpdateHSLFields();
    }

    if (mResId < GX_MAX_DEFAULT_COLORS)
    {
        GetDlgItem(IDC_COLOR_NAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_REQUIRED_FIELD_LEGEND)->ShowWindow(FALSE);
    }
    else
    {
        SetAccessibleFullDescription(GetDlgItem(IDC_COLOR_NAME)->GetSafeHwnd(), L"Required");
        SetAccessibleDescription(GetDlgItem(IDC_COLOR_NAME)->GetSafeHwnd(), L"Required");
    }

    /* Int palette or grayscale mode rgb color can not be edit. */
    if ((mPalette.palette) || (mpProject->mDisplays[mDisplayIndex].colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE))
    {
        mRedSlider.EnableWindow(FALSE);
        mGreenSlider.EnableWindow(FALSE);
        mBlueSlider.EnableWindow(FALSE);
        mHueSlider.EnableWindow(FALSE);
        mSaturationSlider.EnableWindow(FALSE);
        mLuminanceSlider.EnableWindow(FALSE);

        mRedEdit.EnableWindow(FALSE);
        mGreenEdit.EnableWindow(FALSE);
        mBlueEdit.EnableWindow(FALSE);
        mHueEdit.EnableWindow(FALSE);
        mSaturationEdit.EnableWindow(FALSE);
        mLuminanceEdit.EnableWindow(FALSE);
    }

    if (!IsAlphaFormat(mpProject->mDisplays[mDisplayIndex].colorformat))
    {
        mAlphaSlider.EnableWindow(FALSE);
        mAlphaEdit.EnableWindow(FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL color_edit_dialog::IsAlphaFormat(int color_format)
{
    if (color_format == GX_COLOR_FORMAT_32ARGB ||
        color_format == GX_COLOR_FORMAT_32BGRA ||
        color_format == GX_COLOR_FORMAT_4444ARGB ||
        color_format == GX_COLOR_FORMAT_4444BGRA)
    {
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::AnnounceColorUpdate(CString msg)
{
    CMainFrame *pMain = (CMainFrame*) AfxGetApp()->GetMainWnd();

    if (pMain)
    {
        pMain->GetStatusMsgControl()->SetWindowText(msg);
        pMain->GetStatusMsgControl()->NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);

        mColorUpdateMsg.SetWindowText(msg);
        mColorUpdateMsg.NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::PaintColorPreview()
{
    COLORREF wincolor;
    GX_DISPLAY *display = get_target_view_display();

    if (display->gx_display_color_format == GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        int colorIndex = GetColor();
        GX_COLOR color = 0;

        if (mPalette.palette && colorIndex < mPalette.total_size)
        {
            color = mPalette.palette[GetColor()];
        }

        wincolor = resource_item::MakeColorRef(color);
    }
    else
    {
        wincolor = resource_item::MakeColorRef(GetColor());
    }

    CRect boxrect;
    CBrush boxbrush(wincolor);
    CPen newpen(PS_SOLID, 0, wincolor);
    CBrush *oldbrush;
    CPen *oldpen;
    CDC *dc = GetDC();
    CRect color_name_size;
    CRect color_preview_size;

    //Get rectangle box of color name edit control
    GetDlgItem(IDC_COLOR_NAME)->GetWindowRect(&color_name_size);
    ScreenToClient(&color_name_size);

    GetDlgItem(IDC_COLOR_PREVIEW_LABEL)->GetWindowRect(&color_preview_size);
    ScreenToClient(&color_preview_size);

    //Make the preview box align with the top of color name edit control
    GetClientRect(&boxrect);
    oldbrush = dc->SelectObject(&boxbrush);
    oldpen = dc->SelectObject(&newpen);
    dc->Rectangle(color_preview_size.left, color_name_size.top, boxrect.right - 10, color_name_size.bottom);
    dc->SelectObject(oldbrush);
    dc->SelectObject(oldpen);
    ReleaseDC(dc);

}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnPaint()
{
    express_dialog::OnPaint();

    if (mResId >= GX_MAX_DEFAULT_COLORS)
    {
        PaintRequiredAsterisk(IDC_COLOR_NAME);
        PaintRequiredAsterisk(IDC_REQUIRED_FIELD_LEGEND);
    }

    PaintColorPreview();
}


///////////////////////////////////////////////////////////////////////////////
CString color_edit_dialog::GetName()
{
    return mName;
}

///////////////////////////////////////////////////////////////////////////////
GX_COLOR color_edit_dialog::GetColor()
{
    GX_COLOR color = (mAlpha << 24) | (mRed << 16) | (mGreen << 8) | mBlue;

    GX_DISPLAY *display = get_target_view_display();

    if (display->gx_display_color_format == GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        color = mColorIndex;
    }

    return color;
}

// color_edit_dialog message handlers

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditRed()
{
    int val = GetDlgItemInt(IDC_EDIT_RED);
    if (val >= 0 && val <= 255)
    {
        mRed = val;
        mRedSlider.SetPos(mRed);
        UpdateHSLFields();
        PaintColorPreview();   
    }
    else
    {
        Notify("Invalid value for red. Valid range is from 0 to 255.", this);
        SetDlgItemInt(IDC_EDIT_RED, mRed);
    }   
}


///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditGreen()
{
    int val = GetDlgItemInt(IDC_EDIT_GREEN);
    if (val >= 0 && val <= 255)
    {
        mGreen = val;
        mGreenSlider.SetPos(mGreen);
        UpdateHSLFields();
        PaintColorPreview();      
    }
    else
    {
        Notify("Invalid value for green. Valid range is from 0 to 255.", this);
        SetDlgItemInt(IDC_EDIT_GREEN, mGreen);
    }
}


///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditBlue()
{
    int val = GetDlgItemInt(IDC_EDIT_BLUE);
    if (val >= 0 && val <= 255)
    {
        mBlue = val;
        mBlueSlider.SetPos(mBlue);
        UpdateHSLFields();
        PaintColorPreview();    
    }
    else
    {
        Notify("Invalid value for blue. Valid range is from 0 to 255.", this);
        SetDlgItemInt(IDC_EDIT_BLUE, mBlue);
    }  
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditAlpha()
{
    int val = GetDlgItemInt(IDC_EDIT_ALPHA);

    if (val >= 0 && val <= 255)
    {
        mAlpha = val;
        mAlphaSlider.SetPos(mAlpha);
    }
    else
    {
        Notify("Invalid value for alpha. Valid range is from 0 to 255.", this);
        SetDlgItemInt(IDC_EDIT_ALPHA, mAlpha);
    }  
}


///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditHue()
{
    int val = GetDlgItemInt(IDC_EDIT_HUE);
    
    if (val >= 0 && val <= 360)
    {
        mHue = val;
        mHueSlider.SetPos(val);
        UpdateRGBVals();
        UpdateRGBFields();
        mpColorGradientWin->Invalidate();
    }
    else
    {
        Notify("Invalid value for hue. Valid range is from 0 to 360.", this);
        SetDlgItemInt(IDC_EDIT_HUE, mHueSlider.GetPos());
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditLuminance()
{
    int val = GetDlgItemInt(IDC_EDIT_LUMINANCE);
    
    if (val >= 0 && val <= 100)
    {
        mLum = val;
        mLuminanceSlider.SetPos(val);
        UpdateRGBVals();
        UpdateRGBFields();
        mpColorGradientWin->Invalidate();
    }
    else
    {
        Notify("Invalid value for luminance. Valid range is from 0 to 100.", this);
        SetDlgItemInt(IDC_EDIT_LUMINANCE, mLuminanceSlider.GetPos());
    }
}


///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnEnChangeEditSaturation()
{
    int val = GetDlgItemInt(IDC_EDIT_SATURATION);
    
    if (val >= 0 && val <= 100)
    {
        mSat = val;
        mSaturationSlider.SetPos(val);
        UpdateRGBVals();
        UpdateRGBFields();
        mpColorGradientWin->Invalidate();
    }
    else
    {
        Notify("Invalid value for saturation. Valid range is from 0 to 255.", this);
        SetDlgItemInt(IDC_EDIT_SATURATION, mSaturationSlider.GetPos());
    }

}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *bar)
{
    if (nSBCode == SB_THUMBPOSITION ||
        nSBCode == SB_THUMBTRACK ||
        nSBCode == SB_PAGELEFT ||
        nSBCode == SB_PAGERIGHT ||
        nSBCode == SB_LINELEFT ||
        nSBCode == SB_LINERIGHT)
    {
        int ctrl_id = bar->GetDlgCtrlID();
        int curpos = ((CSliderCtrl *)GetDlgItem(ctrl_id))->GetPos();

        nPos = curpos;

        switch (ctrl_id)
        {
        case IDC_RED_SLIDER:
            mRed = nPos;
            SetDlgItemInt(IDC_EDIT_RED, mRed);
            UpdateHSLFields();
            PaintColorPreview();
            mpColorGradientWin->Invalidate();
            break;

        case IDC_GREEN_SLIDER:
            mGreen = nPos;
            SetDlgItemInt(IDC_EDIT_GREEN, mGreen);
            UpdateHSLFields();
            PaintColorPreview();
            mpColorGradientWin->Invalidate();
            break;

        case IDC_BLUE_SLIDER:
            mBlue = nPos;
            SetDlgItemInt(IDC_EDIT_BLUE, mBlue);
            UpdateHSLFields();
            PaintColorPreview();
            mpColorGradientWin->Invalidate();
            break;

        case IDC_ALPHA_SLIDER:
            mAlpha = nPos;
            SetDlgItemInt(IDC_EDIT_ALPHA, mAlpha);
            break;

        case IDC_HUE_SLIDER:
            mHue = nPos;
            SetDlgItemInt(IDC_EDIT_HUE, nPos);
            UpdateRGBVals();
            UpdateRGBFields();
            mpColorGradientWin->Invalidate();
            break;

        case IDC_SAT_SLIDER:
            mSat = nPos;
            SetDlgItemInt(IDC_EDIT_SATURATION, nPos);
            UpdateRGBVals();
            UpdateRGBFields();
            mpColorGradientWin->Invalidate();
            break;

        case IDC_LUM_SLIDER:
            mLum = nPos;
            SetDlgItemInt(IDC_EDIT_LUMINANCE, nPos);
            UpdateRGBVals();
            UpdateRGBFields();
            mpColorGradientWin->Invalidate();
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::UpdateHSLFields()
{
    RgbToHsl(mRed, mGreen, mBlue, mHue, mSat, mLum);

    mHueSlider.SetPos(mHue);
    mSaturationSlider.SetPos(mSat);
    mLuminanceSlider.SetPos(mLum);

    SetDlgItemInt(IDC_EDIT_HUE, mHue);
    SetDlgItemInt(IDC_EDIT_SATURATION, mSat);
    SetDlgItemInt(IDC_EDIT_LUMINANCE, mLum);

    CString msg;
    msg.Format(L"hue %d, saturation: %d, luminance: %d", mHue, mSat, mLum);
    AnnounceColorUpdate(msg);

    if (!mPalette.palette && mpColorGradientWin)
    {
        mpColorGradientWin->Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::UpdateRGBVals()
{
    int hue = GetDlgItemInt(IDC_EDIT_HUE);
    int sat = GetDlgItemInt(IDC_EDIT_SATURATION);
    int lum = GetDlgItemInt(IDC_EDIT_LUMINANCE);

    double huefloat = hue;
    huefloat /= 360.0;

    double satfloat = sat;
    satfloat /= 100.0;

    double lumfloat = lum;
    lumfloat /= 100.0;

    HslToRgb(huefloat, satfloat, lumfloat, mRed, mGreen, mBlue);
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::UpdateRGBFields()
{
    SetDlgItemInt(IDC_EDIT_RED, mRed);
    mRedSlider.SetPos(mRed);
    
    SetDlgItemInt(IDC_EDIT_GREEN, mGreen);
    mGreenSlider.SetPos(mGreen);

    SetDlgItemInt(IDC_EDIT_BLUE, mBlue);
    mBlueSlider.SetPos(mBlue);

    CString msg;
    msg.Format(L"red %d, green: %d, blue: %d", mRed, mGreen, mBlue);
    AnnounceColorUpdate(msg);

    PaintColorPreview();
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::HslToRgb(double hue, double sat, double lum,
    int &red, int &green, int &blue)
{
    double C = (1 - abs(2 * lum - 1)) * sat;
    double H = hue * 6;
    double X = C * (1 - abs((H - (int(H / 2)) * 2) - 1));
    double m = lum - C / 2;
    double red1 = 0, green1 = 0, blue1 = 0;

    if (H >= 6)
    {
        H = 0;
    }

    if (H < 1)
    {
        red1 = C;
        green1 = X;
    }
    else if (H < 2)
    {
        red1 = X;
        green1 = C;
    }
    else if (H < 3)
    {
        green1 = C;
        blue1 = X;
    }
    else if (H < 4)
    {
        green1 = X;
        blue1 = C;
    }
    else if (H < 5)
    {
        red1 = X;
        blue1 = C;
    }
    else
    {
        red1 = C;
        blue1 = X;
    }

    red1 += m;
    green1 += m;
    blue1 += m;
    
    red = int(red1 * 255);
    green = int(green1 * 255);
    blue = int(blue1 * 255);
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::RgbToHsl(int red, int green, int blue, 
    int &hue, int &saturation, int &luminance)
{
    double redfloat = red;
    double greenfloat = green;
    double bluefloat = blue;

    double huefloat = 0.0;
    double satfloat = 0.0;
    double lumfloat = 0.0;

    redfloat /= 255.0;
    greenfloat /= 255.0;
    bluefloat /= 255.0;

    double min = redfloat;
    if (greenfloat < min)
    {
        min = greenfloat;
    }
    if (bluefloat < min)
    {
        min = bluefloat;
    }

    double max = redfloat;
    int maxchannel = 0;

    if (max < greenfloat)
    {
        max = greenfloat;
        maxchannel = 1;
    }
    if (max < bluefloat)
    {
        max = bluefloat;
        maxchannel = 2;
    }
    
    lumfloat = (min + max) / 2.0;

    if (min != max)
    {
        if (lumfloat < 0.5)
        {
            satfloat = (max - min) / (max + min);
        }
        else
        {
            satfloat = (max - min) / (2.0 - max - min);
        }

        switch(maxchannel)
        {
        case 0: // red is our max
            huefloat = (greenfloat - bluefloat) / (max - min);
            break;

        case 1: // green is our max
            huefloat = 2.0 + (bluefloat - redfloat) / (max - min);
            break;

        case 2: // blue is our max
            huefloat = 4.0 + (redfloat - greenfloat) / (max - min);
            break;
        }
    }
    
    // convert values to int
    huefloat *= 60;
    if (huefloat < 0)
    {
        huefloat += 360.0;
    }
    hue = (int) (huefloat + 0.5);       // 0 to 360

    satfloat *= 100;
    saturation = (int) (satfloat + 0.5); // percentage

    lumfloat *= 100;
    luminance = (int) (lumfloat + 0.5); // percentage
    
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::SetPaletteColor(int index)
{
    mColorIndex = index;

    if (mPalette.palette)
    {
        GX_COLOR colorval = mPalette.palette[index];

        mRed = (colorval >> 16) & 0xff;
        mGreen = (colorval >> 8) & 0xff;
        mBlue = colorval & 0xff;

        UpdateRGBFields();
        UpdateHSLFields();
        PaintColorPreview();
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::GetHSLColor(int *hue, int *sat, int *lum)
{
    if (hue)
    {
        (*hue) = mHue;
    }

    if (sat)
    {
        (*sat) = mSat;
    }

    if (lum)
    {
        (*lum) = mLum;
    }
}

///////////////////////////////////////////////////////////////////////////////
void color_edit_dialog::SetHSLColor(int hue, int sat)
{
    mSat = sat;
    mHue = hue;

    SetDlgItemInt(IDC_EDIT_SATURATION, mSat);
    SetDlgItemInt(IDC_EDIT_HUE, mHue);
    mHueSlider.SetPos(mHue);
    mSaturationSlider.SetPos(mSat);
    UpdateRGBVals();
    UpdateRGBFields();
}

///////////////////////////////////////////////////////////////////////////////
BOOL color_edit_dialog::PreTranslateMessage(MSG *pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    CWnd *focus_owner = GetFocus();

    if (focus_owner)
    {
        int ctrl_id = focus_owner->GetDlgCtrlID();

        switch (ctrl_id)
        {
        case IDC_EDIT_RED:
        case IDC_EDIT_GREEN:
        case IDC_EDIT_BLUE:
        case IDC_EDIT_ALPHA:
        case IDC_EDIT_HUE:
        case IDC_EDIT_LUMINANCE:
        case IDC_EDIT_SATURATION:
            if ((pMsg->message == WM_CHAR) && isprint(pMsg->wParam))
            {
                if (pMsg->wParam < '0' || pMsg->wParam > '9')
                {
                    ErrorMsg("Unacceptable Character. You can only type a number here.", this);
                    return TRUE;
                }
            }
            break;
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT color_edit_dialog::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;
    CRect boxrect;
    CStringArray params;

    switch (wParam)
    {
    case TEST_SET_COLOR_NAME:
        SetDlgItemText(IDC_COLOR_NAME, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_COLOR_NAME);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COLOR_NAME, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_COLOR_RED:
        SetDlgItemText(IDC_EDIT_RED, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_EDIT_RED);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_RED, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_COLOR_GREEN:
        SetDlgItemText(IDC_EDIT_GREEN, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_EDIT_GREEN);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_GREEN, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_COLOR_BLUE:
        SetDlgItemText(IDC_EDIT_BLUE, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_EDIT_BLUE);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_BLUE, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_COLOR_HUE:
        SetDlgItemText(IDC_EDIT_HUE, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_EDIT_HUE);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_HUE, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_COLOR_SAT:
        SetDlgItemText(IDC_EDIT_SATURATION, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_EDIT_SATURATION);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_SATURATION, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_COLOR_LUM:
        SetDlgItemText(IDC_EDIT_LUMINANCE, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_EDIT_LUMINANCE);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_LUMINANCE, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_PALETTE_COLOR_INDEX:
        mColorIndex = lParam;
        mRed = (mPalette.palette[mColorIndex] >> 16) & 0xff;
        mGreen = (mPalette.palette[mColorIndex] >> 8) & 0xff;
        mBlue = mPalette.palette[mColorIndex] & 0xff;
        UpdateRGBFields();
        UpdateHSLFields();
        PaintColorPreview();
        break;

    case TEST_SAVE_COLOR_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_COLOR_EDIT:
        OnCancel();
        break;
    }

    return 0;
}
