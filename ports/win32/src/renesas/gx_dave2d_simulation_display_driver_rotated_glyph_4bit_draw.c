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
/*                                                           6.x          */
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
/*  xx-xx-xxxx     Ting Zhu                 Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
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
GX_VALUE    rotated_map_offset_x;
GX_VALUE    rotated_map_offset_y;
GX_VALUE    rotated_left;
GX_VALUE    rotated_top;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }
#endif
    text_color = context -> gx_draw_context_brush.gx_brush_line_color;
    pixel_width = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

    /* pickup pointer to current dispaly driver */
    display = context -> gx_draw_context_display;

    /* Find the width of the glyph */
    pitch = glyph -> gx_glyph_height;
    /* Make it byte-aligned. */
    pitch = (pitch + 1) >> 1;

    y_height = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_left = draw_area -> gx_rectangle_top;
        rotated_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_right - 1);

        rotated_map_offset_x = map_offset -> gx_point_y;
        rotated_map_offset_y = (GX_VALUE)(glyph -> gx_glyph_width - map_offset -> gx_point_x - (GX_VALUE)y_height);
    }
    else
    {
        rotated_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_bottom - 1);
        rotated_top = draw_area -> gx_rectangle_left;

        rotated_map_offset_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y - (GX_VALUE)pixel_width);
        rotated_map_offset_y = map_offset -> gx_point_x;
    }

    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        glyph_row = glyph_row + ((GX_VALUE)pitch * rotated_map_offset_y);
    }

    glyph_row += (rotated_map_offset_x >> 1);

    GX_SET_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format)

    leading_pixel = (rotated_map_offset_x & 1);

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
                           (INT)((UINT)(rotated_left) + col),
                           (INT)((UINT)(rotated_top) + row),
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
                           (INT)((UINT)rotated_left + col),
                           (INT)((UINT)rotated_top + row),
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
                           (INT)((UINT)rotated_left + col),
                           (INT)((UINT)rotated_top + row),
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
                           (INT)((UINT)rotated_left + col),
                           (INT)((UINT)rotated_top + row),
                           text_color, alpha);
            }
        }

        glyph_row += pitch;
    }
}

