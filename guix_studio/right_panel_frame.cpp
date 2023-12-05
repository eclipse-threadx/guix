
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(right_panel_frame, CWnd)

BEGIN_MESSAGE_MAP(right_panel_frame, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

extern CString target_class_name;

right_panel_frame::right_panel_frame()
{
    mp_resource_view = NULL;
    mp_resource_header = NULL;
}

right_panel_frame::~right_panel_frame()
{
    if (mp_resource_header)
    {
        delete mp_resource_header;
    }

    // KGM: For some reason child views are automatically delete,
    // but child windows are not. 
    /*
    if (mp_resource_view)
    {
        delete mp_resource_view;
    }
    */
}

void right_panel_frame::PostNcDestroy()
{
    delete this;
}

int right_panel_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    RECT clientrect;
    RECT childrect;

    CWnd::OnCreate(lpCreateStruct);

    GetClientRect(&clientrect);
    childrect = clientrect;

    /* KGM we have removed this for now, but I'm leaving
       it commented just in case it comes back
    childrect.bottom = childrect.top + VIEW_HEADER_HEIGHT - 1;
    mp_resource_header = new view_header("Resource View");
    mp_resource_header->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, childrect, this, 0, NULL);
    */

    childrect.top = childrect.bottom;
    childrect.bottom -= clientrect.bottom;
    mp_resource_view = new resource_view();
    mp_resource_view->Create(NULL, NULL, WS_CHILD, childrect, this, PROJECT_VIEW_ID, NULL);
    mp_resource_view->ShowWindow(SW_SHOW);
    return 0;
}

void right_panel_frame::OnSize(UINT ntype, int cx, int cy)
{
    CWnd::OnSize(ntype, cx, cy);
    RECT childrect;
//    int bottom;

    if (mp_resource_view)
    {
        GetClientRect(&childrect);
        /*
        bottom = childrect.bottom;
        childrect.bottom = childrect.top + VIEW_HEADER_HEIGHT - 1;
        mp_resource_header->MoveWindow(&childrect);
        
        childrect.top = childrect.bottom;
        childrect.bottom = bottom;
        */

        mp_resource_view->MoveWindow(&childrect);
    }
}

/*
void right_panel_frame::OnDraw(CDC *pDC)
{
    CView::OnDraw(pDC);
}
*/