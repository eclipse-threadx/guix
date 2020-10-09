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
/*                                                           6.1          */
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

    aa = a * a;
    bb = b * b;
    x = 0;
    y = b;
    d = (bb << 2) + aa * (1 - (b << 1));

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
                d += (bb << 1) * ((x << 1) + 3);
            }
            else
            {
                d += (bb << 1) * ((x << 1) + 3) + (aa << 2) * (1 - y);
                y--;
            }
            x++;
        }

        d = bb * x * (x + 1) + aa * y * (y - 1) - aa * bb;

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

            y--;
            if (d < 0)
            {
                x++;
                d += (bb << 1) * (x + 1) - aa * (1 + (y << 1));
            }
            else
            {
                d += aa * (-1 - (y << 1));
            }
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
                d += (bb << 1) * ((x << 1) + 3);
            }
            else
            {
                d += (bb << 1) * ((x << 1) + 3) + (aa << 2) * (1 - y);
                y--;
            }
            x++;
        }

        d = bb * x * (x + 1) + aa * y * (y - 1) - aa * bb;

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

            y--;

            if (d < 0)
            {
                x++;
                d += (bb << 1) * (x + 1) - aa * (1 + (y << 1));
            }
            else
            {
                d += aa * (-1 - (y << 1));
            }
        }
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    }
#endif
}

