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
/*    _gx_canvas_pixel_draw                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares to draw one pixel using current context      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    position                              x,y coordinate to draw        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
/*    [gx_display_driver_pixel_write]       Actually write to canvas      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT  _gx_canvas_pixel_draw(GX_POINT position)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_VIEW         *view;
GX_COLOR         pixcolor;
GX_UBYTE         brush_alpha;

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    /* test to see if we can draw at this position */
    if (!_gx_utility_rectangle_point_detect(&context -> gx_draw_context_dirty, position))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* pick up the pixel color */
    pixcolor = context -> gx_draw_context_brush.gx_brush_line_color;

    /* test to determine if any viewport of the caller overlaps this pixel.
       For each view that overlaps the bounding rectangle, do some drawing.
     */
    view = context -> gx_draw_context_view_head;
#if defined(GX_BRUSH_ALPHA_SUPPORT)
    if (brush_alpha == 0)
    {
        return GX_SUCCESS;
    }
    while (view)
    {
        if (_gx_utility_rectangle_point_detect(&view -> gx_view_rectangle, position))
        {
            if (brush_alpha == 0xff)
            {
                if (display -> gx_display_driver_pixel_write)
                {
                    display -> gx_display_driver_pixel_write(context, position.gx_point_x, position.gx_point_y, pixcolor);
                }
            }
            else
            {
                if (display -> gx_display_driver_pixel_blend)
                {
                    display -> gx_display_driver_pixel_blend(context, position.gx_point_x, position.gx_point_y, pixcolor, brush_alpha);
                }
            }
            break;
        }
        view = view->gx_view_next;
    }
#else
    while (view)
    {
        if (_gx_utility_rectangle_point_detect(&view -> gx_view_rectangle, position))
        {
            if (display -> gx_display_driver_pixel_write)
            {
                display -> gx_display_driver_pixel_write(context, position.gx_point_x, position.gx_point_y, pixcolor);
            }
            break;
        }
        view = view->gx_view_next;
    }
#endif

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

