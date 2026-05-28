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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_canvas.h"
#include "gx_context.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_polygon_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic polygon draw function.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    vertex                                Array of vertexes             */
/*    num                                   Number of vertexes            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_line_draw                  Draw a line into canvas       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_polygon_draw(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num)
{
INT index;
INT brush_width = context -> gx_draw_context_brush.gx_brush_width;
#if defined (GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE     temp_alpha;
UINT         brush_style;
    temp_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    brush_style = context -> gx_draw_context_brush.gx_brush_style;
    if (brush_width > 1)
    {
        if (temp_alpha != 0xff)
        {
            if ((brush_style & GX_BRUSH_ROUND) || (brush_style & GX_BRUSH_ALIAS))
            {
                context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
            }
        }
    }
#endif

    if (brush_width > 0)
    {
        /*round end*/
        for (index = 0; index < num; index++)
        {
            _gx_canvas_line_draw(vertex[index].gx_point_x, vertex[index].gx_point_y,
                                 vertex[(index + 1) % num].gx_point_x, vertex[(index + 1) % num].gx_point_y);
        }
    }
#if defined (GX_BRUSH_ALPHA_SUPPORT)
    context -> gx_draw_context_brush.gx_brush_alpha = temp_alpha;
#endif
}

