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
/*    _gx_display_driver_4bpp_glyph_1bit_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws monochrome font to the 4bpp canvas, clipped     */
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

#define WRITE_PIXEL if (alpha & mask)                      \
    {                                                      \
        *put = *put & (GX_UBYTE)(~putmask);                \
        *put = *put | (GX_UBYTE)(text_color & putmask);    \
    }                                                      \
    putmask = putmask >> 4;                                \
    if (putmask == 0)                                      \
    {                                                      \
        putmask = 0xf0;                                    \
        put++;                                             \
    }                                               


VOID _gx_display_driver_4bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, const GX_GLYPH *glyph)
{
GX_UBYTE *glyph_row;
GX_UBYTE *glyph_data;
UINT      row;
UINT      pixel_per_row;
UINT      pixel_in_first_byte;
UINT      pixel_in_last_byte = 0;
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
GX_UBYTE  putmask;
INT       putstride;

    text_color =  (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_line_color;
    text_color |= (GX_UBYTE)(text_color << 4);
    pixel_per_row = (UINT)draw_area -> gx_rectangle_right - (UINT)draw_area -> gx_rectangle_left + (UINT)1;
    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    /* pickup pointer to current dispaly driver */
    /*display = context -> gx_draw_context_display;*/

    /* Find the width of the glyph, in terms of bytes */
    glyph_width = glyph -> gx_glyph_width;
    /* Make it byte-aligned. */
    glyph_width = (glyph_width + 7) >> 3;

    /* Compute the number of useful bytes from the glyph this routine is going to use.
       Because of map_offset, the first byte may contain pixel bits we don't need to draw;
       And the width of the draw_area may produce part of the last byte in the row to be ignored. */
    num_bytes = ((UINT)map_offset -> gx_point_x + pixel_per_row + 7) >> 3;
    /* Take into account if map_offset specifies the number of bytes to ignore from the beginning of the row. */
    num_bytes -= (UINT)(map_offset -> gx_point_x) >> 3;

    /* Compute the number of pixels to draw from the first byte of the glyph data. */
    pixel_in_first_byte = (UINT)(8 - ((map_offset -> gx_point_x) & 0x7));
    init_mask = (GX_UBYTE)(1 << (pixel_in_first_byte - 1));
    /* Compute the number of pixels to draw from the last byte, if there are more than one byte in a row. */
    if (num_bytes != 1)
    {
        pixel_in_last_byte = (map_offset -> gx_point_x + (INT)pixel_per_row) & 0x7;
        if (pixel_in_last_byte == 0)
        {
            pixel_in_last_byte = 8;
        }
    }
    else
    {
        if ((map_offset -> gx_point_x + (INT)pixel_per_row) < 8)
        {
            pixel_in_first_byte = pixel_per_row;
        }
        else
        {
            pixel_in_last_byte = 0;
        }
    }


    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        glyph_row = glyph_row + ((INT)glyph_width * map_offset -> gx_point_y);
    }

    glyph_row += (map_offset -> gx_point_x >> 3);

    y_height = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

    line_start = (GX_UBYTE *)context -> gx_draw_context_memory;
    line_start += putstride * (draw_area -> gx_rectangle_top);
    line_start += draw_area -> gx_rectangle_left >> 1;


    for (row = 0; row < y_height; row++)
    {
        if (draw_area -> gx_rectangle_left & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }
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
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            case 7:
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            case 6:
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            case 5:
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            case 4:
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            case 3:
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            case 2:
                WRITE_PIXEL;
                mask >>= 1;
                /* fallthrough */            
            default:
                WRITE_PIXEL;
            }
            glyph_data++;
            alpha = *(glyph_data);
            num_bits = 8;
            mask = 0x80;
        }

        glyph_row +=  glyph_width;
        line_start += putstride;
    }

    return;
}

