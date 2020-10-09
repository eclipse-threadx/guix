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
/*    _gxe_canvas_rotated_text_draw                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks error in canvas rotated text draw function.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  pointer to string             */
/*    xcenter                               center point for text drawing */
/*    ycenter                               center point for text drawing */
/*    angle                                 angle at which to rotate text */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_rotated_text_draw          The actual canvas rotated     */
/*                                            text draw function          */
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
UINT _gxe_canvas_rotated_text_draw(GX_CONST GX_CHAR *text,
                                   GX_VALUE xcenter,
                                   GX_VALUE ycenter,
                                   INT angle)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if (text == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    if (!_gx_system_memory_allocator || !_gx_system_memory_free)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    status = _gx_canvas_rotated_text_draw(text, xcenter, ycenter, angle);

    /* Return completion status code. */
    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_rotated_text_draw_ext                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks error in canvas rotated text draw function.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  pointer to string             */
/*    xcenter                               center point for text drawing */
/*    ycenter                               center point for text drawing */
/*    angle                                 angle at which to rotate text */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_rotated_text_draw_ext      The actual canvas rotated     */
/*                                            text draw function          */
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
UINT _gxe_canvas_rotated_text_draw_ext(GX_CONST GX_STRING *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle)
{
UINT status;
UINT text_length = 0;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if ((text == GX_NULL) || (text -> gx_string_ptr == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (_gx_system_current_draw_context == GX_NULL)
    {
        return GX_INVALID_CONTEXT;
    }

    if (!_gx_system_memory_allocator || !_gx_system_memory_free)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    status = _gx_utility_string_length_check(text -> gx_string_ptr, &text_length, text -> gx_string_length);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    if (text_length != text -> gx_string_length)
    {
        return GX_INVALID_STRING_LENGTH;
    }

    status = _gx_canvas_rotated_text_draw_ext(text, xcenter, ycenter, angle);

    /* Return completion status code. */
    return(status);
}

