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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_canvas.h"
#include "gx_system.h"
#include "gx_utility.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_text_draw                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the screen text draw function call.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_start                               x coordinate, text left edge  */
/*    y_start                               y coordinate, text baseline   */
/*    string                                String to draw                */
/*    length                                Length of string to draw      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_text_draw                  The actual function           */
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
UINT  _gxe_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_CHAR *string, INT length)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if (string == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    if (_gx_system_current_draw_context == GX_NULL)
    {
        return GX_INVALID_CONTEXT;
    }

    /* Call actual widget height get function.  */
    status = _gx_canvas_text_draw(x_start, y_start, string, length);

    /* Return successful completion.  */
    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_text_draw_ext                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the screen text draw function call.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_start                               x coordinate, text left edge  */
/*    y_start                               y coordinate, text baseline   */
/*    string                                String to draw                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_text_draw_ext              The actual function           */
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
UINT _gxe_canvas_text_draw_ext(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_STRING *string)
{
UINT status;
UINT text_length = 0;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((string == GX_NULL) || (string -> gx_string_ptr == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    if (_gx_system_current_draw_context == GX_NULL)
    {
        return GX_INVALID_CONTEXT;
    }

    status = _gx_utility_string_length_check(string -> gx_string_ptr, &text_length, string -> gx_string_length);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    if (text_length < string -> gx_string_length)
    {
        return GX_INVALID_STRING_LENGTH;
    }

    /* Call actual widget height get function.  */
    status = _gx_canvas_text_draw_ext(x_start, y_start, string);

    /* Return successful completion.  */
    return(status);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_aligned_text_draw_ext                   PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the canvas aligned text draw         */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string                                String to draw                */
/*    rectangle                             Drawing area                  */
/*    alignment                             Alignment style               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_aligned_text_draw_ext      The actual function           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  04-25-2022     Ting Zhu                 Initial Version 6.1.11        */
/*                                                                        */
/**************************************************************************/
UINT _gxe_canvas_aligned_text_draw(GX_CONST GX_STRING *string, GX_RECTANGLE *rectangle, ULONG alignment)
{
UINT status;
UINT text_length = 0;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((string == GX_NULL) || (rectangle == GX_NULL) || (string -> gx_string_ptr == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (_gx_system_current_draw_context == GX_NULL)
    {
        return GX_INVALID_CONTEXT;
    }

    status = _gx_utility_string_length_check(string -> gx_string_ptr, &text_length, string -> gx_string_length);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    if (text_length < string -> gx_string_length)
    {
        return GX_INVALID_STRING_LENGTH;
    }

    /* Call actual widget height get function.  */
    status = _gx_canvas_aligned_text_draw(string, rectangle, alignment);

    /* Return successful completion.  */
    return(status);
}

