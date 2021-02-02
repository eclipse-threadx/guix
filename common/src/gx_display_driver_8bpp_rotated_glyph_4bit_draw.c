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
/*    _gx_display_driver_8bpp_rotated_glyph_4bit_draw     PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
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
/*    _gx_canvas_text_draw                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_8bpp_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
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
GX_VALUE  rotated_map_offset_x;
GX_VALUE  rotated_map_offset_y;
GX_VALUE  rotated_left;
GX_VALUE  rotated_top;

    text_color = (GX_UBYTE)(context -> gx_draw_context_brush.gx_brush_line_color + 15);
    pixel_width = (UINT)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1);

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
        rotated_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_display_offset_y - draw_area -> gx_rectangle_bottom - 1);
        rotated_top = draw_area -> gx_rectangle_left;

        rotated_map_offset_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y - (GX_VALUE)pixel_width);
        rotated_map_offset_y = map_offset -> gx_point_x;
    }

    leading_pixel = (rotated_map_offset_x & 1);

    pixel_width -= leading_pixel;

    trailing_pixel = pixel_width & 1;

    pixel_width = pixel_width >> 1;

    /* Find the width of the glyph.  */
    pitch = glyph -> gx_glyph_height;

    /* Make it byte-aligned.  */
    pitch = (pitch + 1) >> 1;

    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (rotated_map_offset_y)
    {
        glyph_row = glyph_row + ((INT)pitch * rotated_map_offset_y);
    }

    glyph_row += (rotated_map_offset_x >> 1);

    draw_start = (GX_UBYTE *)context -> gx_draw_context_memory;
    draw_start += context -> gx_draw_context_pitch * rotated_top;
    draw_start += rotated_left;

    for (row = 0; row < y_height; row++)
    {
        glyph_data = glyph_row;

        put = draw_start;

        if (leading_pixel)
        {
            alpha = (*glyph_data) & 0x0f;

            *put = (GX_UBYTE)(text_color - alpha);
            put++;

            glyph_data++;
        }
        for (index = 0; index < pixel_width; index++)
        {
            alpha = (*glyph_data) & 0xf0;

            *put = (GX_UBYTE)(text_color - (alpha >> 4));
            put++;

            alpha = (*glyph_data) & 0x0f;
            *put = (GX_UBYTE)(text_color - alpha);
            put++;
            glyph_data++;
        }

        if (trailing_pixel)
        {
            alpha = (*glyph_data) & 0xf0;
            *put = (GX_UBYTE)(text_color - (alpha >> 4));
            put++;
        }

        glyph_row += pitch;
        draw_start += context -> gx_draw_context_pitch;
    }
}

