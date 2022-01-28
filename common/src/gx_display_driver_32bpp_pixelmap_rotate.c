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

#define REDVAL(_c)     (GX_UBYTE)((_c) >> 16)
#define GREENVAL(_c)   (GX_UBYTE)((_c) >> 8)
#define BLUEVAL(_c)    (GX_UBYTE)(_c)

#define ASSEMBLECOLOR(_r, _g, _b) \
    (0xff000000   |               \
     ((_r) << 16) |               \
     ((_g) << 8)  |               \
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
/*    _gx_display_driver_24xrgb_pixelmap_rotate           PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*      without alpha.                                                    */
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
/*    _gx_display_driver_32bpp_pixelmap_rotate                            */
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
static VOID _gx_display_driver_24xrgb_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                      INT angle, INT cx, INT cy)
{
GX_COLOR     *get;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
INT           alpha;
GX_COLOR      red;
GX_COLOR      green;
GX_COLOR      blue;
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
GX_COLOR      a;
GX_COLOR      b;
GX_COLOR      c;
GX_COLOR      d;
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

    xres = mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv;
    yres = my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

    xres = GX_FIXED_VAL_TO_INT(xres);
    yres = GX_FIXED_VAL_TO_INT(yres);

    /* Calculate the new rotation axis. */

    x = (cx - srcxres) * cosv - (cy - srcyres) * sinv;
    y = (cy - srcyres) * cosv + (cx - srcxres) * sinv;

    xres = GX_FIXED_VAL_TO_INT(x) + xres;
    yres = GX_FIXED_VAL_TO_INT(y) + yres;

    newxpos = xpos + cx - xres;
    newypos = ypos + cy - yres;

    /* Loop through the destination's pixels.  */
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
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_width - 1) &&
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_height - 1))
                {
                    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
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
                    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;

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
                        }

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);

                        if (xx < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width + xx);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);
                        b = *(get + yy * pixelmap -> gx_pixelmap_width + xx + 1);
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

                red = (GX_COLOR)(REDVAL(a) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                                 REDVAL(b) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                                 REDVAL(c) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                                 REDVAL(d) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                green = (GX_COLOR)(GREENVAL(a) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                                   GREENVAL(b) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                                   GREENVAL(c) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                                   GREENVAL(d) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                blue = (GX_COLOR)(BLUEVAL(a) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                                  BLUEVAL(b) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                                  BLUEVAL(c) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                                  BLUEVAL(d) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                if ((alpha > 0) && (alpha < 0xff))
                {
                    red = (red << 8) / (GX_COLOR)alpha;
                    green = (green << 8) / (GX_COLOR)alpha;
                    blue = (blue << 8) / (GX_COLOR)alpha;
                }

                red = BYTE_RANGE(red);
                green = BYTE_RANGE(green);
                blue = BYTE_RANGE(blue);
                alpha = BYTE_RANGE(alpha);

                blend_func(context, x + newxpos, y + newypos, ASSEMBLECOLOR(red, green, blue), (GX_UBYTE)alpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_24xrgb_pixelmap_alpha_rotate            PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*      with alpha.                                                       */
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
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_pixelmap_rotate                            */
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
static VOID _gx_display_driver_24xrgb_pixelmap_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                            INT angle, INT cx, INT cy)
{
GX_COLOR     *get;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
INT           alpha;
GX_COLOR      red;
GX_COLOR      green;
GX_COLOR      blue;
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
GX_COLOR      a;
GX_COLOR      b;
GX_COLOR      c;
GX_COLOR      d;
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

    /* Calculate the new rotation axis. */
    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv;
    yres = my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

    xres = GX_FIXED_VAL_TO_INT(xres);
    yres = GX_FIXED_VAL_TO_INT(yres);

    x = (cx - srcxres) * cosv - (cy - srcyres) * sinv;
    y = (cy - srcyres) * cosv + (cx - srcxres) * sinv;

    xres = GX_FIXED_VAL_TO_INT(x) + xres;
    yres = GX_FIXED_VAL_TO_INT(y) + yres;

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
                    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_width;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + pixelmap -> gx_pixelmap_width);
                    d = *(get + pixelmap -> gx_pixelmap_width + 1);
                }
                else
                {
                    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;

                    a = 0;
                    b = a;
                    c = a;
                    d = a;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * pixelmap -> gx_pixelmap_width);
                        }

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);

                        if (xx < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + xx + 1);
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_width - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);

                        if (yy < pixelmap -> gx_pixelmap_height - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_width + xx);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_width + xx);
                        b = *(get + yy * pixelmap -> gx_pixelmap_width + xx + 1);
                    }
                }

                red = (REDVAL(a) * (a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                       REDVAL(b) * (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                       REDVAL(c) * (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                       REDVAL(d) * (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                green = (GREENVAL(a) * (a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                         GREENVAL(b) * (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                         GREENVAL(c) * (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                         GREENVAL(d) * (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                blue = (BLUEVAL(a) * (a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                        BLUEVAL(b) * (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                        BLUEVAL(c) * (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                        BLUEVAL(d) * (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                alpha = (INT)(((a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                               (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                               (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                               (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16);

                if (alpha)
                {
                    red /= (UINT)alpha;
                    green /= (UINT)alpha;
                    blue /= (UINT)alpha;
                }

                red = BYTE_RANGE(red);
                green = BYTE_RANGE(green);
                blue = BYTE_RANGE(blue);
                alpha = BYTE_RANGE(alpha);

                blend_func(context, x + newxpos, y + newypos, (GX_COLOR)ASSEMBLECOLOR(red, green, blue), (GX_UBYTE)alpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_simple_rotate    PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal help function that hangles 90, 180 and 270 degree pixelmap */
/*    rotation.                                                           */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused assignments, */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_24xrgb_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                             INT angle, INT cx, INT cy)
{
GX_COLOR     *putrow;
GX_COLOR     *put;
GX_COLOR     *get;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
INT           newxpos;
INT           newypos;

GX_DISPLAY   *display;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    display = context -> gx_draw_context_display;
    blend_func = display -> gx_display_driver_pixel_blend;

    clip = context -> gx_draw_context_clip;

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_height;
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        putrow = context -> gx_draw_context_memory;
        putrow += clip -> gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += clip -> gx_rectangle_left;

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
                {
                    blend_func(context, clip -> gx_rectangle_left + x, clip -> gx_rectangle_top + y, *get, (GX_UBYTE)((*get) >> 24));
                }
                else
                {
                    *put++ = *get;
                }
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
    else if (angle == 180)
    {

        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cx);
        newypos = ypos + cy - (height - 1 - cy);

        putrow = context -> gx_draw_context_memory;
        putrow += clip -> gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += clip -> gx_rectangle_left;

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
                {
                    blend_func(context, clip -> gx_rectangle_left + x, clip -> gx_rectangle_top + y, *get, (GX_UBYTE)((*get) >> 24));
                }
                else
                {
                    *put++ = *get;
                }
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
    else
    {
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        putrow = context -> gx_draw_context_memory;
        putrow += clip -> gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += clip -> gx_rectangle_left;

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
                {
                    blend_func(context, clip -> gx_rectangle_left + x, clip -> gx_rectangle_top + y, *get, (GX_UBYTE)((*get) >> 24));
                }
                else
                {
                    *put++ = *get;
                }
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_pixelmap_rotate            PORTABLE C      */
/*                                                           6.1.9        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service rotate a pixelmap directly to canvas memory.           */
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
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_simple_rotate                    */
/*                                          Real display driver pixelmap  */
/*                                            rotate function             */
/*    _gx_display_driver_24xrgb_pixelmap_alpha_rotate                     */
/*                                          Real display driver pixelmap  */
/*                                            rotate function             */
/*    _gx_display_driver_24xrgb_pixelmap_rotate                           */
/*                                          Real display driver pixelmap  */
/*                                            rotate function             */
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
/*  10-15-2021     Ting Zhu                 Modified comment(s),          */
/*                                            corrected format type,      */
/*                                            resulting in version 6.1.9  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_32bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                              INT angle, INT rot_cx, INT rot_cy)
{
    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
        if (angle % 90 == 0)
        {
            /* Simple angle rotate: 90 degree, 180 degree and 270 degree.  */
            _gx_display_driver_24xrgb_pixelmap_simple_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /* with alpha. */
                _gx_display_driver_24xrgb_pixelmap_alpha_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            else
            {
                /* without alpha */
                _gx_display_driver_24xrgb_pixelmap_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
        }
        break;
    }

    return;
}

