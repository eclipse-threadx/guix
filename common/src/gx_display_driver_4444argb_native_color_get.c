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
/*    _gx_display_driver_4444argb_native_color_get        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    16-bit 4444argb format display driver color conversion routine      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               The associated display        */
/*    rawcolor                              Raw Color value to be         */
/*                                            converted                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_COLOR                              Color value in the screen     */
/*                                            color format                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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
GX_COLOR _gx_display_driver_4444argb_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor)
{
GX_COLOR native;

    GX_PARAMETER_NOT_USED(display);

    native = (GX_COLOR)(((rawcolor & 0xf0000000) >> 16) | ((rawcolor & 0x00f00000) >> 12) | ((rawcolor & 0x00f000) >> 8) | ((rawcolor & 0x000000f0) >> 4));
    return(native);
}

