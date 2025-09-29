/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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
/*    _gx_canvas_circle_draw                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws a circle into the currrent context.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detects two rectangles being  */
/*                                            overlap                     */
/*    [gx_display_driver_arc_draw]          Display driver basic          */
/*                                            arc drawing routine         */
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
UINT _gx_canvas_circle_draw(INT xcenter, INT ycenter, UINT r)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     bound;
GX_RECTANGLE     clip_rect;
GX_VIEW         *view;
GX_BRUSH        *brush;
INT              brush_width;
VOID             (*outline_function)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    brush = &context -> gx_draw_context_brush;

    brush_width = (brush -> gx_brush_width + 1) >> 1;

    _gx_utility_rectangle_define(&bound, (GX_VALUE)(xcenter - (INT)r - brush_width), (GX_VALUE)(ycenter - (INT)r - brush_width),
                                 (GX_VALUE)(xcenter + (INT)r + brush_width), (GX_VALUE)(ycenter + (INT)r + brush_width));

    brush_width = brush -> gx_brush_width;

    /* clip the circle bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* configure outline function to utilize */
    outline_function = GX_NULL;

    /*Set normal outline draw function.*/
    if (brush_width == 1)
    {
        /* if anti-alias is requested and this is supported by display, use it */
        if ((brush -> gx_brush_style & GX_BRUSH_ALIAS) &&
            display -> gx_display_driver_anti_aliased_circle_draw != GX_NULL)
        {
            outline_function = display -> gx_display_driver_anti_aliased_circle_draw;
        }
        else
        {
            /* otherwise use non-aliased outline */
            outline_function = display -> gx_display_driver_circle_draw;
        }
    }
    else
    {
        if (brush_width > 1)
        {
            /* if anti-alias is requested and this is supported by display, use it */
            if ((brush -> gx_brush_style & GX_BRUSH_ALIAS) &&
                display -> gx_display_driver_anti_aliased_wide_circle_draw != GX_NULL )
            {
                outline_function = display -> gx_display_driver_anti_aliased_wide_circle_draw;
            }
            else
            {
                /* otherwise use non-aliased outline */
                outline_function = display -> gx_display_driver_wide_circle_draw;
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

        /* we have a view into which we can draw the line, do it */
        GX_RECTANGLE *original_clip = context->gx_draw_context_clip;
        context -> gx_draw_context_clip = &clip_rect;

        if (brush -> gx_brush_style & (GX_BRUSH_SOLID_FILL | GX_BRUSH_PIXELMAP_FILL))
        {
            display -> gx_display_driver_circle_fill(context, xcenter, ycenter, r);
        }

        if (outline_function)
        {
            outline_function(context, xcenter, ycenter, r);
        }

        view = view -> gx_view_next;
        context -> gx_draw_context_clip = original_clip;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}
#endif

