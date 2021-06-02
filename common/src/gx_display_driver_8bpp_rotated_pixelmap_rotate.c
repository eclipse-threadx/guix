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
/*    _gx_display_driver_8bpp_rotated_pixelmap_raw_rotate PORTABLE C      */
/*                                                           6.1.4        */
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
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_rotate                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_raw_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                        INT angle, INT cx, INT cy)
{
GX_UBYTE     *putrow;
GX_UBYTE     *put;
GX_UBYTE     *get;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
INT           idxminx;
INT           idxmaxx;
INT           idxmaxy;
INT           mx[] = {-1, 1, 1, -1};
INT           my[] = {1, 1, -1, -1};
INT           xres;
INT           yres;
INT           x;
INT           y;
INT           xx;
INT           yy;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;

    clip = context -> gx_draw_context_clip;

    /* Set transparent color.  */
    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = pixelmap -> gx_pixelmap_height >> 1;
    srcyres = pixelmap -> gx_pixelmap_width >> 1;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context->gx_draw_context_display->gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        srcyres = pixelmap->gx_pixelmap_width - 1 - srcyres;

        ypos = context->gx_draw_context_canvas->gx_canvas_x_resolution - ypos - pixelmap->gx_pixelmap_width;
        cy = pixelmap->gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip->gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip->gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_left);
    }
    else
    {
        srcxres = pixelmap->gx_pixelmap_height - 1 - srcxres;

        xpos = context->gx_draw_context_canvas->gx_canvas_y_resolution - xpos - pixelmap->gx_pixelmap_height;
        cx = pixelmap->gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip->gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip->gx_rectangle_right;
    }

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT((mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv));
    yres = GX_FIXED_VAL_TO_INT((my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv));

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    /* Calculate the new rotation axis. */
    x = GX_FIXED_VAL_TO_INT((cx - srcxres) * cosv - (cy - srcyres) * sinv);
    y = GX_FIXED_VAL_TO_INT((cy - srcyres) * cosv + (cx - srcxres) * sinv);

    x += xres;
    y += yres;

    newxpos = xpos + cx - x;
    newypos = ypos + cy - y;

    /* For every pixel in destination bitmap, find its position in source bitmap,
       and set the pixel with the value in source bitmap.  */
    for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
    {
        put = putrow;

        for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
        {
            xx = GX_FIXED_VAL_TO_INT((x - xres) * cosv + (y - yres) * sinv);
            yy = GX_FIXED_VAL_TO_INT((y - yres) * cosv - (x - xres) * sinv);

            xx += srcxres;
            yy += srcyres;

            if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_height) &&
                (yy >= 0) && (yy < pixelmap -> gx_pixelmap_width))
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += yy * pixelmap -> gx_pixelmap_height;
                get += xx;

                *put = *get;
            }

            put++;
        }
        putrow += context -> gx_draw_context_pitch;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_transparent_rotate         */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
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
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_transparent_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                INT angle, INT cx, INT cy)
{
GX_UBYTE     *putrow;
GX_UBYTE     *put;
GX_UBYTE     *get;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
INT           idxminx;
INT           idxmaxx;
INT           idxmaxy;
INT           mx[] = {-1, 1, 1, -1};
INT           my[] = {1, 1, -1, -1};
INT           xres;
INT           yres;
INT           x;
INT           y;
INT           xx;
INT           yy;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;

    clip = context -> gx_draw_context_clip;

    /* Set transparent color.  */
    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = pixelmap -> gx_pixelmap_height >> 1;
    srcyres = pixelmap -> gx_pixelmap_width >> 1;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context->gx_draw_context_display->gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        srcyres = pixelmap->gx_pixelmap_width - 1 - srcyres;

        ypos = context->gx_draw_context_canvas->gx_canvas_x_resolution - ypos - pixelmap->gx_pixelmap_width;
        cy = pixelmap->gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip->gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip->gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_left);
    }
    else
    {
        srcxres = pixelmap->gx_pixelmap_height - 1 - srcxres;

        xpos = context->gx_draw_context_canvas->gx_canvas_y_resolution - xpos - pixelmap->gx_pixelmap_height;
        cx = pixelmap->gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip->gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip->gx_rectangle_right;
    }

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT((mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv));
    yres = GX_FIXED_VAL_TO_INT((my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv));

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    /* Calculate the new rotation axis. */
    x = (cx - srcxres) * cosv - (cy - srcyres) * sinv;
    y = (cy - srcyres) * cosv + (cx - srcxres) * sinv;

    x = GX_FIXED_VAL_TO_INT(x) + xres;
    y = GX_FIXED_VAL_TO_INT(y) + yres;

    newxpos = xpos + cx - x;
    newypos = ypos + cy - y;

    /* For every pixel in destination bitmap, find its position in source bitmap,
       and set the pixel with the value in source bitmap.  */
    for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
    {
        put = putrow;

        for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
        {
            xx = GX_FIXED_VAL_TO_INT((x - xres) * cosv + (y - yres) * sinv);
            yy = GX_FIXED_VAL_TO_INT((y - yres) * cosv - (x - xres) * sinv);

            xx += srcxres;
            yy += srcyres;

            if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_height) &&
                (yy >= 0) && (yy < pixelmap -> gx_pixelmap_width))
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += yy * pixelmap -> gx_pixelmap_height;
                get += xx;

                if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                {
                    *put = *get;
                }
            }
            put++;
        }
        putrow += context -> gx_draw_context_pitch;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_simple_rotate              */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_rotate                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                           INT angle, INT cx, INT cy)
{
GX_UBYTE     *putrow;
GX_UBYTE     *put;
GX_UBYTE     *get;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;

clip = context->gx_draw_context_clip;

GX_SWAP_VALS(xpos, ypos);
GX_SWAP_VALS(cx, cy);

if (context->gx_draw_context_display->gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
{
    ypos = context->gx_draw_context_canvas->gx_canvas_x_resolution - ypos - pixelmap->gx_pixelmap_width;
    cy = pixelmap->gx_pixelmap_width - cy - 1;

    rotated_clip.gx_rectangle_left = clip->gx_rectangle_top;
    rotated_clip.gx_rectangle_right = clip->gx_rectangle_bottom;
    rotated_clip.gx_rectangle_top = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_right);
    rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_left);
}
else
{
    xpos = context->gx_draw_context_canvas->gx_canvas_y_resolution - xpos - pixelmap->gx_pixelmap_height;
    cx = pixelmap->gx_pixelmap_height - cx - 1;

    rotated_clip.gx_rectangle_left = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_bottom);
    rotated_clip.gx_rectangle_right = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_top);
    rotated_clip.gx_rectangle_top = clip->gx_rectangle_left;
    rotated_clip.gx_rectangle_bottom = clip->gx_rectangle_right;
}

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                *put++ = *get;
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

        putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                *put++ = *get;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
    else
    {
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                *put++ = *get;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_simple_transparent_rotate  */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_rotate                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_simple_transparent_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                       INT angle, INT cx, INT cy)
{
GX_UBYTE     *putrow;
GX_UBYTE     *put;
GX_UBYTE     *get;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;

    clip = context -> gx_draw_context_clip;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context->gx_draw_context_display->gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        ypos = context->gx_draw_context_canvas->gx_canvas_x_resolution - ypos - pixelmap->gx_pixelmap_width;
        cy = pixelmap->gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip->gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip->gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_x_resolution - 1 - clip->gx_rectangle_left);
    }
    else
    {
        xpos = context->gx_draw_context_canvas->gx_canvas_y_resolution - xpos - pixelmap->gx_pixelmap_height;
        cx = pixelmap->gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context->gx_draw_context_canvas->gx_canvas_y_resolution - 1 - clip->gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip->gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip->gx_rectangle_right;
    }

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                {
                    *put = *get;
                }
                put++;
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

        putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                {
                    *put = *get;
                }
                put++;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
    else
    {
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                {
                    *put = *get;
                }
                put++;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_rotate     PORTABLE C      */
/*                                                           6.1.4        */
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
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_simple_transparent_rotate  */
/*    _gx_display_driver_8bpp_rotated_pixelmap_simple_rotate              */
/*    _gx_display_driver_8bpp_rotated_pixelmap_transparent_rotate         */
/*    _gx_display_driver_8bpp_rotated_pixelmap_raw_rotate                 */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_8bpp_rotated_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                             INT angle, INT rot_cx, INT rot_cy)
{
    if (angle % 90 == 0)
    {
        /* Simple angle rotate: 90 degree, 180 degree and 270 degree.  */
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            _gx_display_driver_8bpp_rotated_pixelmap_simple_transparent_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
        else
        {
            _gx_display_driver_8bpp_rotated_pixelmap_simple_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            /* no compression or alpha */
            _gx_display_driver_8bpp_rotated_pixelmap_transparent_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
        else
        {
            /* no compression or alpha */
            _gx_display_driver_8bpp_rotated_pixelmap_raw_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
    }

    return;
}

