#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CFont AsteriskFont;
extern CFont MediumFont;
extern CFont ViewHeaderFont;

struct FontResizeParam {
    CFont *font;
    int text_scaler;
    int new_text_scaler;
};

// express_dialog
BEGIN_MESSAGE_MAP(express_dialog, CDialog)
 ON_WM_CREATE()
 ON_WM_NCHITTEST()
 ON_WM_SIZE()
 ON_WM_ERASEBKGND()
 ON_BN_CLICKED(IDCANCEL, OnCancel)
 ON_WM_CLOSE()
 ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
express_dialog::express_dialog(int width, int height)
	: CDialog()
{
    m_width = width;
    m_height = height;
    IconId = 0;
    mSavedMsg = L"";
    m_text_scaler = DEFAULT_TEXT_SCALER;
}

///////////////////////////////////////////////////////////////////////////////
express_dialog::express_dialog(int template_id, CWnd *parent)
	: CDialog(template_id, parent)
{
    m_template_id = template_id;
    m_width = m_height = -1;
    IconId = 0;
    mSavedMsg = L"";
    m_text_scaler = DEFAULT_TEXT_SCALER;
}

///////////////////////////////////////////////////////////////////////////////
express_dialog::~express_dialog()
{
}

///////////////////////////////////////////////////////////////////////////////
int express_dialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//    lpCreateStruct->dwExStyle |= WS_EX_APPWINDOW;

    CDialog::OnCreate(lpCreateStruct);

    CRect client;
    CRect size;
    GetWindowRect(&size);

    if (m_width > 0)
    {
        size.right = size.left + m_width;
        size.bottom = size.top + m_height;
        MoveWindow(size);
    }
    GetClientRect(&client);

    SetControlSize();

    int dpi = GetSystemDPI();
    int offset = MulDiv((TITLE_BAR_HEIGHT - CLOSE_BUTTON_HEIGHT - 3) >> 1, dpi, DEFAULT_DPI_96);

    size = client;
    size.top += offset;
    size.right -= offset;
    size.left = size.right - m_close_button_width;
    size.bottom = size.top + m_title_bar_height;
    close_button.Create(NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, size, this, IDCANCEL);
    close_button.LoadBitmaps(IDB_BTN_CLOSE_NORMAL, IDB_BTN_CLOSE_PRESSED);
    SetControlAccessibleName(close_button.GetSafeHwnd(), _T("Close"));

    if (mTitleText)
    {
        SetWindowText(mTitleText);
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
void express_dialog::SetTitleText(CString &title)
{
    mTitleText = title;
    //SetWindowText(mTitleText);
    //SetWindowText(CString(_T("testing")));
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::SetTitleText(char *text)
{
    SetTitleText(CString(text));
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::SetControlSize()
{
    int dpi = GetSystemDPI();
    int text_scaler = GetTextScaler();

    m_close_button_width = MulDiv(CLOSE_BUTTON_WIDTH, dpi, DEFAULT_DPI_96);
    m_close_button_height = MulDiv(CLOSE_BUTTON_HEIGHT, dpi, DEFAULT_DPI_96);
    m_title_bar_height = GetScaledValue(TITLE_BAR_HEIGHT, dpi, text_scaler);
    m_status_bar_height = GetScaledValue(STATUS_BAR_HEIGHT, dpi, text_scaler);
    m_save_button_width = GetScaledValue(SAVE_BUTTON_WIDTH, dpi, text_scaler);
    m_save_button_height = GetScaledValue(SAVE_BUTTON_HEIGHT, dpi, text_scaler);
    m_cancel_button_width = GetScaledValue(CANCEL_BUTTON_WIDTH, dpi, text_scaler);
    m_cancel_button_height = GetScaledValue(CANCEL_BUTTON_HEIGHT, dpi, text_scaler);
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::AddSaveButton(CString title)
{
    CRect client;
    CRect size;

    GetClientRect(&client);

    size.bottom = client.bottom - 4;
    size.top = size.bottom - m_save_button_height;
    size.right = client.right - 4;
    size.left = size.right - m_save_button_width;

    save_button.Create(title, WS_CHILD | WS_VISIBLE | WS_TABSTOP| BS_DEFPUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, size, this, IDOK);
    save_button.LoadBitmaps(IDB_OK_BUTTON_NORMAL, IDB_OK_BUTTON_PRESSED);
    save_button.SetWindowText(title);
    save_button.SetFont(&MediumFont);
    mSavedMsg = L"Saved";
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::AddCancelButton(CString title)
{
    CRect client;
    CRect size;

    GetClientRect(&client);

    size.bottom = client.bottom - 4 - (m_save_button_height - m_cancel_button_height) / 2;
    size.top = size.bottom - m_cancel_button_height;
    size.left = client.left + 4;
    size.right = size.left + m_cancel_button_width;

    cancel_button.Create(title, BS_PUSHBUTTON | BS_CENTER | WS_TABSTOP | BS_VCENTER | WS_CHILD | WS_VISIBLE, size, this, IDCANCEL);
    cancel_button.SetFont(&MediumFont);
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::PaintRequiredAsterisk(int control_id)
{
    // Pick target control that need a required asterisk mark.
    CWnd* control = GetDlgItem(control_id);

    if (!control)
    {
        return;
    }

    CRect controlbox;
    int xpos, ypos;
    SIZE sz;

    CDC* dc = GetDC();
    dc->SetTextColor(COLORREF(0x2020ff));
    dc->SetBkMode(TRANSPARENT);
    CFont* old_font = dc->SelectObject(&AsteriskFont);

    // Get asterisk text size.
    GetTextExtentPoint32(dc->GetSafeHdc(), _T("*"), 1, &sz);

    // Get control size.
    control->GetWindowRect(&controlbox);
    ScreenToClient(&controlbox);

    // Calculate asterisk draw position.
    xpos = controlbox.left - (sz.cx * 3 / 2);
    ypos = controlbox.top;

    // Draw asterisk.
    TextOut(dc->GetSafeHdc(), xpos, ypos, _T("*"), 1);

    dc->SelectObject(old_font);

    ReleaseDC(dc);
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::PaintTitleAndStatusBar(CDC *dc)
{
    CRect size;
    CRect title_rect;
    CDC dcMemory;
    CBitmap fillmap;
    BITMAP bm;
    int xpos;

    // fill the title bar:
    fillmap.LoadBitmap(IDB_HEADER_BACKGROUND);
    fillmap.GetObject(sizeof(BITMAP), &bm);

    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&fillmap);

    GetClientRect(&size);
    xpos = size.left;

    while(xpos < size.right)
    {
        dc->StretchBlt(xpos, size.top, bm.bmWidth, m_title_bar_height, &dcMemory, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        xpos += bm.bmWidth;
    }

    // draw the icon in top-left corner:
    fillmap.DeleteObject();

    int vspace = 4;
    int icon_width = 0;

    title_rect = size;
    title_rect.bottom = title_rect.top + m_title_bar_height;
    if (IconId)
    {
        fillmap.LoadBitmap(IconId);
        fillmap.GetObject(sizeof(BITMAP), &bm);
        dcMemory.SelectObject(fillmap);

        int dpi = GetSystemDPI();
        icon_width = MulDiv(bm.bmWidth, dpi, DEFAULT_DPI_96);

        int vspace = ((m_title_bar_height - icon_width) >> 1);

        title_rect.left += 4;
        dc->StretchBlt(title_rect.left, title_rect.top + vspace, icon_width, icon_width, &dcMemory, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    }

    if (!mTitleText.IsEmpty())
    {
        // draw the caption
        dc->SetTextColor(RGB(240, 240, 240));
        dc->SetBkMode(TRANSPARENT);
        CFont *old_font = dc->SelectObject(&ViewHeaderFont);
        title_rect.left += (icon_width + 4);
        dc->DrawText(mTitleText, &title_rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        dc->SelectObject(old_font);
    }

    // fill the status bar:
    fillmap.DeleteObject();
    fillmap.LoadBitmap(IDB_STATUS_BAR_FILL);
    fillmap.GetObject(sizeof(BITMAP), &bm);
    dcMemory.SelectObject(&fillmap);

    xpos = size.left;
    while(xpos < size.right)
    {
        dc->StretchBlt(xpos, size.bottom - m_status_bar_height, bm.bmWidth, m_status_bar_height, &dcMemory, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        xpos += bm.bmWidth;
    }
}

LRESULT express_dialog::OnNcHitTest(CPoint point)
{

    // TODO: Add your message handler code here and/or call default
    CRect rect;

    GetClientRect(&rect);
    ClientToScreen(&rect);

    rect.bottom = rect.top + m_title_bar_height;

    if (PtInRect(&rect, point))
    {
        return HTCAPTION;
    }

    return CDialog::OnNcHitTest(point);
}

void express_dialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (save_button)
    {
        CRect client;
        CRect size;
        GetClientRect(&client);

        size.bottom = client.bottom - 4;
        size.top = size.bottom - m_save_button_height;
        size.right = client.right - 4;
        size.left = size.right - m_save_button_width;

        /* Position save button.  */
        save_button.MoveWindow(&size);
    }

    if (cancel_button)
    {
        CRect client;
        CRect size;
        GetClientRect(&client);

        size.bottom = client.bottom - 4 - (m_save_button_height - m_cancel_button_height) / 2;
        size.top = size.bottom - m_cancel_button_height;
        size.left = client.left + 4;
        size.right = size.left + m_cancel_button_width;

        /* Position save button.  */
        cancel_button.MoveWindow(&size);
    }

    if (close_button)
    {
        CRect size;
        GetClientRect(&size);

        size.top += 4;
        size.right -= 4;
        size.left = size.right - m_close_button_width;
        size.bottom = size.top + m_close_button_height;

        /* Position close button.  */
        close_button.MoveWindow(&size);
    }
}

BOOL express_dialog::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnEraseBkgnd(pDC);

    PaintTitleAndStatusBar(pDC);

    return true;
}


///////////////////////////////////////////////////////////////////////////////
BOOL express_dialog::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN)
        {
            //block enter key from closing the dialog when save button do not have focus
            CWnd* focus_owner = GetFocus();
            TCHAR class_name[MAX_PATH];

            if (focus_owner == &close_button)
            {
                // Enter key on close button generates IDOK.
                // Call OnCancel directly to end the dialog.
                OnCancel();
                return TRUE;
            }

            GetClassName(focus_owner->GetSafeHwnd(), class_name, MAX_PATH - 1);

            //the enter key on push button equals to click event
            //the enter key on combobox will show/hide combo list
            //the enter key on multi line edit box will insert a new line character
            //do not block enter key on push button, combobox and multi-line edit box

            if (_tcscmp(class_name, _T("Button")) == 0)
            {
                UINT uStyle = ((CButton *)focus_owner)->GetButtonStyle();
                switch (uStyle & 0xf)
                {
                case BS_CHECKBOX:
                case BS_RADIOBUTTON:
                case BS_AUTOCHECKBOX:
                case BS_AUTORADIOBUTTON:
                    return TRUE;
                }
            }
            else if ((_tcscmp(class_name, _T("Edit")) == 0) || _tcscmp(class_name, _T("RICHEDIT50W")) == 0)
            {
                UINT uStyle = focus_owner->GetStyle();
                if (!(uStyle & ES_MULTILINE))
                {
                    return TRUE;
                }
            }
            else
            {
                if (_tcscmp(class_name, _T("ComboBox")) != 0)
                {
                    return TRUE;
                }
            }
        }

    }

    return CDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
// CustomBitmapButton: A custom CBitmapButton that draws text over the button
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//BEGIN_MESSAGE_MAP(custom_bitmap_button, CBitmapButton)
//    ON_WM_PAINT()
//END_MESSAGE_MAP()

custom_bitmap_button::custom_bitmap_button() : CBitmapButton()
{
}

void custom_bitmap_button::DrawBitmap(LPDRAWITEMSTRUCT lpDIS)
{
    ASSERT(lpDIS != NULL);
    // must have at least the first bitmap loaded before calling DrawItem
    ASSERT(m_bitmap.m_hObject != NULL);     // required

    // use the main bitmap for up, the selected bitmap for down
    CBitmap* pBitmap = &m_bitmap;
    UINT state = lpDIS->itemState;
    if ((state & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL)
        pBitmap = &m_bitmapSel;
    else if ((state & ODS_FOCUS) && m_bitmapFocus.m_hObject != NULL)
        pBitmap = &m_bitmapFocus;   // third image for focused
    else if ((state & ODS_DISABLED) && m_bitmapDisabled.m_hObject != NULL)
        pBitmap = &m_bitmapDisabled;   // last image for disabled

    // draw the whole button
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOld = memDC.SelectObject(pBitmap);
    if (pOld == NULL)
        return;     // destructors will clean up

    CRect rect;
    rect.CopyRect(&lpDIS->rcItem);
    // determine bitmaps size for use in StretchBlt
    BITMAP bits;
    pBitmap->GetObject(sizeof(BITMAP), &bits);
    pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
        &memDC, 0, 0, bits.bmWidth, bits.bmHeight, SRCCOPY);
    memDC.SelectObject(pOld);
}

//void custom_bitmap_button::OnPaint()
void custom_bitmap_button::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CString title;
    CDC *dc = GetDC();
    CRect size;
    GetClientRect(&size);
    GetWindowText(title);
    dc->SetBkMode(TRANSPARENT);

    HIGHCONTRAST info = { 0 };
    info.cbSize = sizeof(HIGHCONTRAST);

    SystemParametersInfoW(SPI_GETHIGHCONTRAST, 0, &info, 0);

    if ((info.dwFlags & HCF_HIGHCONTRASTON) && (!title.IsEmpty()))
    {
        CBrush brush;
        brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        dc->FillRect(&lpDrawItemStruct->rcItem, &brush);
    }
    else
    {
        DrawBitmap(lpDrawItemStruct);
    }

    UINT state = lpDrawItemStruct->itemState;
    
    if ((state & ODS_FOCUS) || (state & ODS_SELECTED))
    {
        CRect rect = lpDrawItemStruct->rcItem;
        CPen penBlue(PS_SOLID, 2, RGB_BUTTON_FOCUS);
        CPen* pOldPen = dc->SelectObject(&penBlue);
        CBrush *pOldBrush = (CBrush*)dc->SelectStockObject(NULL_BRUSH);

        rect.DeflateRect(1, 1, 0, 0);
        dc->Rectangle(rect);
        dc->SelectObject(pOldPen);
        dc->SelectObject(pOldBrush);
    }

    if (state & ODS_SELECTED)
    {
        size.left += 2;
        size.top += 2;
    }
    CFont *old_font = dc->SelectObject(&MediumFont);
    dc->SetTextColor(GetSysColor(COLOR_BTNTEXT));
    dc->DrawText(title, &size, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
    dc->SelectObject(old_font);
    ReleaseDC(dc);
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::OnOK()
{
    CMainFrame *pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

    if (pMain && (!mSavedMsg.IsEmpty()))
    {
        pMain->GetStatusMsgControl()->SetWindowText(mSavedMsg);
        pMain->GetStatusMsgControl()->NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);
    }

    CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
INT_PTR express_dialog::DoModal()
{
    // TODO: Add your specialized code here and/or call the base class
    CDialogTemplate dlt;

    if (!dlt.Load(MAKEINTRESOURCE(express_dialog::m_template_id)))
    {
        return -1;
    }

    m_text_scaler = GetTextScaler();

    int point_size = DEFAULT_DLG_FONT_POINT_SIZE * m_text_scaler / DEFAULT_TEXT_SCALER;

    dlt.SetFont(L"MS Shell Dlg", point_size);
    LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
    m_lpszTemplateName = NULL;
    InitModalIndirect(pdata);
    GlobalUnlock(dlt.m_hTemplate);

    return CDialog::DoModal();
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    CWnd *child = CWnd::FromHandle(hwnd);

    CRect rect;
    child->GetWindowRect(&rect);

    CWnd *parent = child->GetParent();
    parent->ScreenToClient(&rect);

    int ctl_id = child->GetDlgCtrlID();
    FontResizeParam *param = (FontResizeParam*)lParam;

    switch (ctl_id)
    {

    case IDCANCEL:
        child->SetFont(param->font);
        break;

    case IDCLOSE:
    case IDOK:
        break;

    case IDC_CREATE_PROJECT:
        parent->GetClientRect(rect);
        rect = recent_project_win::GetCreateNewProjectButtonSize(rect);
        child->MoveWindow(&rect);
        break;

    default:
        child->SetFont(param->font);
        rect.left = rect.left * param->new_text_scaler / param->text_scaler;
        rect.right = rect.right * param->new_text_scaler / param->text_scaler;
        rect.top = rect.top * param->new_text_scaler / param->text_scaler;
        rect.bottom = rect.bottom * param->new_text_scaler / param->text_scaler;
        child->MoveWindow(&rect);
        break;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::CreateDialogFont(int text_scaler)
{
    m_dialog_font.DeleteObject();

    // Get number of pixels per logical inch along the screen height
    int dpi = GetSystemDPI();

    int point_size = DEFAULT_DLG_FONT_POINT_SIZE * text_scaler / DEFAULT_TEXT_SCALER;

    // Convert point size to logic unit
    int height = -MulDiv(point_size, dpi, 72);

    m_dialog_font.CreateFont(height, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Microsoft Sans Serif"));
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::ChangeDialogFontSize()
{
    int new_text_scaler = GetTextScaler();

    if (new_text_scaler != m_text_scaler)
    {
        CreateDialogFont(new_text_scaler);

        SetControlSize();

        CRect rect;
        GetWindowRect(&rect);
        GetParent()->ScreenToClient(&rect);
        int width = rect.Width() * new_text_scaler / m_text_scaler;
        int height = rect.Height() * new_text_scaler / m_text_scaler;
        rect.right = rect.left + width;
        rect.bottom = rect.top + height;
        MoveWindow(&rect);
        FontResizeParam param = { &m_dialog_font, m_text_scaler, new_text_scaler };
        EnumChildWindows(GetSafeHwnd(), EnumChildProc, (long)&param);
        Invalidate();

        m_text_scaler = new_text_scaler;
    }
}

///////////////////////////////////////////////////////////////////////////////
void express_dialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    ChangeDialogFontSize();

    CDialog::OnSettingChange(uFlags, lpszSection);
}
