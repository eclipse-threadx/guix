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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_rotated_wide_line_fill   PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*   Fill a wide line after the four corner points have been calculated.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    pPoints                               Pre-computed end points       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GX_ABS                                Compute the absolute value    */
/*    GX_SWAP_VALUE                         Swap two values               */
/*    [_gx_display_driver_horizontal_line_draw]                           */
/*                                          Driver for horizontal line    */
/*                                            draw                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_simple_wide_line_draw                    */
/*    _gx_display_driver_generic_aliased_wide_line_draw                   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_rotated_wide_line_fill(GX_DRAW_CONTEXT *context, GX_FIXED_POINT *pPoints)
{
/*
   Use Breshenham's line to compute the points along each line that bounds
   the wide line and save the points to an array. Then draw horizontal lines
   to connect the points.
 */

GX_FIXED_POINT *pGet = pPoints;
INT            *pLineEnds;
INT             xmin;
INT             xmax;
INT             xstart;
INT             xend;
INT             ystart;
INT             yend;
INT             curx;
INT             cury;
INT             test;
GX_FIXED_VAL    dx;
GX_FIXED_VAL    dy;
INT             Index;
INT             loop;
INT             width;
INT             xsign;
INT             ysign;
INT             decision;
INT             shift;
VOID            (*line_draw)(GX_DRAW_CONTEXT *context, INT x1, INT x2, INT ypos, INT width, GX_COLOR color);
GX_RECTANGLE   *clip = context -> gx_draw_context_clip;
GX_COLOR        linecolor = context -> gx_draw_context_brush.gx_brush_line_color;
GX_BOOL         aliased = GX_FALSE;
INT             x_fraction;
INT             y_fraction;

    if ((context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_ALIAS) &&
        context -> gx_draw_context_display -> gx_display_driver_pixel_blend)
    {
        aliased = GX_TRUE;
    }

    xmin = GX_FIXED_VAL_TO_INT(pPoints[0].x);
    xmax = GX_FIXED_VAL_RND_UP(pPoints[2].x);

    if (xmin < clip -> gx_rectangle_left)
    {
        xmin = clip -> gx_rectangle_left;
    }

    if (xmax > clip -> gx_rectangle_right)
    {
        xmax = clip -> gx_rectangle_right;
    }

    width = xmax - xmin + 1;

    pLineEnds = _gx_system_scratchpad;

    /* default the point array to being off the screen on both sides: */

    for (loop = 0; loop < width * 2; loop += 2)
    {
        pLineEnds[loop] = 2000;
        pLineEnds[loop + 1] = 0;
    }

    /* Fill in the point array by using Breshenhams line for
       all 4 sides of the bounding rectangle
     */

    for (loop = 0; loop < 4; loop++)
    {
        xstart = pGet -> x;
        ystart = pGet -> y;
        pGet++;
        xend = pGet -> x;
        yend = pGet -> y;
        dx = GX_ABS(xend - xstart);
        dy = GX_ABS(yend - ystart);

        xsign = ysign = 1;

        if (((dx >= dy && (xstart > xend)) ||
             ((dy > dx) && ystart > yend)))
        {
            GX_SWAP_VALS(xend, xstart);
            GX_SWAP_VALS(yend, ystart);
        }
        if (dx)
        {
            xsign = (xend - xstart) / dx;
        }
        if (dy)
        {
            ysign = (yend - ystart) / dy;
        }

        x_fraction = (xstart & GX_FIXED_VAL_FRACTION_MASK);
        y_fraction = (ystart & GX_FIXED_VAL_FRACTION_MASK);

        shift = 0;

        if (dx >= dy)
        {
            if (ysign < 0)
            {
                cury = GX_FIXED_VAL_RND_UP(ystart);

                if (y_fraction)
                {
                    y_fraction = GX_FIXED_VAL_ONE - y_fraction;
                }
            }
            else
            {
                cury = GX_FIXED_VAL_TO_INT(ystart);
            }

            decision = (y_fraction * dx - x_fraction * dy) / GX_FIXED_VAL_ONE;

            if (decision < 0)
            {
                decision += dx;
                cury -= ysign;
            }

            xstart = GX_FIXED_VAL_TO_INT(xstart);

            if (aliased)
            {
                xend = GX_FIXED_VAL_TO_INT(xend);

                if (x_fraction)
                {
                    xstart++;
                    decision += dy;
                }

                if (dy)
                {
                    if ((loop == 1) || (loop == 2))
                    {
                        shift = ysign;
                    }
                }
            }
            else
            {
                xend = GX_FIXED_VAL_RND(xend);

                if (x_fraction >= GX_FIXED_VAL_HALF)
                {
                    xstart++;
                    decision += dy;
                }

                decision += (dx >> 1);
                if (decision >= dx)
                {
                    decision -= dx;
                    cury += ysign;
                }
            }

            for (curx = xstart; curx <= xend; curx++, decision += dy)
            {
                if (decision >= dx)
                {
                    decision -= dx;
                    cury += ysign;
                }

                if ((curx >= xmin) && (curx <= xmax))
                {
                    Index = (curx - xmin) << 1;

                    test = cury + shift;
                    if (test < pLineEnds[Index])
                    {
                        pLineEnds[Index] = test;
                    }

                    if (test > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = test;
                    }
                }
            }
        }
        else
        {
            if (xsign < 0)
            {
                curx = GX_FIXED_VAL_RND_UP(xstart);

                if (x_fraction)
                {
                    x_fraction = GX_FIXED_VAL_FRACTION_MASK - x_fraction;
                }
            }
            else
            {
                curx = GX_FIXED_VAL_TO_INT(xstart);
            }

            decision = (x_fraction * dy - y_fraction * dx) / GX_FIXED_VAL_ONE;

            if (decision < 0)
            {
                decision += dy;
                curx -= xsign;
            }

            ystart = GX_FIXED_VAL_TO_INT(ystart);

            if (aliased)
            {
                yend = GX_FIXED_VAL_TO_INT(yend);

                if (y_fraction)
                {
                    ystart++;
                    decision += dx;
                }

                if (dx)
                {
                    if (loop == 2 || loop == 3)
                    {
                        shift = xsign;
                    }
                }
            }
            else
            {
                yend = GX_FIXED_VAL_RND(yend);

                if (y_fraction >= GX_FIXED_VAL_HALF)
                {
                    ystart++;
                    decision += dx;
                }

                decision += (dy >> 1);

                if (decision >= dy)
                {
                    decision -= dy;
                    curx += xsign;
                }
            }

            for (cury = ystart; cury <= yend; cury++, decision += dx)
            {
                if (decision >= dy)
                {
                    decision -= dy;
                    curx += xsign;
                }

                test = curx + shift;

                if ((test >= xmin) && (test <= xmax))
                {
                    Index = (test - xmin) << 1;

                    if (cury < pLineEnds[Index])
                    {
                        pLineEnds[Index] = cury;
                    }

                    if (cury > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = cury;
                    }
                }
            }
        }
    }

    Index = 0;
    line_draw = context -> gx_draw_context_display -> gx_display_driver_vertical_line_draw;

    for (curx = xmin; curx <= xmax; curx++)
    {
        if (pLineEnds[Index] < clip -> gx_rectangle_top)
        {
            pLineEnds[Index] = clip -> gx_rectangle_top;
        }

        if (pLineEnds[Index + 1] > clip -> gx_rectangle_bottom)
        {
            pLineEnds[Index + 1] = clip -> gx_rectangle_bottom;
        }

        if (pLineEnds[Index] <= pLineEnds[Index + 1])
        {
            line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], curx, 1, linecolor);
        }
        Index += 2;
    }
}

