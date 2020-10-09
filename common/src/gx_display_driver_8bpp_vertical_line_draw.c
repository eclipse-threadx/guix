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
/*    _gx_display_driver_8bpp_vertical_line_draw          PORTABLE C      */
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
/*    width                                 width of the line             */
/*    color                                 Color of line to draw         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_vertical_line_alpha_draw                         */
/*                                          Basic display driver vertical */
/*                                            alpha line draw function    */
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
VOID _gx_display_driver_8bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
{
INT       row;
INT       column;
GX_UBYTE *put;
GX_UBYTE *rowstart;
INT       len = yend - ystart + 1;
#if defined GX_BRUSH_ALPHA_SUPPORT  
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (alpha == 0)
    {
        /* Nothing to drawn. Just return. */
        return;
    }
    if (alpha != 0xff)
    {
        if (context -> gx_draw_context_display -> gx_display_color_format != GX_COLOR_FORMAT_8BIT_PACKED_PIXEL)
        {
            /* Alpha blend is not supported for palette driver. */
            return;
        }

        _gx_display_driver_vertical_line_alpha_draw(context, ystart, yend, xpos, width, color, alpha);
        return;
    }
#endif

    /* pick up starting address of canvas memory */
    rowstart = (GX_UBYTE *)context -> gx_draw_context_memory;

    /* calculate start of scanline */
    rowstart += context -> gx_draw_context_pitch * ystart;

    /* offset into starting pixel */
    rowstart += xpos;

    /* draw line from top to bottom */
    for (row = 0; row < len; row++)
    {
        put = rowstart;

        /* draw line width from left to right */
        for (column = 0; column < width; column++)
        {
            *put++ = (GX_UBYTE)color;
        }

        /* advance to the next scaneline */
        rowstart += context -> gx_draw_context_pitch;
    }
}
