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
/**   Synergy Simulation Display Management (Display)                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"
#include "gx_dave2d_simulation_display_driver.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_8bpp_glyph_4bit_draw           */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws the specified text using the current context,            */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_8bpp_glyph_4bit_draw(GX_DRAW_CONTEXT *context,
                                                                GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
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

    draw_start = (GX_UBYTE *)context -> gx_draw_context_memory;
    draw_start += context -> gx_draw_context_pitch * draw_area -> gx_rectangle_top;
    draw_start += draw_area -> gx_rectangle_left;

    text_color = (GX_UBYTE)(context -> gx_draw_context_brush.gx_brush_line_color + 15);
    pixel_width = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* Find the width of the glyph */
    pitch = glyph -> gx_glyph_width;
    /* Make it byte-aligned. */
    pitch = (pitch + 1) >> 1;

    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        glyph_row = (GX_UBYTE *)(glyph_row + ((GX_VALUE)pitch * map_offset -> gx_point_y));
    }

    glyph_row += (map_offset -> gx_point_x >> 1);

    y_height = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

    leading_pixel = (map_offset -> gx_point_x & 1);

    pixel_width -= leading_pixel;

    trailing_pixel = pixel_width & 1;

    pixel_width = pixel_width >> 1;

    for (row = 0; row < y_height; row++)
    {
        glyph_data = glyph_row;

        put = draw_start;

        if (leading_pixel)
        {
            alpha = (*glyph_data) & 0xf0;
            alpha >>= 4;

            *put = (GX_UBYTE)(text_color - alpha);
            put++;

            glyph_data++;
        }
        for (index = 0; index < pixel_width; index++)
        {
            alpha = (*glyph_data) & 0xf;

            *put = (GX_UBYTE)(text_color - alpha);
            put++;

            alpha = (*glyph_data) & 0xf0;
            *put = (GX_UBYTE)(text_color - (alpha >> 4));
            put++;
            glyph_data++;
        }

        if (trailing_pixel)
        {
            alpha = (*glyph_data) & 0x0f;
            *put = (GX_UBYTE)(text_color - alpha);
            put++;
        }

        glyph_row += pitch;
        draw_start += context -> gx_draw_context_pitch;
    }
}

