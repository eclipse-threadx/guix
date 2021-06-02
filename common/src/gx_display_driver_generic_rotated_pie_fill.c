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
/*    _gx_display_driver_generic_rotated_simple_pie_fill  PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to fill a pie chart.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*    start_angle                           Starting angle                */
/*    end_angle                             Ending angle                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_arc_clipping_get   Get an arc clipping.          */
/*    _gx_utility_rectangle_overlap_detect  Detects two rectangles being  */
/*                                            overlap                     */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
/*    _gx_display_driver_circle_point_get   Get point coord on a circle   */
/*    [gx_display_driver_horizontal_pixelmap_line_draw]                   */
/*                                          Basic display driver          */
/*                                            horizontal pixelmap line    */
/*                                            draw function               */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  03-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ARC_DRAWING_SUPPORT)

static VOID _gx_display_driver_generic_rotated_simple_pie_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter,
                                                               UINT r, INT start_angle, INT end_angle, GX_BOOL skip_end)
{
/* The function will only fill a pie with both start angle and end angle are
   between 0 and 180 or beween 180 and 360.*/

GX_DISPLAY           *display;
GX_BRUSH             *brush;
GX_RECTANGLE         *clip;
GX_RECTANGLE          arc_clip[4];
GX_POINT              point;
GX_POINT              points[3];
INT                   sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT                  *pLineEnds;
GX_POINT             *pGet;
INT                   xmin;
INT                   xmax;
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
INT                   loop;
INT                   width;
INT                   xsign;
INT                   ysign;
INT                   decision;
int                   fillingwards;
VOID                  (*line_draw)(GX_DRAW_CONTEXT *context, INT x1, INT x2, INT ypos, INT width, GX_COLOR color);
INT                   xpos = 0;
GX_PIXELMAP          *pixelmap = GX_NULL;
INT                   skip_line;
GX_FILL_PIXELMAP_INFO info;

    display = context -> gx_draw_context_display;
    brush = &context -> gx_draw_context_brush;
    line_draw = display -> gx_display_driver_vertical_line_draw;

    if (brush -> gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        if (brush -> gx_brush_pixelmap == GX_NULL)
        {
            return;
        }

        /* Pick up brush pixelmap. */
        pixelmap = brush -> gx_brush_pixelmap;

        if (pixelmap -> gx_pixelmap_format != display -> gx_display_color_format)
        {
            /* Display driver only support its native format pixelmap.*/
            /* Nothing should be drawn if pixelmap format isn't support. */
            return;
        }

        memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));

        info.pixelmap = pixelmap;
        info.current_pixel_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
        if (pixelmap -> gx_pixelmap_aux_data_size)
        {
            info.current_aux_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
        }
    }

    pGet = points;

    points[1].gx_point_x = (GX_VALUE)xcenter;
    points[1].gx_point_y = (GX_VALUE)ycenter;

    _gx_utility_circle_point_get(xcenter, ycenter, r, start_angle, &points[0]);
    _gx_utility_circle_point_get(xcenter, ycenter, r, end_angle, &points[2]);

    _gx_display_driver_arc_clipping_get(xcenter, ycenter, r, start_angle, end_angle,
                                        &arc_clip[0], &arc_clip[1], &arc_clip[2], &arc_clip[3]);

    /* Pie is in left side. */
    xmin = points[0].gx_point_x;
    xmax = points[2].gx_point_x;

    if (xmin > xmax)
    {
        GX_SWAP_VALS(xmin, xmax);
    }

    if (xmax < xcenter)
    {
        xmax = xcenter;
    }
    else if (xmin > xcenter)
    {
        xmin = xcenter;
    }

    clip = context -> gx_draw_context_clip;

    if (clip -> gx_rectangle_left > xmin)
    {
        xmin = clip -> gx_rectangle_left;
    }

    if (clip -> gx_rectangle_right < xmax)
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

    curx = 0;
    cury = (INT)r;
    decision = 5 - (INT)(4 * r);

    while (curx <= cury)
    {
        for (loop = 0; loop < 4; loop++)
        {
            point.gx_point_x = (GX_VALUE)(curx * sign[loop][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(cury * sign[loop][1] + ycenter);

            if ((point.gx_point_x >= xmin) && (point.gx_point_x <= xmax))
            {
                if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[1], point))
                {
                    Index = (point.gx_point_x - xmin) << 1;
                    if (point.gx_point_y < pLineEnds[Index])
                    {
                        pLineEnds[Index] = point.gx_point_y;
                    }

                    if (point.gx_point_y > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = point.gx_point_y;
                    }
                }
            }

            point.gx_point_x = (GX_VALUE)(cury * sign[loop][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(curx * sign[loop][1] + ycenter);

            if ((point.gx_point_x >= xmin) && (point.gx_point_x <= xmax))
            {
                if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                    _gx_utility_rectangle_point_detect(&arc_clip[1], point))
                {
                    Index = (point.gx_point_x - xmin) << 1;
                    if (point.gx_point_y < pLineEnds[Index])
                    {
                        pLineEnds[Index] = point.gx_point_y;
                    }

                    if (point.gx_point_y > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = point.gx_point_y;
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
       2 lines of circle sector
     */

    for (loop = 0; loop < 2; loop++)
    {
        xstart = pGet -> gx_point_x;
        ystart = pGet -> gx_point_y;
        pGet++;
        xend = pGet -> gx_point_x;
        yend = pGet -> gx_point_y;
        dx = GX_ABS(xend - xstart);
        dy = GX_ABS(yend - ystart);

        /* Vertical Line. */
        if (xstart == xend)
        {
            continue;
        }

        /* Horizontal Line. */
        if (ystart == yend)
        {
            if (xstart > xend)
            {
                GX_SWAP_VALS(xstart, xend);
                GX_SWAP_VALS(ystart, yend);
            }

            if (skip_end)
            {
                ystart--;
            }

            for (curx = xstart; curx <= xend; curx++)
            {
                if ((curx >= xmin) && (curx <= xmax))
                {
                    Index = (curx - xmin) << 1;
                    if (ystart < pLineEnds[Index])
                    {
                        pLineEnds[Index] = ystart;
                    }

                    if (ystart > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = ystart;
                    }
                }
            }
            continue;
        }

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

                if ((curx >= xmin) && (curx <= xmax))
                {
                    Index = (curx - xmin) << 1;

                    if (cury < pLineEnds[Index])
                    {
                        pLineEnds[Index] = cury;
                    }

                    if (cury - 1 > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = cury - 1;
                    }
                }

                if ((nextx >= xmin) && (nextx <= xmax))
                {
                    Index = (nextx - xmin) << 1;

                    if (nexty < pLineEnds[Index])
                    {
                        pLineEnds[Index] = nexty;
                    }

                    if (nexty - 1 > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = nexty - 1;
                    }
                }
            }
        }
        else
        {
            if (start_angle < 180)
            {
                if (loop == 0)
                {
                    fillingwards = 0;
                }
                else
                {
                    fillingwards = 1;
                }
            }
            else
            {
                if (loop == 0)
                {
                    fillingwards = 1;
                }
                else
                {
                    fillingwards = 0;
                }
            }

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

                if ((curx - 1 + fillingwards >= xmin) && (curx - 1 + fillingwards <= xmax))
                {
                    Index = (curx - 1 + fillingwards - xmin) << 1;

                    if (cury < pLineEnds[Index])
                    {
                        pLineEnds[Index] = cury;
                    }

                    if (cury > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = cury;
                    }
                }

                if ((nextx - 1 + fillingwards >= xmin) && (nextx - 1 + fillingwards <= xmax))
                {
                    Index = (nextx - 1 + fillingwards - xmin) << 1;

                    if (nexty < pLineEnds[Index])
                    {
                        pLineEnds[Index] = nexty;
                    }

                    if (nexty > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = nexty;
                    }
                }
            }
        }
    }

    /* Filling circle sector with horizontal line. */
    if (pixelmap)
    {
        if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            skip_line = (xmax - (xcenter - (INT)r) + 1) % pixelmap -> gx_pixelmap_width;

            /*Skip the un-draw line.*/
            if (skip_line)
            {
                skip_line = pixelmap -> gx_pixelmap_width - skip_line;
            }

            Index = (width - 1) << 1;
            xsign = -1;
            xstart = xmax;
        }
        else
        {
            skip_line = (xmin - clip -> gx_rectangle_left);

            Index = 0;
            xsign = 1;
            xstart = xmin;
        }

        /*Skip the un-draw line.*/
        if (skip_line)
        {
            info.draw = GX_FALSE;
            while (skip_line--)
            {
                display -> gx_display_driver_horizontal_pixelmap_line_draw(context, 0, 0, 0, &info);
            }
        }

        info.draw = GX_TRUE;
        xpos = ycenter - (INT)r;

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

            info.x_offset = pLineEnds[Index] - xpos;

            /* Filling pie area with pixelmap. */
            display -> gx_display_driver_horizontal_pixelmap_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], xstart, &info);

            xstart += xsign;
            Index += xsign;
            Index += xsign;
        }
    }
    else
    {
        Index = 0;
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
                line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], curx, 1,
                          brush -> gx_brush_fill_color);
            }

            Index += 2;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_rotated_pie_fill         PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to fill a pie.                                       */
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
/*    _gx_display_driver_generic_simple_pie_fill                          */
/*                                          Real display driver draw      */
/*                                            filled-pie function         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_pie_draw                                                 */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_rotated_pie_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
    /* The function fills a pie.*/


    if (start_angle < 180)
    {
        if (end_angle < 180)
        {
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, start_angle, end_angle, GX_FALSE);
        }
        else if (end_angle < 360)
        {
            /* Skip-end parameter should only be set when drawing the above area.
               It would be set to GX_TRUE to skip the bottom line to avoid case that this line will be drawn twice, which
               is not correct, when brush alpha is set. */
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 180, GX_TRUE);
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, 180, end_angle, GX_FALSE);
        }
        else
        {
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 180, GX_TRUE);
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, 180, 360, GX_FALSE);
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, 0, end_angle - 360, GX_TRUE);
        }
    }
    else
    {
        if (end_angle < 360)
        {
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, start_angle, end_angle, GX_FALSE);
        }
        else if (end_angle < 540)
        {
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 360, GX_FALSE);
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, 0, end_angle - 360, GX_TRUE);
        }
        else
        {
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 360, GX_FALSE);
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, 0, 180, GX_TRUE);
            _gx_display_driver_generic_rotated_simple_pie_fill(context, xcenter, ycenter, r, 180, end_angle - 360, GX_FALSE);
        }
    }
}

#endif

