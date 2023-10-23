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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_image_reader.h"

#if defined(GX_SOFTWARE_DECODER_SUPPORT)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_rgb2gray                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function converts RGB value to grayscale.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pixel                                 RGB value                     */
/*    gray                                  Grayscale value to return     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
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
VOID _gx_image_reader_rgb2gray(GX_PIXEL *pixel, GX_UBYTE *gray)
{
    (*gray) = (GX_UBYTE)((pixel -> gx_pixel_red * 299 +
                          pixel -> gx_pixel_green * 587 +
                          pixel -> gx_pixel_blue * 114) / 1000);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_gray_threshold_calculate           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calcualtes thredshold value used to create monochrome */
/*    pixlemap.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_image_reader_pixel_read]          Pixel read callback           */
/*    _gx_image_reader_rgb2gray             Conver RGB value to grayscale */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_start                                              */
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
static VOID _gx_image_reader_gray_threshold_calculate(GX_IMAGE_READER *image_reader)
{
INT      threshold = 0;
INT      xval;
INT      yval;
INT      count = 0;
GX_PIXEL pixel;
GX_UBYTE gray;

    for (yval = 0; yval < (INT)image_reader -> gx_image_reader_image_height; yval++)
    {
        for (xval = 0; xval < (INT)image_reader -> gx_image_reader_image_width; xval++)
        {
            image_reader -> gx_image_reader_pixel_read(image_reader, xval, &pixel);

            if (pixel.gx_pixel_alpha > 128)
            {
                /* Convert RGB value to grayscale. */
                _gx_image_reader_rgb2gray(&pixel, &gray);
                threshold += gray;
                count++;
            }
        }

        image_reader -> gx_image_reader_getdata += image_reader -> gx_image_reader_input_stride;
    }

    if (count)
    {
        image_reader -> gx_image_reader_mono_shreshold = (GX_UBYTE)(threshold / count);
    }

    if (image_reader -> gx_image_reader_mono_shreshold == 0)
    {
        /* All opaque pixels are black. */
        image_reader -> gx_image_reader_mono_shreshold = 255;
    }
    else if (image_reader -> gx_image_reader_mono_shreshold == 255)
    {
        /* All opqaute pixels are white. */
        image_reader -> gx_image_reader_mono_shreshold = 0;
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_pixelmap_info_set                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares for image converting.                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    outmap                                Pointer to a pixelmap that is */
/*                                            used to loading converted   */
/*                                            image                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_start                                              */
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
static UINT _gx_image_reader_pixelmap_info_set(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
    outmap -> gx_pixelmap_width = (GX_VALUE)image_reader -> gx_image_reader_image_width;
    outmap -> gx_pixelmap_height = (GX_VALUE)image_reader -> gx_image_reader_image_height;
    outmap -> gx_pixelmap_data_size = 0;
    outmap -> gx_pixelmap_aux_data_size = 0;
    outmap -> gx_pixelmap_format = image_reader -> gx_image_reader_color_format;

    /* Set alpha flag.  */
    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        switch (outmap -> gx_pixelmap_format)
        {
        case GX_COLOR_FORMAT_8BIT_PALETTE:
        case GX_COLOR_FORMAT_4BIT_GRAY:
        case GX_COLOR_FORMAT_MONOCHROME:
            outmap -> gx_pixelmap_flags = GX_PIXELMAP_TRANSPARENT;
            outmap -> gx_pixelmap_transparent_color = GX_TRANSPARENT_COLOR;
            break;

        default:
            outmap -> gx_pixelmap_flags = GX_PIXELMAP_ALPHA;
            break;
        }
    }

    /* Calculate pixelmap data size, which will be used for memory allocation lator. Max width and height is limited to 14 bits,
       so overflow cannot occur. */
    switch (outmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_1555XRGB:
        outmap -> gx_pixelmap_data_size = (ULONG)(outmap -> gx_pixelmap_width * outmap -> gx_pixelmap_height * (INT)sizeof(USHORT));

        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            outmap -> gx_pixelmap_aux_data_size = (ULONG)(outmap -> gx_pixelmap_width * outmap -> gx_pixelmap_height);
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        outmap -> gx_pixelmap_data_size = (ULONG)(outmap -> gx_pixelmap_width * outmap -> gx_pixelmap_height * (INT)sizeof(USHORT));
        break;

    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        outmap -> gx_pixelmap_data_size = (ULONG)(outmap -> gx_pixelmap_width * outmap -> gx_pixelmap_height * (INT)sizeof(GX_COLOR));
        image_reader -> gx_image_reader_mode = (GX_UBYTE)(image_reader -> gx_image_reader_mode & (~GX_IMAGE_READER_MODE_DITHER));
        break;

    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        outmap -> gx_pixelmap_data_size = (ULONG)(outmap -> gx_pixelmap_width * outmap -> gx_pixelmap_height);
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        outmap -> gx_pixelmap_data_size = (ULONG)(((outmap -> gx_pixelmap_width + 1) >> 1) * outmap -> gx_pixelmap_height);
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            outmap -> gx_pixelmap_aux_data_size = (ULONG)(((outmap -> gx_pixelmap_width + 7) >> 3) * outmap -> gx_pixelmap_height);
        }
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            outmap -> gx_pixelmap_data_size = (ULONG)(((outmap -> gx_pixelmap_width + 3) >> 2) * outmap -> gx_pixelmap_height);
        }
        else
        {
            outmap -> gx_pixelmap_data_size = (ULONG)(((outmap -> gx_pixelmap_width + 7) >> 3) * outmap -> gx_pixelmap_height);
        }
        break;

    default:
        return GX_NOT_SUPPORTED;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_start                              PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function converts pixelmap to a specified color format.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    outmap                                Output pixelmap               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memcpy                                                              */
/*    memset                                                              */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
/*    _gx_image_reader_image_decode         Decode specified image        */
/*    _gx_image_reader_pixel_read_callback_set                            */
/*                                          Set pixel read callback       */
/*    _gx_image_reader_pixel_write_callback_set                           */
/*                                          Set pixel write callback      */
/*    _gx_image_reader_dither               Process dither algorithm      */
/*    _gx_image_reader_rle_encode           Compress image with RLE       */
/*                                            algorithm                   */
/*    _gx_image_reader_raw_convert          Convert raw format map to     */
/*                                            specified format            */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT _gx_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
UINT        status;
GX_PIXELMAP srcmap;
GX_PIXELMAP tempmap;
UINT        compressed_data_size;
UINT        compressed_aux_size;
UINT        raw_size;
GX_BOOL     do_compress;

    memset(outmap, 0, sizeof(GX_PIXELMAP));
    memset(&srcmap, 0, sizeof(GX_PIXELMAP));

    if (!_gx_system_memory_allocator)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* Decode input image to raw format. */
    status = _gx_image_reader_image_decode(image_reader, &srcmap);

    if (status == GX_SUCCESS)
    {
        /* Set output pixelmap information. */
        image_reader -> gx_image_reader_image_width = (UINT)srcmap.gx_pixelmap_width;
        image_reader -> gx_image_reader_image_height = (UINT)srcmap.gx_pixelmap_height;

        status = _gx_image_reader_pixelmap_info_set(image_reader, outmap);
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_COMPRESS)
    {
        do_compress = GX_TRUE;
    }
    else
    {
        do_compress = GX_FALSE;
    }

    if((image_reader -> gx_image_reader_image_type == GX_IMAGE_TYPE_JPG && srcmap.gx_pixelmap_format == GX_IMAGE_FORMAT_24BPP) ||
       (image_reader -> gx_image_reader_image_type == GX_IMAGE_TYPE_PNG))
    {

        if (status == GX_SUCCESS)
        {
            if (image_reader -> gx_image_reader_color_format == GX_COLOR_FORMAT_MONOCHROME)
            {
                status = _gx_image_reader_pixel_read_callback_set(image_reader, &srcmap);
                _gx_image_reader_gray_threshold_calculate(image_reader);
            }
        }

        image_reader -> gx_image_reader_mode = (GX_UBYTE)(image_reader -> gx_image_reader_mode & (~GX_IMAGE_READER_MODE_COMPRESS));

        /* Color Space Convert.  */
        if (status == GX_SUCCESS)
        {
            tempmap = *outmap;

            status = _gx_image_reader_pixel_read_callback_set(image_reader, &srcmap);
        }

        if (status == GX_SUCCESS)
        {
            status = _gx_image_reader_pixel_write_callback_set(image_reader, &tempmap);
        }

        if (status == GX_SUCCESS)
        {
            status = _gx_image_reader_colorspace_convert(image_reader, &tempmap);

            _gx_system_memory_free((VOID *)srcmap.gx_pixelmap_data);

            if (srcmap.gx_pixelmap_aux_data)
            {
                _gx_system_memory_free((VOID *)srcmap.gx_pixelmap_aux_data);
            }

            srcmap = tempmap;
        }
    }

    /* Compare compressed size and raw size. */
    if (do_compress)
    {
        image_reader -> gx_image_reader_mode |= GX_IMAGE_READER_MODE_COMPRESS;

        /* Seet pixel read and write callback.  */
        if (status == GX_SUCCESS)
        {
            status = _gx_image_reader_pixel_read_callback_set(image_reader, &srcmap);
        }

        if (status == GX_SUCCESS)
        {
            tempmap = *outmap;
            tempmap.gx_pixelmap_data_size = 0;
            tempmap.gx_pixelmap_aux_data_size = 0;
            tempmap.gx_pixelmap_flags |= GX_PIXELMAP_COMPRESSED;

            status = _gx_image_reader_pixel_write_callback_set(image_reader, &tempmap);
        }

        /* Calculate the storage size that needed for rle encode. */
        if (status == GX_SUCCESS)
        {
            _gx_image_reader_rle_encode_size_get(image_reader, &compressed_data_size, &compressed_aux_size);

            raw_size = outmap -> gx_pixelmap_aux_data_size + outmap -> gx_pixelmap_data_size;

            /* Test wheather the encoded data size is smaller that raw size.  */
            if (compressed_aux_size + compressed_data_size < raw_size)
            {
                outmap -> gx_pixelmap_data_size = compressed_data_size;
                outmap -> gx_pixelmap_aux_data_size = compressed_aux_size;
                outmap -> gx_pixelmap_flags |= GX_PIXELMAP_COMPRESSED;

                _gx_image_reader_pixel_read_callback_set(image_reader, &srcmap);

                status = _gx_image_reader_pixel_write_callback_set(image_reader, outmap);

                if (status == GX_SUCCESS)
                {
                    status = _gx_image_reader_rle_encode(image_reader, outmap);
                }
            }
            else
            {
                *outmap = srcmap;
                memset(&srcmap, 0, sizeof(GX_PIXELMAP));
            }
        }
    }
    else
    {
        *outmap = srcmap;
        memset(&srcmap, 0, sizeof(GX_PIXELMAP));
    }

    /* Release memory that used to load decoded pixelmap data. */
    if (srcmap.gx_pixelmap_data)
    {
        _gx_system_memory_free((VOID *)srcmap.gx_pixelmap_data);
    }

    if (srcmap.gx_pixelmap_aux_data)
    {
        _gx_system_memory_free((VOID *)srcmap.gx_pixelmap_aux_data);
    }

    if (image_reader -> gx_image_reader_png_trans)
    {
        _gx_system_memory_free((VOID *)image_reader -> gx_image_reader_png_trans);
    }

    if (status != GX_SUCCESS)
    {
        if (outmap -> gx_pixelmap_data)
        {
            _gx_system_memory_free((VOID *)outmap -> gx_pixelmap_data);
            outmap -> gx_pixelmap_data = GX_NULL;
        }

        if (outmap -> gx_pixelmap_aux_data)
        {
            _gx_system_memory_free((VOID *)outmap -> gx_pixelmap_aux_data);
            outmap -> gx_pixelmap_aux_data = GX_NULL;
        }
    }

    return status;
}
#endif

