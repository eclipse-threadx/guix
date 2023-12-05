// rich_text_edit_dlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "rich_text_edit_dlg.h"
#include "color_edit_dialog.h"

#define MENU_BAR_HEIGHT_96DPI 29
#define LABLE_WIDTH_96DPI     60
#define COLOR_BUTTON_WIDTH_96DPI 30
#define MIN_RICH_TEXT_EDIT_DLG_WIDTH_96DPI 800

extern CString target_class_name;
extern CFont NormalFont;

extern "C" {
    #include "gx_multi_line_text_view.h"
    #include "gx_rich_text_view.h"
    #include "gx_utility.h"
    extern GX_DISPLAY  target_win_display;
    extern VOID *_win32_canvas_memory_prepare(GX_CANVAS *canvas, GX_RECTANGLE *dirty);
}

enum rich_text_edit_dlg_test_commands {
    TEST_SET_STRING = 1,
    TEST_SAVE,
    TEST_CANCEL
};

#define DEFINE_GUIDXXX(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID CDECL name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
DEFINE_GUIDXXX(IID_ITextDocument, 0x8CC497C0, 0xA1DF, 0x11CE, 0x80, 0x98,
    0x00, 0xAA, 0x00, 0x47, 0xBE, 0x5D);

enum button_ids {
    ID_BOLD_BUTTON = 1024,
    ID_ITALIC_BUTTON,
    ID_UNDERLINE_BUTTON,
    ID_FONT_LABLE,
    ID_FONT_COMBO,
    ID_HIGHLIGHT_COLOR_LABLE,
    ID_HIGHLIGHT_COLOR_COMBO,
    ID_HIGHLIGHT_COLOR_BUTTON,
    ID_TEXT_COLOR_LABLE,
    ID_TEXT_COLOR_COMBO,
    ID_TEXT_COLOR_BUTTON,
    ID_LEFT_ALIGN_BUTTON,
    ID_CENTER_ALIGN_BUTTON,
    ID_RIGHT_ALIGN_BUTTON,
    ID_RICH_EDIT
};

BEGIN_MESSAGE_MAP(custom_rich_edit, CRichEditCtrl)
    ON_WM_KILLFOCUS()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
custom_rich_edit::custom_rich_edit()
{
    mSelStart = 0;
    mSelEnd = 0;
}

///////////////////////////////////////////////////////////////////////////////
void custom_rich_edit::OnSetFocus(CWnd* pOldWnd)
{
    CRichEditCtrl::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    HideSelection(FALSE, TRUE);
    SetSel(mSelStart, mSelEnd);
}

///////////////////////////////////////////////////////////////////////////////
void custom_rich_edit::OnKillFocus(CWnd* pNewWnd)
{
    CRichEditCtrl::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
    HideSelection(FALSE, TRUE);

    GetSel(mSelStart, mSelEnd);
}

BEGIN_MESSAGE_MAP(text_edit_button_frame, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(ID_BOLD_BUTTON, OnClickBold)
    ON_BN_CLICKED(ID_ITALIC_BUTTON, OnClickItalic)
    ON_BN_CLICKED(ID_UNDERLINE_BUTTON, OnClickUnderline)
    ON_BN_CLICKED(ID_LEFT_ALIGN_BUTTON, OnClickLeftAlign)
    ON_BN_CLICKED(ID_CENTER_ALIGN_BUTTON, OnClickCenterAlign)
    ON_BN_CLICKED(ID_RIGHT_ALIGN_BUTTON, OnClickRightAlign)
    ON_BN_CLICKED(ID_HIGHLIGHT_COLOR_BUTTON, OnClickHighlightColor)
    ON_BN_CLICKED(ID_TEXT_COLOR_BUTTON, OnClickTextColor)
    ON_CBN_SELCHANGE(ID_TEXT_COLOR_COMBO, OnChangeTextColor)
    ON_CBN_SELCHANGE(ID_FONT_COMBO, OnChangeFont)
    ON_CBN_SELCHANGE(ID_HIGHLIGHT_COLOR_COMBO, OnChangeHighlightColor)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
text_edit_button_frame::text_edit_button_frame(widget_info *info, CWnd *pParent)
{
    mpInfo = info;
    mHighlightColorName = "";
    mTextColorName="";
    mFontName = "";

    int dpi = GetSystemDPI();

    mLableWidth = MulDiv(LABLE_WIDTH_96DPI, dpi, DEFAULT_DPI_96);
    mColorButtonWidth = MulDiv(COLOR_BUTTON_WIDTH_96DPI, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
text_edit_button_frame::~text_edit_button_frame()
{

}

///////////////////////////////////////////////////////////////////////////////
int text_edit_button_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CFont* font = &NormalFont;

    mBtnBold.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, ID_BOLD_BUTTON);
    mBtnBold.LoadBitmaps(IDB_BOLD, IDB_BOLD);
    SetControlAccessibleName(mBtnBold.GetSafeHwnd(), _T("Bold"));

    mBtnItalic.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, ID_ITALIC_BUTTON);
    mBtnItalic.LoadBitmaps(IDB_ITALIC, IDB_ITALIC);
    SetControlAccessibleName(mBtnItalic.GetSafeHwnd(), _T("Italic"));

    mBtnUnderline.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, ID_UNDERLINE_BUTTON);
    mBtnUnderline.LoadBitmaps(IDB_UNDERLINE, IDB_UNDERLINE);
    SetControlAccessibleName(mBtnUnderline.GetSafeHwnd(), _T("Underline"));

    mBtnLeftAlign.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, ID_LEFT_ALIGN_BUTTON);
    mBtnLeftAlign.LoadBitmaps(IDB_LEFTALIGN, IDB_LEFTALIGN);
    SetControlAccessibleName(mBtnLeftAlign.GetSafeHwnd(), _T("Left Align"));

    mBtnCenterAlign.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, ID_CENTER_ALIGN_BUTTON);
    mBtnCenterAlign.LoadBitmaps(IDB_CENTERALIGN, IDB_CENTERALIGN);
    SetControlAccessibleName(mBtnItalic.GetSafeHwnd(), _T("Center Align"));

    mBtnRightAlign.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, ID_RIGHT_ALIGN_BUTTON);
    mBtnRightAlign.LoadBitmaps(IDB_RIGHTALIGN, IDB_RIGHTALIGN);
    SetControlAccessibleName(mBtnUnderline.GetSafeHwnd(), _T("Right Align"));

    mFontBox.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_SORT, CRect(0, 0, 0, 0), this, ID_FONT_COMBO);
    mFontBox.SetFont(font);

    mHighlightColorBox.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_SORT, CRect(0, 0, 0, 0), this, ID_HIGHLIGHT_COLOR_COMBO);
    mHighlightColorBox.SetFont(font);

    mHighlightColorButton.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, ID_HIGHLIGHT_COLOR_BUTTON);

    mTextColorBox.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_SORT, CRect(0, 0, 0, 0), this, ID_TEXT_COLOR_COMBO);
    mTextColorBox.SetFont(font);

    mFontLable.Create(_T("Font:"), WS_VISIBLE | WS_CHILD | SS_RIGHT, CRect(0, 0, 0, 0), this, ID_FONT_LABLE);
    mFontLable.SetFont(font);

    mHighlightColorLable.Create(_T("Highlight:"), WS_VISIBLE | WS_CHILD | SS_RIGHT, CRect(0, 0, 0, 0), this, ID_HIGHLIGHT_COLOR_LABLE);
    mHighlightColorLable.SetFont(font);

    mTextColorLable.Create(_T("Text Color:"), WS_VISIBLE | WS_CHILD | SS_RIGHT, CRect(0, 0, 0, 0), this, ID_TEXT_COLOR_LABLE);
    mTextColorLable.SetFont(font);

    mTextColorButton.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, ID_TEXT_COLOR_BUTTON);

    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();
    COLORREF color = dlg->MakeColorRef(mpInfo->color_id[NORMAL_FILL_COLOR_INDEX]);
    mHighlightColorButton.SetBoxColor(color);
    color = dlg->MakeColorRef(mpInfo->color_id[NORMAL_TEXT_COLOR_INDEX]);
    mTextColorButton.SetBoxColor(color);
    
    mFontName = InitResList(&mFontBox, RES_TYPE_FONT, mpInfo->font_id[NORMAL_FONT_INDEX]);
    mHighlightColorName = InitResList(&mHighlightColorBox, RES_TYPE_COLOR, mpInfo->color_id[NORMAL_FILL_COLOR_INDEX]);
    mTextColorName = InitResList(&mTextColorBox, RES_TYPE_COLOR, mpInfo->color_id[NORMAL_TEXT_COLOR_INDEX]);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    CRect size;
    CRect client;
    GetClientRect(&client);

    CBitmap icon;
    BITMAP bmp;
    icon.LoadBitmap(IDB_BOLD);
    icon.GetObject(sizeof(BITMAP), &bmp);

    //Position bold button
    size.top = (client.Height() - bmp.bmHeight) >> 1;
    size.bottom = size.top + bmp.bmHeight;
    size.left = client.left + 10;
    size.right = size.left + bmp.bmWidth;

    mBtnBold.MoveWindow(size);

    //Position italic button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mBtnItalic.MoveWindow(size);

    //Position underline button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mBtnUnderline.MoveWindow(size);

    //Position left align button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mBtnLeftAlign.MoveWindow(size);

    //Position center align button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mBtnCenterAlign.MoveWindow(size);

    //Position right align button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mBtnRightAlign.MoveWindow(size);

    int width = (client.right - size.left - 10) / 3 - mLableWidth - mColorButtonWidth;

    size.right = size.left + mLableWidth;
    mFontLable.MoveWindow(&size);

    //position font combo box
    size.left = size.right + 5;
    size.right = size.left + width;
    mFontBox.MoveWindow(&size);

    size.left = size.right + 5;
    size.right = size.left + mLableWidth;
    mHighlightColorLable.MoveWindow(&size);

    //position highlight color combo box
    size.left = size.right + 5;
    size.right = size.left + width;
    mHighlightColorBox.MoveWindow(&size);

    size.left = size.right + 5;
    size.right = size.left + mColorButtonWidth;
    mHighlightColorButton.MoveWindow(&size);

    size.left = size.right + 5;
    size.right = size.left + mLableWidth;
    mTextColorLable.MoveWindow(&size);

    //position text color box
    size.left = size.right + 5;
    size.right = size.left + width;
    mTextColorBox.MoveWindow(&size);

    size.left = size.right + 5;
    size.right = size.left + mColorButtonWidth;
    mTextColorButton.MoveWindow(&size);
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickBold()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedBold();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickItalic()
{
    rich_text_edit_dlg *dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedItalic();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickUnderline()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedUnderline();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickLeftAlign()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedLeftAlign();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickCenterAlign()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedCenterAlign();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickRightAlign()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedRightAlign();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickHighlightColor()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedHighlightColor(mHighlightColorName);

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnClickTextColor()
{
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();

    dlg->SetSelectedTextColor(mTextColorName);

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnChangeFont()
{
    int box_index = mFontBox.GetCurSel();
    mFontBox.GetLBText(box_index, mFontName);
    
    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();
    dlg->SetSelectedFont(mFontName);
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnChangeTextColor()
{
    int box_index = mTextColorBox.GetCurSel();
    int color_id;
    mTextColorBox.GetLBText(box_index, mTextColorName);
    color_id = mTextColorBox.GetItemData(box_index);


    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();
    dlg->SetSelectedTextColor(mTextColorName);

    COLORREF color = dlg->MakeColorRef(color_id);

    mTextColorButton.SetBoxColor(color);
    mTextColorButton.Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void text_edit_button_frame::OnChangeHighlightColor()
{
    int box_index = mHighlightColorBox.GetCurSel();
    int color_id;
    mHighlightColorBox.GetLBText(box_index, mHighlightColorName);
    color_id = mHighlightColorBox.GetItemData(box_index);

    rich_text_edit_dlg* dlg = (rich_text_edit_dlg*)GetParent();
    dlg->SetSelectedHighlightColor(mHighlightColorName);

    COLORREF color = dlg->MakeColorRef(color_id);

    mHighlightColorButton.SetBoxColor(color);
    mHighlightColorButton.Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
BOOL text_edit_button_frame::OnEraseBkgnd(CDC* pDC)
{
    CRect size;

    CBrush grayBrush(GetSysColor(COLOR_BTNFACE));
    CBrush* oldBrush = pDC->SelectObject(&grayBrush);

    GetClientRect(&size);
    pDC->Rectangle(size);
    pDC->SelectObject(oldBrush);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
HBRUSH text_edit_button_frame::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT);

        // Set background brush
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

///////////////////////////////////////////////////////////////////////////////
CString text_edit_button_frame::InitResList(CComboBox *box, int res_type, int sel_res_id)
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();
    CString select_string("");

    if (!project || !pview)
    {
        return select_string;
    }

    int active_display = pview->GetActiveDisplay();
    int res_id;
    int start_id = 0;
    int end_id = project->CountResources(active_display, res_type);
    int active_theme = project->mDisplays[active_display].active_theme;
    int list_index;
    CString name;

    for (res_id = start_id; res_id < end_id; res_id++)
    {
        res_info *info = project->FindResource(active_display, active_theme, res_type, res_id);

        if (info)
        {
            list_index = box->AddString(info->name);
            box->SetItemData(list_index, res_id);

            if (res_id == sel_res_id)
            {
                select_string = info->name;
                box->SetCurSel(list_index);
            }
        }
    }

    return select_string;
}

BEGIN_MESSAGE_MAP(rich_text_preview_win, CWnd)
    ON_WM_CREATE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
rich_text_preview_win::rich_text_preview_win(widget_info *info, CWnd *parent)
{
    mpDisplay = NULL;
    mpWidget = NULL;
    mpCanvas = NULL;
    mpRoot = NULL;
    mpMem = NULL;
    mpInfo = info;

    m_scroll_helper = new CScrollHelper();
    m_scroll_helper->AttachWnd(this);

    CreateDisplay();
    MakeWidgetPreview();
}

///////////////////////////////////////////////////////////////////////////////
rich_text_preview_win::~rich_text_preview_win()
{
    if (m_scroll_helper)
    {
        delete m_scroll_helper;
    }

    if (mpDisplay)
    {
        resource_view::CleanupDisplayResources(mpDisplay);
        guix_studio_delete_display(mpDisplay);
        delete mpDisplay;
    }

    if (mpWidget)
    {
        DeleteWidgetString();
        widget_factory::DeleteWidgets(mpWidget);
    }

    if (mpRoot)
    {
        gx_window_root_delete(mpRoot);
        delete mpRoot;
    }

    if (mpCanvas)
    {
        if (mpCanvas->gx_canvas_memory)
        {
            delete mpCanvas->gx_canvas_memory;
        }
        gx_canvas_delete(mpCanvas);
        delete mpCanvas;
    }
}

///////////////////////////////////////////////////////////////////////////////
int rich_text_preview_win::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

void rich_text_preview_win::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    GX_VALUE width, height;

    gx_widget_width_get(mpInfo->widget, &width);
    gx_widget_height_get(mpInfo->widget, &height);

    m_scroll_helper->SetDisplaySize(width, height);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    m_scroll_helper->OnHScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::CreateDisplay()
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();

    if (!project || !pview)
    {
        return;
    }

    if (!mpDisplay)
    {
        // create a temporary screen and temporary
        // canvas to make a bitmap:
        GX_CANVAS *target_canvas = get_target_win_canvas();

        int canvas_width = target_canvas->gx_canvas_x_resolution;
        int canvas_height = target_canvas->gx_canvas_y_resolution;

        int active_display = pview->GetActiveDisplay();
        int active_theme = project->mDisplays[active_display].active_theme;
        int memsize = 0;

        display_info *info = &project->mDisplays[active_display];

        mpDisplay = new GX_DISPLAY;

        guix_create_app_display(mpDisplay, "text_preview_display",
            canvas_width, canvas_height,
            info->colorformat,
            project->mHeader.target_cpu,
            IsRenesasDave2D(project),
            IsDave2dFontFormat(project, active_display),
            info->themes[active_theme].palette,
            info->themes[active_theme].palette_total_size,
            project->mHeader.palette_mode_aa_text_colors);

        GetResourceView()->BuildResourceTables(active_display, mpDisplay, FALSE);
    }
}


///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::MakeWidgetPreview()
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();

    if (!project || !pview)
    {
        return;
    }

    int active_display = pview->GetActiveDisplay();

    if (!mpWidget)
    {
        int widget_width = mpInfo->size.gx_rectangle_right - mpInfo->size.gx_rectangle_left + 1;
        int widget_height = mpInfo->size.gx_rectangle_bottom - mpInfo->size.gx_rectangle_top + 1;
        GX_RECTANGLE fill_rect;
        gx_utility_rectangle_define(&fill_rect, 0, 0, widget_width - 1, widget_height - 1);

        int bpp = project->mDisplays[active_display].bits_per_pix;
        switch (bpp)
        {
        case 24:
            bpp = 32;
            break;
        }

        //calculate memory size
        int  memsize = widget_width * widget_height * bpp;
        memsize >>= 3;

        UCHAR *mem = new UCHAR[memsize];
        mpCanvas = new GX_CANVAS;
        mpRoot = new GX_WINDOW_ROOT;

        //create canvas and root window for widget drawing
        gx_canvas_create(mpCanvas, "string_test_canvas",
            mpDisplay, GX_CANVAS_SIMPLE,
            widget_width, widget_height, (GX_COLOR *)mem, memsize);

        gx_window_root_create(mpRoot, "string_test_root", mpCanvas,
            GX_STYLE_BORDER_NONE, GX_ID_NONE, &fill_rect);

        mpWidget = widget_factory::GenerateAppScreen(NULL, mpInfo);

        if (mpWidget)
        {
            gx_widget_attach(mpRoot, mpWidget);

            int xpos = mpInfo->size.gx_rectangle_left;
            int ypos = mpInfo->size.gx_rectangle_top;

            if (xpos || ypos)
            {
                gx_widget_shift(mpWidget, -xpos, -ypos, FALSE);
            }

            GX_WIN32_DISPLAY_DRIVER_DATA *data = (GX_WIN32_DISPLAY_DRIVER_DATA *)target_win_display.gx_display_driver_data;
            m_bitmap = data->win32_driver_bmpinfo;
            m_bitmap.gx_bmp_header.biWidth = widget_width;
            m_bitmap.gx_bmp_header.biHeight = widget_height;
        }
    }

    if (mpWidget)
    {
        //Draw widget
        gx_canvas_drawing_initiate(mpCanvas, NULL, &mpWidget->gx_widget_size);

        //Fill background with gray color.
        GX_BRUSH* brush;
        gx_context_brush_get(&brush);

        int old_brush_width = brush->gx_brush_width;
        int old_brush_style = brush->gx_brush_style;
        brush->gx_brush_style |= GX_BRUSH_SOLID_FILL;
        brush->gx_brush_width = 0;
        gx_context_raw_fill_color_set(GX_COLOR_LIGHTGRAY);
        gx_canvas_rectangle_draw(&mpWidget->gx_widget_size);

        brush->gx_brush_width = old_brush_width;
        brush->gx_brush_style = old_brush_style;

        gx_widget_show(mpWidget);
        mpWidget->gx_widget_draw_function(mpWidget);
        gx_widget_hide(mpWidget);

        gx_canvas_drawing_complete(mpCanvas, FALSE);

        mpMem = (UCHAR *)_win32_canvas_memory_prepare(mpCanvas, &mpWidget->gx_widget_size);
    }
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::DeleteWidgetString()
{
    if (mpWidget)
    {
        GX_RICH_TEXT_VIEW* view = (GX_RICH_TEXT_VIEW*)mpWidget;
        if (view->gx_multi_line_text_view_text.gx_string_ptr)
        {
            delete view->gx_multi_line_text_view_text.gx_string_ptr;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::UpdateRichText(CString &richtext)
{
    if (mpWidget)
    {
        GX_STRING string;

        int max_length = richtext.GetLength() * 6 + 1;
        string.gx_string_ptr = new char[max_length];
        strcpy_s((char*)string.gx_string_ptr, max_length, CT2A(richtext.GetString(), CP_UTF8));
        string.gx_string_length = strnlen_s(string.gx_string_ptr, max_length);

        DeleteWidgetString();
        gx_multi_line_text_view_text_set_ext((GX_MULTI_LINE_TEXT_VIEW*)mpWidget, &string);

        MakeWidgetPreview();

        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_preview_win::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages

    if (mpInfo && mpCanvas && mpMem)
    {
        CSize scrollpos = m_scroll_helper->GetScrollPos();
        GX_VALUE width;

        gx_widget_width_get(mpWidget, &width);

        guix_bitmap_flush(dc.GetSafeHdc(), mpWidget->gx_widget_size.gx_rectangle_left - scrollpos.cx,
            mpWidget->gx_widget_size.gx_rectangle_top - scrollpos.cy, width, mpMem, &m_bitmap);
    }
}

// rich_text_edit_dlg dialog

IMPLEMENT_DYNAMIC(rich_text_edit_dlg, express_dialog)

BEGIN_MESSAGE_MAP(rich_text_edit_dlg, express_dialog)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_CONTROL(EN_CHANGE, ID_RICH_EDIT, OnChangeRichText)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
rich_text_edit_dlg::rich_text_edit_dlg(widget_info *info, CWnd* pParent /*=NULL*/)
    : express_dialog(rich_text_edit_dlg::IDD, pParent)
{
    IconId = IDB_STRINGS;
    SetTitleText("Edit Rich Text");

    mpButtonFrame = NULL;
    mpPreview = NULL;
    mpInfo = info;

    mpProject = GetOpenProject();
    mDisplay = mpProject->GetDisplayIndex(mpInfo);

    int active_theme;
    switch (mpProject->mDisplays[mDisplay].colorformat)
    {
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        active_theme = mpProject->mDisplays[mDisplay].active_theme;
        mPalette.palette = mpProject->mDisplays[mDisplay].themes[active_theme].palette;
        mPalette.total_size = mpProject->mDisplays[mDisplay].themes[active_theme].palette_total_size;
        mPalette.used_size = mpProject->mDisplays[mDisplay].themes[active_theme].palette_predefined;
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        ProjectConfigDlg::CreateDefault4BppPalette(mPalette);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        ProjectConfigDlg::CreateDefault1BppPalette(mPalette);
        break;

    default:
        break;
    }

    int dpi = GetSystemDPI();
    mMenubarHeight = MulDiv(MENU_BAR_HEIGHT_96DPI, dpi, DEFAULT_DPI_96);
    mMinDlgWidth = MulDiv(MIN_RICH_TEXT_EDIT_DLG_WIDTH_96DPI, dpi, DEFAULT_DPI_96);

    mpIRich = NULL;
    mpIDoc = NULL;
}

///////////////////////////////////////////////////////////////////////////////
rich_text_edit_dlg::~rich_text_edit_dlg()
{
    if (mpButtonFrame)
    {
        delete mpButtonFrame;
    }

    if (mpPreview)
    {
        delete mpPreview;
    }
    switch (mpProject->mDisplays[mDisplay].colorformat)
    {
    case GX_COLOR_FORMAT_4BIT_GRAY:
    case GX_COLOR_FORMAT_MONOCHROME:
        if (mPalette.palette)
        {
            delete mPalette.palette;
        }
        break;

    default:
        break;
    }

    if (mpIRich)
    {
        mpIRich->Release();
    }

    if (mpIDoc)
    {
        mpIDoc->Release();
    }
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::PositionChildren()
{
    CRect client;
    CRect size;
    INT width;

    GetClientRect(&client);
    size = client;
    size.left = client.left + 10;
    size.top = client.top + m_title_bar_height + 10;
    size.right = size.right - 10;
    size.bottom = size.top + mMenubarHeight;

    // position button frame
    mpButtonFrame->MoveWindow(&size);

    // position rich edit
    width = (client.right - size.left - 20) / 2;
    size.right = size.left + width;
    size.top = client.top + m_title_bar_height + mMenubarHeight + 20;
    size.bottom = client.bottom - m_status_bar_height - 20;
    mRichEdit.MoveWindow(&size);

    // position preview
    size.OffsetRect(CPoint(width + 10, 0));
    mpPreview->MoveWindow(&size);

    Invalidate();
}


// rich_text_edit_dlg message handlers
///////////////////////////////////////////////////////////////////////////////
BOOL rich_text_edit_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    RichEditInit();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////////////////
int rich_text_edit_dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (express_dialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    mpPreview = new rich_text_preview_win(mpInfo, this);
    mpPreview->Create(target_class_name, _T("text_view_preview"), WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER, CRect(0, 0, 0, 0), this, 0);

    mRichEdit.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_RICH_EDIT);

    AddSaveButton();
    AddCancelButton();

    mpButtonFrame = new text_edit_button_frame(mpInfo, this);
    mpButtonFrame->CreateEx(WS_EX_CONTROLPARENT, target_class_name, _T("text_edit_button_frame"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 0);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::OnSize(UINT nType, int cx, int cy)
{
    express_dialog::OnSize(nType, cx, cy);

    PositionChildren();
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    lpMMI->ptMinTrackSize.x = mMinDlgWidth;

    express_dialog::OnGetMinMaxInfo(lpMMI);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::RichEditInit()
{
    // Retrieve rich edit ole
    mpIRich = mRichEdit.GetIRichEditOle();
    mpIRich->QueryInterface(IID_ITextDocument, (void**)&mpIDoc);

    string_table* table = GetActiveStringTable();

    if (table)
    {
        CString id_name = table->GetResourceIdName(mpInfo->string_id[0]);

        CString string = table->GetString(id_name);
        mRichEdit.SetSel(0, -1);
        mRichEdit.ReplaceSel(string);
        //FormatRichText();
    }

    mRichEdit.SetEventMask(ENM_CHANGE);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::OnChangeRichText()
{
    CString text;

    mRichEdit.GetWindowText(text);

    text = resource_gen::RichTextResIdName2ResId(mpProject, mDisplay, text);

    mpPreview->UpdateRichText(text);
    //FormatRichText();
}

///////////////////////////////////////////////////////////////////////////////
#if 0
void rich_text_edit_dlg::FormatRichText()
{
    // Suspend undo processing
    mpIDoc->Undo(tomSuspend, NULL);

    CString string;
    mRichEdit.GetWindowText(string);

    // It seems CRichEditCtrl treat "\r\n" as a single character.
    // Seplace it to "\n" here to simple the charcter processing.
    string.Replace(_T("\r\n"), _T("\n"));

    // Convert wide string to utf8 string.
    int max_length = string.GetLength() * 6 + 1;
    char *utf8_buf = new char[max_length];
    strcpy_s(utf8_buf, max_length, CT2A(string.GetString(), CP_UTF8));

    CHARFORMAT tag_cf;
    CHARFORMAT def_cf;

    // Initiate tag format
    def_cf.cbSize = sizeof(CHARFORMAT);
    def_cf.dwMask = CFM_COLOR;
    def_cf.dwEffects = 0;
    def_cf.crTextColor = GetSysColor(COLOR_BLACK);

    tag_cf = def_cf;
    tag_cf.crTextColor = GetSysColor(COLOR_HIGHLIGHT);

    GX_STRING text;

    text.gx_string_ptr = utf8_buf;
    text.gx_string_length = strlen(utf8_buf);

    GX_STRING tmpstr;
    GX_RICH_TEXT_FORMAT format;
    GX_UBYTE processed_count;
    UINT glyph_len;
    GX_BOOL escape = GX_FALSE;
    INT start_index = 0;
    INT end_index = 0;
    INT tag_index = 0;

    long selstart, selend;

    mRichEdit.GetSel(selstart, selend);
    mRichEdit.HideSelection(TRUE, TRUE);
    int old_first_visible_line = mRichEdit.GetFirstVisibleLine();
    mRichEdit.SetRedraw(FALSE);


    while (text.gx_string_length)
    {
        if ((!escape) && (text.gx_string_ptr[0] == '\\'))
        {
            escape = GX_TRUE;
            processed_count = 1;
        }
        else
        {
            // Test rich text tags.
            if ((!escape) &&
                (text.gx_string_ptr[0] == '<') &&
                (_gx_rich_text_view_tag_enter((GX_RICH_TEXT_VIEW*)mpInfo->widget, &text, &format, &processed_count) == GX_SUCCESS))
            {
                if (start_index != end_index)
                {
                    utf8_buf[end_index] = '\0';
                    CString tostring(CA2T(utf8_buf + start_index, CP_UTF8));

                    mRichEdit.SetSel(tag_index, tag_index + tostring.GetLength());
                    mRichEdit.SetSelectionCharFormat(def_cf);

                    // Calcualte tag start position.
                    tag_index += tostring.GetLength();
                }

                // Set tags hightlight color.
                mRichEdit.SetSel(tag_index, tag_index + processed_count);
                mRichEdit.SetSelectionCharFormat(tag_cf);

                tag_index += processed_count;
                start_index = end_index + processed_count;
            }
            else
            {

                tmpstr = text;

                // Pick up glyph length.
                if (_gx_utility_utf8_string_character_get(&tmpstr, GX_NULL, &glyph_len) != GX_SUCCESS)
                {
                    // Invalid UTF8 string.
                    return;
                }

                processed_count = glyph_len;
            }

            escape = GX_FALSE;
        }

        end_index += processed_count;
        text.gx_string_ptr+= processed_count;
        text.gx_string_length-= processed_count;
    }

    if (start_index != end_index)
    {
        utf8_buf[end_index] = '\0';
        CString tostring(CA2T(utf8_buf + start_index, CP_UTF8));

        mRichEdit.SetSel(tag_index, tag_index + tostring.GetLength());
        mRichEdit.SetSelectionCharFormat(def_cf);

        // Calcualte tag start position.
        tag_index += tostring.GetLength();
    }

    delete utf8_buf;
    mRichEdit.SetSel(selstart, selend);
    mRichEdit.HideSelection(FALSE, TRUE);

    int new_first_visible_line = mRichEdit.GetFirstVisibleLine();
    if (old_first_visible_line != new_first_visible_line)
    {
        mRichEdit.LineScroll(old_first_visible_line - new_first_visible_line);
    }

    mRichEdit.SetRedraw(); 
    mRichEdit.Invalidate();
    mRichEdit.UpdateWindow();

    // Restore undo processing
    mpIDoc->Undo(tomResume, NULL);
}
#endif

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::InsertTag(CString& tag_start, CString& tag_end)
{
    long sel_start, sel_end;

    // Get current selection
    mRichEdit.GetSel(sel_start, sel_end);

    // Insert tag
    CString text = mRichEdit.GetSelText();
    text.Insert(0, tag_start);
    text.Append(tag_end);
    mRichEdit.ReplaceSel(text, TRUE);

    mRichEdit.HideSelection(TRUE, TRUE);

    // Set focus to rich text edit control
    mRichEdit.SetFocus();

    // Reset selection
    int tag_len = tag_start.GetLength();
    mRichEdit.SetSel(sel_start + tag_len, sel_end + tag_len);
    mRichEdit.HideSelection(FALSE, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedBold()
{
    CString tag_start(_T("<b>"));
    CString tag_end(_T("</b>"));

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedItalic()
{
    CString tag_start(_T("<i>"));
    CString tag_end(_T("</i>"));

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedUnderline()
{
    CString tag_start(_T("<u>"));
    CString tag_end(_T("</u>"));

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedLeftAlign()
{
    CString tag_start(_T("<align left>"));
    CString tag_end(_T("</align>"));

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedCenterAlign()
{
    CString tag_start(_T("<align center>"));
    CString tag_end(_T("</align>"));

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedRightAlign()
{
    CString tag_start(_T("<align right>"));
    CString tag_end(_T("</align>"));

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedHighlightColor(CString &id_name)
{
    CString tag_start;
    CString tag_end(_T("</hc>"));

    tag_start.Format(_T("<hc GX_COLOR_ID_%s>"), id_name);

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedTextColor(CString &id_name)
{
    CString tag_start;
    CString tag_end(_T("</c>"));

    tag_start.Format(_T("<c GX_COLOR_ID_%s>"), id_name);

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::SetSelectedFont(CString &id_name)
{
    CString tag_start;
    CString tag_end(_T("</f>"));

    tag_start.Format(_T("<f GX_FONT_ID_%s>"), id_name);

    InsertTag(tag_start, tag_end);
}

///////////////////////////////////////////////////////////////////////////////
BOOL rich_text_edit_dlg::PreTranslateMessage(MSG* pMsg)
{
    if ((pMsg->message == WM_KEYDOWN) ||
        (pMsg->wParam == VK_RETURN))
    {
        INT ctrl_id = GetFocus()->GetDlgCtrlID();
        if (ctrl_id == ID_RICH_EDIT)
        {
            return CDialog::PreTranslateMessage(pMsg);
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_edit_dlg::OnOK()
{
    mRichEdit.GetWindowText(mText);

    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
COLORREF rich_text_edit_dlg::MakeColorRef(int color_id)
{
    GX_WIDGET* widget = mpInfo->widget;

    if (!widget)
    {
        return 0;
    }

    GX_WINDOW_ROOT *root = get_root_window();

    if (!root)
    {
        return 0;
    }

    GX_COLOR gx_color;
    gx_widget_color_get(widget, color_id, &gx_color);

    COLORREF color_ref;

    color_ref = resource_view::GetColorRef(gx_color, root->gx_window_root_canvas->gx_canvas_display->gx_display_color_format, mPalette.palette, mPalette.used_size);
 
    return color_ref;
}

///////////////////////////////////////////////////////////////////////////////
CString rich_text_edit_dlg::GetText()
{
    return mText;
}

//////////////////////////////////////////////////////////////////////////////
LRESULT rich_text_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_SET_STRING:
        mRichEdit.SetWindowText(GetTestingParam(0));
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
