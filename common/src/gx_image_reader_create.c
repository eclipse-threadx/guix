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
/*    _gx_image_reader_create                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a runtime raw image reader / decoder.         */
/*    Currently only jpeg and png raw image types are supported.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader              Image reader control block.               */
/*    read_data                 Pointer to raw input data.                */
/*    read_data_size            Size of raw input data.                   */
/*    color_format              The requested output color format         */
/*    mode                      Compress dither and alpha modes flags     */
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
/*    Application Code                                                    */
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
UINT _gx_image_reader_create(GX_IMAGE_READER *image_reader,
                             GX_CONST GX_UBYTE *data,
                             INT data_size,
                             GX_UBYTE color_format,
                             GX_UBYTE mode)
{
    image_reader -> gx_image_reader_color_format     = color_format;
    image_reader -> gx_image_reader_mode             = mode;
    image_reader -> gx_image_reader_image_type       = 0;
    image_reader -> gx_image_reader_source_data      = data;
    image_reader -> gx_image_reader_source_data_size = (UINT)data_size;
    image_reader -> gx_image_reader_getdata          = GX_NULL;
    image_reader -> gx_image_reader_getauxdata       = GX_NULL;
    image_reader -> gx_image_reader_putauxdata       = GX_NULL;
    image_reader -> gx_image_reader_palette          = GX_NULL;
    image_reader -> gx_image_reader_palette_size     = 0;
    image_reader -> gx_image_reader_image_height     = 0;
    image_reader -> gx_image_reader_image_width      = 0;
    image_reader -> gx_image_reader_png_trans        = 0;
    image_reader -> gx_image_reader_png_palette      = GX_NULL;
    image_reader -> gx_image_reader_png_palette_size = 0;
    image_reader -> gx_image_reader_putdata          = GX_NULL;
    image_reader -> gx_image_reader_putauxdata       = GX_NULL;
    image_reader -> gx_image_reader_pixel_read       = GX_NULL;
    image_reader -> gx_image_reader_pixel_write      = GX_NULL;
    image_reader -> gx_image_reader_size_testing     = GX_FALSE;
    image_reader -> gx_image_reader_mono_shreshold   = 128;
    image_reader -> gx_image_reader_putdata_mask     = 0;
    image_reader -> gx_image_reader_putauxdata_mask  = 0;

    return GX_SUCCESS;
}
#endif

