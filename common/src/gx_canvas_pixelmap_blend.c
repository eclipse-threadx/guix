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
/**   Screen Management (Screen)                                          */
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
/*    _gx_canvas_pixelmap_blend                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares to blend the specified pixelmap with         */
/*      background at the requested position.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_position                            Top-left x-coord to place     */
/*                                            pixelmap                    */
/*    y_position                            Top-left y-coord to place     */
/*                                            pixelmap                    */
/*    pixelmap                              Pointer to actual pixelmap    */
/*                                            to draw                     */
/*    alpha                                 blending value 0-255          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    [gx_display_driver_pixelmap_draw]     Driver level pixelmap blend   */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_sprite_draw                                                     */
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
UINT  _gx_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position,
                                GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     clip_rect;
GX_RECTANGLE     bound;
GX_VIEW         *view;
GX_UBYTE         old_alpha;
VOID             (*pmp_function)(GX_DRAW_CONTEXT *, INT, INT, GX_PIXELMAP *);

    if (alpha == 0)
    {
        return GX_SUCCESS;
    }

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    /* calculate rectangle that bounds the pixelmap */
    _gx_utility_rectangle_define(&bound, x_position, y_position,
                                 (GX_VALUE)(x_position + pixelmap -> gx_pixelmap_width - 1),
                                 (GX_VALUE)(y_position + pixelmap -> gx_pixelmap_height - 1));

    /* clip the line bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* pickup pointer to correct pixelmap blending function */
    if (pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_8BIT_ALPHAMAP)
    {
        pmp_function = display -> gx_display_driver_alphamap_draw;
    }
    else
    {
        pmp_function = display -> gx_display_driver_pixelmap_draw;
    }

    if (pmp_function == GX_NULL)
    {
        return GX_NOT_SUPPORTED;
    }

    /* Set the parameter alpha to brush alpha value. */
    old_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    context -> gx_draw_context_brush.gx_brush_alpha = alpha;

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

        /* we have a view into which we can draw the pixelmap, do it */
        /* first, set the context clip rectangle */
        context -> gx_draw_context_clip = &clip_rect;

        /* now pass the context and drawing params to driver level function */
        pmp_function(context, x_position, y_position, pixelmap);

        /* go to the next view */
        view = view -> gx_view_next;
    }

    context -> gx_draw_context_brush.gx_brush_alpha = old_alpha;

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

