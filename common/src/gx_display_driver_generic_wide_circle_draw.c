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

#if defined(GX_ARC_DRAWING_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_wide_circle_draw         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw circle with specified outline width.         */
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
VOID _gx_display_driver_generic_wide_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r)
{
/* The circle draw function is implemented from midpoint circle algorithm. */

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
INT          *pLineEnds;
INT           ymin;
INT           ymax;
INT           height;
INT           loop;
INT           pos;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE old_alpha;
    old_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
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
        pLineEnds[y] = 2000;
        pLineEnds[y + 1] = 0;
    }

    y2 = ycenter - (INT)r - (brush_width >> 1);
    for (y = y2; y < y2 + brush_width; y++)
    {
        if ((y >= ymin) && (y <= ymax))
        {
            pLineEnds[(y - ymin) << 1] = 0;
        }
    }

    y2 = ycenter + (INT)r - (INT)((brush_width - 1) >> 1);
    for (y = y2; y < y2 + brush_width; y++)
    {
        if ((y >= ymin) && (y <= ymax))
        {
            pLineEnds[(y - ymin) << 1] = 0;
        }
    }

    for (loop = 0; loop < 2; loop++)
    {
        if (loop == 0)
        {
            /* inner circle. */
            r = (UINT)(r - (UINT)((brush_width - 1) >> 1));
        }
        else
        {
            /* outer circle. */
            r += (UINT)(brush_width - 1);
        }

        x = 0;
        y = (INT)r;
        d = 5 - (INT)(4 * r);

        /* Record points in the fourth quarter of the inner circle.  */
        while (x <= y)
        {
            for (index = 0; index < 2; index++)
            {
                x2 = y;
                y2 = x * sign[index] + ycenter;

                if ((y2 >= ymin) && (y2 <= ymax))
                {
                    pos = (y2 - ymin) << 1;

                    if (x2 < pLineEnds[pos])
                    {
                        pLineEnds[pos] = x2;
                    }

                    if (x2 > pLineEnds[pos + 1])
                    {
                        pLineEnds[pos + 1] = x2;
                    }
                }

                x2 = x;
                y2 = y * sign[index] + ycenter;

                if ((y2 >= ymin) && (y2 <= ymax))
                {
                    pos = (y2 - ymin) << 1;

                    if (x2 < pLineEnds[pos])
                    {
                        pLineEnds[pos] = x2;
                    }

                    if (x2 > pLineEnds[pos + 1])
                    {
                        pLineEnds[pos + 1] = x2;
                    }
                }
            }

            if (d < 0)
            {
                d += 8 * x + 12;
            }
            else
            {
                d += 8 * (x - y) + 20;
                y--;
            }
            x++;
        }
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

