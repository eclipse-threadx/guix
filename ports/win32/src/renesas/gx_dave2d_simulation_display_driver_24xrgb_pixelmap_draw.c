/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Synergy Simulation Display Management (Display)                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_dave2d_simulation_display_driver.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_s_display_driver_dave2d_24xrgb_pixelmap_c_a_write        */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
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
/*     _gx_display_driver_24xrgb_pixel_blend                              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                   INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
ULONG           pixel;
UCHAR           alpha_value;
GX_BOOL         has_alpha;
GX_UBYTE        brush_alpha;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        has_alpha = GX_TRUE;
    }
    else
    {
        has_alpha = GX_FALSE;
    }
    /* compressed with  alpha is a one-byte count and four one-byte pixel value */
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (UCHAR)((count & 0x7f) + 1);
                get += 4;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count * 4;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row. */

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (UCHAR)((count & 0x7f) + 1);
                pixel = (*get++);
                pixel = (ULONG)((*get++) << 8) | pixel;
                pixel = (ULONG)((*get++) << 16) | pixel;

                if (has_alpha)
                {
                    alpha_value = *get++;
                    if (brush_alpha != 0xff)
                    {
                        alpha_value = (GX_UBYTE)(brush_alpha * alpha_value / 255);
                    }
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                        }
                        xval++;
                    }
                }
                else
                {
                    get += 1;
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            if (brush_alpha != 0xff)
                            {
                                _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, brush_alpha);
                            }
                            else
                            {
                                _gx_display_driver_32bpp_pixel_write(context, xval, yval, pixel);
                            }
                        }
                        xval++;
                    }
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
                        pixel = (ULONG)((*(get)) | ((*(get + 1)) << 8) | ((*(get + 2)) << 16));
                        if (has_alpha)
                        {
                            alpha_value = *(get + 3);
                            if (brush_alpha != 0xff)
                            {
                                alpha_value = (GX_UBYTE)(brush_alpha * alpha_value / 255);
                            }
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                        }
                        else
                        {
                            if (brush_alpha != 0xff)
                            {
                                _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, brush_alpha);
                            }
                            else
                            {
                                _gx_display_driver_32bpp_pixel_write(context, xval, yval, pixel);
                            }
                        }
                    }
                    get += 4;
                    xval++;
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
/*    _gx_synegy_s_display_driver_32bpp_4444argb_dave2d_pixelmap_c        */
/*    _alpha_write                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
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
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_dave2d_simulation_display_driver_32bpp_4444argb_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
GX_COLOR        pixel;
UCHAR           alpha_value;
UCHAR           temp;
GX_UBYTE        brush_alpha;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* compressed with no alpha is a one-byte count and two one-byte pixel value */
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (UCHAR)((count & 0x7f) + 1);
                get += 2;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count * 2;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row. */

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (UCHAR)((count & 0x7f) + 1);
                pixel = 0;
                /*two one-byte ,first byte is gb , second is ar*/
                alpha_value = (*(get + 1)) & 0xf0;
                if (alpha_value)
                {
                    /*first byte , 0xf0 --> g , 0x0f --> b */
                    temp  = (*get) & 0xf0;
                    pixel = (GX_COLOR)(temp << 8) | pixel;
                    temp = (*get++) & 0x0f;
                    pixel = (GX_COLOR)(temp << 4) | pixel;
                    /*second byte , 0xf0 --> a , 0x0f --> r */
                    temp  = (*get++) & 0x0f;
                    pixel = (GX_COLOR)(temp << 20) | pixel;
                    alpha_value = (GX_UBYTE)(alpha_value | (alpha_value >> 4));
                    alpha_value = (GX_UBYTE)(alpha_value * brush_alpha / 255);

                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                        }
                        xval++;
                    }
                }
                else
                {
                    get += 2;
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
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        /*second byte , 0xf0 --> a , 0x0f --> r */
                        temp  = (*(get + 1)) & 0xf0;
                        alpha_value = temp;
                        pixel = 0;
                        if (alpha_value)
                        {
                            /*first byte , 0xf0 --> g , 0x0f --> b */
                            temp  = (*get) & 0xf0;
                            pixel = (GX_COLOR)(temp << 8) | pixel;
                            temp  = (*get) & 0x0f;
                            pixel = (GX_COLOR)(temp << 4) | pixel;
                            /*second byte , 0xf0 --> a , 0x0f --> r */
                            temp  = (*(get + 1)) & 0x0f;
                            pixel = (GX_COLOR)(temp << 20) | pixel;
                            alpha_value = (GX_UBYTE)(alpha_value | (alpha_value >> 4));
                            alpha_value = (GX_UBYTE)(alpha_value * brush_alpha / 255);
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                        }
                    }

                    get += 2;
                    xval++;
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
/*    _gx_dave2d_s_display_driver_32bpp_4444bgra_dave2d_pixelmap_c_       */
/*    alpha_write                                         PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
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
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_dave2d_simulation_display_driver_32bpp_4444bgra_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
GX_COLOR        pixel;
UCHAR           alpha_value;
UCHAR           temp;
GX_UBYTE        brush_alpha;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* compressed with no alpha is a one-byte count and two one-byte pixel value */
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (UCHAR)((count & 0x7f) + 1);
                get += 2;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count * 2;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row. */

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (UCHAR)((count & 0x7f) + 1);
                pixel = 0;
                /*two one-byte ,first byte is ra , second is bg*/
                /*first byte , 0xf0 --> r , 0x0f --> a */
                temp = (*get) & 0x0f;
                alpha_value = (UCHAR)(temp << 4) | temp;
                if (alpha_value)
                {
                    temp  = (*get++) & 0xf0;
                    pixel = (GX_COLOR)(temp << 16) | pixel;
                    /*second byte , 0xf0 --> b , 0x0f --> g */
                    temp  = (GX_COLOR)(*get) & 0xf0;
                    pixel = temp | pixel;
                    temp  = (*get++) & 0x0f;
                    pixel = (GX_COLOR)(temp << 12) | pixel;
                    alpha_value = (GX_UBYTE)(alpha_value * brush_alpha / 255);
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                        }
                        xval++;
                    }
                }
                else
                {
                    get += 2;
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
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        pixel = 0;
                        /*first byte , 0xf0 --> g , 0x0f --> b */
                        temp  = (*get) & 0x0f;
                        alpha_value = (UCHAR)(temp << 4) | temp;
                        if (alpha_value)
                        {
                            /*first byte , 0xf0 --> g , 0x0f --> b */
                            temp  = (*get) & 0xf0;
                            pixel = (GX_COLOR)(temp << 16) | pixel;
                            /*second byte , 0xf0 --> a , 0x0f --> r */
                            temp  = (*(get + 1)) & 0xf0;
                            pixel = temp | pixel;
                            temp  = (*(get + 1)) & 0x0f;
                            pixel = (GX_COLOR)(temp << 12) | pixel;
                            alpha_value = (GX_UBYTE)(alpha_value * brush_alpha / 255);

                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                        }
                    }

                    get += 2;
                    xval++;
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
/*    _gx_dave2d_s_display_driver_24xrgb_565rgb_pixelmap_c_write          */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of 565rgb format      */
/*    compressed pixlemap file without alpha channel.                     */
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
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_dave2d_simulation_display_driver_24xrgb_565rgb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                          INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_COLOR          *put;
GX_COLOR          *putrow;
UCHAR              count;
USHORT             pixel;
GX_COLOR           pixel_24;
GX_UBYTE           brush_alpha;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* compressed with no alpha is a one-byte count and two one-byte pixel value */
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (UCHAR)((count & 0x7f) + 1);
                get += 2;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count * 2;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row. */
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

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
                count = (UCHAR)((count & 0x7f) + 1);

                pixel = *get++;
                pixel = (USHORT)((*get++) << 8) | pixel;

                /* convert 565rgb format to 24xrgb format */
                pixel_24 = ASSEMBLECOLOR_32BPP((GX_COLOR)(REDVAL_16BPP(pixel) << 3),
                                               (GX_COLOR)(GREENVAL_16BPP(pixel) << 2),
                                               (GX_COLOR)(BLUEVAL_16BPP(pixel) << 3));

                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            *put = pixel_24;
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
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel_24, brush_alpha);
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

                            pixel = *get;
                            pixel = (USHORT)((*(get + 1)) << 8) | pixel;

                            *put = ASSEMBLECOLOR_32BPP((GX_COLOR)(REDVAL_16BPP(pixel) << 3),
                                                       (GX_COLOR)(GREENVAL_16BPP(pixel) << 2),
                                                       (GX_COLOR)(BLUEVAL_16BPP(pixel) << 3));
                        }
                        put++;
                        get += 2;
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
                            pixel = (USHORT)((*(get + 1)) << 8) | pixel;
                            pixel_24 = ASSEMBLECOLOR_32BPP((GX_COLOR)(REDVAL_16BPP(pixel) << 3),
                                                           (GX_COLOR)(GREENVAL_16BPP(pixel) << 2),
                                                           (GX_COLOR)(BLUEVAL_16BPP(pixel) << 3));
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel_24, brush_alpha);
                        }
                        get += 2;
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
/*    _gx_dave2d_s_display_driver_24xrgb_pixelmap_draw   PORTABLE C       */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32xrgb format screen driver pixelmap drawing function that handles  */
/*    compressed or uncompress, with alpha channel.                       */
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
/*     _gx_dave2d_display_driver_24xrgb_pixelmap_compressed_alpha_write   */
/*     _gx_display_driver_32bpp_4444argb_dave2d_pixelmap_compressed       */
/*     _alpha_write                                                       */
/*     _gx_display_driver_32bpp_4444bgra_dave2d_pixelmap_compressed       */
/*     _alpha_write                                                       */
/*     _gx_display_driver_24xrgb_pixelmap_draw                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*  10-31-2022     Kenneth Maxwell          Modify comments, change       */
/*                                            function name,              */
/*                                            resulting in Version 6.2.0  */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_BOOL  synergy_specific_format = GX_FALSE;
GX_BOOL  drawn = GX_FALSE;
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TARGA  &&
        pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        synergy_specific_format = GX_TRUE;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* has both compression and alpha */
            _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with alpha */
            _gx_dave2d_simulation_display_driver_32bpp_4444argb_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    case GX_COLOR_FORMAT_4444BGRA:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with alpha */
            _gx_dave2d_simulation_display_driver_32bpp_4444bgra_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    case GX_COLOR_FORMAT_565RGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed without alpha */
            _gx_dave2d_simulation_display_driver_24xrgb_565rgb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    default:
        break;
    }

    if (!synergy_specific_format && !drawn)
    {
        _gx_display_driver_24xrgb_pixelmap_draw(context, xpos, ypos, pixelmap);
    }

    return;
}

