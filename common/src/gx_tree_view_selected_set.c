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
/**   Tree View Management (Tree View)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_tree_view.h"
#include "gx_system.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_selected_visible                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to make selected item visible.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to the tree view      */
/*                                            control block               */
/*    selected                              New selected item             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_tree_view_position                Reposition tree view childdren*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_view_selected_set                                          */
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
static UINT _gx_tree_view_selected_visible(GX_TREE_VIEW *tree)
{
GX_WIDGET    *parent;
GX_WIDGET    *selected = tree -> gx_tree_view_selected;
GX_MENU_LIST *list;

    parent = selected -> gx_widget_parent;
    while (parent != (GX_WIDGET *)tree)
    {
        if (parent -> gx_widget_type == GX_TYPE_MENU_LIST)
        {
            list = (GX_MENU_LIST *)parent;
            list -> gx_menu_list_owner -> gx_widget_style |= GX_STYLE_MENU_EXPANDED;
            parent = list -> gx_menu_list_owner;
        }
        else
        {
            parent = parent -> gx_widget_parent;
        }
    }

    _gx_tree_view_position(tree);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_selected_set                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function set selected item for the specified tree view.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to the tree view      */
/*                                            control block               */
/*    selected                              New selected item             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark the widget area as dirty */
/*    _gx_widget_event_generate             Create an event and send it to*/
/*                                            the widget parent           */
/*    _gx_tree_view_selected_visible        Make selected item visible.   */
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
UINT  _gx_tree_view_selected_set(GX_TREE_VIEW *tree, GX_WIDGET *selected)
{
    if (tree -> gx_tree_view_selected == selected)
    {
        return GX_SUCCESS;
    }

    if (tree -> gx_tree_view_selected)
    {
        tree -> gx_tree_view_selected -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
        _gx_system_dirty_mark(tree -> gx_tree_view_selected);
    }

    selected -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;

    tree -> gx_tree_view_selected = selected;

    if (tree -> gx_widget_id)
    {
        /* Generate tree select event. */
        _gx_widget_event_generate((GX_WIDGET *)tree, GX_EVENT_TREE_SELECT, selected -> gx_widget_id);
    }

    if (!(selected -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        _gx_tree_view_selected_visible(tree);
    }

    _gx_system_dirty_mark(selected);

    /* Return completion status code. */
    return(GX_SUCCESS);
}
