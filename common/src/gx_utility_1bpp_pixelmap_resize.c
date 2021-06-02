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
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_raw_resize                PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1bpp pixelmap resize function that handles uncompress, without      */
/*    transparent channel.                                                */
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
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_resize                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_1bpp_pixelmap_raw_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
/* The pixelmap resize function is implemented from nearest neighbor
   image scaling algorithm.  */
GX_UBYTE *get;
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE  putmask;
GX_UBYTE  getmask;
INT       putstride;
INT       getstride;
INT       xradio;
INT       yradio;
INT       x;
INT       y;
INT       xx;
INT       yy;

    /* Calculate scale ratio and enlarge it by 256 times to keep precision.  */
    xradio = ((src -> gx_pixelmap_width) << 8) / width;
    yradio = ((src -> gx_pixelmap_height) << 8) / height;

    putstride = (width + 7) >> 3;
    getstride = (src -> gx_pixelmap_width + 7) >> 3;

    /* Fill property values into destination pixelmap structure. */
    destination -> gx_pixelmap_flags = src -> gx_pixelmap_flags;
    destination -> gx_pixelmap_format = src -> gx_pixelmap_format;
    destination -> gx_pixelmap_transparent_color = src -> gx_pixelmap_transparent_color;
    destination -> gx_pixelmap_version_major = src -> gx_pixelmap_version_major;
    destination -> gx_pixelmap_version_minor = src -> gx_pixelmap_version_minor;

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(height * putstride) * sizeof(GX_UBYTE);

    /* Allocate memory to load pixelmap data. */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

    /* Loop through destination's pixel and fill each pixel with the nearest neighbor.  */
    for (y = 0; y < height; y++)
    {
        put = putrow;
        putmask = 0x80;
        for (x = 0; x < width; x++)
        {
            xx = (xradio * x) >> 8;
            yy = (yradio * y) >> 8;

            get = (GX_UBYTE *)src -> gx_pixelmap_data;
            get += yy * getstride;
            get += xx >> 3;

            getmask = (GX_UBYTE)(0x80 >> (xx & 0x07));

            if (*get & getmask)
            {
                *put |= putmask;
            }
            else
            {
                *put &= (GX_UBYTE)(~putmask);
            }

            putmask >>= 1;
            if (putmask == 0)
            {
                put++;
                putmask = 0x80;
            }
        }
        putrow += putstride;
    }

    return GX_SUCCESS;
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_transparent_resize        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1bpp pixelmap resize function that handles uncompress, with         */
/*    transparent channel.                                                */
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
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_resize                                    */
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
static UINT _gx_utility_1bpp_pixelmap_transparent_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
/* The pixelmap resize function is implemented from nearest neighbor
image scaling algorithm.  */
GX_UBYTE *get;
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE  putmask;
GX_UBYTE  puttransmask;
INT       putstride;
INT       getstride;
GX_UBYTE  getmask;
GX_UBYTE  gettransmask;
INT       xradio;
INT       yradio;
INT       x;
INT       y;
INT       xx;
INT       yy;

    /* Calculate scale ratio and enlarge it by 256 times to keep precision.  */
    xradio = ((src -> gx_pixelmap_width) << 8) / width;
    yradio = ((src -> gx_pixelmap_height) << 8) / height;

    putstride = (width + 3) >> 2;
    getstride = (src -> gx_pixelmap_width + 3) >> 2;

    /* Fill property values into destination pixelmap structure. */
    destination -> gx_pixelmap_flags = src -> gx_pixelmap_flags;
    destination -> gx_pixelmap_format = src -> gx_pixelmap_format;
    destination -> gx_pixelmap_transparent_color = src -> gx_pixelmap_transparent_color;
    destination -> gx_pixelmap_version_major = src -> gx_pixelmap_version_major;
    destination -> gx_pixelmap_version_minor = src -> gx_pixelmap_version_minor;

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(height * putstride) * sizeof(GX_UBYTE);

    /* Allocate memory to load pixelmap data. */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

    /* Loop through destination's pixel and fill each pixel with the nearest neighbor.  */
    for (y = 0; y < height; y++)
    {
        put = putrow;
        putmask = 0x80;
        puttransmask = 0x40;
        for (x = 0; x < width; x++)
        {
            xx = (xradio * x) >> 8;
            yy = (yradio * y) >> 8;

            /* set bits first. */
            *put &= (GX_UBYTE)(~putmask);
            *put &= (GX_UBYTE)(~puttransmask);

            /* get pixel data */
            get = (GX_UBYTE *)src -> gx_pixelmap_data;
            get += yy * getstride;
            get += xx >> 2;

            gettransmask = (GX_UBYTE)(0x40 >> ((xx & 0x03) << 1));
            if (*get & gettransmask)
            {
                *put |= puttransmask;
                getmask = (GX_UBYTE)(gettransmask << 1);
                if (*get & getmask)
                {
                    *put |= putmask;
                }
            }

            putmask >>= 2;
            puttransmask >>= 2;
            if (puttransmask == 0)
            {
                put++;
                putmask = 0x80;
                puttransmask = 0x40;
            }
        }
        putrow += putstride;
    }

    return GX_SUCCESS;
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_resize                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1bpp pixelmap resize function that handles uncompress, with or      */
/*    without transparent channel.                                        */
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
/*     _gx_utility_1bpp_pixelmap_transparent_resize                       */
/*                                           Real pixelmap resize routine */
/*     _gx_utility_1bpp_pixelmap_raw_resize  Real pixelmap resize routine */
/*                                                                        */
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
UINT _gx_utility_1bpp_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        /* transparent, no compression */
        status = _gx_utility_1bpp_pixelmap_transparent_resize(src, destination, width, height);
    }
    else
    {
        /* no compression or alpha */
        status = _gx_utility_1bpp_pixelmap_raw_resize(src, destination, width, height);
    }

    return status;
}

