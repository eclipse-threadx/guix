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
#include "gx_display.h"
#include "gx_image_reader.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_rotated_png_draw          PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Rotated 24xrgb format screen driver PNG drawing function.           */
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
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_create                                             */
/*    _gx_image_reader_start                                              */
/*    _gx_display_driver_24xrgb_rotated_pixelmap_draw                     */
/*    _gx_system_memory_free                                              */
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
VOID _gx_display_driver_24xrgb_rotated_png_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_IMAGE_READER image_reader;
GX_PIXELMAP     pic_outmap;

GX_UBYTE        mode = GX_IMAGE_READER_MODE_ALPHA;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        mode |= GX_IMAGE_READER_MODE_ROTATE_CW;
    }
    else
    {
        mode |= GX_IMAGE_READER_MODE_ROTATE_CCW;
    }

    _gx_image_reader_create(&image_reader,
                            pixelmap -> gx_pixelmap_data,
                            (INT)pixelmap -> gx_pixelmap_data_size,
                            GX_COLOR_FORMAT_24XRGB, mode);

    if (_gx_image_reader_start(&image_reader, &pic_outmap) == GX_SUCCESS)
    {
        _gx_display_driver_32bpp_rotated_pixelmap_draw(context, xpos, ypos, &pic_outmap);

        _gx_system_memory_free((VOID *)pic_outmap.gx_pixelmap_data);
    }
}
#endif

