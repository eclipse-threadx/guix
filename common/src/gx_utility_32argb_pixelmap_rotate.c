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

#define REDVAL(_c)     (GX_UBYTE)((_c) >> 16)
#define GREENVAL(_c)   (GX_UBYTE)((_c) >> 8)
#define BLUEVAL(_c)    (GX_UBYTE)(_c)

#define ASSEMBLECOLOR(_r, _g, _b) \
    (((_r) << 16) |               \
     ((_g) << 8) |                \
     (_b))

#define BYTE_RANGE(_c) _c > 255 ? 255 : _c

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
/*    _gx_utility_32argb_pixelmap_raw_rotate              PORTABLE C      */
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
static UINT _gx_utility_32argb_pixelmap_raw_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_COLOR    *put;
GX_COLOR    *get;
INT          srcxres;
INT          srcyres;
INT          cosv;
INT          sinv;
INT          alpha;
GX_COLOR     red, green, blue;
INT          idxminx, idxmaxx, idxmaxy;
INT         *mx;
INT         *my;
GX_FIXED_VAL xres;
GX_FIXED_VAL yres;
INT          width, height;
INT          x, y;
INT          xx, yy;
GX_COLOR     a, b, c, d;
INT          xdiff, ydiff;

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
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(GX_COLOR);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_COLOR *)destination -> gx_pixelmap_data;

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
                    get = (GX_COLOR *)src -> gx_pixelmap_data;
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
                    get = (GX_COLOR *)src -> gx_pixelmap_data;

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

                    if (a)
                    {
                        alpha += (256 - xdiff) * (256 - ydiff);
                    }

                    if (b)
                    {
                        alpha += xdiff * (256 - ydiff);
                    }

                    if (c)
                    {
                        alpha += ydiff * (256 - xdiff);
                    }

                    if (d)
                    {
                        alpha += xdiff * ydiff;
                    }

                    alpha >>= 8;
                }

                red = (GX_COLOR)((REDVAL(a) * (256 - xdiff) * (256 - ydiff) + \
                                  REDVAL(b) * xdiff * (256 - ydiff) +         \
                                  REDVAL(c) * ydiff * (256 - xdiff) +         \
                                  REDVAL(d) * xdiff * ydiff) >> 16);

                green = (GX_COLOR)((GREENVAL(a) * (256 - xdiff) * (256 - ydiff) + \
                                    GREENVAL(b) * xdiff * (256 - ydiff) +         \
                                    GREENVAL(c) * ydiff * (256 - xdiff) +         \
                                    GREENVAL(d) * xdiff * ydiff) >> 16);

                blue = (GX_COLOR)((BLUEVAL(a) * (256 - xdiff) * (256 - ydiff) + \
                                   BLUEVAL(b) * xdiff * (256 - ydiff) +         \
                                   BLUEVAL(c) * ydiff * (256 - xdiff) +         \
                                   BLUEVAL(d) * xdiff * ydiff) >> 16);

                if ((alpha > 0) && (alpha < 0xff))
                {
                    red = (red << 8) / (ULONG)alpha;
                    green = (green << 8) / (ULONG)alpha;
                    blue = (blue << 8) / (ULONG)alpha;
                }

                red = BYTE_RANGE(red);
                green = BYTE_RANGE(green);
                blue = BYTE_RANGE(blue);
                alpha = BYTE_RANGE(alpha);

                *put++ = ASSEMBLECOLOR(red, green, blue) + (ULONG)(alpha << 24);
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
/*    _gx_utility_32argb_pixelmap_alpha_rotate            PORTABLE C      */
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
static UINT _gx_utility_32argb_pixelmap_alpha_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_COLOR *put;
GX_COLOR *get;
INT       srcxres;
INT       srcyres;
INT       cosv;
INT       sinv;
INT       alpha;
GX_COLOR  red, green, blue;
INT       idxminx, idxmaxx, idxmaxy;
INT      *mx;
INT      *my;
INT       xres;
INT       yres;
INT       width, height;
INT       x, y;
INT       xx, yy;
GX_COLOR  a, b, c, d;
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
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(GX_COLOR);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_COLOR *)destination -> gx_pixelmap_data;

    /* Loop through the source's pixels.  */
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx);
            yy = GX_FIXED_VAL_TO_INT(yy);

            xx += srcxres;
            yy += srcyres;

            if ((xx >= -1) && (xx < src -> gx_pixelmap_width) &&
                (yy >= -1) && (yy < src -> gx_pixelmap_height))
            {
                if ((xx >= 0) && (xx < src -> gx_pixelmap_width - 1) && \
                    (yy >= 0) && (yy < src -> gx_pixelmap_height - 1))
                {
                    get = (GX_COLOR *)src -> gx_pixelmap_data;
                    get += yy * src -> gx_pixelmap_width;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + src -> gx_pixelmap_width);
                    d = *(get + src -> gx_pixelmap_width + 1);
                }
                else
                {
                    get = (GX_COLOR *)src -> gx_pixelmap_data;

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

                red = (ULONG)((REDVAL(a) * (a >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                               REDVAL(b) * (b >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                               REDVAL(c) * (c >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                               REDVAL(d) * (d >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

                green = (ULONG)((GREENVAL(a) * (a >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                                 GREENVAL(b) * (b >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                                 GREENVAL(c) * (c >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                                 GREENVAL(d) * (d >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

                blue = (ULONG)((BLUEVAL(a) * (a >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                                BLUEVAL(b) * (b >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                                BLUEVAL(c) * (c >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                                BLUEVAL(d) * (d >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

                alpha = (INT)(((a >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                               (b >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                               (c >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                               (d >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

                if (alpha)
                {
                    red /= (ULONG)alpha;
                    green /= (ULONG)alpha;
                    blue /= (ULONG)alpha;
                }

                red = BYTE_RANGE(red);
                green = BYTE_RANGE(green);
                blue = BYTE_RANGE(blue);
                alpha = BYTE_RANGE(alpha);

                *put++ = ASSEMBLECOLOR(red, green, blue) + ((ULONG)alpha << 24);
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
/*    _gx_utility_32argb_pixelmap_rotate                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32argb pixelmap rotation function that handles uncompress, with or  */
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
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_utility_32argb_pixelmap_alpha_rotate                           */
/*                                          Rotate an 32bpp pixelmap with */
/*                                            alpha channel               */
/*     _gx_utility_32argb_pixelmap_raw_rotate                             */
/*                                          Rotate an 32bpp pixelmap      */
/*                                            without channel             */
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
UINT _gx_utility_32argb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {

        /* alpha, no compression */
        status = _gx_utility_32argb_pixelmap_alpha_rotate(src, angle, destination, rot_cx, rot_cy);
    }
    else
    {
        /* no compression or alpha */
        status = _gx_utility_32argb_pixelmap_raw_rotate(src, angle, destination, rot_cx, rot_cy);
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_32argb_pixelmap_simple_rotate           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32argb pixelmap simple rotation function that hangles 90, 180 and   */
/*    270 degreen case of uncompress, with or without alpha channel.      */
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
UINT _gx_utility_32argb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy)
{
GX_COLOR *put;
GX_COLOR *get;
INT       width, height;
INT       x, y;

    width = src -> gx_pixelmap_height;
    height = src -> gx_pixelmap_width;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(width * height) * sizeof(GX_COLOR);
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (angle == 90)
    {
        put = (GX_COLOR *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_COLOR *)src -> gx_pixelmap_data;
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

        put = (GX_COLOR *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_COLOR *)src -> gx_pixelmap_data;
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
        /* angle = 270. */
        put = (GX_COLOR *)destination -> gx_pixelmap_data;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                get = (GX_COLOR *)src -> gx_pixelmap_data;
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

