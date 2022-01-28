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
/*    _gx_display_driver_4bpp_pixelmap_raw_rotate         PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*      without transparent.                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    rot_cx                                x-coord of rotate center      */
/*    rot_cy                                y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_rotate                             */
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
static VOID _gx_display_driver_4bpp_pixelmap_raw_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
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
INT           newxpos;
INT           newypos;
GX_UBYTE      putmask;
INT           putstride;
INT           getstride;
GX_UBYTE      color;

    clip = context -> gx_draw_context_clip;

    /* Set transparent color.  */
    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = pixelmap -> gx_pixelmap_width >> 1;
    srcyres = pixelmap -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT((mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv));
    yres = GX_FIXED_VAL_TO_INT((my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv));

    getstride = (pixelmap -> gx_pixelmap_width + 1) >> 1;
    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 1;

    /* Calculate the new rotation axis. */
    xres = GX_FIXED_VAL_TO_INT((cx - srcxres) * cosv - (cy - srcyres) * sinv) + xres;
    yres = GX_FIXED_VAL_TO_INT((cy - srcyres) * cosv + (cx - srcxres) * sinv) + yres;

    newxpos = xpos + cx - xres;
    newypos = ypos + cy - yres;

    /* For every pixel in destination bitmap, find its position in source bitmap,
       and set the pixel with the value in source bitmap.  */
    for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
    {
        if (clip -> gx_rectangle_left & 1)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }
        put = putrow;

        for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
        {
            xx = GX_FIXED_VAL_TO_INT((x - xres) * cosv + (y - yres) * sinv) + cx;
            yy = GX_FIXED_VAL_TO_INT((y - yres) * cosv - (x - xres) * sinv) + cy;

            if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_width) &&
                (yy >= 0) && (yy < pixelmap -> gx_pixelmap_height))
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += yy * getstride;
                get += xx >> 1;

                if (xx & 1)
                {
                    color = *get & 0x0f;
                }
                else
                {
                    color = (*get & 0xf0) >> 4;
                }
                color |= (GX_UBYTE)(color << 4);

                *put &= (GX_UBYTE)(~putmask);
                *put |= color & putmask;
            }

            putmask >>= 4;
            if (putmask == 0)
            {
                put++;
                putmask = 0xf0;
            }
        }
        putrow += putstride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_transparent_rotate PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*      with transparent info.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    rot_cx                                x-coord of rotate center      */
/*    rot_cy                                y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_rotate                             */
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
static VOID _gx_display_driver_4bpp_pixelmap_transparent_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                INT angle, INT cx, INT cy)
{
GX_UBYTE     *putrow;
GX_UBYTE     *put;
GX_UBYTE     *get;
GX_UBYTE     *getaux;
GX_UBYTE      putmask;
GX_UBYTE      transmask;
GX_UBYTE      pixel;
INT           putstride;
INT           getauxstride;
INT           getstride;
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
INT           newxpos;
INT           newypos;

    clip = context -> gx_draw_context_clip;
    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    getstride = (pixelmap -> gx_pixelmap_width + 1) >> 1;
    getauxstride = (pixelmap -> gx_pixelmap_width + 7) >> 3;

    /* Set transparent color.  */
    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = pixelmap -> gx_pixelmap_width >> 1;
    srcyres = pixelmap -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT((mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv));
    yres = GX_FIXED_VAL_TO_INT((my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv));

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 1;

    /* Calculate the new rotation axis. */
    x = (cx - srcxres) * cosv - (cy - srcyres) * sinv;
    y = (cy - srcyres) * cosv + (cx - srcxres) * sinv;

    xres = GX_FIXED_VAL_TO_INT(x) + xres;
    yres = GX_FIXED_VAL_TO_INT(y) + yres;

    newxpos = xpos + cx - xres;
    newypos = ypos + cy - yres;

    /* For every pixel in destination bitmap, find its position in source bitmap,
       and set the pixel with the value in source bitmap.  */
    for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
    {
        put = putrow;
        if (clip -> gx_rectangle_left & 1)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }

        for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
        {
            xx = GX_FIXED_VAL_TO_INT((x - xres) * cosv + (y - yres) * sinv) + cx;
            yy = GX_FIXED_VAL_TO_INT((y - yres) * cosv - (x - xres) * sinv) + cy;

            if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_width) &&
                (yy >= 0) && (yy < pixelmap -> gx_pixelmap_height))
            {
                getaux = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                getaux += yy * getauxstride;
                getaux += xx >> 3;

                transmask = (GX_UBYTE)(0x80 >> (xx & 0x07));
                if (!(transmask & (*getaux)))
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    get += yy * getstride;
                    get += xx >> 1;

                    if (xx & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = *get >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);

                    *put &= (GX_UBYTE)(~putmask);
                    *put |= pixel & putmask;
                }
            }

            putmask >>= 4;
            if (putmask == 0)
            {
                put++;
                putmask = 0xf0;
            }
        }
        putrow += putstride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_simple_rotate      PORTABLE C      */
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
/*    rot_cx                                x-coord of rotate center      */
/*    rot_cy                                y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_rotate                             */
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
static VOID _gx_display_driver_4bpp_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                           INT angle, INT cx, INT cy)
{
GX_UBYTE     *putrow;
GX_UBYTE     *put;
GX_UBYTE      putmask;
GX_UBYTE     *get;
INT           putstride;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
INT           newxpos;
INT           newypos;
GX_UBYTE      color;

    clip = context -> gx_draw_context_clip;
    putstride = (context -> gx_draw_context_pitch + 1) >> 1;

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 1;

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_height;
        height = (pixelmap -> gx_pixelmap_width + 1) >> 1;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            if (clip -> gx_rectangle_left & 1)
            {
                putmask = 0x0f;
            }
            else
            {
                putmask = 0xf0;
            }

            put = putrow;

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y >> 1;

                if (y & 1)
                {
                    color = *get & 0x0f;
                }
                else
                {
                    color = (*get & 0xf0) >> 4;
                }
                color |= (GX_UBYTE)(color << 4);

                *put &= (GX_UBYTE)(~putmask);
                *put |= color & putmask;

                putmask >>= 4;
                if (putmask == 0)
                {
                    putmask = 0xf0;
                    put++;
                }
            }

            putrow += putstride;
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
            if (clip -> gx_rectangle_left & 1)
            {
                putmask = 0x0f;
            }
            else
            {
                putmask = 0xf0;
            }
            put = putrow;

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += (height - 1 - y) * ((width + 1) >> 1);
                get += (width - 1 - x) >> 1;

                if ((pixelmap -> gx_pixelmap_width - 1 - x) & 1)
                {
                    color = *get & 0x0f;
                }
                else
                {
                    color = (*get & 0xf0) >> 4;
                }
                color |= (GX_UBYTE)(color << 4);
                *put &= (GX_UBYTE)(~putmask);
                *put |= color & putmask;

                putmask >>= 4;
                if (putmask == 0)
                {
                    putmask = 0xf0;
                    put++;
                }
            }
            putrow += putstride;
        }
    }
    else
    {
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            if (clip -> gx_rectangle_left & 1)
            {
                putmask = 0x0f;
            }
            else
            {
                putmask = 0xf0;
            }
            put = putrow;

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                get += x * ((height + 1) >> 1);
                get += (height - 1 - y) >> 1;

                if ((height - 1 - y) & 1)
                {
                    color = *get & 0x0f;
                }
                else
                {
                    color = (*get & 0xf0) >> 4;
                }
                color |= (GX_UBYTE)(color << 4);

                *put &= (GX_UBYTE)(~putmask);
                *put |= color & putmask;

                putmask >>= 4;
                if (putmask == 0)
                {
                    putmask = 0xf0;
                    put++;
                }
            }

            putrow += putstride;
        }
    }
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_simple_transparent_rotate          */
/*                                                        PORTABLE C      */
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
/*    rot_cx                                x-coord of rotate center      */
/*    rot_cy                                y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_rotate                             */
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
static VOID _gx_display_driver_4bpp_pixelmap_simple_transparent_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                       INT angle, INT cx, INT cy)
{
GX_UBYTE     *get;
GX_UBYTE     *gettransmask;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
INT           newxpos;
INT           newypos;
GX_UBYTE     *put;
GX_UBYTE     *putrow;
INT           putstride;
GX_UBYTE      putmask;
INT           getstride;
INT           getauxstride;
GX_UBYTE      transmask;
GX_UBYTE      pixel;

    if (pixelmap -> gx_pixelmap_aux_data == GX_NULL)
    {
        /* 4bpp transparent pixelmap must have aux data to store the transparent mask info. */
        return;
    }

    clip = context -> gx_draw_context_clip;
    /* 4bpp pixelmap is stored as half byte. And it's transparent info is stored in aux data by one bit */
    getstride = (pixelmap -> gx_pixelmap_width + 1) >> 1;
    getauxstride = (pixelmap -> gx_pixelmap_width + 7) >> 3;
    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 1;

    clip = context -> gx_draw_context_clip;

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            if (clip -> gx_rectangle_left & 1)
            {
                putmask = 0x0f;
            }
            else
            {
                putmask = 0xf0;
            }

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                gettransmask = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                gettransmask += (width - 1 - x) * getauxstride;
                gettransmask += y >> 3;

                transmask = (GX_UBYTE)(0x80 >> (y & 0x07));

                /* if not transparent, draw pixel. else skip. */
                if (!(transmask & (*gettransmask)))
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    get += (width - 1 - x) * getstride;
                    get += y >> 1;

                    if (y & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = *get >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);

                    *put &= (GX_UBYTE)(~putmask);
                    *put |= pixel & putmask;
                }

                putmask >>= 4;
                if (putmask == 0)
                {
                    putmask = 0xf0;
                    put++;
                }
            }
            putrow += putstride;
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
            put = putrow;
            if (clip -> gx_rectangle_left & 1)
            {
                putmask = 0x0f;
            }
            else
            {
                putmask = 0xf0;
            }
            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                gettransmask = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                gettransmask += (height - 1 - y) * getauxstride;
                gettransmask += (width - 1 - x) >> 3;

                /* Calculate and go ot the transmask bit.*/
                transmask = (GX_UBYTE)(0x80 >> ((width - 1 - x) & 0x07));
                /* if not transparent, draw pixel. else skip. */
                if (!(transmask & (*gettransmask)))
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    get += (height - 1 - y) * getstride;
                    get += (width - 1 - x) >> 1;

                    if ((width - 1 - x) & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = *get >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);

                    *put &= (GX_UBYTE)(~putmask);
                    *put |= pixel & putmask;
                }

                putmask >>= 4;
                if (putmask == 0)
                {
                    putmask = 0xf0;
                    put++;
                }
            }
            putrow += putstride;
        }
    }
    else
    {
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        for (y = clip -> gx_rectangle_top - newypos; y <= clip -> gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            if (clip -> gx_rectangle_left & 1)
            {
                putmask = 0x0f;
            }
            else
            {
                putmask = 0xf0;
            }

            for (x = clip -> gx_rectangle_left - newxpos; x <= clip -> gx_rectangle_right - newxpos; x++)
            {
                gettransmask = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                gettransmask += x * getauxstride;
                gettransmask += (height - 1 - y) >> 3;

                transmask = (GX_UBYTE)(0x80 >> ((height - 1 - y) & 0x07));
                /* if not transparent, draw pixel. else skip. */
                if (!(transmask & (*gettransmask)))
                {
                    get = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
                    get += x * getstride;
                    get += (height - 1 - y) >> 1;

                    if ((height - 1 - y) & 1)
                    {
                        pixel = *get & 0x0f;
                    }
                    else
                    {
                        pixel = *get >> 4;
                    }
                    pixel |= (GX_UBYTE)(pixel << 4);

                    *put &= (GX_UBYTE)(~putmask);
                    *put |= pixel & putmask;
                }

                putmask >>= 4;
                if (putmask == 0)
                {
                    putmask = 0xf0;
                    put++;
                }
            }
            putrow += putstride;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_rotate             PORTABLE C      */
/*                                                           6.1          */
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
/*    _gx_display_driver_4bpp_pixelmap_simple_transparent_rotate          */
/*                                          Real pixelmap rotate routine  */
/*    _gx_display_driver_4bpp_pixelmap_simple_rotate                      */
/*                                          Real pixelmap rotate routine  */
/*    _gx_display_driver_4bpp_pixelmap_transparent_rotate                 */
/*                                          Real pixelmap rotate routine  */
/*    _gx_display_driver_4bpp_pixelmap_raw_rotate                         */
/*                                          Real pixelmap rotate routine  */
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
VOID _gx_display_driver_4bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                             INT angle, INT rot_cx, INT rot_cy)
{
    if (angle % 90 == 0)
    {
        /* Simple angle rotate: 90 degree, 180 degree and 270 degree.  */
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            _gx_display_driver_4bpp_pixelmap_simple_transparent_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
        else
        {
            _gx_display_driver_4bpp_pixelmap_simple_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            /* no compression or alpha */
            _gx_display_driver_4bpp_pixelmap_transparent_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
        else
        {
            /* no compression or alpha */
            _gx_display_driver_4bpp_pixelmap_raw_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
        }
    }

    return;
}

