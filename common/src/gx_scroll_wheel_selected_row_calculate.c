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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_selected_row_calculate             PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates the current selected row for scroll wheel  */
/*    widget.                                                             */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_scroll                                        */
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
UINT _gx_scroll_wheel_selected_row_calculate(GX_SCROLL_WHEEL *wheel)
{
INT      yshift;
INT      selected_row;
GX_VALUE half_row_height = (wheel -> gx_scroll_wheel_row_height >> 1);

    yshift = wheel -> gx_scroll_wheel_selected_yshift;
    selected_row = wheel -> gx_scroll_wheel_selected_row;

    if (wheel -> gx_scroll_wheel_wrap_style_check(wheel))
    {
        while (yshift > half_row_height)
        {
            /* Scroll Down. */
            yshift -= wheel -> gx_scroll_wheel_row_height;
            selected_row--;
        }

        while (yshift < -half_row_height)
        {
            /* Scroll Up. */
            yshift += wheel -> gx_scroll_wheel_row_height;
            selected_row++;
        }

        while (selected_row > wheel -> gx_scroll_wheel_total_rows - 1)
        {
            selected_row -= wheel -> gx_scroll_wheel_total_rows;
        }

        while (selected_row < 0)
        {
            selected_row += wheel -> gx_scroll_wheel_total_rows;
        }
    }
    else
    {
        while ((yshift > half_row_height) &&
               (selected_row > 0))
        {
            /* Scroll Down. */
            yshift -= wheel -> gx_scroll_wheel_row_height;
            selected_row--;
        }

        while ((yshift < -half_row_height) &&
               (selected_row < wheel -> gx_scroll_wheel_total_rows - 1))
        {
            /* Scroll Up. */
            yshift += wheel -> gx_scroll_wheel_row_height;
            selected_row++;
        }
    }

    wheel -> gx_scroll_wheel_selected_row = selected_row;
    wheel -> gx_scroll_wheel_selected_yshift = (GX_VALUE)yshift;

    return GX_SUCCESS;
}

