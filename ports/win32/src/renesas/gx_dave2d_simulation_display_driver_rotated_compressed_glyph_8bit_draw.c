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
/*    _gx_dave2d_simulation_display_driver_compressed_glyph_8bit_draw     */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified text using the current context,   */
/*    clipped to one viewport                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Draw context                  */
/*    draw_area                             The rectangle where the glyph */
/*                                            is drawn to                 */
/*    map_offset                            Offset from the glyph map     */
/*    glyph                                 The glyph structure           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Call display driver pixel     */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX internal code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_rotated_compressed_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                              GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_DISPLAY          *display;
GX_UBYTE            *glyph_data;
INT                  row;
INT                  col;
INT                  start_x;
GX_COLOR             text_color;
GX_UBYTE             alpha;
GX_UBYTE             count;
GX_COMPRESSED_GLYPH *compressed_glyph;
GX_RECTANGLE         rotated_draw_area;
GX_POINT             rotated_map_offset;
VOID                 (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

GX_UBYTE             brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        return;
    }

    compressed_glyph = (GX_COMPRESSED_GLYPH *)glyph;

    if (compressed_glyph -> gx_glyph_map_size & 0x8000)
    {
        text_color = context -> gx_draw_context_brush.gx_brush_line_color;

        if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            rotated_draw_area.gx_rectangle_left = draw_area -> gx_rectangle_top;
            rotated_draw_area.gx_rectangle_right = draw_area -> gx_rectangle_bottom;
            rotated_draw_area.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_right - 1);
            rotated_draw_area.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_left - 1);

            rotated_map_offset.gx_point_x = map_offset -> gx_point_y;
            rotated_map_offset.gx_point_y = (GX_VALUE)(glyph -> gx_glyph_width - map_offset -> gx_point_x -
                                                       (GX_VALUE)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1));
        }
        else
        {
            rotated_draw_area.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_bottom - 1);
            rotated_draw_area.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_top - 1);
            rotated_draw_area.gx_rectangle_top = draw_area -> gx_rectangle_left;
            rotated_draw_area.gx_rectangle_bottom = draw_area -> gx_rectangle_right;

            rotated_map_offset.gx_point_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y -
                                                       (GX_VALUE)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1));
            rotated_map_offset.gx_point_y = map_offset -> gx_point_x;
        }

        /* Pickup pointer to current dispaly driver.  */
        display = context -> gx_draw_context_display;

        glyph_data = (GX_UBYTE *)glyph -> gx_glyph_map;

        /* First, skip to the starting row.  */
        for (row = 0; row < rotated_map_offset.gx_point_y; row++)
        {
            col = 0;
            while (col < glyph -> gx_glyph_height)
            {
                count = *glyph_data++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    glyph_data++;
                }
                else
                {
                    count++;
                    glyph_data += count;
                }
                col += count;
            }
        }

        GX_SET_BLEND_FUNCTION(blend_func, display -> gx_display_color_format)
        row = rotated_draw_area.gx_rectangle_top;

        /* Now we are on the first visible row, copy pixels until we get
           to the enf of the last visible row.  */
        while (row < rotated_draw_area.gx_rectangle_bottom + 1)
        {
            start_x = rotated_draw_area.gx_rectangle_left - rotated_map_offset.gx_point_x;
            col = start_x;

            while (col < start_x + glyph -> gx_glyph_height)
            {
                count = *glyph_data++;

                if (count & 0x80)
                {
                    /* Repeat.  */
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    alpha = *glyph_data++;

                    alpha = (GX_UBYTE)(alpha * brush_alpha / 255);

                    if (alpha > 0)
                    {
                        while (count--)
                        {

                            if ((col <= rotated_draw_area.gx_rectangle_right) &&
                                (col >= rotated_draw_area.gx_rectangle_left))
                            {
                                blend_func(context,
                                           col,
                                           row,
                                           text_color,
                                           (GX_UBYTE)alpha);
                            }
                            col++;
                        }
                    }
                    else
                    {
                        col += count;
                    }
                }
                else
                {
                    count++;
                    while (count--)
                    {
                        alpha = *glyph_data++;

                        alpha = (GX_UBYTE)(alpha * brush_alpha / 255);

                        if (alpha > 0)
                        {
                            if (col <= rotated_draw_area.gx_rectangle_right &&
                                col >= rotated_draw_area.gx_rectangle_left)
                            {
                                blend_func(context,
                                           col,
                                           row,
                                           text_color,
                                           (GX_UBYTE)alpha);
                            }
                        }
                        col++;
                    }
                }
            }
            row++;
        }
    }
    else
    {
        _gx_display_driver_generic_rotated_glyph_8bit_draw(context, draw_area, map_offset, glyph);
    }
}

