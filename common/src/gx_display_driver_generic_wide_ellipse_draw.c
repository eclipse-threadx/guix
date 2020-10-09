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
/*    _gx_display_driver_generic_wide_ellipse_draw        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver function to draw ellipse with wide outline.          */
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
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          The display driver horizontal */
/*                                            line drawing function       */
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
VOID _gx_display_driver_generic_wide_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b)
{
/* The ellipse draw function is implemented from midpoint ellipse algorithm. */

GX_DISPLAY   *display;
GX_RECTANGLE *clip;
GX_BRUSH     *brush;
INT           brush_width;
INT           x;
INT           x2;
INT           y;
INT           y2;
INT           d;
INT           sign[2] = {1, -1};
INT           index;
INT           aa;
INT           bb;
INT          *pLineEnds;
INT           ymin;
INT           ymax;
INT           height;
INT           loop;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE old_alpha;
    old_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
#endif
    display = context -> gx_draw_context_display;
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;
    brush_width = brush -> gx_brush_width;

    if ((a <= ((brush_width - 1) >> 1)) ||
        (b <= ((brush_width - 1) >> 1)))
    {
        return;
    }

    pLineEnds = _gx_system_scratchpad;

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

    for (y = 0; y <= height * 2; y += 2)
    {
        pLineEnds[y] = 2000;
        pLineEnds[y + 1] = 0;
    }

    y2 = ycenter - b - (brush_width >> 1);
    for (y = y2; y < y2 + brush_width; y++)
    {
        if ((y >= ymin) && (y <= ymax))
        {
            pLineEnds[(y - ymin) << 1] = 0;
        }
    }

    y2 = ycenter + b - ((brush_width - 1) >> 1);
    for (y = y2; y < y2 + brush_width; y++)
    {
        if ((y >= ymin) && (y <= ymax))
        {
            pLineEnds[(y - ymin) << 1] = 0;
        }
    }

    a -= ((brush_width - 1) >> 1);
    b -= ((brush_width - 1) >> 1);

    aa = a * a;
    bb = b * b;
    x = 0;
    y = b;
    d = (bb << 2) + aa * (1 - (b << 1));

    /* Region I of the first quarter of the inner ellipse.  */
    while ((bb << 1) * (x + 1) < aa * (2 * y - 1))
    {
        for (loop = 0; loop < 2; loop++)
        {
            y2 = y * sign[loop] + ycenter;

            if ((y2 >= ymin) && (y2 <= ymax))
            {
                index = (y2 - ymin) << 1;

                if (x < pLineEnds[index])
                {
                    pLineEnds[index] = x;
                }
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

    /* Region II of the first quarter of the inner ellipse.  */
    while (y >= 0)
    {
        for (loop = 0; loop < 2; loop++)
        {
            y2 = y * sign[loop] + ycenter;

            if ((y2 >= ymin) && (y2 <= ymax))
            {
                index = (y2 - ymin) << 1;

                if (x < pLineEnds[index])
                {
                    pLineEnds[index] = x;
                }
            }
        }

        if (d < 0)
        {
            d += (bb << 1) * (x + 1) + aa * (3 - (y << 1));
            x++;
        }
        else
        {
            d += aa * (3 - (y << 1));
        }
        y--;
    }

    a += brush_width - 1;
    b += brush_width - 1;
    aa = a * a;
    bb = b * b;
    x = 0;
    y = b;
    d = (bb << 2) + (aa << 2) * (1 - b);

    /* Region I of the first quarter of the outer ellipse.  */
    while ((bb << 1) * (x + 1) < aa * (2 * y - 1))
    {
        for (loop = 0; loop < 2; loop++)
        {
            y2 = y * sign[loop] + ycenter;

            if ((y2 >= ymin) && (y2 <= ymax))
            {
                index = (y2 - ymin) << 1;

                if (x > pLineEnds[index + 1])
                {
                    pLineEnds[index + 1] = x;
                }
            }
        }

        if (d < 0)
        {
            d += bb * ((x << 3) + 12);
        }
        else
        {
            d += bb * ((x << 3) + 12) + (aa << 3) * (1 - y);
            y--;
        }
        x++;
    }

    d = (b << 1) * ((x << 1) + 1) + (a << 1) * (y - 1) - (a << 1) * b;

    /* Region II of the first quarter of the outer ellipse.  */
    while (y >= 0)
    {
        for (loop = 0; loop < 2; loop++)
        {
            y2 = y * sign[loop] + ycenter;

            if ((y2 >= ymin) && (y2 <= ymax))
            {
                index = (y2 - ymin) << 1;

                if (x > pLineEnds[index + 1])
                {
                    pLineEnds[index + 1] = x;
                }
            }
        }

        if (d < 0)
        {
            x++;
            d += (bb << 1) * (x + 1) - aa * (1 + (y << 1));
        }
        else
        {
            d += aa * (-1 - (y << 1));
        }
        y--;
    }

    /* Filling outlines with horizontal line. */
    index = 0;
    for (y = ymin; y <= ymax; y++)
    {
        for (loop = 0; loop < 2; loop++)
        {
            x = pLineEnds[index] * sign[loop] + xcenter;
            x2 = pLineEnds[index + 1] * sign[loop] + xcenter;

            if (loop)
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

