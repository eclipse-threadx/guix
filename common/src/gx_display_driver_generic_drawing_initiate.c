
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
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
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
