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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_16bpp_rotated_compressed_      */
/*                                                        glyph_1bit_draw */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws compressed monochrome font on 16bpp canvas,     */
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
/*    _gx_dave2d_simulation_display_driver_16bpp_compressed_glyph_1bit_   */
/*                                                                   draw */
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
VOID _gx_dave2d_simulation_display_driver_16bpp_rotated_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                                    GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_COMPRESSED_GLYPH rotated_glyph;
GX_RECTANGLE        rotated_draw_area;
GX_POINT            rotated_map_offset;

    rotated_glyph = *((GX_COMPRESSED_GLYPH *)glyph);

    GX_SWAP_VALS(rotated_glyph.gx_glyph_height, rotated_glyph.gx_glyph_width);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_draw_area.gx_rectangle_left = draw_area -> gx_rectangle_top;
        rotated_draw_area.gx_rectangle_right = draw_area -> gx_rectangle_bottom;
        rotated_draw_area.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_right - 1);
        rotated_draw_area.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_left - 1);

        rotated_map_offset.gx_point_x = map_offset -> gx_point_y;
        rotated_map_offset.gx_point_y = (GX_VALUE)(glyph -> gx_glyph_width - map_offset -> gx_point_x -
                                                   (GX_VALUE)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1));
    }
    else
    {
        rotated_draw_area.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_bottom - 1);
        rotated_draw_area.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_top - 1);
        rotated_draw_area.gx_rectangle_top = draw_area -> gx_rectangle_left;
        rotated_draw_area.gx_rectangle_bottom = draw_area -> gx_rectangle_right;

        rotated_map_offset.gx_point_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y -
                                                   (GX_VALUE)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1));
        rotated_map_offset.gx_point_y = map_offset -> gx_point_x;
    }

    _gx_dave2d_simulation_display_driver_16bpp_compressed_glyph_1bit_draw(context, &rotated_draw_area, &rotated_map_offset, (GX_GLYPH *)&rotated_glyph);
}

