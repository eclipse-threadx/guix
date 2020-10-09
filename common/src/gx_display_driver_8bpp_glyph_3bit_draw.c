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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_glyph_3bit_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws the specified 3bit glyph using the current context,      */
/*    clipped to one viewport                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Draw context                  */
/*    draw_position                         The X and Y coordinate where  */
/*                                            the glyph is drawn to       */
/*    string                                String to draw                */
/*    count                                 Count of string characters    */
/*    view                                  view to clip drawing within   */
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
/*    _gx_canvas_text_draw                                                */
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
VOID _gx_display_driver_8bpp_glyph_3bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *glyph_row;
GX_UBYTE *glyph_data;
UINT      row;
UINT      pixel_width = 0;
UINT      leading_pixel;
UINT      trailing_pixel;
GX_UBYTE  text_color;
UINT      y_height;
GX_UBYTE  alpha;
UINT      pitch;
UINT      index;
GX_UBYTE *put;
GX_UBYTE *draw_start;

    text_color = (GX_UBYTE)(context -> gx_draw_context_brush.gx_brush_line_color + 7);
    pixel_width = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* Find the width of the glyph */
    pitch = glyph -> gx_glyph_width;
    /* Make it byte-aligned. */
    pitch = (pitch + 1) >> 1;

    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        glyph_row = glyph_row + ((INT)pitch * map_offset -> gx_point_y);
    }

    glyph_row += (map_offset -> gx_point_x >> 1);

    y_height = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

    leading_pixel = (map_offset -> gx_point_x & 1);

    pixel_width -= leading_pixel;

    trailing_pixel = pixel_width & 1;

    pixel_width = pixel_width >> 1;

    draw_start = (GX_UBYTE *)context -> gx_draw_context_memory;
    draw_start += context -> gx_draw_context_pitch * draw_area -> gx_rectangle_top;
    draw_start += draw_area -> gx_rectangle_left;

    for (row = 0; row < y_height; row++)
    {
        glyph_data = glyph_row;

        put = draw_start;

        if (leading_pixel)
        {
            alpha = (*glyph_data) & 0x0f;

            /* use 3bit color gradient instead of 4 bit */
            alpha >>= 1;

            *put = (GX_UBYTE)(text_color - alpha);
            put++;

            glyph_data++;
        }
        for (index = 0; index < pixel_width; index++)
        {
            alpha = (*glyph_data) & 0xf0;

            /* use 3bit color gradient instead of 4 bit */
            alpha >>= 1;

            *put = (GX_UBYTE)(text_color - (alpha >> 4));
            put++;

            alpha = (*glyph_data) & 0x0f;

            /* use 3bit color gradient instead of 4 bit */
            alpha >>= 1;

            *put = (GX_UBYTE)(text_color - alpha);
            put++;
            glyph_data++;
        }

        if (trailing_pixel)
        {
            alpha = (*glyph_data) & 0xf0;

            /* use 3bit color gradient instead of 4 bit */
            alpha >>= 1;

            *put = (GX_UBYTE)(text_color - (alpha >> 4));
            put++;
        }

        glyph_row += pitch;
        draw_start += context -> gx_draw_context_pitch;
    }
}

