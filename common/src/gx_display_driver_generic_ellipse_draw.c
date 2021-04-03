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
/*    _gx_display_driver_generic_ellipse_draw             PORTABLE C      */
/*                                                           6.1.6        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw ellipse.                                     */
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
/*    [gx_display_driver_pixel_write]       Basic display driver pixel    */
/*                                            write function              */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
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
/*  04-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            improved precision,         */
/*                                            resulting in version 6.1.6  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b)
{
/* The ellipse draw function is implemented from Bresenham ellipse algorithm. */
INT           x;
INT           y;
INT           d;
GX_POINT      point;
INT           sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT           index;
INT           aa;
INT           bb;
GX_DISPLAY   *display = context -> gx_draw_context_display;
GX_BRUSH     *brush = &context -> gx_draw_context_brush;
GX_RECTANGLE *clip = context -> gx_draw_context_clip;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE brush_alpha = brush -> gx_brush_alpha;

    if (display -> gx_display_driver_pixel_blend == GX_NULL)
    {
        /* Pixel blend function is null means alpha isn't supported in this driver.
           So set alpha value to 0xff to make it draw the original color in case GX_BRUSH_ALPHA_SUPPORT is defined. */
        brush_alpha = 0xff;
    }
    else
    {
        if (brush_alpha == 0)
        {
            /* Nothing to draw here. */
            return;
        }
    }
#endif

    /* The ellipse is split into 2 regions, region I with dx > dy and resion II with dx <= dy.
       In region I, the midpoint between (x + 1, y) and (x + 1, y - 1) is used to select next point that is closer to the ellipse,
       d(x + 1, y - 0.5) is the distance from the midpoint to the ellipse center,
       if the decision < 0, the midpoint is inside the ellipse, point (x + 1, y - 1) is closer to the ellipse, and be selected for drawing.
       otherwise, (x + 1, y) is selected.
       In region II, the midpoint between (x, y - 1) and (x + 1, y - 1) is used to select next point that is closer to the ellipse,
       d(x + 0.5, y - 1) is the distance from the midpoint to ellipse center,
       if the decision < 0, the midpoint is inside the ellipse, point(x + 1, y - 1) is closer to the ellipse, and be selected for drawing,
       otherwise, (x, y - 1) is selected.

       Ellipse equation is f(x, y) = sqr(b * x) + sqr(a * y) - sqr(a * b).
       First, we assume ellipse is centered at the origin(0, 0), and the first point is (0, b).
       Set initial decision value for region I as d = f(1, b - 0.5) = sqr(b) + sqr(a) * (-b + 0.25).
    */

    aa = a * a;
    bb = b * b;
    x = 0;
    y = b;

    /* Decision is enlarged by 4 to avoid floating point. */
    d = (bb << 2) + aa * (1 - (b << 2));

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    if (brush_alpha != 0xff)
    {

        /* Region I of the first quarter of the ellipse.  */
        while ((bb << 1) * (x + 1) < aa * (2 * y - 1))
        {
            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_blend(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, brush_alpha);
                }
            }

            if (d < 0)
            {
                d += (bb << 2) * ((x << 1) + 3);
            }
            else
            {
                d += (bb << 2) * ((x << 1) + 3) + (aa << 3) * (1 - y);
                y--;
            }
            x++;
        }

        d = bb * ((x << 1) + 1) * ((x << 1) + 1) + (aa << 2) * (y - 1) * (y - 1) - (aa << 2) * bb;

        /* Region II of the first quarter of the ellipse.  */
        while (y >= 0)
        {
            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_blend(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, brush_alpha);
                }
            }

            if (d < 0)
            {
                d += (bb << 3) * (x + 1) + (aa << 2) * (3 - (y << 1));
                x++;
            }
            else
            {
                d += (aa << 2) * (3 - (y << 1));
            }

            y--;
        }
    }
    else
    {
#endif
        /* Region I of the first quarter of the ellipse.  */
        while ((bb << 1) * (x + 1) < aa * (2 * y - 1))
        {
            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_write(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color);
                }
            }

            if (d < 0)
            {
                d += (bb << 2) * ((x << 1) + 3);
            }
            else
            {
                d += (bb << 2) * ((x << 1) + 3) + (aa << 3) * (1 - y);
                y--;
            }
            x++;
        }

        /* Set initial decision value for region II as d = f(x + 0.5, y - 1) = sqr(b * (x + 0.5) + sqr(a * (y - 1)) - sqr(a * b).
           Enlarge the decision by 4 to avoid float calculation. */

        d = bb * ((x << 1) + 1) * ((x << 1) + 1) + (aa << 2) * (y - 1) * (y - 1) - (aa << 2) * bb;

        /* Region II of the first quarter of the ellipse.  */
        while (y >= 0)
        {
            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_write(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color);
                }
            }

            if (d < 0)
            {
                d += (bb << 3) * (x + 1) + (aa << 2) * (3 - (y << 1));
                x++;
            }
            else
            {
                d += (aa << 2) * (3 - (y << 1));
            }

            y--;
        }
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    }
#endif
}

