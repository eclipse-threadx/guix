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
/*    _gx_canvas_ellipse_draw                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws a ellipse into the currrent context.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    xcenter                               x-coord of center of ellipse  */
/*    ycenter                               y-coord of center of ellipse  */
/*    r                                     Radius of circle              */
/*    a                                     Length of the Semi-major Axis */
/*    b                                     Length of the Semi-minor Axis */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define                                        */
/*    _gx_utility_rectangle_overlap_detect                                */
/*    [gx_display_driver_generic_ellipse_draw]                            */
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
UINT _gx_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     bound;
GX_RECTANGLE     clip_rect;
GX_VIEW         *view;
GX_BRUSH        *brush;
INT              brush_width;
VOID             (*outline_function)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    brush = &context -> gx_draw_context_brush;

    brush_width = (brush -> gx_brush_width + 1) >> 1;

    /* Define ellipse bounding rectangle. */
    _gx_utility_rectangle_define(&bound, (GX_VALUE)(xcenter - a - brush_width), (GX_VALUE)(ycenter - b - brush_width),
                                 (GX_VALUE)(xcenter + a + brush_width), (GX_VALUE)(ycenter + b + brush_width));

    brush_width = brush -> gx_brush_width;

    /* clip the ellipse bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* configure outline function to utilize */
    outline_function = GX_NULL;
    
    /* Determine which outline function to use.*/
    if(brush_width == 1)
    {
        /* if anti-alias is requested and this is supported by display, use it */
        if ((brush -> gx_brush_style & GX_BRUSH_ALIAS) &&
            display -> gx_display_driver_anti_aliased_ellipse_draw != GX_NULL)
        {
            outline_function = display -> gx_display_driver_anti_aliased_ellipse_draw;
        }
        else
        {
            outline_function = display -> gx_display_driver_ellipse_draw;
        }
    }
    else
    {
        if (brush_width > 1)
        {
            /* if anti-alias is requested and this is supported by display, use it */
            if ((brush -> gx_brush_style & GX_BRUSH_ALIAS) &&
                display -> gx_display_driver_anti_aliased_wide_ellipse_draw != GX_NULL)
            {
                outline_function = display -> gx_display_driver_anti_aliased_wide_ellipse_draw;
            }
            else
            {
                outline_function = display -> gx_display_driver_wide_ellipse_draw;
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
        context -> gx_draw_context_clip = &clip_rect;

        if ((brush -> gx_brush_style & (GX_BRUSH_SOLID_FILL | GX_BRUSH_PIXELMAP_FILL)) &&
            display -> gx_display_driver_ellipse_fill)
        {
            /* Call display driver ellipse filling routine.  */
            display -> gx_display_driver_ellipse_fill(context, xcenter, ycenter, a, b);
        }

        if (outline_function)
        {
            outline_function(context, xcenter, ycenter, a, b);
        }

        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}
#endif

