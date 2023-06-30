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
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"

#if defined GX_BRUSH_ALPHA_SUPPORT

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_raw_alpha_draw  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing  with brush alpha     */
/*    of uncompressed alpha map file.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    alpha                                 alpha value from 0 to 255     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_draw                            */
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
static VOID  _gx_display_driver_generic_alphamap_raw_alpha_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp, GX_UBYTE alpha)
{
INT                xval;
INT                yval;
GX_UBYTE          *getrowalpha;
GX_CONST GX_UBYTE *getalpha;
GX_UBYTE           combined_alpha;
GX_COLOR           fill_color;
GX_RECTANGLE      *clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    /* Pick up clip rectangle.  */
    clip = context -> gx_draw_context_clip;

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    /* Pick up context fill color.  */
    fill_color = context -> gx_draw_context_brush.gx_brush_fill_color;

    getrowalpha = (UCHAR *)(pmp -> gx_pixelmap_data);
    getrowalpha += (pmp -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    getrowalpha += (clip -> gx_rectangle_left - xpos);

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        getalpha = getrowalpha;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            combined_alpha = (GX_UBYTE)((*getalpha++) * alpha / 255);
            blend_func(context, xval, yval, fill_color, combined_alpha);
        }

        getrowalpha += pmp -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_c_alpha_draw    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing with brush alpha      */
/*    of compressed alpha map file.                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    alpha                                 alpha value from 0 to 255     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_draw                            */
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
static VOID  _gx_display_driver_generic_alphamap_compressed_alpha_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp, GX_UBYTE alpha)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_UBYTE           count;
GX_UBYTE           falpha;
GX_COLOR           fill_color;
GX_RECTANGLE      *clip;
GX_UBYTE           combined_alpha;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    /* Pick up clip rectangle. */
    clip = context -> gx_draw_context_clip;

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    /* Pick up context fill color.  */
    fill_color = context -> gx_draw_context_brush.gx_brush_fill_color;

    get = (GX_CONST GX_UBYTE *)pmp -> gx_pixelmap_data;

    /* compressed with no alpha is a two-byte count and two-byte pixel value */

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pmp -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                get++;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row
     */

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pmp -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (GX_UBYTE)((count & 0x7f) + 1);
                falpha = *get++;

                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                        blend_func(context, xval, yval, fill_color, combined_alpha);
                    }
                    xval++;
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        combined_alpha = (GX_UBYTE)((*get) * alpha / 255);
                        blend_func(context, xval, yval, fill_color, combined_alpha);
                    }
                    get++;
                    xval++;
                }
            }
        }
        yval++;
    }
}
#endif /* GX_BRUSH_ALPHA_SUPPORT*/

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_raw_draw        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    alpha map file.                                                     */
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
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_draw                            */
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
static VOID  _gx_display_driver_generic_alphamap_raw_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp)
{
INT                xval;
INT                yval;
GX_UBYTE          *getrowalpha;
GX_CONST GX_UBYTE *getalpha;
GX_COLOR           fill_color;
GX_RECTANGLE      *clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    /* Pick up clip rectangle.  */
    clip = context -> gx_draw_context_clip;

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    /* Pick up context fill color.  */
    fill_color = context -> gx_draw_context_brush.gx_brush_fill_color;

    getrowalpha = (UCHAR *)(pmp -> gx_pixelmap_data);
    getrowalpha += (pmp -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    getrowalpha += (clip -> gx_rectangle_left - xpos);

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        getalpha = getrowalpha;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            blend_func(context, xval, yval, fill_color, *getalpha++);
        }

        getrowalpha += pmp -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_compressed_draw PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    alpha map file.                                                     */
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
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_draw                            */
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
static VOID  _gx_display_driver_generic_alphamap_compressed_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp)
{
INT                yval;
INT                xval;
GX_CONST GX_UBYTE *get;
GX_UBYTE           count;
GX_UBYTE           pixel;
GX_COLOR           fill_color;
GX_RECTANGLE      *clip;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    /* Pick up clip rectangle.  */
    clip = context -> gx_draw_context_clip;

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    /* Pick up context fill color.  */
    fill_color = context -> gx_draw_context_brush.gx_brush_fill_color;

    get = (GX_CONST GX_UBYTE *)pmp -> gx_pixelmap_data;

    /* compressed with no alpha is a two-byte count and two-byte pixel value */

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip -> gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pmp -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                count = (GX_UBYTE)((count & 0x7f) + 1);
                get++;      /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count;   /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
       to the enf of the last visible row
     */

    while (yval <= clip -> gx_rectangle_bottom)
    {
        xval = xpos;

        while (xval < xpos + pmp -> gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x80)
            {
                /* repeated value */
                count = (GX_UBYTE)((count & 0x7f) + 1);
                pixel = *get++;

                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        blend_func(context, xval, yval, fill_color, pixel);
                    }
                    xval++;
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;
                while (count--)
                {
                    if (xval >= clip -> gx_rectangle_left &&
                        xval <= clip -> gx_rectangle_right)
                    {
                        blend_func(context, xval, yval, fill_color, *get);
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
/*    _gx_display_driver_generic_alphamap_draw            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function blends the context fill color with the canvas         */
/*      background.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pmp                                   Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_generic_alphamap_compressed_alpha_draw           */
/*                                          Real display driver alphamap  */
/*                                            draw function               */
/*    _gx_display_driver_generic_alphamap_raw_alpha_draw                  */
/*                                          Real display driver alphamap  */
/*                                            draw function               */
/*    _gx_display_driver_generic_alphamap_compressed_draw                 */
/*                                          Real display driver alphamap  */
/*                                            draw function               */
/*    _gx_display_driver_generic_alphamap_raw_draw                        */
/*                                          Real display driver alphamap  */
/*                                            draw function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX default draw funtions                                          */
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
VOID  _gx_display_driver_generic_alphamap_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp)
{
#if defined GX_BRUSH_ALPHA_SUPPORT
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (alpha == 0)
    {
        /* Nothing to drawn. Just return. */
        return;
    }

    if (alpha != 0xff)
    {
        if (pmp -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            _gx_display_driver_generic_alphamap_compressed_alpha_draw(context, xpos, ypos, pmp, alpha);
        }
        else
        {
            _gx_display_driver_generic_alphamap_raw_alpha_draw(context, xpos, ypos, pmp, alpha);
        }
        return;
    }
#endif

    if (pmp -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        _gx_display_driver_generic_alphamap_compressed_draw(context, xpos, ypos, pmp);
    }
    else
    {
        _gx_display_driver_generic_alphamap_raw_draw(context, xpos, ypos, pmp);
    }
}

