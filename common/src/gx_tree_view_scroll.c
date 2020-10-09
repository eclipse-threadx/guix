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
#include "gx_widget.h"
#include "gx_tree_view.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_scroll                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service scrolls the tree client area by the specified amount.  */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to tree view          */
/*    x_scroll                              Amount to scroll on x-axis    */
/*    y_scroll                              Amount to scroll on y-axis    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_scroll_shift               Change position of a widget   */
/*    _gx_widget_block_move                 Move a block of widget area   */
/*    _gx_window_scrollbar_find             Find scrollbar for a widget   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_system_dirty_partial_add          Mark partial area of a widget */
/*                                            as dirty                    */
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
UINT _gx_tree_view_scroll(GX_TREE_VIEW *tree, GX_VALUE x_scroll, GX_VALUE y_scroll)
{
GX_RECTANGLE  block;
GX_WIDGET    *child;
GX_SCROLLBAR *scroll;

    block = tree -> gx_window_client;

    if (!(tree -> gx_widget_style & GX_STYLE_TRANSPARENT) &&
        (tree -> gx_widget_style & GX_STYLE_BORDER_THIN))
    {
        /* Widget with think border have a round corner, which have some effects to block move. */
        block.gx_rectangle_right = (GX_VALUE)(block.gx_rectangle_left + 1);
        _gx_system_dirty_partial_add((GX_WIDGET *)tree, &block);

        block = tree -> gx_window_client;
        block.gx_rectangle_bottom = (GX_VALUE)(block.gx_rectangle_top + 1);
        _gx_system_dirty_partial_add((GX_WIDGET *)tree, &block);

        block = tree -> gx_window_client;
        block.gx_rectangle_left = (GX_VALUE)(block.gx_rectangle_left + 2);
        block.gx_rectangle_top = (GX_VALUE)(block.gx_rectangle_top + 2);
    }

    /* Calculate shift values. */
    tree -> gx_tree_view_x_shift = (GX_VALUE)(tree -> gx_tree_view_x_shift + x_scroll);
    tree -> gx_tree_view_y_shift = (GX_VALUE)(tree -> gx_tree_view_y_shift + y_scroll);

    child = tree -> gx_widget_first_child;
    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            _gx_widget_scroll_shift(child, x_scroll, y_scroll, GX_TRUE);
        }
        child = child -> gx_widget_next;
    }

    _gx_widget_block_move((GX_WIDGET *)tree, &block, x_scroll, y_scroll);

    /* if we have a scrollbar, we need to tell it to re-calculate the thumb position */

    scroll = GX_NULL;

    if (x_scroll)
    {
        _gx_window_scrollbar_find((GX_WINDOW *)tree, (USHORT)GX_TYPE_HORIZONTAL_SCROLL, &scroll);
    }
    else
    {
        _gx_window_scrollbar_find((GX_WINDOW *)tree, GX_TYPE_VERTICAL_SCROLL, &scroll);
    }

    if (scroll)
    {
        _gx_scrollbar_reset(scroll, GX_NULL);
    }
    return(GX_SUCCESS);
}

