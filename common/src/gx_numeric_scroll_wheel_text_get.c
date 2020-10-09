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
/**   Numeric Scroll Wheel Management (Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_numeric_scroll_wheel_text_get                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function returns pointer to numeric text.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    row                                   Desired text row              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_ltoa                      Convert integer to string     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_draw                                          */
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
UINT _gx_numeric_scroll_wheel_text_get(GX_NUMERIC_SCROLL_WHEEL *wheel, INT row, GX_STRING *string)
{
INT  step;
INT  val;

    if (row < wheel -> gx_scroll_wheel_total_rows)
    {
        step = wheel -> gx_numeric_scroll_wheel_end_val - wheel -> gx_numeric_scroll_wheel_start_val;

        if (step == (wheel -> gx_scroll_wheel_total_rows - 1))
        {
            step = 1;
        }
        else if (step == (1 - wheel -> gx_scroll_wheel_total_rows))
        {
            step = -1;
        }
        else if (wheel -> gx_scroll_wheel_total_rows > 1)
        {
            step /= (wheel -> gx_scroll_wheel_total_rows - 1);
        }

        val = wheel -> gx_numeric_scroll_wheel_start_val + (step * row);
        _gx_utility_ltoa(val, wheel -> gx_numeric_scroll_wheel_string_buffer, GX_NUMERIC_SCROLL_WHEEL_STRING_BUFFER_SIZE);

        string -> gx_string_ptr = wheel -> gx_numeric_scroll_wheel_string_buffer;
        _gx_utility_string_length_check(string -> gx_string_ptr, &string -> gx_string_length, GX_NUMERIC_SCROLL_WHEEL_STRING_BUFFER_SIZE - 1);
    }
    else
    {
        string -> gx_string_ptr = GX_NULL;
        string -> gx_string_length = 0;
    }

    return(GX_SUCCESS);
}

