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
/*    _gx_display_driver_generic_aliased_arc_draw         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw aliased circle arc.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    xcenter                               x-coord of center of circle   */
/*                                            arc                         */
/*    ycenter                               y-coord of center of circle   */
/*                                            arc                         */
/*    r                                     Radius of circle arc          */
/*    start_angle                           The start angle of circle arc */
/*    end_angle                             The end angle of circle arc   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_arc_clipping_get   Get an arc clipping.          */
/*    _gx_utility_rectangle_overlap_detect  Detect two rectangles being   */
/*                                            overlap to each other       */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
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
/*                                                                        */
/**************************************************************************/
#if defined(GX_ARC_DRAWING_SUPPORT)
VOID _gx_display_driver_generic_aliased_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
/* The arc draw function is implemented by clipping a circle.
   Calculate the arc clipping first, turn on the pixel on circle when the pixel
   is inside the arc clipping area.                                             */

GX_DISPLAY  *display;
GX_RECTANGLE arc_clip[4];
GX_BRUSH    *brush;
INT          x;
INT          y;
GX_POINT     point;
INT          sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT          index;
INT          error;
INT          realval;
GX_UBYTE     alpha1;
GX_UBYTE     alpha2;
VOID         (*blend_func)(GX_DRAW_CONTEXT *context,
                           INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE brush_alpha;

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        /* Nothing to draw here. */
        return;
    }
#endif

    display = context -> gx_draw_context_display;
    blend_func = display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    brush = &context -> gx_draw_context_brush;


    /* Get the clipping rectangles of the circle arc. */
    _gx_display_driver_arc_clipping_get(xcenter, ycenter, r, start_angle, end_angle,
                                        &arc_clip[0], &arc_clip[1], &arc_clip[2], &arc_clip[3]);

    _gx_utility_rectangle_overlap_detect(context -> gx_draw_context_clip, &arc_clip[0], &arc_clip[0]);
    _gx_utility_rectangle_overlap_detect(context -> gx_draw_context_clip, &arc_clip[1], &arc_clip[1]);
    _gx_utility_rectangle_overlap_detect(context -> gx_draw_context_clip, &arc_clip[2], &arc_clip[2]);
    _gx_utility_rectangle_overlap_detect(context -> gx_draw_context_clip, &arc_clip[3], &arc_clip[3]);

    x = 0;
    y = (INT)r;
    error = 0;

    /* Process anti-aliased circle drawing. */
    while (x < y)
    {
        for (index = 0; index < 4; index++)
        {
            alpha1 = (GX_UBYTE)(255 - error);
            alpha2 = (GX_UBYTE)error;
#if defined(GX_BRUSH_ALPHA_SUPPORT)
            alpha1 = (GX_UBYTE)(alpha1 * brush_alpha / 255);
            alpha2 = (GX_UBYTE)(alpha2 * brush_alpha / 255);
#endif

            point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[3], point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
            }

            point.gx_point_y = (GX_VALUE)((y - 1) * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[3], point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
            }

            point.gx_point_x = (GX_VALUE)(y * sign[index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(x * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[3], point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
            }

            point.gx_point_x = (GX_VALUE)((y - 1) * sign[index][0] + xcenter);

            if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
                _gx_utility_rectangle_point_detect(&arc_clip[3], point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
            }
        }

        x++;

        realval = (INT)(r * r) - x * x;
        error = (y << 8) - (INT)(_gx_utility_math_sqrt((UINT)(realval << 10)) << 3);

        while (error >= 255)
        {
            error -= 255;
            y--;
        }
    }

    alpha1 = (GX_UBYTE)(255 - error);
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    alpha1 = (GX_UBYTE)(alpha1 * brush_alpha / 255);
#endif

    for (index = 0; index < 4; index++)
    {

        point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
        point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

        if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
            _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
            _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
            _gx_utility_rectangle_point_detect(&arc_clip[3], point))
        {
            blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
        }
    }
}
#endif

