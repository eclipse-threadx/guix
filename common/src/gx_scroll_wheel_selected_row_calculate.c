/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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

