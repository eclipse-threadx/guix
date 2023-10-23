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
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_gx_image_type_get                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves the image type and stores the image type    */
/*    in the GX_IMAGE_READER control block.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
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
static UINT _gx_image_reader_image_type_get(GX_IMAGE_READER *image_reader)
{
GX_UBYTE        image_type = 0;
GX_CONST GX_UBYTE *buffer;

    buffer = image_reader -> gx_image_reader_source_data;

    if (buffer)
    {
        if (buffer[1] == 'P')
        {
            image_type = GX_IMAGE_TYPE_PNG;
        }
        else
        {
            if (buffer[6] == 'J')
            {
                image_type = GX_IMAGE_TYPE_JPG;
            }
            else
            {
                if (buffer[1] == 0xd8 && buffer[0] == 0xff && (buffer[2] == 0xff))
                {
                    image_type = GX_IMAGE_TYPE_JPG;
                }
            }
        }
    }

    image_reader -> gx_image_reader_image_type = image_type;
    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_image_decode                       PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function decodes input image to raw pixelmap format.           */
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
/*    _gx_image_reader_image_type_get                                     */
/*    _gx_utility_jpeg_decode                                             */
/*    _gx_utility_png_decode                                              */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            modified jpeg decode        */
/*                                            function parameter list,    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
UINT _gx_image_reader_image_decode(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
{
UINT status;

    /* Get image type. */
    _gx_image_reader_image_type_get(image_reader);

    switch (image_reader -> gx_image_reader_image_type)
    {
    case GX_IMAGE_TYPE_JPG:
        status = _gx_image_reader_jpeg_decode(image_reader, outmap);
        break;

    case GX_IMAGE_TYPE_PNG:
        status = _gx_image_reader_png_decode(image_reader -> gx_image_reader_source_data, image_reader -> gx_image_reader_source_data_size, outmap);
        if (outmap -> gx_pixelmap_transparent_color)
        {
            image_reader -> gx_image_reader_png_trans = (GX_COLOR *)outmap -> gx_pixelmap_transparent_color;
        }
        break;

    default:
        return GX_NOT_SUPPORTED;
    }

    if (!(outmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA))
    {
        image_reader -> gx_image_reader_mode = (GX_UBYTE)(image_reader -> gx_image_reader_mode & (ULONG)(~GX_IMAGE_READER_MODE_ALPHA));
    }

    return status;
}
#endif

