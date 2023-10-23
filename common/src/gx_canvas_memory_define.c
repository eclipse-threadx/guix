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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_memory_define                            PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns canvas memory pointer and size                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    memory                                Canvas memory address         */
/*    memsize                               Canvas memory size            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_dirty_mark                 Set the canvas dirty flag     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application software                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_memory_define(GX_CANVAS *canvas, GX_COLOR *memory, ULONG memsize)
{
#ifdef GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
GX_DISPLAY *display = canvas -> gx_canvas_display;
ULONG       canvas_size;

    if (!display)
    {
        return GX_INVALID_DISPLAY;
    }

    canvas_size = (ULONG)(display -> gx_display_driver_row_pitch_get((USHORT)canvas -> gx_canvas_x_resolution) * canvas -> gx_canvas_y_resolution);

    if (memsize < canvas_size)
    {
        canvas -> gx_canvas_status |= GX_CANVAS_PARTIAL_FRAME_BUFFER;
    }
#endif

    /* change the memory pointer value */
    canvas -> gx_canvas_memory = memory;

    /* change the memory size */
    canvas -> gx_canvas_memory_size = memsize;

    /* mark the canvas dirty so that it get refreshed */
    _gx_canvas_dirty_mark(canvas, GX_NULL);

    /* Return successful status.  */
    return(GX_SUCCESS);
}

