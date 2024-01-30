/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
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
#include "gx_system.h"
#include "gx_window.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_shift                                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves the canvas associated with a root window        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    x                                     x-axis shift amount in pixels */
/*    y                                     y-axis shift amount in pixels */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   gx_canvas_offset_set                   Set the canvas offset         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_window_root_event_process                                       */
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
UINT  _gx_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y)
{
GX_VALUE xpos;
GX_VALUE ypos;

    xpos = (GX_VALUE)(canvas -> gx_canvas_display_offset_x + x);
    ypos = (GX_VALUE)(canvas -> gx_canvas_display_offset_y + y);

    return(_gx_canvas_offset_set(canvas, xpos, ypos));
}

