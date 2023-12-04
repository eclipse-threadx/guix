
#include "stdafx.h"
#include "ScrollHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CScrollHelper /////////////////////////////////////////////////////////////////////

CScrollHelper::CScrollHelper()
{
    m_attachWnd   = NULL;
    m_pageSize    = CSize(0,0);
    m_displaySize = CSize(0,0);
    m_scrollPos   = CSize(0,0);
}

CScrollHelper::~CScrollHelper()
{
    DetachWnd();
}

void CScrollHelper::AttachWnd(CWnd* pWnd)
{
    m_attachWnd = pWnd;
}

void CScrollHelper::DetachWnd()
{
    m_attachWnd = NULL;
}

void CScrollHelper::SetDisplaySize(int displayWidth, int displayHeight)
{
    m_displaySize = CSize(displayWidth, displayHeight);

    if ( m_attachWnd != NULL && ::IsWindow(m_attachWnd->m_hWnd) )
        UpdateScrollInfo();
}

const CSize& CScrollHelper::GetDisplaySize() const
{
    return m_displaySize;
}

const CSize& CScrollHelper::GetScrollPos() const
{
    return m_scrollPos;
}


const CSize& CScrollHelper::GetPageSize() const
{
    return m_pageSize;
}

void CScrollHelper::SetScrollPos(int bar, int newpos, BOOL redraw)
{
    if (m_attachWnd == NULL)
        return;

    CRect rect;
    GetClientRectSB(m_attachWnd, rect);
    CSize windowSize(rect.Width(), rect.Height());

    if ((bar == SB_HORZ) && (windowSize.cx < m_displaySize.cx))
    {
        int deltaPos = newpos - m_scrollPos.cx;
        m_scrollPos.cx += deltaPos;
        m_attachWnd->SetScrollPos(SB_HORZ, m_scrollPos.cx, redraw);
        if (redraw)
        {
            m_attachWnd->ScrollWindow(0, -deltaPos);
        }
    }

    if ((bar == SB_VERT) && (windowSize.cy < m_displaySize.cy))
    {
        int deltaPos = newpos - m_scrollPos.cy;
        m_scrollPos.cy += deltaPos;
        m_attachWnd->SetScrollPos(SB_VERT, m_scrollPos.cy, redraw);

        if (redraw)
        {
            m_attachWnd->ScrollWindow(0, -deltaPos);
        }
    }
}

void CScrollHelper::ScrollToOrigin(bool scrollLeft, bool scrollTop)
{
    if ( m_attachWnd == NULL )
        return;

    if ( scrollLeft )
    {
        if ( m_displaySize.cx > 0 && m_pageSize.cx > 0 && m_scrollPos.cx > 0 )
        {
            int deltaPos = -m_scrollPos.cx;
            m_scrollPos.cx += deltaPos;
            m_attachWnd->SetScrollPos(SB_HORZ, m_scrollPos.cx, TRUE);
            m_attachWnd->ScrollWindow(-deltaPos, 0);
        }
    }

    if ( scrollTop )
    {
        if ( m_displaySize.cy > 0 && m_pageSize.cy > 0 && m_scrollPos.cy > 0 )
        {
            int deltaPos = -m_scrollPos.cy;
            m_scrollPos.cy += deltaPos;
            m_attachWnd->SetScrollPos(SB_VERT, m_scrollPos.cy, TRUE);
            m_attachWnd->ScrollWindow(0, -deltaPos);
        }
    }
}

void CScrollHelper::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if ( m_attachWnd == NULL )
        return;

    const int lineOffset = 60;

    // Compute the desired change or delta in scroll position.
    int deltaPos = 0;
    switch( nSBCode )
    {
    case SB_LINELEFT:
        // Left scroll arrow was pressed.
        deltaPos = -lineOffset;
        break;

    case SB_LINERIGHT:
        // Right scroll arrow was pressed.
        deltaPos = lineOffset;
        break;

    case SB_PAGELEFT:
        // User clicked inbetween left arrow and thumb.
        deltaPos = -m_pageSize.cx;
        break;

    case SB_PAGERIGHT:
        // User clicked inbetween thumb and right arrow.
        deltaPos = m_pageSize.cx;
        break;

    case SB_THUMBTRACK:
        // Scrollbar thumb is being dragged.
        deltaPos = Get32BitScrollPos(SB_HORZ, pScrollBar) - m_scrollPos.cx;
        break;

    case SB_THUMBPOSITION:
        // Scrollbar thumb was released.
        deltaPos = Get32BitScrollPos(SB_HORZ, pScrollBar) - m_scrollPos.cx;
        break;

    default:
        // We don't process other scrollbar messages.
        return;
    }

    // Compute the new scroll position.
    int newScrollPos = m_scrollPos.cx + deltaPos;

    // If the new scroll position is negative, we adjust
    // deltaPos in order to scroll the window back to origin.
    if ( newScrollPos < 0 )
        deltaPos = -m_scrollPos.cx;

    // If the new scroll position is greater than the max scroll position,
    // we adjust deltaPos in order to scroll the window precisely to the
    // maximum position.
    int maxScrollPos = m_displaySize.cx - m_pageSize.cx;
    if ( newScrollPos > maxScrollPos )
        deltaPos = maxScrollPos - m_scrollPos.cx;

    // Scroll the window if needed.
    if ( deltaPos != 0 )
    {
        m_scrollPos.cx += deltaPos;
        m_attachWnd->SetScrollPos(SB_HORZ, m_scrollPos.cx, TRUE);
        m_attachWnd->ScrollWindow(-deltaPos, 0);
    }
}

void CScrollHelper::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if ( m_attachWnd == NULL )
        return;

    const int lineOffset = 60;

    // Compute the desired change or delta in scroll position.
    int deltaPos = 0;
    switch( nSBCode )
    {
    case SB_LINEUP:
        // Up arrow button on scrollbar was pressed.
        deltaPos = -lineOffset;
        break;

    case SB_LINEDOWN:
        // Down arrow button on scrollbar was pressed.
        deltaPos = lineOffset;
        break;

    case SB_PAGEUP:
        // User clicked inbetween up arrow and thumb.
        deltaPos = -m_pageSize.cy;
        break;

    case SB_PAGEDOWN:
        // User clicked inbetween thumb and down arrow.
        deltaPos = m_pageSize.cy;
        break;

    case SB_THUMBTRACK:
        // Scrollbar thumb is being dragged.
        deltaPos = Get32BitScrollPos(SB_VERT, pScrollBar) - m_scrollPos.cy;
        break;

    case SB_THUMBPOSITION:
        // Scrollbar thumb was released.
        deltaPos = Get32BitScrollPos(SB_VERT, pScrollBar) - m_scrollPos.cy;
        break;

    default:
        // We don't process other scrollbar messages.
        return;
    }

    // Compute the new scroll position.
    int newScrollPos = m_scrollPos.cy + deltaPos;

    // If the new scroll position is negative, we adjust
    // deltaPos in order to scroll the window back to origin.
    if ( newScrollPos < 0 )
        deltaPos = -m_scrollPos.cy;

    // If the new scroll position is greater than the max scroll position,
    // we adjust deltaPos in order to scroll the window precisely to the
    // maximum position.
    int maxScrollPos = m_displaySize.cy - m_pageSize.cy;
    if ( newScrollPos > maxScrollPos )
        deltaPos = maxScrollPos - m_scrollPos.cy;

    // Scroll the window if needed.
    if ( deltaPos != 0 )
    {
        m_scrollPos.cy += deltaPos;
        m_attachWnd->SetScrollPos(SB_VERT, m_scrollPos.cy, TRUE);
        m_attachWnd->ScrollWindow(0, -deltaPos);
    }
}

BOOL CScrollHelper::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if ( m_attachWnd == NULL )
        return FALSE;

    // Don't do anything if the vertical scrollbar is not enabled.
    int scrollMin = 0, scrollMax = 0;
    m_attachWnd->GetScrollRange(SB_VERT, &scrollMin, &scrollMax);
    if ( scrollMin == scrollMax )
        return FALSE;

    // Compute the number of scrolling increments requested.
    int numScrollIncrements = abs(zDelta) / WHEEL_DELTA;

    // Each scrolling increment corresponds to a certain number of
    // scroll lines (one scroll line is like a SB_LINEUP or SB_LINEDOWN).
    // We need to query the system parameters for this value.
    int numScrollLinesPerIncrement = 0;
    ::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &numScrollLinesPerIncrement, 0);

    // Check if a page scroll was requested.
    if ( numScrollLinesPerIncrement == WHEEL_PAGESCROLL )
    {
        // Call the vscroll message handler to do the work.
        OnVScroll(zDelta > 0 ? SB_PAGEUP : SB_PAGEDOWN, 0, NULL);
        return TRUE;
    }

    // Compute total number of lines to scroll.
    int numScrollLines = numScrollIncrements * numScrollLinesPerIncrement;

    // Adjust numScrollLines to slow down the scrolling a bit more.
    numScrollLines = max(numScrollLines/3, 1);

    // Do the scrolling.
    for(int i = 0; i < numScrollLines; ++i)
    {
        // Call the vscroll message handler to do the work.
        OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, NULL);
    }

    return TRUE;
}

void CScrollHelper::OnSize(UINT nType, int cx, int cy)
{
    UpdateScrollInfo();
}

BOOL CScrollHelper::Scroll(int bar, int delta)
{
    int pos;
    int new_pos;
    int max_pos;

    if (bar == SB_VERT)
    {
        pos = m_scrollPos.cy;
        max_pos = m_displaySize.cy - m_pageSize.cy;

    }
    else if (bar == SB_HORZ)
    {
        pos = m_scrollPos.cx;
        max_pos = m_displaySize.cx - m_pageSize.cx;
    }
    else
    {
        return FALSE;
    }

    new_pos = pos + delta;

    if (delta > 0)
    {
        if (new_pos > max_pos)
        {
            new_pos = max_pos;
        }
    }
    else
    {
        if (new_pos < 0)
        {
            new_pos = 0;
        }
    }

    if (new_pos != pos)
    {
        SetScrollPos(bar, new_pos, TRUE);
        return TRUE;
    }

    return FALSE;
}

int CScrollHelper::Get32BitScrollPos(int bar, CScrollBar* pScrollBar)
{
    // Code below is from MSDN Article ID 152252, "How To Get
    // 32-bit Scroll Position During Scroll Messages".

    // First determine if the user scrolled a scroll bar control
    // on the window or scrolled the window itself.
    ASSERT( m_attachWnd != NULL );
    HWND hWndScroll;
    if ( pScrollBar == NULL )
        hWndScroll = m_attachWnd->m_hWnd;
    else
        hWndScroll = pScrollBar->m_hWnd;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_TRACKPOS;
    ::GetScrollInfo(hWndScroll, bar, &si);

    int scrollPos = si.nTrackPos;

    return scrollPos;
}

void CScrollHelper::UpdateScrollInfo()
{
    if ( m_attachWnd == NULL )
        return;

    // Get the width/height of the attached wnd that includes the area
    // covered by the scrollbars (if any). The reason we need this is
    // because when scrollbars are present, both cx/cy and GetClientRect()
    // when accessed from OnSize() do not include the scrollbar covered
    // areas. In other words, their values are smaller than what you would
    // expect.
    CRect rect;
    GetClientRectSB(m_attachWnd, rect);
    CSize windowSize(rect.Width(), rect.Height());

    // Update horizontal scrollbar.
    CSize deltaPos(0,0);
    UpdateScrollBar(SB_HORZ, windowSize.cx, m_displaySize.cx,
        m_pageSize.cx, m_scrollPos.cx, deltaPos.cx);

    // Update vertical scrollbar.
    UpdateScrollBar(SB_VERT, windowSize.cy, m_displaySize.cy,
        m_pageSize.cy, m_scrollPos.cy, deltaPos.cy);

    // See if we need to scroll the window back in place.
    // This is needed to handle the case where the scrollbar is
    // moved all the way to the right for example, and controls
    // at the left side disappear from the view. Then the user
    // resizes the window wider until scrollbars disappear. Without
    // this code below, the controls off the page will be gone forever.
    if ( deltaPos.cx != 0 || deltaPos.cy != 0 )
    {
        m_attachWnd->ScrollWindow(deltaPos.cx, deltaPos.cy);
    }                                                        
}

void CScrollHelper::UpdateScrollBar(int bar, int windowSize, int displaySize,
                                    LONG& pageSize, LONG& scrollPos, LONG& deltaPos)
{
    int scrollMax = 0;
    deltaPos = 0;
    if ( windowSize < displaySize )
    {
        scrollMax = displaySize - 1;
        if ( pageSize > 0 && scrollPos > 0 )
        {
            // Adjust the scroll position when the window size is changed.
            scrollPos = (LONG)(1.0 * scrollPos * windowSize / pageSize);
        }
        pageSize = windowSize;
        scrollPos = min(scrollPos, displaySize - pageSize);
        deltaPos = m_attachWnd->GetScrollPos(bar) - scrollPos;
    }
    else
    {
        // Force the scrollbar to go away.
        pageSize = 0;
        scrollPos = 0;
        deltaPos = m_attachWnd->GetScrollPos(bar);
    }

    SCROLLINFO si;
    memset(&si, 0, sizeof(SCROLLINFO));
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_ALL;    // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
    si.nMin   = 0;
    si.nMax   = scrollMax;
    si.nPage  = pageSize;
    si.nPos   = scrollPos;
    m_attachWnd->SetScrollInfo(bar, &si, TRUE);
}

// Helper function to get client rect with possible
// modification by adding scrollbar width/height.
void CScrollHelper::GetClientRectSB(CWnd* pWnd, CRect& rect)
{
    ASSERT(pWnd != NULL);

    //CRect winRect;
    //pWnd->GetWindowRect(&winRect);
    //pWnd->ScreenToClient(&winRect);

    pWnd->GetClientRect(&rect);

    int cxSB = ::GetSystemMetrics(SM_CXVSCROLL);
    int cySB = ::GetSystemMetrics(SM_CYHSCROLL);

    int style = pWnd->GetStyle();
    if (style & WS_HSCROLL)
    {
        rect.bottom += cySB;
    }

    if (style & WS_VSCROLL)
    {
        rect.right += cxSB;
    }

    if (rect.Width() < m_displaySize.cx)
    {
        rect.bottom -= cySB;
    }

    if (rect.Height() < m_displaySize.cy)
    {
        rect.right -= cxSB;

        if (rect.Width() < m_displaySize.cx)
        {
            rect.bottom -= cySB;
        }
    }
}
// END

