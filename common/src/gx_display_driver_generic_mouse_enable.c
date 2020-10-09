
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
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_mouse_enable             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the mouse status for software mouse support.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*    enable                                Mouse status                  */
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
VOID _gx_display_driver_generic_mouse_enable(GX_DISPLAY *display, GX_BOOL enable)
{
    if (enable)
    {
        if (!(display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_ENABLED))
        {
            display -> gx_display_mouse.gx_mouse_status |= GX_MOUSE_ENABLED;

            /* reset to current position to force redraw */
            display -> gx_display_mouse_position_set(display, &display -> gx_display_mouse.gx_mouse_position);
        }
    }
    else
    {
        if (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_ENABLED)
        {
            display -> gx_display_mouse.gx_mouse_status &= (GX_UBYTE)(~GX_MOUSE_ENABLED);

            if (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_VISIBLE)
            {
                display -> gx_display_mouse_restore(display);
            }
        }
    }
}
#endif
#endif

