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
#include "gx_utility.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_aliased_wide_line_draw   PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*   Generic display driver function for aliased wide line.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of endpoint           */
/*    ystart                                y-coord of endpoint           */
/*    xend                                  x-coord of endpoint           */
/*    yend                                  y-coord of endpoint           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_simple_wide_line_draw]                           */
/*                                          Basic display driver wide     */
/*                                            line draw function          */
/*    _gx_display_driver_generic_wide_line_points_calculate               */
/*                                          Calculate corners of wide line*/
/*    _gx_display_driver_generic_aliased_filled_circle_draw               */
/*                                          Basic display driver aliased  */
/*                                            circle fill function        */
/*    _gx_display_driver_generic_wide_line_fill                           */
/*                                          Basic display driver wide line*/
/*                                            draw function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            supported display rotation, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_aliased_wide_line_draw(GX_DRAW_CONTEXT *context, INT xstart,
                                                       INT ystart, INT xend, INT yend)
{
GX_DISPLAY     *display = context -> gx_draw_context_display;
INT             brush_width = context -> gx_draw_context_brush.gx_brush_width;
GX_FIXED_POINT *line_points;
GX_FIXED_VAL    sxcenter;
GX_FIXED_VAL    sycenter;
GX_FIXED_VAL    excenter;
GX_FIXED_VAL    eycenter;
GX_RECTANGLE    clip_rect;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE old_alpha;
    old_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
#endif
    /* calculate the corners of this line, save them
       to our points array
     */
    if (!(context -> gx_draw_context_display -> gx_display_driver_pixel_blend))
    {
        display -> gx_display_driver_simple_wide_line_draw(context, xstart, ystart, xend, yend);
        return;
    }

    if ((context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_ROUND) &&
        (brush_width > 2))
    {
        sxcenter = GX_FIXED_VAL_MAKE(xstart);
        sycenter = GX_FIXED_VAL_MAKE(ystart);
        excenter = GX_FIXED_VAL_MAKE(xend);
        eycenter = GX_FIXED_VAL_MAKE(yend);

        if (!(brush_width & 0x01))
        {
            if (ystart == yend)
            {
                /* Horizontal line. */
                sycenter -= GX_FIXED_VAL_HALF;
                eycenter -= GX_FIXED_VAL_HALF;
            }
            else if (xstart == xend)
            {
                /* Vertical line. */
                sxcenter -= GX_FIXED_VAL_HALF;
                excenter -= GX_FIXED_VAL_HALF;
            }
        }

        _gx_display_driver_generic_aliased_filled_circle_draw(context, sxcenter, sycenter,
                                                              GX_FIXED_VAL_MAKE(brush_width) >> 1);

        _gx_display_driver_generic_aliased_filled_circle_draw(context, excenter, eycenter,
                                                              GX_FIXED_VAL_MAKE(brush_width) >> 1);
    }

    if (ystart == yend)
    {
        /* Horizontal line. */

        if (xstart > xend)
        {
            GX_SWAP_VALS(xstart, xend);
        }

        clip_rect.gx_rectangle_left = (GX_VALUE)xstart;
        clip_rect.gx_rectangle_right = (GX_VALUE)xend;
        clip_rect.gx_rectangle_top = (GX_VALUE)(ystart - (brush_width >> 1));
        clip_rect.gx_rectangle_bottom = (GX_VALUE)(clip_rect.gx_rectangle_top + brush_width - 1);

        if (_gx_utility_rectangle_overlap_detect(&clip_rect, context -> gx_draw_context_clip, &clip_rect))
        {
            display -> gx_display_driver_horizontal_line_draw(context,
                                                              clip_rect.gx_rectangle_left,
                                                              clip_rect.gx_rectangle_right,
                                                              clip_rect.gx_rectangle_top,
                                                              clip_rect.gx_rectangle_bottom - clip_rect.gx_rectangle_top + 1,
                                                              context -> gx_draw_context_brush.gx_brush_line_color);
        }
    }
    else if (xstart == xend)
    {
        /* Vertical line. */

        if (ystart > yend)
        {
            GX_SWAP_VALS(ystart, yend);
        }

        clip_rect.gx_rectangle_left = (GX_VALUE)(xstart - (brush_width >> 1));
        clip_rect.gx_rectangle_right = (GX_VALUE)(clip_rect.gx_rectangle_left + brush_width - 1);
        clip_rect.gx_rectangle_top = (GX_VALUE)ystart;
        clip_rect.gx_rectangle_bottom = (GX_VALUE)yend;

        if (_gx_utility_rectangle_overlap_detect(&clip_rect, context -> gx_draw_context_clip, &clip_rect))
        {
            display -> gx_display_driver_vertical_line_draw(context,
                                                            clip_rect.gx_rectangle_top,
                                                            clip_rect.gx_rectangle_bottom,
                                                            clip_rect.gx_rectangle_left,
                                                            clip_rect.gx_rectangle_right - clip_rect.gx_rectangle_left + 1,
                                                            context -> gx_draw_context_brush.gx_brush_line_color);
        }
    }
    else
    {
        line_points = _gx_display_driver_generic_wide_line_points_calculate(context, xstart, ystart,
                                                                            xend, yend, brush_width, GX_TRUE);

        if (display -> gx_display_rotation_angle)
        {
            _gx_display_driver_generic_rotated_wide_line_fill(context, line_points);
        }
        else
        {
            _gx_display_driver_generic_wide_line_fill(context, line_points);
        }
    }

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    context -> gx_draw_context_brush.gx_brush_alpha = old_alpha;
#endif
}

