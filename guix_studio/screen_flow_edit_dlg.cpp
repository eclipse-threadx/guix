// screen_flow_edit_dlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "screen_flow_edit_dlg.h"

enum drag_modes {
    DRAG_NONE = 0,
    DRAG_TOP_LEFT,
    DRAG_TOP,
    DRAG_TOP_RIGHT,
    DRAG_RIGHT,
    DRAG_BOTTOM_RIGHT,
    DRAG_BOTTOM,
    DRAG_BOTTOM_LEFT,
    DRAG_LEFT,
    DRAG_ALL
};

enum zoom_types{
    TYPE_ZOOM_IN = 0,
    TYPE_ZOOM_OUT,
    TYPE_FIT_CONTENT
};

enum ctrl_ids {
    BTN_ID_FIT_CONTENT = 1024,
    BTN_ID_ZOOM_IN,
    BTN_ID_ZOOM_OUT,
    BTN_ID_RUN_PROJECT,
    ID_ZOOM_VALUE_LABLE,
};

#define MIN_SCREEN_FLOW_EDIT_DLG_WIDTH  800
#define MIN_SCREEN_FLOW_EDIT_DLG_HEIGHT 400

#define HDRAG_TIMER 1000
#define VDRAG_TIMER 1001
#define ANIMATION_INTERVAL 100
#define ANIMATION_SHIFT 5

#define BUTTON_FRAME_HEIGHT 29
#define ICON_HEIGHT 22
#define ICON_WIDTH  30
#define MIN_SCREEN_BOX_SIZE 10

#define ZOOM_IN_PUSHED     0x1
#define ZOOM_OUT_PUSHED    0x2
#define FIT_CONTENT_PUSHED 0x4

#define SHIFTED 0x8000

extern CString target_class_name;

BEGIN_MESSAGE_MAP(screen_flow_button_frame, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_BN_CLICKED(BTN_ID_FIT_CONTENT, OnFitContent)
    ON_BN_CLICKED(BTN_ID_ZOOM_OUT, OnZoomOut)
    ON_BN_CLICKED(BTN_ID_ZOOM_IN, OnZoomIn)
    ON_BN_CLICKED(BTN_ID_RUN_PROJECT, OnRunProject)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
screen_flow_button_frame::screen_flow_button_frame(screen_flow *flow, CWnd * pParent)
{
    mpScreenFlow = flow;
}

///////////////////////////////////////////////////////////////////////////////
screen_flow_button_frame::~screen_flow_button_frame()
{

}


///////////////////////////////////////////////////////////////////////////////
int screen_flow_button_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    mFitContent.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, BTN_ID_FIT_CONTENT);
    mFitContent.LoadBitmaps(IDB_SCREEN_FLOW_FIT_CONTENT, IDB_SCREEN_FLOW_FIT_CONTENT_PRESSED);
    SetControlAccessibleName(mFitContent.GetSafeHwnd(), _T("fit content"));

    mZoomOutButton.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, BTN_ID_ZOOM_OUT);
    mZoomOutButton.LoadBitmaps(IDB_SCREEN_FLOW_ZOOM_OUT, IDB_SCREEN_FLOW_ZOOM_OUT_PRESSED);
    SetControlAccessibleName(mZoomOutButton.GetSafeHwnd(), _T("zoom out"));

    mZoomInButton.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_OWNERDRAW | MAX_TAB_STOPS, CRect(0, 0, 0, 0), this, BTN_ID_ZOOM_IN);
    mZoomInButton.LoadBitmaps(IDB_SCREEN_FLOW_ZOOM_IN, IDB_SCREEN_FLOW_ZOOM_IN_PRESSED);
    SetControlAccessibleName(mZoomInButton.GetSafeHwnd(), _T("zoom in"));

    mZoomValueLable.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_ZOOM_VALUE_LABLE);
    UpdateZoomValue();

    mFont.CreateFont(ICON_HEIGHT, 0, 0, 0,
        FW_NORMAL, FALSE, TRUE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    mZoomValueLable.SetFont(&mFont);
    SetLiveRegion(mZoomValueLable.GetSafeHwnd());


    //Add run button
    mRunButton.Create(_T("Run"), BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, BTN_ID_RUN_PROJECT);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_button_frame::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    CRect size;
    CRect client;
    GetClientRect(&client);

    CBitmap icon;
    BITMAP bmp;
    icon.LoadBitmap(IDB_FIT_CONTENT);
    icon.GetObject(sizeof(BITMAP), &bmp);

    //Position fit content button
    size.top = (client.Height() - bmp.bmHeight) >> 1;
    size.bottom = size.top + bmp.bmHeight;
    size.left = client.left + 10;
    size.right = size.left + bmp.bmWidth;

    mFitContent.MoveWindow(size);

    //Position zoom out button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mZoomOutButton.MoveWindow(size);

    //Position zoom in button
    size.OffsetRect(CPoint(bmp.bmWidth + 10, 0));
    mZoomInButton.MoveWindow(size);

    //Position zoom value lable
    size.left = size.right + 10;
    size.right = size.left + ICON_WIDTH * 2;
    mZoomValueLable.MoveWindow(size);

    //Position run button
    size.top = client.top + 2;
    size.bottom = client.bottom - 2;
    size.right = client.right - 10;
    size.left = size.right - SAVE_BUTTON_WIDTH;
    mRunButton.MoveWindow(size);
}


///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_button_frame::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if ((pMsg->message == WM_KEYDOWN) &&
        (pMsg->wParam == VK_RETURN))
    {
        CWnd* focus_owner = GetFocus();
        switch (focus_owner->GetDlgCtrlID())
        {
        case BTN_ID_FIT_CONTENT:
            OnFitContent();
            return TRUE;

        case BTN_ID_ZOOM_OUT:
            OnZoomOut();
            return TRUE;

        case BTN_ID_ZOOM_IN:
            OnZoomIn();
            return TRUE;

        case BTN_ID_RUN_PROJECT:
            OnRunProject();
            return TRUE;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_button_frame::UpdateZoomValue()
{
    CString text;

    text.Format(_T("%d"), mpScreenFlow->GetScale());
    text.Append(_T("%"));
    mZoomValueLable.SetWindowText(text);

    CRect size;
    mZoomValueLable.GetWindowRect(&size);
    ScreenToClient(&size);
    InvalidateRect(&size);

    // Raise a UIA LiveRegionChanged
    // event so that a screen reader is made aware of a change to the LiveRegion.
    // Make sure the updated text is set on the label before making this call.
    mZoomValueLable.NotifyWinEvent(
        EVENT_OBJECT_LIVEREGIONCHANGED,
        OBJID_CLIENT,
        CHILDID_SELF);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_button_frame::OnFitContent()
{
    screen_flow_edit_dlg* dlg = (screen_flow_edit_dlg*)GetParent();

    dlg->Zoom(TYPE_FIT_CONTENT);
    UpdateZoomValue();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_button_frame::OnZoomOut()
{
    screen_flow_edit_dlg* dlg = (screen_flow_edit_dlg*)GetParent();

    dlg->Zoom(TYPE_ZOOM_OUT);
    UpdateZoomValue();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_button_frame::OnZoomIn()
{
    screen_flow_edit_dlg* dlg = (screen_flow_edit_dlg*)GetParent();

    dlg->Zoom(TYPE_ZOOM_IN);
    UpdateZoomValue();
}


///////////////////////////////////////////////////////////////////////////////
void screen_flow_button_frame::OnRunProject()
{
    project_view* project_view = GetProjectView();

    if (project_view)
    {
        project_view->RunApplication(this);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_button_frame::OnEraseBkgnd(CDC* pDC)
{
    HIGHCONTRAST info = { 0 };
    info.cbSize = sizeof(HIGHCONTRAST);

    SystemParametersInfoW(SPI_GETHIGHCONTRAST, 0, &info, 0);

    CBrush *oldBrush;
    CBrush brush;

    if (info.dwFlags & HCF_HIGHCONTRASTON)
    {
        brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
       
    }
    else
    {
        brush.CreateSolidBrush(RGB(215, 215, 215));
    }

    oldBrush = pDC->SelectObject(&brush);

    CRect size;
    GetClientRect(&size);
    pDC->Rectangle(size);
    pDC->SelectObject(oldBrush);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
HBRUSH screen_flow_button_frame::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here
    if (pWnd->GetDlgCtrlID() == ID_ZOOM_VALUE_LABLE)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

BEGIN_MESSAGE_MAP(screen_flow_diagram_win, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_SIZE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
screen_flow_diagram_win::screen_flow_diagram_win(int display, screen_flow *flow, CWnd* pParent)
{
    mpScreenFlow = flow;
    mpDragItem = NULL;
    mConnectionList.RemoveAll();
    mActiveDisplay = display;

    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);

    mDisplayHeight = 0;
    mDisplayWidth = 0;

    mDiagramScale = flow->GetScale();
    mSystemDpi = GetSystemDPI();
    mScale = MulDiv(mDiagramScale, mSystemDpi, DEFAULT_DPI_96);

    InitConnectionList();
}

///////////////////////////////////////////////////////////////////////////////
screen_flow_diagram_win::~screen_flow_diagram_win()
{
    for (int index = 0; index < mConnectionList.GetCount(); index++)
    {
        delete mConnectionList.GetAt(index);
    }
    mConnectionList.RemoveAll();

    delete m_scroll_helper;
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_diagram_win::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnPaint()
{
    CPaintDC dc(this);

    flow_item *item;
    CRect client;
    GetClientRect(&client);

    CDC        dcMem;
    HBITMAP    hbmMem;
    HANDLE     hOld;

    /* Flush canvas memory to off-screen DC. */
    dcMem.CreateCompatibleDC(&dc);
    hbmMem = CreateCompatibleBitmap(dc, client.Width(), client.Height());
    hOld = SelectObject(dcMem, hbmMem);

    dcMem.FillSolidRect(client, GetSysColor(COLOR_3DFACE));

    Graphics       g(dcMem);
    SolidBrush    *brush;
    SolidBrush     normalBrush(Color(255, 0, 0, 0));
    SolidBrush     selectedBrush(Color(255, 0, 120, 219));
    SolidBrush     whiteBrush(Color(255, 255, 255, 255));
    Pen           *pen;
    Pen            normalPen(Color(255, 0, 0, 0), 2);
    Pen            selectedPen(Color(255, 0, 120, 219), 2);
    FontFamily     fontFamily(_T("Times New Roman"));
    Gdiplus::Font  font(&fontFamily, (REAL)(16 * mScale / 100), FontStyleRegular, UnitPixel);
    StringFormat   stringFormat;
    AdjustableArrowCap bigArrow(5, 5);
    RectF           rectF;

    CSize scrollpos = m_scroll_helper->GetScrollPos();

    //draw trigger connection
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    normalPen.SetDashStyle(DashStyleSolid);
    normalPen.SetCustomEndCap(&bigArrow);

    trigger_connection *connection;
    flow_item *src_item, *target_item;
    CPoint scaled_line_start;
    CPoint scaled_line_end;

    for (int index = 0; index < mConnectionList.GetCount(); index++)
    {
        connection = mConnectionList.GetAt(index);

        src_item = mpScreenFlow->GetFlowItem(connection->source_screen);
        target_item = mpScreenFlow->GetFlowItem(connection->target_screen);

        if (src_item && target_item && src_item->enabled && target_item->enabled)
        {
            // scaling line start and end point
            scaled_line_start = GetScaledPoint(connection->line_start);
            scaled_line_end = GetScaledPoint(connection->line_end);

            g.DrawLine(&normalPen,
                scaled_line_start.x - scrollpos.cx,
                scaled_line_start.y - scrollpos.cy,
                scaled_line_end.x - scrollpos.cx,
                scaled_line_end.y - scrollpos.cy);
        }
    }

    //draw screen flow

    // Center-justify each line of text.
    stringFormat.SetAlignment(StringAlignmentCenter);

    // Center the block of text (top to bottom) in the rectangle.
    stringFormat.SetLineAlignment(StringAlignmentCenter);
    stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);

    CRect scaled_rect;

    selectedPen.SetDashStyle(DashStyleDash);

    if (GetFocus() != this)
    {
        Color color;
        normalBrush.GetColor(&color);
        selectedBrush.SetColor(color);
        selectedPen.SetColor(color);
    }

    for (int index = 0; index < mpScreenFlow->GetFlowListCount() + 1; index++)
    {
        if (index < mpScreenFlow->GetFlowListCount())
        {
            item = mpScreenFlow->GetFlowItem(index);

            if (item == mpDragItem)
            {
                //draw selected item on the top of other screen items
                continue;
            }
        }
        else
        {
            item = mpDragItem;
        }

        if (item && item->enabled)
        {
            if (mpDragItem == item)
            {
                brush = &selectedBrush;
                pen = &selectedPen;
            }
            else
            {
                brush = &normalBrush;
                pen = &normalPen;
            }

            scaled_rect = GetScaledRect(item->rect);

            scaled_rect.DeflateRect(1, 1, 1, 1);

            rectF.X = (REAL)(scaled_rect.left - scrollpos.cx);
            rectF.Y = (REAL)(scaled_rect.top - scrollpos.cy);
            rectF.Width = (REAL)scaled_rect.Width() - 1;
            rectF.Height = (REAL)scaled_rect.Height() - 1;

            g.FillRectangle(&whiteBrush, rectF);
            g.DrawRectangle(pen, rectF);
            g.DrawString(item->screen_name, -1, &font, rectF, &stringFormat, brush);
        }
    }

    /* Transfer the off-screen DC to the screen. */
    BitBlt(dc, 0, 0, client.Width(), client.Height(), dcMem, 0, 0, SRCCOPY);

    /* Free-up the off-screen DC.  */
    SelectObject(dcMem, hOld);
    DeleteObject(hbmMem);
    DeleteDC(dcMem);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnLButtonDown(UINT nFlags, CPoint point)
{
    CPoint pt = point;
    CSize scrollpos = m_scroll_helper->GetScrollPos();
    pt.x += scrollpos.cx;
    pt.y += scrollpos.cy;

    SetFocus();
    SetCapture();

    mDragMode = CheckResizeCursor(pt);

    if (mDragMode != DRAG_NONE)
    {
        mDragStart = point;
    }
    else
    {
        CRect dirty;
        CSize scrollpos = m_scroll_helper->GetScrollPos();

        if (mpDragItem)
        {
            dirty = GetScaledRect(mpDragItem->rect);

            dirty.left -= scrollpos.cx;
            dirty.right -= scrollpos.cx;
            dirty.top -= scrollpos.cy;
            dirty.bottom -= scrollpos.cy;

            //mark old drag item as dirty
            InvalidateRect(&dirty);
        }

        // Get flow item under left button down point.
        mpDragItem = mpScreenFlow->GetFlowItem(pt, TRUE);

        if (mpDragItem)
        {
            mDragMode = DRAG_ALL;
            mDragStart = point;
            SelectedVisible();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnLButtonUp(UINT nFlags, CPoint point)
{
    mDragMode = DRAG_NONE;


    ReleaseCapture();
    UpdateDisplaySize();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnRButtonDown(UINT nFlags, CPoint point)
{
    OnLButtonDown(nFlags, point);

    CWnd::OnRButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnRButtonUp(UINT nFlags, CPoint point)
{
    CPoint pt = point;
    CSize scrollpos = m_scroll_helper->GetScrollPos();
    pt.x += scrollpos.cx;
    pt.y += scrollpos.cy;

    ReleaseCapture();

    flow_item *item = mpScreenFlow->GetFlowItem(pt);

    if (item)
    {
        EditFlowItem(item);
    }

    CWnd::OnRButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    if (!mpDragItem)
    {
        SelectFlowItem(FindFirstVisibleItem());
    }
    else
    {
        SelectFlowItem(mpDragItem);
        CRect scaled_rect = GetScaledRect(mpDragItem->rect);
        CSize scrollpos = m_scroll_helper->GetScrollPos();

        scaled_rect.OffsetRect(-scrollpos.cx, -scrollpos.cy);
        InvalidateRect(scaled_rect);
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
    //mDragMode = DRAG_NONE;
    if (mpDragItem)
    {
        CRect scaled_rect = GetScaledRect(mpDragItem->rect);
        CSize scrollpos = m_scroll_helper->GetScrollPos();

        scaled_rect.OffsetRect(-scrollpos.cx, -scrollpos.cy);

        InvalidateRect(scaled_rect);
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    m_scroll_helper->OnSize(nType, cx, cy);
}

///////////////////////////////////////////////////////////////////////////////
flow_item * screen_flow_diagram_win::FindFirstVisibleItem()
{
    flow_item *item;
    flow_item *find = NULL;
    CRect find_rect;
    CRect scaled_rect;
    CRect client;
    CSize scrollpos = m_scroll_helper->GetScrollPos();

    GetClientRect(&client);

    for (int index = 0; index < mpScreenFlow->GetFlowListCount(); index++)
    {
        item = mpScreenFlow->GetFlowItem(index);

        if (item && item->enabled)
        {
            scaled_rect = GetScaledRect(item->rect);
            scaled_rect.OffsetRect(-scrollpos.cx, -scrollpos.cy);

            if ((scaled_rect.left >= client.left) &&
                (scaled_rect.top >= client.top))
            {
                if ((!find) ||
                    (scaled_rect.top < find_rect.top) ||
                    ((scaled_rect.top == find_rect.top) && (scaled_rect.left < find_rect.left)))
                {
                    find = item;
                    find_rect = scaled_rect;
                }
            }
        }
    }

    return find;
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_diagram_win::EditFlowItem(flow_item *item)
{
    if (!item)
    {
        return FALSE;
    }
    flow_item *old_item = new flow_item(*item);

    trigger_list_edit_dlg dlg(mActiveDisplay, mpScreenFlow, item);

    if (dlg.DoModal() == IDOK)
    {
        RemoveConnection(old_item);
        CheckAddConnection(item);

        CRect client;
        GetClientRect(&client);
        InvalidateRect(&client);
    }

    delete old_item;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::SelectFlowItem(flow_item* item)
{
    if (item && item->enabled)
    {
        if (mpDragItem)
        {
            //invalid current selected item
            CSize scrollpos = m_scroll_helper->GetScrollPos();
            CRect scaled_rect = GetScaledRect(mpDragItem ->rect);
            scaled_rect.OffsetRect(-scrollpos.cx, -scrollpos.cy);
            InvalidateRect(scaled_rect);
        }

        CString name = item->screen_name;
        name.Format(_T("Screen flow diagram, item \"%s\" is selected"), name);

        SetControlAccessibleName(this->GetSafeHwnd(), name);

        mpDragItem = item;
        SelectedVisible();
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::HideFlowItem(flow_item* item)
{
    if (item == mpDragItem)
    {
        mpDragItem = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::ShiftLeft(CRect scaled_rect, CRect client, INT scaled_shift_x, INT shift_x)
{
    CSize scrollpos = m_scroll_helper->GetScrollPos();

    if (scaled_rect.left - scrollpos.cx + scaled_shift_x < client.left)
    {
        // exceed client left
        int scaled_left;
        int left;

        scaled_shift_x = client.left + scrollpos.cx - scaled_rect.left;
        scaled_left = scaled_rect.left + scaled_shift_x;
        left = (scaled_left * 100 + mScale - 1) / mScale;
        shift_x = left - mpDragItem->rect.left;
    }
    else if (mpDragItem->rect.left + shift_x >= mpDragItem->rect.right - MIN_SCREEN_BOX_SIZE)
    {
        // exceed minimum screen box width
        shift_x = mpDragItem->rect.right - MIN_SCREEN_BOX_SIZE - mpDragItem->rect.left;
        scaled_shift_x = shift_x * mScale / 100;
    }

    if (shift_x)
    {
        mpDragItem->rect.left += shift_x;
        mDragStart.x += scaled_shift_x;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::ShiftTop(CRect scaled_rect, CRect client, INT scaled_shift_y, INT shift_y)
{
    CSize scrollpos = m_scroll_helper->GetScrollPos();

    if (scaled_rect.top - scrollpos.cy + scaled_shift_y < client.top)
    {
        // exceed client top
        int scaled_top;
        int top;

        scaled_shift_y = client.top  + scrollpos.cy - scaled_rect.top;
        scaled_top = scaled_rect.top + scaled_shift_y;
        top = (scaled_top * 100 + mScale - 1) / mScale;
        shift_y = top - mpDragItem->rect.top;
    }
    else if (mpDragItem->rect.top + shift_y >= mpDragItem->rect.bottom - MIN_SCREEN_BOX_SIZE)
    {
        // exceed minimum screen box height
        shift_y = mpDragItem->rect.bottom - MIN_SCREEN_BOX_SIZE - mpDragItem->rect.top;
        scaled_shift_y = shift_y * mScale / 100;
    }

    if (shift_y)
    {
        mpDragItem->rect.top += shift_y;
        mDragStart.y += scaled_shift_y;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::ShiftRight(CRect scaled_rect, CRect client, INT scaled_shift_x, INT shift_x)
{
    CSize scrollpos = m_scroll_helper->GetScrollPos();

    if (scaled_rect.right - scrollpos.cx + scaled_shift_x> client.right)
    {
        // exceed client right
        int scaled_right;
        int right;

        scaled_shift_x = client.right + scrollpos.cx - scaled_rect.right;
        scaled_right = scaled_rect.right + scaled_shift_x;
        right = (scaled_right * 100) / mScale;
        shift_x = right - mpDragItem->rect.right;
    }
    else if (mpDragItem->rect.right + shift_x <= mpDragItem->rect.left + MIN_SCREEN_BOX_SIZE)
    {
        // exceed minimum screen box width
        shift_x = mpDragItem->rect.left + MIN_SCREEN_BOX_SIZE - mpDragItem->rect.right;
        scaled_shift_x = shift_x * mScale / 100;
    }

    if (shift_x)
    {
        mpDragItem->rect.right += shift_x;
        mDragStart.x += scaled_shift_x;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::ShiftBottom(CRect scaled_rect, CRect client, INT scaled_shift_y, INT shift_y)
{
    CSize scrollpos = m_scroll_helper->GetScrollPos();

    if (scaled_rect.bottom - scrollpos.cy + scaled_shift_y> client.bottom)
    {
        // exceed client bottom
        int scaled_bottom;
        int bottom;

        scaled_shift_y = client.bottom + scrollpos.cy - scaled_rect.bottom;
        scaled_bottom = scaled_rect.bottom + scaled_shift_y;
        bottom = (scaled_bottom * 100) / mScale;
        shift_y = bottom - mpDragItem->rect.bottom;
    }

    if (mpDragItem->rect.bottom + shift_y <= mpDragItem->rect.top + MIN_SCREEN_BOX_SIZE)
    {
        // exceed minimum screen box height
        shift_y = mpDragItem->rect.top + MIN_SCREEN_BOX_SIZE - mpDragItem->rect.bottom;
        scaled_shift_y = shift_y * mScale / 100;
    }

    if (shift_y)
    {
        mpDragItem->rect.bottom += shift_y;
        mDragStart.y += scaled_shift_y;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnMouseMove(UINT nFlags, CPoint point)
{
    if (mDragMode == DRAG_NONE)
    {
        if ((nFlags & MK_LBUTTON) == 0)
        {
            CSize scrollpos = m_scroll_helper->GetScrollPos();
            CPoint pt = point;
            pt.x += scrollpos.cx;
            pt.y += scrollpos.cy;

            CheckResizeCursor(pt);
        }
        return;
    }

    if (mpDragItem && (nFlags & MK_LBUTTON))
    {
        int scaled_shift_x = point.x - mDragStart.x;
        int scaled_shift_y = point.y - mDragStart.y;
        int shift_x = point.x * 100 / mScale - mDragStart.x * 100 / mScale;
        int shift_y = point.y * 100 / mScale - mDragStart.y * 100 / mScale;

        CRect client;
        CRect scaled_rect = GetScaledRect(mpDragItem->rect);

        GetClientRect(&client);

        switch (mDragMode)
        {
        case DRAG_ALL:
            if (scaled_rect.left + scaled_shift_x < client.left)
            {
                int scaled_left;
                int left;

                scaled_shift_x = client.left - scaled_rect.left;
                scaled_left = scaled_rect.left + scaled_shift_x;
                left = (scaled_left * 100 + mScale - 1) / mScale;
                shift_x = left - mpDragItem->rect.left;
            }

            if (shift_x)
            {
                mpDragItem->rect.left += shift_x;
                mpDragItem->rect.right += shift_x;
                mDragStart.x += scaled_shift_x;
            }

            if (scaled_rect.top + scaled_shift_y < client.top)
            {
                int scaled_top;
                int top;

                scaled_shift_y = client.top - scaled_rect.top;
                scaled_top = scaled_rect.top + scaled_shift_y;
                top = (scaled_top * 100 + mScale - 1) / mScale;
                shift_y = top - mpDragItem->rect.top;
            }

            if (shift_y)
            {
                mpDragItem->rect.top += shift_y;
                mpDragItem->rect.bottom += shift_y;
                mDragStart.y += scaled_shift_y;
            }
            break;

        case DRAG_TOP_LEFT:
            ShiftTop(scaled_rect, client, scaled_shift_y, shift_y);
            ShiftLeft(scaled_rect, client, scaled_shift_x, shift_x);
            break;

        case DRAG_TOP:
            ShiftTop(scaled_rect, client, scaled_shift_y, shift_y);
            break;

        case DRAG_TOP_RIGHT:
            ShiftTop(scaled_rect, client, scaled_shift_y, shift_y);
            ShiftRight(scaled_rect, client, scaled_shift_x, shift_x);
            break;

        case DRAG_RIGHT:
            ShiftRight(scaled_rect, client, scaled_shift_x, shift_x);
            break;

        case DRAG_BOTTOM_RIGHT:
            ShiftBottom(scaled_rect, client, scaled_shift_y, shift_y);
            ShiftRight(scaled_rect, client, scaled_shift_x, shift_x);
            break;

        case DRAG_BOTTOM:
            ShiftBottom(scaled_rect, client, scaled_shift_y, shift_y);
            break;

        case DRAG_BOTTOM_LEFT:
            ShiftBottom(scaled_rect, client, scaled_shift_y, shift_y);
            ShiftLeft(scaled_rect, client, scaled_shift_x, shift_x);
            break;

        case DRAG_LEFT:
            ShiftLeft(scaled_rect, client, scaled_shift_x, shift_x);
            break;
        }

        UpdateConnectionPos(mpDragItem);
        InvalidateRect(&client);
    }

    CWnd::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_diagram_win::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

    if (nFlags & MK_CONTROL)
    {
        // TODO: Add your message handler code here and/or call default
        if (zDelta > 0)
        {
            Zoom(TYPE_ZOOM_IN);
        }
        else
        {
            Zoom(TYPE_ZOOM_OUT);
        }

        return CWnd::OnMouseWheel(nFlags, zDelta, pt);
    }
    else
    {
        BOOL wasScrolled = m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
        return wasScrolled;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_diagram_win::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        if (HandleKeydown(pMsg->wParam))
        {
            return TRUE;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////
VOID screen_flow_diagram_win::Zoom(int type)
{
    //make the inner window fit the client window
    int old_scale = mDiagramScale;
    int new_scale = mDiagramScale;

    switch (type)
    {
    case TYPE_ZOOM_IN:
        new_scale = new_scale + 25;

        if (new_scale % 25)
        {
            new_scale -= (new_scale % 25);
        }
        break;

    case TYPE_ZOOM_OUT:
        if (new_scale % 25)
        {
            new_scale -= (new_scale % 25);
        }
        else
        {
            new_scale -= 25;
        }
        break;

    case TYPE_FIT_CONTENT:
    {
        CRect client;
        int border_width = (::GetSystemMetrics(SM_CXBORDER)) * 2;

        GetWindowRect(&client);

        int scale_x = (client.Width() - border_width) * 100 / mDisplayWidth;
        int scale_y = (client.Height() - border_width) * 100 / mDisplayHeight;

        if (scale_x < scale_y)
        {
            new_scale = scale_x;
        }
        else
        {
            new_scale = scale_y;
        }

        new_scale = MulDiv(new_scale, DEFAULT_DPI_96, mSystemDpi);
    }
        break;

    default:
        return;
    }

    if (new_scale > SCREEN_FLOW_MAX_SCALE)
    {
        new_scale = SCREEN_FLOW_MAX_SCALE;
    }
    else if (new_scale < SCREEN_FLOW_MIN_SCALE)
    {
        new_scale = SCREEN_FLOW_MIN_SCALE;
    }

    mDiagramScale = new_scale;

    mpScreenFlow->SetScale(mDiagramScale);
    mScale = MulDiv(mDiagramScale, mSystemDpi, DEFAULT_DPI_96);
    m_scroll_helper->SetDisplaySize(mDisplayWidth * mScale / 100, mDisplayHeight * mScale / 100);
    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
CRect screen_flow_diagram_win::GetScaledRect(CRect rect)
{
    CRect scaled;

    scaled.left = rect.left * mScale / 100;
    scaled.top = rect.top * mScale / 100;
    scaled.right = rect.right * mScale / 100;
    scaled.bottom = rect.bottom * mScale / 100;

    return scaled;
}

///////////////////////////////////////////////////////////////////////////////
CPoint screen_flow_diagram_win::GetScaledPoint(CPoint point)
{
    CPoint scaled;

    scaled.x = point.x * mScale / 100;
    scaled.y = point.y * mScale / 100;

    return scaled;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::InitConnectionList()
{
    flow_item *source;

    for (int index = 0; index < mpScreenFlow->GetFlowListCount(); index++)
    {
        //loop through flow items
        source = mpScreenFlow->GetFlowItem(index);
        CheckAddConnection(source);
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::UpdateDisplaySize()
{
    flow_item *item;

    mDisplayWidth = 0;
    mDisplayHeight = 0;

    for (int index = 0; index < mpScreenFlow->GetFlowListCount(); index++)
    {
        //loop through flow items
        item = mpScreenFlow->GetFlowItem(index);

        if (item->enabled)
        {
            if (item->rect.right > mDisplayWidth)
            {
                mDisplayWidth = item->rect.right;
            }

            if (item->rect.bottom > mDisplayHeight)
            {
                mDisplayHeight = item->rect.bottom;
            }
        }
    }

    m_scroll_helper->SetDisplaySize(mDisplayWidth * mScale / 100, mDisplayHeight * mScale / 100);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::SelectedVisible()
{
    if (mpDragItem)
    {
        CSize scrollpos = m_scroll_helper->GetScrollPos();

        CRect rect = GetScaledRect(mpDragItem->rect);

        rect.MoveToXY(CPoint(rect.left - scrollpos.cx, rect.top - scrollpos.cy));

        CRect client;
        GetClientRect(&client);
        BOOL mark_whole_dirty = FALSE;

        if ((rect.right > client.right) && (rect.left > client.left))
        {
            //move scrollbar right

            scrollpos.cx += (rect.right - client.right);
            m_scroll_helper->SetScrollPos(SB_HORZ, scrollpos.cx, FALSE);
            mark_whole_dirty = TRUE;
        }
        else if ((rect.left < client.left) && (rect.right < client.right))
        {
            scrollpos.cx += (rect.left - client.left);
            m_scroll_helper->SetScrollPos(SB_HORZ, scrollpos.cx, FALSE);
            mark_whole_dirty = TRUE;
        }

        if ((rect.bottom > client.bottom) && (rect.top > client.top))
        {
            //move scrollbar down
            scrollpos.cy += (rect.bottom - client.bottom);
            m_scroll_helper->SetScrollPos(SB_VERT, scrollpos.cy, FALSE);
            mark_whole_dirty = TRUE;
        }
        else if ((rect.top < client.top) && (rect.bottom < client.bottom))
        {
            scrollpos.cy += (rect.top - client.top);
            m_scroll_helper->SetScrollPos(SB_VERT, scrollpos.cy, FALSE);
            mark_whole_dirty = TRUE;
        }

        if (mark_whole_dirty)
        {
            InvalidateRect(&client);
        }
        else
        {
            InvalidateRect(&rect);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_diagram_win::HandleKeydown(UINT message)
{
    BOOL handled = FALSE;

    if (mpDragItem)
    {
        CRect client;
        GetClientRect(&client);
        int nVirtKey = GetKeyState(VK_SHIFT);

        switch (message)
        {
        case VK_RETURN:
            if (EditFlowItem(mpDragItem))
            {
                return TRUE;
            }
            break;

        case VK_UP:
            if (nVirtKey & SHIFTED)
            {
                if (mpDragItem->rect.top + MIN_SCREEN_BOX_SIZE >= mpDragItem->rect.bottom)
                {
                    return TRUE;
                }
                mpDragItem->rect.bottom--;
            }
            else
            {
                if (mpDragItem->rect.top < 1)
                {
                    return TRUE;
                }
                mpDragItem->rect.top -= 1;
                mpDragItem->rect.bottom -= 1;
            }
            handled = TRUE;
            break;

        case VK_DOWN:
            if (nVirtKey & SHIFTED)
            {
                mpDragItem->rect.bottom++;
            }
            else
            {
                mpDragItem->rect.top += 1;
                mpDragItem->rect.bottom += 1;
            }
            handled = TRUE;
            break;

        case VK_LEFT:
            if(nVirtKey & SHIFTED)
            {
                if (mpDragItem->rect.left + MIN_SCREEN_BOX_SIZE >= mpDragItem->rect.right)
                {
                    return TRUE;
                }

                mpDragItem->rect.right--;
            }
            else
            {
                if (mpDragItem->rect.left < 1)
                {
                    return TRUE;
                }
                mpDragItem->rect.left -= 1;
                mpDragItem->rect.right -= 1;
            }
            handled = TRUE;
            break;

        case VK_RIGHT:
            if (nVirtKey & SHIFTED)
            {
                mpDragItem->rect.right++;
            }
            else
            {
                mpDragItem->rect.left += 1;
                mpDragItem->rect.right += 1;
            }
            handled = TRUE;
            break;
        }

        if (handled)
        {
            UpdateConnectionPos(mpDragItem);
            UpdateDisplaySize();
            SelectedVisible();

            InvalidateRect(&client);
        }
    }

    return handled;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::RemoveConnection(flow_item *source)
{
    //remove all connections start from specified source
    if (!source) return;

    flow_item *target;
    trigger_info *trigger;
    CArray<action_info *> *action_list;
    action_info *action;

    trigger = source->trigger_list;

    while (trigger)
    {
        //loop through trigger list
        action_list = &trigger->action_list;

        for (int count = 0; count < action_list->GetCount(); count++)
        {
            //loop through action list
            action = action_list->GetAt(count);

            if (!action->target_widget_name.IsEmpty())
            {
                target = mpScreenFlow->GetFlowItem(action->target_widget_name);
                RemoveConnection(source, target);
            }
        }
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::RemoveConnection(flow_item *source, flow_item *target)
{
    trigger_connection *remove;

    if (!source || !target) return;

    for (int index = 0; index < mConnectionList.GetCount(); index++)
    {
        remove = mConnectionList.GetAt(index);
        if ((remove->source_screen == source->screen_name) &&
            (remove->target_screen == target->screen_name))
        {
            delete remove;
            mConnectionList.RemoveAt(index);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::CheckAddConnection(flow_item *source)
{
    //add all connections start from specified source
    if (!source) return;

    flow_item *target;
    trigger_info *trigger;
    CArray<action_info *> *action_list;
    action_info *action;

    trigger = source->trigger_list;

    while (trigger)
    {
        //loop through trigger list
        action_list = &trigger->action_list;

        for (int count = 0; count < action_list->GetCount(); count++)
        {
            //loop through action list
            action = action_list->GetAt(count);

            if (!action->target_widget_name.IsEmpty())
            {
                target = mpScreenFlow->GetFlowItem(action->target_widget_name);
                CheckAddConnection(source, target);
            }
        }
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::CheckAddConnection(flow_item *source, flow_item *target)
{
    GX_BOOL founded = FALSE;
    trigger_connection *exist;

    if (!source || !target || source == target) return;

    for (int index = 0; index < mConnectionList.GetCount(); index++)
    {
        exist = mConnectionList.GetAt(index);
        if ((exist->source_screen == source->screen_name) &&
            (exist->target_screen == target->screen_name))
        {
            founded = TRUE;
        }
    }

    if (!founded)
    {
        trigger_connection *connection = new trigger_connection;
        connection->source_screen = source->screen_name;
        connection->target_screen = target->screen_name;

        CalculateConnectionPos(connection, source, target);
        mConnectionList.Add(connection);
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::CalculateConnectionPos(trigger_connection *connection, flow_item *source, flow_item *target)
{
    BOOL two_direction = FALSE;
    trigger_connection *other;
    for (int index = 0; index < mConnectionList.GetCount(); index++)
    {
        other = mConnectionList.GetAt(index);

        if (other == connection)
        {
            break;
        }

        if (other->source_screen == connection->target_screen &&
            other->target_screen == connection->source_screen)
        {
            two_direction = TRUE;
        }
    }

    CPoint source_center;
    CPoint target_center;
    CPoint line_start;
    CPoint line_end;

    source_center.x = (source->rect.left + source->rect.right) / 2;
    source_center.y = (source->rect.top + source->rect.bottom) / 2;
    target_center.x = (target->rect.left + target->rect.right) / 2;
    target_center.y = (target->rect.top + target->rect.bottom) / 2;

    float delta = (float)(target_center.x - source_center.x) / (target_center.y - source_center.y);

    if (source_center.y <= target_center.y)
    {
        if (delta <= -1)
        {
            line_start.x = source->rect.left;
            line_start.y = (source->rect.top + source->rect.bottom) / 2;
            line_end.x = target->rect.right;
            line_end.y = (target->rect.top + target->rect.bottom) / 2;
        }
        else if ((delta >= -1) && (delta < 1))
        {
            line_start.x = (source->rect.left + source->rect.right) / 2;
            line_start.y = source->rect.bottom;
            line_end.x = (target->rect.left + target->rect.right) / 2;
            line_end.y = target->rect.top;
        }
        else
        {
            line_start.x = source->rect.right;
            line_start.y = (source->rect.top + source->rect.bottom) / 2;
            line_end.x = target->rect.left;
            line_end.y = (target->rect.top + target->rect.bottom) / 2;
        }
    }
    else
    {
        if (delta >= 1)
        {
            line_start.x = source->rect.left;
            line_start.y = (source->rect.top + source->rect.bottom) / 2;
            line_end.x = target->rect.right;
            line_end.y = (target->rect.top + target->rect.bottom) / 2;
        }
        else if ((delta >= -1) && (delta < 1))
        {
            line_start.x = (source->rect.left + source->rect.right) / 2;
            line_start.y = source->rect.top;
            line_end.x = (target->rect.left + target->rect.right) / 2;
            line_end.y = target->rect.bottom;
        }
        else
        {
            line_start.x = source->rect.right;
            line_start.y = (source->rect.top + source->rect.bottom) / 2;
            line_end.x = target->rect.left;
            line_end.y = (target->rect.top + target->rect.bottom) / 2;
        }
    }

    connection->line_start = line_start;
    connection->line_end = line_end;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::UpdateConnectionPos(flow_item *moved)
{
    trigger_info *trigger = moved->trigger_list;
    flow_item *source;
    flow_item *target;
    trigger_connection *connection = NULL;

    for (int index = 0; index < mConnectionList.GetCount(); index++)
    {
        connection = mConnectionList.GetAt(index);
        if (connection->source_screen == moved->screen_name)
        {
            source = moved;
            target = mpScreenFlow->GetFlowItem(connection->target_screen);
        }
        else if (connection->target_screen == moved->screen_name)
        {
            source = mpScreenFlow->GetFlowItem(connection->source_screen);
            target = moved;
        }
        else
        {
            continue;
        }

        //calculate new connection information
        CalculateConnectionPos(connection, source, target);
    }
}

///////////////////////////////////////////////////////////////////////////////
int screen_flow_diagram_win::CheckResizeCursor(CPoint point)
{
    CRect select_outer;
    CRect select_inner;
    int drag_mode = DRAG_NONE;

    if (!mpDragItem)
    {
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        return DRAG_NONE;
    }

    select_outer =GetScaledRect(mpDragItem->rect);

    select_inner = select_outer;
    select_outer.InflateRect(5, 5);

    if (select_outer.PtInRect(point))
    {
        if (select_inner.PtInRect(point))
        {
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
            return DRAG_NONE;
        }

        // we are between two rectangles, figure out where:
        if (point.y < select_inner.top)
        {
            if (point.x < select_inner.left)
            {
                // top left corner
                ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
                drag_mode = DRAG_TOP_LEFT;
            }
            else
            {
                if (point.x >= select_inner.right)
                {
                    // top right corner
                    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
                    drag_mode = DRAG_TOP_RIGHT;
                }
                else
                {
                    // top
                    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
                    drag_mode = DRAG_TOP;
                }
            }
        }
        else
        {
            if (point.y >= select_inner.bottom)
            {
                if (point.x < select_inner.left)
                {
                    // bottom left corner
                    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
                    drag_mode = DRAG_BOTTOM_LEFT;
                }
                else
                {
                    if (point.x >= select_inner.right)
                    {
                        // bottom right corner
                        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
                        drag_mode = DRAG_BOTTOM_RIGHT;
                    }
                    else
                    {
                        // bottom
                        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
                        drag_mode = DRAG_BOTTOM;
                    }
                }
            }
            else
            {
                ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
                if (point.x < select_inner.left)
                {
                    drag_mode = DRAG_LEFT;
                }
                else
                {
                    drag_mode = DRAG_RIGHT;
                }
            }
        }
    }
    else
    {
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        drag_mode = DRAG_NONE;
    }
    return drag_mode;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scroll_helper->OnHScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_diagram_win::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

// screen_flow_edit_dlg dialog

enum screen_flow_edit_dlg_test_commands{
    TEST_EDIT_TRIGGER_LIST = 1,
    TEST_SAVE_SCREEN_FLOW_EDIT,
    TEST_CANCEL_SCREEN_FLOW_EDIT
};

IMPLEMENT_DYNAMIC(screen_flow_edit_dlg, express_dialog)

BEGIN_MESSAGE_MAP(screen_flow_edit_dlg, express_dialog)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_MESSAGE(STUDIO_TEST, &screen_flow_edit_dlg::OnTestMessage)
    ON_MESSAGE(USR_MSG_CHECK_STATE_CHANGE, &screen_flow_edit_dlg::OnShowHideScreenItem)
    ON_MESSAGE(USR_MSG_NODE_SELECT, &screen_flow_edit_dlg::OnNodeSelect)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
screen_flow_edit_dlg::screen_flow_edit_dlg(int display, CWnd* pParent /*=NULL*/)
    : express_dialog(screen_flow_edit_dlg::IDD, pParent)
{
    SetTitleText("Edit Screen Flow");
    IconId = IDB_MACRO_PLAYBACK_ICON;
    mActiveDisplay = display;

    mpScreenFlow = NULL;
    mpOldScreenFlow = NULL;
    mpScreenFlowDiagram = NULL;
    mpButtonFrame = NULL;
}

///////////////////////////////////////////////////////////////////////////////
screen_flow_edit_dlg::~screen_flow_edit_dlg()
{
    if (mpScreenFlowDiagram)
    {
        delete mpScreenFlowDiagram;
    }

    if (mpButtonFrame)
    {
        delete mpButtonFrame;
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_edit_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    if (pDX->m_bSaveAndValidate)
    {
        // Delete old screen flow
        if (mpOldScreenFlow)
        {
            delete mpOldScreenFlow;
        }
    }
    DDX_Control(pDX, IDC_SCREEN_ITEM_TREE, mScreenItemView);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_edit_dlg::OnCancel()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        // Revert back to old scren flow.
        project->mDisplays[mActiveDisplay].screenflow = mpOldScreenFlow;
        if (mpOldScreenFlow)
        {
            mpOldScreenFlow->RebuildAnimationIdDictionary(mActiveDisplay);
        }
    }

    // Delete new screen flow
    if (mpScreenFlow)
    {
        delete mpScreenFlow;
		mpScreenFlow = NULL;
    }

    express_dialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_edit_dlg::InitScreenFlow(int diagram_width, int diagram_height)
{
    studiox_project *project = GetOpenProject();
    CArray<widget_info *> screen_list;
    if (project)
    {
        if (project->mDisplays[mActiveDisplay].screenflow)
        {
            //Save old screen flow
            mpOldScreenFlow = new screen_flow(*project->mDisplays[mActiveDisplay].screenflow);
        }
        else
        {
            project->mDisplays[mActiveDisplay].screenflow = new screen_flow;
        }
        mpScreenFlow = project->mDisplays[mActiveDisplay].screenflow;

        mpScreenFlow->GetFlowListCount();

        GetProjectView()->GetTopLevelWidgetList(mActiveDisplay, &screen_list);

        widget_info *screen;

        for (int index = 0; index < screen_list.GetCount(); index++)
        {
            screen = screen_list.GetAt(index);

            if (!screen->is_template)
            {
                mpScreenFlow->CheckAddFlowItem(screen->app_name, diagram_width, diagram_height);
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_edit_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    CRect client;
    CRect listsize;
    CRect size;

    GetClientRect(&client);

    //Calculate screen flow diagram window size
    mScreenItemView.GetWindowRect(&listsize);
    ScreenToClient(&listsize);

    size.top = listsize.top;
    size.left = listsize.right + 5;
    size.right = client.right - 10;
    size.bottom = listsize.bottom;

    // init screen flow
    InitScreenFlow(size.Width(), size.Height());

    mpScreenFlowDiagram = new screen_flow_diagram_win(mActiveDisplay, mpScreenFlow);
    mpScreenFlowDiagram->Create(target_class_name, _T("screen flow diagram win"),
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER |WS_TABSTOP, CRect(0, 0, 0, 0), this, 0);
    mpScreenFlowDiagram->MoveWindow(&size);
    mpScreenFlowDiagram->ShowWindow(SW_SHOW);
    mpScreenFlowDiagram->UpdateDisplaySize();

    mScreenItemView.InitImageList();
    PopulateScreenItems();

    // create button frame
    GetDlgItem(IDC_INCLUDE_SCREEN_PROMPT)->GetWindowRect(&listsize);
    ScreenToClient(&listsize);

    size.top = listsize.top;
    size.left = listsize.right + 5;
    size.right = client.right - 10;
    size.bottom = listsize.bottom;

    mpButtonFrame = new screen_flow_button_frame(mpScreenFlow, this);
    mpButtonFrame->CreateEx(WS_EX_CONTROLPARENT, target_class_name, _T("screen flow button frame"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 0);
    mpButtonFrame->SetWindowPos(&mScreenItemView, size.left, size.top, size.Width(), size.Height(), 0);
    mpButtonFrame->ShowWindow(SW_SHOW);

    //Add cancel button
    AddCancelButton();

    //add save button
    AddSaveButton();

    PositionChildren();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_edit_dlg::PositionChildren()
{
    CRect client;
    CRect size;

    GetClientRect(&client);

    if (!mpScreenFlowDiagram)
    {
        return;
    }

    // Calculate screen flow diagram window size
    mScreenItemView.GetWindowRect(&size);
    ScreenToClient(&size);
    
    size.bottom = client.bottom - m_status_bar_height - 10;

    // position screen item view
    mScreenItemView.MoveWindow(&size);

    size.left = size.right + 5;
    size.right = client.right - 10;

    // position screen flow
    mpScreenFlowDiagram->MoveWindow(&size);

    // position button frame
    GetDlgItem(IDC_INCLUDE_SCREEN_PROMPT)->GetWindowRect(&size);
    ScreenToClient(&size);

    size.left = size.right + 5;
    size.right = client.right - 10;

    mpButtonFrame->MoveWindow(&size);

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_edit_dlg::OnSize(UINT nType, int cx, int cy)
{
    express_dialog::OnSize(nType, cx, cy);

    PositionChildren();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow_edit_dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    express_dialog::OnGetMinMaxInfo(lpMMI);

    lpMMI->ptMinTrackSize.x = MIN_SCREEN_FLOW_EDIT_DLG_WIDTH;
    lpMMI->ptMinTrackSize.y = MIN_SCREEN_FLOW_EDIT_DLG_HEIGHT;
}

///////////////////////////////////////////////////////////////////////////////
VOID screen_flow_edit_dlg::PopulateScreenItems()
{
    if (!mpScreenFlow)
    {
        return;
    }

    flow_item *flow;
    int state;
    HTREEITEM hParent;
    HTREEITEM hItem;

    studiox_project *project = GetOpenProject();

    folder_info *folder = project->mDisplays[mActiveDisplay].GetFirstChildFolder();
    widget_info *info;

    while (folder)
    {
        hParent = mScreenItemView.InsertItem(folder->folder_name);

        info = folder->GetFirstChildWidget();

        while (info)
        {
            flow = mpScreenFlow->GetFlowItem(info->app_name);

            if (flow)
            {
                if (flow->enabled)
                {
                    state = CUSTOM_TV_STATE_CHECKED;
                }
                else
                {
                    state = CUSTOM_TV_STATE_UNCHECKED;
                }

                hItem = mScreenItemView.InsertItem(flow->screen_name, hParent);
                mScreenItemView.SetItemState(hItem, state);
            }

            info = info->GetNextWidgetInfo();
        }

        mScreenItemView.Expand(hParent, TVE_EXPAND);
        state = mScreenItemView.GetChildrenState(hParent);
        mScreenItemView.SetItemState(hParent, state);

        folder = folder->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
LRESULT screen_flow_edit_dlg::OnShowHideScreenItem(WPARAM wParam, LPARAM lParam)
{
    HTREEITEM hItem = (HTREEITEM)lParam;

    UINT state = wParam;

    CString name = mScreenItemView.GetItemText(hItem);
    HTREEITEM parent = mScreenItemView.GetParentItem(hItem);
    flow_item *item;

    BOOL enabled;

    if (state == CUSTOM_TV_STATE_CHECKED)
    {
        enabled = TRUE;
    }
    else
    {
        enabled = FALSE;
    }

    CRect client;
    mpScreenFlowDiagram->GetClientRect(&client);

    if (parent)
    {
        //top level screen
        item = mpScreenFlow->GetFlowItem(name);

        if (item)
        {
            item->enabled = enabled;

            if (enabled)
            {
                mpScreenFlow->UpdateFlowItemRect(item, client.Width(), client.Height());
                mpScreenFlowDiagram->UpdateConnectionPos(item);
                mpScreenFlowDiagram->SelectFlowItem(item);
            }
            else
            {
                mpScreenFlowDiagram->HideFlowItem(item);
            }
        }
    }
    else
    {
        //folder
        studiox_project *project = GetOpenProject();

        if (!project)
        {
            return -1;
        }
        folder_info *folder = project->FindFolderInfo(mActiveDisplay, name);
        widget_info *info;

        if (folder)
        {
            info = folder->GetFirstChildWidget();


            while (info)
            {
                item = mpScreenFlow->GetFlowItem(info->app_name);

                if (item)
                {
                    item->enabled = enabled;

                    if (enabled)
                    {
                        mpScreenFlow->UpdateFlowItemRect(item, client.Width(), client.Height());
                        mpScreenFlowDiagram->UpdateConnectionPos(item);
                    }
                    else
                    {
                        mpScreenFlowDiagram->HideFlowItem(item);
                    }
                }
                info = info->GetNextWidgetInfo();
            }
        }
    }

    mpScreenFlowDiagram->UpdateDisplaySize();
    mpScreenFlowDiagram->Invalidate();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT screen_flow_edit_dlg::OnNodeSelect(WPARAM wParam, LPARAM lParam)
{
    HTREEITEM hItem = (HTREEITEM)lParam;
    HTREEITEM parent = mScreenItemView.GetParentItem(hItem);

    if (parent)
    {
        //top level screen
        CString name = mScreenItemView.GetItemText(hItem);
        flow_item *item = mpScreenFlow->GetFlowItem(name);
        mpScreenFlowDiagram->SelectFlowItem(item);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow_edit_dlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN && GetFocus() == &mScreenItemView)
        {
            HTREEITEM hItem = mScreenItemView.GetSelectedItem();
            HTREEITEM parent = mScreenItemView.GetParentItem(hItem);

            if (parent)
            {
                if (mScreenItemView.GetItemCheckState(hItem) == CUSTOM_TV_STATE_CHECKED)
                {
                    mpScreenFlowDiagram->HandleKeydown(VK_RETURN);
                }
            }
            else
            {
                if (mScreenItemView.GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED)
                {
                    mScreenItemView.Expand(hItem, TVE_COLLAPSE);
                }
                else
                {
                    mScreenItemView.Expand(hItem, TVE_EXPAND);
                }
            }

            return TRUE;
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT screen_flow_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    flow_item *item;
    CString screen_name;
    //int scale;

    switch (wParam)
    {
    case TEST_EDIT_TRIGGER_LIST:
        screen_name = GetTestingParam(1);
        item = mpScreenFlow->GetFlowItem(screen_name);
        if (item && mpScreenFlowDiagram)
        {
            mpScreenFlowDiagram->EditFlowItem(item);
/*            scale = mpScreenFlowDiagram->GetScale();
            mpScreenFlowDiagram->SendMessage(WM_RBUTTONDOWN, MK_RBUTTON,
                MAKELPARAM(MulDiv(item->rect.left, scale, 100), MulDiv(item->rect.top, scale, 100)));
            mpScreenFlowDiagram->SendMessage(WM_RBUTTONUP, MK_RBUTTON,
                MAKELPARAM(MulDiv(item->rect.left, scale, 100), MulDiv(item->rect.top, scale, 100)));
*/
        }
        break;

    case TEST_SAVE_SCREEN_FLOW_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_SCREEN_FLOW_EDIT:
        OnCancel();
        break;
    }

    return 0;
}
