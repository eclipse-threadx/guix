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
/*    _gx_dave2d_simulation_display_driver_565rgb_horizontal              */
/*      _pixelmap_line_draw                                               */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb screen driver pixelmap drawing function that handles         */
/*    compressed format.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
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
static VOID _gx_dave2d_simulation_display_driver_565rgb_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                                   INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
USHORT             pixel;
GX_CONST GX_UBYTE *get;
USHORT            *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (USHORT *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + start_pos;

        /* Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
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
                        if (xval >= xstart && xval <= xend)
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
                        if (xval >= xstart && xval <= xend)
                        {
                            *put = (USHORT)((*get) | (USHORT)((*(get + 1)) << 8));
                        }
                        put++;
                        get += 2;
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
            count = *get++;
            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                get += 2;
            }
            else
            {
                count++;
                get += count * 2;
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
/*    _gx_dave2d_simulation_display_driver_565rgb_horizontal              */
/*      _pixelmap_line_draw                                               */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb screen driver pixelmap drawing function that handles         */
/*    compressed or uncompress format.                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_dave2d_simulation_display_driver_565rgb_pixelmap_line_c_write  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_draw            */
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
VOID _gx_dave2d_simulation_display_driver_565rgb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                                                INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
GX_BOOL      drawn = GX_FALSE;
GX_BOOL      synergy_specific_format = GX_FALSE;
GX_PIXELMAP *pixelmap = info -> pixelmap;

    if (pixelmap == GX_NULL)
    {
        return;
    }

    /*Format check, this pixelmap must be synergy format style.*/
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TARGA)
    {
        synergy_specific_format = GX_TRUE;
    }

    /*only support native format.*/
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        _gx_dave2d_simulation_display_driver_565rgb_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        drawn = GX_TRUE;
    }

    if (!synergy_specific_format && !drawn)
    {
        _gx_display_driver_565rgb_horizontal_pixelmap_line_draw(context, xstart, xend, y, info);
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

