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


#include "gx_api.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_utility.h"
#include "gx_system.h"

#define REDVAL_332(_c)   (USHORT)(((_c) >> 5) & 0x07)
#define GREENVAL_332(_c) (USHORT)(((_c) >> 2) & 0x07)
#define BLUEVAL_332(_c)  (USHORT)((_c) & 0x03)

#define ASSEMBLECOLOR_332(_r, _g, _b) \
    ((((_r) & 0x07) << 5)   |         \
     (((_g) & 0x07) << 2)   |         \
     ((_b) & 0x03))

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_raw_rotate              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*    without alpha.                                                      */
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
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_rotate                                  */
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
static UINT _gx_utility_332rgb_pixelmap_raw_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *get;
GX_UBYTE *putalpha;
INT       srcxres;
INT       srcyres;
INT       cosv;
INT       sinv;
USHORT    red, green, blue;
INT       idxminx, idxmaxx, idxmaxy;
INT      *mx;
INT      *my;
INT       xres;
INT       yres;
INT       width, height;
INT       x, y;
INT       xx, yy;
USHORT    a, b, c, d;
INT       alpha;
INT       xdiff, ydiff;

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
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_aux_data_size = (UINT)(height * width) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_aux_data_size);

    if (destination -> gx_pixelmap_aux_data == GX_NULL)
    {
        _gx_system_memory_free((VOID *)destination -> gx_pixelmap_aux_data);

        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_UBYTE *)destination -> gx_pixelmap_data;
    putalpha = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

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
                    get = (GX_UBYTE *)src -> gx_pixelmap_data;
                    get += yy * src -> gx_pixelmap_width;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + src -> gx_pixelmap_width);
                    d = *(get + src -> gx_pixelmap_width + 1);

                    alpha = 0xff;
                }
                else
                {
                    get = (GX_UBYTE *)src -> gx_pixelmap_data;

                    a = 0;
                    b = 0;
                    c = 0;
                    d = 0;
                    alpha = 0;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * src -> gx_pixelmap_width);
                            alpha += xdiff * (256 - ydiff);
                        }

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * src -> gx_pixelmap_width);
                            alpha += xdiff * ydiff;
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha += ydiff * (256 - xdiff);

                        if (xx < src -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                            alpha += xdiff * ydiff;
                        }
                    }
                    else if (xx == src -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        alpha += (256 - xdiff) * (256 - ydiff);

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * src -> gx_pixelmap_width + xx);
                            alpha += ydiff * (256 - xdiff);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        alpha += (256 - xdiff) * (256 - ydiff);

                        b = *(get + yy * src -> gx_pixelmap_width + xx + 1);
                        alpha += xdiff * (256 - ydiff);
                    }

                    alpha >>= 8;
                }

                red = (USHORT)((REDVAL_332(a) * (256 - xdiff) * (256 - ydiff) + \
                                REDVAL_332(b) * xdiff * (256 - ydiff) +         \
                                REDVAL_332(c) * ydiff * (256 - xdiff) +         \
                                REDVAL_332(d) * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL_332(a) * (256 - xdiff) * (256 - ydiff) + \
                                  GREENVAL_332(b) * xdiff * (256 - ydiff) +         \
                                  GREENVAL_332(c) * ydiff * (256 - xdiff) +         \
                                  GREENVAL_332(d) * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL_332(a) * (256 - xdiff) * (256 - ydiff) + \
                                 BLUEVAL_332(b) * xdiff * (256 - ydiff) +         \
                                 BLUEVAL_332(c) * ydiff * (256 - xdiff) +         \
                                 BLUEVAL_332(d) * xdiff * ydiff) >> 16);

                if (alpha && (alpha < 0xff))
                {
                    red = (USHORT)((red << 8) / alpha);
                    green = (USHORT)((green << 8) / alpha);
                    blue = (USHORT)((blue << 8) / alpha);
                }

                red = red > 7 ? 7 : red;
                green = green > 7 ? 7 : green;
                blue = blue > 3 ? 3 : blue;
                alpha = alpha > 255 ? 255 : alpha;

                *put++ = (GX_UBYTE)ASSEMBLECOLOR_332(red, green, blue);
                *putalpha++ = (GX_UBYTE)alpha;
            }
            else
            {
                put++;
                *putalpha++ = 0;
            }
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_alpha_rotate            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*    with alpha.                                                         */
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
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_rotate                                  */
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
static UINT _gx_utility_332rgb_pixelmap_alpha_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *get;
GX_UBYTE *putalpha;
GX_UBYTE *getalpha;
INT       srcxres;
INT       srcyres;
INT       cosv;
INT       sinv;
USHORT    red, green, blue;
INT       idxminx, idxmaxx, idxmaxy;
INT      *mx;
INT      *my;
INT       xres;
INT       yres;
INT       width, height;
INT       x, y;
INT       xx, yy;
USHORT    a, b, c, d;
USHORT    alpha[4];
INT       xdiff, ydiff;

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
    yres = my[idxmaxy] * (srcyres + 2)  * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

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
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_aux_data_size = (UINT)(height * width) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_aux_data_size);

    if (destination -> gx_pixelmap_aux_data == GX_NULL)
    {
        _gx_system_memory_free((VOID *)destination -> gx_pixelmap_data);

        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_UBYTE *)destination -> gx_pixelmap_data;
    putalpha = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

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
                    get = (GX_UBYTE *)src -> gx_pixelmap_data;
                    get += yy * src -> gx_pixelmap_width;
                    get += xx;

                    getalpha = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                    getalpha += yy * src -> gx_pixelmap_width;
                    getalpha += xx;

                    a = *get;
                    alpha[0] = *getalpha;

                    b = *(get + 1);
                    alpha[1] = *(getalpha + 1);

                    c = *(get + src -> gx_pixelmap_width);
                    alpha[2] = *(getalpha + src -> gx_pixelmap_width);

                    d = *(get + src -> gx_pixelmap_width + 1);
                    alpha[3] = *(getalpha + src -> gx_pixelmap_width + 1);
                }
                else
                {
                    get = (GX_UBYTE *)src -> gx_pixelmap_data;
                    getalpha = (GX_UBYTE *)src -> gx_pixelmap_aux_data;

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
                            alpha[1] = *(getalpha + yy * src -> gx_pixelmap_width);
                        }

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * src -> gx_pixelmap_width);
                            alpha[3] = *(getalpha + (yy + 1) * src -> gx_pixelmap_width);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha[2] = *(getalpha + xx);

                        if (xx < src -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                            alpha[3] = *(getalpha + xx + 1);
                        }
                    }
                    else if (xx == src -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        alpha[0] = *(getalpha + yy * src -> gx_pixelmap_width + xx);

                        if (yy < src -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * src -> gx_pixelmap_width + xx);
                            alpha[2] = *(getalpha + (yy + 1) * src -> gx_pixelmap_width + xx);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * src -> gx_pixelmap_width + xx);
                        alpha[0] = *(getalpha + yy * src -> gx_pixelmap_width + xx);

                        b = *(get + yy * src -> gx_pixelmap_width + xx + 1);
                        alpha[1] = *(getalpha + yy * src -> gx_pixelmap_width + xx + 1);
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

                red = (USHORT)((REDVAL_332(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                REDVAL_332(b) * alpha[1] * xdiff * (256 - ydiff) +         \
                                REDVAL_332(c) * alpha[2] * ydiff * (256 - xdiff) +         \
                                REDVAL_332(d) * alpha[3] * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL_332(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                  GREENVAL_332(b) * alpha[1] * xdiff * (256 - ydiff) +         \
                                  GREENVAL_332(c) * alpha[2] * ydiff * (256 - xdiff) +         \
                                  GREENVAL_332(d) * alpha[3] * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL_332(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) + \
                                 BLUEVAL_332(b) * alpha[1] * xdiff * (256 - ydiff) +         \
                                 BLUEVAL_332(c) * alpha[2] * ydiff * (256 - xdiff) +         \
                                 BLUEVAL_332(d) * alpha[3] * xdiff * ydiff) >> 16);

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

                red = red > 7 ? 7 : red;
                green = green > 7 ? 7 : green;
                blue = blue > 3 ? 3 : blue;
                alpha[0] = alpha[0] > 255 ? 255 : alpha[0];

                *put++ = (GX_UBYTE)ASSEMBLECOLOR_332(red, green, blue);
                *putalpha++ = (GX_UBYTE)alpha[0];
            }
            else
            {
                put++;
                *putalpha++ = 0;
            }
        }
    }

    return GX_SUCCESS;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_simple_raw_rotate       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles 90, 180 and 270 degree        */
/*    rotation of an uncompressed pixelmap wihout alpha.                  */
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
/*    _gx_system_memory_allocator           Memory Allocation routine     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_simple_rotate                           */
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
static UINT _gx_utility_332rgb_pixelmap_simple_raw_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *get;
INT       width;
INT       height;
INT       x, y;

    destination -> gx_pixelmap_aux_data = GX_NULL;

    width = src -> gx_pixelmap_width;
    height = src -> gx_pixelmap_height;

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
        GX_SWAP_VALS(width, height);

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
        GX_SWAP_VALS(width, height);

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
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    return GX_SUCCESS;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_simple_alpha_rotate     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles 90, 180 and 270 degree        */
/*    rotation of an uncompressed pixelmap wih alpha.                     */
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
/*    _gx_utility_332rgb_pixelmap_simple_rotate                           */
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
static UINT _gx_utility_332rgb_pixelmap_simple_alpha_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_UBYTE *put;
GX_UBYTE *putalpha;
GX_UBYTE *get;
GX_UBYTE *getalpha;
INT       width;
INT       height;
INT       x, y;

    width = src -> gx_pixelmap_width;
    height = src -> gx_pixelmap_height;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(width * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_aux_data_size = (UINT)(width * height) * sizeof(GX_UBYTE);
    destination -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_aux_data_size);

    if (destination -> gx_pixelmap_aux_data == GX_NULL)
    {
        _gx_system_memory_free((VOID *)destination -> gx_pixelmap_data);
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_UBYTE *)destination -> gx_pixelmap_data;
    putalpha = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

    if (angle == 90)
    {
        GX_SWAP_VALS(width, height);

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                getalpha = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getalpha += (width - 1 - x) * height;
                getalpha += y;

                *put++ = *get;
                *putalpha++ = *getalpha;
            }
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            /* Get new rotation point. */
            *rot_cx = width - 1 - y;
            *rot_cy = x;
        }
    }
    else if (angle == 180)
    {
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                getalpha = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getalpha += (height - 1 - y) * width;
                getalpha += width - 1 - x;

                *put++ = *get;
                *putalpha++ = *getalpha;
            }
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            /* Get new rotation point. */
            *rot_cx = width - 1 - x;
            *rot_cy = height - 1 - y;
        }
    }
    else
    {
        /* angle = 270. */
        GX_SWAP_VALS(width, height);

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_UBYTE *)src -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                getalpha = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
                getalpha += x * height;
                getalpha += height - 1 - y;

                *put++ = *get;
                *putalpha++ = *getalpha;
            }
        }

        if (rot_cx && rot_cy)
        {
            x = *rot_cx;
            y = *rot_cy;

            /* Get new rotation point. */
            *rot_cx = y;
            *rot_cy = height - 1 - x;
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
/*    _gx_utility_332rgb_pixelmap_simple_rotate           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    332rgb pixelmap rotation function that handles 90, 180 and 270      */
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
/*     _gx_utility_332rgb_pixelmap_simple_raw_rotate                      */
/*                                          Rotate an 332rgb format       */
/*                                            pixelmap without alpha      */
/*     _gx_utility_332rgb_pixelmap_simple_alpha_rotate                    */
/*                                          Rotate an 332rgb format       */
/*                                            pixelmap with alpha         */
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
UINT _gx_utility_332rgb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        /* alpha, no compression */
        status = _gx_utility_332rgb_pixelmap_simple_alpha_rotate(src, angle, destination, rot_cx, rot_cy);
    }
    else
    {

        /* no compression or alpha */
        status = _gx_utility_332rgb_pixelmap_simple_raw_rotate(src, angle, destination, rot_cx, rot_cy);
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_332rgb_pixelmap_rotate                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    332rgb pixelmap rotation function that handles uncompress, with or  */
/*    alpha channel.                                                      */
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
/*     _gx_utility_332rgb_pixelmap_raw_rotate                             */
/*                                          Rotate an 332rgb format       */
/*                                            pixelmap without alpha      */
/*     _gx_utility_332rgb_pixelmap_alpha_rotate                           */
/*                                          Rotate an 332rgb format       */
/*                                            pixelmap with alpha         */
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
UINT _gx_utility_332rgb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
UINT status = 0;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        /* alpha, no compression */
        status = _gx_utility_332rgb_pixelmap_alpha_rotate(src, angle, destination, rot_cx, rot_cy);
    }
    else
    {
        /* no compression or alpha */
        status = _gx_utility_332rgb_pixelmap_raw_rotate(src, angle, destination, rot_cx, rot_cy);
    }

    return status;
}

