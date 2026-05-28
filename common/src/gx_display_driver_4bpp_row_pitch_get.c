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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_row_pitch_get               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Calculate row pitch, in bytes, for given canvas width               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    width                              canvas width                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    USHORT                             pitch in bytes                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixel_write                                 */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
USHORT _gx_display_driver_4bpp_row_pitch_get(USHORT width)
{
    return((USHORT)((width + 1) >> 1));
}

