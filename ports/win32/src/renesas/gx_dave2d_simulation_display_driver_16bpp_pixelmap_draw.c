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
#include "gx_context.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_565rgb_pixelmap_c_write        */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
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
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_dave2d_simulation_display_driver_565rgb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                   INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
USHORT         *put;
USHORT         *putrow;
GX_UBYTE        count;
USHORT          pixel;
GX_UBYTE        alpha;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;

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
                count = (GX_UBYTE)((count & 0x7f) + 1);
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
    putrow = (USHORT *)context -> gx_draw_context_memory;
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
                count = (GX_UBYTE)((count & 0x7f) + 1);
                pixel = *get++;
                pixel = (USHORT)((*get++) << 8) | pixel;

                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        if (alpha == 0xff)
                        {
                            *put = pixel;
                        }
                        else
                        {
                            _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha);
                        }
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
                        pixel = (USHORT)((*get) | (USHORT)((*(get + 1)) << 8));
                        if (alpha == 0xff)
                        {
                            *put = pixel;
                        }
                        else
                        {
                            _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha);
                        }
                    }
                    put++;
                    get += 2;
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
/*    _gx_dave2d_simulation_display_driver_16bpp_4444argb_pixelmap_       */
/*    c_alpha_write                                       PORTABLE C      */
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
static VOID _gx_dave2d_simulation_display_driver_16bpp_4444argb_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
USHORT          pixel;
UCHAR           alpha_value;
UCHAR           temp;
UCHAR           brush_alpha;
int             temp_alpha;


GX_RECTANGLE *clip = context -> gx_draw_context_clip;

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
                /*first byte , 0xf0 --> g , 0x0f --> b */
                alpha_value = (*(get + 1)) & 0xf0;
                temp_alpha = alpha_value * brush_alpha;
                temp_alpha /= 255;
                alpha_value = (UCHAR)temp_alpha;

                if (alpha_value)
                {
                    temp  = (*get) & 0xf0;
                    pixel = (USHORT)(temp << 3) | pixel;
                    temp = (*get++) & 0x0f;
                    pixel = (USHORT)(temp << 1) | pixel;
                    /*second byte , 0xf0 --> a , 0x0f --> r */
                    temp  = (*get++) & 0x0f;
                    pixel = (USHORT)(temp << 12) | pixel;
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            if (alpha_value == 0xf0)
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

                        temp_alpha = alpha_value * brush_alpha;
                        temp_alpha /= 255;
                        alpha_value = (UCHAR)temp_alpha;

                        pixel = 0;
                        if (alpha_value)
                        {
                            /*first byte , 0xf0 --> g , 0x0f --> b */

                            temp  = (*get) & 0xf0;
                            pixel = (USHORT)(temp << 3) | pixel;
                            temp  = (*get) & 0x0f;
                            pixel = (USHORT)(temp << 1) | pixel;
                            /*second byte , 0xf0 --> a , 0x0f --> r */
                            temp  = (*(get + 1)) & 0x0f;
                            pixel = (USHORT)(temp << 12) | pixel;
                            if (alpha_value == 0xf0)
                            {
                                _gx_display_driver_16bpp_pixel_write(context, xval, yval, pixel);
                            }
                            else
                            {
                                _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                            }
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
/*    _gx_dave2d_simulation_display_driver_16bpp_4444bgra_pixelmap_       */
/*    compressed_alpha_write                              PORTABLE C      */
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
/*        _gx_display_driver_16bpp_pixel_write                            */
/*        _gx_display_driver_565rgb_pixel_blend                           */
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
static VOID _gx_dave2d_simulation_display_driver_16bpp_4444bgra_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
USHORT          pixel;
GX_UBYTE        alpha_value;
GX_UBYTE        temp;
int             temp_alpha;
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
                alpha_value = (UCHAR)(temp << 4);

                temp_alpha = alpha_value * brush_alpha;
                temp_alpha /= 255;
                alpha_value = (GX_UBYTE)temp_alpha;

                if (alpha_value)
                {
                    temp  = (*get++) & 0xf0;
                    pixel = (USHORT)(temp << 8) | pixel;
                    /*second byte , 0xf0 --> b , 0x0f --> g */
                    temp  = (*get) & 0xf0;
                    pixel = (USHORT)((temp >> 3) | pixel);
                    temp  = (*get++) & 0x0f;
                    pixel = (USHORT)(temp << 7) | pixel;

                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            if (alpha_value == 0xf0)
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
                        alpha_value = (UCHAR)(temp << 4);
                        temp_alpha = alpha_value * brush_alpha;
                        temp_alpha /= 255;
                        alpha_value = (GX_UBYTE)temp_alpha;

                        if (alpha_value)
                        {
                            /*first byte , 0xf0 --> g , 0x0f --> b */
                            temp  = (*get) & 0xf0;
                            pixel = (USHORT)(temp << 8) | pixel;
                            /*second byte , 0xf0 --> a , 0x0f --> r */
                            temp  = (*(get + 1)) & 0xf0;
                            pixel = (USHORT)((temp >> 3) | pixel);
                            temp  = (*(get + 1)) & 0x0f;
                            pixel = (USHORT)(temp << 7) | pixel;

                            if (alpha_value == 0xf0)
                            {
                                _gx_display_driver_16bpp_pixel_write(context, xval, yval, pixel);
                            }
                            else
                            {
                                _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                            }
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
/*    _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_         */
/*    compressed_write                                                    */
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
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_565rgb_pixel_blend                              */
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
static VOID _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                         INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
USHORT          pixel;
GX_UBYTE        alpha_value;
GX_UBYTE        brush_alpha;
int             temp_alpha;
GX_BOOL         has_alpha;
USHORT          red;
USHORT          green;
USHORT          blue;

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
                blue = (USHORT)((*get++) & 0xf8);
                pixel = blue >> 3;
                green = (USHORT)((*get++) & 0xfc);
                pixel = (USHORT)(green << 3) | pixel;
                red = (USHORT)((*get++) & 0xf8);
                pixel = (USHORT)(red << 8) | pixel;

                if (has_alpha)
                {
                    alpha_value = *get++;
                }
                else
                {
                    get += 1;
                    alpha_value = 0xff;
                }
                temp_alpha = alpha_value * brush_alpha;
                temp_alpha /= 255;
                alpha_value = (GX_UBYTE)temp_alpha;

                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
                    }
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
                        red = (USHORT)(*(get + 2) & 0xf8);
                        green = (USHORT)(*(get + 1) & 0xfc);
                        blue = (USHORT)(*(get) & 0xf8);
                        pixel = (USHORT)((blue >> 3) | (green << 3) | (red << 8));

                        if (has_alpha)
                        {
                            alpha_value = *(get + 3);
                        }
                        else
                        {
                            alpha_value = 0xff;
                        }
                        temp_alpha = alpha_value * brush_alpha;
                        temp_alpha /= 255;
                        alpha_value = (GX_UBYTE)temp_alpha;
                        _gx_display_driver_565rgb_pixel_blend(context, xval, yval, pixel, alpha_value);
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
/*    _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap          */
/*    _raw_write                                                          */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
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
/*     _gx_display_driver_565rgb_pixel_blend                              */
/*     _gx_display_driver_16bpp_pixel_write                               */
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
static VOID _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                                                  INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                skipcount;
INT                xval;
INT                yval;
GX_COLOR          *getrow;
GX_CONST GX_COLOR *get;
GX_DISPLAY        *display;
GX_RECTANGLE      *clip;
VOID               (*blend_function)(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR fcolor, GX_UBYTE alpha);
VOID               (*write_function)(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
GX_UBYTE           alpha;
GX_UBYTE           brush_alpha;
int                temp_alpha;
USHORT             color;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;


    display = context -> gx_draw_context_display;
    blend_function = display -> gx_display_driver_pixel_blend;
    write_function = display -> gx_display_driver_pixel_write;

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    clip = context -> gx_draw_context_clip;
    skipcount = (pixelmap -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    skipcount += (clip -> gx_rectangle_left - xpos);

    getrow = (GX_COLOR *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            alpha = (GX_UBYTE)(((*get) & 0xff000000) >> 24);
            temp_alpha = alpha * brush_alpha;
            temp_alpha /= 255;
            alpha = (GX_UBYTE)temp_alpha;

            r = (GX_UBYTE)((REDVAL_24BPP(*get) & 0xf8) >> 3);
            g = (GX_UBYTE)((GREENVAL_24BPP(*get) & 0xfc) >> 2);
            b = (GX_UBYTE)((BLUEVAL_24BPP(*get) & 0xf8) >> 3);
            color = (USHORT)ASSEMBLECOLOR_16BPP((INT)r, (INT)g, (INT)b);
            if (alpha == 0xff)
            {
                write_function(context, xval, yval, color);
            }
            else
            {
                blend_function(context, xval, yval, color, alpha);
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
/*    _gx_dave2d_simulation_display_driver_565rgb_pixelmap_blend          */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb , 4444argb and 4444bgra screen driver pixelmap drawing       */
/*    function that handles compressed or uncompress, without alpha       */
/*    channel.                                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    alpha                                 Blending alpha                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_r_write */
/*     _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_c_write */
/*     _gx_dave2d_simulation_display_driver_16bpp_4444argb_pixelmap       */
/*         _compressed_alpha_write                                        */
/*     _gx_dave2d_simulation_display_driver_565rgb_pixelmap_c_write       */
/*     _gx_display_driver_565rgb_pixelmap_draw                            */
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
VOID _gx_dave2d_simulation_display_driver_565rgb_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
GX_BOOL synergy_specific_format = GX_FALSE;
GX_BOOL drawn = GX_FALSE;

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TARGA &&
        pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        synergy_specific_format = GX_TRUE;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no alpha */
            _gx_dave2d_simulation_display_driver_565rgb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with alpha */
            _gx_dave2d_simulation_display_driver_16bpp_4444argb_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    case GX_COLOR_FORMAT_4444BGRA:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with alpha */
            _gx_dave2d_simulation_display_driver_16bpp_4444bgra_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with alpha */
            _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        else
        {
            _gx_dave2d_simulation_display_driver_16bpp_32argb_pixelmap_raw_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        break;

    default:
        break;
    }

    if (!synergy_specific_format && !drawn)
    {
        if (alpha == 0xff)
        {
            _gx_display_driver_565rgb_pixelmap_draw(context, xpos, ypos, pixelmap);
        }
        else
        {
            _gx_display_driver_565rgb_pixelmap_blend(context, xpos, ypos, pixelmap, alpha);
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_16bpp_pixelmap_draw            */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb , 4444argb and 4444bgra screen driver pixelmap drawing       */
/*    function that handles compressed or uncompress, without alpha       */
/*    channel.                                                            */
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
/*     _gx_dave2d_simulation_display_driver_565rgb_pixelmap_blend         */
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
VOID _gx_dave2d_simulation_display_driver_16bpp_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
    _gx_dave2d_simulation_display_driver_565rgb_pixelmap_blend(context, xpos, ypos, pixelmap, context -> gx_draw_context_brush.gx_brush_alpha);
}

