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
/*    _gx_display_driver_24xrgb_rotated_mcu_draw          PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw one MCU of decoded JPEG data to    */
/*    rotated canvas.                                                     */
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
/*    _gx_display_driver_24xrgb_YCbCr2RGB                                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_rotated_jpeg_decode                       */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
static UINT _gx_display_driver_24xrgb_rotated_mcu_draw(GX_JPEG_INFO *jpeg_info, INT xpos, INT ypos)
{
GX_COLOR        *put;
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
INT              rotated_xpos;
INT              rotated_ypos;
INT              sign;

    h = (jpeg_info -> gx_jpeg_sample_factor[0] >> 4);
    w = (jpeg_info -> gx_jpeg_sample_factor[0] & 0x0f);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_xpos = ypos;
        rotated_ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - xpos - 1;
        sign = 1;
    }
    else
    {
        rotated_xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - ypos - 1;
        rotated_ypos = xpos;
        sign = -1;
    }

    put = context -> gx_draw_context_memory;
    put += rotated_ypos * context -> gx_draw_context_pitch;
    put += rotated_xpos;


    for (x = 0; x < 8 * w; x++)
    {
        for (y = 0; y < 8 * h; y++)
        {

            if ((xpos + x >= clip -> gx_rectangle_left) && (xpos + x <= clip -> gx_rectangle_right) &&
                (ypos + y >= clip -> gx_rectangle_top) && (ypos + y <= clip -> gx_rectangle_bottom))
            {
                coff = x / w + ((y / h) << 3);

                Y = jpeg_info -> gx_jpeg_Y_block[x + y * w * 8];
                Cb = jpeg_info -> gx_jpeg_Cb_block[coff];
                Cr = jpeg_info -> gx_jpeg_Cr_block[coff];

                *(put + y * sign) = (_gx_display_driver_24xrgb_YCbCr2RGB(Y, Cb, Cr) | 0xff000000);
            }
        }

        put -= context -> gx_draw_context_pitch * sign;
    }

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dislay_driver_24xrgb_rotated_jpeg_draw          PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    24xrgb rotated screen driver JPEG image draw routine.               */
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
/*    [_gx_display_driver_24xrgb_rotated_mcu_draw]                        */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
VOID _gx_display_driver_24xrgb_rotated_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{

    _gx_image_reader_jpeg_mcu_decode(pixelmap -> gx_pixelmap_data,
                                     pixelmap -> gx_pixelmap_data_size,
                                     context, xpos, ypos,
                                     _gx_display_driver_24xrgb_rotated_mcu_draw);
}
#endif

