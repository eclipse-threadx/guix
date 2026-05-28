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
/*    _gx_context_brush_get                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Get the brush of the current drawing context.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    return_brush                          Pointer to destination for    */
/*                                            brush                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_single_line_text_input_cursor_dirty_mark                        */
/*    _gx_single_line_text_input_draw                                     */
/*    _gx_widget_text_draw                                                */
/*                                                                        */
/**************************************************************************/
UINT  _gx_context_brush_get(GX_BRUSH **return_brush)
{
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

    /* Get the current brush.  */
    *return_brush = &(context -> gx_draw_context_brush);

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

