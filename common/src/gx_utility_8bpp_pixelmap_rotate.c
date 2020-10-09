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
/*    _gx_utility_8bpp_pixelmap_rotate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function rotates an 8bpp format uncompressed pixelmap with or  */
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
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
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
UINT _gx_utility_8bpp_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
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

    /* Set transparent color.  */
    if (src -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        destination -> gx_pixelmap_transparent_color = src -> gx_pixelmap_transparent_color;
    }
    else
    {
        destination -> gx_pixelmap_transparent_color = 0xff;
    }

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

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(width * height) * sizeof(GX_UBYTE);

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

        for (x = 0; x < width; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xx = GX_FIXED_VAL_TO_INT(xx) + srcxres;
            yy = GX_FIXED_VAL_TO_INT(yy) + srcyres;

            if ((xx >= 0) && (xx < src -> gx_pixelmap_width) &&
                (yy >= 0) && (yy < src -> gx_pixelmap_height))
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += yy * src -> gx_pixelmap_width;
                get += xx;

                *put++ = *get;
            }
            else
            {
                *put++ = (GX_UBYTE)destination -> gx_pixelmap_transparent_color;
            }
        }
        putrow += destination -> gx_pixelmap_width;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_8bpp_pixelmap_simple_rotate             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function rotates an 8bpp format uncompressed pixelmap with     */
/*    special rotation angle 90, 180 or 270 degree.                       */
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
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Memory Allocation routine     */
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
UINT _gx_utility_8bpp_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *get;
INT       width, height;
INT       x, y;

    destination -> gx_pixelmap_transparent_color = src -> gx_pixelmap_transparent_color;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(width * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (angle == 90)
    {
        put = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                *put++ = *get;
            }
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
        GX_SWAP_VALS(width, height);

        put = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                *put++ = *get;
            }
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
        put = (GX_UBYTE *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                *put++ = *get;
            }
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
/*    _gx_utility_8bit_alphamap_rotate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function rotates an 8bit alphamap.                             */
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
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
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
UINT _gx_utility_8bit_alphamap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE          *put;
GX_CONST GX_UBYTE *get;
INT                srcxres;
INT                srcyres;
INT                cosv;
INT                sinv;
INT                alpha;
INT                idxminx;
INT                idxmaxx;
INT                idxmaxy;
INT               *mx;
INT               *my;
INT                xres;
INT                yres;
INT                width;
INT                height;
INT                x;
INT                y;
INT                xx;
INT                yy;
GX_UBYTE           a;
GX_UBYTE           b;
GX_UBYTE           c;
GX_UBYTE           d;
INT                xdiff;
INT                ydiff;

    mx = _gx_system_scratchpad;
    my = mx + 4;

    mx[0] = mx[3] = -1;
    mx[1] = mx[2] = 1;

    my[0] = my[1] = 1;
    my[2] = my[3] = -1;

    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = src -> gx_pixelmap_width >> 1;
    srcyres = src -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv;
    yres = my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

    xres = GX_FIXED_VAL_TO_INT(xres);
    yres = GX_FIXED_VAL_TO_INT(yres);

    /* Calculate destination width and height. */
    width = (xres << 1);
    height = (yres << 1);

    /* Calculate the new rotation axis. */
    if (rot_cx && rot_cy)
    {
        x = ((*rot_cx) - srcxres) * cosv - ((*rot_cy) - srcyres) * sinv;
        y = ((*rot_cy) - srcyres) * cosv + ((*rot_cx) - srcxres) * sinv;

        srcxres = *rot_cx;
        srcyres = *rot_cy;

        x = GX_FIXED_VAL_TO_INT(x) + xres;
        y = GX_FIXED_VAL_TO_INT(y) + yres;

        *rot_cx = x;
        *rot_cy = y;

        xres = *rot_cx;
        yres = *rot_cy;
    }

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;
    destination -> gx_pixelmap_flags |= GX_PIXELMAP_TRANSPARENT;
    destination -> gx_pixelmap_transparent_color = 0;
    destination -> gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_ALPHAMAP;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(height * width);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_UBYTE *)destination -> gx_pixelmap_data;

    /* Loop through the source's pixels.  */
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx) + srcxres;
            yy = GX_FIXED_VAL_TO_INT(yy) + srcyres;

            if ((xx >= -1) && (xx < src -> gx_pixelmap_width) &&
                (yy >= -1) && (yy < src -> gx_pixelmap_height))
            {
                if ((xx >= 0) && (xx < src -> gx_pixelmap_width - 1) && \
                    (yy >= 0) && (yy < src -> gx_pixelmap_height - 1))
                {
                    get = src -> gx_pixelmap_data;
                    get += yy * src -> gx_pixelmap_width;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + src -> gx_pixelmap_width);
                    d = *(get + src -> gx_pixelmap_width + 1);
                }
                else
                {
                    get = src -> gx_pixelmap_data;

                    a = 0;
                    b = a;
                    c = a;
                    d = a;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * src -> gx_pixelmap_width);
                        }

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * src -> gx_pixelmap_width);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);

                        if (xx < src -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                        }
                    }
                    else if (xx == src -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * src -> gx_pixelmap_width + xx);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        b = *(get + yy * src -> gx_pixelmap_width + xx + 1);
                    }
                }

                alpha = (INT)((a * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                               b * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                               c * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                               d * (ULONG)xdiff * (ULONG)ydiff) >> 16);

                *put++ = (GX_UBYTE)alpha;
            }
            else
            {
                *put++ = 0;
            }
        }
    }

    return GX_SUCCESS;
}

