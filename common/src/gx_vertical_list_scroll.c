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
/**   Vertical List (List)                                                */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_scrollbar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vertical_list_scroll                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves up or down the scrollbar.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list widget control  */
/*                                            block                       */
/*    amount                                Shifting value                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_scroll_shift               Shift a widget                */
/*    _gx_vertical_list_up_wrap             Scroll up the vertical list   */
/*    _gx_vertical_list_down_wrap           Scroll down the vertical list */
/*    _gx_widget_block_move                 Widget block move             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_vertical_list_event_process       Vertical list event process   */
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
VOID _gx_vertical_list_scroll(GX_VERTICAL_LIST *list, INT amount)
{
GX_WIDGET    *child;
GX_WIDGET    *last_child;
GX_RECTANGLE  block;
GX_SCROLLBAR *scroll;
GX_BOOL       do_block_move = GX_TRUE;

    if (!amount)
    {
        return;
    }

    /* first shift my child widgets */
    child = list -> gx_widget_first_child;

    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            _gx_widget_scroll_shift(child, 0, amount, GX_TRUE);
        }
        child = child -> gx_widget_next;
    }

    /* next check to see if we need to wrap any child widgets */

    if ((list -> gx_vertical_list_callback != GX_NULL) &&
        (list -> gx_vertical_list_visible_rows < list -> gx_vertical_list_total_rows) &&
        ((list -> gx_vertical_list_child_count < list -> gx_vertical_list_total_rows) || (list -> gx_widget_style & GX_STYLE_WRAP)))
    {
        /* this means we have fewer children than list rows, so we
           need to move and re-use the child widgets
         */
        if (amount < 0)
        {
            _gx_vertical_list_up_wrap(list);
        }
        else
        {
            _gx_vertical_list_down_wrap(list);
        }
    }

    _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_VERTICAL_SCROLL, &scroll);

    if (scroll)
    {
        _gx_scrollbar_reset(scroll, GX_NULL);
    }

    if (list -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        block = list -> gx_window_client;

        if (!(list -> gx_widget_style & (GX_STYLE_WRAP | GX_STYLE_TRANSPARENT)) &&
            (list -> gx_widget_style & GX_STYLE_BORDER_THIN))
        {
            child = _gx_widget_first_client_child_get((GX_WIDGET *)list);
            last_child = _gx_widget_last_client_child_get((GX_WIDGET *)list);

            if (child)
            {
                if ((child -> gx_widget_size.gx_rectangle_top > block.gx_rectangle_top ||
                     last_child -> gx_widget_size.gx_rectangle_bottom < block.gx_rectangle_bottom))
                {
                    /* If the widget has thin border, the round corder of the border
                       will cover the client area, block move will cause trouble. */
                    _gx_system_dirty_mark((GX_WIDGET *)list);
                    do_block_move = GX_FALSE;
                }
            }
        }

        if (do_block_move)
        {
            _gx_widget_block_move((GX_WIDGET *)list, &block, 0, amount);
        }
    }
}

