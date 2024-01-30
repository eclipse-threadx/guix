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
/**   ST Simulation Display Management (Display)                          */
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
/*    _gx_chromart_simulation_display_driver_565rgb_pixelmap_32argb_write */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of 32bpp uncompressed */
/*    pixelemap data to 565 rgb canvas.                                   */
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
/*     _gx_display_driver_565rgb_pixel_write                              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_chromart_simulation_display_driver_565rgb_pixelmap_32argb_write(GX_DRAW_CONTEXT *context,
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
USHORT             color;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;


    display = context -> gx_draw_context_display;
    blend_function = display -> gx_display_driver_pixel_blend;
    write_function = display -> gx_display_driver_pixel_write;

    clip = context -> gx_draw_context_clip;
    skipcount = (pixelmap -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    skipcount += (clip -> gx_rectangle_left - xpos);

    getrow = (GX_COLOR *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;

        for (xval = clip -> gx_rectangle_left; xval < clip -> gx_rectangle_right; xval++)
        {
            alpha = (GX_UBYTE)(((*get) & 0xff000000) >> 24);
            r = (REDVAL_24BPP(*get) & 0xf8) >> 3;
            g = (GREENVAL_24BPP(*get) & 0xfc) >> 2;
            b = (BLUEVAL_24BPP(*get) & 0xf8) >> 3;
            color = ASSEMBLECOLOR_16BPP(r, g, b);
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
/*  _gx_chromart_simulation_display_driver_565rgb_pixelmap_draw           */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function simulates Chrom-Art drawing 32bpp format pixelmap to  */
/*    565rgb format canvas.                                               */
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
/*    _gx_chromart_simulation_display_driver_565rgb_pixelmap_32argb_write */
/*    _gx_display_driver_565rgb_pixelmap_draw                             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_chromart_simulation_display_driver_565rgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_32ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* Compression is not supported by the Chrom-Art hardware, so we
               do not support in software simulation driver. */
            return;
        }
        else
        {
            _gx_chromart_simulation_display_driver_565rgb_pixelmap_32argb_write(context,
                                                                                xpos, ypos, pixelmap);
        }
        break;

    default:
        /* Use generic drawing function for all other formats. */
        _gx_display_driver_565rgb_pixelmap_draw(context, xpos, ypos, pixelmap);
        break;
    }

    return;
}

