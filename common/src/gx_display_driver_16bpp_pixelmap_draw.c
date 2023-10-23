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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixelmap_raw_write        PORTABLE C      */
/*                                                           6.X          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file without alpha channel.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                         INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              xval;
INT              yval;
INT              width;
USHORT          *putrow;
USHORT          *getrow;
USHORT          *put;
GX_CONST USHORT *get;

GX_RECTANGLE    *clip = context -> gx_draw_context_clip;

    putrow = (USHORT *)context -> gx_draw_context_memory;

    GX_CALCULATE_PUTROW(putrow, clip->gx_rectangle_left, clip->gx_rectangle_top, context);

    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = 0; xval < width; xval++)
        {
            *put++ = *get++;
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixelmap_alpha_write      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file with alpha channel.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixel_blend  Display driver basic pixel   */
/*                                             blend function             */
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
static VOID _gx_display_driver_565rgb_pixelmap_alpha_write(GX_DRAW_CONTEXT *context,
                                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                skipcount;
INT                xval;
INT                yval;
USHORT            *getrow;
GX_UBYTE          *getrowalpha;
GX_CONST USHORT   *get;
GX_CONST GX_UBYTE *getalpha;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
void               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    if (blend_func == GX_NULL)
    {
        return;
    }

    /* calculate how many pixels to skip */
    skipcount = (pixelmap -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    skipcount += (clip -> gx_rectangle_left - xpos);
    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    getrowalpha = (GX_UBYTE *)(pixelmap -> gx_pixelmap_aux_data);
    getrowalpha += skipcount;

    for (yval  = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;
        getalpha = getrowalpha;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            blend_func(context, xval, yval, *get++, *getalpha++);
        }
        getrow += pixelmap -> gx_pixelmap_width;
        getrowalpha += pixelmap -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixelmap_compressed_write                 */
/*                                                        PORTABLE C      */
/*                                                           6.X          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file without alpha channel.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              yval;
INT              xval;
GX_CONST USHORT *get;
USHORT          *put;
USHORT          *putrow;
USHORT           count;
USHORT           pixel;

GX_RECTANGLE    *clip = context -> gx_draw_context_clip;

    get = (GX_CONST USHORT *)pixelmap -> gx_pixelmap_data;
    /* compressed with no alpha is a two-byte count and two-byte pixel value */

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1u);
                get++;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row
     */
    putrow = (USHORT *)context -> gx_draw_context_memory;

    GX_CALCULATE_PUTROW(putrow, xpos, yval, context);

    while (yval <= clip -> gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x8000)
            {
                /* repeated value */
                count = (USHORT)((count & 0x7fff) + 1u);
                pixel = *get++;
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        *put = pixel;
                    }
                    put++;
                    xval++;
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;

                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        *put = *get;
                    }
                    put++;
                    get++;
                    xval++;
                }
            }
        }
        putrow +=  context -> gx_draw_context_pitch;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixelmap_compressed_alpha_write           */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file with alpha channel.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixel_blend  Display driver basic pixel   */
/*                                             blend function             */
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
static VOID _gx_display_driver_565rgb_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                      INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_CONST USHORT   *getpixel;
USHORT             count;
USHORT             pixel;
GX_UBYTE           falpha;
GX_UBYTE           brush_alpha;
GX_UBYTE           combined_alpha;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
void               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    if (blend_func == GX_NULL)
    {
        return;
    }

    get = pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* compressed with alpha is byte count, byte alpha, and and two-byte pixel value */

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get;

            if (count & 0x80)
            {
                count = (USHORT)((count & 0x7f) + 1u);
                get +=  4;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += (count * 4);      /* skip string of non-repeated pixels */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row
     */
    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;
        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get;

            if (count & 0x80)
            {
                /* repeated value */
                count = (USHORT)((count & 0x7f) + 1u);
                falpha = *(get + 1);

                if (falpha)
                {
                    get += 2;

                    getpixel = (USHORT *)get;
                    pixel = *getpixel;
                    get += 2;

                    if (brush_alpha == 0xff)
                    {
                        combined_alpha = falpha;
                    }
                    else
                    {
                        combined_alpha = (GX_UBYTE)(falpha * brush_alpha / 255);
                    }

                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, pixel, combined_alpha);
                        }
                        xval++;
                    }
                }
                else
                {
                    get += 4;
                    xval += count;
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            falpha = *(get + 1);
                            get += 2;
                            getpixel = (USHORT *)get;
                            pixel = *getpixel;
                            get += 2;
                            blend_func(context, xval, yval, pixel, falpha);
                        }
                        else
                        {
                            get += 4;
                        }
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            falpha = *(get + 1);
                            get += 2;
                            getpixel = (USHORT *)get;
                            pixel = *getpixel;
                            get += 2;
                            combined_alpha = (GX_UBYTE)(falpha * brush_alpha / 255);
                            blend_func(context, xval, yval, pixel, combined_alpha);
                        }
                        else
                        {
                            get += 4;
                        }
                        xval++;
                    }
                }
            }
        }
        yval++;
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_palette_pixelmap_raw_write  PORTABLE C    */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of raw pixlemap       */
/*    file without transparent for palette pixelmap                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_palette_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                xval;
INT                yval;
INT                width;
USHORT            *putrow;
GX_UBYTE          *getrow;
USHORT            *put;
GX_CONST GX_UBYTE *get;
GX_COLOR          *palette;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    putrow = (USHORT *)context -> gx_draw_context_memory;

    GX_CALCULATE_PUTROW(putrow, clip->gx_rectangle_left, clip->gx_rectangle_top, context);

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = 0; xval < width; xval++)
        {
            r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
            g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
            b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get++]) >> 3);
            *put++ = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_palette_pixelmap_transparent_raw_write    */
/*                                                           PORTABLE C   */
/*                                                           6.X          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of raw pixlemap       */
/*    file with transparent for palette pixelmap.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_palette_pixelmap_transparent_raw_write(GX_DRAW_CONTEXT *context,
                                                                             INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                xval;
INT                yval;
INT                width;
USHORT            *putrow;
GX_UBYTE          *getrow;
USHORT            *put;
GX_CONST GX_UBYTE *get;
GX_COLOR          *palette;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    putrow = (USHORT *)context->gx_draw_context_memory;
    GX_CALCULATE_PUTROW(putrow, clip->gx_rectangle_left, clip->gx_rectangle_top, context);

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = 0; xval < width; xval++)
        {
            if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
            {
                r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get]) >> 3);
                *put = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
            }
            get++;
            put++;
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_palette_pixelmap_transparent_compressed   */
/*      _write                                                            */
/*                                                        PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file with transparent for palette pixelmap                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixel_blend  Display driver basic pixel   */
/*                                             blend function             */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_palette_pixelmap_transparent_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                    INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
USHORT            *put;
USHORT            *putrow;
GX_COLOR          *palette;
GX_UBYTE           brush_alpha;
USHORT             count;
USHORT             pixel;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;
GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    get = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* compressed with no alpha is a one-byte count and one-byte index value */
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (USHORT)((count & 0x7f) + 1u);
                get++;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row. */
    putrow = (USHORT *)context -> gx_draw_context_memory;
    GX_CALCULATE_PUTROW(putrow, xpos, yval, context);

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    while (yval <= clip -> gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (USHORT)((count & 0x7f) + 1u);

                if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                {
                    r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                    g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                    b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get]) >> 3);

                    if (brush_alpha == 0xff)
                    {
                        while (count--)
                        {
                            if (xval >= clip -> gx_rectangle_left &&
                                xval <= clip -> gx_rectangle_right)
                            {
                                *put = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                            }
                            put++;
                            xval++;
                        }
                    }
                    else
                    {
                        pixel = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                        while (count--)
                        {
                            if (xval >= clip -> gx_rectangle_left &&
                                xval <= clip -> gx_rectangle_right)
                            {
                                _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, brush_alpha);
                            }
                            xval++;
                        }
                    }
                }
                else
                {
                    put += count;
                    xval += count;
                }
                get++;
            }
            else
            {
                /* string of non-repeated values */
                count++;

                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                        {
                            if (xval >= clip -> gx_rectangle_left &&
                                xval <= clip -> gx_rectangle_right)
                            {
                                r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                                g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                                b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get]) >> 3);
                                *put = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                            }
                        }
                        put++;
                        get++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                        {
                            if (xval >= clip -> gx_rectangle_left &&
                                xval <= clip -> gx_rectangle_right)
                            {
                                r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                                g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                                b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get]) >> 3);
                                pixel = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                                _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, brush_alpha);
                            }
                        }
                        get++;
                        xval++;
                    }
                }
            }
        }
        putrow += context -> gx_draw_context_pitch;
        yval++;
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_palette_pixelmap_compressed_write         */
/*                                                        PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file without alpha channel for palette pixelmap.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_palette_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                        INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
USHORT            *put;
USHORT            *putrow;
GX_COLOR          *palette;
USHORT             count;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;
GX_UBYTE           brush_alpha;
GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    get = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* compressed with no alpha is a one-byte count and one-byte index value */
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (USHORT)((count & 0x7f) + 1u);
                get++;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row. */
    putrow = (USHORT *)context -> gx_draw_context_memory;
    GX_CALCULATE_PUTROW(putrow, xpos, yval, context);

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    while (yval <= clip -> gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (USHORT)((count & 0x7f) + 1u);

                r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get++]) >> 3);

                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            *put = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                        }
                        put++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_565rgb_pixel_blend(context, xval, yval, (USHORT)ASSEMBLECOLOR_16BPP(r, g, b), brush_alpha);
                        }
                        xval++;
                    }
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                            g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                            b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get]) >> 3);
                            *put = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                        }

                        put++;
                        get++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            r = (GX_UBYTE)(REDVAL_32BPP(palette[*get]) >> 3);
                            g = (GX_UBYTE)(GREENVAL_32BPP(palette[*get]) >> 2);
                            b = (GX_UBYTE)(BLUEVAL_32BPP(palette[*get]) >> 3);
                            _gx_display_driver_565rgb_pixel_blend(context, xval, yval, (USHORT)ASSEMBLECOLOR_16BPP(r, g, b), brush_alpha);
                        }

                        get++;
                        xval++;
                    }
                }
            }
        }
        putrow += context -> gx_draw_context_pitch;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_16bpp_4444argb_pixelmap_raw_write                */
/*                                                         PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file with alpha channel of 4444argb format.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixel_blend  Display driver basic pixel   */
/*                                             blend function             */
/*    _gx_display_driver_16bpp_pixel_write   Display driver basic pixel   */
/*                                             write function             */
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
static VOID _gx_display_driver_16bpp_4444argb_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              skipcount;
INT              xval;
INT              yval;
USHORT          *getrow;
GX_CONST USHORT *get;
UCHAR            alpha_value;
USHORT           pixel;

GX_RECTANGLE    *clip = context -> gx_draw_context_clip;

    /* calculate how many pixels to skip */
    skipcount = (pixelmap -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    skipcount += (clip -> gx_rectangle_left - xpos);
    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            /* 0x000f- -> b , 0x00f0- -> g , 0x0f00- -> r , 0xf000- -> a */
            /*4444bgra - ->  565rgb*/
            alpha_value = (UCHAR)(((*get) & 0xf000) >> 8);
            alpha_value = alpha_value | (alpha_value >> 4);
            if (alpha_value)
            {
                pixel = (USHORT)((((*get) & 0x0f00) << 4) | (((*get) & 0x00f0) << 3) | (((*get) & 0x000f) << 1));
                if (alpha_value == 0xff)
                {
                    _gx_display_driver_16bpp_pixel_write(context, xval, yval, pixel);
                }
                else
                {
                    _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                }
            }
            get++;
        }
        getrow += pixelmap -> gx_pixelmap_width;
    }
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_16bpp_4444argb_pixelmap_compressed_write         */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixelmap data of format 4444argb in 16bpp drivers.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixel_blend  Display driver basic pixel   */
/*                                             blend function             */
/*    _gx_display_driver_16bpp_pixel_write   Display driver basic pixel   */
/*                                             write function             */
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
static VOID _gx_display_driver_16bpp_4444argb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                        INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              yval;
INT              xval;
GX_CONST USHORT *get;
USHORT           count;
USHORT           pixel;
GX_UBYTE         alpha_value;
GX_UBYTE         combined_alpha;
GX_UBYTE         brush_alpha;
GX_UBYTE         r;
GX_UBYTE         g;
GX_UBYTE         b;

GX_RECTANGLE    *clip = context -> gx_draw_context_clip;

    get = (GX_CONST USHORT *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1u);
                get++;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row
     */
    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x8000)
            {
                /* repeated value */
                count = (USHORT)((count & 0x7fff) + 1u);
                pixel = *get++;
                alpha_value = (GX_UBYTE)((pixel & 0xf000) >> 8);
                alpha_value = (alpha_value >> 4) | alpha_value;
                if (alpha_value)
                {
                    r = (GX_UBYTE)((pixel & 0x0f00) >> 7);
                    g = (GX_UBYTE)((pixel & 0x00f0) >> 2);
                    b = (GX_UBYTE)((pixel & 0x000f) << 1);
                    pixel = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);

                    if (brush_alpha == 0xff)
                    {
                        while (count--)
                        {
                            if (xval >= clip -> gx_rectangle_left &&
                                xval <= clip -> gx_rectangle_right)
                            {
                                if (alpha_value == 0xff)
                                {
                                    _gx_display_driver_16bpp_pixel_write(context, xval, yval, pixel);
                                }
                                else
                                {
                                    _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                                }
                            }
                            xval++;
                        }
                    }
                    else
                    {
                        while (count--)
                        {
                            if (xval >= clip -> gx_rectangle_left &&
                                xval <= clip -> gx_rectangle_right)
                            {
                                combined_alpha = (GX_UBYTE)(brush_alpha * alpha_value / 255);
                                _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, combined_alpha);
                            }
                            xval++;
                        }
                    }
                }
                else
                {
                    while (count--)
                    {
                        xval++;
                    }
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;

                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            pixel = *get;
                            alpha_value = (GX_UBYTE)((pixel & 0xf000) >> 8);
                            alpha_value = (alpha_value >> 4) | alpha_value;
                            r = (GX_UBYTE)((pixel & 0x0f00) >> 7);
                            g = (GX_UBYTE)((pixel & 0x00f0) >> 2);
                            b = (GX_UBYTE)((pixel & 0x000f) << 1);
                            pixel = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                            if (alpha_value)
                            {
                                if (alpha_value == 0xff)
                                {
                                    _gx_display_driver_16bpp_pixel_write(context, xval, yval, pixel);
                                }
                                else
                                {
                                    _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                                }
                            }
                        }
                        get++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            pixel = *get;
                            alpha_value = (GX_UBYTE)((pixel & 0xf000) >> 8);
                            alpha_value = (alpha_value >> 4) | alpha_value;
                            r = (GX_UBYTE)((pixel & 0x0f00) >> 7);
                            g = (GX_UBYTE)((pixel & 0x00f0) >> 2);
                            b = (GX_UBYTE)((pixel & 0x000f) << 1);
                            pixel = (USHORT)ASSEMBLECOLOR_16BPP(r, g, b);
                            combined_alpha = (GX_UBYTE)(brush_alpha * alpha_value / 255);
                            _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, combined_alpha);
                        }
                        get++;
                        xval++;
                    }
                }
            }
        }
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixelmap_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb screen driver pixelmap drawing function that handles         */
/*    compressed or uncompress, with or without alpha channel.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_565rgb_pixelmap_compressed_alpha_write          */
/*     _gx_display_driver_565rgb_pixelmap_alpha_write                     */
/*     _gx_display_driver_565rgb_pixelmap_compressed_write                */
/*     _gx_display_driver_565rgb_pixelmap_raw_write                       */
/*     _gx_display_driver_565rgb_palette_pixelmap_compressed_write        */
/*     _gx_display_driver_565rgb_palette_pixelmap_raw_write               */
/*     _gx_display_driver_16bpp_4444argb_pixelmap_raw_write               */
/*     _gx_display_driver_565rgb_palette_pixelmap_transparent_compressed  */
/*       _write                                                           */
/*     _gx_display_driver_565rgb_palette_pixelmap_transparent_raw_write   */
/*     _gx_display_driver_16bpp_4444argb_pixelmap_compressed_write        */
/*     _gx_display_driver_565rgb_pixelmap_blend                           */
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
VOID _gx_display_driver_565rgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                             INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_BOOL  drawn = GX_FALSE;
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        /* Draw nothing here. Just return. */
        return;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (pixelmap -> gx_pixelmap_aux_data == GX_NULL)
        {
            break;
        }

        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* compressed with */
                _gx_display_driver_565rgb_palette_pixelmap_transparent_compressed_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* no compression */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_565rgb_palette_pixelmap_transparent_raw_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* compressed with */

                _gx_display_driver_565rgb_palette_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* no compression */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_565rgb_palette_pixelmap_raw_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        break;

    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_565RGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* has both compression and alpha */
                _gx_display_driver_565rgb_pixelmap_compressed_alpha_write(context,
                                                                          xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* alpha, no compression */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_565rgb_pixelmap_alpha_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        else
        {
            if (brush_alpha == 0xff)
            {
                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
                {
                    /* compressed with no alpha */
                    _gx_display_driver_565rgb_pixelmap_compressed_write(context,
                                                                        xpos, ypos, pixelmap);
                }
                else
                {
                    /* no compression or alpha */
                    _gx_display_driver_565rgb_pixelmap_raw_write(context,
                                                                 xpos, ypos, pixelmap);
                }
                drawn = GX_TRUE;
            }
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /*not write yet*/
            _gx_display_driver_16bpp_4444argb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        else
        {
            if (brush_alpha == 0xff)
            {
                _gx_display_driver_16bpp_4444argb_pixelmap_raw_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
        }
        break;

    default:
        drawn = GX_TRUE;
        break;
    }

    if ((!drawn) && (brush_alpha != 0xff))
    {
        _gx_display_driver_565rgb_pixelmap_blend(context, xpos, ypos, pixelmap, brush_alpha);
    }

    return;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1555xrgb_pixelmap_draw           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1555xrgb screen driver pixelmap drawing function that handles       */
/*    compressed or uncompress, with or without alpha channel.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_565rgb_pixelmap_compressed_alpha_write          */
/*     _gx_display_driver_565rgb_pixelmap_alpha_write                     */
/*     _gx_display_driver_565rgb_pixelmap_compressed_write                */
/*     _gx_display_driver_565rgb_pixelmap_raw_write                       */
/*     _gx_display_driver_15555xrgb_pixelmap_blend                        */
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
VOID _gx_display_driver_1555xrgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_BOOL  drawn = GX_FALSE;
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        /* Draw nothing here. Just return. */
        return;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_1555XRGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* has both compression and alpha */
                _gx_display_driver_565rgb_pixelmap_compressed_alpha_write(context,
                                                                          xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* alpha, no compression */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_565rgb_pixelmap_alpha_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        else
        {
            if (brush_alpha == 0xff)
            {
                if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
                {
                    /* compressed with no alpha */
                    _gx_display_driver_565rgb_pixelmap_compressed_write(context,
                                                                        xpos, ypos, pixelmap);
                }
                else
                {
                    /* no compression or alpha */
                    _gx_display_driver_565rgb_pixelmap_raw_write(context,
                                                                 xpos, ypos, pixelmap);
                }
                drawn = GX_TRUE;
            }
        }
        break;

    default:
        return;
    }

    if (!drawn)
    {
        _gx_display_driver_1555xrgb_pixelmap_blend(context, xpos, ypos, pixelmap, brush_alpha);
    }

    return;
}

