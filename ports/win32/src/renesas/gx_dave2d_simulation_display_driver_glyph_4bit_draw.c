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

#define Extend_alpha_4bit(alpha) alpha = (GX_UBYTE)(alpha | (alpha >> 4));


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_glyph_4bit_raw_draw            */
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
/*    _gx_dave2d_simulation_display_driver_glyph_4bit_draw                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                           GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_DISPLAY *display;
GX_UBYTE   *glyph_row;
GX_UBYTE   *glyph_data;
UINT        row;
UINT        col;
UINT        pixel_width = 0;
UINT        leading_pixel;
UINT        trailing_pixel;
GX_COLOR    text_color;
UINT        y_height;
GX_UBYTE    alpha;
UINT        pitch;
UINT        index;
VOID        (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }
#endif
    text_color = context -> gx_draw_context_brush.gx_brush_line_color;
    pixel_width = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* pickup pointer to current dispaly driver */
    display = context -> gx_draw_context_display;

    /* Find the width of the glyph */
    pitch = glyph -> gx_glyph_width;
    /* Make it byte-aligned. */
    pitch = (pitch + 1) >> 1;

    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        glyph_row = glyph_row + ((GX_VALUE)pitch * map_offset -> gx_point_y);
    }

    glyph_row += (map_offset -> gx_point_x >> 1);

    y_height = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

    blend_func = display -> gx_display_driver_pixel_blend;

    leading_pixel = (map_offset -> gx_point_x & 1);

    pixel_width -= leading_pixel;

    trailing_pixel = pixel_width & 1;

    pixel_width = pixel_width >> 1;

    for (row = 0; row < y_height; row++)
    {
        col = 0;
        glyph_data = glyph_row;

        if (leading_pixel)
        {
            alpha = (*glyph_data) & 0xf0;

            if (alpha > 0)
            {
                Extend_alpha_4bit(alpha)
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                alpha = (GX_UBYTE)(alpha * brush_alpha / 255);
#endif
                blend_func(context,
                           (INT)((UINT)(draw_area -> gx_rectangle_left) + col),
                           (INT)((UINT)(draw_area -> gx_rectangle_top) + row),
                           text_color, alpha);
            }
            col++;
            glyph_data++;
        }
        for (index = 0; index < pixel_width; index++)
        {
            alpha = (*glyph_data) & 0x0f;

            if (alpha > 0)
            {
                alpha = (GX_UBYTE)(alpha << 4);
                Extend_alpha_4bit(alpha)
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                alpha = (GX_UBYTE)(alpha * brush_alpha / 255);
#endif
                blend_func(context,
                           (INT)((UINT)draw_area -> gx_rectangle_left + col),
                           (INT)((UINT)draw_area -> gx_rectangle_top + row),
                           text_color, alpha);
            }
            col++;
            alpha = (*glyph_data) & 0xf0;

            if (alpha > 0)
            {
                Extend_alpha_4bit(alpha)
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                alpha = (GX_UBYTE)(alpha * brush_alpha / 255);
#endif
                blend_func(context,
                           (INT)((UINT)draw_area -> gx_rectangle_left + col),
                           (INT)((UINT)draw_area -> gx_rectangle_top + row),
                           text_color, alpha);
            }
            col++;
            glyph_data++;
        }

        if (trailing_pixel)
        {
            alpha = (*glyph_data) & 0x0f;

            if (alpha > 0)
            {
                alpha = (GX_UBYTE)(alpha << 4);
                Extend_alpha_4bit(alpha)
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                alpha = (GX_UBYTE)(alpha * brush_alpha / 255);
#endif
                blend_func(context,
                           (INT)((UINT)draw_area -> gx_rectangle_left + col),
                           (INT)((UINT)draw_area -> gx_rectangle_top + row),
                           text_color, alpha);
            }
        }

        glyph_row += pitch;
    }
}

