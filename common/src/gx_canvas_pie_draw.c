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
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_pie_draw                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws a pie into the currrent context.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    xcenter                               x-coord of center of circle   */
/*                                            arc                         */
/*    ycenter                               y-coord of center of circle   */
/*                                            arc                         */
/*    r                                     Radius of circle arc          */
/*    start_angle                           The start angle of circle arc */
/*    end_angle                             The end angle of circle arc   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    [gx_display_driver_pie_fill]          Display driver basic          */
/*                                            pie drawing routine         */
/*    [gx_display_driver_arc_draw]          Display driver basic          */
/*                                            arc drawing routine         */
/*    _gx_display_driver_anti_aliased_arc_draw                            */
/*                                          Display driver basic aliased  */
/*                                            arc drawing routine         */
/*    _gx_utility_circle_point_get          Get point position on a circle*/
/*    _gx_canvas_line_draw                  Draw a line                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application code                                                    */
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
UINT _gx_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
/* A pie is a fraction of a circle delimited by two lines that span from the center of the
   circle to the one side each. */

GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     bound;
GX_RECTANGLE     clip_rect;
GX_VIEW         *view;
GX_BRUSH        *brush;
GX_POINT         point;
INT              brush_width;
VOID             (*outline_function)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE brush_alpha;
#endif

    while (start_angle < 0)
    {
        start_angle += 360;
    }

    while (end_angle < 0)
    {
        end_angle += 360;
    }

    if (start_angle >= 360)
    {
        start_angle %= 360;
    }

    if (end_angle >= 360)
    {
        end_angle %= 360;
    }

    if (end_angle <= start_angle)
    {
        end_angle += 360;
    }
    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    brush = &context -> gx_draw_context_brush;

    brush_width = ((INT)(brush -> gx_brush_width + 1)) >> 1;

    /* Define pie bounding rectangle. */
    _gx_utility_rectangle_define(&bound, (GX_VALUE)(xcenter - (INT)r - brush_width), (GX_VALUE)(ycenter - (INT)r - brush_width),
                                 (GX_VALUE)(xcenter + (INT)r + brush_width), (GX_VALUE)(ycenter + (INT)r + brush_width));

    brush_width = brush -> gx_brush_width;


    /* clip the pie bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* Determine outline function to utilize.*/
    outline_function = GX_NULL;

    /* Determine which outline function to use.*/
    if (brush_width == 1)
    {
        /* if anti-alias is requested and this is supported by display, use it */
        if ((brush -> gx_brush_style & GX_BRUSH_ALIAS) &&
            display -> gx_display_driver_anti_aliased_arc_draw != GX_NULL)
        {
            outline_function = display -> gx_display_driver_anti_aliased_arc_draw;
        }
        else
        {
            /* otherwise use non-aliased outline */
            outline_function = display -> gx_display_driver_arc_draw;
        }
    }
    else
    {
        if (brush_width > 1)
        {
            /* if anti-alias is requested and this is supported by display, use it */
            if ((brush -> gx_brush_style & GX_BRUSH_ALIAS) &&
                display -> gx_display_driver_anti_aliased_wide_arc_draw)
            {
                outline_function = display -> gx_display_driver_anti_aliased_wide_arc_draw;
            }
            else
            {
                /* otherwise use non-aliased outline */
                outline_function = display -> gx_display_driver_wide_arc_draw;
            }
        }
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

        /* we have a view into which we can draw the arc, do it */
        context -> gx_draw_context_clip = &clip_rect;

        if (brush_width)
        {
            brush_width -= 1;
            brush_width >>= 1;
        }

        if ((brush -> gx_brush_style & (GX_BRUSH_SOLID_FILL | GX_BRUSH_PIXELMAP_FILL)) &&
            (r > (UINT)brush_width))
        {
            display -> gx_display_driver_pie_fill(context, xcenter, ycenter, r - (UINT)brush_width, start_angle, end_angle);
        }

#if defined(GX_BRUSH_ALPHA_SUPPORT)
        brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
        if (brush -> gx_brush_width > 1)
        {
            context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
        }
#endif
        if (outline_function)
        {
            outline_function(context, xcenter, ycenter, r, start_angle, end_angle);
        }

        if (r > (UINT)brush_width)
        {
            _gx_utility_circle_point_get(xcenter, ycenter, r - (UINT)brush_width, start_angle, &point);

            /* Draw delimiting line. */
            _gx_canvas_line_draw((GX_VALUE)xcenter, (GX_VALUE)ycenter, point.gx_point_x, point.gx_point_y);

            _gx_utility_circle_point_get(xcenter, ycenter, r - (UINT)brush_width, end_angle, &point);

            /* Draw delimiting line. */
            _gx_canvas_line_draw((GX_VALUE)xcenter, (GX_VALUE)ycenter, point.gx_point_x, point.gx_point_y);
        }

#if defined(GX_BRUSH_ALPHA_SUPPORT)
        context -> gx_draw_context_brush.gx_brush_alpha = brush_alpha;
#endif
        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

#endif

