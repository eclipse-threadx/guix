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
/*    _gx_image_reader_8bit_alpha_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads 8bit alpha value from input pixelmap data.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_8bit_alpha_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += index;

    pixel -> gx_pixel_red = 0;
    pixel -> gx_pixel_green = 0;
    pixel -> gx_pixel_blue = 0;
    pixel -> gx_pixel_alpha = *pLine;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_1bit_pixel_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_1bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_COLOR *pal;
UINT      palindex;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += (index >> 3);

    palindex = *pLine;
    palindex >>= (7 - (index & 0x7));
    palindex &= 0x01;

    if ((image_reader -> gx_image_reader_png_trans) &&
        (palindex == image_reader -> gx_image_reader_png_trans[0]))
    {
        /* Transparent */
        pixel -> gx_pixel_alpha = 0;
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
    }
    else
    {
        pal = (GX_COLOR *)image_reader -> gx_image_reader_png_palette;

        if (pal)
        {
            /* Palette */
            if (palindex < image_reader -> gx_image_reader_png_palette_size)
            {
                pixel -> gx_pixel_red = (GX_UBYTE)(pal[palindex] >> 16);
                pixel -> gx_pixel_green = (GX_UBYTE)(pal[palindex] >> 8);
                pixel -> gx_pixel_blue = (GX_UBYTE)pal[palindex];
                pixel -> gx_pixel_alpha = 0xff;
            }
            else
            {
                memset(pixel, 0, sizeof(GX_PIXEL));
            }
        }
        else
        {
            /* Gray */
            palindex *= 255;
            pixel -> gx_pixel_red = (GX_UBYTE)palindex;
            pixel -> gx_pixel_green = (GX_UBYTE)palindex;
            pixel -> gx_pixel_blue = (GX_UBYTE)palindex;
            pixel -> gx_pixel_alpha = 0xff;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_2bit_pixel_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_2bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_COLOR *pal;
UINT      palindex;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += (index >> 2);

    palindex = *pLine;
    palindex >>= (3 - (index & 0x3)) * 2;
    palindex &= 0x03;

    if ((image_reader -> gx_image_reader_png_trans) &&
        (palindex == image_reader -> gx_image_reader_png_trans[0]))
    {
        /* Transparent */
        pixel -> gx_pixel_alpha = 0;
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
    }
    else
    {
        pal = image_reader -> gx_image_reader_png_palette;

        if (pal)
        {
            /* Palette */
            if (palindex < image_reader -> gx_image_reader_png_palette_size)
            {
                pixel -> gx_pixel_red = (GX_UBYTE)(pal[palindex] >> 16);
                pixel -> gx_pixel_green = (GX_UBYTE)(pal[palindex] >> 8);
                pixel -> gx_pixel_blue = (GX_UBYTE)pal[palindex];
                pixel -> gx_pixel_alpha = 0xff;
            }
            else
            {
                memset(pixel, 0, sizeof(GX_PIXEL));
            }
        }
        else
        {
            /* Gray */
            palindex = palindex * 255 / 3;
            pixel -> gx_pixel_red = (GX_UBYTE)palindex;
            pixel -> gx_pixel_green = (GX_UBYTE)palindex;
            pixel -> gx_pixel_blue = (GX_UBYTE)palindex;
            pixel -> gx_pixel_alpha = 0xff;
        }
    }

    return GX_SUCCESS;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4bit_pixel_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_4bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_COLOR *pal;
UINT      palindex;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += (index >> 1);

    palindex = *pLine;
    palindex >>= (1 - (index & 0x01)) * 4;
    palindex &= 0x0f;

    if ((image_reader -> gx_image_reader_png_trans) &&
        (palindex == image_reader -> gx_image_reader_png_trans[0]))
    {
        /* Transparent */
        pixel -> gx_pixel_alpha = 0;
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
    }
    else
    {
        pal = image_reader -> gx_image_reader_png_palette;

        if (pal)
        {
            /* Pixel */
            if (palindex < image_reader -> gx_image_reader_png_palette_size)
            {
                pixel -> gx_pixel_red = (GX_UBYTE)(pal[palindex] >> 16);
                pixel -> gx_pixel_green = (GX_UBYTE)(pal[palindex] >> 8);
                pixel -> gx_pixel_blue = (GX_UBYTE)pal[palindex];
                pixel -> gx_pixel_alpha = 0xff;
            }
            else
            {
                memset(pixel, 0, sizeof(GX_PIXEL));
            }
        }
        else
        {
            /* Gray */
            palindex = palindex * 255 / 15;
            pixel -> gx_pixel_red = (GX_UBYTE)palindex;
            pixel -> gx_pixel_green = (GX_UBYTE)palindex;
            pixel -> gx_pixel_blue = (GX_UBYTE)palindex;
            pixel -> gx_pixel_alpha = 0xff;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_8bit_pixel_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_8bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_COLOR *palette;
UINT      palsize;
UINT      palindex;

    palindex = *(image_reader -> gx_image_reader_getdata + index);

    if ((image_reader -> gx_image_reader_png_trans) &&
        (palindex == image_reader -> gx_image_reader_png_trans[0]))
    {
        /* Transparent */
        pixel -> gx_pixel_alpha = 0;
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
    }
    else
    {
        palette = image_reader -> gx_image_reader_png_palette;
        palsize = image_reader -> gx_image_reader_png_palette_size;

        if (palette)
        {
            /* Palette */
            if (palindex < palsize)
            {
                pixel -> gx_pixel_red = (GX_UBYTE)(palette[palindex] >> 16);
                pixel -> gx_pixel_green = (GX_UBYTE)(palette[palindex] >> 8);
                pixel -> gx_pixel_blue = (GX_UBYTE)palette[palindex];
                pixel -> gx_pixel_alpha = 0xff;
            }
            else
            {
                memset(pixel, 0, sizeof(GX_PIXEL));
            }
        }
        else
        {
            /* Gray */
            pixel -> gx_pixel_red = (GX_UBYTE)palindex;
            pixel -> gx_pixel_green = (GX_UBYTE)palindex;
            pixel -> gx_pixel_blue = (GX_UBYTE)palindex;
            pixel -> gx_pixel_alpha = 0xff;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_8bit_palette_pixel_read            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_8bit_palette_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_COLOR *palette = image_reader -> gx_image_reader_palette;
UINT      palindex;

    palindex = *(image_reader -> gx_image_reader_getdata + index);

    if (palette)
    {
        /* Palette */
        if ((image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA) &&
            (GX_TRANSPARENT_COLOR == palindex))
        {
            pixel -> gx_pixel_alpha = 0;
            pixel -> gx_pixel_red = 0;
            pixel -> gx_pixel_green = 0;
            pixel -> gx_pixel_blue = 0;
        }
        else
        {
            pixel -> gx_pixel_red = (GX_UBYTE)(palette[palindex] >> 16);
            pixel -> gx_pixel_green = (GX_UBYTE)(palette[palindex] >> 8);
            pixel -> gx_pixel_blue = (GX_UBYTE)palette[palindex];
            pixel -> gx_pixel_alpha = 0xff;
        }
    }
    else
    {
        memset(pixel, 0, sizeof(GX_PIXEL));
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_16bit_gray_alpha_read              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_16bit_gray_alpha_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += (index << 1);

    /* Trans is prohibited for this color types, since a full alpha channel is already present. */
    pixel -> gx_pixel_red = *pLine;
    pixel -> gx_pixel_green = *pLine;
    pixel -> gx_pixel_blue = *pLine++;
    pixel -> gx_pixel_alpha = *pLine;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_32bit_gray_alpha_read              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_32bit_gray_alpha_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += (index << 2);

    pixel -> gx_pixel_red = *pLine;
    pixel -> gx_pixel_green = pixel -> gx_pixel_red;
    pixel -> gx_pixel_blue = pixel -> gx_pixel_red;
    pLine += 2;
    pixel -> gx_pixel_alpha = (GX_UBYTE)*pLine;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_24bit_pixel_read                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_24bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_COLOR *pTrans = image_reader -> gx_image_reader_png_trans;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += index * 3;

    pixel -> gx_pixel_red = *pLine++;
    pixel -> gx_pixel_green = *pLine++;
    pixel -> gx_pixel_blue = *pLine;

    if ((pTrans) &&
        (pixel -> gx_pixel_red == pTrans[0]) &&
        (pixel -> gx_pixel_green == pTrans[1]) &&
        (pixel -> gx_pixel_blue == pTrans[2]))
    {
        pixel -> gx_pixel_alpha = 0;
    }
    else
    {
        pixel -> gx_pixel_alpha = 0xff;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_48bit_pixel_read                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_48bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_COLOR *pTrans = image_reader -> gx_image_reader_png_trans;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += index * 6;

    pixel -> gx_pixel_red = *pLine;
    pLine += 2;
    pixel -> gx_pixel_green = *pLine;
    pLine += 2;
    pixel -> gx_pixel_blue = *pLine;

    if ((pTrans) &&
        (pixel -> gx_pixel_red == (GX_UBYTE)pTrans[0]) &&
        (pixel -> gx_pixel_green == (GX_UBYTE)pTrans[1]) &&
        (pixel -> gx_pixel_blue == (GX_UBYTE)pTrans[2]))
    {
        pixel -> gx_pixel_alpha = 0;
    }
    else
    {
        pixel -> gx_pixel_alpha = 0xff;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_64bit_pixel_read                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_64bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;

    pLine = image_reader -> gx_image_reader_getdata;
    pLine += (index << 3);

    pixel -> gx_pixel_red = *pLine;
    pLine += 2;
    pixel -> gx_pixel_green = *pLine;
    pLine += 2;
    pixel -> gx_pixel_blue = *pLine;
    pLine += 2;
    pixel -> gx_pixel_alpha = *pLine;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_32argb_pixel_read                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_32argb_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_COLOR *pLine;

    pLine = (GX_COLOR *)image_reader -> gx_image_reader_getdata;
    pLine += index;

    pixel -> gx_pixel_alpha = (GX_UBYTE)((*pLine) >> 24);
    pixel -> gx_pixel_red = (GX_UBYTE)((*pLine) >> 16);
    pixel -> gx_pixel_green = (GX_UBYTE)((*pLine) >> 8);
    pixel -> gx_pixel_blue = (GX_UBYTE)(*pLine);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_32bit_pixel_read                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_32bit_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pget;

    pget = (GX_UBYTE *)image_reader -> gx_image_reader_getdata;
    pget += (index << 2);

    pixel -> gx_pixel_red = *pget++;
    pixel -> gx_pixel_green = *pget++;
    pixel -> gx_pixel_blue = *pget++;
    pixel -> gx_pixel_alpha = *pget++;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_16bit_gray_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_16bit_gray_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE *pget;

    pget = (GX_UBYTE *)image_reader -> gx_image_reader_getdata;
    pget += (index << 1);

    if ((image_reader -> gx_image_reader_png_trans) &&
        ((*pget) == (GX_UBYTE)image_reader -> gx_image_reader_png_trans[0]))
    {
        pixel -> gx_pixel_alpha = 0;
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
    }
    else
    {
        pixel -> gx_pixel_red = *pget;
        pixel -> gx_pixel_green = *pget;
        pixel -> gx_pixel_blue = *pget;
        pixel -> gx_pixel_alpha = 0xff;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_565rgb_pixel_read                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads a pixel from input pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_565rgb_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    pLine = (USHORT *)image_reader -> gx_image_reader_getdata;
    pLine += index;

    pixel -> gx_pixel_red = (GX_UBYTE)(((*pLine) & 0xf800) >> 8);
    pixel -> gx_pixel_green = (GX_UBYTE)(((*pLine) & 0x07e0) >> 3);
    pixel -> gx_pixel_blue = (GX_UBYTE)(((*pLine) & 0x001f) << 3);

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        pAlpha = (GX_UBYTE *)image_reader -> gx_image_reader_getauxdata;
        pAlpha += index;

        pixel -> gx_pixel_alpha = (*pAlpha);
    }
    else
    {
        pixel -> gx_pixel_alpha = 0;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_1555xrgb_pixel_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads an 1555xrgb format pixel from input pixelmap    */
/*    data structure.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_1555xrgb_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    pLine = (USHORT *)image_reader -> gx_image_reader_getdata;
    pLine += index;

    pixel -> gx_pixel_red = (GX_UBYTE)(((*pLine) & 0x7c00) >> 7);
    pixel -> gx_pixel_green = (GX_UBYTE)(((*pLine) & 0x03e0) >> 2);
    pixel -> gx_pixel_blue = (GX_UBYTE)(((*pLine) & 0x001f) << 3);

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        pAlpha = (GX_UBYTE *)image_reader -> gx_image_reader_getauxdata;
        pAlpha += index;

        pixel -> gx_pixel_alpha = (*pAlpha);
    }
    else
    {
        pixel -> gx_pixel_alpha = 0;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4444argb_pixel_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads an 4444argb format pixel from input pixelmap    */
/*    data structure.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_4444argb_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
USHORT        color;
const USHORT *data = (USHORT *)(image_reader -> gx_image_reader_getdata);

    color = *(data + index);

    pixel -> gx_pixel_alpha = (GX_UBYTE)((color & 0xf000) >> 8);
    pixel -> gx_pixel_red = (GX_UBYTE)((color & 0xf00) >> 4);
    pixel -> gx_pixel_green = (GX_UBYTE)(color & 0xf0);
    pixel -> gx_pixel_blue = (GX_UBYTE)((color & 0xf) << 4);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4bit_grayscale_pixel_read          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads an 4bit grayscale format pixel (no transparency)*/
/*    from input pixelmap data structure.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_4bit_grayscale_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE color;

    color = *(image_reader -> gx_image_reader_getdata + (index >> 1));

    if (index & 1)
    {
        color &= 0x0f;
    }
    else
    {
        color &= 0xf0;
        color >>= 4;
    }

    color |= (GX_UBYTE)(color << 4);
    pixel -> gx_pixel_red = color;
    pixel -> gx_pixel_green = color;
    pixel -> gx_pixel_blue = color;
    pixel -> gx_pixel_alpha = 0xff;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4bit_grayscale_transparent_read    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads an 4bit grayscale format pixel                  */
/*    (with transparency)from input pixelmap data structure.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_4bit_grayscale_transparent_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
const GX_UBYTE *data = image_reader -> gx_image_reader_getdata;
const GX_UBYTE *auxdata = image_reader -> gx_image_reader_getauxdata;
INT             aux_index = (index >> 3);
INT             color_index = (index >> 1);
GX_UBYTE        color = *(data + color_index);
GX_UBYTE        transparency = *(auxdata + aux_index);

    /* Get transparent mask. */
    transparency = (GX_UBYTE)(transparency >> (7 - (index & 0x07)));

    if (transparency & 0x01)
    {
        color = 0;
        pixel -> gx_pixel_alpha = 0;
    }
    else
    {
        if (index & 1)
        {
            color &= 0xf;
        }
        else
        {
            color &= 0xf0;
            color >>= 4;
        }
        color = (GX_UBYTE)(color | color << 4);
        pixel -> gx_pixel_alpha = 0xff;
    }

    pixel -> gx_pixel_red = color;
    pixel -> gx_pixel_green = color;
    pixel -> gx_pixel_blue = color;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_1bpp_pixel_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads an 1bpp format pixel (no transparency) from     */
/*    input pixelmap data structure.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_1bpp_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE color;

    color = *(image_reader -> gx_image_reader_getdata + (index >> 3));

    if (color & (0x80 >> (index & 0x07)))
    {
        pixel -> gx_pixel_red = 0xff;
        pixel -> gx_pixel_green = 0xff;
        pixel -> gx_pixel_blue = 0xff;
    }
    else
    {
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
    }
    pixel -> gx_pixel_alpha = 0xff;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_1bpp_transparent_read              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads an 1bpp format pixel (no transparency) from     */
/*    input pixelmap data structure.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                         Image reader control block     */
/*    index                                Row index.                     */
/*    pixel                                Retrieved pixel.               */
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
/*    _gx_image_reader_pixel_read_callback_set                            */
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
static UINT _gx_image_reader_1bpp_transparent_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
GX_UBYTE color;

    color = *(image_reader -> gx_image_reader_getdata + (index >> 2));

    if (color & (0x40 >> ((index & 0x03) << 1)))
    {
        if (color & (0x80 >> ((index & 0x03) << 1)))
        {
            pixel -> gx_pixel_red = 0xff;
            pixel -> gx_pixel_green = 0xff;
            pixel -> gx_pixel_blue = 0xff;
        }
        else
        {
            pixel -> gx_pixel_red = 0;
            pixel -> gx_pixel_green = 0;
            pixel -> gx_pixel_blue = 0;
        }
        pixel -> gx_pixel_alpha = 0xff;
    }
    else
    {
        pixel -> gx_pixel_red = 0;
        pixel -> gx_pixel_green = 0;
        pixel -> gx_pixel_blue = 0;
        pixel -> gx_pixel_alpha = 0;
    }

    return GX_SUCCESS;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_pixel_read_callback_set            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets pixel read callback of the image reader.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    inmap                                 Input pixelmap                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_1bit_pixel_read      Read 1bit internal format     */
/*                                            pixel                       */
/*    _gx_image_reader_2bit_pixel_read      Read 2bit internal format     */
/*                                            pixel                       */
/*    _gx_image_reader_4bit_pixel_read      Read 4bit internal format     */
/*                                            pixel                       */
/*    _gx_image_reader_8bit_pixel_read      Read 8bit internal format     */
/*                                            pixel                       */
/*    _gx_image_reader_16bit_gray_read      Read 16bit internal format    */
/*                                            pixel                       */
/*    _gx_image_reader_16bit_gray_alpha_read                              */
/*                                          Read 16bit gray:alpha internal*/
/*                                            format pixel                */
/*    _gx_image_reader_32bit_gray_alpha_read                              */
/*                                          Read 32bit gray:alpha internal*/
/*                                            format pixel                */
/*    _gx_image_reader_24bit_pixel_read     Read 24bit internal format    */
/*                                            pixel                       */
/*    _gx_image_reader_32bit_pixel_read     Read 32bit internal format    */
/*                                            pixel                       */
/*    _gx_image_reader_48bit_pixel_read     Read 48bit internal format    */
/*                                            pixel                       */
/*    _gx_image_reader_64bit_pixel_read     Read 64bit internal format    */
/*                                            pixel                       */
/*    _gx_image_reader_32argb_pixel_read    Read 32argb guix format pixel */
/*                                            pixel                       */
/*    _gx_image_reader_565rgb_pixel_read    Read 565rgb guix format pixel */
/*    _gx_image_reader_1555xrgb_pixel_read  Read 1555xrgb guix format     */
/*                                            pixel                       */
/*    _gx_image_reader_8bit_palette_pixel_read                            */
/*                                          Read 8bit palette guix format */
/*                                            pixel                       */
/*    _gx_image_reader_8bit_alpha_read      Read 8bit alphamap guix format*/
/*                                            pixel                       */
/*    _gx_image_reader_4444argb_pixel_read  Read 4444argb guix format     */
/*                                            pixel                       */
/*    _gx_image_reader_4bit_grayscale_transparent_read                    */
/*                                          Read 4bit grayscale guix      */
/*                                            format pixel with           */
/*                                            transparency                */
/*    _gx_image_reader_4bit_grayscale_pixel_read                          */
/*                                          Read 4bit grayscale guix      */
/*                                            format pxiel                */
/*    _gx_image_reader_1bpp_transparent_read                              */
/*                                          Read 1bpp guix format pixel   */
/*                                            with transparency           */
/*    _gx_image_reader_1bpp_pixel_read      Read 1bpp guix format pixel   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_image_reader_colorspace_convert                                  */
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
UINT _gx_image_reader_pixel_read_callback_set(GX_IMAGE_READER *image_reader, GX_PIXELMAP *inmap)
{

    switch (inmap -> gx_pixelmap_format)
    {
    case GX_IMAGE_FORMAT_1BPP:
        if (inmap -> gx_pixelmap_aux_data)
        {
            image_reader -> gx_image_reader_png_palette = (GX_COLOR *)inmap -> gx_pixelmap_aux_data;
            image_reader -> gx_image_reader_png_palette_size = inmap -> gx_pixelmap_aux_data_size / sizeof(GX_COLOR);
        }
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_1bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width >> 3);
        break;

    case GX_IMAGE_FORMAT_2BPP:
        if (inmap -> gx_pixelmap_aux_data)
        {
            image_reader -> gx_image_reader_png_palette = (GX_COLOR *)inmap -> gx_pixelmap_aux_data;
            image_reader -> gx_image_reader_png_palette_size = inmap -> gx_pixelmap_aux_data_size / sizeof(GX_COLOR);
        }
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_2bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width >> 2);
        break;

    case GX_IMAGE_FORMAT_4BPP:
        if (inmap -> gx_pixelmap_aux_data)
        {
            image_reader -> gx_image_reader_png_palette = (GX_COLOR *)inmap -> gx_pixelmap_aux_data;
            image_reader -> gx_image_reader_png_palette_size = inmap -> gx_pixelmap_aux_data_size / sizeof(GX_COLOR);
        }

        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_4bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width >> 1);
        break;

    case GX_IMAGE_FORMAT_8BPP:
        if (inmap -> gx_pixelmap_aux_data)
        {
            image_reader -> gx_image_reader_png_palette = (GX_COLOR *)inmap -> gx_pixelmap_aux_data;
            image_reader -> gx_image_reader_png_palette_size = inmap -> gx_pixelmap_aux_data_size / sizeof(GX_COLOR);
        }

        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_8bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)inmap -> gx_pixelmap_width;
        break;

    case GX_IMAGE_FORMAT_16BPP_GRAY:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_16bit_gray_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 1);
        break;

    case GX_IMAGE_FORMAT_16BPP_GRAY_ALPHA: /* Internal format: gray: alpha byte stream. */
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_16bit_gray_alpha_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 1);
        break;

    case GX_IMAGE_FORMAT_32BPP_GRAY_ALPHA: /* Internal format: gray: alpha byte stream. */
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_32bit_gray_alpha_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 2);
        break;

    case GX_IMAGE_FORMAT_24BPP: /* Internal format: r:g:b byte stream. */
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_24bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width * 3);
        break;

    case GX_IMAGE_FORMAT_32BPP: /* Internal format: r:g:b:a byte strem. */
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_32bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 2);
        break;

    case GX_IMAGE_FORMAT_48BPP:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_48bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width * 6);
        break;

    case GX_IMAGE_FORMAT_64BPP:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_64bit_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 3);
        break;

    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_32argb_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 2);
        break;

    case GX_COLOR_FORMAT_565RGB:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_565rgb_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 1);
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_1555xrgb_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 1);
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_8bit_palette_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)inmap -> gx_pixelmap_width;
        break;

    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_8bit_alpha_read;
        image_reader -> gx_image_reader_input_stride = (UINT)inmap -> gx_pixelmap_width;
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        image_reader -> gx_image_reader_pixel_read = _gx_image_reader_4444argb_pixel_read;
        image_reader -> gx_image_reader_input_stride = (UINT)(inmap -> gx_pixelmap_width << 1);
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (inmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            image_reader -> gx_image_reader_pixel_read = _gx_image_reader_4bit_grayscale_transparent_read;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_read = _gx_image_reader_4bit_grayscale_pixel_read;
        }
        image_reader -> gx_image_reader_input_stride = (UINT)((inmap -> gx_pixelmap_width + 1) >> 1);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        if (inmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            image_reader -> gx_image_reader_pixel_read = _gx_image_reader_1bpp_transparent_read;
            image_reader -> gx_image_reader_input_stride = (UINT)((inmap -> gx_pixelmap_width + 3) >> 2);
        }
        else
        {
            image_reader -> gx_image_reader_pixel_read = _gx_image_reader_1bpp_pixel_read;
            image_reader -> gx_image_reader_input_stride = (UINT)((inmap -> gx_pixelmap_width + 7) >> 3);
        }
        break;

    default:
        return GX_NOT_SUPPORTED;
    }

    image_reader -> gx_image_reader_getdata = (GX_UBYTE *)inmap -> gx_pixelmap_data;
    image_reader -> gx_image_reader_getauxdata = (GX_UBYTE *)inmap -> gx_pixelmap_aux_data;

    return GX_SUCCESS;
}
#endif

