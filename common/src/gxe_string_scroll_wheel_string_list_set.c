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
#include "gx_utility.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_string_scroll_wheel_string_list_set            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in string scroll wheel string list  */
/*    set call.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    string_list                           String list to be set         */
/*    string_count                          The number of assigned strings*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_string_scroll_wheel_string_list_set                             */
/*                                          Actual string scroll wheel    */
/*                                            string list set call        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gxe_string_scroll_wheel_string_list_set(GX_STRING_SCROLL_WHEEL *wheel,
                                               GX_CONST GX_CHAR **string_list,
                                               INT string_count)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer. */
    if (wheel == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    /* Check for invalid widget. */
    if (wheel -> gx_widget_type == 0)
    {
        return GX_INVALID_WIDGET;
    }

    /* Check for invalid list size. */
    if (string_count < 0)
    {
        return GX_INVALID_VALUE;
    }

    status = _gx_string_scroll_wheel_string_list_set(wheel, string_list, string_count);

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_string_scroll_wheel_string_list_set_ext        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in string scroll wheel string list  */
/*    set call.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    string_list                           String list to be set         */
/*    string_count                          The number of assigned strings*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_string_scroll_wheel_string_list_set_ext                         */
/*                                          Actual string scroll wheel    */
/*                                            string list set ext call    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gxe_string_scroll_wheel_string_list_set_ext(GX_STRING_SCROLL_WHEEL *wheel,
                                                  GX_CONST GX_STRING *string_list,
                                                  INT string_count)
{
UINT                status;
INT                 string_id;
GX_CONST GX_STRING *string;
UINT                string_length;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer. */
    if (wheel == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    /* Check for invalid widget. */
    if (wheel -> gx_widget_type == 0)
    {
        return GX_INVALID_WIDGET;
    }

    /* Check for invalid list size. */
    if (string_count < 0)
    {
        return GX_INVALID_VALUE;
    }

    if (string_list)
    {
        /* Test string length. */
        for (string_id = 0; string_id < string_count; string_id++)
        {
            string = &string_list[string_id];
            if (string -> gx_string_ptr)
            {
                status = _gx_utility_string_length_check(string -> gx_string_ptr, &string_length, string -> gx_string_length);

                if (status != GX_SUCCESS)
                {
                    return status;
                }
            }
            else
            {
                string_length = 0;
            }

            if (string_length != string -> gx_string_length)
            {
                return GX_INVALID_STRING_LENGTH;
            }
        }
    }

    status = _gx_string_scroll_wheel_string_list_set_ext(wheel, string_list, string_count);

    return status;
}

