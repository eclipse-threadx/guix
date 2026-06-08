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
/*    _gx_utility_rectangle_inside_detect                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Detect if a second rectangle is completely within the first.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    outer                                 First rectangle               */
/*    Inner                                 Second rectangle              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    [GX_TRUE | GX_FALSE]                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
GX_BOOL  _gx_utility_rectangle_inside_detect(GX_RECTANGLE *outer, GX_RECTANGLE *inner)
{
    if (inner -> gx_rectangle_left >= outer -> gx_rectangle_left &&
        inner -> gx_rectangle_top >= outer -> gx_rectangle_top &&
        inner -> gx_rectangle_right <= outer -> gx_rectangle_right &&
        inner -> gx_rectangle_bottom <= outer -> gx_rectangle_bottom)
    {
        return GX_TRUE;
    }
    return GX_FALSE;
}

