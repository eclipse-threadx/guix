/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_ertical_pattern_line_draw   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 8bpp color format vertical line draw function.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    ystart                                y-coord of top endpoint       */
/*    yend                                  y-coord of bottom endpoint    */
/*    xpos                                  x-coord of left edge          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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
VOID _gx_display_driver_8bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos)
{
INT       row;
GX_UBYTE *put;
GX_UBYTE *rowstart;
ULONG     pattern;
ULONG     mask;
GX_UBYTE  on_color;
GX_UBYTE  off_color;

INT       len = yend - ystart + 1;

    /* pick up starting address of canvas memory */
    rowstart =  (GX_UBYTE *)context -> gx_draw_context_memory;

    /* calculate start of scanline */
    rowstart += context -> gx_draw_context_pitch * ystart;

    /* offset into starting pixel */
    rowstart += xpos;

    /* pick up the requested pattern and mask */
    pattern = context -> gx_draw_context_brush.gx_brush_line_pattern;
    mask = context -> gx_draw_context_brush.gx_brush_pattern_mask;
    on_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_line_color;
    off_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_fill_color;

    /* draw line from top to bottom */
    for (row = 0; row < len; row++)
    {
        put = rowstart;

        if (pattern & mask)
        {
            *put = on_color;
        }
        else
        {
            *put = off_color;
        }

        mask >>= 1;
        if (!mask)
        {
            mask = 0x80000000;
        }

        /* advance to the next scaneline */
        rowstart +=  context -> gx_draw_context_pitch;
    }
    /* save current masks value back to brush */
    context -> gx_draw_context_brush.gx_brush_pattern_mask = mask;
}

