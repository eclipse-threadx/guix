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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_pixelmap_resize                         PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service resize a pixelmap and return the resized pixelmap.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The source pixelmap           */
/*    destination                           The resized pixelmap to be    */
/*                                            returned                    */
/*    width                                 New width                     */
/*    height                                New height                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_32argb_pixelmap_resize    Resize 32bpp pixelmap         */
/*    _gx_utility_16bpp_pixelmap_resize     Resize 565rgb pixelmap        */
/*    _gx_utility_1555xrgb_pixelmap_resize  Resize 1555xrgb pixelmap      */
/*    _gx_utility_4444argb_pixelmap_resize  Resize 4444argb pixelmap      */
/*    _gx_utility_8bpp_pixelmap_resize      Resize 8bpp pixelmap          */
/*    _gx_utility_8bit_alphamap_resize      Resize 8bit alphamap          */
/*    _gx_utility_4bpp_pixelmap_resize      Resize 4bpp pixelmap          */
/*    _gx_utility_1bpp_pixelmap_resize      Resize 1bpp pixelmap          */
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
/*                                            added 565bgr format support,*/
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
UINT        status = GX_SUCCESS;
GX_PIXELMAP rotated_src;

    memset(destination, 0, sizeof(GX_PIXELMAP));

    /* Limited pixelmap width to 14 bits. */
    if (src -> gx_pixelmap_width > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_WIDTH;
    }

    /* Limited pixelmap height to 14 bits. */
    if (src -> gx_pixelmap_height > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_HEIGHT;
    }

    if ((src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90) ||
        (src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_270))
    {
        rotated_src = (*src);
        GX_SWAP_VALS(rotated_src.gx_pixelmap_width, rotated_src.gx_pixelmap_height);
        GX_SWAP_VALS(width, height);

        src = &rotated_src;
    }

    switch (src -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:

        /* Call 32argb pixelmap resize.  */
        status = _gx_utility_32argb_pixelmap_resize(src, destination, width, height);
        break;
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
        /* Call 16bpp pixelmap resize.  */
        status = _gx_utility_16bpp_pixelmap_resize(src, destination, width, height);
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        status = _gx_utility_1555xrgb_pixelmap_resize(src, destination, width, height);
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        status = _gx_utility_4444argb_pixelmap_resize(src, destination, width, height);
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        /* Call 8bpp pixelmap resize.  */
        status = _gx_utility_8bpp_pixelmap_resize(src, destination, width, height);
        break;

    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
        status = _gx_utility_8bit_alphamap_resize(src, destination, width, height);
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        status = _gx_utility_4bpp_pixelmap_resize(src, destination, width, height);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        status = _gx_utility_1bpp_pixelmap_resize(src, destination, width, height);
        break;

    default:
        status = GX_NOT_SUPPORTED;
        break;
    }

    if ((src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90) ||
        (src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_270))
    {
        GX_SWAP_VALS(destination -> gx_pixelmap_width, destination -> gx_pixelmap_height);
    }

    return status;
}

