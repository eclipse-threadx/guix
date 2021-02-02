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
/*    _gx_display_driver_8bpp_rotated_pixelmap_raw_write  PORTABLE C      */
/*                                                           6.1.4        */
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
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                xval;
INT                yval;
GX_UBYTE          *putrow;
GX_UBYTE          *getrow;
GX_UBYTE          *put;
GX_CONST GX_UBYTE *get;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;

    GX_SWAP_VALS(xpos, ypos);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1);
        ypos = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width);
    }
    else
    {
        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
        xpos = (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height);
    }

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            *put++ = *get++;
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_height;
    }
}



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_compressed_write           */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                      INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_UBYTE          *put;
GX_UBYTE          *putrow;
GX_UBYTE           count;
GX_UBYTE           pixel = 0;
GX_RECTANGLE      *clip = context -> gx_draw_context_clip;

GX_RECTANGLE       rotated_clip;

    GX_SWAP_VALS(xpos, ypos);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1);
        ypos = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width);
    }
    else
    {
        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
        xpos = (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height);
    }

    get = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_data;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);

                /* Skip repeated pixel value.  */
                get++;
            }
            else
            {
                count++;

                /* Skip raw pixel values.  */
                get += count;
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row.  */
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < (xpos + pixelmap -> gx_pixelmap_height))
        {
            count = *get++;

            if (count & 0x80)
            {

                /* Repeated value.  */
                count = (GX_UBYTE)((count & 0x7f) + 1);
                pixel = *get++;

                while (count--)
                {
                    if (xval >= rotated_clip.gx_rectangle_left &&
                        xval <= rotated_clip.gx_rectangle_right)
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
                    if (xval >= rotated_clip.gx_rectangle_left &&
                        xval <= rotated_clip.gx_rectangle_right)
                    {
                        *put = *get;
                    }
                    put++;
                    get++;
                    xval++;
                }
            }
        }
        putrow +=  context -> gx_draw_context_pitch;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_transparent_write          */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_transparent_write(GX_DRAW_CONTEXT *context,
                                                                       INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                xval;
INT                yval;
GX_UBYTE          *putrow;
GX_UBYTE          *getrow;
GX_UBYTE          *put;
GX_UBYTE           inval;
GX_CONST GX_UBYTE *get;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;

    GX_SWAP_VALS(xpos, ypos);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1);
        ypos = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width);
    }
    else
    {
        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
        xpos = (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height);
    }

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            inval = *get++;
            if (inval == pixelmap -> gx_pixelmap_transparent_color)
            {
                put++;
            }
            else
            {
                *put++ = inval;
            }
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_compressed_transparent_    */
/*                                                                  write */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_8bpp_rotated_pixelmap_compressed_transparent_write(GX_DRAW_CONTEXT *context,
                                                                                  INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_UBYTE          *put;
GX_UBYTE          *putrow;
GX_UBYTE           count;
GX_UBYTE           pixel = 0;
GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;

    GX_SWAP_VALS(xpos, ypos);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1);
        ypos = (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width);
    }
    else
    {
        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
        xpos = (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height);
    }

    get = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_data;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);

                /* Skip repeated pixel value.  */
                get++;
            }
            else
            {
                count++;

                /* Skip raw pixel values.  */
                get += count;
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row.  */
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;
        while (xval < (xpos + pixelmap -> gx_pixelmap_height))
        {
            count = *get++;

            if (count & 0x80)
            {

                /* Repeated value.  */
                count = (GX_UBYTE)((count & 0x7f) + 1);
                pixel = *get++;

                if (pixel == pixelmap -> gx_pixelmap_transparent_color)
                {
                    put += count;
                    xval += count;
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            *put = pixel;
                        }
                        put++;
                        xval++;
                    }
                }
            }
            else
            {

                /* String of non-repeated values.  */
                count++;

                while (count--)
                {
                    pixel = *get++;

                    if (xval >= rotated_clip.gx_rectangle_left &&
                        xval <= rotated_clip.gx_rectangle_right &&
                        pixel != pixelmap -> gx_pixelmap_transparent_color)
                    {
                        *put = pixel;
                    }
                    put++;
                    xval++;
                }
            }
        }
        putrow +=  context -> gx_draw_context_pitch;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_rotated_pixelmap_draw       PORTABLE C      */
/*                                                           6.1.4        */
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
/*     _gx_display_driver_8bit_rotated_pixelmap_compressed_write          */
/*     _gx_display_driver_8bit_rotated_pixelmap_compressed_transparent_   */
/*                                                                  write */
/*     _gx_display_driver_8bit_rotated_pixelmap_transparent_write         */
/*     _gx_display_driver_8bit_rotated_pixelmap_raw_write                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_8bpp_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                   INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{

    if (pixelmap -> gx_pixelmap_format != GX_COLOR_FORMAT_8BIT_PALETTE ||
        (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA))
    {

        /* Wrong color format for this driver.  */
        return;
    }

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {

            /* Has both compression and transparent.  */
            _gx_display_driver_8bpp_rotated_pixelmap_compressed_transparent_write(context, xpos, ypos, pixelmap);
        }
        else
        {

            /* Transparent, no compression.  */
            _gx_display_driver_8bpp_rotated_pixelmap_transparent_write(context, xpos, ypos, pixelmap);
        }
    }
    else
    {
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {

            /* Compressed with no transparency.  */
            _gx_display_driver_8bpp_rotated_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
        }
        else
        {

            /* No compression or transaprency.  */
            _gx_display_driver_8bpp_rotated_pixelmap_raw_write(context, xpos, ypos, pixelmap);
        }
    }
}

