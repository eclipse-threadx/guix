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
/*    _gx_display_driver_8bpp_rotated_vertical_line_draw  PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 8bpp color format rotated vertical line draw function.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    ystart                                y-coord of top endpoint       */
/*    yend                                  y-coord of bottom endpoint    */
/*    xpos                                  x-coord of left edge          */
/*    width                                 width of the line             */
/*    color                                 Color of line to draw         */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_8bpp_rotated_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
{
INT       row;
INT       column;
GX_UBYTE *put;
GX_UBYTE *rowstart;
INT       len = yend - ystart + 1;

    /* Pick up starting address of canvas memory.  */
    rowstart = (GX_UBYTE *)context -> gx_draw_context_memory;

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
            *put++ = (GX_UBYTE)color;
        }

        /* Ddvance to the next scaneline.  */
        rowstart -= context -> gx_draw_context_pitch;
    }
}

