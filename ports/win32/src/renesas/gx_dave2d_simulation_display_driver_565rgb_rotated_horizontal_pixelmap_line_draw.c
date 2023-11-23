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
/*    _gx_dave2d_simulation_display_driver_565rgb_rotated_horizontal_     */
/*                                         pixelmap_line_compressed_write */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
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
/*    ystart                                y-coord of line top           */
/*    yend                                  y-coord of line bottom        */
/*    x                                     x-coord of line left          */
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
/*  xx-xx-xxxx     Ting Zhu                 Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static VOID _gx_dave2d_simulation_display_driver_565rgb_rotated_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                                           INT ystart, INT yend, INT x, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
USHORT             pixel;
GX_CONST GX_UBYTE *get;
USHORT            *put;
GX_PIXELMAP       *pixelmap;
INT                xstart;
INT                xend;
INT                y;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        xstart = ystart;
        xend = yend;
        y = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - x - 1;
    }
    else
    {
        xstart = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - yend - 1;
        xend = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - ystart - 1;
        y = x;
        info -> x_offset = (yend - ystart + 1 + info -> x_offset) % info -> pixelmap -> gx_pixelmap_height;

        if (info -> x_offset)
        {
            info -> x_offset = info -> pixelmap -> gx_pixelmap_height - info -> x_offset;
        }
    }

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position.  */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_height);

        put = (USHORT *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + start_pos;

        /* Repeat the draw operation to fill the whole dirty area.  */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat.  */
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_height)
            {
                count = *get++;
                if (count & 0x80)
                {

                    /* Repeated value.  */
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

                    /* String of non-repeated values.  */
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
            start_pos += pixelmap -> gx_pixelmap_height;
        }
    }
    else
    {
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        while (xval < pixelmap -> gx_pixelmap_height)
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

    /* This line is drawn. cache the pointer for next line draw.  */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_565rgb_horizontal              */
/*      _pixelmap_line_draw                                               */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
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
/*    ystart                                y-coord of line top           */
/*    yend                                  y-coord of line bottom        */
/*    x                                     x-coord of line left          */
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
/*  xx-xx-xxxx     Ting Zhu                 Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_565rgb_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                                                        INT ystart, INT yend, INT x, GX_FILL_PIXELMAP_INFO *info)
{
GX_BOOL      drawn = GX_FALSE;
GX_BOOL      synergy_specific_format = GX_FALSE;
GX_PIXELMAP *pixelmap = info -> pixelmap;

    if (pixelmap == GX_NULL)
    {
        return;
    }

    /* Format check, this pixelmap must be synergy format style.  */
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TARGA)
    {
        synergy_specific_format = GX_TRUE;
    }

    /* Only support native format.  */
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        _gx_dave2d_simulation_display_driver_565rgb_rotated_horizontal_pixelmap_line_compressed_write(context, ystart, yend, x, info);
        drawn = GX_TRUE;
    }

    if (!synergy_specific_format && !drawn)
    {
        _gx_display_driver_565rgb_rotated_horizontal_pixelmap_line_draw(context, ystart, yend, x, info);
        return;
    }

    if (drawn)
    {
        /* Current pixelmap has gone over, so the offset pointer should be reset.  */
        if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
        {
            info -> current_pixel_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_data;
            info -> current_aux_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_aux_data;
        }
    }
}

