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
/*    _gx_display_driver_generic_glyph_4bit_draw          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
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
GX_UBYTE    brush_alpha = 0xff;

#if defined (GX_BRUSH_ALPHA_SUPPORT)
INT         alpha_sum;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }
#endif

    text_color =  context -> gx_draw_context_brush.gx_brush_line_color;
    pixel_width = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* pickup pointer to current dispaly driver */
    display = context -> gx_draw_context_display;

    if (display -> gx_display_driver_pixel_blend == GX_NULL)
    {
        return;
    }

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

    blend_func = display -> gx_display_driver_pixel_blend;

    leading_pixel = (map_offset -> gx_point_x & 1);

    pixel_width -= leading_pixel;

    trailing_pixel = pixel_width & 1;

    pixel_width = pixel_width >> 1;

    if (brush_alpha == 0xff)
    {
        for (row = 0; row < y_height; row++)
        {
            col = 0;
            glyph_data = glyph_row;

            if (leading_pixel)
            {
                alpha = (*glyph_data) & 0x0f;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha << 4));

                if (alpha > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha);
                }
                col++;
                glyph_data++;
            }

            for (index = 0; index < pixel_width; index++)
            {
                alpha = (*glyph_data) & 0xf0;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha >> 4));

                if (alpha > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha);
                }
                col++;

                alpha = (*glyph_data) & 0x0f;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha << 4));

                if (alpha > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha);
                }
                col++;
                glyph_data++;
            }

            if (trailing_pixel)
            {
                alpha = (*glyph_data) & 0xf0;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha >> 4));

                if (alpha > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha);
                }
            }
            glyph_row += pitch;
        }
    }
#if defined (GX_BRUSH_ALPHA_SUPPORT)
    else
    {
        for (row = 0; row < y_height; row++)
        {
            col = 0;
            glyph_data = glyph_row;

            if (leading_pixel)
            {
                alpha = (*glyph_data) & 0x0f;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha << 4));

                alpha_sum = alpha * brush_alpha / 255;

                if (alpha_sum > 0)
                {
                    blend_func(context,
                        draw_area->gx_rectangle_left + (GX_VALUE)col,
                        draw_area->gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha_sum);
                }
                col++;
                glyph_data++;
            }

            for (index = 0; index < pixel_width; index++)
            {
                alpha = (*glyph_data) & 0xf0;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha >> 4));
                alpha_sum = alpha * brush_alpha / 255;

                if (alpha_sum > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha_sum);
                }
                col++;

                alpha = (*glyph_data) & 0x0f;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha << 4));
                alpha_sum = alpha * brush_alpha / 255;

                if (alpha_sum > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha_sum);
                }
                col++;
                glyph_data++;
            }

            if (trailing_pixel)
            {
                alpha = (*glyph_data) & 0xf0;
                alpha = (GX_UBYTE)(alpha | (GX_UBYTE)(alpha >> 4));
                alpha_sum = alpha * brush_alpha / 255;

                if (alpha_sum > 0)
                {
                    blend_func(context,
                        draw_area -> gx_rectangle_left + (GX_VALUE)col,
                        draw_area -> gx_rectangle_top + (GX_VALUE)row,
                        text_color, (GX_UBYTE)alpha_sum);
                }
            }
            glyph_row += pitch;
        }
    }
#endif

}

