/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Tree View Management (View)                                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_tree_view.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tree_view_selected_detect                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to test if a menu contain selected item.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to the tree view      */
/*                                            control block               */
/*    test_menu                             Menu for test                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tree_view_selected_detect            Detect if a menu contain      */
/*                                             selected item              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_menu_pen_down_event_process                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _tree_view_selected_detect(GX_TREE_VIEW *tree, GX_MENU *test_menu)
{
GX_MENU_LIST *list = &test_menu -> gx_menu_list;
GX_WIDGET    *child;
GX_BOOL       found = GX_FALSE;

    if (!tree -> gx_tree_view_selected)
    {
        return found;
    }
    child = list -> gx_widget_first_child;

    while (child)
    {
        if (child == tree -> gx_tree_view_selected)
        {
            found = GX_TRUE;
            break;
        }

        if (child -> gx_widget_type == GX_TYPE_MENU)
        {
            found = _tree_view_selected_detect(tree, (GX_MENU *)child);

            if (found)
            {
                break;
            }
        }
        child = child -> gx_widget_next;
    }

    return found;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_item_find                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to find a tree view item that is           */
/*    intersecting the specified horizontal line.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    start                                  Widget to start searching    */
/*    line_y                                 Y coordinate of the          */
/*                                             horizontal line            */
/*    returned_item                          Pointer to retrieved item    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_client_child_get     Get the first client child    */
/*    _gx_widget_next_client_child_get      Get the next client child     */
/*    _gx_tree_view_item_find               Find tree view item with test */
/*                                            point                       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_menu_pen_down_event_process                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_tree_view_item_find(GX_WIDGET *start, GX_VALUE line_y, GX_WIDGET **returned_item)
{
UINT          status = GX_SUCCESS;
GX_WIDGET    *child;
GX_WIDGET    *next;
GX_WIDGET    *found = NULL;
GX_MENU_LIST *list;
GX_BOOL       search_child;

    child = _gx_widget_first_client_child_get((GX_WIDGET *)start);

    while (child && child -> gx_widget_type == GX_TYPE_MENU_LIST)
    {
        child = _gx_widget_next_client_child_get(child);
    }

    while (child)
    {
        /* Pick up next client child. */
        next = _gx_widget_next_client_child_get(child);

        while (next && next -> gx_widget_type == GX_TYPE_MENU_LIST)
        {
            next = _gx_widget_next_client_child_get(next);
        }

        /* Check if the widget if intersecting the specified horizontal line. */
        if ((line_y >= child -> gx_widget_size.gx_rectangle_top) &&
            (line_y <= child -> gx_widget_size.gx_rectangle_bottom))
        {
            /* Line y is in current item. */
            found = child;
            break;
        }

        search_child = GX_FALSE;

        if (next)
        {
            if (line_y < next -> gx_widget_size.gx_rectangle_top)
            {
                search_child = GX_TRUE;
            }
            else if (line_y <= next -> gx_widget_size.gx_rectangle_bottom)
            {
                /* Line y is inside next item. */
                found = next;
                break;
            }
        }
        else
        {
            search_child = GX_TRUE;
        }

        if (search_child)
        {
            /* Line y is before next item. */
            if (child -> gx_widget_type == GX_TYPE_MENU)
            {
                list = &((GX_MENU *)child) -> gx_menu_list;

                if (list -> gx_widget_status & GX_STATUS_VISIBLE)
                {
                    child = list -> gx_widget_first_child;
                    continue;
                }
            }
            else
            {
                /* Not found. */
                break;
            }
        }

        child = next;
    }

    *returned_item = found;

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_root_icon_rect_get                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to get the root icon draw area of a tree   */
/*    view item.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to the tree view      */
/*                                            control block               */
/*    item                                  Pointer to the tree view item */
/*                                            whose root icon draw area is*/
/*                                            retrieved                   */
/*    root_rect                             Retrieved root icon draw area */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_pixelmap_get               Get pixelmap by supplied ID   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_menu_pen_down_event_process                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_tree_view_root_icon_rect_get(GX_TREE_VIEW *tree, GX_WIDGET *item, GX_RECTANGLE *root_rect)
{
GX_PIXELMAP *map;

    _gx_widget_pixelmap_get((GX_WIDGET *)tree, tree -> gx_tree_view_expand_pixelmap_id, &map);

    if (map)
    {
        root_rect -> gx_rectangle_left = (GX_VALUE)(item -> gx_widget_size.gx_rectangle_left - (tree -> gx_tree_view_indentation >> 1));
        root_rect -> gx_rectangle_left = (GX_VALUE)(root_rect -> gx_rectangle_left - (map -> gx_pixelmap_width >> 1));
        root_rect -> gx_rectangle_top = (GX_VALUE)(item -> gx_widget_size.gx_rectangle_top + item -> gx_widget_size.gx_rectangle_bottom - 1);
        root_rect -> gx_rectangle_top = (GX_VALUE)(root_rect -> gx_rectangle_top >> 1);
        root_rect -> gx_rectangle_top = (GX_VALUE)(root_rect -> gx_rectangle_top - (map -> gx_pixelmap_height >> 1));
        root_rect -> gx_rectangle_right = (GX_VALUE)((root_rect -> gx_rectangle_left + map -> gx_pixelmap_width - 1));
        root_rect -> gx_rectangle_bottom = (GX_VALUE)((root_rect -> gx_rectangle_top + map -> gx_pixelmap_height - 1));
    }
    else
    {
        memset(root_rect, 0, sizeof(GX_RECTANGLE));
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_menu_pen_down_event_process                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to process pen down event for the tree     */
/*    view.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to tree view control  */
/*                                            block                       */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_tree_view_item_find               Find tree view item with test */
/*                                            point                       */
/*    _gx_tree_view_root_icon_rect_get      Retrieve root icon rectangle  */
/*                                            of specified tree view item */
/*    _tree_view_selected_detect            Detect if a menu contain      */
/*                                            selected item               */
/*    _gx_tree_view_selected_set            Set new selected item         */
/*    _gx_tree_view_position                Position a tree view          */
/*    _gx_system_dirty_partial_add          Mark partial area of a widget */
/*                                            as dirty                    */
/*    _gx_system_top_widget_find            Find top widget that contain  */
/*                                            test point                  */
/*    _gx_utility_rectangle_point_detect    Test if the point is resides  */
/*                                            in the rectangle.           */
/*    _gx_window_event_process              Default window event process  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_view_event_process                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_tree_menu_pen_down_event_process(GX_TREE_VIEW *tree, GX_EVENT *event_ptr)
{
GX_WIDGET   *widget = (GX_WIDGET *)tree;
GX_WIDGET   *child = GX_NULL;
GX_RECTANGLE root_rect;
GX_POINT    *point;
GX_BOOL      check_selection = GX_FALSE;

    point = &event_ptr -> gx_event_payload.gx_event_pointdata;

    /* Find tree view item that is intersecting the horizontal line of the click point. */
    _gx_tree_view_item_find((GX_WIDGET *)tree, point -> gx_point_y, &child);

    if (child && (child -> gx_widget_type == GX_TYPE_MENU) &&
        ((GX_MENU *)child) -> gx_menu_list.gx_widget_first_child)
    {
        _gx_tree_view_root_icon_rect_get(tree, child, &root_rect);

        if (_gx_utility_rectangle_point_detect(&root_rect, *point))
        {
            if (child -> gx_widget_style & GX_STYLE_MENU_EXPANDED)
            {
                child -> gx_widget_style &= (ULONG)(~GX_STYLE_MENU_EXPANDED);

                if (_tree_view_selected_detect(tree, (GX_MENU *)child))
                {
                    _gx_tree_view_selected_set(tree, child);
                }
            }
            else
            {
                child -> gx_widget_style |= GX_STYLE_MENU_EXPANDED;
            }

            _gx_tree_view_position(tree);

            root_rect = tree -> gx_window_client;
            if (!(tree -> gx_widget_style & GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES))
            {
                root_rect.gx_rectangle_top = child -> gx_widget_size.gx_rectangle_top;
            }

            _gx_system_dirty_partial_add((GX_WIDGET *)tree, &root_rect);
        }
        else
        {
            check_selection = GX_TRUE;
        }
    }
    else
    {
        check_selection = GX_TRUE;
        _gx_window_event_process((GX_WINDOW *)widget, event_ptr);
    }

    if (check_selection)
    {
        child = _gx_system_top_widget_find((GX_WIDGET *)tree, *point, GX_STATUS_SELECTABLE);

        if (child)
        {
            if ((child -> gx_widget_type == GX_TYPE_MENU) ||
                (child -> gx_widget_parent -> gx_widget_type == GX_TYPE_MENU_LIST))
            {
                _gx_tree_view_selected_set(tree, child);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_event_process                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes an event for the tree menu.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to tree menu control  */
/*                                            block                       */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_event_process              Default window event process  */
/*    _gx_tree_view_position                Position the children of the  */
/*                                            tree view                   */
/*    _gx_tree_menu_pen_down_event_process  Handle pen down event         */
/*    _gx_tree_view_scroll                  Scroll tree view client area  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_tree_view_event_process(GX_TREE_VIEW *tree, GX_EVENT *event_ptr)
{
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        _gx_window_event_process((GX_WINDOW *)tree, event_ptr);
        if (tree -> gx_widget_parent && (tree -> gx_widget_parent -> gx_widget_type != GX_TYPE_MENU_LIST))
        {
            _gx_tree_view_position(tree);
        }
        break;

    case GX_EVENT_PEN_DOWN:
        _gx_tree_menu_pen_down_event_process(tree, event_ptr);
        break;

    case GX_EVENT_VERTICAL_SCROLL:
        _gx_tree_view_scroll(tree, 0, (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_intdata[1] - event_ptr -> gx_event_payload.gx_event_intdata[0]));
        break;

    case GX_EVENT_HORIZONTAL_SCROLL:
        _gx_tree_view_scroll(tree, (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_intdata[1] - event_ptr -> gx_event_payload.gx_event_intdata[0]), 0);
        break;

    default:
        return _gx_window_event_process((GX_WINDOW *)tree, event_ptr);
    }

    return(GX_SUCCESS);
}

