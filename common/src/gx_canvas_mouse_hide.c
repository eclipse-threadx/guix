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
/*    _gx_canvas_mouse_hide                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service makes the mouse hidden.                                */
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
/**************************************************************************/
#if defined(GX_MOUSE_SUPPORT)
UINT _gx_canvas_mouse_hide(GX_CANVAS *canvas)
{
GX_DISPLAY           *display;

    display = canvas -> gx_canvas_display;

    if (display -> gx_display_mouse.gx_mouse_canvas == canvas && (display -> gx_display_mouse_enable != GX_NULL))
    {
        display -> gx_display_mouse_enable(display, GX_FALSE);
        return GX_SUCCESS;
    }

    return GX_FAILURE;
}
#endif

