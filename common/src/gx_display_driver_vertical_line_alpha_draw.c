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

#if defined GX_BRUSH_ALPHA_SUPPORT  
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_vertical_line_alpha_draw         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic driver function that handles drawing vertical line with     */
/*    brush alpha.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    ystart                                y-coord of top endpoint       */
/*    yend                                  y-coord of bottom endpoint    */
/*    xpos                                  x-coord of left edge          */
/*    width                                 width of the line             */
/*    color                                 Color of line to write        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_vertical_line_alpha_draw                   */
/*    _gx_display_driver_16bpp_vertical_line_alpha_draw                   */
/*    _gx_display_driver_8bpp_vertical_line_alpha_draw                    */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_vertical_line_alpha_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color, GX_UBYTE alpha)
{
INT     xval;
INT     yval;
INT     xend;
VOID  (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    xend = xpos + width;
    /* draw line from top to bottom */
    for (yval = ystart; yval <= yend; yval++)
    {
        /* draw line width from left to right */
        for (xval = xpos; xval < xend; xval++)
        {
            blend_func(context, xval, yval, color, alpha);
        }
    }
}

#endif
