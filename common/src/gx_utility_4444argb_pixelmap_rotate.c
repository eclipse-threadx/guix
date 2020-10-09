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

#define ALPHAVAL(_c) (GX_UBYTE)(((_c) >> 12) & 0xf)
#define REDVAL(_c)   (GX_UBYTE)(((_c) >> 8) & 0xf)
#define GREENVAL(_c) (GX_UBYTE)(((_c) >> 4) & 0xf)
#define BLUEVAL(_c)  (GX_UBYTE)(((_c)) & 0xf)

#define ASSEMBLECOLOR(_a, _r, _g, _b)            \
    (USHORT)((((_a) & 0xf) << 12)              | \
             (((_r) & 0xf) << 8)               | \
             (((_g) & 0xf) << 4)               | \
             ((_b) & 0xf))

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
/*    _gx_utility_4444argb_pixelmap_rotate                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper funciton that rotate an 4444argb format uncompressed*/
/*    pixelmap.                                                           */
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
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*    _gx_utility_math_cos                  Calculate cosine of supplied  */
/*                                            angle                       */
/*    _gx_utility_math_sin                  Calculate sine of supplied    */
/*                                            angle                       */
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
UINT _gx_utility_4444argb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
USHORT *get;
USHORT *put;
INT     srcxres;
INT     srcyres;
INT     cosv;
INT     sinv;
USHORT  red, green, blue;
INT     idxminx, idxmaxx, idxmaxy;
INT    *mx;
INT    *my;
INT     xres;
INT     yres;
INT     width, height;
INT     x, y;
INT     xx, yy;
USHORT  a, b, c, d;
USHORT  alpha[4];
INT     xdiff, ydiff;

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
    destination -> gx_pixelmap_flags |= GX_PIXELMAP_ALPHA;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(USHORT);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (USHORT *)destination -> gx_pixelmap_data;

    /* Loop through the destination's pixels.  */
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

                if ((xx >= 0) && (xx < src -> gx_pixelmap_width - 1) &&
                    (yy >= 0) && (yy < src -> gx_pixelmap_height - 1))
                {
                    get = (USHORT *)src -> gx_pixelmap_data;
                    get += yy * src -> gx_pixelmap_width;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + src -> gx_pixelmap_width);
                    d = *(get + src -> gx_pixelmap_width + 1);
                    alpha[0] = ALPHAVAL(a);
                    alpha[1] = ALPHAVAL(b);
                    alpha[2] = ALPHAVAL(c);
                    alpha[3] = ALPHAVAL(d);
                }
                else
                {
                    get = (USHORT *)src -> gx_pixelmap_data;

                    a = 0;
                    b = 0;
                    c = 0;
                    d = 0;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * src -> gx_pixelmap_width);
                            alpha[1] = ALPHAVAL(b);
                        }

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * src -> gx_pixelmap_width);
                            alpha[3] = ALPHAVAL(d);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha[2] = ALPHAVAL(c);

                        if (xx < src -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                            alpha[3] = ALPHAVAL(d);
                        }
                    }
                    else if (xx == src -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        alpha[0] = ALPHAVAL(a);

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * src -> gx_pixelmap_width + xx);
                            alpha[2] = ALPHAVAL(c);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        alpha[0] = ALPHAVAL(a);

                        b = *(get + yy * src -> gx_pixelmap_width + xx + 1);
                        alpha[1] = ALPHAVAL(b);
                    }

                    if (!a)
                    {
                        alpha[0] = 0;
                    }

                    if (!b)
                    {
                        alpha[1] = 0;
                    }

                    if (!c)
                    {
                        alpha[2] = 0;
                    }

                    if (!d)
                    {
                        alpha[3] = 0;
                    }
                }

                red = (USHORT)((REDVAL(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                REDVAL(b) * alpha[1] * xdiff * (256 - ydiff) +         \
                                REDVAL(c) * alpha[2] * ydiff * (256 - xdiff) +         \
                                REDVAL(d) * alpha[3] * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                  GREENVAL(b) * alpha[1] * xdiff * (256 - ydiff) +         \
                                  GREENVAL(c) * alpha[2] * ydiff * (256 - xdiff) +         \
                                  GREENVAL(d) * alpha[3] * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                 BLUEVAL(b) * alpha[1] * xdiff * (256 - ydiff) +         \
                                 BLUEVAL(c) * alpha[2] * ydiff * (256 - xdiff) +         \
                                 BLUEVAL(d) * alpha[3] * xdiff * ydiff) >> 16);

                alpha[0] = (USHORT)((alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                     alpha[1] * xdiff * (256 - ydiff) +         \
                                     alpha[2] * ydiff * (256 - xdiff) +         \
                                     alpha[3] * xdiff * ydiff) >> 16);

                if (alpha[0])
                {
                    red /= alpha[0];
                    green /= alpha[0];
                    blue /= alpha[0];
                }
                red = red > 15 ? 15 : red;
                green = green > 15 ? 15 : green;
                blue = blue > 15 ? 15 : blue;
                alpha[0] = alpha[0] > 15 ? 15 : alpha[0];

                *put++ = ASSEMBLECOLOR(alpha[0], red, green, blue);
            }
            else
            {
                *put++ = 0;
            }
        }
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_4444argb_pixelmap_simple_rotate           PORTABLE C    */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    4444argb pixelmap rotation function that handles 90, 180 and 270    */
/*    degree case.                                                        */
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
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
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
UINT _gx_utility_4444argb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
USHORT *put;
USHORT *get;
INT     width;
INT     height;
INT     x;
INT     y;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(width * height) * sizeof(USHORT);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (angle == 90)
    {
        put = (USHORT *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (USHORT *)src -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                *put++ = *get;
            }
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            /* Get new rotation point. */
            *rot_cx = (width - 1 - y);
            *rot_cy = x;
        }
    }
    else if (angle == 180)
    {
        GX_SWAP_VALS(width, height);
        put = (USHORT *)destination -> gx_pixelmap_data;
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (USHORT *)src -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                *put++ = *get;
            }
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            /* Get new rotation point. */
            *rot_cx = (width - 1 - x);
            *rot_cy = (height - 1 - y);
        }
    }
    else
    {
        /* angle  = 270. */
        put = (USHORT *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (USHORT *)src -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                *put++ = *get;
            }
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            /* Get new rotation point. */
            *rot_cx = y;
            *rot_cy = (height - 1 - x);
        }
    }

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    return GX_SUCCESS;
}

