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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_raw_write                 */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated            */
/*    uncompressed pixlemap data without alpha channel.                   */
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
static VOID _gx_display_driver_32bpp_rotated_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           xval;
INT           yval;
GX_COLOR     *putrow;
GX_COLOR     *getrow;
GX_COLOR     *put;
GX_COLOR     *get;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_RECTANGLE  rotated_clip;

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

    getrow = (GX_COLOR *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    /* Brush alpha is 0xff means draw pixelmap to memory directly.  */
    putrow = context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

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
/*    _gx_display_driver_32bpp_rotated_pixelmap_alpha_write               */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated            */
/*    uncompressed pixlemap data with alpha channel.                      */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32bpp_pixel_write                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_pixelmap_alpha_write(GX_DRAW_CONTEXT *context,
                                                                  INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           xval;
INT           yval;
GX_COLOR      color;
ULONG        *getrow;
ULONG        *get;
UCHAR         alpha_value;
GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_RECTANGLE  rotated_clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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

    getrow = (ULONG *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        get = getrow;
        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            alpha_value = ALPHAVAL_32BPP(*get);
            if (alpha_value)
            {
                color = *get;
                if (alpha_value == 255)
                {
                    _gx_display_driver_32bpp_pixel_write(context, xval, yval, color);
                }
                else
                {
                    color |= 0xff000000;
                    blend_func(context, xval, yval, color, alpha_value);
                }
            }
            get++;
        }
        getrow += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_compressed_write          */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated compressed */
/*    pixlemap data without alpha channel.                                */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32argb_pixel_blend                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                       INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_COLOR *get;
GX_COLOR          *put;
GX_COLOR          *putrow;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_CONST GX_UBYTE *get_count;
GX_UBYTE           brush_alpha;
GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    get = (GX_CONST GX_COLOR *)pixelmap -> gx_pixelmap_data;
    get_count = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1u);

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
       to the enf of the last visible row.  */
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get_count++;

            if (count & 0x80)
            {

                /* Repeated value.  */
                count = (GX_UBYTE)((count & 0x7f) + 1u);
                pixel = (*get++);

                if (brush_alpha == 0xff)
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
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, pixel, brush_alpha);
                        }
                        xval++;
                    }
                }
            }
            else
            {

                /* String of non-repeated values.  */
                count++;

                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            *put = (*get);
                        }
                        put++;
                        get++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, *get, brush_alpha);
                        }
                        get++;
                        xval++;
                    }
                }
            }
        }
        putrow += context -> gx_draw_context_pitch;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_compressed_alpha_write    */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32argb_pixel_blend                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                             INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_COLOR *get;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_CONST GX_UBYTE *get_count;
GX_UBYTE           brush_alpha;
GX_UBYTE           alpha;
GX_UBYTE           combined_alpha;
GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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

    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    get = (GX_CONST GX_COLOR *)pixelmap -> gx_pixelmap_data;
    get_count = (GX_CONST GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get_count++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1u);

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
       to the enf of the last visible row.  */

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get_count++;

            if (count & 0x80)
            {

                /* Repeated value.  */
                count = (GX_UBYTE)((count & 0x7f) + 1u);
                alpha = (GX_UBYTE)((*get) >> 24);
                pixel = (*get++) | 0xff000000;

                combined_alpha = (GX_UBYTE)(alpha * brush_alpha / 255);

                if (combined_alpha)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, pixel, combined_alpha);
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

                /* String of non-repeated values.  */
                count++;

                while (count--)
                {
                    if (xval >= rotated_clip.gx_rectangle_left &&
                        xval <= rotated_clip.gx_rectangle_right)
                    {
                        alpha = (GX_UBYTE)((*get) >> 24);
                        pixel = (*get) | 0xff000000;
                        combined_alpha = (GX_UBYTE)(alpha * brush_alpha / 255);
                        blend_func(context, xval, yval, pixel, combined_alpha);
                    }
                    get++;
                    xval++;
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
/*    _gx_display_driver_32bpp_rotated_palette_pixelmap_compressed_write  */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated compressed */
/*    pixlemap file without transparent of palette pixelmap.              */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            moved NULL pointer check to */
/*                                            caller function,            */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_palette_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
GX_COLOR       *put;
GX_COLOR       *putrow;
GX_COLOR        pixel;
GX_COLOR       *palette;
GX_UBYTE        brush_alpha;
GX_RECTANGLE   *clip = context -> gx_draw_context_clip;
GX_RECTANGLE    rotated_clip;
VOID            (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* Compressed with  alpha is a one-byte count and  one-byte pixel index,
       first, skip to the starting row.   */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (UCHAR)((count & 0x7f) + 1);

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
       to the enf of the last visible row. */
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    /* Now we are on the first visible row, copy pixels until we get
       to the end of the last visible row.  */
    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        xval = xpos;
        put = putrow;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* Repeated value.  */
                count = (UCHAR)((count & 0x7f) + 1);

                pixel = palette[*get++];

                if (brush_alpha == 0xff)
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
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, pixel, brush_alpha);
                        }
                        xval++;
                    }
                }
            }
            else
            {
                /* String of non-repeated values.  */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            pixel = palette[*get];
                            *put = pixel;
                        }
                        get++;
                        put++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            pixel = palette[*get];
                            blend_func(context, xval, yval, pixel, brush_alpha);
                        }
                        get++;
                        xval++;
                    }
                }
            }
        }
        putrow += context -> gx_draw_context_pitch;
        yval++;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_palette_pixelmap_write             */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated            */
/*    uncompressed pixlemap file without transparent of palette pixelmap. */
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
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_palette_pixelmap_write(GX_DRAW_CONTEXT *context,
                                                                    INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           xval;
INT           yval;
GX_UBYTE     *getrow;
GX_UBYTE     *get;
GX_COLOR     *palette;
GX_COLOR     *put;
GX_COLOR     *putrow;
GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_RECTANGLE  rotated_clip;

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

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += (pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos));
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    /* Now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row.  */
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            *put++ = palette[*get++];
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_palette_pixelmap_transparent_write */
/*                                                                        */
/*                                                         PORTABLE C     */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file with transparent of palette pixelmap.                 */
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
/*    _gx_display_driver_32bpp_pixel_write                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_palette_pixelmap_transparent_write(GX_DRAW_CONTEXT *context,
                                                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           xval;
INT           yval;
GX_UBYTE     *getrow;
GX_UBYTE     *get;
GX_COLOR     *palette;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_RECTANGLE  rotated_clip;

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

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;
    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += (pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos));
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        get = getrow;
        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
            {
                _gx_display_driver_32bpp_pixel_write(context, xval, yval, palette[*get]);
            }
            get++;
        }

        getrow += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_palette_pixelmap_transparent_      */
/*                                                       compressed_write */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated compressed */
/*    pixlemap file with transparent of palette pixelmap.                 */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32argb_pixel_blend                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_palette_pixelmap_transparent_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT             yval;
INT             xval;
GX_CONST UCHAR *get;
UCHAR           count;
GX_COLOR        pixel;
GX_COLOR       *palette;
GX_COLOR       *put;
GX_COLOR       *putrow;
GX_UBYTE        brush_alpha;
GX_RECTANGLE   *clip = context -> gx_draw_context_clip;
GX_RECTANGLE    rotated_clip;
VOID            (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    get = (GX_CONST UCHAR *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* Compressed with  alpha is a one-byte count and  one-byte pixel index,
       first, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (UCHAR)((count & 0x7f) + 1);

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
       to the end of the last visible row. */
    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        xval = xpos;
        put = putrow;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get++;
            if (count & 0x80)
            {
                /* repeated value */
                count = (UCHAR)((count & 0x7f) + 1);
                if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                {
                    pixel = palette[*get];
                    if (brush_alpha == 0xff)
                    {
                        while (count--)
                        {
                            if (xval >= rotated_clip.gx_rectangle_left &&
                                xval <= rotated_clip.gx_rectangle_right)
                            {
                                *put = pixel;
                            }
                            xval++;
                            put++;
                        }
                    }
                    else
                    {
                        while (count--)
                        {
                            if (xval >= rotated_clip.gx_rectangle_left &&
                                xval <= rotated_clip.gx_rectangle_right)
                            {
                                blend_func(context, xval, yval, pixel, brush_alpha);
                            }
                            xval++;
                        }
                    }
                }
                else
                {
                    xval += count;
                    put += count;
                }

                get++;
            }
            else
            {
                /* string of non-repeated values */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                        {
                            if (xval >= rotated_clip.gx_rectangle_left &&
                                xval <= rotated_clip.gx_rectangle_right)
                            {
                                pixel = palette[*get];
                                *put = pixel;
                            }
                        }
                        get++;
                        xval++;
                        put++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
                        {
                            if (xval >= rotated_clip.gx_rectangle_left &&
                                xval <= rotated_clip.gx_rectangle_right)
                            {
                                pixel = palette[*get];
                                blend_func(context, xval, yval, pixel, brush_alpha);
                            }
                        }
                        get++;
                        xval++;
                    }
                }
            }
        }
        yval++;
        putrow += context -> gx_draw_context_pitch;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_565rgb_pixelmap_raw_write          */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of 565rgb format      */
/*    uncompressed pixlemap file without alpha channel.                   */
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
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_565rgb_pixelmap_raw_write(GX_DRAW_CONTEXT *context,
                                                                       INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              xval;
INT              yval;
GX_COLOR        *putrow;
USHORT          *getrow;
GX_COLOR        *put;
GX_CONST USHORT *get;

GX_RECTANGLE    *clip = context -> gx_draw_context_clip;
GX_RECTANGLE     rotated_clip;

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

    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    getrow += (rotated_clip.gx_rectangle_left - xpos);

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            *put++ = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff,
                                                    REDVAL_16BPP(*get) << 3,
                                                    GREENVAL_16BPP(*get) << 2,
                                                    BLUEVAL_16BPP(*get) << 3);
            get++;
        }
        putrow += context -> gx_draw_context_pitch;
        getrow += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_compressed_write  */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated compressed */
/*    pixelmap data of 565rgb format in 32bpp driver.                     */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_565rgb_pixelmap_compressed_write(GX_DRAW_CONTEXT *context,
                                                                              INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              yval;
INT              xval;
GX_CONST USHORT *get;
USHORT           count;
GX_COLOR         pixel;
GX_UBYTE         r;
GX_UBYTE         g;
GX_UBYTE         b;
GX_UBYTE         brush_alpha;
GX_COLOR        *put;
GX_COLOR        *putrow;
GX_RECTANGLE    *clip = context -> gx_draw_context_clip;
GX_RECTANGLE     rotated_clip;
VOID             (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    get = (GX_CONST USHORT *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1);

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
       to the enf of the last visible row.  */
    putrow = (GX_COLOR *)context -> gx_draw_context_memory;
    putrow += yval * context -> gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        xval = xpos;
        put = putrow;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x8000)
            {

                /* Repeated value.  */
                count = (USHORT)((count & 0x7fff) + 1);
                pixel = *get++;
                r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);

                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            *put = pixel;
                        }
                        xval++;
                        put++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, pixel, brush_alpha);
                        }
                        xval++;
                    }
                }
            }
            else
            {

                /* String of non-repeated values.  */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            pixel = *get;
                            r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                            g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                            b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                            pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                            *put = pixel;
                        }
                        get++;
                        put++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            pixel = *get;
                            r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                            g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                            b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                            pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                            blend_func(context, xval, yval, pixel, brush_alpha);
                        }
                        get++;
                        xval++;
                    }
                }
            }
        }
        yval++;
        putrow += context -> gx_draw_context_pitch;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_compressed_alpha_ */
/*                                                               write    */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed-alpha   */
/*    pixelmap data of 565rgb format with 32bpp driver.                   */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32bpp_pixel_write                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_565rgb_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                    INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_UBYTE           alpha_value;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;
GX_UBYTE           brush_alpha;
GX_UBYTE           combined_alpha;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);

                /* Skip repeated pixel value.  */
                get += 4;
            }
            else
            {
                count++;

                /* Skip raw pixel values.  */
                get += (count * 4);
            }
            xval += count;
        }
    }

    /* Now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row.  */
    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get;

            if (count & 0x80)
            {

                /* Repeated value.  */
                count = (GX_UBYTE)((count & 0x7f) + 1);
                alpha_value = *(get + 1);

                if (alpha_value)
                {
                    if (brush_alpha == 0xff)
                    {
                        get += 2;
                        pixel = *(USHORT *)get;
                        r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                        g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                        b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                        pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                        get += 2;
                        while (count--)
                        {
                            if (xval >= rotated_clip.gx_rectangle_left &&
                                xval <= rotated_clip.gx_rectangle_right)
                            {
                                if (alpha_value == 0xff)
                                {
                                    _gx_display_driver_32bpp_pixel_write(context, xval, yval, pixel);
                                }
                                else
                                {
                                    blend_func(context, xval, yval, pixel, alpha_value);
                                }
                            }
                            xval++;
                        }
                    }
                    else
                    {
                        combined_alpha = (GX_UBYTE)(alpha_value * brush_alpha / 255);
                        if (combined_alpha)
                        {
                            get += 2;
                            pixel = *(USHORT *)get;
                            r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                            g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                            b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                            pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                            get += 2;
                            while (count--)
                            {
                                if (xval >= rotated_clip.gx_rectangle_left &&
                                    xval <= rotated_clip.gx_rectangle_right)
                                {
                                    blend_func(context, xval, yval, pixel, combined_alpha);
                                }
                                xval++;
                            }
                        }
                        else
                        {
                            get += 4;
                            xval += count;
                        }
                    }
                }
                else
                {
                    xval += count;
                    get += 4;
                }
            }
            else
            {

                /* String of non-repeated values.  */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            alpha_value = *(get + 1);
                            get += 2;
                            if (alpha_value)
                            {
                                pixel = *(USHORT *)get;
                                r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                                g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                                b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                                pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                                if (alpha_value == 0xff)
                                {
                                    _gx_display_driver_32bpp_pixel_write(context, xval, yval, pixel);
                                }
                                else
                                {
                                    blend_func(context, xval, yval, pixel, alpha_value);
                                }
                            }
                            get += 2;
                        }
                        else
                        {
                            get += 4;
                        }
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            alpha_value = *(get + 1);
                            get += 2;
                            if (alpha_value)
                            {
                                pixel = *(USHORT *)get;
                                r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                                g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                                b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                                pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                                combined_alpha = (GX_UBYTE)(brush_alpha * alpha_value / 255);
                                blend_func(context, xval, yval, pixel, combined_alpha);
                            }
                            get += 2;
                        }
                        else
                        {
                            get += 4;
                        }
                        xval++;
                    }
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
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_alpha_write       */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of non_compressed     */
/*    but with alpha channel pixelmap data of 565rgb format with 32bpp    */
/*    driver.                                                             */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32bpp_pixel_write                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_565rgb_pixelmap_alpha_write(GX_DRAW_CONTEXT *context,
                                                                         INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT                skipcount;
INT                xval;
INT                yval;
GX_CONST GX_UBYTE *getalpha;
GX_CONST USHORT   *get;
USHORT            *getrow;
GX_UBYTE          *getrowalpha;
GX_UBYTE           r;
GX_UBYTE           g;
GX_UBYTE           b;
GX_COLOR           pixel;
GX_UBYTE           alpha_value;

GX_RECTANGLE      *clip = context -> gx_draw_context_clip;
GX_RECTANGLE       rotated_clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    skipcount = (pixelmap -> gx_pixelmap_height) * (rotated_clip.gx_rectangle_top - ypos);
    skipcount += (rotated_clip.gx_rectangle_left - xpos);
    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    getrowalpha = (GX_UBYTE *)(pixelmap -> gx_pixelmap_aux_data);
    getrowalpha += skipcount;

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        get = getrow;
        getalpha = getrowalpha;

        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            alpha_value = *getalpha++;
            pixel = *get++;
            if (alpha_value)
            {
                r = (GX_UBYTE)(REDVAL_16BPP(pixel) << 3);
                g = (GX_UBYTE)(GREENVAL_16BPP(pixel) << 2);
                b = (GX_UBYTE)(BLUEVAL_16BPP(pixel) << 3);
                pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                if (alpha_value == 0xff)
                {
                    _gx_display_driver_32bpp_pixel_write(context, xval, yval, pixel);
                }
                else
                {
                    blend_func(context, xval, yval, pixel, alpha_value);
                }
            }
        }
        getrow += pixelmap -> gx_pixelmap_height;
        getrowalpha += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_4444argb_pixelmap_alpha_write      */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file with alpha channel of 4444argb format.                */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*    _gx_display_driver_32bpp_pixel_write                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_4444argb_pixelmap_alpha_write(GX_DRAW_CONTEXT *context,
                                                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              skipcount;
INT              xval;
INT              yval;
USHORT          *getrow;
GX_CONST USHORT *get;
UCHAR            alpha_value;
ULONG            pixel;

GX_RECTANGLE    *clip = context -> gx_draw_context_clip;
GX_RECTANGLE     rotated_clip;
VOID             (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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

    /* Calculate how many pixels to skip.  */
    skipcount = (pixelmap -> gx_pixelmap_height) * (rotated_clip.gx_rectangle_top - ypos);
    skipcount += (rotated_clip.gx_rectangle_left - xpos);
    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    for (yval = rotated_clip.gx_rectangle_top; yval <= rotated_clip.gx_rectangle_bottom; yval++)
    {
        get = getrow;

        for (xval = rotated_clip.gx_rectangle_left; xval <= rotated_clip.gx_rectangle_right; xval++)
        {
            alpha_value = (UCHAR)(((*get) & 0xf000) >> 8);
            if (alpha_value)
            {

                /* Convert 4444argb pixel to 24xrgb pixel.  */
                pixel = (GX_COLOR)((((*get) & 0x0f00) << 12) | (((*get) & 0x00f0) << 8) | (((*get) & 0x000f) << 4));
                pixel |= 0xff000000;
                if (alpha_value == 0xf0)
                {
                    _gx_display_driver_32bpp_pixel_write(context, xval, yval, pixel);
                }
                else
                {
                    blend_func(context, xval, yval, pixel, alpha_value);
                }
            }
            get++;
        }
        getrow += pixelmap -> gx_pixelmap_height;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_4444argb_rotated_pixelmap_compressed_     */
/*                                                            alpha_write */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of rotated compressed */
/*    pixelmap data of 4444argb format.                                   */
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
/*    _gx_display_driver_24xrgb_pixel_blend                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_rotated_pixelmap_draw                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*  03-02-2021     Ting Zhu                 Modified comment(s), changed  */
/*                                            blend function set macro,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_32bpp_rotated_4444argb_pixelmap_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                      INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT              yval;
INT              xval;
GX_CONST USHORT *get;
USHORT           count;
GX_COLOR         pixel;
GX_UBYTE         falpha;
GX_UBYTE         brush_alpha;
GX_UBYTE         combined_alpha;
GX_UBYTE         r;
GX_UBYTE         g;
GX_UBYTE         b;
GX_RECTANGLE    *clip = context -> gx_draw_context_clip;
GX_RECTANGLE     rotated_clip;
VOID             (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    GX_SET_32BPP_BLEND_FUNCTION(blend_func, context -> gx_draw_context_display -> gx_display_color_format);

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
    get = (GX_CONST USHORT *)pixelmap -> gx_pixelmap_data;
    brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    /* First, skip to the starting row.  */
    for (yval = ypos; yval < rotated_clip.gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1);

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
       to the enf of the last visible row.  */
    while (yval <= rotated_clip.gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pixelmap -> gx_pixelmap_height)
        {
            count = *get++;

            if (count & 0x8000)
            {
                /* Repeated value.  */
                count = (USHORT)((count & 0x7fff) + 1);
                pixel = *get++;
                falpha = (GX_UBYTE)(((USHORT)pixel & 0xf000) >> 8);
                falpha = (falpha >> 4) | falpha;
                if (falpha)
                {
                    r = (GX_UBYTE)(((USHORT)pixel & 0x0f00) >> 4);
                    r = (GX_UBYTE)((r >> 4) | r);
                    g = (GX_UBYTE)((USHORT)pixel & 0x00f0);
                    g = (GX_UBYTE)((g >> 4) | g);
                    b = (GX_UBYTE)((USHORT)pixel & 0x000f);
                    b = (GX_UBYTE)((b << 4) | b);
                    pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                    if (brush_alpha == 0xff)
                    {
                        combined_alpha = falpha;
                    }
                    else
                    {
                        combined_alpha = (GX_UBYTE)(brush_alpha * falpha / 255);
                    }

                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            blend_func(context, xval, yval, pixel, combined_alpha);
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

                /* String of non-repeated values.  */
                count++;
                if (brush_alpha == 0xff)
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            pixel = *get;
                            falpha = (GX_UBYTE)(((USHORT)pixel & 0xf000) >> 8);
                            falpha = (falpha >> 4) | falpha;
                            r = (GX_UBYTE)(((USHORT)pixel & 0x0f00) >> 4);
                            r = (GX_UBYTE)((r >> 4) | r);
                            g = (GX_UBYTE)((USHORT)pixel & 0x00f0);
                            g = (GX_UBYTE)((g >> 4) | g);
                            b = (GX_UBYTE)((USHORT)pixel & 0x000f);
                            b = (GX_UBYTE)((b << 4) | b);
                            pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                            if (falpha)
                            {
                                blend_func(context, xval, yval, pixel, falpha);
                            }
                        }
                        get++;
                        xval++;
                    }
                }
                else
                {
                    while (count--)
                    {
                        if (xval >= rotated_clip.gx_rectangle_left &&
                            xval <= rotated_clip.gx_rectangle_right)
                        {
                            pixel = *get;
                            falpha = (GX_UBYTE)(((USHORT)pixel & 0xf000) >> 8);
                            falpha = (falpha >> 4) | falpha;
                            combined_alpha = (GX_UBYTE)(falpha * brush_alpha / 255);
                            r = (GX_UBYTE)(((USHORT)pixel & 0x0f00) >> 4);
                            r = (GX_UBYTE)((r >> 4) | r);
                            g = (GX_UBYTE)((USHORT)pixel & 0x00f0);
                            g = (GX_UBYTE)((g >> 4) | g);
                            b = (GX_UBYTE)((USHORT)pixel & 0x000f);
                            b = (GX_UBYTE)((b << 4) | b);
                            pixel = (GX_COLOR)ASSEMBLECOLOR_32ARGB(0xff, r, g, b);
                            blend_func(context, xval, yval, pixel, combined_alpha);
                        }
                        get++;
                        xval++;
                    }
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
/*    _gx_display_driver_24xrgb_rotated_pixelmap_draw     PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32xrgb format screen driver pixelmap drawing function that handles  */
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
/*    _gx_display_driver_24xrgb_rotated_palette_pixelmap_transparent_     */
/*                                                       compressed_write */
/*    _gx_display_driver_24xrgb_rotated_palette_pixelmap_transparent_write*/
/*    _gx_display_driver_24xrgb_rotated_palette_pixelmap_compressed_write */
/*    _gx_display_driver_24xrgb_rotated_palette_pixelmap_write            */
/*    _gx_display_driver_24xrgb_rotated_4444argb_pixelmap_compressed_     */
/*                                                            alpha_write */
/*    _gx_display_driver_24xrgb_rotated_4444argb_pixelmap_alpha_write     */
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_compressed_alpha_ */
/*                                                                  write */
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_alpha_write       */
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_compressed_write  */
/*    _gx_display_driver_24xrgb_rotated_565rgb_pixelmap_raw_write         */
/*    _gx_display_driver_24xrgb_rotated_pixelmap_compressed_alpha_write   */
/*    _gx_display_driver_24xrgb_rotated_pixelmap_alpha_write              */
/*    _gx_display_driver_24xrgb_rotated_pixelmap_compressed_write         */
/*    _gx_display_driver_24xrgb_rotated_pixelmap_raw_write                */
/*    _gx_display_driver_24xrgb_rotated_pixelmap_blend                    */
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
/*  03-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added 8bit palette table    */
/*                                            NULL pointer check,         */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_32bpp_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                    INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
GX_BOOL  drawn = GX_FALSE;
GX_UBYTE brush_alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (brush_alpha == 0)
    {
        /* Draw nothing here. Just return.  */
        return;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (pixelmap -> gx_pixelmap_aux_data == GX_NULL)
        {

            /* Palette data is not availlable.  */
            return;
        }

        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* Compressed with transparent.  */
                _gx_display_driver_32bpp_rotated_palette_pixelmap_transparent_compressed_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* No compression with transparent.  */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_32bpp_rotated_palette_pixelmap_transparent_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* Compressed with no alpha.  */
                _gx_display_driver_32bpp_rotated_palette_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* No compression or alpha.  */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_32bpp_rotated_palette_pixelmap_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* Compressed.  */
            _gx_display_driver_32bpp_rotated_4444argb_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
            drawn = GX_TRUE;
        }
        else
        {
            /* No compression.  */
            if (brush_alpha == 0xff)
            {
                _gx_display_driver_32bpp_rotated_4444argb_pixelmap_alpha_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
        }
        break;

    case GX_COLOR_FORMAT_565RGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* Compressed with alpha.  */
                _gx_display_driver_32bpp_rotated_565rgb_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {
                /* Uncompressed with alpha.  */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_32bpp_rotated_565rgb_pixelmap_alpha_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {

                /* Compressed without alpha.  */
                _gx_display_driver_32bpp_rotated_565rgb_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {

                /* Uncompressed withou alpha.  */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_32bpp_rotated_565rgb_pixelmap_raw_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        break;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {

                /* Has both compression and alpha.  */
                _gx_display_driver_32bpp_rotated_pixelmap_compressed_alpha_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {

                /* Alpha, no compression.  */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_32bpp_rotated_pixelmap_alpha_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {

                /* Compressed with no alpha.  */
                _gx_display_driver_32bpp_rotated_pixelmap_compressed_write(context, xpos, ypos, pixelmap);
                drawn = GX_TRUE;
            }
            else
            {

                /* No compression or alpha.  */
                if (brush_alpha == 0xff)
                {
                    _gx_display_driver_32bpp_rotated_pixelmap_raw_write(context, xpos, ypos, pixelmap);
                    drawn = GX_TRUE;
                }
            }
        }
        break;

    default:
        break;
    }

    if ((!drawn) && (brush_alpha != 0xff))
    {
        _gx_display_driver_32bpp_rotated_pixelmap_blend(context, xpos, ypos, pixelmap, brush_alpha);
    }

    return;
}

