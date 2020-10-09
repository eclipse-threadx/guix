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
/*    _gx_display_driver_4bpp_pixelmap_raw_write          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file without transparent.                                  */
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
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_4bpp_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                       INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                       xval;
INT                       yval;
INT                       width;
GX_UBYTE                 *putrow;
GX_CONST GX_UBYTE        *getrow;
GX_UBYTE                 *put;
GX_UBYTE                  putmask;
INT                       putstride;
INT                       getstride;
INT                       offset;
INT                       offset_row;
GX_UBYTE                  pixel;
GX_RECTANGLE             *clip = context -> gx_draw_context_clip;

    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    getstride = (pixelmap -> gx_pixelmap_width + 1) >> 1;

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 1;

    offset_row = clip->gx_rectangle_left - xpos;
    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += getstride * (clip -> gx_rectangle_top - ypos);
    getrow += offset_row >> 1;

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        offset = offset_row;

        if (clip -> gx_rectangle_left & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }

        for (xval = 0; xval < width; xval++)
        {
            pixel = *(getrow + (offset >> 1));
            if (offset & 1)
            {
                pixel &= 0x0f;
            }
            else
            {
                pixel >>= 4;
            }
            pixel |= (GX_UBYTE)(pixel << 4);

            *put &= (GX_UBYTE)(~putmask);
            *put |= pixel & putmask;

            offset++;
            putmask >>= 4;
            if (putmask == 0)
            {
                putmask = 0xf0;
                put++;
            }

        }
        putrow += putstride;
        getrow += getstride;
    }
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_transparent_write  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file with transparency.                                    */
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
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_4bpp_pixelmap_transparent_write(GX_DRAW_CONTEXT *context,
                                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
const GX_UBYTE *getrow;
const GX_UBYTE *getauxrow;
GX_UBYTE       *put;
GX_UBYTE       *putrow;
GX_UBYTE        putmask;
GX_UBYTE        transmask;
INT             putstride;
INT             getstride;
INT             getauxstride;
GX_VALUE        offset;
GX_VALUE        offset_row;
GX_UBYTE        pixel;
GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    
    offset_row = (GX_VALUE)(clip -> gx_rectangle_left - xpos);
    getstride = (pixelmap -> gx_pixelmap_width + 1) >> 1;
    getrow = (const GX_UBYTE *)pixelmap -> gx_pixelmap_data;
    getrow += getstride * (clip -> gx_rectangle_top - ypos);

    getauxstride = (pixelmap -> gx_pixelmap_width + 7) >> 3;
    getauxrow = (const GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
    getauxrow += getauxstride * (clip -> gx_rectangle_top - ypos);

    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += (clip -> gx_rectangle_top * putstride);
    putrow += (clip -> gx_rectangle_left >> 1);

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        offset = offset_row;
        put = putrow;
        if (clip->gx_rectangle_left & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }
        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            transmask = (GX_UBYTE)(0x80 >> (offset & 0x07));
            if ((*(getauxrow + (offset >> 3)) & transmask) == 0)
            {
                pixel = *(getrow + (offset >> 1));
                if (offset & 1)
                {
                    pixel &= 0x0f;
                    pixel |= (GX_UBYTE)(pixel << 4);
                }
                else
                {
                    pixel &= 0xf0;
                    pixel |= pixel >> 4;
                }
                *put &= (GX_UBYTE)(~putmask);
                *put |= pixel & putmask;
            }
            offset++;

            putmask >>= 4;
            if (putmask == 0)
            {
                putmask = 0xf0;
                put++;
            }
        }

        getrow += getstride;
        putrow += putstride;
        getauxrow += getauxstride;
    }


}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_compressed_write   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file.                                                      */
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
/*    _gx_display_driver_4bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_4bpp_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                              INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
const GX_UBYTE *get;
const GX_UBYTE *get_count;
GX_UBYTE       *put;
GX_UBYTE       *putrow;
GX_UBYTE        count;
GX_UBYTE        color;
GX_UBYTE        putmask;
GX_UBYTE        getmask;
INT             putstride;
GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    get = (const GX_UBYTE *)pixelmap -> gx_pixelmap_data;
    get_count = (const GX_UBYTE*)pixelmap -> gx_pixelmap_aux_data;
    putstride = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution + 1) >> 1;
    getmask = 0xf0;

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;
            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                getmask >>= 4;      /* skip repeated pixel value */
                if (getmask == 0)
                {
                    get++;
                    getmask = 0xf0;
                }
            }
            else
            {
                count++;
                get += count >> 1;
                if ((getmask == 0x0f) && (count & 1))
                {
                    get++;
                }

                getmask = (GX_UBYTE)( getmask >> (GX_UBYTE)((count & 1) << 2));
                if (getmask == 0)
                {
                    getmask = 0xf0;
                }
                /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
       to the end of the last visible row*/
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += yval * putstride;
    putrow += clip -> gx_rectangle_left >> 1;
    count = 0;

    while (yval <= clip -> gx_rectangle_bottom)
    {
        if (clip -> gx_rectangle_left & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }
        put = putrow;
        xval = xpos;

        while (xval < (xpos + pixelmap -> gx_pixelmap_width))
        {
            count = *get_count++;
            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                color = (GX_UBYTE)(*get & getmask);
                if (getmask == 0xf0)
                {
                    color |= color >> 4;
                }
                else
                {
                    color |= (GX_UBYTE)(color << 4);
                }
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        *put &= (GX_UBYTE)(~putmask);
                        *put |= (color & putmask);

                        putmask >>= 4;
                        if (putmask == 0)
                        {
                            putmask = 0xf0;
                            put++;
                        }
                    }
                    xval++;
                }
                getmask >>= 4;
                if (getmask == 0)
                {
                    getmask = 0xf0;
                    get++;
                }
            }
            else
            {
                count++;
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        color = (GX_UBYTE)(*get & getmask);
                        if (getmask == 0xf0)
                        {
                            color |= color >> 4;
                        }
                        else
                        {
                            color |= (GX_UBYTE)(color << 4);
                        }
                        *put &= (GX_UBYTE)(~putmask);
                        *put |= (color & putmask);

                        putmask >>= 4;
                        if (putmask == 0)
                        {
                            putmask = 0xf0;
                            put++;
                        }
                    }
                    
                    xval++;
                    getmask >>= 4;
                    if (getmask == 0)
                    {
                        getmask = 0xf0;
                        get++;
                    }
                }
            }
        }
        putrow += putstride;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_compressed_alpha_write             */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file with transparent.                                     */
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
/*    _gx_display_driver_4bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_4bpp_pixelmap_compressed_transparent_write(GX_DRAW_CONTEXT *context,
                                                                          INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                 xval;
INT                 yval;
const GX_UBYTE     *get;
const GX_UBYTE     *get_count;
GX_UBYTE            color;
GX_UBYTE            count;
GX_UBYTE           *put;
GX_UBYTE           *putrow;
GX_UBYTE            putmask;
INT                 putstride;
GX_RECTANGLE       *clip = context -> gx_draw_context_clip;

    get = (const GX_UBYTE *)pixelmap -> gx_pixelmap_data;
    get_count = (const GX_UBYTE*)pixelmap -> gx_pixelmap_aux_data;
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                get++;
            }
            else
            {
                count++;
                get += count;
            }
            xval += count;
        }
    }

    putstride = (context -> gx_draw_context_pitch + 1) >> 1;
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += (clip -> gx_rectangle_top * putstride);
    putrow += (xpos >> 1);

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;
        put = putrow;
        if (xpos & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }
        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;
            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                color = *get++;
                /* 0xff means transparent.*/
                if (color == 0xff)
                {
                    xval += count;
                    while (count --)
                    {
                        putmask >>= 4;
                        if (putmask == 0)
                        {
                            putmask = 0xf0;
                            put++;
                        }
                    }
                }
                else
                {
                    color |= (GX_UBYTE)(color << 4);
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            *put &= (GX_UBYTE)(~putmask);
                            *put |= (GX_UBYTE)(color & putmask);
                        }
                        xval++;
                        putmask >>= 4;

                        if (putmask == 0)
                        {
                            putmask = 0xf0;
                            put++;
                        }
                    }
                }
            }
            else
            {
                count++;
                while (count--)
                {
                    color = *get++;
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        /* 0xff means transparent color, so should skip */
                        if (color != 0xff)
                        {
                            color |= (GX_UBYTE)(color << 4);
                            *put &= (GX_UBYTE)(~putmask);
                            *put |= (GX_UBYTE)(color & putmask);
                        }
                    }

                    xval++;
                    putmask >>= 4;
                    if (putmask == 0)
                    {
                        putmask = 0xf0;
                        put++;
                    }
                }
            }
        }

        putrow += putstride;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixelmap_draw               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    4bit screen driver pixelmap drawing function that handles           */
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
/*     _gx_display_driver_4bpp_pixelmap_compressed_write                  */
/*                                          Real pixelmap draw routine    */
/*     _gx_display_driver_4bpp_pixelmap_compressed_transparent_write      */
/*                                          Real pixelmap draw routine    */
/*     _gx_display_driver_4bpp_pixelmap_raw_write                         */
/*                                          Real pixelmap draw routine    */
/*     _gx_display_driver_4bpp_pixelmap_transparent_write                 */
/*                                          Real pixelmap draw routine    */
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
VOID _gx_display_driver_4bpp_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{

    if (pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_4BIT_GRAY ||
        (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA))
    {
        /* wrong color format for this driver */
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        /* Pixelmap wtih transparent must be compressed. */
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            _gx_display_driver_4bpp_pixelmap_compressed_transparent_write(context, xpos, ypos, pixelmap);
        }
        else
        {
            _gx_display_driver_4bpp_pixelmap_transparent_write(context, xpos, ypos, pixelmap);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no transparency */
            _gx_display_driver_4bpp_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
        }
        else
        {
            /* no compression or transaprency */
            _gx_display_driver_4bpp_pixelmap_raw_write(context, xpos, ypos, pixelmap);
        }
    }
}

