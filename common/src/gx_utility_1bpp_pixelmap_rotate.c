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
#include "gx_display.h"
#include "gx_context.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_rotate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1bpp pixelmap rotation function that handles uncompress, with or    */
/*    without alpha channel.                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The pixelmap to be rotated    */
/*    angle                                 The angle to be rotated       */
/*    destination                           The rotated bitmap to be      */
/*                                            returned                    */
/*    rot_cx                                X coordinate of rotation      */
/*                                            center                      */
/*    rot_cy                                Y coordinate of rotation      */
/*                                            center                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
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
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_1bpp_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *putrow;
GX_UBYTE *put;
GX_UBYTE *get;
INT       srcxres;
INT       srcyres;
INT       cosv;
INT       sinv;
INT       idxminx, idxmaxx, idxmaxy;
INT       mx[] = {-1, 1, 1, -1};
INT       my[] = {1, 1, -1, -1};
INT       xres;
INT       yres;
INT       width, height;
INT       x, y;
INT       xx, yy;
INT       putstride;
INT       getstride;
GX_UBYTE  putmask;
GX_UBYTE  putTransmask;
GX_UBYTE  getTransmask;
GX_UBYTE  getmask;
GX_BOOL   InputAlpha = GX_FALSE;

    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = src -> gx_pixelmap_width >> 1;
    srcyres = src -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT((mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv));
    yres = GX_FIXED_VAL_TO_INT((my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv));

    /* Calculate destination width and height. */
    width = (xres << 1);
    height = (yres << 1);

    if (rot_cx && rot_cy)
    {
        /* Calculate the new rotation axis. */
        x = ((*rot_cx) - srcxres) * cosv - ((*rot_cy) - srcyres) * sinv;
        y = ((*rot_cy) - srcyres) * cosv + ((*rot_cx) - srcxres) * sinv;

        x = GX_FIXED_VAL_TO_INT(x) + xres;
        y = GX_FIXED_VAL_TO_INT(y) + yres;

        srcxres = *rot_cx;
        srcyres = *rot_cy;

        *rot_cx = x;
        *rot_cy = y;

        xres = *rot_cx;
        yres = *rot_cy;
    }

    /* Set width and height of destination pixelmap.  */
    destination -> gx_pixelmap_width = (GX_VALUE)width;
    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_flags |= GX_PIXELMAP_TRANSPARENT;
    putstride = (width + 3) >> 2;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        getstride = (src -> gx_pixelmap_width + 3) >> 2;
        InputAlpha = GX_TRUE;
    }
    else
    {
        getstride = (src -> gx_pixelmap_width + 7) >> 3;
    }

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(putstride * height) * sizeof(GX_UBYTE);
    /* Allocate memory for destination pixelmap to load pixel information.  */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

    /* For every pixel in destination bitmap, find its position in source bitmap,
       and set the pixel with the value in source bitmap.  */
    for (y = 0; y < height; y++)
    {
        put = putrow;
        putmask = 0x80;
        putTransmask = 0x40;

        for (x = 0; x < width; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xx = GX_FIXED_VAL_TO_INT(xx) + srcxres;
            yy = GX_FIXED_VAL_TO_INT(yy) + srcyres;

            *put = (GX_UBYTE)(*put & (~putTransmask));
            *put = (GX_UBYTE)(*put & (~putmask));
            if ((xx >= 0) && (xx < src -> gx_pixelmap_width) &&
                (yy >= 0) && (yy < src -> gx_pixelmap_height))
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += yy * getstride;
                if (InputAlpha)
                {
                    get += xx >> 2;
                    getTransmask = (GX_UBYTE)(0x40 >> ((xx & 0x03) << 1));
                    if (*get & getTransmask)
                    {
                        *put |= putTransmask;
                        getmask = (GX_UBYTE)(getTransmask << 1);
                        if (*get & getmask)
                        {
                            *put |= putmask;
                        }
                    }
                }
                else
                {
                    *put |= putTransmask;
                    get += xx >> 3;
                    getmask = (GX_UBYTE)(0x80 >> (xx & 0x07));
                    if (*get & getmask)
                    {
                        *put |= putmask;
                    }
                }
            }

            putTransmask >>= 2;
            putmask >>= 2;

            if (putTransmask == 0)
            {
                put++;
                putTransmask = 0x40;
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
/*    _gx_utility_1bpp_pixelmap_simple_raw_rotate         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles 90, 180 and 270 degree        */
/*      rotation of an uncompressed pixelmap with or wihout alpha.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The pixelmap to be rotated    */
/*    angle                                 The angle to be rotated       */
/*    destination                           The rotated bitmap to be      */
/*                                            returned                    */
/*    rot_cx                                X coordinate of rotation      */
/*                                            center                      */
/*    rot_cy                                Y coordinate of rotation      */
/*                                            center                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_simple_rotate                             */
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
static UINT _gx_utility_1bpp_pixelmap_simple_raw_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE *get;
INT       width, height;
INT       x, y;
INT       putstride;
GX_UBYTE  getmask;
GX_UBYTE  putmask;
INT       getstride;

    getstride = (src -> gx_pixelmap_width + 7) >> 3;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    if (angle == 180)
    {
        GX_SWAP_VALS(width, height);
    }

    putstride = (width + 7) >> 3;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(putstride * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (angle == 90)
    {
        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putmask = 0x80;
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (width - 1 - x) * getstride;
                get += y >> 3;
                getmask = (GX_UBYTE)(0x80 >> (y & 0x07));

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

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            *rot_cx = (width - 1 - y);
            *rot_cy = x;
        }
    }
    else if (angle == 180)
    {

        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putmask = 0x80;
            for (x = 0; x < width; x++)
            {
                getmask = (GX_UBYTE)(0x80 >> ((width - 1 - x) & 0x07));

                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (height - 1 - y) * getstride;
                get += (width - 1 - x) >> 3;

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
        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            *rot_cx = (width - 1 - x);
            *rot_cy = (height - 1 - y);
        }
    }
    else
    {
        /* angle = 270. */
        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putmask = 0x80;

            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += x * getstride;
                get += (height - 1 - y) >> 3;

                getmask = (GX_UBYTE)(0x80 >> ((height - 1 - y) & 0x07));

                if (getmask & (*get))
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

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            *rot_cx = y;
            *rot_cy = (height - 1 - x);
        }
    }

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_simple_transparent_rotate PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles 90, 180 and 270 degree        */
/*      rotation of an uncompressed pixelmap with or wihout alpha.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The pixelmap to be rotated    */
/*    angle                                 The angle to be rotated       */
/*    destination                           The rotated bitmap to be      */
/*                                            returned                    */
/*    rot_cx                                X coordinate of rotation      */
/*                                            center                      */
/*    rot_cy                                Y coordinate of rotation      */
/*                                            center                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_simple_rotate                             */
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
static UINT _gx_utility_1bpp_pixelmap_simple_transparent_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE *get;
INT       width, height;
INT       x, y;
INT       putstride;
GX_UBYTE  getmask;
GX_UBYTE  putmask;
GX_UBYTE  puttransmask;
GX_UBYTE  gettransmask;
INT       getstride;

    getstride = (src -> gx_pixelmap_width + 3) >> 2;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    if (angle == 180)
    {
        GX_SWAP_VALS(width, height);
    }

    putstride = (width + 3) >> 2;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(putstride * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (angle == 90)
    {
        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            puttransmask = 0x40;
            putmask = 0x80;

            getmask = (GX_UBYTE)(0x80 >> ((y & 0x03) << 1));
            gettransmask = (getmask >> 1);

            for (x = 0; x < width; x++)
            {
                *put &= (GX_UBYTE)(~puttransmask);
                *put &= (GX_UBYTE)(~putmask);

                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (width - 1 - x) * getstride;
                get += y >> 2;

                if (*get & gettransmask)
                {
                    *put |= puttransmask;

                    if (*get & getmask)
                    {
                        *put |= putmask;
                    }
                }
                puttransmask >>= 2;
                putmask >>= 2;
                if (puttransmask == 0)
                {
                    put++;
                    putmask = 0x80;
                    puttransmask = 0x40;
                }
            }
            putrow += putstride;
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            *rot_cx = (width - 1 - y);
            *rot_cy = x;
        }
    }
    else if (angle == 180)
    {
        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putmask = 0x80;
            puttransmask = 0x40;
            for (x = 0; x < width; x++)
            {
                /* set bits first. */
                *put &= (GX_UBYTE)(~putmask);
                *put &= (GX_UBYTE)(~puttransmask);

                gettransmask = (GX_UBYTE)(0x40 >> (((width - 1 - x) & 0x03) << 1));

                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (height - 1 - y) * getstride;
                get += (width - 1 - x) >> 2;

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
        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            *rot_cx = (width - 1 - x);
            *rot_cy = (height - 1 - y);
        }
    }
    else
    {
        /* angle = 270. */
        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putmask = 0x80;
            puttransmask = 0x40;

            for (x = 0; x < width; x++)
            {
                /* set bits first. */
                *put &= (GX_UBYTE)(~putmask);
                *put &= (GX_UBYTE)(~puttransmask);

                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += x * getstride;
                get += (height - 1 - y) >> 2;

                gettransmask = (GX_UBYTE)(0x40 >> (((height - 1 - y) & 0x03) << 1));
                if (gettransmask & (*get))
                {
                    *put |= puttransmask;
                    getmask = (GX_UBYTE)(gettransmask << 1);
                    if (getmask & (*get))
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

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            *rot_cx = y;
            *rot_cy = (height - 1 - x);
        }
    }

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    return GX_SUCCESS;
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_simple_rotate             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles 90, 180 and 270 degree        */
/*      rotation of an uncompressed pixelmap with or wihout alpha.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The pixelmap to be rotated    */
/*    angle                                 The angle to be rotated       */
/*    destination                           The rotated bitmap to be      */
/*                                            returned                    */
/*    rot_cx                                X coordinate of rotation      */
/*                                            center                      */
/*    rot_cy                                Y coordinate of rotation      */
/*                                            center                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_1bpp_pixelmap_simple_transparent_rotate                 */
/*                                          Real pixelmap rotate function */
/*    _gx_utility_1bpp_pixelmap_simple_raw_rotate                         */
/*                                          Real pixelmap rotate function */
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
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_1bpp_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        /* with transparent, no compression */
        status = _gx_utility_1bpp_pixelmap_simple_transparent_rotate(src, angle, destination, rot_cx, rot_cy);
    }
    else
    {
        /* no compression or transparent */
        status = _gx_utility_1bpp_pixelmap_simple_raw_rotate(src, angle, destination, rot_cx, rot_cy);
    }

    return status;
}

