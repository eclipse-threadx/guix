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
/**   String Scroll Wheel Management (Scroll Wheel)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_string_scroll_wheel_text_get                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function returns pointer into string list.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    row                                   desired text row              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_get           Retrieve string by ID               */
/*    _gx_system_private_string_get   Retrieve string pointer in          */
/*                                      dynamically copied string buffer  */
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
UINT  _gx_string_scroll_wheel_text_get(GX_STRING_SCROLL_WHEEL *wheel, INT row, GX_STRING *string)
{
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
GX_CONST GX_CHAR **string_list_deprecated;
UINT               status = GX_SUCCESS;
#endif

    string -> gx_string_ptr = GX_NULL;
    string -> gx_string_length = 0;

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    if ((wheel -> gx_string_scroll_wheel_string_list_deprecated == GX_NULL &&
         wheel -> gx_string_scroll_wheel_string_list == GX_NULL &&
         wheel -> gx_string_scroll_wheel_string_id_list == GX_NULL) ||
        (row >= wheel -> gx_scroll_wheel_total_rows))
    {
        return GX_SUCCESS;
    }
#else
    if ((wheel -> gx_string_scroll_wheel_string_list == GX_NULL &&
         wheel -> gx_string_scroll_wheel_string_id_list == GX_NULL) ||
        row >= wheel -> gx_scroll_wheel_total_rows)
    {
        return GX_SUCCESS;
    }
#endif

    if (wheel -> gx_string_scroll_wheel_string_id_list)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)wheel, wheel -> gx_string_scroll_wheel_string_id_list[row], string);
    }
    else
    {
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
        if (wheel -> gx_string_scroll_wheel_string_list_deprecated)
        {
            _gx_system_private_string_list_get(wheel -> gx_string_scroll_wheel_string_list_deprecated, &string_list_deprecated, wheel -> gx_widget_style);
            string -> gx_string_ptr = string_list_deprecated[row];

            status = _gx_utility_string_length_check(string -> gx_string_ptr, &string -> gx_string_length, GX_MAX_STRING_LENGTH);

            if (status != GX_SUCCESS)
            {
                string -> gx_string_ptr = GX_NULL;
                string -> gx_string_length = 0;
            }
            return status;
        }
#endif

        *string = wheel -> gx_string_scroll_wheel_string_list[row];
    }

    return GX_SUCCESS;
}

