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
/**   Image Reader Management(Image Reader)                               */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_image_reader.h"
#include "gx_system.h"

#if defined(GX_SOFTWARE_DECODER_SUPPORT)

#define PIXEL_CMP(_p1, _p2)                       \
    (_p1.gx_pixel_alpha == _p2.gx_pixel_alpha) && \
    (_p1.gx_pixel_red == _p2.gx_pixel_red) &&     \
    (_p1.gx_pixel_green == _p2.gx_pixel_green) && \
    (_p1.gx_pixel_blue == _p2.gx_pixel_blue)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_rle_count_location_get             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves the count put position.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    put_count                             Retrieved count put position. */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_rle_one_row_encode                                 */
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
static UINT _gx_image_reader_rle_count_location_get(GX_IMAGE_READER *image_reader, GX_UBYTE **put_count)
{
    switch (image_reader -> gx_image_reader_color_format)
    {
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        (*put_count) = image_reader -> gx_image_reader_putauxdata++;
        break;

    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_1555XRGB:
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            (*put_count) = image_reader -> gx_image_reader_putdata;
        }
        else
        {
            (*put_count) = image_reader -> gx_image_reader_putdata;
            image_reader -> gx_image_reader_putdata += 2;
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        (*put_count) = image_reader -> gx_image_reader_putdata;
        image_reader -> gx_image_reader_putdata += 2;
        break;

    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        (*put_count) = image_reader -> gx_image_reader_putdata++;
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        (*put_count) = image_reader -> gx_image_reader_putauxdata++;
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        (*put_count) = image_reader -> gx_image_reader_putdata;
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
/*    _gx_image_reader_rle_count_write                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes repeat count to output pixelmap data structure.*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    count                                 The number of pixels that     */
/*                                            follows.                    */
/*    put_count                             The position to put "count".  */
/*    repeat                                Wheather the following pixels */
/*                                            are repeated or not.        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_rle_one_row_encode                                 */
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
static UINT _gx_image_reader_rle_count_write(GX_IMAGE_READER *image_reader, INT *count,
                                             GX_UBYTE *put_count, GX_BOOL repeat)
{
INT write_count;

    write_count = (*count) - 1;

    switch (image_reader -> gx_image_reader_color_format)
    {
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_1555XRGB:
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            if (write_count > 127)
            {
                write_count = 127;
            }

            if (repeat)
            {
                write_count |= 0x80;
            }

            if (!image_reader -> gx_image_reader_size_testing)
            {
                *put_count = (GX_UBYTE)write_count;
            }

            (*count) = (write_count & 0x7f) + 1;
        }
        else
        {
            if (repeat)
            {
                write_count |= 0x8000;
            }

            if (!image_reader -> gx_image_reader_size_testing)
            {
                *((USHORT *)put_count) = (USHORT)write_count;
            }

            (*count) = (write_count & 0x7fff) + 1;
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        if (repeat)
        {
            write_count |= 0x8000;
        }

        if (!image_reader -> gx_image_reader_size_testing)
        {
            *((USHORT *)put_count) = (USHORT)write_count;
        }

        *count = (write_count & 0x7fff) + 1;
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            if (write_count > 0x1f)
            {
                write_count = 0x1f;
            }
            if (repeat)
            {
                write_count |= 0x20;
            }
            if (!image_reader -> gx_image_reader_size_testing)
            {
                *put_count |= (GX_UBYTE)(write_count << 2);
            }

            (*count)  = (write_count & 0x1f) + 1;
        }
        else
        {
            if (write_count > 0x3f)
            {
                write_count = 0x3f;
            }
            if (repeat)
            {
                write_count |= 0x40;
            }
            if (!image_reader -> gx_image_reader_size_testing)
            {
                (*put_count) |= (GX_UBYTE)(write_count << 1);
            }

            (*count)  = (write_count & 0x3f) + 1;
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
    default:
        if (write_count > 127)
        {
            write_count = 127;
        }

        if (repeat)
        {
            write_count |= 0x80;
        }

        if (!image_reader -> gx_image_reader_size_testing)
        {
            (*put_count) = (GX_UBYTE)write_count;
        }

        (*count) = (write_count & 0x7f) + 1;
        break;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_rle_duplicates_count               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates the number of duplicate pixels.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block.    */
/*    index                                Position of the pixel to       */
/*                                           calcualte from.              */
/*    repeat_counts                        The number of duplicate pixels.*/
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
/*    _gx_image_reader_rle_one_row_encode                                 */
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
static UINT _gx_image_reader_rle_duplicates_count(GX_IMAGE_READER *image_reader, INT index, INT *repeat_counts)
{
UINT     width;
INT      duplicates = 1;
GX_PIXEL pre_pixel;
GX_PIXEL cur_pixel;


    image_reader -> gx_image_reader_pixel_read(image_reader, index++, &pre_pixel);

    width = image_reader -> gx_image_reader_image_width;

    while ((UINT)index < width)
    {
        image_reader -> gx_image_reader_pixel_read(image_reader, index, &cur_pixel);

        if (PIXEL_CMP(pre_pixel, cur_pixel))
        {
            duplicates++;
        }
        else
        {
            break;
        }
        index++;
    }

    (*repeat_counts) = duplicates;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_rle_one_row_encode                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This funcitons encodes one row of the input pixelmap.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rle_duplicates_count Calculate the number of       */
/*                                            duplicate pixels            */
/*    _gx_image_reader_rle_count_write      Write out duplicated pixel    */
/*                                            count                       */
/*    _gx_image_reader_rle_count_location_get                             */
/*                                          Retrieve the address to put   */
/*                                            duplicated pixel count      */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_rle_encode                                         */
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
static UINT _gx_image_reader_rle_one_row_encode(GX_IMAGE_READER *image_reader)
{
UINT      status = GX_SUCCESS;
UINT      col = 0;
INT       raw_count = 0;
INT       count = 0;
GX_UBYTE *putCount = GX_NULL;
UINT      width;
GX_PIXEL  pixel;

    width = image_reader -> gx_image_reader_image_width;

    while (col < width)
    {
        /* Calculate the number of duplicate pixels. */
        _gx_image_reader_rle_duplicates_count(image_reader, (INT)col, &count);

        if (count >= 3)
        {
            if (raw_count)
            {
                _gx_image_reader_rle_count_write(image_reader, &raw_count, putCount, GX_FALSE);
            }

            /* Retrieve the count put position.  */
            status = _gx_image_reader_rle_count_location_get(image_reader, &putCount);

            if (status != GX_SUCCESS)
            {
                return status;
            }

            /* Write count. */
            _gx_image_reader_rle_count_write(image_reader, &count, putCount, GX_TRUE);

            image_reader -> gx_image_reader_pixel_read(image_reader, (INT)col, &pixel);

            /* Write pixel value. */
            image_reader -> gx_image_reader_pixel_write(image_reader, &pixel);

            col += (UINT)count;
            raw_count = 0;
        }
        else
        {

            if (!raw_count)
            {
                status = _gx_image_reader_rle_count_location_get(image_reader, &putCount);

                if (status != GX_SUCCESS)
                {
                    return status;
                }
            }

            raw_count++;

            image_reader -> gx_image_reader_pixel_read(image_reader, (INT)col, &pixel);

            /* Write pixel value. */
            image_reader -> gx_image_reader_pixel_write(image_reader, &pixel);

            col++;

            if (raw_count == 128 || col == width)
            {
                _gx_image_reader_rle_count_write(image_reader, &raw_count, putCount, GX_FALSE);

                raw_count = 0;
            }
        }
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_rle_encode_size_get                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the size of encoded pixelmap.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    datasize                              Retrieved data size.          */
/*    auxsize                               Retrieved aux data size.      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rle_one_row_encode    Process one row encoding     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_rle_colorspace_convert                             */
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
UINT _gx_image_reader_rle_encode_size_get(GX_IMAGE_READER *image_reader, UINT *data_size, UINT *aux_size)
{
UINT status = GX_SUCCESS;
UINT row;
UINT height;
UINT width;
UINT aux_stride;


    height = image_reader -> gx_image_reader_image_height;
    width = image_reader -> gx_image_reader_image_width;
    aux_stride = width;

    /* Process rle encode logic, but not output compressed data. */
    image_reader -> gx_image_reader_size_testing = GX_TRUE;
    image_reader -> gx_image_reader_putdata = GX_NULL;
    image_reader -> gx_image_reader_putauxdata = GX_NULL;

    switch (image_reader -> gx_image_reader_color_format)
    {
    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            aux_stride = (aux_stride + 7) >> 3;
        }
        break;
    }

    /* Test size of compressed data to see if it is smaller.  */
    for (row = 0; row < height; row++)
    {
        status = _gx_image_reader_rle_one_row_encode(image_reader);

        if (status != GX_SUCCESS)
        {
            return status;
        }

        image_reader -> gx_image_reader_getdata += image_reader -> gx_image_reader_input_stride;
        image_reader -> gx_image_reader_getauxdata += aux_stride;
    }

    if (image_reader -> gx_image_reader_color_format == GX_COLOR_FORMAT_4BIT_GRAY)
    {
        if (!(image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA) &&
            image_reader -> gx_image_reader_putdata_mask == 0x0f)
        {
            image_reader -> gx_image_reader_putdata++;
        }
    }

    (*data_size) = (UINT)image_reader -> gx_image_reader_putdata;
    (*aux_size) = (UINT)image_reader -> gx_image_reader_putauxdata;

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_rle_encode                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function converts a raw map into a compressed map.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    outmap                                Encoded pixelmap.             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rle_one_row_encode   Process one row encoding      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_rle_colorspace_convert                             */
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
UINT _gx_image_reader_rle_encode(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
UINT     status = GX_SUCCESS;
GX_VALUE height;
GX_VALUE width;
INT      row;
GX_VALUE aux_stride;

    height = outmap -> gx_pixelmap_height;
    width = outmap -> gx_pixelmap_width;
    aux_stride = width;

    /* If size testing is TRUE, process rle encode logic but not output compressed data.
       If size testing is FALSE, process rle encode logic, and output compressed data. */
    image_reader -> gx_image_reader_size_testing = GX_FALSE;

    switch (image_reader -> gx_image_reader_color_format)
    {
    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            aux_stride = (GX_VALUE)((aux_stride + 7) >> 3);
        }
        break;
    }

    /* Process RLE encode.  */
    for (row = 0; row < height; row++)
    {
        /* Encode one row data. */
        status = _gx_image_reader_rle_one_row_encode(image_reader);

        if (status != GX_SUCCESS)
        {
            return status;
        }

        image_reader -> gx_image_reader_getdata += image_reader -> gx_image_reader_input_stride;
        image_reader -> gx_image_reader_getauxdata += aux_stride;
    }

    return status;
}
#endif

