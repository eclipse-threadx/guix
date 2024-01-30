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
#include "gx_display.h"
#include "gx_context.h"
#include "gx_dave2d_simulation_display_driver.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_32bpp_horizontal               */
/*      _pixelmap_line_compressed_write                                   */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
static VOID _gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                                  INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_UBYTE           alpha_value;
GX_COLOR           pixel;
GX_CONST GX_UBYTE *get;
GX_BOOL            has_alpha;
GX_PIXELMAP       *pixelmap;
VOID               (*write_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color);
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    write_func = _gx_display_driver_32bpp_pixel_write;
    blend_func = _gx_display_driver_24xrgb_pixel_blend;
    pixelmap = info -> pixelmap;

    if ((write_func == GX_NULL) ||
        (blend_func == GX_NULL))
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            has_alpha = GX_TRUE;
        }
        else
        {
            has_alpha = GX_FALSE;
        }

        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /* Repeat the draw operation to fill the whole dirty area. */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /*Start from where we need to repeat.*/
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    /* repeated value */
                    pixel = (*get++);
                    pixel = (ULONG)((*get++) << 8) | pixel;
                    pixel = (ULONG)((*get++) << 16) | pixel;

                    if (has_alpha)
                    {
                        alpha_value = *get++;
                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                blend_func(context, xval, y, pixel, alpha_value);
                            }
                            xval++;
                        }
                    }
                    else
                    {
                        get += 1;
                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                write_func(context, xval, y, pixel);
                            }
                            xval++;
                        }
                    }
                }
                else
                {
                    count++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            pixel = (ULONG)((*(get)) | ((*(get + 1)) << 8) | ((*(get + 2)) << 16));
                            if (has_alpha)
                            {
                                alpha_value = *(get + 3);
                                blend_func(context, xval, y, pixel, alpha_value);
                            }
                            else
                            {
                                write_func(context, xval, y, pixel);
                            }
                        }
                        get += 4;
                        xval++;
                    }
                }
            }
            start_pos += pixelmap -> gx_pixelmap_width;
        }
    }
    else
    {
        /*Just do skip operation here. */
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
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

    /*This line is drawn. cache the pointer for next line draw.*/
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_32bpp_horizontal               */
/*      _pixelmap_line_draw                               PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32bpp screen driver horizontal pixelmap line drawing function that  */
/*    handles compressed or uncompress, with or without alpha channel.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_dave2d_simulation_display_driver_32bpp_horizontal             */
/*       _pixelmap_line_compressed_write                                  */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_draw             */
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
VOID _gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                                               INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
GX_BOOL drawn = GX_FALSE;
GX_BOOL synergy_specific_format = GX_FALSE;

    if (info -> pixelmap == GX_NULL)
    {
        return;
    }

    /*Format check, this pixelmap must be synergy format style.*/
    if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TARGA)
    {
        synergy_specific_format = GX_TRUE;
    }

    if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        /* compressed with or without alpha */
        _gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        drawn = GX_TRUE;
    }

    if (!synergy_specific_format && !drawn)
    {
        _gx_display_driver_32bpp_horizontal_pixelmap_line_draw(context, xstart, xend, y, info);
        return;
    }

    if (drawn)
    {
        /*Current pixelmap has gone over, so the offset pointer should be reset.*/
        if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
        {
            info -> current_pixel_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_data;
            info -> current_aux_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_aux_data;
        }
    }
}

