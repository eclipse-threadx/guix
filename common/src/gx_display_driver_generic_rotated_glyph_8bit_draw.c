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
/*    _gx_display_driver_generic_rotated_glyph_8bit_draw  PORTABLE C      */
/*                                                           6.1.3        */
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
/*    _gx_display_driver_565rgb_pixel_blend Call display driver pixel     */
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
VOID _gx_display_driver_generic_rotated_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *glyph_row;
GX_UBYTE *glyph_data;
UINT      row;
UINT      col;
UINT      pixel_width = 0;
GX_COLOR  text_color;
UINT      y_height;
GX_UBYTE  alpha1;
GX_UBYTE  brush_alpha = 0xff;
GX_VALUE  rotated_map_offset_x;
GX_VALUE  rotated_map_offset_y;
GX_VALUE  rotated_left;
GX_VALUE  rotated_top;
VOID     (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }
#endif

    GX_SET_BLEND_FUNCTION(blend_func, context->gx_draw_context_display->gx_display_color_format)

    text_color = context -> gx_draw_context_brush.gx_brush_line_color;

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

    glyph_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (rotated_map_offset_y)
    {
        glyph_row = glyph_row + (glyph -> gx_glyph_height * rotated_map_offset_y);
    }

    glyph_row += rotated_map_offset_x;

    if (brush_alpha == 0xff)
    {
        for (row = 0; row < y_height; row++)
        {
            glyph_data = glyph_row;

            for (col = 0; col < pixel_width; col++)
            {
                alpha1 = *glyph_data;

                if (alpha1 > 0)
                {
                    blend_func(context,
                               rotated_left + (GX_VALUE)col,
                               rotated_top + (GX_VALUE)row,
                               text_color, (GX_UBYTE)alpha1);
                }
                glyph_data++;
            }
            glyph_row += glyph -> gx_glyph_height;
        }
    }
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    else
    {
        for (row = 0; row < y_height; row++)
        {
            glyph_data = glyph_row;

            for (col = 0; col < pixel_width; col++)
            {
                alpha1 = *glyph_data;
                alpha1 = (GX_UBYTE)(alpha1 * brush_alpha / 255);

                if (alpha1 > 0)
                {
                    blend_func(context,
                               rotated_left + (GX_VALUE)col,
                               rotated_top + (GX_VALUE)row,
                               text_color, (GX_UBYTE)alpha1);
                }

                glyph_data++;
            }
            glyph_row += glyph -> gx_glyph_height;
        }
    }
#endif
}

