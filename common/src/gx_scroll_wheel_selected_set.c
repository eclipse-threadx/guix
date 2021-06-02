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
#include "gx_scroll_wheel.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_selected_set                       PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets the current selected row index for scroll wheel  */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    row                                   Selected row to be set        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT _gx_scroll_wheel_selected_set(GX_SCROLL_WHEEL *wheel, INT row)
{
INT dist;

    if ((row < 0) || (wheel -> gx_scroll_wheel_total_rows == 0))
    {
        row = 0;
    }
    else if (row > wheel -> gx_scroll_wheel_total_rows - 1)
    {
        row = wheel -> gx_scroll_wheel_total_rows - 1;
    }

    if (wheel -> gx_scroll_wheel_selected_row == row)
    {
        return GX_SUCCESS;
    }

    if (wheel -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        if (wheel -> gx_scroll_wheel_wrap_style_check(wheel))
        {
            if (GX_ABS(wheel -> gx_scroll_wheel_selected_row - row) <
                GX_ABS(wheel -> gx_scroll_wheel_selected_row + wheel -> gx_scroll_wheel_total_rows - row))
            {
                dist = wheel -> gx_scroll_wheel_selected_row - row;
            }
            else
            {
                dist = wheel -> gx_scroll_wheel_selected_row + wheel -> gx_scroll_wheel_total_rows - row;
            }
        }
        else
        {
            dist = wheel -> gx_scroll_wheel_selected_row - row;
        }

        wheel -> gx_scroll_wheel_animation_steps = 10;
        wheel -> gx_scroll_wheel_animation_speed = (GX_VALUE)(dist * wheel -> gx_scroll_wheel_row_height) / 10;
        wheel -> gx_scroll_wheel_shift_error = (GX_BYTE)(dist * wheel -> gx_scroll_wheel_row_height) % 10;
        _gx_system_timer_start((GX_WIDGET *)wheel, GX_ANIMATION_TIMER, 2, 2);
    }
    else
    {
        wheel -> gx_scroll_wheel_selected_row = row;
    }

    return GX_SUCCESS;
}

