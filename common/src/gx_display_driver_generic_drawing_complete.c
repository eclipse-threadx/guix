
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

#if defined(GX_MOUSE_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_drawing_complete         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function provides default drawing complete operatoin for       */
/*    generic display driver instance. Performs mouse cursor drawing      */
/*    if mouse support is enabled.                                        */
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
VOID _gx_display_driver_generic_drawing_complete(GX_DISPLAY *display, GX_CANVAS *canvas)
{
GX_CANVAS      *mouse_canvas;
    
    if (display -> gx_display_mouse.gx_mouse_cursor_info)
    {
        mouse_canvas = display -> gx_display_mouse.gx_mouse_canvas;

        if (canvas == mouse_canvas)
        {
            if (canvas -> gx_canvas_draw_nesting == 1)
            {
                if ((display -> gx_display_mouse.gx_mouse_status & (GX_MOUSE_ENABLED | GX_MOUSE_VISIBLE)) == GX_MOUSE_ENABLED)
                {
                    display -> gx_display_mouse_capture(display);
                    display -> gx_display_mouse_draw(display);
                }
            }
        }
    }
}
#endif
#endif

