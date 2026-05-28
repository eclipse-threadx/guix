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
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_rectangle_center                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service centers the rectangle within another rectangle.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rectangle                             Rectangle to center           */
/*    within                                Rectangle to center within    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within)
{

INT x_space;
INT y_space;
INT x_delta;
INT y_delta;

    x_space = (((within -> gx_rectangle_right -
                 within -> gx_rectangle_left) + 1) -
               ((rectangle -> gx_rectangle_right -
                 rectangle -> gx_rectangle_left) + 1)) / 2;

    y_space = (((within -> gx_rectangle_bottom -
                 within -> gx_rectangle_top) + 1) -
               ((rectangle -> gx_rectangle_bottom -
                 rectangle -> gx_rectangle_top) + 1)) / 2;

    x_delta = within -> gx_rectangle_left + x_space - rectangle -> gx_rectangle_left;
    y_delta = within -> gx_rectangle_top + y_space - rectangle -> gx_rectangle_top;

    rectangle -> gx_rectangle_left = (GX_VALUE)(rectangle -> gx_rectangle_left + x_delta);
    rectangle -> gx_rectangle_top = (GX_VALUE)(rectangle -> gx_rectangle_top + y_delta);
    rectangle -> gx_rectangle_right = (GX_VALUE)(rectangle -> gx_rectangle_right + x_delta);
    rectangle -> gx_rectangle_bottom = (GX_VALUE)(rectangle -> gx_rectangle_bottom + y_delta);
    return GX_SUCCESS;
}

