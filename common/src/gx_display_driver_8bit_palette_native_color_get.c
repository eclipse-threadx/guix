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
/*    _gx_display_driver_8bit_palette_native_color_get    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    8-bit palette mode display driver color conversion routine.         */
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
GX_COLOR _gx_display_driver_8bit_palette_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor)
{
ULONG     nearest_dist = 0x7fffffff;
ULONG     dist;
INT       red_dist;
INT       green_dist;
INT       blue_dist;
GX_COLOR  pal_color;
INT       pal_index;
INT       nearest_index = 0;
GX_COLOR *palette = display -> gx_display_palette;
INT       palsize = (INT)(display -> gx_display_palette_size);

    if (palette && palsize)
    {
        for (pal_index = 0; pal_index < palsize; pal_index++)
        {
            pal_color = palette[pal_index];
            red_dist = (INT)(((rawcolor >> 16) & 0xff) - ((pal_color >> 16) & 0xff));
            green_dist = (INT)(((rawcolor >> 8) & 0xff) - ((pal_color >> 8) & 0xff));
            blue_dist = (INT)((rawcolor & 0xff) - (pal_color & 0xff));

            dist = (ULONG)((red_dist * red_dist) + (green_dist * green_dist) + (blue_dist * blue_dist));

            if (dist < nearest_dist)
            {
                nearest_dist = dist;
                nearest_index = pal_index;
            }
        }
    }
    return((GX_COLOR)nearest_index);
}

