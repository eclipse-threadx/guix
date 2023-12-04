// recent_project_win.cpp : implementation file
//

#include "studiox_includes.h"
#include "recent_project_win.h"
extern INI_INFO StudioXIni;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NEW_BUTTON_HEIGHT 36
#define NEW_BUTTON_WIDTH 190

extern CString target_class_name;
extern CFont MediumFont;
    

BEGIN_MESSAGE_MAP(recent_project_win, express_dialog)
 ON_WM_CREATE()
 ON_WM_SHOWWINDOW()
 ON_BN_CLICKED(IDC_CREATE_PROJECT, OnBnClickedCreateProject)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
recent_project_win::recent_project_win(int width, int height, CWnd* pParent /*=NULL*/)
	: express_dialog(width, height)
{
    IconId = IDB_RECENT_PROJECT;
    SetTitleText("Recent Projects");
}

///////////////////////////////////////////////////////////////////////////////
recent_project_win::~recent_project_win()
{
}

///////////////////////////////////////////////////////////////////////////////
CRect recent_project_win::GetCreateNewProjectButtonSize(CRect &parentSize)
{
    CRect size;

    int dpi = GetSystemDPI();
    int text_scaler = GetTextScaler();

    size.bottom = parentSize.bottom - 4;
    size.top = size.bottom - GetScaledValue(NEW_BUTTON_HEIGHT, dpi, text_scaler);
    size.right = parentSize.right - 4;
    size.left = size.right - GetScaledValue(NEW_BUTTON_WIDTH, dpi, text_scaler);

    return size;
}

///////////////////////////////////////////////////////////////////////////////
int recent_project_win::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CRect client;
    CRect size;

    express_dialog::OnCreate(lpCreateStruct);
    GetClientRect(&client);

    int space = MulDiv(10, GetSystemDPI(), DEFAULT_DPI_96);;
    size.left = client.left + space;
    size.right = client.right - space;
    size.top = client.top + m_title_bar_height + space;
    size.bottom = client.bottom - m_status_bar_height - space;

    mRecentListFrame.Create(target_class_name, _T("Recent Projects"),
        WS_VISIBLE | WS_CHILD | WS_TABSTOP, size, this, 0);

    size = GetCreateNewProjectButtonSize(client);
    new_button.Create(_T("Create New Project"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | WS_TABSTOP, size, this, IDC_CREATE_PROJECT);
    new_button.LoadBitmaps(IDB_NEW_PROJECT_NORMAL, IDB_NEW_PROJECT_PRESSED);
    new_button.SetWindowText(_T("Create New Project..."));
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void recent_project_win::OnShowWindow(BOOL Show, UINT status)
{
    if (Show)
    {
        mRecentListFrame.UpdateRecentList();
    }
    CWnd::OnShowWindow(Show, status);
}

///////////////////////////////////////////////////////////////////////////////
void recent_project_win::UpdateRecentList()
{
    mRecentListFrame.UpdateRecentList();
}

///////////////////////////////////////////////////////////////////////////////
void recent_project_win::OnBnClickedCreateProject()
{
    // TODO: Add your control notification handler code here
    GetProjectView()->NewProject(this);
}

///////////////////////////////////////////////////////////////////////////////
BOOL recent_project_win::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN &&
        pMsg->wParam == VK_RETURN &&
        GetFocus() == &new_button)
    {
        OnBnClickedCreateProject();
        return TRUE;
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(recent_list_frame, CWnd)
ON_WM_CTLCOLOR()
ON_WM_CREATE()
//ON_WM_SHOWWINDOW()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_ERASEBKGND()
ON_WM_SETFOCUS()
ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
recent_list_frame::recent_list_frame(CWnd* parent)
{
    if (!StudioXIni.recent_project_paths[0].IsEmpty())
    {
        mHighlightRow = 0;
    }
    else
    {
        mHighlightRow = -1;
    }
    mHiBrush.CreateSolidBrush(RGB_COLOR_BLUE);
    mBgBrush.CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
    mGrayBrush.CreateSolidBrush(RGB_COLOR_GRAY);
}

///////////////////////////////////////////////////////////////////////////////
recent_list_frame::~recent_list_frame()
{

}

///////////////////////////////////////////////////////////////////////////////
int recent_list_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }
    int id_list[] = { IDC_RECENT_1, IDC_RECENT_2, IDC_RECENT_3, IDC_RECENT_4, IDC_RECENT_5 };
    CRect size;
    GetClientRect(&size);

    int row_height = GetRowHeight();
    int space = (size.Height() - row_height * MAX_RECENT_PROJECTS) / (MAX_RECENT_PROJECTS - 1);
    size.bottom = size.top + row_height;

    mNumRecentProjects = 0;

    for (int index = 0; index < MAX_RECENT_PROJECTS; index++)
    {
        mRecentList[index].Create(StudioXIni.recent_project_paths[index], WS_VISIBLE | SS_LEFT | SS_WORDELLIPSIS, size, this, id_list[index]);
        mRecentList[index].SetFont(&MediumFont);
        SetLiveRegion(mRecentList[index].GetSafeHwnd());
        size.OffsetRect(0, row_height + space);

        if (StudioXIni.recent_project_paths[index].IsEmpty())
        {
            mRecentList[index].ShowWindow(FALSE);
        }
        else
        {
            mNumRecentProjects++;
        }
    }
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
void recent_list_frame::UpdateRecentList(void)
{
    mNumRecentProjects = 0;
    for (int index = 0; index < MAX_RECENT_PROJECTS; index++)
    {
        if (!StudioXIni.recent_project_paths[index].IsEmpty())
        {
            mRecentList[index].ShowWindow(TRUE);
            mNumRecentProjects++;
        }
        else
        {
            mRecentList[index].ShowWindow(FALSE);
        }

        mRecentList[index].SetWindowText(StudioXIni.recent_project_paths[index]);
    }
}

///////////////////////////////////////////////////////////////////////////////
int recent_list_frame::FindStatic(CPoint point)
{
    int newselect = -1;
    CString text;
    CRect size;
    ClientToScreen(&point);

    for (int index = 0; index < mNumRecentProjects; index++)
    {
        mRecentList[index].GetWindowText(text);

        if (!text.IsEmpty())
        {
            mRecentList[index].GetWindowRect(&size);

            if (size.PtInRect(point))
            {
                newselect = index;
                break;
            }
        }
    }

    return newselect;
}

///////////////////////////////////////////////////////////////////////////////
int recent_list_frame::GetRowHeight()
{
    CDC* dc = GetDC();
    CFont* old_font = dc->SelectObject(&MediumFont);
    int row_height = dc->GetTextExtent(_T("fg"), 1).cy;
    dc->SelectObject(old_font);
    ReleaseDC(dc);
    return row_height;
}

///////////////////////////////////////////////////////////////////////////////
void recent_list_frame::OnMouseMove(UINT nFlags, CPoint point)
{
    int newselect = FindStatic(point);

    if ((newselect >= 0) && (newselect != mHighlightRow))
    {
        mRecentList[mHighlightRow].Invalidate(FALSE);
        mHighlightRow = newselect;
        mRecentList[mHighlightRow].Invalidate(FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void recent_list_frame::OnLButtonDown(UINT nFlags, CPoint point)
{
    int newselect = FindStatic(point);

    if (newselect >= 0)
    {
        GetProjectView()->OpenHistoryProject(newselect);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL recent_list_frame::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_UP:
            if (mHighlightRow > 0)
            {
                mRecentList[mHighlightRow].Invalidate();
                mHighlightRow--;
                mRecentList[mHighlightRow].Invalidate();
                mRecentList[mHighlightRow].NotifyWinEvent(EVENT_OBJECT_LIVEREGIONCHANGED, OBJID_CLIENT, CHILDID_SELF);
            }
            return TRUE;

        case VK_DOWN:
            if ((mHighlightRow >= 0) && (mHighlightRow < mNumRecentProjects - 1))
            {
                mRecentList[mHighlightRow].Invalidate();
                mHighlightRow++;
                mRecentList[mHighlightRow].Invalidate();
                mRecentList[mHighlightRow].NotifyWinEvent(EVENT_OBJECT_LIVEREGIONCHANGED, OBJID_CLIENT, CHILDID_SELF);
            }
            return TRUE;

        case VK_RETURN:
            if (mHighlightRow >= 0)
            {
                GetProjectView()->OpenHistoryProject(mHighlightRow);
            }
            return TRUE;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
HBRUSH recent_list_frame::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC)
    {
        if ((mHighlightRow >= 0) &&
            (&mRecentList[mHighlightRow] == pWnd))
        {
            pDC->SetTextColor(RGB_COLOR_WHITE);
            pDC->SetBkMode(TRANSPARENT);

            if (GetFocus() == this)
            {
                return mHiBrush;
            }
            else
            {
                return mGrayBrush;
            }
        }
    }

    return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);;
}

///////////////////////////////////////////////////////////////////////////////
BOOL recent_list_frame::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CBrush* pOldBrush = pDC->SelectObject(&mBgBrush);

    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed

    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void recent_list_frame::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    if (mHighlightRow >= 0)
    {
        // Invalidate selected row.
        mRecentList[mHighlightRow].Invalidate();

        // Raise a UIA LiveRegionChanged
        // event so that a screen reader is made aware of a change to the LiveRegion.
        // Make sure the updated text is set on the label before making this call.
        mRecentList[mHighlightRow].NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);
    }
}


void recent_list_frame::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    if (mHighlightRow >= 0)
    {
        // Invalidate selected row.
        mRecentList[mHighlightRow].Invalidate();
    }
}
