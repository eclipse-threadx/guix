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
#include "gx_context.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_pixelmap_simple_rotation                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service rotate a pixelmap by 90, 180 or 270 degree.            */
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
/*    _gx_utility_32argb_pixelmap_simple_rotate                           */
/*                                          Rotate 32bpp pixelmap with    */
/*                                            simple case                 */
/*    _gx_utility_565rgb_pixelmap_simple_rotate                           */
/*                                          Rotate 565rgb pixelmap with   */
/*                                            simple case                 */
/*    _gx_utility_4444argb_pixelmap_simple_rotate                         */
/*                                          Rotate 4444argb pixelmap with */
/*                                            simple case                 */
/*    _gx_utility_8bpp_pixelmap_simple_rotate                             */
/*                                          Rotate 8bit palette pixelmap  */
/*                                            with simple case            */
/*    _gx_utility_332rgb_pixelmap_simple_rotate                           */
/*                                          Rotate 332rgb pixelmap with   */
/*                                            simple case                 */
/*    _gx_utility_4bpp_pixelmap_simple_rotate                             */
/*                                          Rotate 4bpp pixelmap with     */
/*                                            simple case                 */
/*    _gx_utility_1bpp_pixelmap_simple_rotate                             */
/*                                          Rotate 1bpp pixelmap with     */
/*                                            simple case                 */
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
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
    angle = angle % 360;

    if ((angle == 0) || (angle % 90))
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
        _gx_utility_32argb_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_1555XRGB:
        /* Call 565rgb pixelmap rotate.  */
        _gx_utility_565rgb_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        _gx_utility_4444argb_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    case  GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        /* Call 332rgb pixelmap rotate.  */
        _gx_utility_332rgb_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
        /* Call 8bpp pixelmap rotate.  */
        _gx_utility_8bpp_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        _gx_utility_4bpp_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        _gx_utility_1bpp_pixelmap_simple_rotate(src, angle, destination, rot_cx, rot_cy);
        break;

    default:
        return GX_INVALID_FORMAT;
    }
    if (destination -> gx_pixelmap_data)
    {
        return GX_SUCCESS;
    }
    return GX_FAILURE;
}

