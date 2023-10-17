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
#include "gx_display.h"
#include "gx_context.h"
#include "gx_dave2d_simulation_display_driver.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_32bpp_rotated_horizontal_      */
/*                                                     pixelmap_line_draw */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.2.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32bpp screen driver horizontal pixelmap line drawing function that  */
/*    handles compressed or uncompress, with or without alpha channel.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_    */
/*                                                              line_draw */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2022     Kenneth Maxwell          Initial Version 6.2.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_32bpp_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                                                       INT ystart, INT yend, INT x, GX_FILL_PIXELMAP_INFO *info)
{
INT          xstart;
INT          xend;
INT          y;
GX_PIXELMAP *old_map = info -> pixelmap;
GX_PIXELMAP  map = *old_map;

    info -> pixelmap = &map;
    GX_SWAP_VALS(map.gx_pixelmap_height, map.gx_pixelmap_width);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        xstart = ystart;
        xend = yend;
        y = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - x - 1;
    }
    else
    {
        xstart = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - yend - 1;
        xend = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - ystart - 1;
        y = x;
        info -> x_offset = (yend - ystart + 1 + info -> x_offset) % old_map -> gx_pixelmap_height;

        if (info -> x_offset)
        {
            info -> x_offset = old_map -> gx_pixelmap_height - info -> x_offset;
        }
    }

    _gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_line_draw(context, xstart, xend, y, info);

    info -> pixelmap = old_map;
}

