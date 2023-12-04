
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(left_top_panel_frame, CWnd)

BEGIN_MESSAGE_MAP(left_top_panel_frame, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

left_top_panel_frame::left_top_panel_frame()
{
    mp_project_view = NULL;
    mp_project_header = NULL;

    SetControlSize();
}

void left_top_panel_frame::PostNcDestroy()
{
    delete this;
}
left_top_panel_frame::~left_top_panel_frame()
{
    // KGM: For some reason child views are automatically
    // deleted, but child windows are not. Go figure.

    /*
    if (mp_project_view)
    {
        delete mp_project_view;
    }
    */
    if (mp_project_header)
    {
        delete mp_project_header;
    }
}

void left_top_panel_frame::SetControlSize()
{
    int dpi = GetSystemDPI();
    int text_scaler = GetTextScaler();
    m_header_height = GetScaledValue(VIEW_HEADER_HEIGHT, dpi, text_scaler);
}

int left_top_panel_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    RECT clientrect;
    RECT childrect;

    CWnd::OnCreate(lpCreateStruct);

    GetClientRect(&clientrect);
    childrect = clientrect;

    childrect.bottom = childrect.top + m_header_height - 1;
    mp_project_header = new view_header("Project View", IDB_PROJECT_VIEW);
    mp_project_header->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, childrect, this, 0, NULL);

    childrect.top = childrect.bottom;
    childrect.bottom -= (clientrect.bottom - clientrect.top) / 2;
    mp_project_view = new project_view();
    mp_project_view->Create(NULL, NULL, WS_CHILD, childrect, this, PROJECT_VIEW_ID, NULL);
    mp_project_view->ShowWindow(SW_SHOW);
    return 0;
}

void left_top_panel_frame::PositionChildren()
{
    RECT childrect;
    int bottom;

    if (mp_project_view && mp_project_header)
    {
        GetClientRect(&childrect);
        bottom = childrect.bottom;
        childrect.bottom = childrect.top + m_header_height - 1;
        mp_project_header->MoveWindow(&childrect);

        childrect.top = childrect.bottom;
        childrect.bottom = bottom;
        mp_project_view->MoveWindow(&childrect);
    }
}

void left_top_panel_frame::OnSize(UINT ntype, int cx, int cy)
{
    CWnd::OnSize(ntype, cx, cy);
    PositionChildren();
}


void left_top_panel_frame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CWnd::OnSettingChange(uFlags, lpszSection);

    SetControlSize();
    PositionChildren();
}


IMPLEMENT_DYNCREATE(left_bottom_panel_frame, CWnd)

BEGIN_MESSAGE_MAP(left_bottom_panel_frame, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

left_bottom_panel_frame::left_bottom_panel_frame()
{
    mp_props_header = NULL;
    mp_properties_win = NULL;

    SetControlSize();
}

void left_bottom_panel_frame::PostNcDestroy()
{
    delete this;
}
left_bottom_panel_frame::~left_bottom_panel_frame()
{
    if (mp_properties_win)
    {
        delete mp_properties_win;
    }
    if (mp_props_header)
    {
        delete mp_props_header;
    }
}

void left_bottom_panel_frame::SetControlSize()
{
    int dpi = GetSystemDPI();
    int text_scaler = GetTextScaler();
    m_header_height = GetScaledValue(VIEW_HEADER_HEIGHT, dpi, text_scaler);
}

int left_bottom_panel_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    RECT clientrect;
    RECT childrect;

    CWnd::OnCreate(lpCreateStruct);

    GetClientRect(&clientrect);
    childrect = clientrect;

    childrect.top = childrect.top;
    childrect.bottom = childrect.top + m_header_height - 1;
    mp_props_header = new view_header("Properties View", IDB_PROPS_VIEW);
    mp_props_header->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, childrect, this, 0, NULL);

    childrect.top = childrect.bottom + 1;
    childrect.bottom = clientrect.bottom;
    mp_properties_win = new properties_win();
    mp_properties_win->Create(IDD_PROPS_TEMPLATE, this);
    mp_properties_win->ShowWindow(SW_SHOW);
    return 0;
}

void left_bottom_panel_frame::PositionChildren()
{
    RECT childrect;
    int bottom;

    if (mp_props_header && mp_properties_win)
    {
        GetClientRect(&childrect);
        bottom = childrect.bottom;

        childrect.bottom = childrect.top + m_header_height - 1;
        mp_props_header->MoveWindow(&childrect);

        childrect.top = childrect.bottom;
        childrect.bottom = bottom;
        mp_properties_win->MoveWindow(&childrect);
    }
}

void left_bottom_panel_frame::OnSize(UINT ntype, int cx, int cy)
{
    CWnd::OnSize(ntype, cx, cy);
    PositionChildren();
}

void left_bottom_panel_frame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CWnd::OnSettingChange(uFlags, lpszSection);

    SetControlSize();
    PositionChildren();
}