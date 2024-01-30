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
#include "gx_canvas.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_canvas_line_draw                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the canvas line draw function.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_start                               x-coord of endpoint1          */
/*    y_start                               y-coord of endpoint1          */
/*    x_end                                 x-coord of endpoint2          */
/*    y_end                                 y-coord of endpoint2          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_line_draw                  The actual function           */
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
UINT  _gxe_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end)
{
UINT             status;
GX_DRAW_CONTEXT *context;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;
    if (context == GX_NULL)
    {
        return GX_INVALID_CONTEXT;
    }

    /* Call the actual function.  */
    status = _gx_canvas_line_draw(x_start, y_start, x_end, y_end);

    /* Return successful completion.  */
    return(status);
}

