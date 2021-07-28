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

extern VOID _gx_image_reader_rgb2gray(GX_PIXEL *pixel, GX_UBYTE *gray);
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_8bit_alpha_write                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a pixel to output pixemap data structure.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_8bit_alpha_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_UBYTE *palpha;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        palpha = (GX_UBYTE *)image_reader -> gx_image_reader_putdata;

        *palpha = pixel -> gx_pixel_alpha;
    }

    image_reader -> gx_image_reader_putdata += 1;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_8bit_palette_write                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a palette index to output pixemap data         */
/*    structure.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_8bit_palette_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
INT       palindex;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (GX_UBYTE *)image_reader -> gx_image_reader_putdata;

        if ((image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA) && (pixel -> gx_pixel_alpha < 128))
        {
            *pLine = GX_TRANSPARENT_COLOR;
        }
        else
        {
            /* Find the palette color that is nearest to the wanted color. */
            _gx_image_reader_nearest_palette_color_get(image_reader, pixel, &palindex);

            *pLine = (GX_UBYTE)palindex;
        }
    }

    image_reader -> gx_image_reader_putdata += 1;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_24xrgb_pixel_write                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a pixel to output pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_24xrgb_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_COLOR *pLine;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (GX_COLOR *)image_reader -> gx_image_reader_putdata;

        (*pLine) = (GX_COLOR)((pixel -> gx_pixel_red << 16));
        (*pLine) = (GX_COLOR)((*pLine) | ((ULONG)(pixel -> gx_pixel_green) << 8));
        (*pLine) = (GX_COLOR)((*pLine) | pixel -> gx_pixel_blue);
        (*pLine) = (GX_COLOR)((*pLine) | 0xff000000);
    }

    image_reader -> gx_image_reader_putdata += 4;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_24xrgb_rotated_pixel_write         PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a pixel to output rotated pixelmap data        */
/*    structure.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_24xrgb_rotated_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_COLOR *pLine;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (GX_COLOR *)image_reader -> gx_image_reader_putdata;

        (*pLine) = (GX_COLOR)((pixel -> gx_pixel_red << 16));
        (*pLine) = (GX_COLOR)((*pLine) | ((ULONG)(pixel -> gx_pixel_green) << 8));
        (*pLine) = (GX_COLOR)((*pLine) | pixel -> gx_pixel_blue);
        (*pLine) = (GX_COLOR)((*pLine) | ((ULONG)(pixel -> gx_pixel_alpha) << 24));
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ROTATE_CW)
    {
        image_reader -> gx_image_reader_putdata -= (image_reader -> gx_image_reader_image_height << 2);
    }
    else
    {
        image_reader -> gx_image_reader_putdata += (image_reader -> gx_image_reader_image_height << 2);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_32argb_pixel_write                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a pixel to output pixelmap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_32argb_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_COLOR *pLine;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (GX_COLOR *)image_reader -> gx_image_reader_putdata;

        (*pLine) = (GX_COLOR)(pixel -> gx_pixel_alpha << 24);
        (*pLine) = (GX_COLOR)((*pLine) | ((ULONG)(pixel -> gx_pixel_red) << 16));
        (*pLine) = (GX_COLOR)((*pLine) | ((ULONG)(pixel -> gx_pixel_green) << 8));
        (*pLine) = (GX_COLOR)((*pLine) | pixel -> gx_pixel_blue);
    }

    image_reader -> gx_image_reader_putdata += 4;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_565rgb_rle_pixel_write             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a pixel to output pixemap data structure.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_565rgb_rle_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            pAlpha = (GX_UBYTE *)image_reader -> gx_image_reader_putdata;

            /* Skip count. */
            pAlpha++;
            pLine++;

            (*pAlpha) = pixel -> gx_pixel_alpha;
        }

        pixel -> gx_pixel_red &= 0xf8;
        pixel -> gx_pixel_green &= 0xfc;
        pixel -> gx_pixel_blue &= 0xf8;

        (*pLine) = (USHORT)(pixel -> gx_pixel_red << 8);
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 3));
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_blue >> 3));
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        image_reader -> gx_image_reader_putdata += 4;
    }
    else
    {
        image_reader -> gx_image_reader_putdata += 2;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_565rgb_pixel_write                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a pixel to output pixemap data structure.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_565rgb_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

    pixel -> gx_pixel_red &= 0xf8;
    pixel -> gx_pixel_green &= 0xfc;
    pixel -> gx_pixel_blue &= 0xf8;

    (*pLine) = (USHORT)(pixel -> gx_pixel_red << 8);
    (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 3));
    (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_blue >> 3));

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        pAlpha = image_reader -> gx_image_reader_putauxdata;

        (*pAlpha) = pixel -> gx_pixel_alpha;
    }

    image_reader -> gx_image_reader_putauxdata += 1;
    image_reader -> gx_image_reader_putdata += 2;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_565rgb_rotated_pixel_write         PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function rotates a pixel to output pixemap data structure.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  03-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_565rgb_rotated_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    if(!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

        pixel -> gx_pixel_red &= 0xf8;
        pixel -> gx_pixel_green &= 0xfc;
        pixel -> gx_pixel_blue &= 0xf8;

        (*pLine) = (USHORT)(pixel -> gx_pixel_red << 8);
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 3));
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_blue >> 3));

        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            pAlpha = image_reader -> gx_image_reader_putauxdata;

            (*pAlpha) = pixel -> gx_pixel_alpha;
        }
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ROTATE_CW)
    {
        image_reader -> gx_image_reader_putauxdata -= image_reader -> gx_image_reader_image_height;
        image_reader -> gx_image_reader_putdata -= (image_reader -> gx_image_reader_image_height << 1);
    }
    else
    {
        image_reader -> gx_image_reader_putauxdata += image_reader -> gx_image_reader_image_height;
        image_reader -> gx_image_reader_putdata += (image_reader -> gx_image_reader_image_height << 1);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_1555xrgb_rle_pixel_write           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes an 1555xrgb format pixel to output pixemap     */
/*    data structure.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_1555xrgb_rle_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
        {
            pAlpha = (GX_UBYTE *)image_reader -> gx_image_reader_putdata;

            /* Skip count. */
            pAlpha++;
            pLine++;

            (*pAlpha) = pixel -> gx_pixel_alpha;
        }

        pixel -> gx_pixel_red &= 0xf8;
        pixel -> gx_pixel_green &= 0xf8;
        pixel -> gx_pixel_blue &= 0xf8;

        (*pLine) = (USHORT)(pixel -> gx_pixel_red << 7);
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 2));
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_blue >> 3));
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        image_reader -> gx_image_reader_putdata += 4;
    }
    else
    {
        image_reader -> gx_image_reader_putdata += 2;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_1555xrgb_pixel_write               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes an 1555xrgb format pixel to output pixemap     */
/*    data structure.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_1555xrgb_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT   *pLine;
GX_UBYTE *pAlpha;

    pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

    pixel -> gx_pixel_red &= 0xf8;
    pixel -> gx_pixel_green &= 0xf8;
    pixel -> gx_pixel_blue &= 0xf8;

    (*pLine) = (USHORT)(pixel -> gx_pixel_red << 7);
    (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 2));
    (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_blue >> 3));

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ALPHA)
    {
        pAlpha = image_reader -> gx_image_reader_putauxdata;

        (*pAlpha) = pixel -> gx_pixel_alpha;
    }

    image_reader -> gx_image_reader_putauxdata += 1;
    image_reader -> gx_image_reader_putdata += 2;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4444argb_pixel_write               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes an 4444argb format pixel (no transparency) to  */
/*    output pixemap data structure.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_4444argb_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT *pLine;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

        pixel -> gx_pixel_alpha &= 0xf0;
        pixel -> gx_pixel_red &= 0xf0;
        pixel -> gx_pixel_green &= 0xf0;
        pixel -> gx_pixel_blue &= 0xf0;

        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_red << 4));
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 0));
        (*pLine) = (USHORT)((*pLine) | pixel -> gx_pixel_blue >> 4);
        (*pLine) = (USHORT)((*pLine) | 0xf000);
    }

    image_reader -> gx_image_reader_putdata += 2;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4444argb_transparent_write         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes an 4444argb format pixel (with transparency)   */
/*    to output pixemap data structure.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
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
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_4444argb_transparent_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
USHORT *pLine;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        pLine = (USHORT *)image_reader -> gx_image_reader_putdata;

        pixel -> gx_pixel_alpha &= 0xf0;
        pixel -> gx_pixel_red &= 0xf0;
        pixel -> gx_pixel_green &= 0xf0;
        pixel -> gx_pixel_blue &= 0xf0;

        (*pLine) = (USHORT)(pixel -> gx_pixel_alpha << 8);
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_red << 4));
        (*pLine) = (USHORT)((*pLine) | (pixel -> gx_pixel_green << 0));
        (*pLine) = (USHORT)((*pLine) | pixel -> gx_pixel_blue >> 4);
    }

    image_reader -> gx_image_reader_putdata += 2;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4bit_grayscale_pixel_write         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes an 4bit grayscale format pixel                 */
/*    (no transparency)to output pixelmap data structure.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rgb2gray             Convert RGB value to grayscale*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_4bit_grayscale_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_UBYTE  gray;
GX_UBYTE  color;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        _gx_image_reader_rgb2gray(pixel, &gray);
        gray >>= 4;

        color = (GX_UBYTE)(gray | (gray << 4));
        pixel -> gx_pixel_red = color;
        pixel -> gx_pixel_green = color;
        pixel -> gx_pixel_blue = color;

        pLine = image_reader -> gx_image_reader_putdata;

        if (image_reader -> gx_image_reader_putdata_mask & 0xf0)
        {
            (*pLine) = (GX_UBYTE)(gray << 4);
        }
        else
        {
            (*pLine) |= gray;
        }
    }

    image_reader -> gx_image_reader_putdata_mask >>= 4;

    if (!image_reader -> gx_image_reader_putdata_mask)
    {
        image_reader -> gx_image_reader_putdata++;
        image_reader -> gx_image_reader_putdata_mask = 0xf0;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_4bit_grayscale_transparent_write   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes an 4bit grayscale format pixel                 */
/*    (with transparency)to output pixelmap data structure.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rgb2gray             Convert RGB value to grayscale*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_4bit_grayscale_transparent_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_UBYTE *pAuxData;
GX_UBYTE  gray;
GX_UBYTE  color;

    if (!image_reader -> gx_image_reader_size_testing)
    {
        _gx_image_reader_rgb2gray(pixel, &gray);
        gray >>= 4;

        color = (GX_UBYTE)(gray | (gray << 4));
        pixel -> gx_pixel_red = color;
        pixel -> gx_pixel_green = color;
        pixel -> gx_pixel_blue = color;

        pLine = image_reader -> gx_image_reader_putdata;

        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_COMPRESS)
        {
            /* with compression, with transparent: 1 byte color
               0x00-0x0f: pixel value
               0xff:      transparent value */
            if (pixel -> gx_pixel_alpha < 128)
            {
                (*pLine) = GX_TRANSPARENT_COLOR;
            }
            else
            {
                (*pLine) = gray;
            }
        }
        else
        {
            /* without compression, with transparent: half byte color,
               1-bit transparent go into auxdata. */
            if (image_reader -> gx_image_reader_putdata_mask & 0xf0)
            {
                (*pLine) = (GX_UBYTE)(gray << 4);
            }
            else
            {
                (*pLine) |= gray;
            }

            pAuxData = image_reader -> gx_image_reader_putauxdata;

            if (pixel -> gx_pixel_alpha > 128)
            {
                (*pAuxData) &= (GX_UBYTE)(~image_reader -> gx_image_reader_putauxdata_mask);
            }
            else
            {
                (*pAuxData) |= image_reader -> gx_image_reader_putauxdata_mask;
            }
        }
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_COMPRESS)
    {
        image_reader -> gx_image_reader_putdata++;
    }
    else
    {

        image_reader -> gx_image_reader_putdata_mask >>= 4;

        if (!image_reader -> gx_image_reader_putdata_mask)
        {
            image_reader -> gx_image_reader_putdata++;
            image_reader -> gx_image_reader_putdata_mask = 0xf0;
        }

        image_reader -> gx_image_reader_putauxdata_mask >>= 1;
        if (!image_reader -> gx_image_reader_putauxdata_mask)
        {
            image_reader -> gx_image_reader_putauxdata++;
            image_reader -> gx_image_reader_putauxdata_mask = 0x80;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_monochrome_pixel_write             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a monochrome format pixel (no transparency) to */
/*    output pixelmap data structure.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rgb2gray             Convert RGB value to grayscale*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_monochrome_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_UBYTE  gray;
GX_UBYTE  color;

    pLine = image_reader -> gx_image_reader_putdata;

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_COMPRESS)
    {
        /* [7-bit counter][1-bit color]*/
        if (!image_reader -> gx_image_reader_size_testing)
        {
            _gx_image_reader_rgb2gray(pixel, &gray);
            if (gray > image_reader -> gx_image_reader_mono_shreshold)
            {
                (*pLine) |= 0x01;
            }
            else
            {
                (*pLine) &= 0xfe;
            }
        }
        image_reader -> gx_image_reader_putdata++;
    }
    else
    {
        /* [1-bit color]*/
        _gx_image_reader_rgb2gray(pixel, &gray);
        if (gray > image_reader -> gx_image_reader_mono_shreshold)
        {
            (*pLine) |= image_reader -> gx_image_reader_putdata_mask;
            color = 255;
        }
        else
        {
            (*pLine) &= (GX_UBYTE)(~image_reader -> gx_image_reader_putdata_mask);
            color = 0;
        }

        pixel -> gx_pixel_red = color;
        pixel -> gx_pixel_green = color;
        pixel -> gx_pixel_blue = color;
        pixel -> gx_pixel_alpha = 0xff;

        image_reader -> gx_image_reader_putdata_mask >>= 1;

        if (!image_reader -> gx_image_reader_putdata_mask)
        {
            image_reader -> gx_image_reader_putdata++;
            image_reader -> gx_image_reader_putdata_mask = 0x80;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_monochrome_transparent_write       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function writes a monochrome format pixel (with transparency)  */
/*    to output pixelmap data structure.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pixel                                 Pixel to write.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_rgb2gray             Convert RGB value to grayscale*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_pixel_write_callback_set                           */
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
static UINT _gx_image_reader_monochrome_transparent_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
GX_UBYTE *pLine;
GX_UBYTE  gray;
GX_UBYTE  trans_mask;
GX_UBYTE  color;

    pLine = image_reader -> gx_image_reader_putdata;

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_COMPRESS)
    {
        /* [6-bit counter][1-bit color][1-bit transparency]*/
        if (!image_reader -> gx_image_reader_size_testing)
        {
            if (pixel -> gx_pixel_alpha < 128)
            {
                /* Mark as transparency. */
                (*pLine) &= 0xfc;
            }
            else
            {
                (*pLine) |= 0x01;

                /* Write color: 0 or 1. */
                _gx_image_reader_rgb2gray(pixel, &gray);
                if (gray > image_reader -> gx_image_reader_mono_shreshold)
                {
                    (*pLine) |= 0x02;
                }
                else
                {
                    (*pLine) &= 0xfd;
                }
            }
        }
        image_reader -> gx_image_reader_putdata++;
    }
    else
    {
        /* [1-bit color][1-bit transparency]*/
        trans_mask = image_reader -> gx_image_reader_putdata_mask >> 1;
        if (pixel -> gx_pixel_alpha < 128)
        {
            /* Mark as transparency. */
            (*pLine) &= (GX_UBYTE)(~trans_mask);
            (*pLine) &= (GX_UBYTE)(~image_reader -> gx_image_reader_putdata_mask);
        }
        else
        {
            (*pLine) |= trans_mask;

            _gx_image_reader_rgb2gray(pixel, &gray);
            if (gray > image_reader -> gx_image_reader_mono_shreshold)
            {
                (*pLine) |= image_reader -> gx_image_reader_putdata_mask;
                color = 255;
            }
            else
            {
                (*pLine) &= (GX_UBYTE)(~image_reader -> gx_image_reader_putdata_mask);
                color = 0;
            }
            pixel -> gx_pixel_red = color;
            pixel -> gx_pixel_green = color;
            pixel -> gx_pixel_blue = color;
        }

        image_reader -> gx_image_reader_putdata_mask >>= 2;

        if (!image_reader -> gx_image_reader_putdata_mask)
        {
            image_reader -> gx_image_reader_putdata++;
            image_reader -> gx_image_reader_putdata_mask = 0x80;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_pixel_rotated_write_callback_set   PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets pixel write callback of the image reader when    */
/*    rotation mode is set.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    outmap                                Outpu pixelmap.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_565rgb_rotated_pixel_write                         */
/*                                          Write 565rgb format pixel in  */
/*                                            rotation mode               */
/*    _gx_image_reader_24xrgb_rotated_pixel_write                         */
/*                                          Write 24xrgn format pixel in  */
/*                                            rotation mode               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_pixel_write_callback_set                           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            added 24xrgb format support,*/
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_pixel_rotated_write_callback_set(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
    switch (outmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_565RGB:
        image_reader -> gx_image_reader_pixel_write = _gx_image_reader_565rgb_rotated_pixel_write;

        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ROTATE_CW)
        {
            outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ROTATED_90;

            image_reader -> gx_image_reader_putdatarow = (GX_UBYTE *)outmap -> gx_pixelmap_data;
            image_reader -> gx_image_reader_putdatarow += (outmap -> gx_pixelmap_width - 1) * (outmap -> gx_pixelmap_height << 1);

            if (outmap -> gx_pixelmap_aux_data)
            {
                image_reader -> gx_image_reader_putauxdatarow = (GX_UBYTE *)outmap -> gx_pixelmap_aux_data;
                image_reader -> gx_image_reader_putauxdatarow += (outmap -> gx_pixelmap_width - 1) * outmap -> gx_pixelmap_height;
            }

            image_reader -> gx_image_reader_putdatarow_stride = 2;
            image_reader -> gx_image_reader_putauxdatarow_stride = 1;
        }
        else
        {
            outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ROTATED_270;

            image_reader -> gx_image_reader_putdatarow = (GX_UBYTE *)outmap -> gx_pixelmap_data;
            image_reader -> gx_image_reader_putdatarow += (outmap -> gx_pixelmap_height - 1) * 2;

            if (outmap -> gx_pixelmap_aux_data)
            {
                image_reader -> gx_image_reader_putauxdatarow = (GX_UBYTE *)outmap -> gx_pixelmap_aux_data;
                image_reader -> gx_image_reader_putauxdatarow += (outmap -> gx_pixelmap_height - 1);
            }

            image_reader -> gx_image_reader_putdatarow_stride = -2;
            image_reader -> gx_image_reader_putauxdatarow_stride = -1;
        }
        break;

    case GX_COLOR_FORMAT_24XRGB:
        image_reader -> gx_image_reader_pixel_write = _gx_image_reader_24xrgb_rotated_pixel_write;

        if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ROTATE_CW)
        {
            outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ROTATED_90;

            image_reader -> gx_image_reader_putdatarow = (GX_UBYTE *)outmap -> gx_pixelmap_data;
            image_reader -> gx_image_reader_putdatarow += (outmap -> gx_pixelmap_width - 1) * (outmap -> gx_pixelmap_height << 2);

            image_reader -> gx_image_reader_putdatarow_stride = 4;
        }
        else
        {
            outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ROTATED_270;

            image_reader -> gx_image_reader_putdatarow = (GX_UBYTE *)outmap -> gx_pixelmap_data;
            image_reader -> gx_image_reader_putdatarow += ((outmap -> gx_pixelmap_height - 1) << 2);

            image_reader -> gx_image_reader_putdatarow_stride = -4;
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
/*    _gx_image_reader_pixel_write_callback_set           PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets pixel write callback of the image reader.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block    */
/*    outmap                                Outpu pixelmap.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_565rgb_rle_pixel_write                             */
/*                                          Write 565 rgb fromat pixel    */
/*                                            in compress mode            */
/*    _gx_image_reader_565rgb_pixel_write   Write 565 rgb format pixel    */
/*    _gx_image_reader_1555xrgb_rle_pixel_write                           */
/*                                          Write 1555xrgb format pixel   */
/*                                            in compress mode            */
/*    _gx_image_reader_1555xrgb_pixel_write Write 1555xrgb format pixel   */
/*    _gx_image_reader_4444argb_transparent_write                         */
/*                                          Write 4444argb format pixel   */
/*                                            with transparency           */
/*    _gx_image_reader_4444argb_pixel_write Write 4444argb format pixel   */
/*    _gx_image_reader_32argb_pixel_write   Write 32argb format pixel     */
/*    _gx_image_reader_24xrgb_pixel_write   Write 24xrgb format pixel     */
/*    _gx_image_reader_8bit_alpha_write     Write 8bit alphamap format    */
/*                                            pixel                       */
/*    _gx_image_reader_8bit_palette_write   Write 8bit palette format     */
/*                                            pixel                       */
/*    _gx_image_reader_4bit_grayscale_transparent_write                   */
/*                                          Write 4bit grayscale format   */
/*                                            pixel with transparency     */
/*    _gx_image_reader_4bit_grayscale_pixel_write                         */
/*                                          Write 4bit grayscale format   */
/*                                            pixel                       */
/*    _gx_image_reader_monochrome_transparent_write                       */
/*                                          Write 1bpp format pixel with  */
/*                                            transparency                */
/*    _gx_image_reader_monochrome_pixel_write                             */
/*                                          Write 1bpp format pixel       */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocator function          */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            supported rotation mode,    */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_image_reader_pixel_write_callback_set(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
    /* Allocate memory for data of the output pixelmap. */
    if (outmap -> gx_pixelmap_data_size)
    {
        outmap -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(outmap -> gx_pixelmap_data_size);

        if (outmap -> gx_pixelmap_data == GX_NULL)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        memset((void *)outmap -> gx_pixelmap_data, 0, outmap -> gx_pixelmap_data_size);

        image_reader -> gx_image_reader_putdata = (GX_UBYTE *)outmap -> gx_pixelmap_data;
    }

    /* Allocate memory for aux data of the output pixelmap. */
    if (outmap -> gx_pixelmap_aux_data_size)
    {
        outmap -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(outmap -> gx_pixelmap_aux_data_size);

        if (outmap -> gx_pixelmap_aux_data == GX_NULL)
        {
            _gx_system_memory_free((void *)outmap -> gx_pixelmap_data);
            outmap -> gx_pixelmap_data = GX_NULL;
            return GX_SYSTEM_MEMORY_ERROR;
        }

        memset((void *)outmap -> gx_pixelmap_aux_data, 0, outmap -> gx_pixelmap_aux_data_size);

        image_reader -> gx_image_reader_putauxdata = (GX_UBYTE *)outmap -> gx_pixelmap_aux_data;
    }

    if (image_reader -> gx_image_reader_mode & (GX_IMAGE_READER_MODE_ROTATE_CW | GX_IMAGE_READER_MODE_ROTATE_CCW))
    {
        return _gx_image_reader_pixel_rotated_write_callback_set(image_reader, outmap);
    }

    /* Set pixel write callback.  */
    switch (outmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_565RGB:
        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_565rgb_rle_pixel_write;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_565rgb_pixel_write;
        }
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_1555xrgb_rle_pixel_write;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_1555xrgb_pixel_write;
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_4444argb_transparent_write;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_4444argb_pixel_write;
        }
        break;

    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_32argb_pixel_write;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_24xrgb_pixel_write;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
        image_reader -> gx_image_reader_pixel_write = _gx_image_reader_8bit_alpha_write;
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        image_reader -> gx_image_reader_pixel_write = _gx_image_reader_8bit_palette_write;
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_4bit_grayscale_transparent_write;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_4bit_grayscale_pixel_write;
        }
        image_reader -> gx_image_reader_putdata_mask = 0xf0;
        image_reader -> gx_image_reader_putauxdata_mask = 0x80;
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        if (outmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_monochrome_transparent_write;
        }
        else
        {
            image_reader -> gx_image_reader_pixel_write = _gx_image_reader_monochrome_pixel_write;
        }
        image_reader -> gx_image_reader_putdata_mask = 0x80;
        break;

    default:
        return GX_NOT_SUPPORTED;
    }

    return GX_SUCCESS;
}
#endif

