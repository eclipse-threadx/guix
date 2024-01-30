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

#define DRAW_PIXEL  if (alpha & mask) \
    {                                 \
        *put = text_color;            \
    }                                 \
    put++;                            \
    mask = (unsigned char)(mask << 1);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
#define BLEND_PIXEL if (alpha & mask)                             \
    {                                                             \
        blend_func(context, xval, yval, text_color, brush_alpha); \
    }                                                             \
    xval++;                                                       \
    mask = (unsigned char)(mask << 1);
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_16bpp_glyph_1bit_raw_draw      */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This functions draw monochrome font on 16bpp canvas, clipped to     */
/*    one viewport.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Draw context                  */
/*    draw_area                             The region bound by the       */
/*                                            rectangle where the glyph   */
/*                                            is drawn                    */
/*    map_offset                            X,Y offset into the glyph map */
/*    glyph                                 Pointer to the glyph          */
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
/*    GUIX internal code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_16bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                 GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *glyph_row;
GX_UBYTE *glyph_data;
UINT      row;
UINT      pixel_per_row;
UINT      pixel_in_first_byte;
UINT      pixel_in_last_byte;
USHORT    text_color;
UINT      y_height;
GX_UBYTE  alpha;
UINT      glyph_width;
USHORT   *put;
UINT      num_bytes;
UINT      num_bits;
USHORT   *line_start;
GX_UBYTE  mask;
GX_UBYTE  init_mask;
UINT      i;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE brush_alpha;
INT      xval, yval;
VOID     (*blend_func)(GX_DRAW_CONTEXT *, INT, INT, GX_COLOR, GX_UBYTE);

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }

    blend_func = _gx_display_driver_565rgb_pixel_blend;
#endif

    text_color = (USHORT)context -> gx_draw_context_brush.gx_brush_line_color;
    pixel_per_row = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* Find the width of the glyph, in terms of bytes */
    glyph_width = glyph -> gx_glyph_width;

    /* Make it byte-aligned. */
    glyph_width = (glyph_width + 7) >> 3;

    /* Compute the number of useful bytes from the glyph this routine is going to use.
       Because of map_offset, the first byte may contain pixel bits we don't need to draw;
       And the width of the draw_area may produce part of the last byte in the row to be ignored. */
    num_bytes = ((UINT)(map_offset -> gx_point_x) + pixel_per_row + 7) >> 3;

    /* Take into account if map_offset specifies the number of bytes to ignore from the beginning of the row. */
    num_bytes = num_bytes - (UINT)((UINT)(map_offset -> gx_point_x) >> 3);

    /* Compute the number of pixels to draw from the first byte of the glyph data. */
    pixel_in_first_byte = (UINT)(8 - ((UINT)(map_offset -> gx_point_x) & 0x7));
    init_mask = (GX_UBYTE)(0x80 >> ((GX_UBYTE)pixel_in_first_byte - 1));

    /* Compute the number of pixels to draw from the last byte, if there are more than one byte in a row. */
    if (num_bytes != 1)
    {
        pixel_in_last_byte = ((UINT)(map_offset -> gx_point_x) + pixel_per_row) & 0x7;
        if (pixel_in_last_byte == 0)
        {
            pixel_in_last_byte = 8;
        }
    }
    else
    {
        pixel_in_first_byte = pixel_per_row;
    }


    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        glyph_row = glyph_row + ((GX_VALUE)glyph_width * map_offset -> gx_point_y);
    }

    glyph_row += (map_offset -> gx_point_x >> 3);

    y_height = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

    line_start = (USHORT *)context -> gx_draw_context_memory;
    line_start += context -> gx_draw_context_pitch * (draw_area -> gx_rectangle_top);
    line_start += draw_area -> gx_rectangle_left;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    if (brush_alpha != 0xff)
    {
        yval = draw_area -> gx_rectangle_top;

        for (row = 0; row < y_height; row++)
        {
            glyph_data = glyph_row;
            xval = draw_area -> gx_rectangle_left;

            mask = init_mask;
            num_bits = pixel_in_first_byte;

            for (i = 0; i < num_bytes; i++)
            {
                alpha = *(glyph_data++);

                if ((i == (num_bytes - 1)) && (num_bytes > 1))
                {
                    num_bits = pixel_in_last_byte;
                }
                switch (num_bits)
                {
                case 8:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 7:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 6:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 5:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 4:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 3:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 2:
                    BLEND_PIXEL;
                    /* fallthrough */
                case 1:
                    if (alpha & mask)
                    {
                        blend_func(context, xval, yval, text_color, brush_alpha);
                    }
                    xval++;
                    break;
                }
                num_bits = 8;
                mask = 0x01;
            }

            glyph_row += glyph_width;
            yval++;
        }
    }
    else
    {
#endif
        for (row = 0; row < y_height; row++)
        {
            glyph_data = glyph_row;

            mask = init_mask;
            num_bits = pixel_in_first_byte;
            put = line_start;

            for (i = 0; i < num_bytes; i++)
            {
                alpha = *(glyph_data++);

                if ((i == (num_bytes - 1)) && (num_bytes > 1))
                {
                    num_bits = pixel_in_last_byte;
                }
                switch (num_bits)
                {
                case 8:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 7:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 6:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 5:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 4:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 3:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 2:
                    DRAW_PIXEL;
                    /* fallthrough */
                case 1:
                    if (alpha & mask)
                    {
                        *put = text_color;
                    }
                    put++;
                    break;
                }
                num_bits = 8;
                mask = 0x01;
            }

            glyph_row += glyph_width;
            line_start += context -> gx_draw_context_pitch;
        }
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    }
#endif
    return;
}

