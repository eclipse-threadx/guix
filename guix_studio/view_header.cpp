
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(view_header, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

extern CFont ViewHeaderFont;
extern CFont NormalFont;


view_header ::view_header(char *title, int IconId)
{
    m_title = CString(title);
    m_icon_id = IconId;

    BITMAP bmp;

    int dpi = GetSystemDPI();

    if (m_icon_id)
    {
        if (m_left_bmp.LoadBitmap(m_icon_id))
        {
            m_left_bmp.GetBitmap(&bmp);
            m_icon_src_width = bmp.bmWidth;
            m_icon_src_height = bmp.bmHeight;
            m_icon_width = MulDiv(m_icon_src_width, dpi, DEFAULT_DPI_96);
            m_icon_height = MulDiv(m_icon_src_height, dpi, DEFAULT_DPI_96);
        }
    }
}

void view_header::FillHeader(CDC *dc, int y, int xstart, int xend)
{
    CDC dcMemory;
    CBitmap fillmap;
    fillmap.LoadBitmap(IDB_HEADER_BACKGROUND);
    BITMAP bm;

    // go through hoops to get bitmap width:
    fillmap.GetObject(sizeof(BITMAP), &bm);
    int width = bm.bmWidth;

    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&fillmap);

    CRect client;
    GetClientRect(&client);

    while(xstart < xend)
    {
        dc->StretchBlt(xstart, y, width, client.Height(), &dcMemory, 0, 0, width, bm.bmHeight, SRCCOPY);
        xstart += width;
    }
}

void view_header::PaintIcon(CDC *dc, int x, int y)
{
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&m_left_bmp);
    dc->StretchBlt(x, y, m_icon_width, m_icon_height, &dcMemory, 0, 0, m_icon_src_width, m_icon_src_height, SRCCOPY);
}

void view_header::PaintRightIcon(CDC *dc, int iconId, CRect &size)
{
    CBitmap map;
    BITMAP bm;

    map.LoadBitmap(iconId);

    // go through hoops to get bitmap width:
    map.GetObject(sizeof(BITMAP), &bm);
    int width = bm.bmWidth;
    int right = size.right - (width + 8);

    int vspace = size.bottom - size.top;
    vspace -= bm.bmHeight;
    vspace /= 2;

    CDC dcMemory;
    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&map);
    dc->BitBlt(right, size.top + vspace, width, bm.bmHeight, &dcMemory, 0, 0, SRCCOPY);
}

void view_header::OnPaint()
{
    CFont *old_font;
    CRect size;

    GetClientRect(&size);
    CDC *dc = GetDC();
    CWnd::OnPaint();
    FillHeader(dc, size.top, size.left, size.right);

    int vspace = size.bottom - size.top;
    vspace -= m_icon_height;
    vspace /= 2;

    if (m_icon_id)
    {
        PaintIcon(dc, size.left + 8, size.top + vspace);
        size.left += m_icon_width + 8;

        PaintRightIcon(dc, IDB_MINUS, size);
    }

    dc->SetTextColor(RGB(255, 255, 255));
    dc->SetBkMode(TRANSPARENT);
    old_font = dc->SelectObject(&ViewHeaderFont);
    size.left += 6;
    dc->DrawText(m_title, &size, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    dc->SelectObject(old_font);
    ReleaseDC(dc);
}

