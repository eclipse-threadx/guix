#include "studiox_includes.h"
#include "wait_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(wait_dialog, CDialog)

// wait_dialog
BEGIN_MESSAGE_MAP(wait_dialog, CDialog)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_SHOWWINDOW()
    ON_MESSAGE(USR_MSG_DIALOG_VISIBLE, OnDialogVisible)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
wait_dialog::wait_dialog(int width, int height, const char *msg, CWnd *parent)
    : CDialog(wait_dialog::IDD, parent)
{
    m_width = width;
    m_height = height;
    mMessage = CString(msg);
    mIconId = 0;
    m_work_thread = 0;
}

///////////////////////////////////////////////////////////////////////////////
wait_dialog::~wait_dialog()
{
}

///////////////////////////////////////////////////////////////////////////////
int wait_dialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
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
    CenterWindow();
    GetClientRect(&client);

    SetWindowText(_T("Wait Dialog"));

    size = client;
    size.top += 44;
    size.right -= 4;
    size.left += 4;
    size.bottom -= 4;
    wait_msg.Create(mMessage, WS_CHILD|WS_VISIBLE|SS_CENTER, size, this);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////
void wait_dialog::PaintTitleBar(CDC *dc)
{
    CRect size;
    CDC dcMemory;
    CBitmap fillmap;
    BITMAP bm;

    // fill the title bar:
    fillmap.LoadBitmap(IDB_HEADER_BACKGROUND);

    // go through hoops to get bitmap width:
    fillmap.GetObject(sizeof(BITMAP), &bm);
    int width = bm.bmWidth;
    int height = bm.bmHeight;

    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&fillmap);

    GetClientRect(&size);
    int xpos = size.left;
    while(xpos < size.right)
    {
        dc->BitBlt(xpos, size.top, width, height, &dcMemory, 0, 0, SRCCOPY);
        xpos += width;
    }

    // draw the icon in top-left corner:
    fillmap.DeleteObject();

    if (mIconId)
    {
        fillmap.LoadBitmap(mIconId);
        fillmap.GetObject(sizeof(BITMAP), &bm);
        dcMemory.SelectObject(fillmap);
        dc->BitBlt(size.left + 4, size.top + 4, bm.bmWidth, bm.bmHeight, &dcMemory, 0, 0, SRCCOPY);
    }

    // draw the caption
    dc->SetTextColor(RGB(240, 240, 240));
    dc->SetBkMode(TRANSPARENT);
    dc->TextOut(size.left + bm.bmWidth + 8, size.top + 4, _T("Please Wait..."));
}

///////////////////////////////////////////////////////////////////////////////
void wait_dialog::OnPaint()
{
    CDC *dc = GetDC();
    CDialog::OnPaint();
    PaintTitleBar(dc);
    ReleaseDC(dc);
}

///////////////////////////////////////////////////////////////////////////////
void wait_dialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    PostMessage(USR_MSG_DIALOG_VISIBLE, 0, 0);
}

LRESULT wait_dialog::OnDialogVisible(WPARAM wParam, LPARAM lParam)
{
    if (m_work_thread)
    {
        ResumeThread(m_work_thread);
    }
    return 0;
}
