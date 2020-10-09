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
/*    _gx_display_driver_1bpp_horizontal_pixelmap_line_raw_write          */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file without transparency.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
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
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_1bpp_horizontal_pixelmap_line_raw_write(GX_DRAW_CONTEXT *context,
                                                                       INT xstart, INT xend, INT y,
                                                                       GX_FILL_PIXELMAP_INFO *info)
{
INT             xval;
const GX_UBYTE *get;
GX_UBYTE       *put;
GX_UBYTE        getmask;
GX_UBYTE        putmask;
INT             getstride;
INT             putstride;
INT             pic_width;
INT             offset;
GX_UBYTE        pixel;
GX_PIXELMAP    *pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;
    get = info -> current_pixel_ptr;
    getstride = (pic_width + 7) >> 3;

    if ((info -> draw) && (xstart <= xend))
    {
        putstride = (context -> gx_draw_context_pitch + 7) >> 3;
        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * putstride;
        put += xstart >> 3;

        putmask = (GX_UBYTE)(0x80 >> (xstart & 0x07));

        /*calculate the offset.*/
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            pixel = *(get + (offset >> 3));
            getmask = (GX_UBYTE)(0x80 >> (offset & 0x07));
            if (pixel & getmask)
            {
                *put |= putmask;
            }
            else
            {
                *put &= (GX_UBYTE)(~putmask);
            }
            offset++;

            if (offset >= pic_width)
            {
                offset -= pic_width;
            }

            putmask >>= 1;
            if (putmask == 0)
            {
                put++;
                putmask = 0x80;
            }
        }
    }

    /*This line is drawn. Update the pointer position for next row.*/
    info -> current_pixel_ptr += (UINT)getstride * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1bpp_horizontal_pixelmap_line_t_write            */
/*                                                        PORTABLE C      */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
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
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_1bpp_horizontal_pixelmap_line_transparent_write(GX_DRAW_CONTEXT *context,
                                                                               INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT             xval;
const GX_UBYTE *get;
GX_UBYTE       *put;
GX_UBYTE        putmask;
GX_UBYTE        getmask;
GX_UBYTE        pixel;
INT             getstride;
INT             putstride;
INT             offset;
INT             pic_width;
GX_PIXELMAP    *pixelmap = info -> pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;
    get = info -> current_pixel_ptr;
    getstride = (pic_width + 3) >> 2;

    if ((info -> draw) && (xstart <= xend))
    {
        putstride = (context -> gx_draw_context_pitch + 7) >> 3;
        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * putstride;
        put += xstart >> 3;

        putmask = (GX_UBYTE)(0x80 >> (xstart & 0x07));

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            getmask = (GX_UBYTE)(0x40 >> ((offset & 0x03) << 1));
            pixel = *(get + (offset >> 2));
            if (getmask & pixel)
            {
                /* If not transparent, draw it. */
                if ((getmask << 1) & pixel)
                {
                    *put |= putmask;
                }
                else
                {
                    *put &= (GX_UBYTE)(~putmask);
                }
            }

            offset++;
            if (offset >= pic_width)
            {
                offset = 0;
            }

            putmask >>= 1;
            if (putmask == 0)
            {
                putmask = 0x80;
                put++;
            }
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)getstride * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1bpp_horizontal_pixelmap_line_compressed_write   */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file without transparent.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
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
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_1bpp_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                              INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_UBYTE           pixel;
GX_UBYTE           putmask;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calcualte draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * ((context -> gx_draw_context_pitch + 7) >> 3) + (xstart >> 3);

        putmask = (GX_UBYTE)(0x80 >> (xstart & 0x07));

        /* Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)(((count & 0x7f) >> 1) + 1);
                    pixel = *get++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
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
                        if (xval >= xstart && xval <= xend)
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
            start_pos += pixelmap -> gx_pixelmap_width;
        }
    }
    else
    {
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get;
            if (count & 0x80)
            {
                count = (GX_UBYTE)(((count & 0x7f) >> 1) + 1);
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

    /* This line is drawn. cache the pointer for next line draw. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*  _gx_display_driver_1bpp_horizontal_pixelmap_line_c_t_write            */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
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
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_1bpp_horizontal_pixelmap_line_compressed_transparent_write(GX_DRAW_CONTEXT *context,
                                                                                          INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_UBYTE           pixel;
GX_UBYTE           putmask;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * ((context -> gx_draw_context_pitch + 7) >> 3) + (xstart >> 3);

        putmask = (GX_UBYTE)(0x80 >> (xstart & 0x07));

        /*Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;
            /*Start from where we need to repeat.*/
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)(((count & 0x7f) >> 2) + 1);
                    pixel = *get++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            if (pixel & 0x01)
                            {
                                if (pixel & 0x02)
                                {
                                    *put |= putmask;
                                }
                                else
                                {
                                    *put &= (GX_UBYTE)(~putmask);
                                }
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
                    count = (GX_UBYTE)((count >> 2) + 1);
                    while (count--)
                    {
                        pixel = *get++;
                        if (xval >= xstart && xval <= xend)
                        {
                            if (pixel & 0x01)
                            {
                                if (pixel & 0x02)
                                {
                                    *put |= putmask;
                                }
                                else
                                {
                                    *put &= (GX_UBYTE)(~putmask);
                                }
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
            start_pos += pixelmap -> gx_pixelmap_width;
        }
    }
    else
    {
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
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

    /* This line is drawn. cache the pointer for next line draw. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1bpp_horizontal_pixelmap_line_draw               */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1bpp screen driver horizontal pixelmap line drawing function that   */
/*    handles compressed or uncompress, with or without alpha channel.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_1bpp_horizontal_pixelmap_line_c_t_write         */
/*                                          Real pixelmap line draw       */
/*                                            function                    */
/*     _gx_display_driver_1bpp_horizontal_pixelmap_line_t_write           */
/*                                          Real pixelmap line draw       */
/*                                            function                    */
/*     _gx_display_driver_1bpp_horizontal_pixelmap_line_compressed_write  */
/*                                          Real pixelmap line draw       */
/*                                            function                    */
/*     _gx_display_driver_1bpp_horizontal_pixelmap_line_raw_write         */
/*                                          Real pixelmap line draw       */
/*                                            function                    */
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
VOID _gx_display_driver_1bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
GX_PIXELMAP *pixelmap = info -> pixelmap;

    if (pixelmap == GX_NULL)
    {
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* has both compression and alpha */
            _gx_display_driver_1bpp_horizontal_pixelmap_line_compressed_transparent_write(context, xstart, xend, y, info);
        }
        else
        {
            /* alpha, no compression */
            _gx_display_driver_1bpp_horizontal_pixelmap_line_transparent_write(context, xstart, xend, y, info);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no alpha */
            _gx_display_driver_1bpp_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        }
        else
        {
            /* no compression or alpha */
            _gx_display_driver_1bpp_horizontal_pixelmap_line_raw_write(context, xstart, xend, y, info);
        }
    }

    /* Current pixelmap has gone over, so the offset pointer should be reset. */
    if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
    {
        info -> current_pixel_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_data;
        info -> current_aux_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_aux_data;
    }
}

