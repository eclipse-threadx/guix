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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_context.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_context_raw_brush_define                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Set the brush of the current dispaly context.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    line_color                            Color of line                 */
/*    fill_color                            Color of fill                 */
/*    style                                 Style of brush                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_brush_define                      Define the brush              */
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
UINT _gx_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style)
{
GX_DISPLAY      *display;
GX_BRUSH        *brush;
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

    display = context -> gx_draw_context_display;

    /* Set the brush.  */
    brush = &context -> gx_draw_context_brush;
    _gx_brush_define(brush,
                     display -> gx_display_driver_native_color_get(display, line_color),
                     display -> gx_display_driver_native_color_get(display, fill_color), style);

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

