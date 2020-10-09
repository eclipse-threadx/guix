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
#include "gx_menu.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_tree_view.h"
#include "gx_window.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_position                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions a tree menu.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Tree view control block       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_client_child_get     Get the first client child    */
/*    _gx_widget_next_client_child_get      Get the next client child     */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_widget_width_get                  Get widget width              */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_menu_position                     Position a menu widget        */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_widget_hide                       Hide a widget                 */
/*    _gx_widget_show                       Show a widget                 */
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
UINT  _gx_tree_view_position(GX_TREE_VIEW *tree)
{
GX_WIDGET    *child;
GX_RECTANGLE  size;
GX_RECTANGLE *client;
GX_VALUE      height;
GX_VALUE      width;
GX_VALUE      bottom;
GX_VALUE      max_right = 0;
GX_MENU_LIST *list;
GX_SCROLLBAR *scroll;

    client = &tree -> gx_window_client;

    size.gx_rectangle_left = (GX_VALUE)(client -> gx_rectangle_left + tree -> gx_tree_view_indentation + tree -> gx_tree_view_x_shift);
    bottom = (GX_VALUE)(client -> gx_rectangle_top - 1 + tree -> gx_tree_view_y_shift);

    child = _gx_widget_first_client_child_get((GX_WIDGET *)tree);

    /* Reposition tree view items. */
    while (child)
    {
        if (child -> gx_widget_type == GX_TYPE_MENU_LIST)
        {
            child = _gx_widget_next_client_child_get(child);
            continue;
        }

        _gx_widget_height_get(child, &height);
        _gx_widget_width_get(child, &width);
        size.gx_rectangle_top = (GX_VALUE)(bottom + 1);
        size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + height - 1);
        size.gx_rectangle_right = (GX_VALUE)(size.gx_rectangle_left + width - 1);

        _gx_widget_resize((GX_WIDGET *)child, &size);

        if (child -> gx_widget_type == GX_TYPE_MENU)
        {
            _gx_menu_position((GX_MENU *)child, tree -> gx_tree_view_indentation);

            list = &((GX_MENU *)child) -> gx_menu_list;

            if (list -> gx_widget_parent && list -> gx_widget_first_child)
            {
                bottom = list -> gx_widget_size.gx_rectangle_bottom;

                if (list -> gx_widget_size.gx_rectangle_right > max_right)
                {
                    max_right = list -> gx_widget_size.gx_rectangle_right;
                }
            }
            else
            {
                if (list -> gx_widget_parent)
                {
                    _gx_widget_detach((GX_WIDGET *)list);
                }

                bottom = size.gx_rectangle_bottom;

                if (size.gx_rectangle_right > max_right)
                {
                    max_right = size.gx_rectangle_right;
                }
            }
        }
        else
        {
            bottom = size.gx_rectangle_bottom;
        }

        child -> gx_widget_status &= (ULONG)(~GX_STATUS_ACCEPTS_FOCUS);
        child = _gx_widget_next_client_child_get(child);
    }

    tree -> gx_tree_view_tree_width = (GX_VALUE)(max_right - (client -> gx_rectangle_left + tree -> gx_tree_view_x_shift) + 1);
    tree -> gx_tree_view_tree_height = (GX_VALUE)(bottom - (client -> gx_rectangle_top + tree -> gx_tree_view_y_shift) + 1);

    _gx_window_scrollbar_find((GX_WINDOW *)tree, (USHORT)GX_TYPE_HORIZONTAL_SCROLL, &scroll);

    width = (GX_VALUE)(client -> gx_rectangle_right - client -> gx_rectangle_left + 1);
    height = (GX_VALUE)(client -> gx_rectangle_bottom - client -> gx_rectangle_top + 1);

    if (scroll)
    {
        _gx_scrollbar_reset(scroll, GX_NULL);

        if (tree -> gx_tree_view_tree_width <= width)
        {
            _gx_widget_hide((GX_WIDGET *)scroll);
        }
        else
        {
            _gx_widget_show((GX_WIDGET *)scroll);
        }
    }

    _gx_window_scrollbar_find((GX_WINDOW *)tree, GX_TYPE_VERTICAL_SCROLL, &scroll);

    if (scroll)
    {
        _gx_scrollbar_reset(scroll, GX_NULL);

        if (tree -> gx_tree_view_tree_height <= height)
        {
            _gx_widget_hide((GX_WIDGET *)scroll);
        }
        else
        {
            _gx_widget_show((GX_WIDGET *)scroll);
        }
    }

    if ((tree -> gx_widget_style & GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES) &&
        (tree -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        _gx_system_dirty_mark((GX_WIDGET *)tree);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

