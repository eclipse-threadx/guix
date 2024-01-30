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
#include "gx_canvas.h"
#include "gx_port.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_mouse_hide                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors for the canvas mouse hide call.     */
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
/*    _gx_canvas_mouse_hide                 Actual canvas mouse hide      */
/*                                            call                        */
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
UINT _gxe_canvas_mouse_hide(GX_CANVAS *canvas)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for the invalid input pointers.  */
    if ((canvas == GX_NULL) ||
        (canvas -> gx_canvas_display == GX_NULL) ||
        (canvas -> gx_canvas_memory == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Call the actual mouse hotspot set function.  */
    status = _gx_canvas_mouse_hide(canvas);

    /* Return completion  status.  */
    return status;
}
#endif

