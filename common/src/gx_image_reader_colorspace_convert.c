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
#include "gx_image_reader.h"

#define GX_FS_SCALE             10
#define GX_DITHER_SPECKLE_LIMIT 400

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_one_row_dither                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This helper function does dithering for one row of the pixelmap.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    error_ptr                             Memory for error information  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_image_reader_pixel_read]          Read a pixel from input buffer*/
/*    [gx_image_reader_pixel_write]         Write a pixel out with        */
/*                                            specified color format      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_colorspace_convert                                 */
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
static VOID _gx_image_reader_one_row_dither(GX_IMAGE_READER *image_reader, INT *error_ptr)
{
UINT     width;
UINT     err_width;
UINT     col;
GX_PIXEL want_pixel;
GX_PIXEL written_pixel;
INT      red_err;
INT      green_err;
INT      blue_err;
INT     *next_err;
INT     *cur_err;

    width = image_reader -> gx_image_reader_image_width;
    err_width = width + 2;

    cur_err = error_ptr;
    next_err = error_ptr + err_width * 3;
    memset(next_err, 0, sizeof(INT) * err_width * 3);

    for (col = 0; col < width; col++)
    {
        image_reader -> gx_image_reader_pixel_read(image_reader, (INT)col, &want_pixel);

        red_err = (cur_err[col + 1] >> GX_FS_SCALE);
        green_err = (cur_err[err_width + col + 1] >> GX_FS_SCALE);
        blue_err = (cur_err[err_width * 2 + col + 1] >> GX_FS_SCALE);

        if (want_pixel.gx_pixel_red + red_err > 255)
        {
            want_pixel.gx_pixel_red = 255;
        }
        else if (want_pixel.gx_pixel_red + red_err < 0)
        {
            want_pixel.gx_pixel_red = 0;
        }
        else
        {
            want_pixel.gx_pixel_red = (GX_UBYTE)(want_pixel.gx_pixel_red + red_err);
        }

        if (want_pixel.gx_pixel_green + green_err > 255)
        {
            want_pixel.gx_pixel_green = 255;
        }
        else if (want_pixel.gx_pixel_green + green_err < 0)
        {
            want_pixel.gx_pixel_green = 0;
        }
        else
        {
            want_pixel.gx_pixel_green = (GX_UBYTE)(want_pixel.gx_pixel_green + green_err);
        }

        if (want_pixel.gx_pixel_blue + blue_err > 255)
        {
            want_pixel.gx_pixel_blue = 255;
        }
        else if (want_pixel.gx_pixel_blue + blue_err < 0)
        {
            want_pixel.gx_pixel_blue = 0;
        }
        else
        {
            want_pixel.gx_pixel_blue = (GX_UBYTE)(want_pixel.gx_pixel_blue + blue_err);
        }

        written_pixel = want_pixel;

        image_reader -> gx_image_reader_pixel_write(image_reader, &written_pixel);

        /* Calculate color space convert error. */
        red_err = ((want_pixel.gx_pixel_red - written_pixel.gx_pixel_red) << GX_FS_SCALE);
        green_err = ((want_pixel.gx_pixel_green - written_pixel.gx_pixel_green) << GX_FS_SCALE);
        blue_err = ((want_pixel.gx_pixel_blue - written_pixel.gx_pixel_blue) << GX_FS_SCALE);

        /* Red error.  */
        if (GX_ABS(red_err) > GX_DITHER_SPECKLE_LIMIT)
        {
            cur_err[col + 2]  += ((red_err * 7) >> 4);
            next_err[col]     += ((red_err * 3) >> 4);
            next_err[col + 1] += ((red_err * 5) >> 4);
            next_err[col + 2] += ((red_err) >> 4);
        }
        else
        {
            cur_err[col + 2]  = 0;
            next_err[col]     = 0;
            next_err[col + 1] = 0;
            next_err[col + 2] = 0;
        }

        /* Green error. */
        if (GX_ABS(green_err) > GX_DITHER_SPECKLE_LIMIT)
        {
            cur_err[err_width + col + 2]  += ((green_err * 7) >> 4);
            next_err[err_width + col]     += ((green_err * 3) >> 4);
            next_err[err_width + col + 1] += ((green_err * 5) >> 4);
            next_err[err_width + col + 2] += ((green_err) >> 4);
        }
        else
        {
            cur_err[err_width + col + 2]  = 0;
            next_err[err_width + col]     = 0;
            next_err[err_width + col + 1] = 0;
            next_err[err_width + col + 2] = 0;
        }

        /* Blue error.  */
        if (GX_ABS(blue_err) > GX_DITHER_SPECKLE_LIMIT)
        {
            cur_err[err_width * 2 + col + 2] += ((blue_err * 7) >> 4);
            next_err[err_width * 2 + col] += ((blue_err * 3) >> 4);
            next_err[err_width * 2 + col + 1] += ((blue_err * 5) >> 4);
            next_err[err_width * 2 + col + 2] += ((blue_err) >> 4);
        }
        else
        {
            cur_err[err_width * 2 + col + 2]  = 0;
            next_err[err_width * 2 + col]     = 0;
            next_err[err_width * 2 + col + 1] = 0;
            next_err[err_width * 2 + col + 2] = 0;
        }
    }

    memcpy((void *)cur_err, (void *)next_err, err_width * 3 * sizeof(INT)); /* Use case of memcpy is verified. */
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_one_row_convert                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This helper function converts pixel data to specified color format  */
/*    for one row of the pixelmap.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    width                                 Pointer to pixelmap width     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_image_reader_pixel_read]          Read a pixel from input buffer*/
/*    [gx_image_reader_pixel_write]         Write a pixel out with        */
/*                                            specified color format      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_colorspace_convert                                 */
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
static VOID _gx_image_reader_one_row_convert(GX_IMAGE_READER *image_reader, INT *width)
{
INT      xval;
GX_PIXEL pixel;

    for (xval = 0; xval < (*width); xval++)
    {
        image_reader -> gx_image_reader_pixel_read(image_reader, xval, &pixel);
        image_reader -> gx_image_reader_pixel_write(image_reader, &pixel);
    }
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_one_row_rotated_convert            PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This helper function converts pixel data to specified color format  */
/*    for one row of the pixelmap with rotation.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    width                                 Pointer to pixelmap width     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_image_reader_pixel_read]          Read a pixel from input buffer*/
/*    [gx_image_reader_pixel_write]         Write a pixel out with        */
/*                                            specified color format      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_colorspace_convert                                 */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
static VOID _gx_image_reader_one_row_rotated_convert(GX_IMAGE_READER *image_reader, INT *width)
{
INT      xval;
GX_PIXEL pixel;

    image_reader -> gx_image_reader_putdata = image_reader -> gx_image_reader_putdatarow;
    image_reader -> gx_image_reader_putauxdata = image_reader -> gx_image_reader_putauxdatarow;

    for (xval = 0; xval < (*width); xval++)
    {
        image_reader -> gx_image_reader_pixel_read(image_reader, xval, &pixel);
        image_reader -> gx_image_reader_pixel_write(image_reader, &pixel);
    }

    image_reader -> gx_image_reader_putdatarow += image_reader -> gx_image_reader_putdatarow_stride;
    image_reader -> gx_image_reader_putauxdatarow += image_reader -> gx_image_reader_putauxdatarow_stride;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_colorspace_convert                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function converts a pixelmap to the specified color format     */
/*    with or without dithering process.                                  */
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
/*    _gx_image_reader_one_row_dither      Process dithering for one row  */
/*    _gx_image_reader_one_row_convert     Process direct convert for one */
/*                                           row                          */
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
UINT _gx_image_reader_colorspace_convert(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
INT  yval;
INT *param = GX_NULL;
INT  width;
INT  height;
VOID (*row_convert)(GX_IMAGE_READER *, INT *);

    width = outmap -> gx_pixelmap_width;
    height = outmap -> gx_pixelmap_height;

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_DITHER)
    {
        /* This function is called to convert decoded png/jpeg data to guix format pixelmap,
           max png/jpeg width and heigt is limited to 14 bits, so overflow cannot occur. */
        param = (INT *)_gx_system_memory_allocator(sizeof(INT) * (UINT)(width + 2) * 6);

        if (!param)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        memset((VOID *)param, 0, (sizeof(INT) * (UINT)(width + 2) * 3));
        row_convert = _gx_image_reader_one_row_dither;
    }
    else
    {
        param = &width;

        if (image_reader -> gx_image_reader_mode & (GX_IMAGE_READER_MODE_ROTATE_CW | GX_IMAGE_READER_MODE_ROTATE_CCW))
        {
            row_convert = _gx_image_reader_one_row_rotated_convert;
        }
        else
        {
            row_convert = _gx_image_reader_one_row_convert;
        }
    }

    image_reader -> gx_image_reader_size_testing = GX_FALSE;

    /* Run color space convert.  */
    for (yval = 0; yval < height; yval++)
    {
        row_convert(image_reader, param);

        image_reader -> gx_image_reader_getdata += image_reader -> gx_image_reader_input_stride;
        image_reader -> gx_image_reader_getauxdata += width;

        switch (image_reader -> gx_image_reader_color_format)
        {
        case GX_COLOR_FORMAT_4BIT_GRAY:
            if (image_reader -> gx_image_reader_putdata_mask != 0xf0)
            {
                image_reader -> gx_image_reader_putdata++;
                image_reader -> gx_image_reader_putdata_mask = 0xf0;
            }

            if (image_reader -> gx_image_reader_putauxdata)
            {
                if (image_reader -> gx_image_reader_putauxdata_mask != 0x80)
                {
                    image_reader -> gx_image_reader_putauxdata++;
                    image_reader -> gx_image_reader_putauxdata_mask = 0x80;
                }
            }
            break;

        case GX_COLOR_FORMAT_MONOCHROME:
            if (image_reader -> gx_image_reader_putdata_mask != 0x80)
            {
                image_reader -> gx_image_reader_putdata++;
                image_reader -> gx_image_reader_putdata_mask = 0x80;
            }
            break;
        }
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_DITHER)
    {
        _gx_system_memory_free((VOID *)param);
    }

    return GX_SUCCESS;
}
#endif

