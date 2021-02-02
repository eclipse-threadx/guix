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


#define PIXEL_WRITE(loc, val) (*(loc) = ((GX_UBYTE)val))

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_simple_line_draw    PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 8bpp color format rotated line draw function.               */
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
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_8bpp_rotated_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend)
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

GX_UBYTE     *put;
GX_UBYTE     *next_put;

GX_BOOL       clipped = GX_TRUE;
INT           dx;
INT           dy;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_COLOR      linecolor = context -> gx_draw_context_brush.gx_brush_line_color;
GX_RECTANGLE  rotated_clip;

    GX_SWAP_VALS(xstart, ystart);
    GX_SWAP_VALS(xend, yend);
    clip = context -> gx_draw_context_clip;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        ystart = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ystart - 1;
        yend = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - yend - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1);
    }
    else
    {
        xstart = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xstart - 1;
        xend = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xend - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    dx = GX_ABS(xend - xstart);
    dy = GX_ABS(yend - ystart);

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

    put = (GX_UBYTE *)(context -> gx_draw_context_memory) + ystart * context -> gx_draw_context_pitch + xstart;
    next_put = (GX_UBYTE *)(context -> gx_draw_context_memory) + yend * context -> gx_draw_context_pitch + xend;


    end_point.gx_point_x = (GX_VALUE)xstart;
    end_point.gx_point_y = (GX_VALUE)ystart;

    if (_gx_utility_rectangle_point_detect(&rotated_clip, end_point))
    {
        end_point.gx_point_x = (GX_VALUE)xend;
        end_point.gx_point_y = (GX_VALUE)yend;

        if (_gx_utility_rectangle_point_detect(&rotated_clip, end_point))
        {
            clipped = GX_FALSE;
        }
    }

    if (clipped)
    {
        /* here if we must do clipping in the inner loop, because one
           or both of the end points are outside clipping rectangle */

        /* Calculate the middle point of the line.  */
        mid_point.gx_point_x = (GX_VALUE)(xend + xstart) >> 1;
        mid_point.gx_point_y = (GX_VALUE)(yend + ystart) >> 1;

        /* Judge the &rotated_clip in which side.  */
        if (_gx_utility_rectangle_point_detect(&rotated_clip, mid_point))
        {

            /* the &rotated_clip in two sides.  */
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

                    if (curx >= rotated_clip.gx_rectangle_left &&
                        cury >= rotated_clip.gx_rectangle_top &&
                        cury <= rotated_clip.gx_rectangle_bottom)
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
                    if (nextx <= rotated_clip.gx_rectangle_right &&
                        nexty >= rotated_clip.gx_rectangle_top &&
                        nexty <= rotated_clip.gx_rectangle_bottom)
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
                    if (nextx >= rotated_clip.gx_rectangle_left &&
                        nextx <= rotated_clip.gx_rectangle_right &&
                        nexty <= rotated_clip.gx_rectangle_bottom)
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

                    if (curx >= rotated_clip.gx_rectangle_left &&
                        curx <= rotated_clip.gx_rectangle_right &&
                        cury >= rotated_clip.gx_rectangle_top)
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
            /* The &rotated_clip stay at one side.  */
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

                if (_gx_utility_rectangle_overlap_detect(&rotated_clip, &half_rectangle, &half_over))
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

                    if (curx >= rotated_clip.gx_rectangle_left &&
                        curx <= rotated_clip.gx_rectangle_right &&
                        cury >= rotated_clip.gx_rectangle_top &&
                        cury <= rotated_clip.gx_rectangle_bottom)
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

                if (_gx_utility_rectangle_overlap_detect(&rotated_clip, &half_rectangle, &half_over))
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
                    if (curx >= rotated_clip.gx_rectangle_left &&
                        curx <= rotated_clip.gx_rectangle_right &&
                        cury >= rotated_clip.gx_rectangle_top &&
                        cury <= rotated_clip.gx_rectangle_bottom)
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
            put = (GX_UBYTE *)(context -> gx_draw_context_memory) + ystart * context -> gx_draw_context_pitch + xstart;
            next_put = (GX_UBYTE *)(context -> gx_draw_context_memory) + yend * context -> gx_draw_context_pitch + xend;

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

