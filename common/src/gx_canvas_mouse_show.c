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
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_mouse_show                               PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service makes the mouse visible.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_mouse_enable]             enabled or disabled mouse     */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added canvas status check,  */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_MOUSE_SUPPORT)
UINT _gx_canvas_mouse_show(GX_CANVAS *canvas)
{
GX_DISPLAY           *display;
GX_MOUSE_CURSOR_INFO *mouse_info;

#if defined(GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER)
    if (canvas -> gx_canvas_status & GX_CANVAS_PARTIAL_FRAME_BUFFER)
    {
        /* Not supported.  */
        return GX_FAILURE;
    }
#endif

    display = canvas -> gx_canvas_display;
    mouse_info = display -> gx_display_mouse.gx_mouse_cursor_info;

    if ((mouse_info != GX_NULL) && (display -> gx_display_mouse_enable != GX_NULL))
    {
        if (display -> gx_display_mouse.gx_mouse_canvas == canvas)
        {
            display -> gx_display_mouse_enable(display, GX_TRUE);
            return GX_SUCCESS;
        }
    }
    return GX_FAILURE;
}
#endif

