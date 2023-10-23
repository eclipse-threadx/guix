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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_create                                  PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the canvas function call.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    name                                  Name of canvas                */
/*    display                               Display control block         */
/*    type                                  Type of canvas                */
/*    width                                 Width of canvas               */
/*    height                                Height of canvas              */
/*    memory_area                           Memory area of canvas with    */
/*                                            each pixel of GX_COLOR      */
/*    memory_size                           Size of canvas memory area    */
/*    canvas_control_block_size             Size of the canvas control    */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_create                     Actual canvas create call     */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s), verify   */
/*                                            the memory size only when   */
/*                                            memory_area is provided,    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gxe_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name, GX_DISPLAY *display, UINT type,
                         UINT width, UINT height, GX_COLOR *memory_area, ULONG memory_size,
                         UINT canvas_control_block_size)
{
UINT status;
UINT pitch;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((canvas == GX_NULL) || (display == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    if (canvas_control_block_size != sizeof(GX_CANVAS))
    {
        return(GX_INVALID_CANVAS_SIZE);
    }

    /* Check for widget already created.  */
    if (canvas -> gx_canvas_display != GX_NULL)
    {
        return(GX_ALREADY_CREATED);
    }

    if (memory_area)
    {
        pitch = (UINT)(display -> gx_display_driver_row_pitch_get((USHORT)width));
        if (memory_size < pitch * height)
        {
            return GX_INVALID_SIZE;
        }
    }

    if (type & (~((UINT)(GX_CANVAS_SIMPLE | GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE |
                         GX_CANVAS_MODIFIED | GX_CANVAS_COMPOSITE))))
    {
        return(GX_INVALID_TYPE);
    }

    /* Call the actual canvas create function.  */
    status = _gx_canvas_create(canvas, name, display, type, width, height, memory_area, memory_size);

    /* Return completion status.  */
    return status;
}

