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
#include "gx_scroll_wheel.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_scroll                     PORTABLE C      */
/*                                                           6.2.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves up or down the generic scroll wheel.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Generic scroll wheel control  */
/*                                            block                       */
/*    amount                                Shifting value                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_scroll_wheel_selected_row_calculate                             */
/*                                          Calculate the new selected row*/
/*    _gx_widget_first_visible_client_child_get                           */
/*                                          Get the first visible client  */
/*    _gx_widget_next_visible_client_child_get                            */
/*                                          Get the next visible client   */
/*    _gx_widget_scroll_shift               Shift a widget                */
/*    _gx_generic_scroll_wheel_up_wrap      Scroll up the scroll wheel    */
/*    _gx_generic_scroll_wheel_down_wrap    Scroll down the scroll wheel  */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021        Ting Zhu              Initial Version 6.1.7         */
/*  10-31-2022        Kenneth Maxwell       Modified comment(s),          */
/*                                            changed return type,        */
/*                                            resulting in version 6.2.0  */
/*  03-08-2023        Ting Zhu              Modified comment(s),          */
/*                                            changed return type,        */
/*                                            resulting in version 6.2.1  */
/*                                                                        */
/**************************************************************************/
UINT _gx_generic_scroll_wheel_scroll(GX_GENERIC_SCROLL_WHEEL *wheel, GX_VALUE shift)
{
GX_WIDGET *child;
INT        y_shift;
INT        min_shift;
INT        max_shift;

    if (!shift)
    {
        return GX_SUCCESS;
    }

    if (!wheel -> gx_scroll_wheel_wrap_style_check((GX_SCROLL_WHEEL *)wheel))
    {
        if ((shift > 0 && wheel -> gx_scroll_wheel_selected_row == 0) ||
            (shift < 0 && wheel -> gx_scroll_wheel_selected_row == wheel -> gx_scroll_wheel_total_rows - 1))
        {
            y_shift = wheel -> gx_scroll_wheel_selected_yshift + shift;

            min_shift = (wheel -> gx_scroll_wheel_selected_row - wheel -> gx_scroll_wheel_total_rows + 1) * wheel -> gx_scroll_wheel_row_height;
            max_shift = (wheel -> gx_scroll_wheel_selected_row * wheel -> gx_scroll_wheel_row_height);

            if ((y_shift < min_shift) || (y_shift > max_shift))
            {

                /* Slow down the speed when scroll outside the valid row range.  */
                shift = (GX_VALUE)(shift + wheel -> gx_scroll_wheel_shift_error);
                wheel -> gx_scroll_wheel_shift_error = (GX_BYTE)(shift % 4);
                shift /= 4;
            }
            else
            {
                wheel -> gx_scroll_wheel_shift_error = 0;
            }
        }
    }

    wheel -> gx_scroll_wheel_selected_yshift = (GX_VALUE)(wheel -> gx_scroll_wheel_selected_yshift + shift);

    /* Calculate the new selected row.  */
    _gx_scroll_wheel_selected_row_calculate((GX_SCROLL_WHEEL *)wheel);


    /* First shift my child widgets.  */
    child = _gx_widget_first_visible_client_child_get((GX_WIDGET *)wheel);

    while (child)
    {
        _gx_widget_scroll_shift(child, 0, shift, GX_TRUE);

        if (child -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            child -> gx_widget_style &= (~GX_STYLE_DRAW_SELECTED);
        }

        child = _gx_widget_next_visible_client_child_get(child);
    }

    /* Next check to see if we need to wrap any child widgets.  */

    if ((wheel -> gx_generic_scroll_wheel_callback != GX_NULL) &&
        (wheel -> gx_generic_scroll_wheel_visible_rows < wheel -> gx_scroll_wheel_total_rows) &&
        ((wheel -> gx_generic_scroll_wheel_child_count < wheel -> gx_scroll_wheel_total_rows) || (wheel -> gx_widget_style & GX_STYLE_WRAP)))
    {

        /* This means we have fewer children than list rows, so we
           need to move and re-use the child widgets.  */
        if (shift < 0)
        {
            _gx_generic_scroll_wheel_up_wrap(wheel);
        }
        else
        {
            _gx_generic_scroll_wheel_down_wrap(wheel);
        }
    }

    if (wheel -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)wheel);
    }
    return GX_SUCCESS;
}

