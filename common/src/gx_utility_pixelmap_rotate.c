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
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_pixelmap_rotation                       PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service rotate a pixelmap.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The pixelmap to rotate        */
/*    angle                                 The angle to rotate           */
/*    destination                           Destination buffer for        */
/*                                            rotated pixelmap.           */
/*    rot_cx                                X coordinate of rotation      */
/*                                            center                      */
/*    rot_cy                                Y coordinate of rotation      */
/*                                            center                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_32argb_pixelmap_rotate    Rotate 32bpp pixelmap         */
/*    _gx_utility_565rgb_pixelmap_rotate    Rotate 565rgb pixelmap        */
/*    _gx_utility_4444argb_pixelmap_rotate  Rotate 4444argb pixelmap      */
/*    _gx_utility_8bpp_pixelmap_rotate      Rotate 8bit palette pixelmap  */
/*    _gx_utility_332rgb_pixelmap_rotate    Rotate 332rgb pixelmap        */
/*    _gx_utility_8bit_alphamap_rotate      Rotate 8bit alphamap          */
/*    _gx_utility_4bpp_pixelmap_rotate      Rotate 4bpp pixelmap          */
/*    _gx_utility_1bpp_pixelmap_rotate      Rotate 1bpp pixelmap          */
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
UINT _gx_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
UINT        status = GX_SUCCESS;
GX_PIXELMAP rotated_src;

    /* Limit pixelmap width to 14bits. */
    if (src -> gx_pixelmap_width > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_WIDTH;
    }

    /* Limit pixelmap height to 14bits. */
    if (src -> gx_pixelmap_height > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_HEIGHT;
    }

    if ((src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90) ||
        (src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_270))
    {
        rotated_src = (*src);
        GX_SWAP_VALS(rotated_src.gx_pixelmap_width, rotated_src.gx_pixelmap_height);
        src = &rotated_src;

        if (rot_cx && rot_cy)
        {
            GX_SWAP_VALS(*rot_cx, *rot_cy);

            if (src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
            {
                *rot_cy = src -> gx_pixelmap_height - 1 - (*rot_cy);
            }
            else
            {
                *rot_cx = src -> gx_pixelmap_width - 1 - (*rot_cx);
            }
        }
    }

    angle = angle % 360;

    if (angle == 0)
    {
        return GX_INVALID_VALUE;
    }
    else if (angle < 0)
    {
        angle += 360;
    }

    if ((_gx_system_memory_allocator == GX_NULL) || (_gx_system_memory_free == GX_NULL))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        return GX_INVALID_FORMAT;
    }

    if ((angle % 90) == 0)
    {
        _gx_utility_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
    }
    else
    {
        memset(destination, 0, sizeof(GX_PIXELMAP));
        destination -> gx_pixelmap_format = src -> gx_pixelmap_format;
        destination -> gx_pixelmap_version_major = src -> gx_pixelmap_version_major;
        destination -> gx_pixelmap_version_minor = src -> gx_pixelmap_version_major;
        destination -> gx_pixelmap_flags = src -> gx_pixelmap_flags;

        switch (src -> gx_pixelmap_format)
        {
        case GX_COLOR_FORMAT_32ARGB:
        case GX_COLOR_FORMAT_24XRGB:
            /* Call 32argb pixelmap rotate. */
            status = _gx_utility_32argb_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_565RGB:
        case GX_COLOR_FORMAT_565BGR:
            /* Call 565rgb pixelmap rotate.  */
            status = _gx_utility_565rgb_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_1555XRGB:
            status = _gx_utility_1555xrgb_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_4444ARGB:
            status = _gx_utility_4444argb_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
            status = _gx_utility_332rgb_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_8BIT_PALETTE:
            /* Call 8bpp pixelmap rotate.  */
            status = _gx_utility_8bpp_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
            /* Call 8bit alphamap rotate.  */
            status = _gx_utility_8bit_alphamap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_4BIT_GRAY:
            status = _gx_utility_4bpp_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        case GX_COLOR_FORMAT_MONOCHROME:
            status = _gx_utility_1bpp_pixelmap_rotate(src, angle, destination, rot_cx, rot_cy);
            break;

        default:
            return GX_INVALID_FORMAT;
        }
    }

    if ((src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90) ||
        (src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_270))
    {
        GX_SWAP_VALS(destination -> gx_pixelmap_width, destination -> gx_pixelmap_height);

        if (rot_cx && rot_cy)
        {
            GX_SWAP_VALS(*rot_cx, *rot_cy);

            if (src -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
            {
                *rot_cx = destination -> gx_pixelmap_width - 1 - (*rot_cx);
            }
            else
            {
                *rot_cy = destination -> gx_pixelmap_height - 1 - (*rot_cy);
            }
        }
    }

    return status;
}

