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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_16bpp_simple_line_draw           PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16bpp color format line draw function.                      */
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
/*    _gx_display_driver_simple_line_alpha_draw                           */
/*                                          Display driver basic simpile  */
/*                                            line alpha draw function    */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_16bpp_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend)
{
INT           curx;
INT           cury;
INT           x_sign;
INT           y_sign;
INT           decision;
INT           nextx;
INT           nexty;
INT           y_increment;
GX_POINT      end_point;
GX_POINT      mid_point;
GX_RECTANGLE  half_rectangle;
GX_RECTANGLE  half_over;
INT           sign;
INT           steps;

USHORT       *put;
USHORT       *next_put;

GX_BOOL       clipped = GX_TRUE;
INT           dx = GX_ABS(xend - xstart);
INT           dy = GX_ABS(yend - ystart);

GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_COLOR      linecolor = context -> gx_draw_context_brush.gx_brush_line_color;

#if defined GX_BRUSH_ALPHA_SUPPORT
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (alpha == 0)
    {
        /* Nothing to drawn. Just return. */
        return;
    }
    if (alpha != 0xff)
    {
        _gx_display_driver_simple_line_alpha_draw(context, xstart, ystart, xend, yend, alpha);
        return;
    }
#endif

    if (((dx >= dy && (xstart > xend)) || ((dy > dx) && ystart > yend)))
    {
        GX_SWAP_VALS(xend, xstart);
        GX_SWAP_VALS(yend, ystart);
    }
    x_sign = (xend - xstart) / dx;
    y_sign = (yend - ystart) / dy;

    if (y_sign > 0)
    {
        y_increment = context -> gx_draw_context_pitch;
    }
    else
    {
        y_increment = 0 - context -> gx_draw_context_pitch;
    }

    put = (USHORT *)(context -> gx_draw_context_memory);
    GX_CALCULATE_PUTROW(put, xstart, ystart, context);

    next_put = (USHORT *)(context -> gx_draw_context_memory);
    GX_CALCULATE_PUTROW(next_put, xend, yend, context);


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
                        put += y_increment;
                    }

                    if (curx >= clip -> gx_rectangle_left &&
                        cury >= clip -> gx_rectangle_top &&
                        cury <= clip -> gx_rectangle_bottom)
                    {
                        break;
                    }
                    put++;
                }
                for (; curx <= mid_point.gx_point_x;
                     curx++, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        cury += y_sign;
                        put += y_increment;
                    }
                    PIXEL_WRITE(put, linecolor);
                    put++;
                }
                for (nextx = xend, nexty = yend, decision = (dx >> 1); nextx > mid_point.gx_point_x;
                     nextx--, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        nexty -= y_sign;
                        next_put -= y_increment;
                    }
                    if (nextx <= clip -> gx_rectangle_right &&
                        nexty >= clip -> gx_rectangle_top &&
                        nexty <= clip -> gx_rectangle_bottom)
                    {
                        break;
                    }
                    next_put--;
                }

                for (; nextx > mid_point.gx_point_x;
                     nextx--, decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        nexty -= y_sign;
                        next_put -= y_increment;
                    }
                    PIXEL_WRITE(next_put, linecolor);
                    next_put--;
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
                        next_put -= x_sign;
                    }
                    if (nextx >= clip -> gx_rectangle_left &&
                        nextx <= clip -> gx_rectangle_right &&
                        nexty <= clip -> gx_rectangle_bottom)
                    {
                        break;
                    }
                    next_put -= context -> gx_draw_context_pitch;
                }

                for (; nexty > mid_point.gx_point_y;
                     nexty--, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        nextx -= x_sign;
                        next_put -= x_sign;
                    }
                    PIXEL_WRITE(next_put, linecolor);
                    next_put -= context -> gx_draw_context_pitch;
                }

                /* walk out the clipping point.  */
                for (curx = xstart, cury = ystart, decision = (dy >> 1); cury < mid_point.gx_point_y;
                     cury++, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        curx += x_sign;
                        put += x_sign;
                    }

                    if (curx >= clip -> gx_rectangle_left &&
                        curx <= clip -> gx_rectangle_right &&
                        cury >= clip -> gx_rectangle_top)
                    {
                        break;
                    }
                    put += context -> gx_draw_context_pitch;
                }
                for (; cury <= mid_point.gx_point_y;
                     cury++, decision += dx)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        curx += x_sign;
                        put += x_sign;
                    }
                    PIXEL_WRITE(put, linecolor);
                    put += context -> gx_draw_context_pitch;
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
                    y_increment = 0 - y_increment;
                    y_sign = 0 - y_sign;
                    put = next_put;
                }
                for (decision = (dx >> 1); steps > 0; curx += sign, decision += dy, steps--)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        cury += y_sign;
                        put += y_increment;
                    }

                    if (curx >= clip -> gx_rectangle_left &&
                        curx <= clip -> gx_rectangle_right &&
                        cury >= clip -> gx_rectangle_top &&
                        cury <= clip -> gx_rectangle_bottom)
                    {
                        PIXEL_WRITE(put, linecolor);
                    }
                    put += sign;
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
                    y_increment = context -> gx_draw_context_pitch;
                    sign = 1;
                }
                else
                {
                    curx = xend;
                    cury = yend;
                    steps = yend - mid_point.gx_point_y;
                    sign = -1;
                    y_increment = 0 - context -> gx_draw_context_pitch;
                    x_sign = 0 - x_sign;
                    put = next_put;
                }

                for (decision = (dy >> 1); steps > 0; cury += sign, decision += dx, steps--)
                {
                    if (decision >= dy)
                    {
                        decision -= dy;
                        curx += x_sign;
                        put += x_sign;
                    }
                    if (curx >= clip -> gx_rectangle_left &&
                        curx <= clip -> gx_rectangle_right &&
                        cury >= clip -> gx_rectangle_top &&
                        cury <= clip -> gx_rectangle_bottom)
                    {
                        PIXEL_WRITE(put, linecolor);
                    }
                    put += y_increment;
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
            put = (USHORT *)(context -> gx_draw_context_memory) + ystart * context -> gx_draw_context_pitch + xstart;
            next_put = (USHORT *)(context -> gx_draw_context_memory) + yend * context -> gx_draw_context_pitch + xend;

            for (curx = xstart, cury = ystart, nextx = xend, nexty = yend,
                 decision = (dx >> 1); curx <= nextx; curx++, nextx--,
                 decision += dy)
            {

                if (decision >= dx)
                {
                    decision -= dx;
                    cury += y_sign;
                    nexty -= y_sign;

                    put += y_increment;
                    next_put -= y_increment;
                }
                PIXEL_WRITE(put, linecolor);
                PIXEL_WRITE(next_put, linecolor);

                put++;
                next_put--;
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

                    put += x_sign;
                    next_put -= x_sign;
                }
                PIXEL_WRITE(put, linecolor);
                PIXEL_WRITE(next_put, linecolor);

                put += context -> gx_draw_context_pitch;
                next_put -= context -> gx_draw_context_pitch;
            }
        }
    }
}

