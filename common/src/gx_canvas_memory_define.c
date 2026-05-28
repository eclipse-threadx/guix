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

