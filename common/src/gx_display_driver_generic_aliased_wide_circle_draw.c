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
/*    _gx_display_driver_generic_aliased_wide_circle_draw PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver function to draw anti-aliased circle with            */
/*    specified outline width.                                            */
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
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          Basic display driver          */
/*                                            horizontal line draw routine*/
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
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            fixed compile warnings,     */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ARC_DRAWING_SUPPORT)
VOID _gx_display_driver_generic_aliased_wide_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r)
{
/* The circle draw function is implemented from midpoint circle algorithm. */

GX_DISPLAY   *display;
GX_RECTANGLE *clip;
GX_BRUSH     *brush;
INT           brush_width;
INT           x = 0;
INT           x2;
INT           y;
INT           y2;
GX_POINT      point;
INT           sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT           index;
INT           error = 0;
INT           loop;
INT          *pLineEnds;
INT           ymin;
INT           ymax;
INT           height;
GX_UBYTE      alpha1;
GX_UBYTE      alpha2;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context,
                            INT x, INT y, GX_COLOR color, GX_UBYTE error);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE temp_alpha;
    temp_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
#endif

    display = context -> gx_draw_context_display;
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;
    brush_width = brush -> gx_brush_width;

    if (r <= (UINT)((brush_width - 1) >> 1))
    {
        return;
    }

    blend_func = display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    ymin = ycenter - (INT)r - (brush_width >> 1);
    ymax = ycenter + (INT)r + (brush_width >> 1);

    if (ymin < clip -> gx_rectangle_top)
    {
        ymin = clip -> gx_rectangle_top;
    }

    if (ymax > clip -> gx_rectangle_bottom)
    {
        ymax = clip -> gx_rectangle_bottom;
    }

    height = ymax - ymin + 1;
    pLineEnds = _gx_system_scratchpad;

    for (y = 0; y <= height * 2; y += 2)
    {
        pLineEnds[y] = 0;
        pLineEnds[y + 1] = 0;
    }

    /* loop = 0: draw inner circle, and save the point array.
       loop = 1: draw outer circle, and save the point array.  */
    for (loop = 0; loop < 2; loop++)
    {
        if (loop == 0)
        {
            /* Calculate the inner circle radius.  */
            r = (UINT)(r - (UINT)((brush_width - 1) >> 1));
        }
        else
        {
            /* Calculate the outer circle radious.  */
            r += (UINT)(brush_width - 1);
        }

        x = 0;
        y = (INT)r;
        error = 0;

        while (x < y)
        {

            for (index = 0; index < 2; index++)
            {
                x2 = y;
                y2 = x * sign[index][0] + ycenter;

                if ((y2 >= ymin) && (y2 <= ymax))
                {
                    pLineEnds[((y2 - ymin) << 1) + loop] = x2 - loop;
                }
            }

            alpha1 = (GX_UBYTE)(255 - error);
            alpha2 = (GX_UBYTE)error;

            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(x, y).  */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
                }

                point.gx_point_y = (GX_VALUE)((y - 1) * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(x, y - 1). */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, (GX_UBYTE)alpha2);
                }

                point.gx_point_x = (GX_VALUE)(y * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(x * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point (y, x). */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
                }

                point.gx_point_x = (GX_VALUE)((y - 1) * sign[index][0] + xcenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point (y - 1, x. )*/
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
                }
            }

            x++;

            y2 = (INT)(r * r) - x * x;

            /* error is the distance from mathmatical point to
               drawing point, which is used to determin alpha value.  */
            error = (y << 8) - (INT)(_gx_utility_math_sqrt((UINT)(y2 << 10)) << 3);

            while (error >= 255)
            {
                error -= 255;
                y--;

                for (index = 0; index < 2; index++)
                {
                    x2 = x;
                    y2 = y * sign[index][0] + ycenter;

                    if ((y2 >= ymin) && (y2 <= ymax))
                    {
                        pLineEnds[((y2 - ymin) << 1) + loop] = x2 - loop;
                    }
                }
            }
        }
    }

    for (index = 0; index < 4; index++)
    {
        point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
        point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

        if (_gx_utility_rectangle_point_detect(clip, point))
        {
            blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, (GX_UBYTE)(255 - error));
        }
    }

    index = 0;

    /* Filling outlines with horizontal line. */
    for (y = ymin; y <= ymax; y++)
    {
        for (loop = 0; loop < 2; loop++)
        {
            x = pLineEnds[index] * sign[loop][0] + xcenter;
            x2 = pLineEnds[index + 1] * sign[loop][0] + xcenter;

            if (x > x2)
            {
                GX_SWAP_VALS(x, x2);
            }

            if (x < clip -> gx_rectangle_left)
            {
                x = clip -> gx_rectangle_left;
            }

            if (x2 > clip -> gx_rectangle_right)
            {
                x2 = clip -> gx_rectangle_right;
            }

            display -> gx_display_driver_horizontal_line_draw(context, x, x2, y, 1,
                                                              brush -> gx_brush_line_color);
        }
        index += 2;
    }

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    context -> gx_draw_context_brush.gx_brush_alpha = temp_alpha;
#endif
}
#endif

