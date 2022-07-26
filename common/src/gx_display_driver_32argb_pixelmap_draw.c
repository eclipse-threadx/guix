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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32xrgb_pixelmap_alpha_write      PORTABLE C      */
/*                                                           6.1.7        */
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
/*     _gx_display_driver_32argb_pixel_blend                              */
/*                                          Display driver basic pixel    */
/*                                            blend function              */
/*     _gx_display_driver_32bpp_pixel_write                               */
/*                                          Display driver basic pixel    */
/*                                            write function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32argb_pixelmap_draw                             */
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
static VOID _gx_display_driver_32argb_pixelmap_alpha_write(GX_DRAW_CONTEXT *context,
                                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           xval;
INT           yval;
GX_COLOR      color;
INT           width;
ULONG        *get;
UCHAR         alpha_value;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    get = (ULONG *)((ULONG)pixelmap -> gx_pixelmap_data + (sizeof(GX_COLOR) * (UINT)(pixelmap -> gx_pixelmap_width) * (UINT)(((INT)(clip -> gx_rectangle_top) - ypos))));
    get += (clip -> gx_rectangle_left - xpos);

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            color = *get;
            alpha_value = (UCHAR)(color >> 24);
            if (alpha_value == 255)
            {
                _gx_display_driver_32bpp_pixel_write(context, xval, yval, color);
            }
            else
            {
                _gx_display_driver_32argb_pixel_blend(context, xval, yval, color, 0xff);
            }
            get++;
        }
        get += pixelmap -> gx_pixelmap_width - width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32xrgb_pixelmap_compressed_alpha_write           */
/*                                                        PORTABLE C      */
/*                                                           6.1.12       */
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
/*     _gx_display_driver_32argb_pixel_blend                              */
/*                                          Display driver basic pixel    */
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
/*  07-29-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1.12 */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32argb_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                      INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_COLOR *get;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_CONST GX_UBYTE *get_count;
GX_UBYTE           brush_alpha;
GX_UBYTE           alpha;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    get = (GX_CONST GX_COLOR *)pixelmap -> gx_pixelmap_data;
    get_count = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1u);
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
       to the end of the last visible row
     */

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (GX_UBYTE)((count & 0x7f) + 1u);
                alpha = (GX_UBYTE)((*get) >> 24);
                pixel = *get++;

                if (alpha)
                {
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_32argb_pixel_blend(context, xval, yval, pixel, brush_alpha);
                        }
                        xval++;
                    }
                }
                else
                {
                    xval += count;
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
                        pixel = *get;

                        _gx_display_driver_32argb_pixel_blend(context, xval, yval, pixel, brush_alpha);
                    }
                    get++;
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
/*    _gx_display_driver_32argb_pixelmap_compressed_write                 */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
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
/*     _gx_display_driver_32argb_pixel_blend                              */
/*                                          Display driver basic pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32argb_pixelmap_draw                             */
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
static VOID _gx_display_driver_32argb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_COLOR *get;
GX_COLOR          *put;
GX_COLOR          *putrow;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_CONST GX_UBYTE *get_count;
GX_UBYTE           brush_alpha;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    get = (GX_CONST GX_COLOR *)pixelmap -> gx_pixelmap_data;
    get_count = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1u);
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
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= clip -> gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (GX_UBYTE)((count & 0x7f) + 1u);
                pixel = *get++;

                if (brush_alpha == 0xff)
                {
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
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_32argb_pixel_blend(context, xval, yval, pixel, brush_alpha);
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
                            *put = *get;
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
                            _gx_display_driver_32argb_pixel_blend(context, xval, yval, *get, brush_alpha);
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
/*    _gx_display_driver_32argb_pixelmap_raw_write        PORTABLE C      */
/*                                                           6.1          */
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
/*    _gx_display_driver_32argb_pixelmap_draw                             */
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
static VOID _gx_display_driver_32argb_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                         INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           xval;
INT           yval;
INT           width;
GX_COLOR     *putrow;
GX_COLOR     *getrow;
GX_COLOR     *put;
GX_COLOR     *get;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    getrow = (GX_COLOR *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    /* brush alpha is 0xff means draw pixelmap to memory directly. */
    putrow = context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += clip -> gx_rectangle_left;

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
/*    _gx_display_driver_32xrgb_pixelmap_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32xrgb format screen driver pixelmap drawing function that handles  */
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
/*     _gx_display_driver_32argb_pixelmap_compressed_alpha_write          */
/*                                          Real Pixelmap draw function   */
/*     _gx_display_driver_32argb_pixelmap_alpha_write                     */
/*                                          Real Pixelmap draw function   */
/*     _gx_display_driver_32argb_pixelmap_compressed_write                */
/*                                          Real Pixelmap draw function   */
/*     _gx_display_driver_32argb_pixelmap_raw_write                       */
/*                                          Real Pixelmap draw function   */
/*     _gx_display_driver_32argb_pixelmap_blend                           */
/*                                          Basic display driver function */
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
VOID _gx_display_driver_32argb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                             INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_BOOL  drawn = GX_FALSE;
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        /* Draw nothing here. Just return. */
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        /* Not supported yet. */
        return;
    }

    if (pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_32ARGB)
    {
        /* wrong color format for this driver */
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* has both compression and alpha */
            _gx_display_driver_32argb_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        else
        {
            if (brush_alpha == 0xff)
            {
                /* alpha, no compression */
                _gx_display_driver_32argb_pixelmap_alpha_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no alpha */
            _gx_display_driver_32argb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        else
        {
            /* no compression or alpha */
            if (brush_alpha == 0xff)
            {
                _gx_display_driver_32argb_pixelmap_raw_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
        }
    }

    if (!drawn)
    {
        _gx_display_driver_32argb_pixelmap_blend(context, xpos, ypos, pixelmap, brush_alpha);
    }
}

