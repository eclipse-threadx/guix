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
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_children_position          PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions the children for the generic scroll wheel.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Shift rectangle               */
/*    _gx_window_client_height_get          Retrieve the client height    */
/*                                            of the widget               */
/*    _gx_widget_first_visible_client_child_get                           */
/*                                          Get the first visible client  */
/*    _gx_widget_next_visible_client_child_get                            */
/*                                          Get the next visible client   */
/*    _gx_widget_resize                     resizes the widget            */
/*    _gx_generic_scroll_wheel_scroll       Scroll the generic scroll wheel*/
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
UINT _gx_generic_scroll_wheel_children_position(GX_GENERIC_SCROLL_WHEEL *wheel)
{
GX_RECTANGLE  childsize;
GX_WIDGET    *child;
GX_VALUE      height;
GX_VALUE      row_height;
GX_VALUE      selected_top;
GX_RECTANGLE *client;
INT           top_rows;
INT           row;
INT           selected_row;

    /* Pick up first visible child.  */
    child = _gx_widget_first_visible_client_child_get((GX_WIDGET*)wheel);

    if (!child)
    {
        return GX_SUCCESS;
    }

    /* Get row height. */
    row_height = wheel -> gx_scroll_wheel_row_height;

    if (!row_height)
    {
        return GX_SUCCESS;
    }

    /* Get client rectangle.  */
    client = &wheel -> gx_window_client;

    /* Calculate rectangle top of the selected row.  */
    selected_top = (GX_VALUE)((client -> gx_rectangle_top + client -> gx_rectangle_bottom) >> 1);
    selected_top = (GX_VALUE)(selected_top - (row_height >> 1));
    selected_top = (GX_VALUE)(selected_top + wheel -> gx_scroll_wheel_selected_yshift);

    /* Calculate first child size.  */
    childsize = *client;

    top_rows = (selected_top -client -> gx_rectangle_top + row_height) / row_height;

    if ((wheel -> gx_scroll_wheel_total_rows) && top_rows >= wheel -> gx_scroll_wheel_total_rows)
    {
        top_rows = wheel -> gx_scroll_wheel_total_rows - 1;
    }

    childsize.gx_rectangle_top = (GX_VALUE)(selected_top - (top_rows * row_height));
    childsize.gx_rectangle_bottom = (GX_VALUE)(childsize.gx_rectangle_top + row_height - 1);

    /* Initiate child count as 0.  */
    wheel -> gx_generic_scroll_wheel_child_count = 0;

    /* Save selected row.  */
    selected_row = wheel -> gx_scroll_wheel_selected_row;

    row = wheel -> gx_generic_scroll_wheel_top_index;
    wheel -> gx_scroll_wheel_selected_row = row + top_rows;

    /* Resize child widgets.  */
    while (child)
    {

        /* Increment child count. */
        wheel -> gx_generic_scroll_wheel_child_count++;

        if (childsize.gx_rectangle_top == selected_top)
        {
            child -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
        }
        else
        {
            child -> gx_widget_style &= (~GX_STYLE_DRAW_SELECTED);
        }

        child -> gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;

        /* Resize widget. */
        _gx_widget_resize(child, &childsize);

        _gx_utility_rectangle_shift(&childsize, 0, row_height);

        child = _gx_widget_next_visible_client_child_get(child);
    }

    /* Get client height.  */
    _gx_window_client_height_get((GX_WINDOW*)wheel, &height);

    /* Calculate number of visible rows.  */
    wheel -> gx_generic_scroll_wheel_visible_rows = (GX_VALUE)((height + row_height - 1) / row_height);

    if (selected_row != wheel -> gx_scroll_wheel_selected_row)
    {

        /* If current selected row is not match the configured row,
           scroll the widget to the configured row selected.  */
        row = wheel -> gx_scroll_wheel_selected_row - selected_row;

        _gx_generic_scroll_wheel_scroll(wheel, (GX_VALUE)(row_height * row));
    }

    return GX_SUCCESS;
}

