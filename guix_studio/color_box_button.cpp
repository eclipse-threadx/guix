#include "studiox_includes.h"

extern CFont NormalFont;

BEGIN_MESSAGE_MAP(color_box_button, CButton)
    ON_WM_PAINT()
    ON_WM_ENABLE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
color_box_button::color_box_button()
{
    m_boxcolor = 0;
    m_hover = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
color_box_button::~color_box_button()
{
}

///////////////////////////////////////////////////////////////////////////////
void color_box_button::SetBoxColor(COLORREF color)
{
    m_boxcolor = color;
}

///////////////////////////////////////////////////////////////////////////////
void color_box_button::OnPaint()
{
    CPaintDC dc(this);

    COLORREF color_border = GetSysColor(COLOR_ACTIVEBORDER);
    COLORREF color_highlight = GetSysColor(COLOR_HIGHLIGHT);

    CRect      rect;
    CString    text;
    RectF      rectF;
    int        state;

    GetClientRect(&rect);

    CDC        dcMem;
    HBITMAP    hbmMem;
    HANDLE     hOld;

    /* Flush canvas memory to off-screen DC. */
    dcMem.CreateCompatibleDC(&dc);
    hbmMem = CreateCompatibleBitmap(dc.GetSafeHdc(), rect.Width(), rect.Height());
    hOld = SelectObject(dcMem, hbmMem);

    Graphics   g(dcMem);

    dc.SelectObject(&NormalFont);
    Gdiplus::Font  font(dc.GetSafeHdc());
    StringFormat   stringFormat;

    // Center-justify each line of text.
    stringFormat.SetAlignment(StringAlignmentCenter);

    // Center the block of text (top to bottom) in the rectangle.
    stringFormat.SetLineAlignment(StringAlignmentCenter);


    rectF.X = (REAL)rect.left;
    rectF.Y = (REAL)rect.top;
    rectF.Width = (REAL)rect.Width() - 1;
    rectF.Height = (REAL)rect.Height() - 1;

    GetWindowText(text);

    state = GetState();
    if (state & BST_PUSHED)
    {
        SolidBrush selectBrush(Color(20, GetRValue(color_highlight), GetGValue(color_highlight), GetBValue(color_highlight)));
        Pen        selectPen(Color(128, GetRValue(color_highlight), GetGValue(color_highlight), GetBValue(color_highlight)));

        g.FillRectangle(&selectBrush, rectF);
        g.DrawRectangle(&selectPen, rectF);
    }
    else if ((state & BST_FOCUS) || m_hover)
    {
        SolidBrush selectBrush(Color(20, GetRValue(color_highlight), GetGValue(color_highlight), GetBValue(color_highlight)));
        Pen        selectPen(Color(GetRValue(color_highlight), GetGValue(color_highlight), GetBValue(color_highlight)));

        g.FillRectangle(&selectBrush, rectF);
        g.DrawRectangle(&selectPen, rectF);
    }
    else
    {
        SolidBrush normalBrush(Color(20, GetRValue(color_border), GetGValue(color_border), GetBValue(color_border)));
        Pen normalPen(Color(GetRValue(color_border), GetGValue(color_border), GetBValue(color_border)));

        g.FillRectangle(&normalBrush, rectF);
        g.DrawRectangle(&normalPen, rectF);
    }

    // Draw color box
    rectF.Inflate(-1, -1);
    SolidBrush fill_brush(Color(GetRValue(m_boxcolor), GetGValue(m_boxcolor), GetBValue(m_boxcolor)));
    Pen line_pen(Color(0, 0, 0));
    g.FillRectangle(&fill_brush, rectF);
    g.DrawRectangle(&line_pen, rectF);

    /* Transfer the off-screen DC to the screen. */
    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);

    /* Free-up the off-screen DC.  */
    SelectObject(dcMem, hOld);
    DeleteObject(hbmMem);
    DeleteDC(dcMem);
}

///////////////////////////////////////////////////////////////////////////////
void color_box_button::OnEnable(BOOL bEnable)
{
    CButton::OnEnable(bEnable);

    Invalidate();
    UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////
void color_box_button::OnMouseLeave()
{
    m_hover = FALSE;
    Invalidate();

    CButton::OnMouseLeave();
}

///////////////////////////////////////////////////////////////////////////////
void color_box_button::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_hover)
    {
        m_hover = TRUE;
    }
    CButton::OnMouseMove(nFlags, point);
}
