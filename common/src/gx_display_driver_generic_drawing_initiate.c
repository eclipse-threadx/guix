
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
#include "gx_canvas.h"
#include "gx_utility.h"

#if defined(GX_MOUSE_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_drawing_initiate         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initiates drawing on the specified display. If mouse  */
/*    support is enable, this function implements hiding the mouse cursor */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*    canvas                                Canvas control block          */
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
#if !defined (GX_HARDWARE_MOUSE_SUPPORT)
VOID _gx_display_driver_generic_drawing_initiate(GX_DISPLAY *display, GX_CANVAS *canvas)
{
GX_RECTANGLE    mouse_rect;
GX_CANVAS      *mouse_canvas;

    if (display -> gx_display_mouse.gx_mouse_cursor_info)
    {
        mouse_canvas = display -> gx_display_mouse.gx_mouse_canvas;
        if (canvas == mouse_canvas)
        {
            if (canvas -> gx_canvas_draw_nesting == 1 && (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_VISIBLE))
            {
                if (_gx_utility_rectangle_overlap_detect(&display -> gx_display_mouse.gx_mouse_rect, &canvas -> gx_canvas_dirty_area, &mouse_rect))
                {
                    /* restore captured data */
                    display -> gx_display_mouse_restore(display);
                }
            }
        }
    }
}
#endif
#endif
