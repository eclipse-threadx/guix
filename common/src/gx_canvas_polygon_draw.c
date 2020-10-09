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
#include "gx_display.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_polygon_draw                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws a polygon into the current context.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    point_array                           Array of points of the polygon*/
/*                                            terminated by NULL entry.   */
/*    number_of_points                      Number of points of polygon   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    [gx_display_driver_polygon_draw]      The display driver basic      */
/*                                            polygon drawing routine     */
/*    [gx_display_driver_polygon_fill]      The display driver basic      */
/*                                            polygon drawing routine     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT  _gx_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     bound;
GX_VIEW         *view;
GX_RECTANGLE     clip_rect;
INT              i;
GX_VALUE         left = point_array[0].gx_point_x;
GX_VALUE         right = point_array[0].gx_point_x;
GX_VALUE         top = point_array[0].gx_point_y;
GX_VALUE         bottom = point_array[0].gx_point_y;
GX_VALUE         width;

    /* Calculate the minumum bounding rectangle. */
    for (i = 1; i < number_of_points; i++)
    {
        if (point_array[i].gx_point_x < left)
        {
            left = point_array[i].gx_point_x;
        }
        if (point_array[i].gx_point_x > right)
        {
            right = point_array[i].gx_point_x;
        }
        if (point_array[i].gx_point_y < top)
        {
            top = point_array[i].gx_point_y;
        }
        if (point_array[i].gx_point_y > bottom)
        {
            bottom = point_array[i].gx_point_y;
        }
    }

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    width = (GX_VALUE)((context -> gx_draw_context_brush.gx_brush_width + 1) >> 1);

    _gx_utility_rectangle_define(&bound, (GX_VALUE)(left - width), (GX_VALUE)(top - width), (GX_VALUE)(right + width), (GX_VALUE)(bottom + width));

    /* clip the polygon bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
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
        if (!_gx_utility_rectangle_overlap_detect(&view -> gx_view_rectangle, &bound, &clip_rect))
        {
            view = view -> gx_view_next;
            continue;
        }

        /* we have a view into which we can draw the polygon, do it */
        context -> gx_draw_context_clip = &clip_rect;

        if (context -> gx_draw_context_brush.gx_brush_style & (GX_BRUSH_SOLID_FILL | GX_BRUSH_PIXELMAP_FILL))
        {
            display -> gx_display_driver_polygon_fill(context, point_array, number_of_points);
        }

        if (display -> gx_display_driver_polygon_draw)
        {
            /* Draw polygon. */
            display -> gx_display_driver_polygon_draw(context, point_array, number_of_points);
        }

        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

