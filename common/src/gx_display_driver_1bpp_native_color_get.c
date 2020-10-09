/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


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
/*    _gx_display_driver_1bpp_native_color_get            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1-bit display driver color conversion routine                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
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
GX_COLOR _gx_display_driver_1bpp_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor)
{
    GX_PARAMETER_NOT_USED(display);

    if ((((rawcolor >> 16) & 0xff) < 0x80) &&
        (((rawcolor >> 8) & 0xff) < 0x80)  &&
        ((rawcolor & 0xff) < 0x80))
    {
        return 0x0;
    }
    else
    {
        return 0x1;
    }
}

