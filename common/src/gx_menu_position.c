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
/**   Menu Management (Menu)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_menu.h"
#include "gx_widget.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_menu_one_level_position                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions first level items for a menu widget.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    menu                                  Pointer to menu control block */
/*    indentation                           Menu indentations             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_widget_detach                     Detach a widget from its      */
/*                                            parent                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
UINT  _gx_menu_one_level_position(GX_MENU *menu, GX_VALUE indentation)
{
GX_RECTANGLE *menu_size = &menu -> gx_widget_size;
GX_WIDGET    *list = (GX_WIDGET *)&menu -> gx_menu_list;
GX_WIDGET    *item;
GX_RECTANGLE  size;
GX_VALUE      height;
GX_VALUE      width;
INT           max_width = 0;
GX_MENU_LIST *item_list;

    size.gx_rectangle_left = (GX_VALUE)(menu_size -> gx_rectangle_left + indentation);
    size.gx_rectangle_right = (GX_VALUE)(menu_size -> gx_rectangle_right);
    size.gx_rectangle_top = (GX_VALUE)(menu_size -> gx_rectangle_bottom + 1);
    size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top - 1);

    /* Reposition menu list. */
    _gx_widget_resize(list, &size);

    if (menu -> gx_widget_style & GX_STYLE_MENU_EXPANDED)
    {
        if (!list -> gx_widget_parent)
        {
            _gx_widget_link(menu -> gx_widget_parent, (GX_WIDGET *)list);
        }
    }
    else
    {
        if (list -> gx_widget_parent)
        {
            _gx_widget_detach((GX_WIDGET *)list);
        }
    }

    item = list -> gx_widget_first_child;
    size.gx_rectangle_bottom = menu_size -> gx_rectangle_bottom;

    /* Reposition items of menu list. */
    while (item)
    {
        if (item -> gx_widget_type == GX_TYPE_MENU_LIST)
        {
            item = item -> gx_widget_next;
            continue;
        }

        height = (GX_VALUE)(item -> gx_widget_size.gx_rectangle_bottom - item -> gx_widget_size.gx_rectangle_top + 1);
        width = (GX_VALUE)(item -> gx_widget_size.gx_rectangle_right - item -> gx_widget_size.gx_rectangle_left + 1);

        if (width > max_width)
        {
            max_width = width;
        }

        size.gx_rectangle_top = (GX_VALUE)(size.gx_rectangle_bottom + 1);
        size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + height - 1);
        size.gx_rectangle_right = (GX_VALUE)(size.gx_rectangle_left + width - 1);

        _gx_widget_resize(item, &size);

        switch (item -> gx_widget_type)
        {
        case GX_TYPE_MENU:
            item_list = &((GX_MENU *)item) -> gx_menu_list;
            if (item_list -> gx_widget_parent)
            {
                _gx_widget_shift((GX_WIDGET *)item_list,
                                 (GX_VALUE)(size.gx_rectangle_left + indentation - item_list -> gx_widget_size.gx_rectangle_left),
                                 (GX_VALUE)(size.gx_rectangle_bottom + 1 - item_list -> gx_widget_size.gx_rectangle_top), GX_FALSE);

                height = (GX_VALUE)(item_list -> gx_widget_size.gx_rectangle_bottom - size.gx_rectangle_top + 1);
                width = (GX_VALUE)(item_list -> gx_widget_size.gx_rectangle_right - size.gx_rectangle_left + 1);

                if (width > max_width)
                {
                    max_width = width;
                }
            }
            break;

        default:
            break;
        }

        size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + height - 1);

        item = item -> gx_widget_next;
    }

    if (menu -> gx_widget_style & GX_STYLE_MENU_EXPANDED)
    {
        size.gx_rectangle_top = (GX_VALUE)(menu_size -> gx_rectangle_bottom + 1);
        size.gx_rectangle_right = (GX_VALUE)(size.gx_rectangle_left + max_width - 1);
        _gx_widget_resize(list, &size);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_menu_position                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions a menu and its menu list.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    menu                                  Pointer to menu control block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_menu_one_level_position           Position a menu widget        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
UINT  _gx_menu_position(GX_MENU *menu, GX_VALUE indentation)
{
UINT          status = GX_SUCCESS;
GX_WIDGET    *parent = (GX_WIDGET *)menu;
GX_WIDGET    *child;
GX_MENU_LIST *child_list;

    child = menu -> gx_menu_list.gx_widget_first_child;

    /* Reposition items of menu list. */
    while (child)
    {
        if (child -> gx_widget_type == GX_TYPE_MENU)
        {
            child_list = &((GX_MENU *)child) -> gx_menu_list;

            if (child_list -> gx_widget_first_child)
            {
                child = child_list -> gx_widget_first_child;
                continue;
            }
            else if (child_list -> gx_widget_parent)
            {
                _gx_widget_detach((GX_WIDGET *)child_list);
            }
        }

        while ((child -> gx_widget_next == GX_NULL) && (child != parent))
        {
            /* Child parent is menu list. */
            child_list = (GX_MENU_LIST *)child -> gx_widget_parent;
            child = child_list -> gx_menu_list_owner;

            _gx_menu_one_level_position((GX_MENU *)child, indentation);
        }

        if (child == parent)
        {
            break;
        }

        child = child -> gx_widget_next;
    }

    /* Return completion status code. */
    return(status);
}

