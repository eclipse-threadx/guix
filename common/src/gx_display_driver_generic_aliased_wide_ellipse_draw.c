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
#include "gx_system.h"

#if defined(GX_ARC_DRAWING_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_aliased_wide_ellipse_draw                */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver function to draw anti-aliased ellipse with wide      */
/*    outline.                                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of ellipse  */
/*    ycenter                               y-coord of center of ellipse  */
/*    a                                     Length of the Semi-major Axis */
/*    b                                     Length of the Semi-minor Axis */
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
/*    _gx_utility_math_sqrt                 Compute the square root value */
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
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_aliased_wide_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b)
{
GX_DISPLAY   *display;
GX_RECTANGLE *clip;
GX_BRUSH     *brush;
INT           x;
INT           x2;
INT           y;
INT           y2;
GX_POINT      point;
INT           sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT           index;
INT           aa;
INT           bb;
INT          *pLineEnds;
INT           ymin;
INT           ymax;
INT           height;
INT           error;
INT           loop;
INT           brush_width;
GX_UBYTE      alpha1;
GX_UBYTE      alpha2;

VOID          (*blend_func)(GX_DRAW_CONTEXT *context,
                            INT x, INT y, GX_COLOR color, GX_UBYTE error);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE old_alpha;

    old_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
#endif
    display = context -> gx_draw_context_display;
    blend_func = display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;
    brush_width = brush -> gx_brush_width;

    if ((a <= ((brush_width - 1) >> 1)) ||
        (b <= ((brush_width - 1) >> 1)))
    {
        return;
    }

    ymin = ycenter - b - (brush_width >> 1);
    ymax = ycenter + b + (brush_width >> 1);

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

    for (y = 0; y <= height * 2; y++)
    {
        pLineEnds[y] = 0;
    }

    /* loop = 0: draw inner ellipse, and save point array.
       loop = 1: draw outer ellipse, and save point array. */
    for (loop = 0; loop < 2; loop++)
    {
        if (loop == 0)
        {
            a -= ((brush_width - 1) >> 1);
            b -= ((brush_width - 1) >> 1);
        }
        else
        {
            a += brush_width - 1;
            b += brush_width - 1;
        }

        aa = a * a;
        bb = b * b;
        x = 0;
        y = b;
        error = 0;

        /* Region I of the first quarter of the ellipse.  */
        while (2 * bb * (x + 1) < aa * (2 * y - 1))
        {
            alpha1 = (GX_UBYTE)(255 - error);
            alpha2 = (GX_UBYTE)error;

            /* error is the distance between mathmatic point to
               drawing point, which is used to do pixel blending.  */
            y2 = bb - bb * (x + 1) * (x + 1) / aa;
            error = (y << 8) - (INT)(_gx_utility_math_sqrt((UINT)(y2 << 10)) << 3);

            if (error >= 510)
            {
                /* The slope in point(x + 1, y) is greater than -1,
                   make point(x, y) the delimit pixel, break here. */
                y2 = bb - bb * x * x / aa;
                error = (y << 8) - (INT)(_gx_utility_math_sqrt((UINT)(y2 << 10)) << 3);
                break;
            }

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
                    /* Draw point(x, y - 1).  */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
                }
            }


            if (error >= 255)
            {
                error -= 255;
                y--;

                for (index = 0; index < 2; index++)
                {
                    y2 = y * sign[index][0] + ycenter;

                    if ((y2 >= ymin) && (y2 <= ymax))
                    {
                        if (loop == 0)
                        {
                            /* Inner circle points.  */
                            pLineEnds[(y2 - ymin) << 1] = x + 1;
                        }
                        else
                        {
                            /* Outer circle points.  */
                            pLineEnds[((y2 - ymin) << 1) + 1] = x;
                        }
                    }
                }
            }

            x++;
        }

        /* Draw delimit pixel where der*/
        for (index = 0; index < 4; index++)
        {
            point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

            if (_gx_utility_rectangle_point_detect(clip, point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, (GX_UBYTE)(255 - error));
            }
        }

        /* Region II of the first quarter of the ellipse.  */
        while (y > 0)
        {
            y--;

            y2 = aa - aa * y * y / bb;
            error = (INT)(_gx_utility_math_sqrt((UINT)(y2 << 10)) << 3) - (x << 8);

            while (error >= 255)
            {
                error -= 255;
                x++;
            }

            alpha1 = (GX_UBYTE)(255 - error);
            alpha2 = (GX_UBYTE)error;

            for (index = 0; index < 2; index++)
            {
                y2 = y * sign[index][0] + ycenter;

                if ((y2 >= ymin) && (y2 <= ymax))
                {
                    if (loop == 0)
                    {
                        /* Inner circle points.  */
                        pLineEnds[(y2 - ymin) << 1] = x + 1;
                    }
                    else
                    {
                        /* Outer circle points.  */
                        pLineEnds[((y2 - ymin) << 1) + 1] = x;
                    }
                }
            }

            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(x, y). */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
                }

                point.gx_point_x = (GX_VALUE)((x + 1) * sign[index][0] + xcenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(x + 1, y). */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha2);
                }
            }
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
    context -> gx_draw_context_brush.gx_brush_alpha = old_alpha;
#endif
}
#endif

