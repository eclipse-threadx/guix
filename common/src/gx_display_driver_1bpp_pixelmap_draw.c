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
/*    _gx_display_driver_1bpp_pixelmap_raw_write          PORTABLE C      */
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
/*    _gx_display_driver_1bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_1bpp_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                       INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             xval;
INT             yval;
INT             width;
GX_UBYTE       *putrow;
GX_UBYTE       *getrow;
GX_UBYTE       *put;
GX_CONST GX_UBYTE *get;
GX_UBYTE        putmask;
GX_UBYTE        getmask;
INT             putstride;
INT             getstride;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;


    putstride = (context -> gx_draw_context_pitch + 7) >> 3;
    getstride = (pixelmap -> gx_pixelmap_width + 7) >> 3;

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += (clip -> gx_rectangle_top * putstride);
    putrow += (clip -> gx_rectangle_left >> 3);

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += (clip -> gx_rectangle_top - ypos) * getstride;
    getrow += ((clip -> gx_rectangle_left - xpos) >> 3);

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        putmask = (GX_UBYTE)(0x80 >> (clip -> gx_rectangle_left & 0x07));
        getmask = (GX_UBYTE)(0x80 >> ((clip -> gx_rectangle_left - xpos) & 0x07));

        for (xval = 0; xval < width; xval++)
        {
            if ((*get) & getmask)
            {
                *put |= putmask;
            }
            else
            {
                *put = (GX_UBYTE)(*put & (~putmask));
            }

            getmask >>= 1;
            putmask >>= 1;

            if (!getmask)
            {
                getmask = 0x80;
                get++;
            }

            if (!putmask)
            {
                putmask = 0x80;
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
/*    _gx_display_driver_1bpp_pixelmap_transparent_write  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
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
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_1bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_1bpp_pixelmap_transparent_write(GX_DRAW_CONTEXT *context,
                                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             xval;
INT             yval;
INT             width;
GX_UBYTE       *putrow;
GX_UBYTE       *getrow;
GX_UBYTE       *put;
GX_CONST GX_UBYTE *get;
GX_UBYTE        putmask;
GX_UBYTE        transmask;
GX_UBYTE        getmask;
INT             putstride;
INT             getstride;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;


    putstride = (context -> gx_draw_context_pitch + 7) >> 3;
    getstride = (pixelmap -> gx_pixelmap_width + 3) >> 2;

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += (clip -> gx_rectangle_top * putstride);
    putrow += (clip -> gx_rectangle_left >> 3);

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += (clip -> gx_rectangle_top - ypos) * getstride;
    getrow += ((clip -> gx_rectangle_left - xpos) >> 2);

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        putmask = (GX_UBYTE)(0x80 >> (clip -> gx_rectangle_left & 0x07));
        getmask = (GX_UBYTE)(0x80 >> (((clip -> gx_rectangle_left - xpos) & 0x03) << 1));
        transmask = getmask >> 1;

        for (xval = 0; xval < width; xval++)
        {
            /* 2 bits for one pixel, first bit is color value,
               the second bit is transparent. */
            if (((*get) & transmask) != 0)
            {
                if ((*get) & getmask)
                {
                    *put |= putmask;
                }
                else
                {
                    *put = (GX_UBYTE)(*put & (~putmask));
                }
            }

            getmask >>= 2;
            transmask >>= 2;
            putmask >>= 1;

            if (!getmask)
            {
                getmask = 0x80;
                transmask = 0x40;
                get++;
            }

            if (!putmask)
            {
                putmask = 0x80;
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
/*    _gx_display_driver_1bpp_pixelmap_compressed_write   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap without transparent file.                                  */
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
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_1bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_1bpp_pixelmap_compressed_write(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT               xval;
INT               yval;
GX_UBYTE         *putrow;
GX_UBYTE          putmask;
GX_UBYTE         *put;
const GX_UBYTE   *get;
INT               putstride;
GX_UBYTE          count;
GX_UBYTE          pixel;
GX_RECTANGLE     *clip = context -> gx_draw_context_clip; 

    get = (const GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip->gx_rectangle_top; yval++)
    {
        xval = xpos;
        while (xval < xpos + pixelmap -> gx_pixelmap_width)
        {
            count = *get;
            if (count & 0x80)
            {
                count = (GX_UBYTE)(((count & 0x7f)>> 1) + 1);
                get++;
            }
            else
            {
                count = (GX_UBYTE)((count >> 1) + 1);
                get += count;
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
    to the end of the last visible row */
    putstride = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution + 7 )>> 3;
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip->gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 3;
    count = 0;

    while (yval <= clip->gx_rectangle_bottom)
    {
        xval = xpos;
        put = putrow;
        putmask = (GX_UBYTE)(0x80 >> (clip -> gx_rectangle_left & 0x07));

        while (xval < (xpos + pixelmap->gx_pixelmap_width))
        {
            count = *get;
            if (count & 0x80)
            {
                count = (GX_UBYTE)(((count & 0x7f) >> 1) + 1);
                pixel = *get++;
                while (count--)
                {
                    if (xval >= clip->gx_rectangle_left &&
                        xval <= clip->gx_rectangle_right)
                    {
                        if (pixel & 0x01)
                        {
                            *put |= putmask;
                        }
                        else
                        {
                            *put &= (GX_UBYTE)(~putmask);
                        }
                        putmask >>= 1;
                        if (putmask == 0)
                        {
                            put++;
                            putmask = 0x80;
                        }
                    }
                    xval++;
                }
            }
            else
            {
                count = (GX_UBYTE)((count >> 1) + 1);
                while (count--)
                {
                    pixel = *get++;
                    if (xval >= clip->gx_rectangle_left &&
                        xval <= clip->gx_rectangle_right)
                    {
                        if (pixel & 0x01)
                        {
                            *put |= putmask;
                        }
                        else
                        {
                            *put &= (GX_UBYTE)(~putmask);
                        }

                        putmask >>= 1;
                        if (putmask == 0)
                        {
                            put++;
                            putmask = 0x80;
                        }
                    }
                    xval++;
                }
            }
        }
        yval++;
        putrow += putstride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1bpp_pixelmap_c_t_write          PORTABLE C      */
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
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_1bpp_pixelmap_draw                               */
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
static VOID _gx_display_driver_1bpp_pixelmap_compressed_transparent_write(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT               xval;
INT               yval;
GX_UBYTE         *putrow;
GX_UBYTE          putmask;
GX_UBYTE         *put;
const GX_UBYTE   *get;
INT               putstride;
GX_UBYTE          count;
GX_UBYTE          pixel;
GX_RECTANGLE     *clip = context->gx_draw_context_clip;

    get = (const GX_UBYTE *)(pixelmap->gx_pixelmap_data);
    /* first, skip to the starting row */
    for (yval = ypos; yval < clip->gx_rectangle_top; yval++)
    {
        xval = xpos;
        while ( xval < xpos + pixelmap->gx_pixelmap_width)
        {
            count = *get;
            if (count & 0x80)
            {
                count = (GX_UBYTE)(((count & 0x7f) >> 2) + 1);
                get++;
            }
            else
            {
                count = (GX_UBYTE)((count >> 2) + 1);
                get += count;
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
    to the end of the last visible row */
    putstride = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution + 7) >> 3;
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * putstride;
    putrow += clip -> gx_rectangle_left >> 3;
    count = 0;

    while (yval <= clip->gx_rectangle_bottom)
    {
        xval = xpos;
        put = putrow;
        putmask = (GX_UBYTE)(0x80 >> (clip->gx_rectangle_left & 0x07));

        while (xval < (xpos + pixelmap->gx_pixelmap_width))
        {
            count = *get;
            if (count & 0x80)
            {
                count = (GX_UBYTE)(((count & 0x7f) >> 2) + 1);
                pixel = *get++;
                while (count--)
                {
                    /* if(pixel & 0x01) means un-transparent. */
                    if (pixel & 0x01)
                    {
                        if (xval >= clip->gx_rectangle_left &&
                            xval <= clip->gx_rectangle_right)
                        {
                            if (pixel & 0x02)
                            {
                                *put |= putmask;
                            }
                            else
                            {
                                *put &= (GX_UBYTE)(~putmask);
                            }
                            putmask >>= 1;
                            if (putmask == 0)
                            {
                                put++;
                                putmask = 0x80;
                            }
                        }
                    }
                    else
                    {
                        if (xval >= clip->gx_rectangle_left &&
                            xval <= clip->gx_rectangle_right)
                        {
                            putmask >>= 1;
                            if (putmask == 0)
                            {
                                put++;
                                putmask = 0x80;
                            }
                        }
                    }
                    xval++;
                }
            }
            else
            {
                count = (GX_UBYTE)((count >> 2) + 1);
                while (count--)
                {
                    pixel = *get++;

                    /* if(pixel & 0x01) means un-transparent. */
                    if (pixel & 0x01)
                    {
                        if (xval >= clip->gx_rectangle_left &&
                            xval <= clip->gx_rectangle_right)
                        {
                            if (pixel & 0x02)
                            {
                                *put |= putmask;
                            }
                            else
                            {
                                *put &= (GX_UBYTE)(~putmask);
                            }
                            putmask >>= 1;
                            if (putmask == 0)
                            {
                                put++;
                                putmask = 0x80;
                            }
                        }
                    }
                    else
                    {
                        if (xval >= clip->gx_rectangle_left &&
                            xval <= clip->gx_rectangle_right)
                        {
                            putmask >>= 1;
                            if (putmask == 0)
                            {
                                put++;
                                putmask = 0x80;
                            }
                        }
                    }
                    xval++;
                }
            }
        }
        yval++;
        putrow += putstride;
    }
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1bit_pixelmap_draw               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    8bit screen driver pixelmap drawing function that handles           */
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
/*     _gx_display_driver_1bpp_pixelmap_compressed_transparent_write      */
/*     _gx_display_driver_1bpp_pixelmap_transparent_write                 */
/*     _gx_display_driver_1bpp_pixelmap_compressed_write                  */
/*     _gx_display_driver_1bpp_pixelmap_raw_write                         */
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
VOID _gx_display_driver_1bpp_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{

    if (pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_MONOCHROME)
    {
        /* wrong color format for this driver */
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            _gx_display_driver_1bpp_pixelmap_compressed_transparent_write(context,
                                                                          xpos, ypos, pixelmap);
        }
        else
        {
            _gx_display_driver_1bpp_pixelmap_transparent_write(context, xpos, ypos, pixelmap);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            _gx_display_driver_1bpp_pixelmap_compressed_write(context,
                                                              xpos, ypos, pixelmap);
        }
        else
        {
            /* no compression or alpha */
            _gx_display_driver_1bpp_pixelmap_raw_write(context,
                                                       xpos, ypos, pixelmap);
        }
    }
}

