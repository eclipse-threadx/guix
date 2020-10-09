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
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_raw_write          */
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
/*    info                                  Pointer to                    */
/*                                            GX_FILL_PIXELMAP_INFO struct*/
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
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_4bpp_horizontal_pixelmap_line_raw_write(GX_DRAW_CONTEXT *context,
                                                                       INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT             xval;
const GX_UBYTE *get;
GX_UBYTE       *put;
GX_UBYTE        pixel;
GX_UBYTE        putmask;
INT             getstride;
INT             putstride;
INT             pic_width;
INT             offset;
GX_PIXELMAP    *pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;
    get = info -> current_pixel_ptr;
    getstride = (pic_width + 1) >> 1;

    if ((info -> draw) && (xstart <= xend))
    {
        putstride = (context -> gx_draw_context_pitch + 1) >> 1;
        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * putstride;
        put += xstart >> 1;

        if (xstart & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            pixel = *(get + (offset >> 1));
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
            offset++;

            if (offset >= pic_width)
            {
                offset -= pic_width;
            }

            *put &= (GX_UBYTE)(~putmask);
            *put |= putmask & pixel;
            putmask >>= 4;
            if (putmask == 0)
            {
                put++;
                putmask = 0xf0;
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
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_t_write            */
/*                                                        PORTABLE C      */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  Pointer to                    */
/*                                            GX_FILL_PIXELMAP_INFO struct*/
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
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_4bpp_horizontal_pixelmap_line_transparent_write(GX_DRAW_CONTEXT *context,
                                                                               INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT             xval;
const GX_UBYTE *get;
const GX_UBYTE *getaux;
GX_UBYTE       *put;
GX_UBYTE        putmask;
GX_UBYTE        pixel;
GX_UBYTE        transmask;
INT             getstride;
INT             getauxstride;
INT             putstride;
INT             offset;
INT             pic_width;
GX_PIXELMAP    *pixelmap = info -> pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;
    get = info -> current_pixel_ptr;
    getaux = info -> current_aux_ptr;
    getstride = (pic_width + 1) >> 1;
    getauxstride = (pic_width + 7) >> 3;

    if ((info -> draw) && (xstart <= xend))
    {
        putstride = (context -> gx_draw_context_pitch + 1) >> 1;
        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * putstride;
        put += xstart >> 1;
        if (xstart & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            transmask = (GX_UBYTE)(0x80 >> (offset & 0x07));
            if ((*(getaux + (offset >> 3)) & transmask) == 0)
            {
                pixel = *(get + (offset >> 1));
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
            if (offset >= pic_width)
            {
                offset = 0;
            }

            putmask >>= 4;
            if (putmask == 0)
            {
                putmask = 0xf0;
                put++;
            }
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)getstride * sizeof(GX_UBYTE);
    info -> current_aux_ptr += (UINT)getauxstride * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_write   */
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
/*    info                                  Pointer to                    */
/*                                            GX_FILL_PIXELMAP_INFO struct*/
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
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                              INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_CONST GX_UBYTE *get_count = GX_NULL;
GX_UBYTE           pixel;
GX_UBYTE           putmask;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;
GX_UBYTE           getmask = 0;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* This means it's the draw operation. */
        /* Skip the invisible pixels.*/
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * ((context -> gx_draw_context_pitch + 1) >> 1) + (xstart >> 1);

        if (xstart & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }
        /*Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;

            /*Start from where we need to repeat.*/
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
            get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
            getmask = info -> mask;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get_count++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    pixel = (GX_UBYTE)(*get & getmask);
                    if (getmask == 0xf0)
                    {
                        pixel |= pixel >> 4;
                    }
                    else
                    {
                        pixel |= (GX_UBYTE)(pixel << 4);
                    }
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            *put &= (GX_UBYTE)(~putmask);
                            *put |= (pixel & putmask);
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
                        if (xval >= xstart && xval <= xend)
                        {
                            pixel = (GX_UBYTE)(*get & getmask);
                            if (getmask == 0xf0)
                            {
                                pixel |= pixel >> 4;
                            }
                            else
                            {
                                pixel |= (GX_UBYTE)(pixel << 4);
                            }
                            *put &= (GX_UBYTE)(~putmask);
                            *put |= (pixel & putmask);

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
            start_pos += pixelmap -> gx_pixelmap_width;
        }
    }
    else
    {
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
        getmask = info -> mask;
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

                getmask = (GX_UBYTE)(getmask >> (GX_UBYTE)((count & 1) << 2));
                if (getmask == 0)
                {
                    getmask = 0xf0;
                }
                /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* This line is drawn. cache the pointer for next line draw. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
    info -> current_aux_ptr = (GX_UBYTE *)get_count;
    info -> mask = getmask;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*  _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_          */
/*  transparent_write                                                     */
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
/*    info                                  Pointer to                    */
/*                                            GX_FILL_PIXELMAP_INFO struct*/
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
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_transparent_write(GX_DRAW_CONTEXT *context,
                                                                                          INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_CONST GX_UBYTE *get_count = GX_NULL;
GX_UBYTE           pixel;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;
GX_UBYTE           putmask;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* This means it's the draw operation. */
        /* Skip the invisible pixels.*/
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * ((context -> gx_draw_context_pitch + 1) >> 1) + (start_pos >> 1);

        if (start_pos & 0x01)
        {
            putmask = 0x0f;
        }
        else
        {
            putmask = 0xf0;
        }

        /*Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
            get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get_count++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    pixel = *get++;
                    /* 0xff means transparent.*/
                    if (pixel == 0xff)
                    {
                        xval += count;
                        while (count--)
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
                        pixel |= (GX_UBYTE)(pixel << 4);
                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                *put &= (GX_UBYTE)(~putmask);
                                *put |= (GX_UBYTE)(pixel & putmask);
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
                        pixel = *get++;
                        if (xval >= xstart && xval <= xend)
                        {
                            /* 0xff means transparent color, so should skip */
                            if (pixel != 0xff)
                            {
                                pixel |= (GX_UBYTE)(pixel << 4);
                                *put &= (GX_UBYTE)(~putmask);
                                *put |= (GX_UBYTE)(pixel & putmask);
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
            start_pos += pixelmap -> gx_pixelmap_width;
        }
    }
    else
    {
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
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


    /* This line is drawn. cache the pointer for next line draw. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
    info -> current_aux_ptr = (GX_UBYTE *)get_count;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_horizontal_pixelmap_line_draw               */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    4bpp screen driver horizontal pixelmap line drawing function that   */
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
/*     _gx_display_driver_4bpp_horizontal_pixelmap_line_c_t_write         */
/*                                          Real display driver horizontal*/
/*                                            pixelmap line draw function */
/*     _gx_display_driver_4bpp_horizontal_pixelmap_line_t_write           */
/*                                          Real display driver horizontal*/
/*                                            pixelmap line draw function */
/*     _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_write  */
/*                                          Real display driver horizontal*/
/*                                            pixelmap line draw function */
/*     _gx_display_driver_4bpp_horizontal_pixelmap_line_raw_write         */
/*                                          Real display driver horizontal*/
/*                                            pixelmap line draw function */
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
VOID _gx_display_driver_4bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                           INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
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
            _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_transparent_write(context, xstart, xend, y, info);
        }
        else
        {
            /* alpha, no compression */
            _gx_display_driver_4bpp_horizontal_pixelmap_line_transparent_write(context, xstart, xend, y, info);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no alpha */
            _gx_display_driver_4bpp_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        }
        else
        {
            /* no compression or alpha */
            _gx_display_driver_4bpp_horizontal_pixelmap_line_raw_write(context, xstart, xend, y, info);
        }
    }

    /* Current pixelmap has gone over, so the offset pointer should be reset. */
    if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
    {
        info -> current_pixel_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_data;
        info -> current_aux_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_aux_data;
    }
}

