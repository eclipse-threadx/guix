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
#include "gx_dave2d_simulation_display_driver.h"
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_c_a_blend      */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of compressed        */
/*    pixlemap file with alpha channel.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_24xrgb_pixel_blend                              */
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
static VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_compressed_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                                         INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT                xval;
INT                yval;
GX_CONST GX_UBYTE *get;
GX_UBYTE           count;
ULONG              pixel;
ULONG              combined_alpha;
GX_UBYTE           self_alpha;
GX_RECTANGLE      *clip;

    clip = context -> gx_draw_context_clip;
    get = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_data;

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
                count = (GX_UBYTE)((count & 0x7f) + 1);
                get += 4;
            }
            else
            {
                count++;
                get += count * 4;
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
                count = (GX_UBYTE)((count & 0x7f) + 1);
                pixel = (*get++);
                pixel = (ULONG)((*get++) << 8) | pixel;
                pixel = (ULONG)((*get++) << 16) | pixel;
                pixel |= 0xff000000;
                self_alpha = *get++;
                if (self_alpha)
                {
                    combined_alpha = (ULONG)(self_alpha * alpha);
                    combined_alpha /= 255;
                    while (count--)
                    {
                        if (xval >= clip -> gx_rectangle_left &&
                            xval <= clip -> gx_rectangle_right)
                        {
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, (GX_UBYTE)combined_alpha);
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
                count++;
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        self_alpha = *(get + 3);
                        if (self_alpha)
                        {
                            pixel = (ULONG)((UINT)((*(get)) | ((*(get + 1)) << 8) | ((*(get + 2)) << 16)) | 0xff000000);
                            combined_alpha = (ULONG)(self_alpha * alpha);
                            combined_alpha /= 255;
                            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, (GX_UBYTE)combined_alpha);
                        }
                    }
                    xval++;
                    get += 4;
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
/*    _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_c_blend        */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of compressed        */
/*    pixlemap file with alpha channel.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_24xrgb_pixel_blend                              */
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
static VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_compressed_blend(GX_DRAW_CONTEXT *context,
                                                                                   INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT                xval;
INT                yval;
GX_CONST GX_UBYTE *get;
GX_UBYTE           count;
ULONG              pixel;
GX_RECTANGLE      *clip;

    clip = context -> gx_draw_context_clip;
    get = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_data;

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
                count = (GX_UBYTE)((count & 0x7f) + 1);
                get += 4;
            }
            else
            {
                count++;
                get += count * 4;
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
                count = (GX_UBYTE)((count & 0x7f) + 1);
                pixel = (*get++);
                pixel = (ULONG)((*get++) << 8) | pixel;
                pixel = (ULONG)((*get++) << 16) | pixel;
                pixel |= 0xff000000;
                /*skip the useless alpha value.*/
                get++;
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha);
                    }
                    xval++;
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
                        pixel = (ULONG)((UINT)((*(get)) | ((*(get + 1)) << 8) | ((*(get + 2)) << 16)) | 0xff000000);
                        _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha);
                    }
                    xval++;
                    get += 4;
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
/*    _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_blend          */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of uncompressed      */
/*    pixlemap file without alpha channel.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_24xrgb_pixel_blend                              */
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
VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                                                 INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
GX_BOOL synergy_specific_format = GX_FALSE;
GX_BOOL drawn = GX_FALSE;

    if (alpha == 0)
    {
        return;
    }

    if ((pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TARGA) &&
        (pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_8BIT_PALETTE))
    {
        synergy_specific_format = GX_TRUE;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_compressed_alpha_blend(context, xpos, ypos, pixelmap, alpha);
        }
        else
        {
            _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_compressed_blend(context, xpos, ypos, pixelmap, alpha);
        }
        drawn = GX_TRUE;
    }

    if (!synergy_specific_format && !drawn)
    {
        _gx_display_driver_24xrgb_pixelmap_blend(context, xpos, ypos, pixelmap, alpha);
    }
}

