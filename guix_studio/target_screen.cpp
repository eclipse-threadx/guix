
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SELECT_BOX_LINE_WIDTH 1
#define SELECT_HANDLE_SIZE 5
#define DEFAULT_CROSSHAIR_SIZE 10

int vsnap_line_list[SNAP_LINE_TYPE_COUNT / 2] = {
    SNAP_LINE_TYPE_LEFT,
    SNAP_LINE_TYPE_VERTICAL,
    SNAP_LINE_TYPE_RIGHT
};

int hsnap_line_list[SNAP_LINE_TYPE_COUNT / 2] = {
    SNAP_LINE_TYPE_TOP,
    SNAP_LINE_TYPE_HORIZONTAL,
    SNAP_LINE_TYPE_BOTTOM
};

CArray<WIDGET_SELECT_ENTRY> SelectedWidgets;
#define DRAG_ALL        NUM_SIZE_HANDLES
ULONG select_pattern = 0xf0f0f0f0;

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

enum target_screen_test_commands{
    TEST_GENERATE_CHECKSUM = 1,
    TEST_GENERATE_SCREENSHOT,
    TEST_DESELECT,
    TEST_DIRTY_ROOT_WINDOW,
    TEST_MOVE_WINDOW,
    TEST_MOVE_SELECTED,
    TEST_ADD_WIDGET,
    TEST_ADD_TEMPLATE,
    TEST_ZOOM_IN,
    TEST_LBUTTON_DOWN,
    TEST_LBUTTON_UP,
    TEST_MOUSEMOVE,
    TEST_MOUSEMOVE_POST
};

///////////////////////////////////////////////////////////////////////////////
// the custom drawing function for my GUI selection box:
extern "C" {
VOID SelectBoxDraw(GX_WIDGET *widget)
{
    int count = SelectedWidgets.GetCount();
    int index;

    for (index = 0; index < count; index++)
    {
        WIDGET_SELECT_ENTRY selected = SelectedWidgets.GetAt(index);
        if (selected.info->widget == widget)
        {
            // we found the widget, call it's original draw function
            selected.draw_func(widget);

            // draw the outer outline
            gx_context_raw_brush_define(0x00ffffff, 0x00000000, GX_BRUSH_OUTLINE);
            gx_context_brush_width_set(1);
            gx_context_brush_pattern_set(select_pattern);
            gx_canvas_rectangle_draw(&widget->gx_widget_size);
            gx_context_brush_pattern_set(0);
            break;
        }
    }
}
}
///////////////////////////////////////////////////////////////////////////////
void RefreshSelectedWidgets()
{
    int index;
    int count = SelectedWidgets.GetCount();
    
    if ((!count) || IsGuixCanvasRefreshBlocked())
    {
        return;
    }

    ULONG temp = select_pattern  & 0x03;
    select_pattern >>= 2;
    temp <<= 30;
    select_pattern |= temp;
    
    GX_RECTANGLE invalid;

    invalid = SelectedWidgets[0].info->widget->gx_widget_size;

    for (index = 1; index < count; index++)
    {
        gx_utility_rectangle_combine(&invalid, &SelectedWidgets[index].info->widget->gx_widget_size);
    }
    GX_CANVAS *canvas;
    gx_widget_canvas_get(SelectedWidgets[0].info->widget, &canvas);

    GX_ENTER_CRITICAL
    _gx_canvas_drawing_initiate(canvas,
        (GX_WIDGET *) get_root_window(), &invalid);

    gx_context_raw_brush_define(0x00ffffff, 0x00000000, GX_BRUSH_OUTLINE);
    gx_context_brush_width_set(1);

    for (index = 0; index < count; index++)
    {
        WIDGET_SELECT_ENTRY entry = SelectedWidgets[index];
        _gx_canvas_drawing_initiate(canvas,
            entry.info->widget, &entry.info->widget->gx_widget_size);
        gx_context_brush_pattern_set(select_pattern);
        gx_canvas_rectangle_draw(&entry.info->widget->gx_widget_size);
        _gx_canvas_drawing_complete(canvas, GX_FALSE);
    }
    gx_context_brush_pattern_set(0);

    /* Indicate that drawing on this canvas is complete.  */
    _gx_canvas_drawing_complete(canvas, GX_TRUE);
    GX_EXIT_CRITICAL

    WPARAM wparam = MAKEWPARAM(invalid.gx_rectangle_left, invalid.gx_rectangle_top);
    LPARAM lparam = MAKELPARAM(invalid.gx_rectangle_right, invalid.gx_rectangle_bottom);
    PostMessage(get_target_win_handle(), GUIX_DIRTY, wparam, lparam);
}


///////////////////////////////////////////////////////////////////////////////
VOID  _gx_system_timer_update(VOID);

VOID CALLBACK TimerProc(
  _In_  HWND hwnd,
  _In_  UINT uMsg,
  _In_  UINT_PTR idEvent,
  _In_  DWORD dwTime)
{
    //_gx_system_timer_update();
    RefreshSelectedWidgets();
}

BEGIN_MESSAGE_MAP(target_screen, CWnd)
	ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_MESSAGE(GUIX_READY, target_screen::OnGuixReady)
    ON_MESSAGE(GUIX_DIRTY, target_screen::OnGuixDirty)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
target_screen::target_screen()
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 0, 0);
    SelectedWidgets.RemoveAll();
    mpSelectedParent = NULL;
    m_display_index = -1;
    mDragMode = DRAG_NONE;
    mpDragSelected = NULL;
    mFoldUndo = FALSE;
    mZoomScale = 100;
    mGridSnapInfo.crosshair_enabled = FALSE;
    mGridSnapInfo.crosshair_size = DEFAULT_CROSSHAIR_SIZE;
    mGridSnapInfo.snap_line_count = 0;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::UpdateWindowPos()
{
    target_frame *frame = (target_frame *)GetParent();
    CRect frame_size;
    CRect size;
    int width;
    int height;

    GetClientRect(&size);
    width = size.Width();
    height = size.Height();

    frame->GetClientRect(&frame_size);

    if (size.Width() > frame_size.Width())
    {
        size.left -= frame->GetScrollPos(SB_HORZ);
    }
    else
    {
        size.left = (frame_size.Width() - width) >> 1;
    }

    if (size.Height() > frame_size.Height())
    {
        size.top = -frame->GetScrollPos(SB_VERT);
    }
    else
    {
        size.top = (frame_size.Height() - height) >> 1;
    }

    size.right = size.left + width;
    size.bottom = size.top + height;

    MoveWindow(&size);
    GX_WIDGET *root = (GX_WIDGET *) get_root_window();
    gx_system_dirty_mark(root);
}

///////////////////////////////////////////////////////////////////////////////
int target_screen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CWnd::OnCreate(lpCreateStruct);
    SetWindowText(_T("Target Screen"));
    CenterWindow();
    EnableWindow(TRUE);
    initialize_guix(m_hWnd);
    SetTimer(1, 500, TimerProc);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnCloseProject(void)
{
    GX_DISPLAY *target_display = get_target_view_display();
    SwitchTopWidget(NULL);
    GetResourceView()->CleanupDisplayResources(target_display);
    GetTargetScreen()->ConfigureDisplay(-1);
    guix_cleanup_target_win_canvas();
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *target_screen::FindSelectedWidget(GX_POINT point)
{
    for (int index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        if (gx_utility_rectangle_point_detect(&SelectedWidgets[index].info->widget->gx_widget_size, point))
        {
            return SelectedWidgets[index].info->widget;
        }
    }
    return NULL;
}
///////////////////////////////////////////////////////////////////////////////
/*return parent type. return 0 while do not have parent or pointer is wrong.*/
USHORT target_screen::GetWidgetParentType(widget_info *info)
{
    GX_WIDGET *parent = NULL;
    USHORT type = 0;

    if (info)
    {
        if (info->widget)
        {
            parent = info->widget->gx_widget_parent;
            if (parent)
            {
                type = parent->gx_widget_type;
            }
        }
    }
    return type;
}

///////////////////////////////////////////////////////////////////////////////
// is this moving of child widget of horizontal/vertical list allowed?
BOOL target_screen::IsWidgetResizable(USHORT parent_type, widget_info *info, GX_RECTANGLE &new_size)
{

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return FALSE;
    }

    if (project->IsWidgetPositionLocked())
    {
        if (info->size.gx_rectangle_left != new_size.gx_rectangle_left ||
            info->size.gx_rectangle_top != new_size.gx_rectangle_top ||
            info->size.gx_rectangle_right != new_size.gx_rectangle_right ||
            info->size.gx_rectangle_bottom != new_size.gx_rectangle_bottom)
        {
            Notify("Widget position is locked, resize is not allowed!");
            return FALSE;
        }
    }

    switch (parent_type)
    {
    case GX_TYPE_HORIZONTAL_LIST:
        if ((info->size.gx_rectangle_left != new_size.gx_rectangle_left) ||
            (info->size.gx_rectangle_top != new_size.gx_rectangle_top) ||
            (info->size.gx_rectangle_bottom != new_size.gx_rectangle_bottom))
        {
            Notify("Only right-side size change of horizontal list child widget is allowed.");
            return FALSE;
        }
        break;

    case GX_TYPE_VERTICAL_LIST:
        if ((info->size.gx_rectangle_left != new_size.gx_rectangle_left) ||
            (info->size.gx_rectangle_top != new_size.gx_rectangle_top) ||
            (info->size.gx_rectangle_right != new_size.gx_rectangle_right))
        {
            Notify("Only bottom-side size change of vertical list child widget is allowed.");
            return FALSE;
        }
        break;

    case GX_TYPE_ACCORDION_MENU:
        if ((info->size.gx_rectangle_top != new_size.gx_rectangle_top) ||
            (info->size.gx_rectangle_left != new_size.gx_rectangle_left) ||
            (info->size.gx_rectangle_right != new_size.gx_rectangle_right))
        {
            Notify("Only bottom-side size change of accordion menu child widget is allowed.");
            return FALSE;
        }
        break;

    case GX_TYPE_MENU_LIST:
        if ((info->size.gx_rectangle_top != new_size.gx_rectangle_top) ||
            (info->size.gx_rectangle_left != new_size.gx_rectangle_left))
        {
            Notify("Only bottom and right side size change of menu list child widget is allowed.");
            return FALSE;
        }
        break;

    case GX_TYPE_TREE_VIEW:
        if ((info->size.gx_rectangle_top != new_size.gx_rectangle_top) ||
            (info->size.gx_rectangle_left != new_size.gx_rectangle_left))
        {
            Notify("Only bottom and right side size change of tree view child widget is allowed.");
            return FALSE;
        }
        break;

    case GX_TYPE_GENERIC_SCROLL_WHEEL:
        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::SelectedWidgetsCanBeMoved()
{
    GX_WIDGET *selected;
    studiox_project *project;

    project = GetOpenProject();
    if (!project)
    {
        return FALSE;
    }

    if (project->IsWidgetPositionLocked())
    {
        return FALSE;
    }

    for (int index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        selected = SelectedWidgets[index].info->widget;

        switch(selected->gx_widget_type)
        {
        case GX_TYPE_VERTICAL_SCROLL:
        case GX_TYPE_HORIZONTAL_SCROLL:
            return FALSE;
        
        default:
            break;
        }

        if (selected->gx_widget_parent)
        {
            switch (selected->gx_widget_parent->gx_widget_type)
            {
            case GX_TYPE_VERTICAL_LIST:
            case GX_TYPE_HORIZONTAL_LIST:
            case GX_TYPE_ACCORDION_MENU:
            case GX_TYPE_MENU_LIST:
            case GX_TYPE_TREE_VIEW:
            case GX_TYPE_GENERIC_SCROLL_WHEEL:
                return FALSE;
            }
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnLButtonDown(UINT nFlags, CPoint point)
{
    GX_POINT gxp;
    int scale = GetZoomScale();

    point.x = point.x * 100 / scale;
    point.y = point.y * 100 / scale;

    gxp.gx_point_x = (GX_VALUE)point.x;
    gxp.gx_point_y = (GX_VALUE)point.y;

    mFoldUndo = FALSE;

    if (nFlags & MK_CONTROL)
    {
        SelectWidget(gxp, TRUE);
        return;
    }

    mDragMode = CheckResizeCursor(point);

    if (mDragMode != DRAG_NONE)
    {
        mDragStart = gxp;
    }
    else
    {
        if (!SelectWidget(gxp))
        {
            // Add a check here to see if the click is inside one of multiple:
            GX_WIDGET* find = FindSelectedWidget(gxp);
            if (find && SelectedWidgetsCanBeMoved())
            {
                mDragMode = DRAG_ALL;
                mDragStart = gxp;
                mpDragSelected = find;
            }
        }
    }

    if ((nFlags & MK_LBUTTON) && (mDragMode != DRAG_NONE))
    {
        int delta_x = 0;
        int delta_y = 0;
        CalSnapDelta(delta_x, delta_y, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnRButtonDown(UINT nFlags, CPoint pos)
{
    OnLButtonDown(nFlags, pos);

    widget_info *selected = GetSingleSelect();

    if (selected)
    {
        CMenu menu;
        CMenu* pPopup;

        //Load top level menu
        menu.LoadMenu(IDR_WIDGET_MENU);
        pPopup = menu.GetSubMenu(0);
        ClientToScreen(&pos);

        CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
        CMenu insert_menu;
        CMenu base_menu;
        CMenu *pInsertMenu;

        insert_menu.LoadMenu(IDR_INSERT_MENU);
        pInsertMenu = insert_menu.GetSubMenu(0);

        if (pInsertMenu)
        {
            pInsertMenu->RemoveMenu(ID_INSERT_FOLDER, MF_BYCOMMAND);

            if (GetProjectView()->CreateBaseMenu(&base_menu))
            {
                pInsertMenu->AppendMenu(MF_POPUP, (UINT)base_menu.m_hMenu, _T("Template"));
            }

            pPopup->AppendMenu(MF_POPUP, (UINT)pInsertMenu->m_hMenu, _T("Insert"));
        }

        //show pop up menu
        pPopup->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, (CMainFrame *) AfxGetMainWnd());
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnLButtonUp(UINT nFlags, CPoint point)
{
    mDragMode = DRAG_NONE;
    mFoldUndo = FALSE;

    if (mGridSnapInfo.crosshair_enabled)
    {
        InvalidateCrosshairArea();
        mGridSnapInfo.crosshair_enabled = FALSE;
    }

    if (mGridSnapInfo.snap_line_count)
    {
        InvalidateSnapLineArea();
        mGridSnapInfo.snap_line_count = 0;
    }

    mpDragSelected = NULL;
}

///////////////////////////////////////////////////////////////////////////////
/*The following two functions will reposition the neibor widget.*/
void target_screen::RepositionListChildren(widget_info *info)
{
    widget_info *neighbor_info;
    USHORT type = GetWidgetParentType(info);
    GX_RECTANGLE newsize;
    
    if ((type == GX_TYPE_VERTICAL_LIST) ||
        (type == GX_TYPE_ACCORDION_MENU))
    {
        if (info->GetNextWidgetInfo())
        {
            neighbor_info = info->GetNextWidgetInfo();
            int shift = neighbor_info->size.gx_rectangle_top - info->size.gx_rectangle_bottom;
            while (neighbor_info)
            {
                /*Reposition the nether neighbor widgets(if has one). */
                newsize = neighbor_info->size;
                newsize.gx_rectangle_top -= shift;
                newsize.gx_rectangle_bottom -= shift;
                
                widget_factory::MoveWidget(neighbor_info, newsize);
                neighbor_info = neighbor_info->GetNextWidgetInfo();
            }
        }
    }
    else
    {
        if (type == GX_TYPE_HORIZONTAL_LIST)
        {
            if (info->GetNextWidgetInfo())
            {
                neighbor_info = info->GetNextWidgetInfo();
                INT shift = neighbor_info->size.gx_rectangle_left - info->size.gx_rectangle_right;
                /*Reposition the right neighbor widgets(if has one).*/
                while (neighbor_info)
                {
                    newsize = neighbor_info->size;
                    newsize.gx_rectangle_left -= shift;
                    newsize.gx_rectangle_right -= shift;
                    widget_factory::MoveWidget(neighbor_info, newsize);
                    neighbor_info = neighbor_info->GetNextWidgetInfo();
                }
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void target_screen::WidgetWasMoved(widget_info *info)
{
    /*Check the basetype of the widget and its parent to
      see if we need to reposition the parent widget.*/

    if (!info)
    {
        return;
    }

    USHORT parent_type = GetWidgetParentType(info);
    widget_info *menu_info;

    switch (info->basetype)
    {
    case GX_TYPE_VERTICAL_LIST:
    case GX_TYPE_HORIZONTAL_LIST:
    case GX_TYPE_ACCORDION_MENU:
    case GX_TYPE_TREE_VIEW:
    case GX_TYPE_GENERIC_SCROLL_WHEEL:
        GetProjectView()->CheckParentRefresh(info);
        break;

    default:
        switch (parent_type)
        {
        case GX_TYPE_HORIZONTAL_LIST:
        case GX_TYPE_VERTICAL_LIST:
            RepositionListChildren(info);
            break;

        case GX_TYPE_ACCORDION_MENU:
        case GX_TYPE_TREE_VIEW:
            menu_info = GetOpenProject()->FindWidgetInfo(info->widget->gx_widget_parent);
            gx_system_dirty_mark(menu_info->widget);
            GetProjectView()->CheckParentRefresh(menu_info);
            break;

        case GX_TYPE_MENU_LIST:
            {
                GX_MENU_LIST *menu_list = (GX_MENU_LIST *)info->widget->gx_widget_parent;

                menu_info = GetOpenProject()->FindWidgetInfo(menu_list->gx_menu_list_owner);
                GetProjectView()->CheckParentRefresh(menu_info);
            }
            break;
        }
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
widget_info *target_screen::GetSingleSelect()
{
    if (SelectedWidgets.GetCount() == 1)
    {
        return SelectedWidgets.GetAt(0).info;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::IsWidgetSelected(GX_WIDGET *widget)
{
    for (int index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        if (SelectedWidgets.GetAt(index).info->widget == widget)
        {
            return TRUE;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::CopySelectedWidgets()
{
    widget_info *selected;
    CArray<widget_info *> info_list;
    int index; 
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    for (index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        selected = SelectedWidgets[index].info;

        if (selected)
        {
            info_list.Add(selected);
        }
    }

    if (info_list.GetCount())
    {
        copy_paste_engine cpe;
        cpe.CopyToClipboard(info_list, m_hWnd);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::ShiftSelectedWidgets(int delta_x, int delta_y)
{
    widget_info *selected;
    GX_RECTANGLE size;
    int index;

    UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets, mFoldUndo);
    mFoldUndo = TRUE;

    for (index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        selected = SelectedWidgets[index].info;

        if (selected)
        {
            size = selected->widget->gx_widget_size;
            gx_utility_rectangle_shift(&size, delta_x, delta_y);
            widget_factory::MoveWidget(selected, size);
        }
    }

    GetPropsWin()->WidgetWasMoved();
}

///////////////////////////////////////////////////////////////////////////////
int target_screen::CheckResizeCursor(CPoint point)
{
    GX_RECTANGLE select_outer;
    GX_RECTANGLE select_inner;
    GX_POINT test;
    int drag_mode = DRAG_NONE;

    widget_info *selected = GetSingleSelect();
    if (!selected)
    {
       ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        return DRAG_NONE;
    }

    if (selected->widget->gx_widget_type == GX_TYPE_HORIZONTAL_SCROLL ||
        selected->widget->gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
    {
       ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        return DRAG_NONE;
    }

    select_outer = selected->widget->gx_widget_size;
    select_inner = select_outer;
    gx_utility_rectangle_resize(&select_outer, 5);
    test.gx_point_x = (GX_VALUE)point.x;
    test.gx_point_y = (GX_VALUE)point.y;

    if (gx_utility_rectangle_point_detect(&select_outer, test))
    {
        if (gx_utility_rectangle_point_detect(&select_inner, test))
        {
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
            return DRAG_NONE;
        }

        // we are between two rectangles, figure out where:
        if (test.gx_point_y < select_inner.gx_rectangle_top)
        {
            if (test.gx_point_x < select_inner.gx_rectangle_left)
            {
                // top left corner
                drag_mode = DRAG_TOP_LEFT;
            }
            else
            {
                if (test.gx_point_x > select_inner.gx_rectangle_right)
                {
                    // top right corner
                    drag_mode = DRAG_TOP_RIGHT;
                }
                else
                {
                    // top
                    drag_mode = DRAG_TOP;
                }
            }
        }
        else
        {
            if (test.gx_point_y > select_inner.gx_rectangle_bottom)
            {
                if (test.gx_point_x < select_inner.gx_rectangle_left)
                {
                    // bottom left corner
                    drag_mode = DRAG_BOTTOM_LEFT;
                }
                else
                {
                    if (test.gx_point_x > select_inner.gx_rectangle_right)
                    {
                        // bottom right corner
                        drag_mode = DRAG_BOTTOM_RIGHT;
                    }
                    else
                    {
                        // bottom
                        drag_mode = DRAG_BOTTOM;
                    }
                }
            }
            else
            {
                if (test.gx_point_x < select_inner.gx_rectangle_left)
                {
                    drag_mode = DRAG_LEFT;
                }
                else
                {
                    drag_mode = DRAG_RIGHT;
                }
            }
        }

        if (selected->basetype == GX_TYPE_RADIAL_PROGRESS_BAR)
        {
            drag_mode = ConvertRadialProgressDragMode(drag_mode, select_inner, test);
        }

        switch (drag_mode)
        {
        case DRAG_LEFT:
        case DRAG_RIGHT:
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
            break;

        case DRAG_TOP:
        case DRAG_BOTTOM:
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
            break;

        case DRAG_TOP_LEFT:
        case DRAG_BOTTOM_RIGHT:
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
            break;

        case DRAG_TOP_RIGHT:
        case DRAG_BOTTOM_LEFT:
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
            break;
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
int target_screen::CalSnap2GridDelta(int value, int &delta)
{
    if (!delta)
    {
        return 0;
    }

    int spacing = GetOpenProject()->mHeader.grid_spacing;
    int half_spacing = spacing / 2;
    int dist;
    int snap_dist = 0;

    // Calculate the distance from the new position to the grid line.
    dist = (value + delta) % spacing;

    if (dist != 0)
    {
        // Snap to grid.
        if (abs(dist) < half_spacing)
        {
            snap_dist = -dist;
        }
        else
        {
            int sign = abs(dist) / dist;
            snap_dist = (spacing * sign - dist);
        }
    }

    delta += snap_dist;

    return snap_dist;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::OnAltKeyDown()
{
    BOOL result = FALSE;
    if (mGridSnapInfo.crosshair_enabled)
    {
        InvalidateCrosshairArea();
        mGridSnapInfo.crosshair_enabled = FALSE;
        result = TRUE;
    }

    if (mGridSnapInfo.snap_line_count)
    {
        InvalidateSnapLineArea();
        mGridSnapInfo.snap_line_count = 0;
        result = TRUE;
    }

    if (result)
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);

        int x, y;
        x = pt.x;
        y = pt.y;
        SendMessage(WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(x, y));
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::InvalidateCrosshairArea()
{
    CRect rect;
    int scale;

    if (mGridSnapInfo.crosshair_enabled)
    {
        scale = GetZoomScale();

        rect.left = mGridSnapInfo.crosshair_cx - mGridSnapInfo.crosshair_size;
        rect.right = mGridSnapInfo.crosshair_cx + mGridSnapInfo.crosshair_size;
        rect.top = mGridSnapInfo.crosshair_cy - mGridSnapInfo.crosshair_size;
        rect.bottom = mGridSnapInfo.crosshair_cy + mGridSnapInfo.crosshair_size;

        scale = GetZoomScale();

        rect.left = rect.left * scale / 100;
        rect.top = rect.top * scale / 100;
        rect.right = (rect.right + 1) * scale / 100;
        rect.bottom = (rect.bottom + 1) * scale / 100;

        InvalidateRect(&rect);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::InvalidateSnapLineArea()
{
    SNAPLINE_INFO *snapline;
    CRect rect;
    INT scale;

    // Invalidate snap line areas.
    for (int index = 0; index < mGridSnapInfo.snap_line_count; index++)
    {
        snapline = &mGridSnapInfo.snap_lines[index];

        if (snapline->direction == SNAP_LINE_DIRECTION_VERTICAL)
        {
            rect.left = snapline->target_x_y;
            rect.top = snapline->target_top_left;
            rect.right = snapline->target_x_y + 1;
            rect.bottom = snapline->target_bottom_right + 1;
        }
        else
        {
            rect.left = snapline->target_top_left;
            rect.top = snapline->target_x_y;
            rect.right = snapline->target_bottom_right + 1;
            rect.bottom = snapline->target_x_y + 1;
        }

        scale = GetZoomScale();

        rect.left = rect.left * scale / 100;
        rect.top = rect.top * scale / 100;
        rect.right = rect.right * scale / 100;
        rect.bottom = rect.bottom * scale / 100;

        InvalidateRect(&rect);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::UpdateSnapLines(int delta_x, int delta_y)
{
    SNAPLINE_INFO *snapline;

    for (int index = 0; index < mGridSnapInfo.snap_line_count; index++)
    {
        snapline = &mGridSnapInfo.snap_lines[index];

        if (snapline->direction == SNAP_LINE_DIRECTION_VERTICAL)
        {
            // Update source snap line.
            snapline->source_top_left += delta_y;
            snapline->source_bottom_right += delta_y;

            // Combine source and target snaplines.
            if (snapline->source_top_left < snapline->target_top_left)
            {
                snapline->target_top_left = snapline->source_top_left;
            }

            if (snapline->source_bottom_right > snapline->target_bottom_right)
            {
                snapline->target_bottom_right = snapline->source_bottom_right;
            }
        }
        else
        {
            // Update source snapline.
            snapline->source_top_left += delta_x;
            snapline->source_bottom_right += delta_x;

            // Combine source and target snaplines.
            if (snapline->source_top_left < snapline->target_top_left)
            {
                snapline->target_top_left = snapline->source_top_left;
            }
            
            if(snapline->source_bottom_right > snapline->target_bottom_right)
            {
                snapline->target_bottom_right = snapline->source_bottom_right;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int target_screen::GetSnapLineVal(GX_RECTANGLE &size, int snap_line_type)
{
    switch (snap_line_type)
    {
    case SNAP_LINE_TYPE_LEFT:
        return size.gx_rectangle_left;

    case SNAP_LINE_TYPE_RIGHT:
        return size.gx_rectangle_right;

    case SNAP_LINE_TYPE_VERTICAL:
        return ((size.gx_rectangle_left + size.gx_rectangle_right) >> 1);

    case SNAP_LINE_TYPE_TOP:
        return size.gx_rectangle_top;

    case SNAP_LINE_TYPE_BOTTOM:
        return size.gx_rectangle_bottom;

    case SNAP_LINE_TYPE_HORIZONTAL:
        return ((size.gx_rectangle_top + size.gx_rectangle_bottom) >> 1);

    default:
        return 0;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::IsVerticalSnapLineType(int snap_line_type)
{
    switch (snap_line_type)
    {
    case SNAP_LINE_TYPE_LEFT:
    case SNAP_LINE_TYPE_RIGHT:
    case SNAP_LINE_TYPE_VERTICAL:
        return TRUE;

    default:
        return FALSE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::TestAddSnapLine(SNAPLINE_INFO &test)
{
    SNAPLINE_INFO *snapline;

    for (int index = 0; index < mGridSnapInfo.snap_line_count; index++)
    {
        snapline = &mGridSnapInfo.snap_lines[index];

        if ((snapline->direction == test.direction) &&
            (snapline->target_x_y == test.target_x_y))
        {
            if (test.target_top_left < snapline->target_top_left)
            {
                snapline->target_top_left = test.target_top_left;
            }

            if (test.target_bottom_right > snapline->target_bottom_right)
            {
                snapline->target_bottom_right = test.target_bottom_right;
            }

            return;
        }
    }

    if (mGridSnapInfo.snap_line_count < MAX_SNAP_LINES)
    {
        mGridSnapInfo.snap_lines[mGridSnapInfo.snap_line_count++] = test;
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::ClearSnapLine(int snapline_direction)
{
    SNAPLINE_INFO *snapline;

    for (int index = 0; index < mGridSnapInfo.snap_line_count; index++)
    {
        snapline = &mGridSnapInfo.snap_lines[index];

        if (snapline->direction == snapline_direction)
        {
            mGridSnapInfo.snap_line_count--;
            for (int loop = index; loop < mGridSnapInfo.snap_line_count; loop++)
            {
                mGridSnapInfo.snap_lines[loop] = mGridSnapInfo.snap_lines[loop + 1];
            }
            index--;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::Snap2Widget(GX_RECTANGLE &src_size, GX_RECTANGLE &target_size,
                                int src_snap_line_type, int direction,
                                int &snap_dist)
{
    BOOL result = FALSE;
    int  src_val;
    int *src_type_list;
    int  src_type_count;
    int  target_val;
    int  target_snap_line_type;
    int *target_type_list;
    int  target_type_count;
    int  dist;

    if (src_snap_line_type >= 0)
    {
        src_type_list = &src_snap_line_type;
        src_type_count = 1;
    }
    else
    {
        src_type_count = (SNAP_LINE_TYPE_COUNT / 2);
    }

    if (direction == SNAP_LINE_DIRECTION_VERTICAL)
    {
        target_type_list = vsnap_line_list;

        if (src_snap_line_type < 0)
        {
            src_type_list = vsnap_line_list;
        }
    }
    else
    {
        target_type_list = hsnap_line_list;

        if (src_snap_line_type < 0)
        {
            src_type_list = hsnap_line_list;
        }
    }
    
    SNAPLINE_INFO snapline;
    target_type_count = (SNAP_LINE_TYPE_COUNT / 2);

    for (int src_index = 0; src_index < src_type_count; src_index++)
    {
        src_snap_line_type = src_type_list[src_index];
        src_val = GetSnapLineVal(src_size, src_snap_line_type);

        for (int target_index = 0; target_index < target_type_count; target_index++)
        {
            target_snap_line_type = target_type_list[target_index];
            target_val = GetSnapLineVal(target_size, target_snap_line_type);

            dist = target_val - src_val;
            if (GX_ABS(dist) <= GX_ABS(snap_dist))
            {
                if ((GX_ABS(dist) < GX_ABS(snap_dist)) || ((dist * snap_dist) < 0))
                {
                    ClearSnapLine(direction);
                }

                snap_dist = target_val - src_val;

                snapline.target_x_y = target_val;
                snapline.direction = direction;

                if (direction == SNAP_LINE_DIRECTION_VERTICAL)
                {
                    snapline.target_top_left = target_size.gx_rectangle_top;
                    snapline.target_bottom_right = target_size.gx_rectangle_bottom;

                    snapline.source_top_left = src_size.gx_rectangle_top;
                    snapline.source_bottom_right = src_size.gx_rectangle_bottom;
                }
                else
                {
                    snapline.target_top_left = target_size.gx_rectangle_left;
                    snapline.target_bottom_right = target_size.gx_rectangle_right;

                    snapline.source_top_left = src_size.gx_rectangle_left;
                    snapline.source_bottom_right = src_size.gx_rectangle_right;
                }

                TestAddSnapLine(snapline);

                result = TRUE;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::CalSnap2WidgetDelta(GX_WIDGET *selected, int &delta, int direction, int snap_dist, int src_snap_line_type)
{
    GX_WIDGET *parent = selected->gx_widget_parent;
    studiox_project *project = GetOpenProject();

    if ((!parent) || (!project))
    {
        return FALSE;
    }

    GX_RECTANGLE src_size = selected->gx_widget_size;

    if (direction == SNAP_LINE_DIRECTION_VERTICAL)
    {
        gx_utility_rectangle_shift(&src_size, delta, 0);
    }
    else
    {
        gx_utility_rectangle_shift(&src_size, 0, delta);
    }

    if (GX_ABS(snap_dist) > project->mHeader.snap_spacing)
    {
        snap_dist = project->mHeader.snap_spacing;
    }

    BOOL find = FALSE;

    if (Snap2Widget(src_size, parent->gx_widget_size, src_snap_line_type, direction, snap_dist))
    {
        find = TRUE;
    }


    GX_WIDGET *child = parent->gx_widget_first_child;

    while (child)
    {
        if ((child != selected) &&
            Snap2Widget(src_size, child->gx_widget_size, src_snap_line_type, direction, snap_dist))
        {
            find = TRUE;
        }

        child = child->gx_widget_next;
    }

    if (find)
    {
        delta += snap_dist;
    }

    return find;
}

///////////////////////////////////////////////////////////////////////////////
int target_screen::ConvertRadialProgressDragMode(int drag_mode, GX_RECTANGLE rect, GX_POINT point)
{
    switch (drag_mode)
    {
    case DRAG_TOP_LEFT:
        if (abs(point.gx_point_x - rect.gx_rectangle_left) < abs(point.gx_point_y - rect.gx_rectangle_top))
        {
            // Mouse cursor is near to left margin
            drag_mode = DRAG_LEFT;
        }
        else
        {
            drag_mode = DRAG_TOP;
        }
        break;

    case DRAG_TOP_RIGHT:
        if (abs(point.gx_point_x - rect.gx_rectangle_right) < abs(point.gx_point_y - rect.gx_rectangle_top))
        {
            // Mouse cursor is near to right margin
            drag_mode = DRAG_RIGHT;
        }
        else
        {
            drag_mode = DRAG_TOP;
        }
        break;

    case DRAG_BOTTOM_LEFT:
        if (abs(point.gx_point_x - rect.gx_rectangle_left) < abs(point.gx_point_y - rect.gx_rectangle_bottom))
        {
            drag_mode = DRAG_LEFT;
        }
        else
        {
            drag_mode = DRAG_BOTTOM;
        }
        break;

    case DRAG_BOTTOM_RIGHT:
        if (abs(point.gx_point_x - rect.gx_rectangle_right) < abs(point.gx_point_y - rect.gx_rectangle_bottom))
        {
            drag_mode = DRAG_RIGHT;
        }
        else
        {
            drag_mode = DRAG_BOTTOM;
        }
        break;;
    }

    return drag_mode;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::UpdateRadialProgressSize(GX_RECTANGLE *size, int mDragMode, int delta_x, int delta_y)
{
    switch (mDragMode)
    {
    case DRAG_LEFT:
        size->gx_rectangle_left += delta_x;
        size->gx_rectangle_right -= delta_x;
        size->gx_rectangle_top += delta_x;
        size->gx_rectangle_bottom -= delta_x;
        break;

    case DRAG_RIGHT:
        size->gx_rectangle_left -= delta_x;
        size->gx_rectangle_right += delta_x;
        size->gx_rectangle_top -= delta_x;
        size->gx_rectangle_bottom += delta_x;
        break;

    case DRAG_TOP:
        size->gx_rectangle_top += delta_y;
        size->gx_rectangle_bottom -= delta_y;
        size->gx_rectangle_left += delta_y;
        size->gx_rectangle_right -= delta_y;
        break;

    case DRAG_BOTTOM:
        size->gx_rectangle_top -= delta_y;
        size->gx_rectangle_bottom += delta_y;
        size->gx_rectangle_left -= delta_y;
        size->gx_rectangle_right += delta_y;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::UpdateWidgetSize(GX_RECTANGLE *size, int mDrawMode, int delta_x, int delta_y)
{
    switch (mDragMode)
    {
    case DRAG_TOP_LEFT:
        size->gx_rectangle_left += delta_x;
        size->gx_rectangle_top += delta_y;
        break;

    case DRAG_TOP_RIGHT:
        size->gx_rectangle_right += delta_x;
        size->gx_rectangle_top += delta_y;
        break;

    case DRAG_BOTTOM_RIGHT:
        size->gx_rectangle_right += delta_x;
        size->gx_rectangle_bottom += delta_y;
        break;

    case DRAG_BOTTOM_LEFT:
        size->gx_rectangle_left += delta_x;
        size->gx_rectangle_bottom += delta_y;
        break;

    case DRAG_LEFT:
        size->gx_rectangle_left += delta_x;
        break;

    case DRAG_RIGHT:
        size->gx_rectangle_right += delta_x;
        break;

    case DRAG_TOP:
        size->gx_rectangle_top += delta_y;
        break;

    case DRAG_BOTTOM:
        size->gx_rectangle_bottom += delta_y;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::CalSnapDelta(int &delta_x, int &delta_y, int snap_dist)
{
    int nAltKey = GetKeyState(VK_MENU);

    if (nAltKey & KEY_STATE_DOWN)
    {
        // Disable snap feature when ALT key is down.
        return;
    }

    studiox_project *project = GetOpenProject();
    project_header *header = &project->mHeader;

    if ((!header->snap_enabled) && (!header->snap_to_widget_enabled))
    {
        return;
    }

    GX_WIDGET *selected = SelectedWidgets[0].info->widget;
    GX_RECTANGLE size = selected->gx_widget_size;

    int snap_delta_x = delta_x;
    int snap_delta_y = delta_y;
    int snap_dist_x = snap_dist;
    int snap_dist_y = snap_dist;
    BOOL snap_result_x = FALSE;
    BOOL snap_result_y = FALSE;

    if (header->snap_enabled)
    {
        if (mGridSnapInfo.crosshair_enabled)
        {
            InvalidateCrosshairArea();
            mGridSnapInfo.crosshair_enabled = FALSE;
        }

        // Calculate snap to grid delta and snap distance

        switch (mDragMode)
        {
        case DRAG_ALL:
            if (mpDragSelected)
            {
                size = mpDragSelected->gx_widget_size;

                // Calculate center point of drag selected widget.
                mGridSnapInfo.crosshair_cx = ((size.gx_rectangle_left + size.gx_rectangle_right) >> 1);
                mGridSnapInfo.crosshair_cy = ((size.gx_rectangle_top + size.gx_rectangle_bottom) >> 1);

                // Adjust delta value to make selected center point snap to grid
                snap_dist_x = CalSnap2GridDelta(mGridSnapInfo.crosshair_cx, delta_x);
                snap_dist_y = CalSnap2GridDelta(mGridSnapInfo.crosshair_cy, delta_y);

                mGridSnapInfo.crosshair_enabled = TRUE;
            }
            break;

        case DRAG_TOP_LEFT:
            snap_dist_x = CalSnap2GridDelta(size.gx_rectangle_left, delta_x);
            snap_dist_y = CalSnap2GridDelta(size.gx_rectangle_top, delta_y);
            break;

        case DRAG_TOP_RIGHT:
            snap_dist_x = CalSnap2GridDelta(size.gx_rectangle_right, delta_x);
            snap_dist_y = CalSnap2GridDelta(size.gx_rectangle_top, delta_y);
            break;

        case DRAG_BOTTOM_RIGHT:
            snap_dist_x = CalSnap2GridDelta(size.gx_rectangle_right, delta_x);
            snap_dist_y = CalSnap2GridDelta(size.gx_rectangle_bottom, delta_y);
            break;

        case DRAG_BOTTOM_LEFT:
            snap_dist_x = CalSnap2GridDelta(size.gx_rectangle_left, delta_x);
            snap_dist_y = CalSnap2GridDelta(size.gx_rectangle_bottom, delta_y);
            break;

        case DRAG_LEFT:
            snap_dist_x = CalSnap2GridDelta(size.gx_rectangle_left, delta_x);
            break;

        case DRAG_RIGHT:
            snap_dist_x = CalSnap2GridDelta(size.gx_rectangle_right, delta_x);
            break;

        case DRAG_TOP:
            snap_dist_y = CalSnap2GridDelta(size.gx_rectangle_top, delta_y);
            break;

        case DRAG_BOTTOM:
            snap_dist_y = CalSnap2GridDelta(size.gx_rectangle_bottom, delta_y);
            break;
        }
    }

    if (header->snap_to_widget_enabled)
    {
        if (mGridSnapInfo.snap_line_count)
        {
            // Invalidate old snap line area.
            InvalidateSnapLineArea();
            mGridSnapInfo.snap_line_count = 0;
        }

        // Calculate snap to widget delta.

        switch (mDragMode)
        {
        case DRAG_ALL:
            if (mpDragSelected)
            {
                snap_result_x = CalSnap2WidgetDelta(mpDragSelected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x);
                snap_result_y = CalSnap2WidgetDelta(mpDragSelected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y);
            }
            break;

        case DRAG_TOP_LEFT:
            snap_result_x = CalSnap2WidgetDelta(selected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x, SNAP_LINE_TYPE_LEFT);
            snap_result_y = CalSnap2WidgetDelta(selected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y, SNAP_LINE_TYPE_TOP);
            break;

        case DRAG_TOP_RIGHT:
            snap_result_x = CalSnap2WidgetDelta(selected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x, SNAP_LINE_TYPE_RIGHT);
            snap_result_y = CalSnap2WidgetDelta(selected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y, SNAP_LINE_TYPE_TOP);
            break;

        case DRAG_BOTTOM_RIGHT:
            snap_result_x = CalSnap2WidgetDelta(selected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x, SNAP_LINE_TYPE_RIGHT);
            snap_result_y = CalSnap2WidgetDelta(selected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y, SNAP_LINE_TYPE_BOTTOM);
            break;

        case DRAG_BOTTOM_LEFT:
            snap_result_x = CalSnap2WidgetDelta(selected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x, SNAP_LINE_TYPE_LEFT);
            snap_result_y = CalSnap2WidgetDelta(selected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y, SNAP_LINE_TYPE_BOTTOM);
            break;

        case DRAG_LEFT:
            snap_result_x = CalSnap2WidgetDelta(selected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x, SNAP_LINE_TYPE_LEFT);
            break;

        case DRAG_RIGHT:
            snap_result_x = CalSnap2WidgetDelta(selected, snap_delta_x, SNAP_LINE_DIRECTION_VERTICAL, snap_dist_x, SNAP_LINE_TYPE_RIGHT);
            break;

        case DRAG_TOP:
            snap_result_y = CalSnap2WidgetDelta(selected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y, SNAP_LINE_TYPE_TOP);
            break;

        case DRAG_BOTTOM:
            snap_result_y = CalSnap2WidgetDelta(selected, snap_delta_y, SNAP_LINE_DIRECTION_HORIZONTAL, snap_dist_y, SNAP_LINE_TYPE_BOTTOM);
            break;
        }

        if (snap_result_x)
        {
            delta_x = snap_delta_x;
        }

        if (snap_result_y)
        {
            delta_y = snap_delta_y;
        }
    }

    if (header->snap_enabled && mGridSnapInfo.crosshair_enabled)
    {
        // Update crosshair coordinates
        if (header->snap_enabled)
        {
            mGridSnapInfo.crosshair_cx += delta_x;
            mGridSnapInfo.crosshair_cy += delta_y;
        }

        InvalidateCrosshairArea();
    }

    if (header->snap_to_widget_enabled && mGridSnapInfo.snap_line_count)
    {
        // Update snap lines.
        UpdateSnapLines(delta_x, delta_y);

        // Invalidate new snap line area
        InvalidateSnapLineArea();
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnMouseMove(UINT nFlags, CPoint point)
{
    GX_RECTANGLE size;
    int delta_x;
    int delta_y;

    int scale = GetZoomScale();
    point.x = point.x * 100 / scale;
    point.y = point.y * 100 / scale;

    if (mDragMode == DRAG_NONE)
    {
        if ((nFlags & MK_LBUTTON) == 0)
        {
            CheckResizeCursor(point);
        }
        return;
    }

    if (nFlags & MK_LBUTTON && SelectedWidgets.GetCount())
    {
        delta_x = point.x - mDragStart.gx_point_x;
        delta_y = point.y - mDragStart.gx_point_y;

        if (!delta_x && !delta_y)
        {
            return;
        }

        studiox_project *project = GetOpenProject();
        project_header *header = &project->mHeader;

        GX_WIDGET *selected = SelectedWidgets[0].info->widget;
        size = selected->gx_widget_size;

        widget_info *info = project->FindWidgetInfo(selected);

        if (selected && (header->snap_enabled || header->snap_to_widget_enabled))
        {
            CalSnapDelta(delta_x, delta_y, header->snap_spacing);
        }

        if (!delta_x && !delta_y)
        {
            return;
        }

        if (mDragMode == DRAG_ALL)
        {
            mDragStart.gx_point_x += delta_x;
            mDragStart.gx_point_y += delta_y;

            ShiftSelectedWidgets(delta_x, delta_y);
        }
        else
        {
            if (info->basetype == GX_TYPE_RADIAL_PROGRESS_BAR)
            {
                UpdateRadialProgressSize(&size, mDragMode, delta_x, delta_y);
            }
            else
            {
                UpdateWidgetSize(&size, mDragMode, delta_x, delta_y);
            }

            mDragStart.gx_point_x += delta_x;
            mDragStart.gx_point_y += delta_y;

            UINT parent_type = GetWidgetParentType(info);

            /* Check if widget moving is allowed. */
            if (!IsWidgetResizable(parent_type, info, size))
            {
                return;
            }

            if (info)
            {
                UndoManager()->AddEntry(UNDO_TYPE_SIZE, info, 0, mFoldUndo);
                widget_factory::MoveWidget(info, size);

                WidgetWasMoved(info);
                GetPropsWin()->WidgetWasMoved();
                mFoldUndo = TRUE;
            }
            else
            {
                ErrorMsg("Internal error: widget information is not available");
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
LRESULT target_screen::OnGuixReady(WPARAM wp, LPARAM lp)
{
    Invalidate();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT target_screen::OnGuixDirty(WPARAM wp, LPARAM lp)
{
    CRect rect;

    int scale = GetZoomScale();

    // In guix, rectangle.width = rectangle.right - rectangle.left + 1
    // In mfc, rectangle.width = rectangle.right - rectangle.left
    rect.left = GET_X_LPARAM(wp);
    rect.top = GET_Y_LPARAM(wp);
    rect.right = (GET_X_LPARAM(lp) + 1);
    rect.bottom = (GET_Y_LPARAM(lp) + 1);

    //scale the guix dirty area to get the screen dirty
    rect.left = rect.left * scale / 100;
    rect.top = rect.top * scale / 100;
    rect.right = rect.right * scale / 100;
    rect.bottom = rect.bottom * scale / 100;

    InvalidateRect(&rect);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
void target_screen::OnPaint()
{
    GX_CANVAS *canvas = get_target_win_canvas();

    CPaintDC dc(this);

    if (canvas ->gx_canvas_memory)
    {
        CRect    rect;
        GX_RECTANGLE dirty;
        int scale = GetZoomScale();

        dc.GetClipBox(&rect);

        //convert screen dirty area to guix dirty area
        rect.left = rect.left * 100 / scale;
        rect.top = rect.top * 100 / scale;
        rect.right = rect.right * 100 / scale;
        rect.bottom = rect.bottom * 100 / scale;

        gx_utility_rectangle_define(&dirty,
            (GX_VALUE)rect.left,
            (GX_VALUE)rect.top,
            (GX_VALUE)rect.right,
            (GX_VALUE)rect.bottom);

        if (!IsGuixCanvasRefreshBlocked())
        {
            /* make sure our guix canvas is up to date */
            _gx_system_canvas_refresh();
        }

        studiox_project *project = GetOpenProject();

        if (project)
        {
            if (project->mHeader.grid_enabled)
            {
                mGridSnapInfo.grid_space = project->mHeader.grid_spacing;
            }
            else
            {
                mGridSnapInfo.grid_space = 0;
                
            }

            guix_canvas_paint(dc.GetSafeHdc(), dirty, scale, &mGridSnapInfo);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::DeSelect()
{
    GX_WIDGET* parent;

    for (int index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        /* replace the drawing functions replaced to draw the selection box */
        if (SelectedWidgets[index].info->widget)
        {
            SelectedWidgets[index].info->widget->gx_widget_draw_function = SelectedWidgets[index].draw_func;

            if (SelectedWidgets[index].info->widget->gx_widget_type == GX_TYPE_DROP_LIST)
            {
                gx_drop_list_close((GX_DROP_LIST *) SelectedWidgets[index].info->widget);
            }

            parent = SelectedWidgets[index].info->widget->gx_widget_parent;
            while (parent)
            {
                if (parent->gx_widget_type == GX_TYPE_DROP_LIST)
                {
                    gx_drop_list_close((GX_DROP_LIST *)parent);
                    break;
                }
                parent = parent->gx_widget_parent;
            }
            gx_system_dirty_mark(SelectedWidgets[index].info->widget->gx_widget_parent);
        }
    }
    SelectedWidgets.RemoveAll();
    mpSelectedParent = NULL;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::ConfigureDisplay(int display_index, BOOL bForce)
{
    GX_WIDGET *root = (GX_WIDGET *) get_root_window();

    if (root)
    {
        while(root->gx_widget_first_child)
        {
            gx_widget_detach(root->gx_widget_first_child);
        }
    }

    if (m_display_index == display_index && !bForce)
    {
        return TRUE;
    }

    m_display_index = display_index;
    studiox_project *project = GetOpenProject();

    if (display_index >= 0 && project)
    {
        display_info *info = &project->mDisplays[display_index];

        int canvas_xres = info->xres;
        int canvas_yres = info->yres;

        if (info->rotation_angle == GX_SCREEN_ROTATION_CW ||
            info->rotation_angle == GX_SCREEN_ROTATION_CCW)
        {
            GX_SWAP_VALS(canvas_xres, canvas_yres);
        }

        int active_theme = project->mDisplays[display_index].active_theme;

        // reconfigure the canvas and root window
        if (!guix_configure_target_win_canvas(canvas_xres,
            canvas_yres,
            info->colorformat,
            project->mHeader.target_cpu,
            IsRenesasDave2D(project),
            IsDave2dFontFormat(project, display_index),
            info->themes[active_theme].palette,
            info->themes[active_theme].palette_total_size,
            project->mHeader.palette_mode_aa_text_colors))
        {
            ErrorMsg("Internal Error: Unable to configure GUIX canvas");
            return FALSE;
        }

        int scale = GetZoomScale();

        // resize myself to match canvas size:
        SetWindowPos(NULL, 0, 0, canvas_xres * scale / 100, canvas_yres * scale / 100, SWP_NOMOVE);

        CenterWindow();
        target_frame *frame = (target_frame *) GetParent();
        frame->UpdateScrollSize();
        frame->Invalidate();
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::SwitchTopWidget(widget_info *info)
{
    int DisplayIndex = GetProjectView()->GetActiveDisplay();
    GX_WIDGET *root = (GX_WIDGET *) get_root_window();
    widget_info *old_info;

    if (!root && DisplayIndex >= 0)
    {
        // if there is an active display but no root window, it
        // means we haven't configured this display yet. Do it now.

        ConfigureDisplay(DisplayIndex);
        root = (GX_WIDGET *) get_root_window();
        DisplayIndex = -1;      // don't configure again
    }
    if (!root)
    {
        // something is badly wrong, but don't crash:
        ErrorMsg("Internal Error: Unable to configure selected display.");
        return;
    }
    if (info)
    {
        if (info->widget == NULL)
        {
            // top level screen has not been created, create it now:

            widget_factory::GenerateWidgets(NULL, info, TRUE, FALSE);
            if (info->widget == NULL)
            {
                ErrorMsg("Internal Error: Unable to create selected screen.");
                return;
            }
        }
        if (root -> gx_widget_first_child)
        {
            if (root->gx_widget_first_child == info->widget)
            {
                // if the top-level window of this root is already the
                // window we are switching to, then we are done:
                return;
            }
            else
            {
                // top-level child is changing, delete the old screen:
                old_info = GetOpenProject()->FindWidgetInfo(root ->gx_widget_first_child);
                if (old_info)
                {
                    widget_factory::CleanupWidgets(old_info, TRUE, FALSE);
                }
            }
        }
    }

    DeSelect();

    if (info)
    {
        // if I haven't checked my display configuration already, do it now:
        if (DisplayIndex >= 0)
        {
            ConfigureDisplay(DisplayIndex);
        }

        root = (GX_WIDGET *) get_root_window();

        if (info->widget)
        {
            gx_widget_attach(root, info->widget);
            gx_widget_show(info->widget);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void target_screen::MakeTopWindowVisible(widget_info *info)
{
    widget_info *top_info = info;
    widget_info *parent_info = info;
    studiox_project *project = GetOpenProject();

    GX_WINDOW_ROOT *root = get_root_window();

    if (!root)
    {
        return;
    }

    if (root->gx_widget_first_child == NULL)
    {
        // No top screen visible right now, need to switch to the top screen of current selected widget
        // Show root window to make new top screen visible
        gx_widget_show(root);
    }
    else
    {
        if (!project || info->widget != NULL)
        {
            return;
        }
    }

    while(parent_info)
    {
        parent_info = project->FindParentInfo(parent_info);
        if (parent_info)
        {
            top_info = parent_info;
        }
    }
    if (top_info)
    {
        SwitchTopWidget(top_info);            
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::SelectWidget(widget_info *info, BOOL echo_to_project_view, BOOL multiple)
{
    //GX_WIDGET *top_win;
    widget_info *selected = GetSingleSelect();

    if (info == selected)
    {
        if (echo_to_project_view)
        {
            GetProjectView()->WidgetSelected(info);
        }
        return FALSE;
    }
    if (!multiple)
    {
        DeSelect();
    }

    if (info == NULL)
    {
        return FALSE;
    }

    if (!echo_to_project_view)
    {
        // this select came from project view,
        // check for switching top-level windows:
        MakeTopWindowVisible(info);
    }

    if (info->widget == NULL)
    {
        ErrorMsg("Internal Error: Selected widget has not been created");
        return FALSE;
    }

    GX_WIDGET *parent = info->widget->gx_widget_parent;

    if (parent && (parent->gx_widget_type == GX_TYPE_ACCORDION_MENU))
    {
        parent = parent->gx_widget_parent;
    }

    if (parent)
    {
        //check if the widget is a child of a menu list
        GX_WIDGET *menu = GX_NULL;
        
        switch (parent->gx_widget_type)
        {

        case GX_TYPE_MENU_LIST:
            menu = ((GX_MENU_LIST *)parent)->gx_menu_list_owner;
            break;

        case GX_TYPE_MENU:
            menu = parent;
            break;
        }

        //expand the menu if it's not expanded.
        if (menu)
        {
            menu_service_provider *provider = (menu_service_provider *)widget_factory::GetServiceProvider(menu->gx_widget_type);
            widget_info *menu_info = GetOpenProject()->FindWidgetInfo(menu);
            provider->AssignStyle(menu_info, menu_info->style | GX_STYLE_MENU_EXPANDED);
        }
    }

    // when the user selects a drop-list, open the drop list popoup:
    GX_WIDGET *drop_test = info->widget;
    while(drop_test)
    {
        if (drop_test->gx_widget_type == GX_TYPE_DROP_LIST)
        {
            GX_DROP_LIST *drop = (GX_DROP_LIST *) drop_test;
            gx_drop_list_open(drop);
            break;
        }
        drop_test = drop_test->gx_widget_parent;
    }


    WIDGET_SELECT_ENTRY new_select;
    new_select.draw_func = info->widget->gx_widget_draw_function;
    new_select.info = info;

    SelectedWidgets.Add(new_select);
    gx_system_dirty_mark(info->widget);

    mpSelectedParent = parent;

    if (echo_to_project_view)
    {
        if (SelectedWidgets.GetCount() <= 1)
        {
            GetProjectView()->WidgetSelected(info);
        }
        else
        {
            GetProjectView()->WidgetSelected(NULL);
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL target_screen::SelectWidget(GX_POINT &point, BOOL bMultiple)
{
    GX_WIDGET  *select;
    widget_info *info;

    GX_WIDGET *root = (GX_WIDGET *) get_root_window();

    if (!root)
    {
        return FALSE;
    }
    select = _gx_system_top_widget_find(root, point, 0);

    
    /* We must have widget information to select a widget
       (it could be a child of a template)
    */
    while(select)
    {
        info = GetOpenProject()->FindWidgetInfo(select);
        if (info)
        {
            break;
        }
        select = select->gx_widget_parent;
    }

    if (!select)
    {
        return FALSE;
    }


    if (select == root)
    {
        DeSelect();
        GetProjectView()->WidgetSelected(NULL);
    }
    else
    {
        // certain things we are not allowed to select
        if (select ->gx_widget_type == GX_TYPE_POPUP_LIST)
        {
            return FALSE;
        }

        if (select ->gx_widget_parent)
        {
            if (select->gx_widget_parent->gx_widget_type == GX_TYPE_VERTICAL_SCROLL ||
                select->gx_widget_parent->gx_widget_type == GX_TYPE_HORIZONTAL_SCROLL ||
                select->gx_widget_type == GX_TYPE_MENU_LIST)
            {
                select = select->gx_widget_parent;
            }
        }
        if (!IsWidgetSelected(select))
        {
            if (bMultiple && SelectedWidgets.GetCount() > 0)
            {
                if (select->gx_widget_parent !=
                    SelectedWidgets[0].info->widget->gx_widget_parent)
                {
                    ErrorMsg("When selecting multiple widgets, all selected widgets must have the same parent.");
                    return FALSE;
                }
            }
   
            SelectWidget(info, TRUE, bMultiple);
        }
    }
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAddWidget(int type)
{
    widget_info *info;
    folder_info *selected_folder = NULL;
    GX_WIDGET *parent = NULL;
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    widget_info *selected = GetSingleSelect();

    if (selected)
    {
        if (type == GX_TYPE_HORIZONTAL_SCROLL ||
            type == GX_TYPE_VERTICAL_SCROLL)
        {
            if (selected->basetype < GX_TYPE_WINDOW)
            {
                ErrorMsg("Scroll bars can only be added to Window parents");
                return;
            }
        }
        else if (type == GX_TYPE_MENU)
        {
            if ((selected->basetype != GX_TYPE_ACCORDION_MENU) &&
                (selected->basetype != GX_TYPE_TREE_VIEW) &&
                (selected->basetype != GX_TYPE_MENU))
            {
                ErrorMsg("The menu widget can only be added to Menu, Accordion Menu and Tree View widgets.");
                return;
            }
        }
        else if (type == GX_TYPE_GENERIC_SCROLL_WHEEL)
        {
            if (project_lib_version() < GX_VERSION_WITH_GENERIC_SCROLL_WHEEL)
            {
                ErrorMsg("The project GUIX library version does not support this widget type.");
                return;
            }
        }

        parent = selected->widget;
    }
    else
    {
        if (GetProjectView()->IsFolderSelected())
        {
            selected_folder = GetProjectView()->GetSelectedFolder();
            if (!selected_folder)
            {
                return;
            }
            parent = (GX_WIDGET *) get_root_window();
        }
    }
          
    widget_service_provider *provider = widget_factory::GetServiceProvider(type);
        
    if (provider)
    {
        if (!parent)
        {
            return;
        }
            
        info = provider->CreateNewInstance(parent);

        if (project->mHeader.snap_enabled)
        {
            int grid_spacing = project->mHeader.grid_spacing;
            int xoff = (info->size.gx_rectangle_left) % grid_spacing;
            int yoff = (info->size.gx_rectangle_top) % grid_spacing;

            info->size.gx_rectangle_left -= xoff;
            info->size.gx_rectangle_top -= yoff;
            info->size.gx_rectangle_right -= xoff;
            info->size.gx_rectangle_bottom -= yoff;

            widget_factory::MoveWidget(info, info->size);
        }

        if (info) 
        {
            if (parent == (GX_WIDGET *) get_root_window())
            {
                project->AddWidgetToFolder(selected_folder, info);
            }
            else
            {
                project->AddWidgetToParent(selected, info);
            }
            UndoManager()->AddEntry(UNDO_TYPE_INSERT_WIDGET, info);

            BOOL update_size = FALSE;
            switch(parent->gx_widget_type)
            {
            case GX_TYPE_VERTICAL_LIST:
            case GX_TYPE_POPUP_LIST:
                gx_vertical_list_children_position((GX_VERTICAL_LIST *) parent);
                update_size = TRUE;
                break;
                
            case GX_TYPE_HORIZONTAL_LIST:
                gx_horizontal_list_children_position((GX_HORIZONTAL_LIST *) parent);
                update_size = TRUE;
                break;

            case GX_TYPE_ACCORDION_MENU:
            case GX_TYPE_TREE_VIEW:
            case GX_TYPE_MENU:
                GetProjectView()->CheckParentRefresh(parent);
                update_size = TRUE;
                break;

            case GX_TYPE_GENERIC_SCROLL_WHEEL:
                gx_generic_scroll_wheel_children_position((GX_GENERIC_SCROLL_WHEEL*)parent);
                update_size = TRUE;
                break;

            default:
                SelectWidget(info, TRUE, FALSE);
                break;
            }

            if (update_size)
            {
                memcpy_s(&info->size, sizeof(info->size), &info->widget->gx_widget_size, sizeof(GX_RECTANGLE));
                GetPropsWin()->WidgetWasModified(info);
            }
        }   
    }
    else
    {
        ErrorMsg("Internal error: Service provider is not available");
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAddTemplate(widget_info *base)
{
    GX_WIDGET *parent = NULL;
    folder_info *selected_folder = NULL;
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    widget_info *selected = GetSingleSelect();

    if (selected)
    {
        if (selected ->basetype == GX_TYPE_DROP_LIST)
        {
            ErrorMsg("Child widgets must be added to the drop-list popup using the gx_drop_list_popup_get API");
            return;
        }
        if (base->basetype == GX_TYPE_HORIZONTAL_SCROLL ||
            base->basetype == GX_TYPE_VERTICAL_SCROLL)
        {
            if (selected->basetype < GX_TYPE_WINDOW)
            {
                ErrorMsg("Scroll bars can only be added to Window parents");
                return;
            }
        }
        parent = selected->widget;
    }
    else
    {
        if (GetProjectView()->IsFolderSelected())
        {
            selected_folder = GetProjectView()->GetSelectedFolder();
            if (!selected_folder)
            {
                return;
            }
            parent = (GX_WIDGET *) get_root_window();
        }

    }

    if (!parent)
    {
        return;
    }
    template_service_provider *provider = (template_service_provider *) widget_factory::GetServiceProvider(GX_TYPE_TEMPLATE);
        
    if (provider)
    {
        widget_info *info = provider->CreateNewInstance(parent, base);

        if (info) 
        {
            if (parent == (GX_WIDGET *) get_root_window()) 
            {
                project->AddWidgetToFolder(selected_folder, info);
            }
            else
            {
                project->AddWidgetToParent(selected, info);
            }
            UndoManager()->AddEntry(UNDO_TYPE_INSERT_WIDGET, info);

            switch(parent->gx_widget_type)
            {
            case GX_TYPE_VERTICAL_LIST:
            case GX_TYPE_POPUP_LIST:
                gx_vertical_list_children_position((GX_VERTICAL_LIST *) parent);
                break;
                
            case GX_TYPE_HORIZONTAL_LIST:
                gx_horizontal_list_children_position((GX_HORIZONTAL_LIST *) parent);
                break;

            case GX_TYPE_ACCORDION_MENU:
            case GX_TYPE_TREE_VIEW:
                GetProjectView()->CheckParentRefresh(parent);
                break;

            default:
                SelectWidget(info, TRUE, FALSE);
                break;
            }
        }   
    }
    else
    {
        ErrorMsg("Internal error: Service provider not available");
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::TestDropItem(resource_item *item,
     CPoint InPoint, BOOL shifted)
{
    int display;
    GX_POINT point;
    GX_WIDGET *root = (GX_WIDGET *) get_root_window();

    ScreenToClient(&InPoint);

    int scale = GetZoomScale();

    point.gx_point_x = (GX_VALUE)(InPoint.x * 100 / scale);
    point.gx_point_y = (GX_VALUE)(InPoint.y * 100 / scale);
    widget_info *info;

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }
    int DisplayIndex = GetProjectView()->GetActiveDisplay();

    GX_WIDGET *target = _gx_system_top_widget_find(root, point, 0);

    if (!target || target == root)
    {
        return;
    }
    info = project->FindWidgetInfo(target);
    widget_service_provider *provider = widget_factory::GetServiceProvider(target->gx_widget_type);

    if (!info || !provider)
    {
        return;
    }

    if (info->basetype == GX_TYPE_TEMPLATE)
    {
        // cannot change properties of item based on template 
        return;
    }

    int type = RES_TYPE_STRING;

    if (item->mpRes)
    {
        type = item->mpRes->type;
    }

    switch(type)
    {
    case RES_TYPE_FONT:
        UndoManager()->AddEntry(UNDO_TYPE_FONT, info, 0);
        provider->AssignFont(info, 0, project->GetResourceId(DisplayIndex, item->mpRes));
        break;

    case RES_TYPE_COLOR:
        if (shifted)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, info, 1);
            provider->AssignColor(info, 1, project->GetResourceId(DisplayIndex, item->mpRes));
        }
        else
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, info, 0);
            provider->AssignColor(info, 0, project->GetResourceId(DisplayIndex, item->mpRes));
        }
        break;

    case RES_TYPE_PIXELMAP:
        if (shifted)
        {
            UndoManager()->AddEntry(UNDO_TYPE_PIXELMAP, info, 1);
            provider->AssignPixelmap(info, 1, project->GetResourceId(DisplayIndex, item->mpRes));
        }
        else
        {
            UndoManager()->AddEntry(UNDO_TYPE_PIXELMAP, info, 0);
            provider->AssignPixelmap(info, 0, project->GetResourceId(DisplayIndex, item->mpRes));
        }
        break;

    case RES_TYPE_STRING:
        display = GetProjectView()->GetActiveDisplay();
        if (display >= 0)
        {
            UndoManager()->AddEntry(UNDO_TYPE_STRING, info, 0);
            string_table *pt = project->mDisplays[display].stable;
            provider->AssignText(info, 0, pt->GetResourceId(item->GetStringId()));
        }
        break;

    default:
        break;
    }
    // update the properties window
    GetPropsWin()->WidgetWasModified(info);
    project->SetModified();
}


///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAlignLeft()
{
    int index;
    int left = 65535;
    GX_RECTANGLE newsize;

    if (!SelectedWidgetsCanBeMoved())
    {
        return;
    }

    if (SelectedWidgets.GetCount() > 1)
    {
        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            if (SelectedWidgets[index].info->widget->gx_widget_size.gx_rectangle_left < left)
            {
                left = SelectedWidgets[index].info->widget->gx_widget_size.gx_rectangle_left;
            }
        }
        UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets);

        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            GX_WIDGET *selected = SelectedWidgets[index].info->widget;

            if (selected->gx_widget_size.gx_rectangle_left != left)
            {
                newsize = selected->gx_widget_size;
                gx_utility_rectangle_shift(&newsize, left - newsize.gx_rectangle_left, 0);
                widget_factory::MoveWidget(SelectedWidgets[index].info, newsize);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAlignTop()
{
    int index;
    int top = 65535;
    GX_RECTANGLE newsize;
    GX_WIDGET *selected;

    if (!SelectedWidgetsCanBeMoved())
    {
        return;
    }

    if (SelectedWidgets.GetCount() > 1)
    {
        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info->widget;

            if (selected->gx_widget_size.gx_rectangle_top < top)
            {
                top = selected->gx_widget_size.gx_rectangle_top;
            }
        }
        UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets);

        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info->widget;
            if (selected->gx_widget_size.gx_rectangle_top != top)
            {
                newsize = selected->gx_widget_size;
                gx_utility_rectangle_shift(&newsize, 0, top - newsize.gx_rectangle_top);
                widget_factory::MoveWidget(SelectedWidgets[index].info, newsize);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAlignRight()
{
    int index;
    int right = -65535;
    GX_RECTANGLE newsize;
    GX_WIDGET *selected;

    if (!SelectedWidgetsCanBeMoved())
    {
        return;
    }

    if (SelectedWidgets.GetCount() > 1)
    {
        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info->widget;

            if (selected->gx_widget_size.gx_rectangle_right > right)
            {
                right = selected->gx_widget_size.gx_rectangle_right;
            }
        }
        UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets);

        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info->widget;
            if (selected->gx_widget_size.gx_rectangle_right != right)
            {
                newsize = selected->gx_widget_size;
                gx_utility_rectangle_shift(&newsize, right - newsize.gx_rectangle_right, 0);
                widget_factory::MoveWidget(SelectedWidgets[index].info, newsize);
            }
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAlignBottom()
{
    int index;
    int bottom = -65535;
    GX_RECTANGLE newsize;
    GX_WIDGET *selected;

    if (!SelectedWidgetsCanBeMoved())
    {
        return;
    }

    if (SelectedWidgets.GetCount() > 1)
    {
        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info->widget;

            if (selected->gx_widget_size.gx_rectangle_bottom > bottom)
            {
                bottom = selected->gx_widget_size.gx_rectangle_bottom;
            }
        }
        UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets);

        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info->widget;
            if (selected->gx_widget_size.gx_rectangle_bottom != bottom)
            {
                newsize = selected->gx_widget_size;
                gx_utility_rectangle_shift(&newsize, 0, bottom - newsize.gx_rectangle_bottom);
                widget_factory::MoveWidget(SelectedWidgets[index].info, newsize);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnHSpaceEqually()
{
    GX_WIDGET *selected;
    GX_RECTANGLE size;
    int index;
    int used_space = 0;
    int left = 65535;
    int right = -65535;
    int total_space = 0;
    int inner_space = 0;
    int left_most_index;
    int right_neighbor_index;

    if (!SelectedWidgetsCanBeMoved())
    {
        return;
    }

    int count = SelectedWidgets.GetCount();

    if (count > 2)
    {
        for (index = 0; index < count; index++)
        {
            selected = SelectedWidgets[index].info->widget;
            if (selected->gx_widget_size.gx_rectangle_left < left)
            {
                left = selected->gx_widget_size.gx_rectangle_left;
                left_most_index = index;
            }
            if (selected->gx_widget_size.gx_rectangle_right > right)
            {
                right = selected->gx_widget_size.gx_rectangle_right;
            }
            used_space += (selected->gx_widget_size.gx_rectangle_right - selected->gx_widget_size.gx_rectangle_left) + 1;
        }

        total_space = right - left + 1;
        inner_space = ((total_space - used_space) + count) / (count - 1);

        // OK, starting with the left-most index, find the widget that is nearest moving right. Use the left
        // edge to determine "nearest", if widgets are overlapped and wider than each other then some strange
        // things might happen, nothing we can do about that.

        right_neighbor_index = FindNearestSelected(left_most_index, FALSE);
        UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets);

        while (right_neighbor_index >= 0)
        {
            selected = SelectedWidgets[right_neighbor_index].info->widget; 
            size = selected->gx_widget_size;
            left = SelectedWidgets[left_most_index].info->widget->gx_widget_size.gx_rectangle_right + inner_space;

            if (size.gx_rectangle_left != left)
            {
                gx_utility_rectangle_shift(&size, left - size.gx_rectangle_left, 0);
                widget_factory::MoveWidget(SelectedWidgets[right_neighbor_index].info, size);
            }
            left_most_index = right_neighbor_index;
            right_neighbor_index = FindNearestSelected(left_most_index, FALSE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnVSpaceEqually()
{
    GX_WIDGET *selected;
    GX_RECTANGLE size;
    int index;
    int used_space = 0;
    int top = 65535;
    int bottom = -65535;
    int total_space = 0;
    int inner_space = 0;
    int top_most_index;
    int below_neighbor_index;

    if (!SelectedWidgetsCanBeMoved())
    {
        return;
    }

    int count = SelectedWidgets.GetCount();

    if (count > 2)
    {
        for (index = 0; index < count; index++)
        {
            selected = SelectedWidgets[index].info->widget;
            if (selected->gx_widget_size.gx_rectangle_top < top)
            {
                top = selected->gx_widget_size.gx_rectangle_top;
                top_most_index = index;
            }
            if (selected->gx_widget_size.gx_rectangle_bottom > bottom)
            {
                bottom = selected->gx_widget_size.gx_rectangle_bottom;
            }
            used_space += (selected->gx_widget_size.gx_rectangle_bottom - selected->gx_widget_size.gx_rectangle_top) + 1;
        }

        total_space = bottom - top + 1;
        inner_space = ((total_space - used_space) + count) / (count - 1);

        // OK, starting with the left-most index, find the widget that is nearest moving right. Use the left
        // edge to determine "nearest", if widgets are overlapped and wider than each other then some strange
        // things might happen, nothing we can do about that.

        below_neighbor_index = FindNearestSelected(top_most_index, TRUE);
        UndoManager()->AddEntry(UNDO_TYPE_POSITION, SelectedWidgets);

        while (below_neighbor_index >= 0)
        {
            selected = SelectedWidgets[below_neighbor_index].info->widget; 
            size = selected->gx_widget_size;
            top = SelectedWidgets[top_most_index].info->widget->gx_widget_size.gx_rectangle_bottom + inner_space;

            if (size.gx_rectangle_top != top)
            {
                gx_utility_rectangle_shift(&size, 0, top - size.gx_rectangle_top);
                widget_factory::MoveWidget(SelectedWidgets[below_neighbor_index].info, size);
            }
            top_most_index = below_neighbor_index;
            below_neighbor_index = FindNearestSelected(top_most_index, TRUE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnMakeEqualWidth()
{
    int index;
    int widest = 0;
    int width;
    GX_RECTANGLE newsize;
    widget_info *selected;

    studiox_project *project = GetOpenProject();
    if (project && project->IsWidgetPositionLocked())
    {
        return;
    }

    if (SelectedWidgets.GetCount() > 1)
    {
        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info;

            width = selected->size.gx_rectangle_right - selected->size.gx_rectangle_left + 1;
            if (width > widest)
            {
                widest = width;
            }
        }
        UndoManager()->AddEntry(UNDO_TYPE_SIZE, SelectedWidgets);

        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info;
            width = selected->size.gx_rectangle_right - selected->size.gx_rectangle_left + 1;

            if (width != widest)
            {
                newsize = selected->size;
                newsize.gx_rectangle_right = newsize.gx_rectangle_left + widest - 1;

                if (selected->basetype == GX_TYPE_RADIAL_PROGRESS_BAR)
                {
                    newsize.gx_rectangle_bottom = newsize.gx_rectangle_top + widest - 1;
                }

                widget_factory::MoveWidget(selected, newsize);
                WidgetWasMoved(selected);
            }
        }
        project_view::CheckParentRefresh(SelectedWidgets[0].info->widget->gx_widget_parent);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnMakeEqualHeight()
{
    int index;
    int tallest = 0;
    int height;
    GX_RECTANGLE newsize;
    widget_info *selected;

    studiox_project *project = GetOpenProject();
    if (project && project->IsWidgetPositionLocked())
    {
        return;
    }

    if (SelectedWidgets.GetCount() > 1)
    {
        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info;

            height = selected->size.gx_rectangle_bottom - selected->size.gx_rectangle_top + 1;
            if (height > tallest)
            {
                tallest = height;
            }
        }
        UndoManager()->AddEntry(UNDO_TYPE_SIZE, SelectedWidgets);

        for (index = 0; index < SelectedWidgets.GetCount(); index++)
        {
            selected = SelectedWidgets[index].info;
            height = selected->size.gx_rectangle_bottom - selected->size.gx_rectangle_top + 1;

            if (height != tallest)
            {
                newsize = selected->size;
                newsize.gx_rectangle_bottom = newsize.gx_rectangle_top + tallest - 1;

                if (selected->basetype == GX_TYPE_RADIAL_PROGRESS_BAR)
                {
                    newsize.gx_rectangle_right = newsize.gx_rectangle_left + tallest - 1;
                }

                widget_factory::MoveWidget(selected, newsize);
                WidgetWasMoved(selected);
            }
        }
        project_view::CheckParentRefresh(SelectedWidgets[0].info->widget->gx_widget_parent);
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::OnAutosize()
{
    int index;
    widget_info *selected;
    studiox_project *project = GetOpenProject();

    if (project && project->IsWidgetPositionLocked())
    {
        return;
    }

    UndoManager()->AddEntry(UNDO_TYPE_SIZE, SelectedWidgets);

    for (index = 0; index < SelectedWidgets.GetCount(); index++)
    {
        selected = SelectedWidgets[index].info;

        if (selected && project)
        {
            widget_service_provider *provider = widget_factory::GetServiceProvider(selected->basetype);

            if (provider)
            {                
                provider->Autosize(selected);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::ZoomIn(int scale)
{
    SetZoomScale(scale);
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }
    display_info *info = &project->mDisplays[m_display_index];

    int width;
    int height;

    if (project->mDisplays[m_display_index].rotation_angle == GX_SCREEN_ROTATION_CW ||
        project->mDisplays[m_display_index].rotation_angle == GX_SCREEN_ROTATION_CCW)
    {
        width = (info->yres - 1) * scale / 100;
        height = (info->xres - 1) * scale / 100;
    }
    else
    {
        width = (info->xres - 1) * scale / 100;
        height = (info->yres - 1) * scale / 100;
    }

    // resize target screen
    SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE);

    target_frame *frame = (target_frame *)GetParent();
    frame->UpdateScrollSize();
    frame->Invalidate();

    UpdateWindowPos();
}

///////////////////////////////////////////////////////////////////////////////
int target_screen::FindNearestSelected(int start_index, BOOL vertical)
{
    int count = SelectedWidgets.GetCount();
    int test_index;
    int test_dist;
    int start_edge;
    int least_index = -1;
    int least_dist = 65535;

    if (start_index >= 0 && start_index < count)
    {
        if (vertical)
        {
            start_edge = SelectedWidgets[start_index].info->size.gx_rectangle_top;
        }
        else
        {
            start_edge = SelectedWidgets[start_index].info->size.gx_rectangle_left;
        }

        for (test_index = 0; test_index < count; test_index++)
        {
            if (test_index != start_index)
            {
                if (vertical)
                {
                    test_dist = SelectedWidgets[test_index].info->size.gx_rectangle_top - start_edge;
                }
                else
                {
                    test_dist = SelectedWidgets[test_index].info->size.gx_rectangle_left - start_edge;
                }

                if (test_dist > 0 && test_dist < least_dist)
                {
                    least_dist = test_dist;
                    least_index = test_index;
                }
            }
        }   
    }
    return least_index;
}

#define QUOTIENT 0x04c11db7

///////////////////////////////////////////////////////////////////////////////
void target_screen::crc32_init(void)
{
    int i, j;

    unsigned int crc;

    for (i = 0; i < 256; i++)
    {
        crc = i << 24;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80000000)
                crc = (crc << 1) ^ QUOTIENT;
            else
                crc = crc << 1;
        }
        crc_table[i] = crc;
    }
}

///////////////////////////////////////////////////////////////////////////////
unsigned int target_screen::calc_crc(unsigned char *data, int len)
{
    unsigned int        result;
    int                 i;

    crc32_init();

    result = *data++ << 24;
    result |= *data++ << 16;
    result |= *data++ << 8;
    result |= *data++;
    result = ~ result;
    len -=4;
    
    for (i=0; i<len; i++)
    {
        result = (result << 8 | *data++) ^ crc_table[result >> 24];
    }
    
    return ~result;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::canvas_to_bitmap(CString filename, GX_CANVAS *canvas)
{
    CFile *p_file = new CFile();

    if (!p_file->Open(filename, CFile::modeCreate | CFile::modeWrite))
    {
        CString msg;
        msg.Format(_T("Could not open bmp file:\n%s\nPlease check file path."), filename);
        ErrorMsg(msg);
        delete p_file;
        return;
    }

    BITMAPFILEHEADER file_header;
    GX_BMP_INFO info;

    GX_DISPLAY *display;
    GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;

    display = canvas->gx_canvas_display;
    driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)(display->gx_display_driver_data);
    info = driver_instance->win32_driver_bmpinfo;
    info.gx_bmp_header.biHeight *= -1;

    memset(&file_header, 0, sizeof(BITMAPFILEHEADER));

    file_header.bfType = 'MB';
    file_header.bfSize = canvas->gx_canvas_memory_size + sizeof(BITMAPFILEHEADER) + sizeof(GX_BMP_INFO);
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(GX_BMP_INFO);
    p_file->Write(&file_header, sizeof(BITMAPFILEHEADER));
    p_file->Write(&info, sizeof(GX_BMP_INFO));
    p_file->Write(canvas->gx_canvas_memory, canvas->gx_canvas_memory_size);

    p_file->Close();
    delete p_file;
}

///////////////////////////////////////////////////////////////////////////////
void target_screen::target_screen_to_bitmap(CString filename, GX_CANVAS *canvas)
{
    LPBYTE lpBits;              // memory pointer  
    CDC *dc = GetDC();
    HDC hDC = dc->GetSafeHdc();
    CRect rect;

    GetClientRect(&rect);

    HDC hdcMem = CreateCompatibleDC(hDC);
    HBITMAP hbmMem = CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());

    SelectObject(hdcMem, hbmMem);

    BitBlt(hdcMem, 0, 0, rect.Width(), rect.Height(), hDC, 0, 0, SRCCOPY);

    GX_BMP_INFO info;
    GX_DISPLAY *display;
    GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;

    display = canvas->gx_canvas_display;
    driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)(display->gx_display_driver_data);

    BITMAP bitmap;
    GetObject(hbmMem, sizeof(BITMAP), &bitmap);

    info = driver_instance->win32_driver_bmpinfo;
    info.gx_bmp_header.biWidth = bitmap.bmWidth;
    info.gx_bmp_header.biHeight = -bitmap.bmHeight;
    info.gx_bmp_header.biSizeImage = ((bitmap.bmWidth * info.gx_bmp_header.biBitCount + 31) / 32) * 4 * bitmap.bmHeight;

    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, info.gx_bmp_header.biSizeImage);

    // Removed by Guo. This assert cannot be build by release.
    //assert(lpBits);

    // Retrieve the color table (RGBQUAD array) and the bits  
    // (array of palette indices) from the DIB.  
    GetDIBits(hdcMem, hbmMem, 0, info.gx_bmp_header.biHeight, lpBits, (LPBITMAPINFO)&info, DIB_RGB_COLORS);

    // Create the .BMP file.  
    CFile *p_file = new CFile();

    if (!p_file->Open(filename, CFile::modeCreate | CFile::modeWrite))
    {
        CString msg;
        msg.Format(_T("Could not open bmp file:\n%s\nPlease check file path."), filename);
        ErrorMsg(msg);
        delete p_file;
        return;
    }

    BITMAPFILEHEADER file_header;
    memset(&file_header, 0, sizeof(BITMAPFILEHEADER));

    file_header.bfType = 'MB'; 
    file_header.bfSize = info.gx_bmp_header.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(GX_BMP_INFO);
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(GX_BMP_INFO);

    p_file->Write(&file_header, sizeof(BITMAPFILEHEADER));
    p_file->Write(&info, sizeof(GX_BMP_INFO));
    p_file->Write(lpBits, info.gx_bmp_header.biSizeImage);

    p_file->Close();
    delete p_file;

    DeleteDC(hdcMem);
    DeleteObject(hbmMem); 
    GlobalFree((HGLOBAL)lpBits);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT target_screen::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    GX_WINDOW_ROOT *root;
    GX_CANVAS *canvas;
    LRESULT result = 0;
    int xshift, yshift;
    GX_WIDGET *top;
    widget_info *base = NULL;
    studiox_project *project;
    CStringArray param;
    int x, y, display;

    switch(wParam)
    {
    case TEST_GENERATE_CHECKSUM:
        //SelectWidget(NULL, FALSE, FALSE); 
        if(GetOpenProject())
        {
            DeSelect();
            GetProjectView()->DeSelect();

            _gx_system_canvas_refresh();
            root = get_root_window();
            canvas = root->gx_window_root_canvas;
            result = calc_crc((unsigned char *)canvas->gx_canvas_memory, canvas->gx_canvas_memory_size);
        }
        break;

    case TEST_GENERATE_SCREENSHOT:
        if (GetOpenProject())
        {
            root = get_root_window();
            canvas_to_bitmap(GetTestingParam(0), root->gx_window_root_canvas);
        }
        break;

    case TEST_DESELECT:
        DeSelect();
        GetProjectView()->DeSelect();
        break;

    case TEST_DIRTY_ROOT_WINDOW:
        root = get_root_window();
        gx_system_dirty_mark(root);
        break;

    case TEST_MOVE_WINDOW:
        project = GetOpenProject();
        xshift = yshift = lParam;
        root = get_root_window();
        top = root->gx_widget_first_child;
        SelectWidget(project->FindWidgetInfo(top), FALSE, FALSE);
        ShiftSelectedWidgets(xshift, yshift);
        break;

    case TEST_MOVE_SELECTED:
        xshift = yshift = lParam;
        ShiftSelectedWidgets(xshift, yshift);
        break;

    case TEST_ADD_WIDGET:
        OnAddWidget(lParam);
        break;

    case TEST_ADD_TEMPLATE:
        project = GetOpenProject();
        if (project)
        {
            SplitString(GetTestingParam(0), ',', &param);
            display = _tstoi(param.GetAt(1));
            if (display >= 0)
            {
                base = project->FindWidgetInfo(project->mDisplays[display].GetFirstChildFolder(), (CString)param.GetAt(0), FALSE);
            }
            if (base)
            {
                OnAddTemplate(base);
            }
        }
        break;

    case TEST_ZOOM_IN:
        ZoomIn(lParam);
        break;

    case TEST_LBUTTON_DOWN:
        SplitString(GetTestingParam(0), ',', &param);
        x = _tstoi(param.GetAt(0));
        y = _tstoi(param.GetAt(1));
        SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
        break;

    case TEST_LBUTTON_UP:
        SplitString(GetTestingParam(0), ',', &param);
        x = _tstoi(param.GetAt(0));
        y = _tstoi(param.GetAt(1));
        SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
        break;

    case TEST_MOUSEMOVE:
    case TEST_MOUSEMOVE_POST:
        if (wParam == TEST_MOUSEMOVE_POST)
        {
            SplitString(GetTestingParam(1), ',', &param);
        }
        else
        {
            SplitString(GetTestingParam(0), ',', &param);
        }
        x = _tstoi(param.GetAt(0));
        y = _tstoi(param.GetAt(1));
        SendMessage(WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(x, y));
        break;
     
    default:
        break;
    }
    return result;
}
