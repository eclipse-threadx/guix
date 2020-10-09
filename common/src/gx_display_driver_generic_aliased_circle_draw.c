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
/*    _gx_display_driver_generic_aliased_circle_draw      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw anti-aliased circle.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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

VOID _gx_display_driver_generic_aliased_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r)
{
/* The circle draw function is implemented from midpoint circle algorithm. */

GX_DISPLAY   *display;
GX_RECTANGLE *clip;
GX_BRUSH     *brush;
INT           x;
INT           y;
GX_POINT      point;
INT           sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT           index;
INT           error;
INT           yi;
GX_UBYTE      alpha1;
GX_UBYTE      alpha2;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context,
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
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;

    blend_func = display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    x = 0;
    y = (INT)r;
    error = 0;

    while (x < y)
    {
        alpha1 = (GX_UBYTE)(255 - error);
        alpha2 = (GX_UBYTE)error;
#if defined(GX_BRUSH_ALPHA_SUPPORT)
        alpha1 = (GX_UBYTE)(alpha1 * brush_alpha / 255);
        alpha2 = (GX_UBYTE)(alpha2 * brush_alpha / 255);
#endif

        for (index = 0; index < 4; index++)
        {
            point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(clip, point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
            }

            point.gx_point_y = (GX_VALUE)((y - 1) * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(clip, point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
            }

            point.gx_point_x = (GX_VALUE)(y * sign[index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(x * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(clip, point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
            }

            point.gx_point_x = (GX_VALUE)((y - 1) * sign[index][0] + xcenter);

            if (_gx_utility_rectangle_point_detect(clip, point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
            }
        }

        x++;

        yi = (INT)(r * r) - x * x;
        error = (y << 8) - (INT)(_gx_utility_math_sqrt((UINT)(yi << 10)) << 3);

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

        if (_gx_utility_rectangle_point_detect(clip, point))
        {
            blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
        }
    }
}

#endif

