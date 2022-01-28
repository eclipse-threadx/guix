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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/
#define GX_SOURCE_CODE

#define REDVAL_332(_c)   (USHORT)(((_c) >> 5) & 0x07)
#define GREENVAL_332(_c) (USHORT)(((_c) >> 2) & 0x07)
#define BLUEVAL_332(_c)  (USHORT)((_c) & 0x03)

#define ASSEMBLECOLOR_332(_r, _g, _b) \
    ((((_r) & 0x07) << 5) |           \
     (((_g) & 0x07) << 2) |           \
     ((_b) & 0x03))

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
/*    _gx_display_driver_332rgb_pixelmap_simple_alpha_rotate              */
/*                                                        PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotates 332rgb format pixelmap with   */
/*    alpha in 90, 180 or 270 degree.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_332rgb_pixelmap_rotate                           */
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
static VOID _gx_display_driver_332rgb_pixelmap_simple_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                   INT angle, INT cx, INT cy)
{
GX_UBYTE     *get;
GX_UBYTE     *getalpha;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
INT           newxpos;
INT           newypos;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    clip = context -> gx_draw_context_clip;
    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (GX_NULL == blend_func)
    {
        return;
    }

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_height;
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                get += (width - 1 - x) * height;
                get += y;
                getalpha += (width - 1 - x) * height;
                getalpha += y;
                blend_func(context, x + newxpos, y + newypos, *get, *getalpha);
            }
        }
    }
    else if (angle == 180)
    {

        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cx);
        newypos = ypos + cy - (height - 1 - cy);


        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;
                getalpha += (height - 1 - y) * width;
                getalpha += width - 1 - x;

                blend_func(context, x + newxpos, y + newypos, *get, *getalpha);
            }
        }
    }
    else
    {
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                get += x * height;
                get += height - 1 - y;
                getalpha += x * height;
                getalpha += height - 1 - y;

                blend_func(context, x + newxpos, y + newypos, *get, *getalpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_332rgb_pixelmap_alpha_rotate     PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotates an 332rgb format pixelmap     */
/*    with alpha.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_rotate                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            corrected logic,            */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_332rgb_pixelmap_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                            INT angle, INT cx, INT cy)
{
GX_UBYTE     *get;
GX_UBYTE     *getalpha;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
USHORT        red;
USHORT        green;
USHORT        blue;
INT           idxminx;
INT           idxmaxx;
INT           idxmaxy;
INT          *mx;
INT          *my;
INT           xres;
INT           yres;
INT           x;
INT           y;
INT           xx;
INT           yy;
USHORT        a;
USHORT        b;
USHORT        c;
USHORT        d;
USHORT        alpha[4];
INT           xdiff;
INT           ydiff;
INT           newxpos;
INT           newypos;
GX_DISPLAY   *display;
GX_RECTANGLE *clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    clip = context -> gx_draw_context_clip;
    display = context -> gx_draw_context_display;
    blend_func = display -> gx_display_driver_pixel_blend;
    if (!blend_func)
    {
        return;
    }

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
    srcxres = pixelmap -> gx_pixelmap_width >> 1;
    srcyres = pixelmap -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT(mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv);
    yres = GX_FIXED_VAL_TO_INT(my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv);

    /* Calculate the new rotation axis. */
    xres = GX_FIXED_VAL_TO_INT((cx - srcxres) * cosv - (cy - srcyres) * sinv) + xres;
    yres = GX_FIXED_VAL_TO_INT((cy - srcyres) * cosv + (cx - srcxres) * sinv) + yres;

    newxpos = xpos + cx - xres;
    newypos = ypos + cy - yres;

    /* Loop through the source's pixels.  */
    for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
    {
        for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx) + cx;
            yy = GX_FIXED_VAL_TO_INT(yy) + cy;

            if ((xx >= -1) && (xx < pixelmap -> gx_pixelmap_width) &&
                (yy >= -1) && (yy < pixelmap -> gx_pixelmap_height))
            {
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_width - 1) && \
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_height - 1))
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_width;
                    get += xx;

                    getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                    getalpha += yy * pixelmap -> gx_pixelmap_width;
                    getalpha += xx;

                    a = *get;
                    alpha[0] = *getalpha;

                    b = *(get + 1);
                    alpha[1] = *(getalpha + 1);

                    c = *(get + pixelmap -> gx_pixelmap_width);
                    alpha[2] = *(getalpha + pixelmap -> gx_pixelmap_width);

                    d = *(get + pixelmap -> gx_pixelmap_width + 1);
                    alpha[3] = *(getalpha + pixelmap -> gx_pixelmap_width + 1);
                }
                else
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

                    a = 0;
                    b = 0;
                    c = 0;
                    d = 0;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * pixelmap -> gx_pixelmap_width);
                            alpha[1] = *(getalpha + yy * pixelmap -> gx_pixelmap_width);
                        }

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width);
                            alpha[3] = *(getalpha + (yy + 1) * pixelmap -> gx_pixelmap_width);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha[2] = *(getalpha + xx);

                        if (xx < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                            alpha[3] = *(getalpha + xx + 1);
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);
                        alpha[0] = *(getalpha + yy * pixelmap -> gx_pixelmap_width + xx);

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width + xx);
                            alpha[2] = *(getalpha + (yy + 1) * pixelmap -> gx_pixelmap_width + xx);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);
                        alpha[0] = *(getalpha + yy * pixelmap -> gx_pixelmap_width + xx);

                        b = *(get + yy * pixelmap -> gx_pixelmap_width + xx + 1);
                        alpha[1] = *(getalpha + yy * pixelmap -> gx_pixelmap_width + xx + 1);
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

                red = (USHORT)((REDVAL_332(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                REDVAL_332(b) * alpha[1] * xdiff * (256 - ydiff) +
                                REDVAL_332(c) * alpha[2] * ydiff * (256 - xdiff) +
                                REDVAL_332(d) * alpha[3] * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL_332(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                  GREENVAL_332(b) * alpha[1] * xdiff * (256 - ydiff) +
                                  GREENVAL_332(c) * alpha[2] * ydiff * (256 - xdiff) +
                                  GREENVAL_332(d) * alpha[3] * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL_332(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                 BLUEVAL_332(b) * alpha[1] * xdiff * (256 - ydiff) +
                                 BLUEVAL_332(c) * alpha[2] * ydiff * (256 - xdiff) +
                                 BLUEVAL_332(d) * alpha[3] * xdiff * ydiff) >> 16);

                alpha[0] = (USHORT)((alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                     alpha[1] * xdiff * (256 - ydiff) +
                                     alpha[2] * ydiff * (256 - xdiff) +
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

                blend_func(context, x + newxpos, y + newypos, (GX_COLOR)ASSEMBLECOLOR_332(red, green, blue), (GX_UBYTE)alpha[0]);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_332rgb_pixelmap_raw_rotate      PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotates an 332rgb format pixelmap     */
/*    without compression, without alpha.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_332rgb_pixelmap_rotate                           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            corrected logic,            */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_332rgb_pixelmap_raw_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                          INT angle, INT cx, INT cy)
{
GX_UBYTE     *get;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
USHORT        red;
USHORT        green;
USHORT        blue;
INT           idxminx;
INT           idxmaxx;
INT           idxmaxy;
INT          *mx;
INT          *my;
INT           xres;
INT           yres;
INT           x;
INT           y;
INT           xx;
INT           yy;
USHORT        a;
USHORT        b;
USHORT        c;
USHORT        d;
INT           alpha;
INT           xdiff;
INT           ydiff;
INT           newxpos;
INT           newypos;
GX_DISPLAY   *display;
GX_RECTANGLE *clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    clip = context -> gx_draw_context_clip;
    display = context -> gx_draw_context_display;
    blend_func = display -> gx_display_driver_pixel_blend;
    if (!blend_func)
    {
        return;
    }

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
    srcxres = pixelmap -> gx_pixelmap_width >> 1;
    srcyres = pixelmap -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT(mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv);
    yres = GX_FIXED_VAL_TO_INT(my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv);

    /* Calculate the new rotation axis. */
    xres = GX_FIXED_VAL_TO_INT((cx - srcxres) * cosv - (cy - srcyres) * sinv) + xres;
    yres = GX_FIXED_VAL_TO_INT((cy - srcyres) * cosv + (cx - srcxres) * sinv) + yres;

    newxpos = xpos + cx - xres;
    newypos = ypos + cy - yres;

    /* Loop through the source's pixels.  */
    for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
    {
        for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx) + cx;
            yy = GX_FIXED_VAL_TO_INT(yy) + cy;

            if ((xx >= -1) && (xx < pixelmap -> gx_pixelmap_width) &&
                (yy >= -1) && (yy < pixelmap -> gx_pixelmap_height))
            {
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_width - 1) && \
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_height - 1))
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_width;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + pixelmap -> gx_pixelmap_width);
                    d = *(get + pixelmap -> gx_pixelmap_width + 1);

                    alpha = 0xff;
                }
                else
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;

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
                            b = *(get + yy * pixelmap -> gx_pixelmap_width);
                            alpha += xdiff * (256 - ydiff);
                        }

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width);
                            alpha += xdiff * ydiff;
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha += ydiff * (256 - xdiff);

                        if (xx < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                            alpha += xdiff * ydiff;
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);
                        alpha += (256 - xdiff) * (256 - ydiff);

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width + xx);
                            alpha += ydiff * (256 - xdiff);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);
                        alpha += (256 - xdiff) * (256 - ydiff);

                        b = *(get + yy * pixelmap -> gx_pixelmap_width + xx + 1);
                        alpha += xdiff * (256 - ydiff);
                    }

                    alpha >>= 8;
                }

                red = (USHORT)((REDVAL_332(a) * (256 - xdiff) * (256 - ydiff) +
                                REDVAL_332(b) * xdiff * (256 - ydiff) +
                                REDVAL_332(c) * ydiff * (256 - xdiff) +
                                REDVAL_332(d) * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL_332(a) * (256 - xdiff) * (256 - ydiff) +
                                  GREENVAL_332(b) * xdiff * (256 - ydiff) +
                                  GREENVAL_332(c) * ydiff * (256 - xdiff) +
                                  GREENVAL_332(d) * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL_332(a) * (256 - xdiff) * (256 - ydiff) +
                                 BLUEVAL_332(b) * xdiff * (256 - ydiff) +
                                 BLUEVAL_332(c) * ydiff * (256 - xdiff) +
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

                blend_func(context, x + newxpos, y + newypos, (GX_COLOR)ASSEMBLECOLOR_332(red, green, blue), (GX_UBYTE)alpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_332rgb_pixelmap_rotate           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service rotate a 332rgb format pixelmap to canvas memory.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    rot_cx                                x-coord of rotating center.   */
/*    rot_cy                                y-coord of rotationg center.  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_rotate                             */
/*                                          Rotate 8bpp format pixelmap   */
/*    _gx_display_driver_332rgb_pixelmap_simple_alpha_rotate              */
/*                                          Rotate 332rgb format pixelmap */
/*                                            in simple case              */
/*    _gx_display_driver_332rgb_pixelmap_alpha_rotate                     */
/*                                          Rotate 332rgb format pixelmap */
/*                                            with alpha                  */
/*    _gx_display_driver_332rgb_pixelmap_raw_rotate                       */
/*                                          Rotate 332rgb format pixelmap */
/*                                            without alpha               */
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_332rgb_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                               INT angle, INT rot_cx, INT rot_cy)
{
    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        if (angle % 90 == 0)
        {
            /* Simple angle rotate: 90 degree, 180 degree and 270 degree.  */
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /*Alpha, no compression.*/
                _gx_display_driver_332rgb_pixelmap_simple_alpha_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            else
            {
                _gx_display_driver_8bpp_pixelmap_simple_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /*Not correct yet.*/
                _gx_display_driver_332rgb_pixelmap_alpha_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            else
            {
                /* no compression or alpha */
                _gx_display_driver_332rgb_pixelmap_raw_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
        }
        break;
    }

    return;
}

