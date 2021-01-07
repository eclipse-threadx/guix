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
#include "gx_context.h"
#include "gx_system.h"
#include "gx_image_reader.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_YCbCr2RGB                 PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Converts YCbCr value to 565RGB color space.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    y                                     Luminance                     */
/*    cb                                    Chroma (Blue-difference)      */
/*    cr                                    Chroma (Red-difference)       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Value                                 565 RGB value                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_565rgb_one_mcu_draw                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            made function public,       */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
USHORT _gx_display_driver_565rgb_YCbCr2RGB(INT y, INT cb, INT cr)
{
INT red;
INT green;
INT blue;

    cb = cb - 128;
    cr = cr - 128;

    red = y + cr + (cr >> 2) + (cr >> 3);
    green = y - ((cb >> 2) + (cb >> 4) + (cb >> 5)) - ((cr >> 1) + (cr >> 3) + (cr >> 4) + (cr >> 6));
    blue = y + cb + (cb >> 1) + (cb >> 2);


    /* Make sure the range of the RGB values are within bound. */
    if (red > 255)
    {
        red = 31;
    }
    else if (red < 0)
    {
        red = 0;
    }
    else
    {
        red = red >> 3;
    }

    if (green > 255)
    {
        green = 63;
    }
    else if (green < 0)
    {
        green = 0;
    }
    else
    {
        green = green >> 2;
    }

    if (blue > 255)
    {
        blue = 31;
    }
    else if (blue < 0)
    {
        blue = 0;
    }
    else
    {
        blue = blue >> 3;
    }

    return (USHORT)((red << 11) | (green << 5 | blue));
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_jpeg_16bpp_mcu_draw              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw one MCU of decoded JPEG data.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               GUIX draw context             */
/*    xpos                                  X position to draw            */
/*    ypos                                  y position to draw            */
/*    jpeg_info                             JPEG control block            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_YCbCr2RGB                                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_jpeg_decode                                      */
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
static UINT _gx_display_driver_565rgb_mcu_draw(GX_JPEG_INFO *jpeg_info, INT xpos, INT ypos)
{
USHORT          *put;
INT              x;
INT              y;
INT              w;
INT              h;
INT              coff;
INT              Y;
INT              Cb;
INT              Cr;

GX_DRAW_CONTEXT *context = jpeg_info -> gx_jpeg_draw_context;
GX_RECTANGLE    *clip = context -> gx_draw_context_clip;

    h = (jpeg_info -> gx_jpeg_sample_factor[0] >> 4);
    w = (jpeg_info -> gx_jpeg_sample_factor[0] & 0x0f);

    put = (USHORT *)context -> gx_draw_context_memory;
    put += ypos * context -> gx_draw_context_pitch;
    put += xpos;

    for (y = 0; y < 8 * w; y++)
    {
        for (x = 0; x < 8 * h; x++)
        {

            if ((xpos + x >= clip -> gx_rectangle_left) && (xpos + x <= clip -> gx_rectangle_right) &&
                (ypos + y >= clip -> gx_rectangle_top) && (ypos + y <= clip -> gx_rectangle_bottom))
            {
                coff = x / w + ((y / h) << 3);

                Y = jpeg_info -> gx_jpeg_Y_block[x + y * w * 8];
                Cb = jpeg_info -> gx_jpeg_Cb_block[coff];
                Cr = jpeg_info -> gx_jpeg_Cr_block[coff];

                put[x] = _gx_display_driver_565rgb_YCbCr2RGB(Y, Cb, Cr);
            }
        }

        put += context -> gx_draw_context_pitch;
    }

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dislay_driver_565rgb_jpeg_draw                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb screen driver JPEG image draw routine.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_mcu_decode                                    */
/*    [_gx_display_driver_565rgb_mcu_draw]                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
VOID _gx_display_driver_565rgb_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{

    _gx_image_reader_jpeg_mcu_decode(pixelmap -> gx_pixelmap_data,
                                     pixelmap -> gx_pixelmap_data_size,
                                     context, xpos, ypos,
                                     _gx_display_driver_565rgb_mcu_draw);
}
#endif

