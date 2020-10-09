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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_window.h"
#include "gx_canvas.h"
#include "gx_animation.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_animation_canvas_define                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks error in animation canvas define function.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Animation control block       */
/*    canvas                                Pointer to animation canvas   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_canvas_define           The actual animation canvas   */
/*                                            define function             */
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
UINT  _gxe_animation_canvas_define(GX_ANIMATION *animation, GX_CANVAS *canvas)
{
UINT  status = GX_SUCCESS;
GX_DISPLAY *display;
ULONG required_size;

    if (animation == GX_NULL ||
        canvas == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    display = canvas -> gx_canvas_display;
    if (display == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    required_size = (ULONG)(display -> gx_display_driver_row_pitch_get((USHORT) canvas->gx_canvas_x_resolution));
    required_size = required_size * (ULONG) canvas->gx_canvas_y_resolution;

    if (canvas ->gx_canvas_memory_size < required_size)
    {
        return GX_INVALID_MEMORY_SIZE;
    }

    status = _gx_animation_canvas_define(animation, canvas);

    /* Return completion status code. */
    return(status);
}

