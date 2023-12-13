
#include "studiox_includes.h"

#include "gx_api.h"
#include "target_view.h"
#include "studiox_util.h"
#include "clone_repo_dialog.h"
#include "color_edit_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString target_class_name;
extern CBrush WinBackBrush;
extern INI_INFO StudioXIni;

//IMPLEMENT_DYNCREATE(target_view, CScrollView)
IMPLEMENT_DYNCREATE(target_view, CView)

//BEGIN_MESSAGE_MAP(target_view, CScrollView)
BEGIN_MESSAGE_MAP(target_view, CView)
//	ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
target_view::target_view()
{
    mpRecentDialog = NULL;
    mpTargetFrame = NULL;
    mpTargetScreen = NULL;
    mpToolBar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
target_view::~target_view()
{
    delete mpTargetScreen;
    delete mpRecentDialog;
    //delete mpTargetHeader;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_view::PreCreateWindow(CREATESTRUCT &cs)
{
    cs.style |= WS_CLIPCHILDREN;
    return CView::PreCreateWindow(cs);
    //return CScrollView::PreCreateWindow(cs);
}


///////////////////////////////////////////////////////////////////////////////
int target_view::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    RECT childrect;

    CView::OnCreate(lpCreateStruct);
    SetWindowText(_T("Target View"));

    // register class was here

    GetClientRect(&childrect);

    mpTargetFrame = new target_frame();
    mpTargetFrame->Create(target_class_name, NULL,
        WS_CHILD, childrect, this, TARGET_FRAME_ID, NULL);
    
    //GetDialogCtrlID

    // the dimensions used here don't really matter. The target
    // screen resizes itself to the actual display size automatically.

    mpTargetScreen = new target_screen();
    childrect.left = 0;
    childrect.top = 0;
   childrect.right = 320;
    childrect.bottom = 240;
    mpTargetScreen->Create(target_class_name, NULL,
        WS_CHILD|WS_VISIBLE, childrect, mpTargetFrame, 0, NULL);
    mpTargetScreen->CenterWindow();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void target_view::OnInitialUpdate()
{
    CView::OnInitialUpdate();
    CMainFrame *frame = (CMainFrame *) GetParentFrame();
    frame->SetTargetView(this);

    if (!mpRecentDialog)
    {
        int dpi = GetSystemDPI();
        int text_scaler = GetTextScaler();
        int width = GetScaledValue(RECENT_WINDOW_WIDTH, dpi, text_scaler);
        int height = GetScaledValue(RECENT_WINDOW_HEIGHT, dpi, text_scaler);

        mpRecentDialog = new recent_project_win(width, height, frame);
        mpRecentDialog->Create(IDD_DIALOG_TEMPLATE, frame);
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL target_view::OnEraseBkgnd(CDC *pDC)
{
      // Set brush to desired background color
      //CBrush backBrush(RGB(255, 128, 128));

      // Save old brush
      CBrush* pOldBrush = pDC->SelectObject(&WinBackBrush);

      CRect rect;
      pDC->GetClipBox(&rect);     // Erase the area needed

      pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
          PATCOPY);
      pDC->SelectObject(pOldBrush);
      return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void target_view::OnDraw(CDC *pDC)
{
    CView::OnDraw(pDC);
}

///////////////////////////////////////////////////////////////////////////////
void target_view::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    CRect size;
    GetClientRect(&size);
    mpTargetFrame->MoveWindow(&size);
    mpTargetScreen->UpdateWindowPos();
}

///////////////////////////////////////////////////////////////////////////////
void target_view::DisplayTarget()
{
    mpRecentDialog->ShowWindow(SW_HIDE);
    mpTargetScreen->CenterWindow();
    mpTargetFrame->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
void target_view::DisplayRecentProjects()
{
    mpTargetFrame->ShowWindow(SW_HIDE);

    mpRecentDialog->CenterWindow();
    mpRecentDialog->ShowWindow(SW_SHOW);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void target_view::InitialDisplay(void)
{
#if !defined(FORCE_CLONE_DIALOG)
    if (StudioXIni.first_run)
#endif
    {
        CWnd *parent = AfxGetApp()->m_pMainWnd;
        clone_repo_dialog *dlg = new clone_repo_dialog(parent);
        dlg->DoModal();
        delete(dlg);
    }
    DisplayRecentProjects();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(target_frame, CScrollView)
    ON_WM_SIZE()
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

target_frame::target_frame()
{
}

BOOL target_frame::PreCreateWindow(CREATESTRUCT &cs)
{
    cs.style |= WS_CLIPCHILDREN;
    return CScrollView::PreCreateWindow(cs);
}

///////////////////////////////////////////////////////////////////////////////
void target_frame::OnLButtonDown(UINT nFlags, CPoint point)
{
    GetProjectView()->DeSelect();
}

///////////////////////////////////////////////////////////////////////////////
void target_frame::OnKeyDown(UINT nChar, UINT Rep, UINT flags)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }
    BOOL snap_enabled = project->mHeader.snap_enabled;
    int  snap_spacing = project->mHeader.snap_spacing;

    switch(nChar)
    {   
    case VK_DELETE:
        GetProjectView()->OnDeleteWidget();
        break;

    case VK_UP:
        if (snap_enabled)
        {
            GetTargetScreen()->ShiftSelectedWidgets(0, -snap_spacing);
        }
        else
        {
            GetTargetScreen()->ShiftSelectedWidgets(0, -1);
        }
        break;

    case VK_DOWN:
        if (snap_enabled)
        {
            GetTargetScreen()->ShiftSelectedWidgets(0, snap_spacing);
        }
        else
        {
            GetTargetScreen()->ShiftSelectedWidgets(0, 1);
        }
        break;
   
    case VK_LEFT:
        if (snap_enabled)
        {
            GetTargetScreen()->ShiftSelectedWidgets(-snap_spacing, 0);
        }
        else
        {
            GetTargetScreen()->ShiftSelectedWidgets(-1, 0);
        }
        break;

    case VK_RIGHT:
        if (snap_enabled)
        {
            GetTargetScreen()->ShiftSelectedWidgets(snap_spacing, 0);
        }
        else
        {
            GetTargetScreen()->ShiftSelectedWidgets(1, 0);
        }
        break;

    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_frame::UpdateScrollSize()
{
    CWnd *child = GetWindow(GW_CHILD);
    CRect size;
    CSize sizetotal;

    if (child)
    {
        child->GetClientRect(&size);
        sizetotal.cx = size.right - size.left + 2;
        sizetotal.cy = size.bottom - size.top + 2;
        SetScrollSizes(MM_TEXT, sizetotal);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_frame::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();
    UpdateScrollSize();
}

///////////////////////////////////////////////////////////////////////////////
void target_frame::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    UpdateScrollSize();
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_frame::OnEraseBkgnd(CDC *dc)
{
    CRect client;
    GetClientRect(&client);
    TRIVERTEX vertex[2] ;
    vertex[0].x     = client.left;
    vertex[0].y     = client.top;
    vertex[0].Red   = 0xc600;
    vertex[0].Green = 0xc600;
    vertex[0].Blue  = 0xc600;
    vertex[0].Alpha = 0x0000;

    vertex[1].x     = client.right;
    vertex[1].y     = client.bottom; 
    vertex[1].Red   = 0x0000;
    vertex[1].Green = 0x0000;
    vertex[1].Blue  = 0x0000;
    vertex[1].Alpha = 0x0000;

    // Create a GRADIENT_RECT structure that 
    // references the TRIVERTEX vertices. 
    GRADIENT_RECT gRect;
    gRect.UpperLeft  = 0;
    gRect.LowerRight = 1;

    // Draw a shaded rectangle. 
    dc->GradientFill(vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void target_frame::OnDraw(CDC* pDC)
{
    // outline the target screen:
    CWnd *child = GetWindow(GW_CHILD);
    CRect size;
    CPen whitepen(PS_SOLID, 1, RGB(215, 215, 215));
    CPen blackpen(PS_SOLID, 1, RGB(59, 59, 59));
    CPen graypen(PS_SOLID, 2, RGB(161, 161, 161));
    CPen *oldpen = pDC->SelectObject(&whitepen);

    if (!child)
    {
        return;
    }

    child->GetWindowRect(&size);
    ScreenToClient(&size);
    pDC->MoveTo(size.right + 1, size.top -1);
    pDC->LineTo(size.right + 1, size.bottom + 1);
    pDC->LineTo(size.left - 1, size.bottom + 1);
    pDC->SelectObject(&blackpen);
    pDC->LineTo(size.left - 1, size.top - 1);
    pDC->LineTo(size.right + 1, size.top - 1);

    pDC->SelectObject(&blackpen);
    pDC->MoveTo(size.right + 8, size.top - 1);
    pDC->LineTo(size.right + 1, size.top - 1);
    pDC->LineTo(size.right + 1, size.top - 8);

    pDC->MoveTo(size.left - 8, size.top - 1);
    pDC->LineTo(size.left - 1, size.top - 1);
    pDC->LineTo(size.left - 1, size.top - 8);

    pDC->SelectObject(&graypen);
    pDC->MoveTo(size.right + 8, size.bottom + 1);
    pDC->LineTo(size.right + 1, size.bottom + 1);
    pDC->LineTo(size.right + 1, size.bottom + 9);

    pDC->MoveTo(size.left - 8, size.bottom + 1);
    pDC->LineTo(size.left - 1, size.bottom + 1);
    pDC->LineTo(size.left - 1, size.bottom + 8);

    pDC->SelectObject(oldpen);
    return;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_frame::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    // TODO: Add your specialized code here and/or call the base class
    Invalidate();
    return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}


///////////////////////////////////////////////////////////////////////////////
void target_view::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    mpTargetFrame->SetFocus();
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_view::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_SYSKEYDOWN)
    {
        int nAltKey = GetKeyState(VK_MENU);

        switch (pMsg->wParam)
        {
        case VK_UP:
        case VK_LEFT:
            if (nAltKey & KEY_STATE_DOWN)
            {
                //move focus to project view
                project_view* view = GetProjectView();
                if (view)
                {
                    CMainFrame* frame = (CMainFrame*)GetParentFrame();
                    frame->SetActiveView(view);
                    return TRUE;
                }
            }
            break;

        case VK_DOWN:
        case VK_RIGHT:
            if (nAltKey & KEY_STATE_DOWN)
            {
                //Move focus to resource window
                resource_view* view = GetResourceView();
                if (view)
                {
                    CMainFrame* frame = (CMainFrame*)GetParentFrame();
                    frame->SetActiveView(view);
                    return TRUE;
                }
            }
            break;

        case VK_MENU:
            if ((nAltKey & KEY_STATE_DOWN) && mpTargetScreen->OnAltKeyDown())
            {
                return TRUE;
            }
            break;

        default:
            break;
        }
    }

    return CView::PreTranslateMessage(pMsg);
}
