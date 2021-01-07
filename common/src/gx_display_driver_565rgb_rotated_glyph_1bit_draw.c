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

#define DRAW_PIXEL  if (alpha & mask) \
    {                                 \
        *put = text_color;            \
    }                                 \
    put++;                            \
    mask = mask >> 1;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
#define BLEND_PIXEL if (alpha & mask)                             \
    {                                                             \
        blend_func(context, xval, yval, text_color, brush_alpha); \
    }                                                             \
    xval++;                                                       \
    mask = mask >> 1;
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_glyph_1bit_draw   PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
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
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_565rgb_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
/* GX_DISPLAY      *display;*/
GX_UBYTE *glyph_row;
GX_UBYTE *glyph_data;
UINT      row;
UINT      pixel_per_row;
UINT      pixel_in_first_byte;
UINT      pixel_in_last_byte = 0;
USHORT    text_color;
UINT      y_height;
GX_UBYTE  alpha;
UINT      glyph_width;
USHORT   *put;
UINT      num_bytes;
UINT      num_bits;
USHORT   *line_start;
GX_UBYTE  mask, init_mask;
UINT      i;
GX_VALUE  rotated_map_offset_x;
GX_VALUE  rotated_map_offset_y;
GX_VALUE  rotated_draw_left;
GX_VALUE  rotated_draw_top;

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
    pixel_per_row = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);
    y_height = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* Find the width of the glyph, in terms of bytes */
    glyph_width = glyph -> gx_glyph_height;

    /* Make it byte-aligned. */
    glyph_width = (glyph_width + 7) >> 3;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_draw_left = draw_area -> gx_rectangle_top;
        rotated_draw_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_right - 1);

        rotated_map_offset_x = map_offset -> gx_point_y;
        rotated_map_offset_y = (GX_VALUE)(glyph -> gx_glyph_width - map_offset -> gx_point_x - (GX_VALUE)y_height);
    }
    else
    {
        rotated_draw_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_display_offset_y - draw_area -> gx_rectangle_bottom - 1);
        rotated_draw_top = draw_area -> gx_rectangle_left;

        rotated_map_offset_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y - (GX_VALUE)pixel_per_row);
        rotated_map_offset_y = map_offset -> gx_point_x;
    }

    /* Compute the number of useful bytes from the glyph this routine is going to use.
       Because of map_offset, the first byte may contain pixel bits we don't need to draw;
       And the width of the draw_area may produce part of the last byte in the row to be ignored. */
    num_bytes = (UINT)(((INT)rotated_map_offset_x + (INT)pixel_per_row + 7) >> 3);

    /* Take into account if map_offset specifies the number of bytes to ignore from the beginning of the row. */
    num_bytes -= ((UINT)(rotated_map_offset_x)) >> 3;

    /* Compute the number of pixels to draw from the first byte of the glyph data. */
    pixel_in_first_byte = (UINT)(8 - ((rotated_map_offset_x) & 0x7));
    init_mask = (GX_UBYTE)(1 << (pixel_in_first_byte - 1));

    /* Compute the number of pixels to draw from the last byte, if there are more than one byte in a row. */
    if (num_bytes != 1)
    {
        pixel_in_last_byte = ((UINT)rotated_map_offset_x + pixel_per_row) & 0x7;
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

    if (rotated_map_offset_y)
    {
        glyph_row = (GX_UBYTE *)(glyph_row + ((INT)glyph_width * (INT)(rotated_map_offset_y)));
    }

    glyph_row += (rotated_map_offset_x >> 3);

    line_start = (USHORT *)context -> gx_draw_context_memory;
    line_start += context -> gx_draw_context_pitch * (rotated_draw_top);
    line_start += rotated_draw_left;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    if (brush_alpha != 0xff)
    {
        yval = rotated_draw_top;
        for (row = 0; row < y_height; row++)
        {
            xval = rotated_draw_left;
            glyph_data = glyph_row;
            alpha = *(glyph_data);
            mask = init_mask;
            num_bits = pixel_in_first_byte;
            for (i = 0; i < num_bytes; i++)
            {
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
                default:
                    if (alpha & mask)
                    {
                        blend_func(context, xval, yval, text_color, brush_alpha);
                    }
                    xval++;
                    break;
                }
                glyph_data++;
                alpha = *(glyph_data);
                num_bits = 8;
                mask = 0x80;
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
            alpha = *(glyph_data);
            mask = init_mask;
            num_bits = pixel_in_first_byte;
            put = line_start;
            for (i = 0; i < num_bytes; i++)
            {
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
                default:
                    if (alpha & mask)
                    {
                        *put = text_color;
                    }
                    put++;
                    break;
                }
                glyph_data++;
                alpha = *(glyph_data);
                num_bits = 8;
                mask = 0x80;
            }

            glyph_row += glyph_width;
            line_start += context -> gx_draw_context_pitch;
        }
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    }
#endif
    return;
}

