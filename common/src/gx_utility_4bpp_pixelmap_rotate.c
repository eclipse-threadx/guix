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
/*    _gx_utility_4bpp_pixelmap_rotate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    4bpp pixelmap rotation function that handles uncompress, with or    */
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
UINT _gx_utility_4bpp_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
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
INT       putauxstride;
INT       getauxstride;
GX_UBYTE  putmask;
GX_UBYTE  pixel;
GX_UBYTE *putaux;
GX_UBYTE *putauxrow;
GX_UBYTE  putauxmask;
GX_UBYTE  transmask;
GX_UBYTE *getaux;
GX_BOOL   draw = GX_TRUE;

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
    putstride = (width + 1) >> 1;
    putauxstride = (width + 7) >> 3;
    getstride = (src -> gx_pixelmap_width + 1) >> 1;
    getauxstride = 0;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        getauxstride = (src -> gx_pixelmap_width + 7) >> 3;
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

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_aux_data_size = (UINT)(putauxstride * height) * sizeof(GX_UBYTE);

    /* Allocate memory for destination pixelmap to load pixel information.  */
    destination -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_aux_data_size);

    if (destination -> gx_pixelmap_aux_data == GX_NULL)
    {
        _gx_system_memory_free((void *)destination -> gx_pixelmap_data);
        return GX_SYSTEM_MEMORY_ERROR;
    }

    putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;
    putauxrow = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

    /* For every pixel in destination bitmap, find its position in source bitmap,
       and set the pixel with the value in source bitmap.  */
    for (y = 0; y < height; y++)
    {
        put = putrow;
        putaux = putauxrow;
        putmask = 0xf0;
        putauxmask = 0x80;

        for (x = 0; x < width; x++)
        {
            xx = GX_FIXED_VAL_TO_INT((x - xres) * cosv + (y - yres) * sinv);
            yy = GX_FIXED_VAL_TO_INT((y - yres) * cosv - (x - xres) * sinv);

            xx += srcxres;
            yy += srcyres;

            if (getauxstride)
            {
                getaux = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getaux += yy * getauxstride;
                getaux += xx >> 3;
                transmask = (GX_UBYTE)(0x80 >> (xx & 0x07));

                if (transmask & (*getaux))
                {
                    draw = GX_FALSE;
                }
                else
                {
                    draw = GX_TRUE;
                }
            }

            /* Set bits first. */
            *put &= (GX_UBYTE)(~putmask);
            if (draw && (xx >= 0) && (xx < src -> gx_pixelmap_width) &&
                (yy >= 0) && (yy < src -> gx_pixelmap_height))
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += yy * getstride;
                get += xx >> 1;

                if (xx & 1)
                {
                    pixel = *get & 0x0f;
                }
                else
                {
                    pixel = (*get & 0xf0) >> 4;
                }
                pixel |= (GX_UBYTE)(pixel << 4);
                /* set pixel */
                *put |= pixel & putmask;
                /* Set transparent info. */
                *putaux &= (GX_UBYTE)(~putauxmask);
            }
            else
            {
                *putaux |= putauxmask;
            }

            putauxmask >>= 1;
            if (putauxmask == 0)
            {
                putaux++;
                putauxmask = 0x80;
            }

            putmask >>= 4;
            if (putmask == 0)
            {
                put++;
                putmask = 0xf0;
            }
        }
        putrow += putstride;
        putauxrow += putauxstride;
    }

    return GX_SUCCESS;
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_4bpp_pixelmap_simple_raw_rotate         PORTABLE C      */
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
/*   _gx_utility_4bpp_pixelmap_simple_rotate                              */
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
static UINT _gx_utility_4bpp_pixelmap_simple_raw_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE *get;
INT       width, height;
INT       x, y;
INT       putstride;
GX_UBYTE  putmask;
GX_UBYTE  pixel;
INT       getstride;

    getstride = (src -> gx_pixelmap_width + 1) >> 1;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    if (angle == 180)
    {
        GX_SWAP_VALS(width, height);
    }

    putstride = (width + 1) >> 1;

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
            putmask = 0xf0;
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (width - 1 - x) * getstride;
                get += y >> 1;

                if (y & 1)
                {
                    pixel = *get & 0x0f;
                }
                else
                {
                    pixel = (*get & 0xf0) >> 4;
                }
                pixel |= (GX_UBYTE)(pixel << 4);
                *put &= (GX_UBYTE)(~putmask);
                *put |= pixel & putmask;

                putmask >>= 4;
                if (putmask == 0)
                {
                    put++;
                    putmask = 0xf0;
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
            putmask = 0xf0;
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (height - 1 - y) * getstride;
                get += (width - 1 - x) >> 1;

                if ((width - 1 - x) & 1)
                {
                    pixel = *get & 0x0f;
                }
                else
                {
                    pixel = (*get & 0xf0) >> 4;
                }
                pixel |= (GX_UBYTE)(pixel << 4);
                *put &= (GX_UBYTE)(~putmask);
                *put |= pixel & putmask;

                putmask >>= 4;
                if (putmask == 0)
                {
                    put++;
                    putmask = 0xf0;
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
            putmask = 0xf0;

            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += x * getstride;
                get += (height - 1 - y) >> 1;

                if ((height - 1 - y) & 1)
                {
                    pixel = *get & 0x0f;
                }
                else
                {
                    pixel = (*get & 0xf0) >> 4;
                }
                pixel |= (GX_UBYTE)(pixel << 4);
                *put &= (GX_UBYTE)(~putmask);
                *put |= pixel & putmask;

                putmask >>= 4;
                if (putmask == 0)
                {
                    put++;
                    putmask = 0xf0;
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
    destination -> gx_pixelmap_width  = (GX_VALUE)width;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_4bpp_pixelmap_simple_transparent_rotate PORTABLE C      */
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
/*   _gx_utility_4bpp_pixelmap_simple_rotate                              */
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
static UINT _gx_utility_4bpp_pixelmap_simple_transparent_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE *putaux;
GX_UBYTE *putauxrow;
GX_UBYTE *get;
INT       width, height;
INT       x, y;
INT       putstride;
INT       putauxstride;
GX_UBYTE  putmask;
GX_UBYTE  pixel;
GX_UBYTE  transmask;
GX_UBYTE  putauxmask;
GX_UBYTE *getaux;
INT       getstride;
INT       getauxstride;

    getstride = (src -> gx_pixelmap_width + 1) >> 1;
    getauxstride = (src -> gx_pixelmap_width + 7) >> 3;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    if (angle == 180)
    {
        GX_SWAP_VALS(width, height);
    }

    putstride = (width + 1) >> 1;
    putauxstride = (width + 7) >> 3;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(putstride * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_aux_data_size = (UINT)(putauxstride * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_aux_data_size);

    if (destination -> gx_pixelmap_aux_data == GX_NULL)
    {
        _gx_system_memory_free((void *)destination -> gx_pixelmap_data);
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (angle == 90)
    {
        putrow = (GX_UBYTE *)destination -> gx_pixelmap_data;
        putauxrow = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putaux = putauxrow;
            putmask = 0xf0;
            putauxmask = 0x80;
            for (x = 0; x < width; x++)
            {
                /* set bits first. */
                *put &= (GX_UBYTE)(~putmask);
                getaux = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getaux += (width - 1 - x) * getauxstride;
                getaux += y >> 3;

                transmask = (GX_UBYTE)(0x80 >> (y & 0x07));
                if (transmask & (*getaux))
                {
                    /* transparent pixel, set destination true */
                    *putaux |= putauxmask;
                }
                else
                {
                    /* not transparent, set destination bit to 0. */
                    *putaux &= (GX_UBYTE)(~putauxmask);

                    get = (GX_UBYTE *)src -> gx_pixelmap_data;
                    get += (width - 1 - x) * getstride;
                    get += y >> 1;

                    if (y & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = (*get & 0xf0) >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);

                    /* set data. */
                    *put |= pixel & putmask;
                }

                putmask >>= 4;
                if (putmask == 0)
                {
                    put++;
                    putmask = 0xf0;
                }
                putauxmask >>= 1;
                if (putauxmask == 0)
                {
                    putaux++;
                    putauxmask = 0x80;
                }
            }
            putrow += putstride;
            putauxrow += putauxstride;
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
        putauxrow = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putaux = putauxrow;
            putmask = 0xf0;
            putauxmask = 0x80;
            for (x = 0; x < width; x++)
            {
                /* set bits first. */
                *put &= (GX_UBYTE)(~putmask);
                getaux = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getaux += (height - 1 - y) * getauxstride;
                getaux += (width - 1 - x) >> 3;

                transmask = (GX_UBYTE)(0x80 >> ((width - 1 - x) & 0x07));
                if (transmask & (*getaux))
                {
                    /* transparent pixel, set destination true */
                    *putaux |= putauxmask;
                }
                else
                {
                    /* not transparent, set destination bit to 0. */
                    *putaux &= (GX_UBYTE)(~putauxmask);

                    get = (GX_UBYTE *)src -> gx_pixelmap_data;
                    get += (height - 1 - y) * getstride;
                    get += (width - 1 - x) >> 1;

                    if ((width - 1 - x) & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = (*get & 0xf0) >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);
                    *put |= pixel & putmask;
                }
                putmask >>= 4;
                if (putmask == 0)
                {
                    put++;
                    putmask = 0xf0;
                }
                putauxmask >>= 1;
                if (putauxmask == 0)
                {
                    putaux++;
                    putauxmask = 0x80;
                }
            }
            putrow += putstride;
            putauxrow += putauxstride;
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
        putauxrow = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

        for (y = 0; y < height; y++)
        {
            put = putrow;
            putaux = putauxrow;
            putmask = 0xf0;
            putauxmask = 0x80;

            for (x = 0; x < width; x++)
            {
                /* set bits first. */
                *put &= (GX_UBYTE)(~putmask);
                getaux = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getaux += x * getauxstride;
                getaux += (height - 1 - y) >> 3;

                transmask = (GX_UBYTE)(0x80 >> ((height - 1 - y) & 0x07));
                if (transmask & (*getaux))
                {
                    /* transparent pixel, set destination true */
                    *putaux |= putauxmask;
                }
                else
                {
                    /* not transparent, set destination bit to 0. */
                    *putaux &= (GX_UBYTE)(~putauxmask);

                    get = (GX_UBYTE *)src -> gx_pixelmap_data;
                    get += x * getstride;
                    get += (height - 1 - y) >> 1;

                    if ((height - 1 - y) & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = (*get & 0xf0) >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);
                    *put |= pixel & putmask;
                }
                putmask >>= 4;
                if (putmask == 0)
                {
                    put++;
                    putmask = 0xf0;
                }
                putauxmask >>= 1;
                if (putauxmask == 0)
                {
                    putaux++;
                    putauxmask = 0x80;
                }
            }
            putrow += putstride;
            putauxrow += putauxstride;
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
/*    _gx_utility_4bpp_pixelmap_simple_rotate             PORTABLE C      */
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
/*    _gx_utility_4bpp_pixelmap_simple_transparent_rotate                 */
/*                                          Real pixelmap rotate function */
/*    _gx_utility_4bpp_pixelmap_simple_raw_rotate                         */
/*                                          Real pixelmap rotate function */
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
UINT _gx_utility_4bpp_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        /* with transparent, no compression */
        status = _gx_utility_4bpp_pixelmap_simple_transparent_rotate(src, angle, destination, rot_cx, rot_cy);
    }
    else
    {
        /* no compression or transparent */
        status = _gx_utility_4bpp_pixelmap_simple_raw_rotate(src, angle, destination, rot_cx, rot_cy);
    }

    return status;
}

