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
/**   Image Reader Management (Image Reader)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_image_reader.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_nearest_palette_color_get          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves the nearest palette color.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    want_color                            Wanted color value.           */
/*    index                                 Retrieved color index.        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
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
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
UINT _gx_image_reader_nearest_palette_color_get(GX_IMAGE_READER *image_reader, GX_PIXEL *want_color, INT *index)
{
ULONG     nearest_dist = 0x7fffffff;
ULONG     dist;
GX_UBYTE  red_dist;
GX_UBYTE  green_dist;
GX_UBYTE  blue_dist;
UINT      pal_index;
INT       nearest_index = 0;
GX_COLOR *palette = image_reader -> gx_image_reader_palette;
UINT      palsize = image_reader -> gx_image_reader_palette_size;
GX_PIXEL  pal_color;

    if (palette)
    {
        for (pal_index = 0; pal_index < palsize; pal_index++)
        {
            pal_color.gx_pixel_red = (GX_UBYTE)(palette[pal_index] >> 16);
            pal_color.gx_pixel_green = (GX_UBYTE)(palette[pal_index] >> 8);
            pal_color.gx_pixel_blue = (GX_UBYTE)palette[pal_index];

            red_dist = (GX_UBYTE)GX_ABS(want_color -> gx_pixel_red - pal_color.gx_pixel_red);
            green_dist = (GX_UBYTE)GX_ABS(want_color -> gx_pixel_green - pal_color.gx_pixel_green);
            blue_dist = (GX_UBYTE)GX_ABS(want_color -> gx_pixel_blue - pal_color.gx_pixel_blue);

            dist = (ULONG)((red_dist * red_dist) + (green_dist * green_dist) + (blue_dist * blue_dist));

            if (dist < nearest_dist)
            {
                nearest_dist = dist;
                nearest_index = (INT)pal_index;
            }
        }

        want_color -> gx_pixel_red = (GX_UBYTE)(palette[nearest_index] >> 16);
        want_color -> gx_pixel_green = (GX_UBYTE)(palette[nearest_index] >> 8);
        want_color -> gx_pixel_blue = (GX_UBYTE)palette[nearest_index];

        *index = nearest_index;
    }
    else
    {
        want_color -> gx_pixel_red = 0;
        want_color -> gx_pixel_green = 0;
        want_color -> gx_pixel_blue = 0;
    }

    return GX_SUCCESS;
}
#endif

