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
/**   Scroll Wheel Management (Generic Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_total_rows_set             PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns total number of the generic scroll wheel.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll Wheel control block    */
/*    count                                 Number of rows                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_hide                       Hide a widget                 */
/*    _gx_widget_first_visible_client_child_get                           */
/*                                          Get the first visible client  */
/*    _gx_widget_next_visible_client_child_get                            */
/*                                          Get the next visible client   */
/*    _gx_generic_scroll_wheel_children_position                          */
/*                                          Position the children of the  */
/*                                            generic scroll wheel        */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
UINT _gx_generic_scroll_wheel_total_rows_set(GX_GENERIC_SCROLL_WHEEL *wheel, INT count)
{
INT        row;
GX_WIDGET *test;

    /* Update total count of rows. */
    wheel -> gx_scroll_wheel_total_rows = count;

    /* Update selected row. */
    if (count == 0)
    {
        wheel -> gx_scroll_wheel_selected_row = 0;
    }
    else if (wheel -> gx_scroll_wheel_selected_row >= count)
    {
        wheel -> gx_scroll_wheel_selected_row = count - 1;
    }

    /* Rreset child count and make all children visible. */
    wheel -> gx_generic_scroll_wheel_child_count = 0;

    test = _gx_widget_first_client_child_get((GX_WIDGET *)wheel);

    while (test)
    {
        if (!(test->gx_widget_status & GX_STATUS_VISIBLE))
        {
            _gx_widget_show(test);
        }

        wheel->gx_generic_scroll_wheel_child_count++;

        test = _gx_widget_next_client_child_get(test);
    }

    test = wheel -> gx_widget_last_child;

    /* Check whether list child count is larger than count. */
    while (test && (wheel -> gx_generic_scroll_wheel_child_count > count))
    {
        if (!(test -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            _gx_widget_hide(test);
            wheel -> gx_generic_scroll_wheel_child_count--;
        }

        test = test -> gx_widget_previous;
    }

    wheel -> gx_generic_scroll_wheel_top_index = 0;

    if (wheel -> gx_generic_scroll_wheel_callback)
    {

        /* Re-populate list child. */
        row = 0;

        test = _gx_widget_first_visible_client_child_get((GX_WIDGET *)wheel);

        while (test)
        {
            /* Reset child id. */
            test -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
            wheel -> gx_generic_scroll_wheel_callback(wheel, test, row++);

            test = _gx_widget_next_visible_client_child_get(test);
        }
    }

    _gx_generic_scroll_wheel_children_position(wheel);

    /* Refresh screen. */
    if (wheel -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)wheel);
    }
    return GX_SUCCESS;
}

