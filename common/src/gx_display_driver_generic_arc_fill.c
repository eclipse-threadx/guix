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
/*    _gx_display_driver_generic_arc_fill                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to fill circle sector.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*    start_angle                           The start angle of circle arc */
/*    end_angle                             The end angle of circle arc   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          Basic display driver          */
/*                                            horizontal line draw routine*/
/*    _gx_display_driver_arc_clipping_get   Get an arc clipping.          */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
/*    _gx_utility_circle_point_get          Get point coord on a circle   */
/*    [gx_display_driver_horizontal_pixelmap_line_draw]                   */
/*                                          Basic display driver          */
/*                                            horizontal pixelmap line    */
/*                                            draw function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_arc_draw                                 */
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
#if defined(GX_ARC_DRAWING_SUPPORT)

VOID _gx_display_driver_generic_arc_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{

GX_DISPLAY           *display;
GX_RECTANGLE         *clip;
GX_RECTANGLE          arc_clip[4];
GX_BRUSH             *brush;
GX_POINT              point;
INT                   sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT                  *pLineEnds;
INT                   ymin;
INT                   ymax;
INT                   xstart;
INT                   xend;
INT                   ystart;
INT                   yend;
INT                   curx;
INT                   cury;
INT                   nextx;
INT                   nexty;
INT                   dx;
INT                   dy;
INT                   Index;
INT                   Index1;
INT                   height;
INT                   xsign;
INT                   ysign;
INT                   decision;
VOID                  (*line_draw)(GX_DRAW_CONTEXT *context, INT x1, INT x2, INT ypos, INT width, GX_COLOR color);
GX_PIXELMAP          *pixelmap = GX_NULL;
INT                   xpos;
INT                   ypos;
INT                   inner_offset;
GX_VALUE              format;
INT                   skip_line;
GX_FILL_PIXELMAP_INFO info;

    display = context -> gx_draw_context_display;
    brush = &context -> gx_draw_context_brush;
    inner_offset = brush -> gx_brush_width;
    line_draw = display -> gx_display_driver_horizontal_line_draw;
    clip = context -> gx_draw_context_clip;

    if (inner_offset)
    {
        inner_offset -= 1;
        inner_offset >>= 1;
    }

    if (r <= (UINT)inner_offset)
    {
        return;
    }

    if (brush -> gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        /* Pick up pixelmap. */
        pixelmap = brush -> gx_brush_pixelmap;

        if (pixelmap == GX_NULL)
        {
            return;
        }
        if (pixelmap -> gx_pixelmap_width <= 0 ||
            pixelmap -> gx_pixelmap_height <= 0)
        {
            return;
        }
        format = display -> gx_display_color_format;
        memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));

        if (format >= GX_COLOR_FORMAT_24XRGB)
        {
            /*32ARGB FORMAT use 24xrgb driver for now. So this is a special case.*/
            if (pixelmap -> gx_pixelmap_format < GX_COLOR_FORMAT_24XRGB)
            {
                /* Display driver only support its native format pixelmap.*/
                /* Nothing should be drawn if pixelmap format isn't support. */
                return;
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_format != format)
            {
                /* Display driver only support its native format pixelmap.*/
                /* Nothing should be drawn if pixelmap format isn't support. */
                return;
            }
            else if (pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_4BIT_GRAY)
            {
                info.mask = 0xf0;
            }
        }

        info.pixelmap = brush -> gx_brush_pixelmap;
        info.current_pixel_ptr = (GX_UBYTE *)info.pixelmap -> gx_pixelmap_data;

        if (pixelmap -> gx_pixelmap_aux_data_size)
        {
            info.current_aux_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
        }
    }

    r = (UINT)(r - (UINT)inner_offset);

    ymax = ycenter + (INT)r;
    ymin = ycenter - (INT)r;

    /* Get two endpoint of the arc. */
    _gx_utility_circle_point_get(xcenter, ycenter, r, start_angle, &point);

    xstart = point.gx_point_x;
    ystart = point.gx_point_y;

    _gx_utility_circle_point_get(xcenter, ycenter, r, end_angle, &point);

    xend = point.gx_point_x;
    yend = point.gx_point_y;

    xpos = xcenter - (INT)r;
    ypos = ycenter - (INT)r;
    skip_line = 0;

    /* Calculate minimum y line. */
    if (((start_angle < 90) && (end_angle < 90)) ||
        ((start_angle > 90) && (end_angle < 450)))
    {
        if (ystart > yend)
        {
            ymin = yend;
        }
        else
        {
            ymin = ystart;
        }
    }

    if (clip -> gx_rectangle_top > ymin)
    {
        ymin = clip -> gx_rectangle_top;
    }
    skip_line = ymin - ypos;

    /* Calculate maximum y line. */
    if (((start_angle < 270) && (end_angle < 270)) ||
        ((start_angle > 270) && (end_angle < 630)))
    {
        if (ystart > yend)
        {
            ymax = ystart;
        }
        else
        {
            ymax = yend;
        }
    }

    if (clip -> gx_rectangle_bottom < ymax)
    {
        ymax = clip -> gx_rectangle_bottom;
    }

    height = ymax - ymin + 1;

    /* default the point array to being off the screen on both sides: */
    pLineEnds = _gx_system_scratchpad;

    for (Index = 0; Index < height * 2; Index += 2)
    {
        pLineEnds[Index] = 2000;
        pLineEnds[Index + 1] = 0;
    }

    /* Get the clipping rectangles of the circle arc. */
    _gx_display_driver_arc_clipping_get(xcenter, ycenter, r, start_angle, end_angle, &arc_clip[0], &arc_clip[1], &arc_clip[2], &arc_clip[3]);

    curx = 0;
    cury = (INT)r;
    decision = (INT)(5 - 4 * r);

    while (curx <= cury)
    {
        for (Index = 0; Index < 4; Index++)
        {
            point.gx_point_x = (GX_VALUE)(curx * sign[Index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(cury * sign[Index][1] + ycenter);

            if ((point.gx_point_y >= ymin) &&
                (point.gx_point_y <= ymax))
            {
                if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[3], point))
                {
                    Index1 = (point.gx_point_y - ymin) << 1;
                    if (point.gx_point_x < pLineEnds[Index1])
                    {
                        pLineEnds[Index1] = point.gx_point_x;
                    }

                    if (point.gx_point_x > pLineEnds[Index1 + 1])
                    {
                        pLineEnds[Index1 + 1] = point.gx_point_x;
                    }
                }
            }

            point.gx_point_x = (GX_VALUE)(cury * sign[Index][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(curx * sign[Index][1] + ycenter);

            if ((point.gx_point_y >= ymin) &&
                (point.gx_point_y <= ymax))
            {
                if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[1], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[2], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[3], point))
                {
                    Index1 = (point.gx_point_y - ymin) << 1;
                    if (point.gx_point_x < pLineEnds[Index1])
                    {
                        pLineEnds[Index1] = point.gx_point_x;
                    }

                    if (point.gx_point_x > pLineEnds[Index1 + 1])
                    {
                        pLineEnds[Index1 + 1] = point.gx_point_x;
                    }
                }
            }
        }

        if (decision < 0)
        {
            decision += 8 * curx + 12;
        }
        else
        {
            decision += 8 * (curx - cury) + 20;
            cury--;
        }
        curx++;
    }

    /* Fill in the point array by using Breshenhams line for
       the line that connect two endpoints of the arc. */

    dx = GX_ABS(xend - xstart);
    dy = GX_ABS(yend - ystart);

    /* Horizontal Line. */
    if (ystart != yend)
    {
        /* Vertical Line. */
        if (xstart == xend)
        {
            if (ystart > yend)
            {
                GX_SWAP_VALS(xstart, xend);
                GX_SWAP_VALS(ystart, yend);
            }

            for (cury = ystart; cury <= yend; cury++)
            {
                if ((cury >= ymin) &&
                    (cury <= ymax))
                {
                    Index = (cury - ymin) << 1;
                    if (xstart <= pLineEnds[Index])
                    {
                        pLineEnds[Index] = xstart;
                    }

                    if (xstart > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = xstart;
                    }
                }
            }
        }
        else
        {
            /* Simple Line. */
            if (((dx >= dy && (xstart > xend)) ||
                 ((dy > dx) && ystart > yend)))
            {
                GX_SWAP_VALS(xend, xstart);
                GX_SWAP_VALS(yend, ystart);
            }

            xsign = (xend - xstart) / dx;
            ysign = (yend - ystart) / dy;

            if (dx >= dy)
            {
                for (curx = xstart, cury = ystart, nextx = xend, nexty = yend,
                     decision = (dx >> 1); curx <= nextx; curx++, nextx--,
                     decision += dy)
                {
                    if (decision >= dx)
                    {
                        decision -= dx;
                        cury += ysign;
                        nexty -= ysign;
                    }

                    if ((cury >= ymin) && (cury <= ymax))
                    {
                        Index = (cury - ymin) << 1;

                        if (curx < pLineEnds[Index])
                        {
                            pLineEnds[Index] = curx;
                        }

                        if (curx > pLineEnds[Index + 1])
                        {
                            pLineEnds[Index + 1] = curx;
                        }
                    }

                    if ((nexty >= ymin) && (nexty <= ymax))
                    {
                        Index1 = (nexty - ymin) << 1;

                        if (nextx < pLineEnds[Index1])
                        {
                            pLineEnds[Index1] = nextx;
                        }

                        if (nextx > pLineEnds[Index1 + 1])
                        {
                            pLineEnds[Index1 + 1] = nextx;
                        }
                    }
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
                        curx += xsign;
                        nextx -= xsign;
                    }

                    if ((cury >= ymin) && (cury <= ymax))
                    {
                        Index = (cury - ymin) << 1;
                        if (curx < pLineEnds[Index])
                        {
                            pLineEnds[Index] = curx;
                        }

                        if (curx > pLineEnds[Index + 1])
                        {
                            pLineEnds[Index + 1] = curx;
                        }
                    }

                    if ((nexty >= ymin) && (nexty <= ymax))
                    {
                        Index1 = (nexty - ymin) << 1;

                        if (nextx < pLineEnds[Index1])
                        {
                            pLineEnds[Index1] = nextx;
                        }

                        if (nextx > pLineEnds[Index1 + 1])
                        {
                            pLineEnds[Index1 + 1] = nextx;
                        }
                    }
                }
            }
        }
    }

    if (pixelmap)
    {
        /* Skip the un-draw line.*/
        if (skip_line > 0)
        {
            skip_line %= info.pixelmap -> gx_pixelmap_height;
            info.draw = GX_FALSE;
            while (skip_line--)
            {
                display -> gx_display_driver_horizontal_pixelmap_line_draw(context, 0, 0, cury, &info);
            }
        }
        info.draw = GX_TRUE;
    }

    Index = 0;
    for (cury = ymin; cury <= ymax; cury++)
    {
        if (pLineEnds[Index] < clip -> gx_rectangle_left)
        {
            pLineEnds[Index] = clip -> gx_rectangle_left;
        }

        if (pLineEnds[Index + 1] > clip -> gx_rectangle_right)
        {
            pLineEnds[Index + 1] = clip -> gx_rectangle_right;
        }

        if (pixelmap)
        {
            /* Filling arc area with pixelmap. */
            info.x_offset = pLineEnds[Index] - xpos;
            display -> gx_display_driver_horizontal_pixelmap_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], cury, &info);
        }
        else
        {
            /* Fill arc with horizontal lines. */
            line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], cury, 1, brush -> gx_brush_fill_color);
        }

        Index += 2;
    }
}

#endif

