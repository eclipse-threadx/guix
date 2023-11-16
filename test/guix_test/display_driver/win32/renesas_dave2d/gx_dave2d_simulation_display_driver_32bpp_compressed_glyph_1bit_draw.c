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


#define DRAW_PIXEL if (alpha & mask) \
    {                                \
        *put = text_color;           \
    }                                \
    put++;                           \
    mask = (unsigned char)(mask << 1);

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_32bpp_compressed_glyph_1bit_   */
/*    draw                                                                */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws compressed monochrome font on 32bpp canvas,     */
/*    clipped to one viewport.                                            */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_32bpp_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                            GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE            *glyph_data;
INT                  row;
INT                  col;
INT                  start_x;
GX_COLOR             text_color;
GX_UBYTE             alpha_value;
GX_UBYTE             alpha;
GX_UBYTE             mask;
GX_UBYTE             count;
GX_UBYTE             index;
GX_COMPRESSED_GLYPH *compressed_glyph;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
VOID (*blend_func)(GX_DRAW_CONTEXT *, INT, INT, GX_COLOR, GX_UBYTE);
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }

    GX_SET_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);
#endif

    compressed_glyph = (GX_COMPRESSED_GLYPH *)glyph;

    if (compressed_glyph -> gx_glyph_map_size & 0x8000)
    {
        text_color = (GX_COLOR)context -> gx_draw_context_brush.gx_brush_line_color;

        glyph_data = (GX_UBYTE *)glyph -> gx_glyph_map;

        /* First, skip to the starting row.  */
        for (row = 0; row < map_offset -> gx_point_y; row++)
        {
            col = 0;
            while (col < glyph -> gx_glyph_width)
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
                col += count * 8;
            }
        }

        row = draw_area -> gx_rectangle_top;

        /* Now we are on the first visible row, copy pixels until we get
           to the enf of the last visible row.  */
        while (row < draw_area -> gx_rectangle_bottom + 1)
        {
            start_x = draw_area -> gx_rectangle_left - map_offset -> gx_point_x;
            col = start_x;

            while (col < start_x + glyph -> gx_glyph_width)
            {
                count = *glyph_data++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    alpha_value = *glyph_data++;
                    while (count--)
                    {
                        mask = 0x01;
                        if (alpha_value != 0)
                        {
                            for (index = 0; index < 8; index++)
                            {

                                alpha = alpha_value & mask;
                                mask = (GX_UBYTE)(mask << 1);
                                if (alpha != 0)
                                {
                                    if ((col <= draw_area -> gx_rectangle_right) &&
                                        (col >= draw_area -> gx_rectangle_left))
                                    {
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                                        if (brush_alpha != 0xff)
                                        {
                                            blend_func(context, col, row, text_color, brush_alpha);
                                        }
                                        else
                                        {
#endif
                                            _gx_display_driver_32bpp_pixel_write(context, col, row, text_color);
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                                        }
#endif
                                    }
                                }

                                col++;
                            }
                        }
                        else
                        {
                            col += 8;
                        }
                    }
                }
                else
                {
                    count++;
                    while (count--)
                    {
                        alpha_value = *glyph_data++;
                        mask = 0x01;
                        if (alpha_value != 0)
                        {
                            for (index = 0; index < 8; index++)
                            {
                                alpha = alpha_value & mask;
                                mask = (GX_UBYTE)(mask << 1);
                                if (alpha != 0)
                                {
                                    if ((col <= draw_area -> gx_rectangle_right) &&
                                        (col >= draw_area -> gx_rectangle_left))
                                    {
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                                        if (brush_alpha != 0xff)
                                        {
                                            blend_func(context, col, row, text_color, brush_alpha);
                                        }
                                        else
                                        {
#endif
                                            _gx_display_driver_32bpp_pixel_write(context, col, row, text_color);
#if defined(GX_BRUSH_ALPHA_SUPPORT)
                                        }
#endif
                                    }
                                }
                                col++;
                            }
                        }
                        else
                        {
                            col += 8;
                        }
                    }
                }
            }
            row++;
        }
    }
    else
    {
        _gx_dave2d_simulation_display_driver_32bpp_glyph_1bit_draw(context, draw_area, map_offset, glyph);
    }
}

