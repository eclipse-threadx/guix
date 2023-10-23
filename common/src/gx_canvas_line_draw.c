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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_line_draw                                PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws the specified line on the current context.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_start                            x-coord of endpoint1             */
/*    y_start                            y-coord of endpoint1             */
/*    x_end                              x-coord of endpoint2             */
/*    y_end                              y-coord of endpoint2             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                             Completion status                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_utility_rectanlge_define        Define a rectangle               */
/*    gx_utility_rectangle_overlap_detect                                 */
/*                                       Detect rectangle overlap         */
/*    [gx_display_driver_line_draw]      The generic display driver line  */
/*                                         drawing routine                */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                       The display driver horizontal    */
/*                                         line drawing function          */
/*    [gx_display_driver_vertical_line_draw]                              */
/*                                       The display driver vertical      */
/*                                         line drawing function          */
/*    [gx_display_driver_horizontal_pattern_line_draw]                    */
/*                                       The display driver horizontal    */
/*                                         pattern line drawing function  */
/*    [gx_display_driver_vertical_pattern_line_draw]                      */
/*                                       The display driver vertical      */
/*                                         pattern line drawing function  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s), fixed a  */
/*                                            pattern line draw issue,    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     clip_rect;
GX_RECTANGLE     bound;
GX_VIEW         *view;
GX_BRUSH        *brush;
GX_BOOL          simple_line = GX_FALSE;
GX_VALUE         width;
GX_BOOL          anti_aliased;
GX_VALUE         brush_width;

    /* calculate rectangle that bounds the line. This bounding rectangle
       is later adjusted based on line width */

    /* are line coordinates left to right? */

    if (x_start <= x_end)
    {
        /* are line coordinates bottom to top? */
        if (y_start >= y_end)
        {
            _gx_utility_rectangle_define(&bound, x_start, y_end, x_end, y_start);
        }
        else
        {
            /* line is defined left to right, top to bottom */
            _gx_utility_rectangle_define(&bound, x_start, y_start, x_end, y_end);
        }
    }
    else
    {
        if (y_start >= y_end)
        {
            /* line is defined right to left, bottom to top */
            _gx_utility_rectangle_define(&bound, x_end, y_end, x_start, y_start);
        }
        else
        {
            /* line is defined right to left, top to bottom */
            _gx_utility_rectangle_define(&bound, x_end, y_start, x_start, y_end);
        }
    }

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;
    brush = &context -> gx_draw_context_brush;
    brush_width = brush -> gx_brush_width;

    if (brush_width == 0)
    {
        /* Check brush width. Just return if width is 0. */
        return(GX_SUCCESS);
    }
    
    if ((brush_width == 1) ||
        ((brush -> gx_brush_style & GX_BRUSH_ROUND) == 0))
    {
        /* If we are drawing a horizontal or vertial line with
        square line ends, we can do a much simpler and faster
        line drawing function. Check for these special cases.
        */

        /* the brush 1 pixel wide or not round, check for horizontal or vertical */
        if ((y_start == y_end) || (x_start == x_end))
        {
            /* yes, simple line case */
            simple_line = GX_TRUE;
        }
    }

    /* pick up current display driver */
    display = context->gx_draw_context_display;
    /* Angled line using current context brush and canvas: */
    anti_aliased = GX_FALSE;

    if (brush -> gx_brush_style & GX_BRUSH_ALIAS)
    {
        /* The caller requested an anti-aliased line. Does the driver
        support it? */
        if (brush_width == 1)
        {
            if (display -> gx_display_driver_anti_aliased_line_draw)
            {
                /* Yes, the driver supports anti-aliased lines, so use them: */
                anti_aliased = GX_TRUE;
            }
        }
        else
        {
            if (display -> gx_display_driver_anti_aliased_wide_line_draw)
            {
                /* Yes, the driver supports anti-aliased lines, so use them: */
                anti_aliased = GX_TRUE;
            }
        }
    }

    /* We need to expand the bounding rectangle to account for
       rounded ends and line width > 1
     */
    if (simple_line)
    {
        if(!brush -> gx_brush_line_pattern)
        {
            if (x_start == x_end)
            {
                /* vertical line centered around x coords */
                bound.gx_rectangle_left  = (GX_VALUE)(x_start - (brush_width / 2));
                bound.gx_rectangle_right = (GX_VALUE)(bound.gx_rectangle_left + brush_width - 1);
            }
            else
            {
                /* horizontal line centered around y coords */
                bound.gx_rectangle_top = (GX_VALUE)(y_start - (brush_width / 2));
                bound.gx_rectangle_bottom = (GX_VALUE)(bound.gx_rectangle_top + brush_width - 1);
            }
        }
    }
    else
    {
        width = (GX_VALUE)((brush_width + 1) / 2);

        if (anti_aliased)
        {
            width = (GX_VALUE)(width + 1);
        }

        /* increase the bound by 1/2 the line width */
        bound.gx_rectangle_top    = (GX_VALUE)(bound.gx_rectangle_top - width);
        bound.gx_rectangle_left   = (GX_VALUE)(bound.gx_rectangle_left - width);
        bound.gx_rectangle_right  = (GX_VALUE)(bound.gx_rectangle_right + width);
        bound.gx_rectangle_bottom = (GX_VALUE)(bound.gx_rectangle_bottom + width);
    }

    /* clip the line bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* test to determine if the bounding rectangle overlaps the region we are allowed to draw
       into. For each view that overlaps the bounding rectangle, do some drawing.
     */
    view = context -> gx_draw_context_view_head;

    while (view)
    {
        if (!_gx_utility_rectangle_overlap_detect(&view -> gx_view_rectangle, &bound, &clip_rect))
        {
            view = view -> gx_view_next;
            continue;
        }

        /* we have a view into which we can draw the line, do it */
        context -> gx_draw_context_clip = &clip_rect;

        if (simple_line)
        {
            if (y_start == y_end)
            {
                if (brush -> gx_brush_line_pattern)
                {
                    if (clip_rect.gx_rectangle_left > x_start)
                    {
                        width = (GX_VALUE)((clip_rect.gx_rectangle_left - x_start) & 0x1F);
                        context -> gx_draw_context_brush.gx_brush_pattern_mask >>= width;
                    }

                    /* Call display driver's simple horizontal pattern line drawing function.  */
                    display -> gx_display_driver_horizontal_pattern_line_draw(context,
                                                                              clip_rect.gx_rectangle_left,
                                                                              clip_rect.gx_rectangle_right,
                                                                              y_start);

                    if (clip_rect.gx_rectangle_right < x_end)
                    {
                        width = (GX_VALUE)((x_end - clip_rect.gx_rectangle_right) & 0x1F);
                        if ((context -> gx_draw_context_brush.gx_brush_pattern_mask >> width) == 0)
                        {
                            context -> gx_draw_context_brush.gx_brush_pattern_mask <<= (32 - width);
                        }
                        else
                        {
                            context -> gx_draw_context_brush.gx_brush_pattern_mask >>= width;
                        }
                    }
                }
                else
                {
                    /* Call display driver's simple horizontal line drawing function.  */
                    display -> gx_display_driver_horizontal_line_draw(context,
                                                                      clip_rect.gx_rectangle_left,
                                                                      clip_rect.gx_rectangle_right,
                                                                      clip_rect.gx_rectangle_top,
                                                                      clip_rect.gx_rectangle_bottom - clip_rect.gx_rectangle_top + 1,
                                                                      brush -> gx_brush_line_color);
                }
            }
            else
            {
                if (brush -> gx_brush_line_pattern)
                {
                    if (clip_rect.gx_rectangle_top > y_start)
                    {
                        width = (GX_VALUE)((clip_rect.gx_rectangle_top - y_start) & 0x1F);
                        context -> gx_draw_context_brush.gx_brush_pattern_mask >>= width;
                    }

                    /* Call display driver's simple vertical line drawing function.  */
                    display -> gx_display_driver_vertical_pattern_line_draw(context,
                                                                            clip_rect.gx_rectangle_top,
                                                                            clip_rect.gx_rectangle_bottom,
                                                                            x_start);

                    if (clip_rect.gx_rectangle_bottom < y_end)
                    {
                        width = (GX_VALUE)((y_end - clip_rect.gx_rectangle_bottom) & 0x1F);
                        if ((context -> gx_draw_context_brush.gx_brush_pattern_mask >> width) == 0)
                        {
                            context -> gx_draw_context_brush.gx_brush_pattern_mask <<= (32 - width);
                        }
                        else
                        {
                            context -> gx_draw_context_brush.gx_brush_pattern_mask >>= width;
                        }
                    }
                }
                else
                {
                    /* Call display driver's simple vertical line drawing function.  */
                    display -> gx_display_driver_vertical_line_draw(context,
                                                                    clip_rect.gx_rectangle_top,
                                                                    clip_rect.gx_rectangle_bottom,
                                                                    clip_rect.gx_rectangle_left,
                                                                    clip_rect.gx_rectangle_right - clip_rect.gx_rectangle_left + 1,
                                                                    brush -> gx_brush_line_color);
                }
            }
            view = view -> gx_view_next;
            continue;
        }


        if (anti_aliased)
        {
            if (brush_width == 1)
            {
                display -> gx_display_driver_anti_aliased_line_draw(context, x_start, y_start, x_end, y_end);
            }
            else
            {
                display -> gx_display_driver_anti_aliased_wide_line_draw(context, x_start, y_start, x_end, y_end);
            }
        }
        else
        {
            if (brush_width == 1)
            {
                display -> gx_display_driver_simple_line_draw(context, x_start, y_start, x_end, y_end);
            }
            else
            {
                display -> gx_display_driver_simple_wide_line_draw(context, x_start, y_start, x_end, y_end);
            }
        }
        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

