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
#include "gx_dave2d_simulation_display_driver.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_s_display_driver_24xrgb_pixelmap_draw   PORTABLE C       */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32xrgb format screen driver pixelmap drawing function that handles  */
/*    compressed or uncompress, with alpha channel.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_dave2d_display_driver_24xrgb_pixelmap_compressed_alpha_write   */
/*     _gx_display_driver_32bpp_4444argb_dave2d_pixelmap_compressed       */
/*     _alpha_write                                                       */
/*     _gx_display_driver_32bpp_4444bgra_dave2d_pixelmap_compressed       */
/*     _alpha_write                                                       */
/*     _gx_display_driver_24xrgb_pixelmap_draw                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                        INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_RECTANGLE  rotated_clip;
GX_PIXELMAP   temp_map = *pixelmap;

    GX_SWAP_VALS(xpos, ypos);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1);
        ypos = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width);
    }
    else
    {
        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
        xpos = (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height);
    }

    context -> gx_draw_context_clip = &rotated_clip;
    GX_SWAP_VALS(temp_map.gx_pixelmap_height, temp_map.gx_pixelmap_width);
    _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_draw(context, xpos, ypos, &temp_map);
    context -> gx_draw_context_clip = clip;
}

