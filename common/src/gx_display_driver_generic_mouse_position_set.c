/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
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
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_utility.h"

#if defined(GX_MOUSE_SUPPORT)
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_mouse_position_set       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the mouse position for software mouse support.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*    position                              Position of mouse cursor      */
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
/**************************************************************************/
VOID  _gx_display_driver_generic_mouse_position_set(GX_DISPLAY *display, GX_POINT *pos)
{
GX_RECTANGLE mouse_rect;
GX_CANVAS   *canvas;

    if (display->gx_display_mouse.gx_mouse_cursor_info)
    {
        canvas = display -> gx_display_mouse.gx_mouse_canvas;
        mouse_rect = display -> gx_display_mouse.gx_mouse_rect;

        /* First hide the mouse if the area under mouse has been captured */
        if (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_VISIBLE)
        {
            display -> gx_display_mouse_restore(display);
        }

        display -> gx_display_mouse.gx_mouse_position = *pos;

        if (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_ENABLED)
        {
            display -> gx_display_mouse_capture(display);
            display -> gx_display_mouse_draw(display);
        }

        if (display -> gx_display_driver_buffer_toggle)
        {
            _gx_utility_rectangle_combine(&mouse_rect, &display -> gx_display_mouse.gx_mouse_rect);
            canvas -> gx_canvas_dirty_area = mouse_rect;
            display -> gx_display_driver_buffer_toggle(canvas, &canvas -> gx_canvas_dirty_area);
        }
    }
}
#endif
#endif

