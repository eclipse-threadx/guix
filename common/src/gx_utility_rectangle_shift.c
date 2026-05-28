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
/*    _gx_utility_rectangle_shift                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service shifts the rectangle by the specified values.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rectangle                             Rectangle to shift            */
/*    x_shift                               Number of pixels to shift     */
/*                                          on the x-axis                 */
/*    y_shift                               Number of pixels to shift     */
/*                                          on the y-axis                 */
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
UINT  _gx_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift)
{
    rectangle -> gx_rectangle_left   = (GX_VALUE)(rectangle -> gx_rectangle_left + x_shift);
    rectangle -> gx_rectangle_top    = (GX_VALUE)(rectangle -> gx_rectangle_top + y_shift);
    rectangle -> gx_rectangle_right  = (GX_VALUE)(rectangle -> gx_rectangle_right + x_shift);
    rectangle -> gx_rectangle_bottom = (GX_VALUE)(rectangle -> gx_rectangle_bottom + y_shift);
    return GX_SUCCESS;
}

