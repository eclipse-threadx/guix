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
/*    _gx_vertical_list_invisible_page_scroll             PORTABLE C      */
/*                                                           6.1.8        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves up or down the vertical list by pages.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    list                                  Vertical list widget control  */
/*                                            block                       */
/*    num_pages                             The number of pages to scroll */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_client_child_get     Get first client child        */
/*    _gx_widget_next_client_child_get      Get next client child         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_vertical_list_scroll                                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  08-02-2021     Ting Zhu                 Initial Version 6.1.8         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_vertical_list_invisible_page_scroll(GX_VERTICAL_LIST *list, INT num_pages)
{
INT maximum_scroll_pages;

    if (!(list -> gx_widget_style & GX_STYLE_WRAP))
    {

        /* Wrap style is not applied, need to calculate scroll limit.  */
        if (num_pages < 0)
        {

            /* Calculate maximum up scroll num_pages.  */
            maximum_scroll_pages = (list -> gx_vertical_list_total_rows - list -> gx_vertical_list_top_index - list -> gx_vertical_list_child_count);
            maximum_scroll_pages /= list -> gx_vertical_list_child_count;

            /* Remain one page for normal scroll routine. */
            if (maximum_scroll_pages >= 1)
            {
                maximum_scroll_pages--;
            }

            if (maximum_scroll_pages < (-num_pages))
            {
                num_pages = (-maximum_scroll_pages);
            }
        }
        else
        {

            /* Calculate maximum down scroll num_pages.   */
            maximum_scroll_pages = list -> gx_vertical_list_top_index / list -> gx_vertical_list_child_count;

            /* Remain one page for normal scroll routine. */
            if (maximum_scroll_pages >= 1)
            {
                maximum_scroll_pages--;
            }

            if (maximum_scroll_pages < num_pages)
            {
                num_pages = maximum_scroll_pages;
            }
        }
    }

    if (num_pages)
    {

        /* Update top index according to scroll num_pages.  */
        list -> gx_vertical_list_top_index -= num_pages * list -> gx_vertical_list_child_count;

        if (list -> gx_widget_style & GX_STYLE_WRAP)
        {

            /* Wrap page index.  */
            if (num_pages < 0)
            {
                while (list -> gx_vertical_list_top_index >= list -> gx_vertical_list_total_rows)
                {
                    list -> gx_vertical_list_top_index -= list -> gx_vertical_list_total_rows;
                }
            }
            else
            {
                while (list -> gx_vertical_list_top_index < 0)
                {
                    list -> gx_vertical_list_top_index += list -> gx_vertical_list_total_rows;
                }
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vertical_list_scroll                            PORTABLE C      */
/*                                                           6.1.8        */
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
/*  08-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            fixed scroll overflow issue,*/
/*                                            resulting in version 6.1.8  */
/*                                                                        */
/**************************************************************************/
VOID _gx_vertical_list_scroll(GX_VERTICAL_LIST *list, INT amount)
{
GX_WIDGET    *child;
GX_WIDGET    *last_child;
GX_RECTANGLE  block;
GX_SCROLLBAR *scroll;
GX_BOOL       do_block_move = GX_TRUE;
GX_BOOL       reuse_child_widgets = GX_FALSE;
INT           page_size;
INT           num_pages;

    if (!amount)
    {
        return;
    }

    if ((list -> gx_vertical_list_callback != GX_NULL) &&
        (list -> gx_vertical_list_visible_rows < list -> gx_vertical_list_total_rows) &&
        ((list -> gx_vertical_list_child_count < list -> gx_vertical_list_total_rows) || (list -> gx_widget_style & GX_STYLE_WRAP)))
    {
        reuse_child_widgets = GX_TRUE;

        /* Calculate the page size.  */
        page_size = (list -> gx_vertical_list_child_height * list -> gx_vertical_list_child_count);

        if (!page_size)
        {
            return;
        }

        /* Calculate the number of pages to be scrolled.  */
        if (amount < 0)
        {
            num_pages = (amount + page_size) / page_size;
        }
        else
        {
            num_pages = (amount - page_size) / page_size;
        }

        if (num_pages)
        {

            /* Calculate the remainning scroll amount.  */
            amount -= (num_pages * page_size);

            /* Scroll pages.  */
            _gx_vertical_list_invisible_page_scroll(list, num_pages);
        }
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

    if (reuse_child_widgets)
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

