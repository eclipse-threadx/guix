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
/*    _gx_dave2d_simulation_display_driver_8bpp_glyph_1bit_draw           */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws monochrome font to the 8bpp canvas, clipped     */
/*    to one viweport.                                                    */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_8bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *glyph_row;
GX_UBYTE *glyph_data;
UINT      row;
UINT      pixel_per_row;
UINT      pixel_in_first_byte;
UINT      pixel_in_last_byte;
GX_UBYTE  text_color;
UINT      y_height;
GX_UBYTE  alpha;
UINT      glyph_width;
GX_UBYTE *put;
UINT      num_bytes;
UINT      num_bits;
GX_UBYTE *line_start;
GX_UBYTE  mask, init_mask;
UINT      i;

    text_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_line_color;
    pixel_per_row = (UINT)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1);

    /* pickup pointer to current dispaly driver */
    /*display = context -> gx_draw_context_display;*/

    /* Find the width of the glyph, in terms of bytes */
    glyph_width = glyph -> gx_glyph_width;
    /* Make it byte-aligned. */
    glyph_width = (glyph_width + 7) >> 3;

    /* Compute the number of useful bytes from the glyph this routine is going to use.
       Because of map_offset, the first byte may contain pixel bits we don't need to draw;
       And the width of the draw_area may produce part of the last byte in the row to be ignored. */
    num_bytes = (UINT)(((UINT)(map_offset -> gx_point_x) + pixel_per_row + 7) >> 3);

    /* Take into account if map_offset specifies the number of bytes to ignore from the beginning of the row. */
    num_bytes = (UINT)(num_bytes - (UINT)((UINT)(map_offset -> gx_point_x) >> 3));

    /* Compute the number of pixels to draw from the first byte of the glyph data. */
    pixel_in_first_byte = (UINT)(8 - ((map_offset -> gx_point_x) & 0x7));
    init_mask = (GX_UBYTE)(0x80 >> (pixel_in_first_byte - 1));

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

    line_start = (GX_UBYTE *)context -> gx_draw_context_memory;
    line_start += context -> gx_draw_context_pitch * (draw_area -> gx_rectangle_top);
    line_start += draw_area -> gx_rectangle_left;


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
                DRAW_PIXEL;
                /* fallthrough */
            }
            num_bits = 8;
            mask = 0x01;
        }

        glyph_row += glyph_width;
        line_start += context -> gx_draw_context_pitch;
    }

    return;
}

