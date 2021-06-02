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
/**   Scroll Wheel Management (Scroll Wheel)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_scroll                             PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function scrolls a scroll widget widget.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    shift                                 Value to be shift             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_scroll_wheel_selected_row_calculate                             */
/*                                          Calculate current selected row*/
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_scroll_wheel_event_process                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            modified wrap style test    */
/*                                            logic,                      */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT _gx_scroll_wheel_scroll(GX_SCROLL_WHEEL *wheel, GX_VALUE shift)
{
INT y_shift;
INT min_shift;
INT max_shift;

    if (!wheel -> gx_scroll_wheel_wrap_style_check(wheel))
    {
        y_shift = wheel -> gx_scroll_wheel_selected_yshift + shift;

        if ((shift > 0 && wheel -> gx_scroll_wheel_selected_row == 0) ||
            (shift < 0 && wheel -> gx_scroll_wheel_selected_row == wheel -> gx_scroll_wheel_total_rows - 1))
        {
            y_shift = wheel -> gx_scroll_wheel_selected_yshift + shift;

            min_shift = (wheel -> gx_scroll_wheel_selected_row - wheel -> gx_scroll_wheel_total_rows + 1) *
                wheel -> gx_scroll_wheel_row_height;
            max_shift = (wheel -> gx_scroll_wheel_selected_row * wheel -> gx_scroll_wheel_row_height);

            if ((y_shift < min_shift) || (y_shift > max_shift))
            {
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

    _gx_scroll_wheel_selected_row_calculate(wheel);

    if (wheel -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)wheel);
    }
    return GX_SUCCESS;
}

