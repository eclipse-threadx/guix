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
/*    _gx_canvas_pixelmap_draw                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares to draw the specified pixelmap at the        */
/*    requested position.                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_position                            Top-left x-coord to place     */
/*                                            pixelmap                    */
/*    y_position                            Top-left y-coord to place     */
/*                                            pixelmap                    */
/*    pixelmap                              Pointer to actual pixelmap    */
/*                                            to draw                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    [gx_display_driver_pixelmap_draw]     The display driver pixelmap   */
/*                                            draw routine                */
/*    [gx_display_driver_jpeg_draw]         The display driver JPEG draw  */
/*                                            routine                     */
/*    [gx_display_driver_png_draw]          The display driver PNG draw   */
/*                                            routine                     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_pixelmap_blend                                           */
/*    _gx_canvas_pixelmap_tile                                            */
/*    _gx_checkbox_draw                                                   */
/*    _gx_icon_button_draw                                                */
/*    _gx_icon_draw                                                       */
/*    _gx_pixelmap_button_draw                                            */
/*    _gx_pixelmap_prompt_draw                                            */
/*    _gx_pixelmap_slider_draw                                            */
/*    _gx_radio_button_draw                                               */
/*    _gx_scroll_thumb_draw                                               */
/*    _gx_scrollbar_draw                                                  */
/*    _gx_window_draw                                                     */
/*    _gx_scroll_thumb_draw                                               */
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
UINT  _gx_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     clip_rect;
GX_RECTANGLE     bound;
GX_VIEW         *view;
VOID             (*pmp_function)(GX_DRAW_CONTEXT *, INT, INT, GX_PIXELMAP *);

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

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

    /* pickup pointer to correct pixelmap drawing function */
    pmp_function = GX_NULL;

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_RAW_FORMAT)
    {
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
        if ((pixelmap->gx_pixelmap_data[0] == 0xff) && (pixelmap->gx_pixelmap_data[1] == 0xd8))
        {
            /* JPEG */
            pmp_function = display -> gx_display_driver_jpeg_draw;
        }
        else
        {
            if (pixelmap -> gx_pixelmap_data[1] == 'P')
            {
                /* PNG */
                pmp_function = display -> gx_display_driver_png_draw;
            }
        }
#endif
    }
    else
    {
        if (pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_8BIT_ALPHAMAP)
        {
            pmp_function = display -> gx_display_driver_alphamap_draw;
        }
        else
        {
            pmp_function = display -> gx_display_driver_pixelmap_draw;
        }
    }

    if (!pmp_function)
    {
        /* display driver does not support requested action */
        return GX_FAILURE;
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

        /* we have a view into which we can draw the pixelmap, do it */

        /* first, set the context clip rectangle */
        context -> gx_draw_context_clip = &clip_rect;

        /* now pass the context and drawing params to driver level function */
        pmp_function(context, x_position, y_position, pixelmap);

        /* go to the next view */
        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

