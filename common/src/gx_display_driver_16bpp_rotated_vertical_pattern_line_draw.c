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
/*    _gx_display_driver_16bpp_rotated_vertical_pattern_line_draw         */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16bpp color format rotated vertical pattern line draw       */
/*    function.                                                           */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_16bpp_rotated_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos)
{
INT     row;
USHORT *put;
USHORT *rowstart;
ULONG   pattern;
ULONG   mask;
USHORT  on_color;
USHORT  off_color;

INT     len = yend - ystart + 1;

    /* pick up starting address of canvas memory */
    rowstart = (USHORT *)context -> gx_draw_context_memory;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        /* calculate start of scanline */
        rowstart += (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - xpos - 1) * context -> gx_draw_context_pitch;

        /* offset into starting pixel */
        rowstart += ystart;
    }
    else
    {
        /* calculate start of scanline */
        rowstart += xpos * context -> gx_draw_context_pitch;

        /* offset into starting pixel */
        rowstart += (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - yend - 1);
    }

    /* pick up the requested pattern and mask */
    pattern = context -> gx_draw_context_brush.gx_brush_line_pattern;
    mask = context -> gx_draw_context_brush.gx_brush_pattern_mask;
    on_color = (USHORT)context -> gx_draw_context_brush.gx_brush_line_color;
    off_color = (USHORT)context -> gx_draw_context_brush.gx_brush_fill_color;

    put = rowstart;

    /* draw line from left to right */
    for (row = 0; row < len; row++)
    {
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

        /* advance to the right */
        put++;
    }

    /* save current masks value back to brush */
    context -> gx_draw_context_brush.gx_brush_pattern_mask = mask;
}

