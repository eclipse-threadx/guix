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
/*    _gx_display_driver_32bpp_rotated_vertical_line_draw PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic rotated vertical line draw function for 16bpp canvas.       */
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
/*    _gx_display_driver_vertical_line_alpha_draw                         */
/*                                          Basic display driver vertical */
/*                                            line alpha draw function    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_32bpp_rotated_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
{
INT    row;
INT    column;
ULONG *put;
ULONG *rowstart;
INT    len = yend - ystart + 1;

#if defined GX_BRUSH_ALPHA_SUPPORT
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (alpha == 0)
    {
        /* Nothing to drawn. Just return.  */
        return;
    }
    if (alpha != 0xff)
    {
        _gx_display_driver_vertical_line_alpha_draw(context, ystart, yend, xpos, width, color, alpha);
        return;
    }
#endif

    /* Pick up starting address of canvas memory.  */
    rowstart = (ULONG *)context -> gx_draw_context_memory;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {

        /* Calculate start of scanline.  */
        rowstart += (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - xpos - 1) * context -> gx_draw_context_pitch;

        /* Offset into starting pixel.  */
        rowstart += ystart;
    }
    else
    {
        rowstart += (xpos + width - 1) * context -> gx_draw_context_pitch;
        rowstart += (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - yend - 1);
    }

    /* Draw line width from left to right.  */
    for (column = 0; column < width; column++)
    {
        put = rowstart;

        /* Draw line from top to bottom.  */
        for (row = 0; row < len; row++)
        {
            *put++ = (ULONG)color;
        }

        /* Advance to the next scaneline.  */
        rowstart -= context -> gx_draw_context_pitch;
    }
}

