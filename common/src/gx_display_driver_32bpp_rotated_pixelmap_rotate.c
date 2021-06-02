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
/*    _gx_display_driver_32bpp_rotated_pixelmap_raw_rotate                */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an rotated uncompressed        */
/*    pixelmap without alpha.                                             */
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
/*    _gx_display_driver_32bpp_rotated_pixelmap_rotate                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_pixelmap_raw_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
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
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

    clip = context -> gx_draw_context_clip;

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
    srcxres = pixelmap -> gx_pixelmap_height >> 1;
    srcyres = pixelmap -> gx_pixelmap_width >> 1;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        srcyres = pixelmap -> gx_pixelmap_width - 1 - srcyres;

        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        srcxres = pixelmap -> gx_pixelmap_height - 1 - srcxres;

        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv;
    yres = my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

    xres = GX_FIXED_VAL_TO_INT(xres);
    yres = GX_FIXED_VAL_TO_INT(yres);

    /* Calculate the new rotation axis.  */

    x = (cx - srcxres) * cosv - (cy - srcyres) * sinv;
    y = (cy - srcyres) * cosv + (cx - srcxres) * sinv;

    x = GX_FIXED_VAL_TO_INT(x) + xres;
    y = GX_FIXED_VAL_TO_INT(y) + yres;

    newxpos = xpos + cx - x;
    newypos = ypos + cy - y;

    /* Loop through the destination's pixels.  */
    for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
    {
        for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx) + srcxres;
            yy = GX_FIXED_VAL_TO_INT(yy) + srcyres;

            if ((xx >= -1) && (xx < pixelmap -> gx_pixelmap_height) &&
                (yy >= -1) && (yy < pixelmap -> gx_pixelmap_width))
            {
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_height - 1) &&
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_width - 1))
                {
                    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_height;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + pixelmap -> gx_pixelmap_height);
                    d = *(get + pixelmap -> gx_pixelmap_height + 1);

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
                        /* Handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * pixelmap -> gx_pixelmap_height);
                        }

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* Handle top edge.  */
                        c = *(get + xx);

                        if (xx < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + xx + 1);
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_height - 1)
                    {
                        /* Handle right edget.  */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height + xx);
                        }
                    }
                    else
                    {
                        /* Handle bottom edge.  */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);
                        b = *(get + yy * pixelmap -> gx_pixelmap_height + xx + 1);
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

                red = (GX_COLOR)(REDVAL_24BPP(a) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                                 REDVAL_24BPP(b) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                                 REDVAL_24BPP(c) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                                 REDVAL_24BPP(d) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                green = (GX_COLOR)(GREENVAL_24BPP(a) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                                   GREENVAL_24BPP(b) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                                   GREENVAL_24BPP(c) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                                   GREENVAL_24BPP(d) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                blue = (GX_COLOR)(BLUEVAL_24BPP(a) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                                  BLUEVAL_24BPP(b) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                                  BLUEVAL_24BPP(c) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                                  BLUEVAL_24BPP(d) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

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

                blend_func(context, x + newxpos, y + newypos, ASSEMBLECOLOR_32ARGB((ULONG)0xff, red, green, blue), (GX_UBYTE)alpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_32bppted_pixelmap_alpha_rotate          PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an rotated uncompressed        */
/*    pixelmap with alpha.                                                */
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
/*    _gx_display_driver_32bpp_rotated_pixelmap_rotate                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_pixelmap_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
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
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

    clip = context -> gx_draw_context_clip;

    mx = _gx_system_scratchpad;
    my = mx + 4;

    mx[0] = mx[3] = -1;
    mx[1] = mx[2] = 1;

    my[0] = my[1] = 1;
    my[2] = my[3] = -1;

    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center.  */
    srcxres = pixelmap -> gx_pixelmap_height >> 1;
    srcyres = pixelmap -> gx_pixelmap_width >> 1;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        srcyres = pixelmap -> gx_pixelmap_width - 1 - srcyres;

        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        srcxres = pixelmap -> gx_pixelmap_height - 1 - srcxres;

        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv;
    yres = my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

    xres = GX_FIXED_VAL_TO_INT(xres);
    yres = GX_FIXED_VAL_TO_INT(yres);

    x = GX_FIXED_VAL_TO_INT(xres) + xres;
    y = GX_FIXED_VAL_TO_INT(yres) + yres;

    newxpos = xpos + cx - x;
    newypos = ypos + cy - y;

    /* Loop through the source's pixels.  */
    for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
    {
        for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx);
            yy = GX_FIXED_VAL_TO_INT(yy);

            xx += srcxres;
            yy += srcyres;

            if ((xx >= -1) && (xx < pixelmap -> gx_pixelmap_height) &&
                (yy >= -1) && (yy < pixelmap -> gx_pixelmap_width))
            {
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_height - 1) && \
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_width - 1))
                {
                    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_height;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + pixelmap -> gx_pixelmap_height);
                    d = *(get + pixelmap -> gx_pixelmap_height + 1);
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
                        /* Handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * pixelmap -> gx_pixelmap_height);
                        }

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* Handle top edge.  */
                        c = *(get + xx);

                        if (xx < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + xx + 1);
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_height - 1)
                    {
                        /* Handle right edget.  */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height + xx);
                        }
                    }
                    else
                    {
                        /* Handle bottom edge.  */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);
                        b = *(get + yy * pixelmap -> gx_pixelmap_height + xx + 1);
                    }
                }

                red = (REDVAL_24BPP(a) * (a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                       REDVAL_24BPP(b) * (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                       REDVAL_24BPP(c) * (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                       REDVAL_24BPP(d) * (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                green = (GREENVAL_24BPP(a) * (a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                         GREENVAL_24BPP(b) * (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                         GREENVAL_24BPP(c) * (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                         GREENVAL_24BPP(d) * (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

                blue = (BLUEVAL_24BPP(a) * (a >> 24) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                        BLUEVAL_24BPP(b) * (b >> 24) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                        BLUEVAL_24BPP(c) * (c >> 24) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                        BLUEVAL_24BPP(d) * (d >> 24) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

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

                blend_func(context, x + newxpos, y + newypos, (GX_COLOR)ASSEMBLECOLOR_32ARGB((ULONG)0xff, red, green, blue), (GX_UBYTE)alpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_simple_rotate             */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal help function that hangles 90, 180 and 270 degree rotated  */
/*    pixelmap rotation.                                                  */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
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
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

    clip = context -> gx_draw_context_clip;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        putrow = context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
                {
                    blend_func(context, rotated_clip.gx_rectangle_left + x, rotated_clip.gx_rectangle_top + y, (*get) | 0xff000000, (GX_UBYTE)((*get) >> 24));
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

        width = pixelmap -> gx_pixelmap_height;
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - (width - 1 - cx);
        newypos = ypos + cy - (height - 1 - cy);

        putrow = context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
                {
                    blend_func(context, rotated_clip.gx_rectangle_left + x, rotated_clip.gx_rectangle_top + y, (*get) | 0xff000000, (GX_UBYTE)((*get) >> 24));
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
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        putrow = context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
                {
                    blend_func(context, rotated_clip.gx_rectangle_left + x, rotated_clip.gx_rectangle_top + y, (*get) | 0xff000000, (GX_UBYTE)((*get) >> 24));
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
/*    _gx_display_driver_32bpp_rotated_pixelmap_rotate   PORTABLE C       */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service rotate a rotated pixelmap directly to canvas memory.   */
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
/*    _gx_display_driver_32bpp_rotated_pixelmap_simple_rotate             */
/*    _gx_display_driver_32bpp_rotated_pixelmap_alpha_rotate              */
/*    _gx_display_driver_32bpp_rotated_pixelmap_rotate                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_32bpp_rotated_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                      INT angle, INT rot_cx, INT rot_cy)
{
    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
        if (angle % 90 == 0)
        {
            /* Simple angle rotate: 90 degree, 180 degree and 270 degree.  */
            _gx_display_driver_32bpp_rotated_pixelmap_simple_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /* With alpha.  */
                _gx_display_driver_32bpp_rotated_pixelmap_alpha_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            else
            {
                /* Without alpha.  */
                _gx_display_driver_32bpp_rotated_pixelmap_raw_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
        }
        break;
    }

    return;
}

