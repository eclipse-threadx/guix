
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
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_canvas.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_mouse_draw               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws the mouse image for software mouse.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
#if defined(GX_MOUSE_SUPPORT)
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
VOID _gx_display_driver_generic_mouse_draw(GX_DISPLAY *display)
{
GX_DRAW_CONTEXT  mouse_context;
GX_DRAW_CONTEXT *old_context;
GX_PIXELMAP     *map;
GX_VALUE         left;
GX_VALUE         top;
GX_RESOURCE_ID   image_id;
GX_CANVAS       *canvas;

    if(display -> gx_display_mouse.gx_mouse_cursor_info)
    {
        image_id = display -> gx_display_mouse.gx_mouse_cursor_info -> gx_mouse_cursor_image_id;

        if (image_id && image_id < display -> gx_display_pixelmap_table_size)
        {
            canvas = display -> gx_display_mouse.gx_mouse_canvas;
            left = display -> gx_display_mouse.gx_mouse_position.gx_point_x;
            top = display -> gx_display_mouse.gx_mouse_position.gx_point_y;
            left = (GX_VALUE)(left - display -> gx_display_mouse.gx_mouse_cursor_info -> gx_mouse_cursor_hotspot_x);
            top = (GX_VALUE)(top - display -> gx_display_mouse.gx_mouse_cursor_info -> gx_mouse_cursor_hotspot_y);
            map = display -> gx_display_pixelmap_table[image_id];

            old_context = _gx_system_current_draw_context;

            // set up draw context clip area
            mouse_context.gx_draw_context_dirty = display -> gx_display_mouse.gx_mouse_rect;

            // set up draw context view
            mouse_context.gx_draw_context_simple_view.gx_view_next = NULL;
            mouse_context.gx_draw_context_simple_view.gx_view_rectangle = display -> gx_display_mouse.gx_mouse_rect;
            mouse_context.gx_draw_context_view_head = &mouse_context.gx_draw_context_simple_view;

            mouse_context.gx_draw_context_canvas = canvas;
            /* Mouse pixelmap should never been blend. */
            mouse_context.gx_draw_context_brush.gx_brush_alpha = 0xff;
            mouse_context.gx_draw_context_display = display;
            mouse_context.gx_draw_context_memory = canvas -> gx_canvas_memory;
            mouse_context.gx_draw_context_pitch = canvas -> gx_canvas_x_resolution;

            _gx_system_current_draw_context = &mouse_context;
            _gx_canvas_pixelmap_draw(left, top, map);

            _gx_system_current_draw_context = old_context;
            display -> gx_display_mouse.gx_mouse_status |= GX_MOUSE_VISIBLE;
        }
    }
}

#endif
#endif

