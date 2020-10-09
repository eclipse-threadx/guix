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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_system_string_width_get                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in system string width get call.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    font                                  Font used by the string       */
/*    string                                Pointer to string             */
/*    string_length                         Length of string              */
/*    return_width                          Destination for width of      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_string_width_get           Actual system string width    */
/*                                            get call                    */
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
UINT  _gxe_system_string_width_get(GX_CONST GX_FONT *font, GX_CONST GX_CHAR *string, INT string_length, GX_VALUE *return_width)
{
UINT status;
UINT length;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if ((font == GX_NULL) || (string == GX_NULL) || (return_width == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid font.  */
    if (font -> gx_font_glyphs.gx_font_normal_glyphs == GX_NULL)
    {
        return(GX_INVALID_FONT);
    }

    if(string_length > 0)
    {
        /* Calculate input string length. */
        status = _gx_utility_string_length_check(string, &length, GX_MAX_STRING_LENGTH);

        if (status != GX_SUCCESS)
        {
            return status;
        }

        /* Check for invalid string length.  */
        if (string_length > (INT)length)
        {
            return(GX_INVALID_STRING_LENGTH);
        }
    }

    /* Call actual system string width get.  */
    status = _gx_system_string_width_get(font, string, string_length, return_width);

    /* Return status.  */
    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_string_width_get_ext                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in system string width get call.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    font                                  Font used by the string       */
/*    string                                Pointer to string             */
/*    return_width                          Destination for width of      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_string_width_get_ext       Actual system string width    */
/*                                            get exe call                */
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
UINT  _gxe_system_string_width_get_ext(GX_CONST GX_FONT *font, GX_CONST GX_STRING *string, GX_VALUE *return_width)
{
UINT status;
UINT length;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if ((font == GX_NULL) || (string == GX_NULL) || (return_width == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid font.  */
    if (font -> gx_font_glyphs.gx_font_normal_glyphs == GX_NULL)
    {
        return(GX_INVALID_FONT);
    }

    /* Calculate input string length. */
    status = _gx_utility_string_length_check(string -> gx_string_ptr, &length, string -> gx_string_length);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    /* Check for invalid string length.  */
    if (string -> gx_string_length > length)
    {
        return(GX_INVALID_STRING_LENGTH);
    }

    /* Call actual system string width get.  */
    status = _gx_system_string_width_get_ext(font, string, return_width);

    /* Return status.  */
    return(status);
}

