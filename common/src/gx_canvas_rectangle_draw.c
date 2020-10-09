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
/*    _gx_canvas_rectangle_fill                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Support function for gx_canvas_rectangle_draw, draw a wide border   */
/*    by drawing a filled rectangle.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rectangle                             Rectangle specification       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    _gx_canvas_line_draw                  Draw a line                   */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          Display driver horizontal     */
/*                                            line draw routine           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_rectangle_draw                                           */
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
static UINT  _gx_canvas_rectangle_fill(GX_RECTANGLE *rectangle, GX_COLOR fill_color)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     fill_rect;
GX_RECTANGLE     bound;
GX_VIEW         *view;
GX_VALUE         rectangle_height;

    /* pick up current drawing context */
    context = _gx_system_current_draw_context;

    /* Check to see if the rectangle overlaps the dirty area.  */
    if (!_gx_utility_rectangle_overlap_detect(rectangle, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;


    /* test to determine if the bounding rectangle overlaps the region we are allowed to draw
       into. For each view that overlaps the bounding rectangle, do some drawing.
     */
    view = context -> gx_draw_context_view_head;

    while (view)
    {
        if (_gx_utility_rectangle_overlap_detect(&bound, &view -> gx_view_rectangle, &fill_rect))
        {
            /* Pickup the height.  */
            rectangle_height = (GX_VALUE)(fill_rect.gx_rectangle_bottom - fill_rect.gx_rectangle_top + 1);
            context -> gx_draw_context_clip = &fill_rect;

            display -> gx_display_driver_horizontal_line_draw(context,
                                                              fill_rect.gx_rectangle_left,
                                                              fill_rect.gx_rectangle_right,
                                                              fill_rect.gx_rectangle_top,
                                                              rectangle_height, fill_color);
        }
        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_rectangle_draw                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws the specified rectangle on the current context.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rectangle                             Rectangle specification       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    _gx_canvas_line_draw                  Draw a line                   */
/*    _gx_canvas_rectangle_draw             Draw a rectangle              */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          Display driver horizontal     */
/*                                            line draw routine           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_scrollbar_draw                                                  */
/*    _gx_slider_needle_draw                                              */
/*    _gx_widget_border_draw                                              */
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
UINT  _gx_canvas_rectangle_draw(GX_RECTANGLE *rectangle)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     fill_rect;
GX_RECTANGLE     bound;
GX_VIEW         *view;
GX_COLOR         fill_color;
GX_VALUE         rectangle_height;
GX_VALUE         brush_width;

    /* pick up current drawing context */
    context = _gx_system_current_draw_context;

    /* Check to see if the rectangle overlaps the dirty area.  */
    if (!_gx_utility_rectangle_overlap_detect(rectangle, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* if the rectangle has a border, call either the line function (for a single-pixel border)
       or the rectangle functions (for a wide border), then reduce the size of the rectangle
       we are drawing here
     */

    fill_rect = *rectangle;
    brush_width = context -> gx_draw_context_brush.gx_brush_width;

    switch (brush_width)
    {
    case 0:
        /* no border */
        break;

    case 1:
        /* thin border */
        _gx_canvas_line_draw(rectangle -> gx_rectangle_left,
                             (GX_VALUE)(rectangle -> gx_rectangle_top + 1),
                             rectangle -> gx_rectangle_left,
                             (GX_VALUE)(rectangle -> gx_rectangle_bottom - 1));

        _gx_canvas_line_draw(rectangle -> gx_rectangle_right,
                             (GX_VALUE)(rectangle -> gx_rectangle_top + 1),
                             rectangle -> gx_rectangle_right,
                             (GX_VALUE)(rectangle -> gx_rectangle_bottom - 1));

        _gx_canvas_line_draw(rectangle -> gx_rectangle_left,
                             rectangle -> gx_rectangle_top,
                             rectangle -> gx_rectangle_right,
                             rectangle -> gx_rectangle_top);

        _gx_canvas_line_draw(rectangle -> gx_rectangle_left,
                             rectangle -> gx_rectangle_bottom,
                             rectangle -> gx_rectangle_right,
                             rectangle -> gx_rectangle_bottom);

        fill_rect.gx_rectangle_top = (GX_VALUE)(fill_rect.gx_rectangle_top + 1);
        fill_rect.gx_rectangle_bottom = (GX_VALUE)(fill_rect.gx_rectangle_bottom - 1);
        fill_rect.gx_rectangle_left = (GX_VALUE)(fill_rect.gx_rectangle_left + 1);
        fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_right - 1);
        break;

    default:

        /* draw the wide top line */
        fill_rect.gx_rectangle_bottom = (GX_VALUE)(fill_rect.gx_rectangle_top + brush_width - 1);
        _gx_canvas_rectangle_fill(&fill_rect, context -> gx_draw_context_brush.gx_brush_line_color);

        /* draw the wide bottom line */
        fill_rect.gx_rectangle_bottom = rectangle -> gx_rectangle_bottom;
        fill_rect.gx_rectangle_top = (GX_VALUE)(fill_rect.gx_rectangle_bottom - brush_width + 1);
        _gx_canvas_rectangle_fill(&fill_rect, context -> gx_draw_context_brush.gx_brush_line_color);

        /* draw the wide left side */
        fill_rect.gx_rectangle_top = (GX_VALUE)(rectangle -> gx_rectangle_top + brush_width);
        fill_rect.gx_rectangle_bottom = (GX_VALUE)(fill_rect.gx_rectangle_bottom - brush_width);
        fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_left + brush_width - 1);
        _gx_canvas_rectangle_fill(&fill_rect, context -> gx_draw_context_brush.gx_brush_line_color);

        /* draw the wide right side */
        fill_rect.gx_rectangle_right = rectangle -> gx_rectangle_right;
        fill_rect.gx_rectangle_left = (GX_VALUE)(fill_rect.gx_rectangle_right - brush_width + 1);
        _gx_canvas_rectangle_fill(&fill_rect, context -> gx_draw_context_brush.gx_brush_line_color);

        /* subtract the border from the fill_rect */
        fill_rect.gx_rectangle_bottom = (GX_VALUE)(rectangle -> gx_rectangle_bottom - brush_width);
        fill_rect.gx_rectangle_top = (GX_VALUE)(rectangle -> gx_rectangle_top + brush_width);
        fill_rect.gx_rectangle_left = (GX_VALUE)(rectangle -> gx_rectangle_left + brush_width);
        fill_rect.gx_rectangle_right = (GX_VALUE)(rectangle -> gx_rectangle_right - brush_width);
    }

    if (context -> gx_draw_context_brush.gx_brush_width != 0)
    {
        /* Check again, after reducing size, to see if the rectangle overlaps the dirty area.  */
        if (!_gx_utility_rectangle_overlap_detect(&fill_rect, &context -> gx_draw_context_dirty, &bound))
        {
            /* nothing to draw, return */
            return GX_SUCCESS;
        }
    }

    if ((context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_SOLID_FILL) == 0)
    {
        return GX_SUCCESS;
    }

    /* test to determine if the bounding rectangle overlaps the region we are allowed to draw
       into. For each view that overlaps the bounding rectangle, do some drawing.
     */
    view = context -> gx_draw_context_view_head;

    /* save the fill color for later */
    fill_color = context -> gx_draw_context_brush.gx_brush_fill_color;

    while (view)
    {
        if (_gx_utility_rectangle_overlap_detect(&bound, &view -> gx_view_rectangle, &fill_rect))
        {
            /* Pickup the height.  */
            rectangle_height = (GX_VALUE)(fill_rect.gx_rectangle_bottom - fill_rect.gx_rectangle_top + 1);
            context -> gx_draw_context_clip = &fill_rect;

            display -> gx_display_driver_horizontal_line_draw(context,
                                                              fill_rect.gx_rectangle_left,
                                                              fill_rect.gx_rectangle_right,
                                                              fill_rect.gx_rectangle_top,
                                                              rectangle_height, fill_color);
        }
        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

