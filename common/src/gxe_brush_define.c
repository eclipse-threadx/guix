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
/**   Utility (Utility)                                                   */
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
/*    _gxe_brush_define                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the brush define function call.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    brush                                 Pointer to brush              */
/*    line_color                            Color of brush line           */
/*    fill_color                            Color of brush fill           */
/*    style                                 Brush style                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_brush_define                      Actual brush define call      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT  _gxe_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style)
{
UINT status;

    /* Check for invalid input pointers.  */
    if (brush == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Call the actual brush define function.  */
    status = _gx_brush_define(brush, line_color, fill_color, style);

    /* Return completion status.  */
    return status;
}

