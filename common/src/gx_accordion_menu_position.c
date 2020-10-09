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
/**   Accordion Menu Management (Menu)                                    */
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
/*    _gx_accordion_menu_one_level_position               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions first level items for an accordion menu.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Accordion menu control block  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_widget_shift                      Shift a widget                */
/*    _gx_widget_resize                     Resize a widget               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_accordion_menu_position                                         */
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
static UINT  _gx_accordion_menu_one_level_position(GX_ACCORDION_MENU *accordion)
{
GX_WIDGET    *child = accordion -> gx_widget_first_child;
GX_RECTANGLE  size;
GX_RECTANGLE  client;
GX_VALUE      height;
GX_VALUE      bottom;
GX_VALUE      border_width;
GX_MENU_LIST *list;

    _gx_widget_border_width_get((GX_WIDGET *)accordion, &border_width);
    _gx_widget_client_get((GX_WIDGET *)accordion, border_width, &client);

    size.gx_rectangle_left = client.gx_rectangle_left;
    size.gx_rectangle_right = client.gx_rectangle_right;
    size.gx_rectangle_top = client.gx_rectangle_top;
    size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top - 1);

    bottom = size.gx_rectangle_bottom;
    accordion -> gx_accordion_menu_expand_item = GX_NULL;

    /* Reposition accordion menu items. */
    while (child)
    {
        switch (child -> gx_widget_type)
        {
        case GX_TYPE_MENU:
            list = &((GX_MENU *)child) -> gx_menu_list;

            if (child -> gx_widget_style & GX_STYLE_MENU_EXPANDED)
            {
                if (!accordion -> gx_accordion_menu_expand_item && list -> gx_widget_first_child)
                {
                    accordion -> gx_accordion_menu_expand_item = child;
                }
                else
                {
                    child -> gx_widget_style &= (ULONG)(~GX_STYLE_MENU_EXPANDED);
                    _gx_widget_detach((GX_WIDGET *)list);

                    list -> gx_widget_size.gx_rectangle_bottom = (GX_VALUE)(list -> gx_widget_size.gx_rectangle_top - 1);
                }
            }

            _gx_widget_height_get(child, &height);

            size.gx_rectangle_top = (GX_VALUE)(bottom + 1);
            size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + height - 1);

            _gx_widget_resize(child, &size);

            if (list -> gx_widget_parent)
            {
                _gx_widget_shift((GX_WIDGET *)list,
                                 (GX_VALUE)(size.gx_rectangle_left - list -> gx_widget_size.gx_rectangle_left),
                                 (GX_VALUE)(size.gx_rectangle_bottom + 1 - list -> gx_widget_size.gx_rectangle_top), GX_FALSE);

                bottom = list -> gx_widget_size.gx_rectangle_bottom;
            }
            else
            {
                bottom = size.gx_rectangle_bottom;
            }
            break;

        case GX_TYPE_MENU_LIST:
            break;

        default:
            _gx_widget_height_get(child, &height);
            size.gx_rectangle_top = (GX_VALUE)(bottom + 1);
            size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + height - 1);

            _gx_widget_resize(child, &size);
            bottom = size.gx_rectangle_bottom;
            break;
        }

        child -> gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;
        child = child -> gx_widget_next;
    }

    size = accordion -> gx_widget_size;
    size.gx_rectangle_bottom = (GX_VALUE)(bottom + border_width);

    /* Resize accordion menu. */
    _gx_widget_resize((GX_WIDGET *)accordion, &size);

    /* Return completion status code. */
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_position                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions an accordion menu and its items.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Accordion menu control block  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_menu_one_level_position           Position a menu widget        */
/*    _gx_accordion_one_levelmenu_position  Position an accordion menu    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT  _gx_accordion_menu_position(GX_ACCORDION_MENU *accordion)
{
GX_WIDGET    *parent = (GX_WIDGET *)accordion;
GX_WIDGET    *child;
GX_MENU_LIST *child_list;

    child = accordion -> gx_widget_first_child;

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
        else if (child -> gx_widget_type == GX_TYPE_ACCORDION_MENU)
        {
            if (child -> gx_widget_first_child)
            {
                child = child -> gx_widget_first_child;
                continue;
            }
        }

        while ((child -> gx_widget_next == GX_NULL) && (child != parent))
        {
            child = child -> gx_widget_parent;

            if (child -> gx_widget_type == GX_TYPE_MENU_LIST)
            {
                child_list = (GX_MENU_LIST *)child;
                child = child_list -> gx_menu_list_owner;
            }

            if ((child -> gx_widget_type == GX_TYPE_MENU))
            {
                _gx_menu_one_level_position((GX_MENU *)child, 0);
            }
            else
            {
                _gx_accordion_menu_one_level_position((GX_ACCORDION_MENU *)child);
            }
        }

        if (child == parent)
        {
            break;
        }

        child = child -> gx_widget_next;
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

