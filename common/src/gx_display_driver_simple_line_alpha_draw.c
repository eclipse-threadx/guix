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


#define PIXEL_WRITE(loc, val) (*(loc) = ((USHORT)val))

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_display.h"

#if defined (GX_BRUSH_ALPHA_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_simple_line_alpha_draw           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic driver function that handles drawing lines with brush       */
/*    alpha.                                                              */
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
/*    GX_ABS                                Compute the absolute value    */
/*    GX_SWAP_VALUE                         Swap two values               */
/*    [PIXEL_WRITE]                         Driver level pixel write      */
/*                                            routine                     */
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
VOID _gx_display_driver_simple_line_alpha_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend, GX_UBYTE alpha)
{
INT           curx;
INT           cury;
INT           x_sign;
INT           y_sign;
INT           decision;
INT           nextx;
INT           nexty;
GX_POINT      end_point;
GX_POINT      mid_point;
GX_RECTANGLE  half_rectangle;
GX_RECTANGLE  half_over;
INT           sign;
INT           steps;
GX_BOOL       clipped = GX_TRUE;
INT           dx = GX_ABS(xend - xstart);
INT           dy = GX_ABS(yend - ystart);

GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_COLOR      linecolor = context -> gx_draw_context_brush.gx_brush_line_color;
VOID        (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    if (((dx >= dy && (xstart > xend)) || ((dy > dx) && ystart > yend)))
    {
        GX_SWAP_VALS(xend, xstart);
        GX_SWAP_VALS(yend, ystart);
    }
    x_sign = (xend - xstart) / dx;
    y_sign = (yend - ystart) / dy;

    end_point.gx_point_x = (GX_VALUE)xstart;
    end_point.gx_point_y = (GX_VALUE)ystart;

    if (_gx_utility_rectangle_point_detect(clip, end_point))
    {
        end_point.gx_point_x = (GX_VALUE)xend;
        end_point.gx_point_y = (GX_VALUE)yend;

        if (_gx_utility_rectangle_point_detect(clip, end_point))
        {
            clipped = GX_FALSE;
        }
    }

    if (clipped)
    {
        /* here if we must do clipping in the inner loop, because one
        or both of the end points are outside clipping rectangle */

        /* Calculate the middle point of the line.  */
        mid_point.gx_point_x = (GX_VALUE)((xend + xstart) >> 1);
        mid_point.gx_point_y = (GX_VALUE)((yend + ystart) >> 1);

        /* Judge the clip in which side.  */
        if (_gx_utility_rectangle_point_detect(clip, mid_point))
        {

            /* the clip in two sides.  */
            if (dx >= dy)
            {
                /* walk out the clipping point.  */
                for (curx = xstart, cury = ystart, decision = (dx >> 1); curx < mid_point.gx_point_x;
                    curx++, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        cury += y_sign;
                    }

                    if (curx >= clip -> gx_rectangle_left &&
                        cury >= clip -> gx_rectangle_top &&
                        cury <= clip -> gx_rectangle_bottom)
                    {
                        break;
                    }
                }
                for (; curx <= mid_point.gx_point_x;
                    curx++, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        cury += y_sign;
                    }

                    blend_func(context, curx, cury, linecolor, alpha);
                }
                for (nextx = xend, nexty = yend, decision = (dx >> 1); nextx > mid_point.gx_point_x;
                    nextx--, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        nexty -= y_sign;
                    }
                    if (nextx <= clip -> gx_rectangle_right &&
                        nexty >= clip -> gx_rectangle_top &&
                        nexty <= clip -> gx_rectangle_bottom)
                    {
                        break;
                    }
                }

                for (; nextx > mid_point.gx_point_x;
                    nextx--, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        nexty -= y_sign;
                    }
                    blend_func(context, nextx, nexty, linecolor, alpha);
                }
            }
            else
            {
                for (nextx = xend, nexty = yend, decision = (dy >> 1); nexty > mid_point.gx_point_y;
                    nexty--, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        nextx -= x_sign;
                    }
                    if (nextx >= clip -> gx_rectangle_left &&
                        nextx <= clip -> gx_rectangle_right &&
                        nexty <= clip -> gx_rectangle_bottom)
                    {
                        break;
                    }
                }

                for (; nexty > mid_point.gx_point_y;
                    nexty--, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        nextx -= x_sign;
                    }
                    blend_func(context, nextx, nexty, linecolor, alpha);
                }

                /* walk out the clipping point.  */
                for (curx = xstart, cury = ystart, decision = (dy >> 1); cury < mid_point.gx_point_y;
                    cury++, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        curx += x_sign;
                    }

                    if (curx >= clip -> gx_rectangle_left &&
                        curx <= clip -> gx_rectangle_right &&
                        cury >= clip -> gx_rectangle_top)
                    {
                        break;
                    }
                }
                for (; cury <= mid_point.gx_point_y;
                    cury++, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        curx += x_sign;
                    }
                    blend_func(context, curx, cury, linecolor, alpha);
                }
            }  
        }
        else
        {
            /* The clip stay at one side.  */
            if (dx >= dy)
            {
                half_rectangle.gx_rectangle_left = (GX_VALUE)xstart;
                half_rectangle.gx_rectangle_right = mid_point.gx_point_x;
                if (y_sign == 1)
                {
                    half_rectangle.gx_rectangle_top = (GX_VALUE)ystart;
                    half_rectangle.gx_rectangle_bottom = mid_point.gx_point_y;
                }
                else
                {
                    half_rectangle.gx_rectangle_top = mid_point.gx_point_y;
                    half_rectangle.gx_rectangle_bottom = (GX_VALUE)ystart;
                }

                if (_gx_utility_rectangle_overlap_detect(clip, &half_rectangle, &half_over))
                {
                    curx = xstart;
                    cury = ystart;
                    steps = mid_point.gx_point_x - curx + 1;
                    sign = 1;
                }
                else
                {
                    curx = xend;
                    cury = yend;
                    steps = xend - mid_point.gx_point_x;
                    sign = -1;
                    y_sign = 0 - y_sign;
                }
                for (decision = (dx >> 1); steps > 0; curx += sign, decision += dy, steps--)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        cury += y_sign;
                    }

                    if (curx >= clip -> gx_rectangle_left &&
                        curx <= clip -> gx_rectangle_right &&
                        cury >= clip -> gx_rectangle_top &&
                        cury <= clip -> gx_rectangle_bottom)
                    {
                        blend_func(context, curx, cury, linecolor, alpha);
                    }
                }
            }
            else
            {
                half_rectangle.gx_rectangle_top = (GX_VALUE)ystart;
                half_rectangle.gx_rectangle_bottom = mid_point.gx_point_y;
                if (x_sign == 1)
                {
                    half_rectangle.gx_rectangle_right = mid_point.gx_point_x;
                    half_rectangle.gx_rectangle_left = (GX_VALUE)xstart;
                }
                else
                {
                    half_rectangle.gx_rectangle_right = (GX_VALUE)xstart;
                    half_rectangle.gx_rectangle_left = mid_point.gx_point_x;
                }

                if (_gx_utility_rectangle_overlap_detect(clip, &half_rectangle, &half_over))
                {
                    curx = xstart;
                    cury = ystart;
                    steps = mid_point.gx_point_y - cury + 1;
                    sign = 1;
                }
                else
                {
                    curx = xend;
                    cury = yend;
                    steps = yend - mid_point.gx_point_y;
                    sign = -1;
                    x_sign = 0 - x_sign;
                }

                for (decision = (dy >> 1); steps > 0; cury += sign, decision += dx, steps--)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        curx += x_sign;
                    }
                    if (curx >= clip -> gx_rectangle_left &&
                        curx <= clip -> gx_rectangle_right &&
                        cury >= clip -> gx_rectangle_top &&
                        cury <= clip -> gx_rectangle_bottom)
                    {
                        blend_func(context, curx, cury, linecolor, alpha);
                    }
                }
            }
        }
    }
    else
    {
        /* here if both line ends lie within clipping rectangle, we can
        run a faster inner loop */
        if (dx >= dy)
        {
            for (curx = xstart, cury = ystart, nextx = xend, nexty = yend,
                decision = (dx >> 1); curx <= nextx; curx++, nextx--,
                decision += dy)
            {

                if (decision >= dx)
                {
                    decision -= dx;
                    cury += y_sign;
                    nexty -= y_sign;

                }
                blend_func(context, curx, cury, linecolor, alpha);
                blend_func(context, nextx, nexty, linecolor, alpha);
            }
        }
        else
        {

            for (curx = xstart, cury = ystart, nextx = xend, nexty = yend,
                decision = (dy >> 1); cury <= nexty; cury++, nexty--,
                decision += dx)
            {
                if (decision >= dy)
                {
                    decision -= dy;
                    curx += x_sign;
                    nextx -= x_sign;
                }
                blend_func(context, curx, cury, linecolor, alpha);
                blend_func(context, nextx, nexty, linecolor, alpha);
            }
        }
    }
}

#endif /* GX_BRUSH_ALPHA_SUPPORT */
