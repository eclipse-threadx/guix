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
/*    _gx_display_driver_24xrgb_pixelmap_raw_blend        PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
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
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_24xrgb_pixelmap_raw_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT           yval;
INT           xval;
INT           draw_width;
GX_COLOR      color;
ULONG        *get;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    get = (GX_COLOR *)pixelmap -> gx_pixelmap_data;
    get +=  pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    get += (clip -> gx_rectangle_left - xpos);

    draw_width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            color = *get & 0x00ffffff;
            get++;
            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, color, alpha);
        }
        get += pixelmap -> gx_pixelmap_width - draw_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_alpha_blend      PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of uncompressed      */
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
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_24xrgb_pixelmap_alpha_blend(GX_DRAW_CONTEXT *context,
                                                           INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
int           xval;
int           yval;
int           color;
int           width;
ULONG        *get;
UCHAR         alpha_value;
ULONG         combined_alpha;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    get = (GX_COLOR *)(pixelmap -> gx_pixelmap_data + (INT)sizeof(GX_COLOR) * pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos));
    get += (clip -> gx_rectangle_left - xpos);

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            alpha_value = (UCHAR)(*get >> 24);

            if (alpha_value)
            {
                combined_alpha = alpha_value;
                combined_alpha *= alpha;
                combined_alpha /= 255;

                if (combined_alpha)
                {
                    color = *get & 0x00ffffff;
                    _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, (GX_COLOR)color, (GX_UBYTE)combined_alpha);
                }
            }
            get++;
        }
        get += pixelmap -> gx_pixelmap_width - width;
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_palette_pixelmap_blend      PORTABLE C    */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file without transparent of palette pixelmap.              */
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
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_24xrgb_palette_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                                             INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT           xval;
INT           yval;
GX_COLOR      color;
INT           width;
GX_UBYTE     *get;
GX_COLOR     *palette;
GX_UBYTE      r;
GX_UBYTE      g;
GX_UBYTE      b;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    get = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data + pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos));
    get += (clip -> gx_rectangle_left - xpos);

    palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

    width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;


        for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
        {
            for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
            {
                r = (GX_UBYTE)REDVAL_32BPP(palette[*get]);
                g = (GX_UBYTE)GREENVAL_32BPP(palette[*get]);
                b = (GX_UBYTE)BLUEVAL_32BPP(palette[*get++]);

                color = (GX_COLOR)ASSEMBLECOLOR_24BPP(r, g, b);

                _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, color, alpha);
            }

            get += pixelmap -> gx_pixelmap_width - width;
        }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_palette_pixelmap_transparent_blend        */
/*                                                         PORTABLE C     */
/*                                                           6.1.7        */
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
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_24xrgb_palette_pixelmap_transparent_blend(GX_DRAW_CONTEXT *context,
                                                                         INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
 {
INT           xval;
INT           yval;
GX_COLOR      color;
INT           width;
GX_UBYTE     *get;
GX_COLOR     *palette;
GX_UBYTE      r;
GX_UBYTE      g;
GX_UBYTE      b;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

     get = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data + pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos));
     get += (clip -> gx_rectangle_left - xpos);

     palette = (GX_COLOR *)pixelmap -> gx_pixelmap_aux_data;

     width = clip -> gx_rectangle_right - clip -> gx_rectangle_left + 1;

     for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
     {
         for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
         {
             if ((*get) != pixelmap -> gx_pixelmap_transparent_color)
             {
                 r = (GX_UBYTE)REDVAL_32BPP(palette[*get]);
                 g = (GX_UBYTE)GREENVAL_32BPP(palette[*get]);
                 b = (GX_UBYTE)BLUEVAL_32BPP(palette[*get]);
                 color = (GX_COLOR)ASSEMBLECOLOR_24BPP(r, g, b);
                 _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, color, alpha);
             }
             get++;
         }

         get += pixelmap -> gx_pixelmap_width - width;
     }
 }

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_4444argb_pixelmap_alpha_blend             */
/*                                                            PORTABLE C  */
/*                                                           6.1          */
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
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
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
static VOID _gx_display_driver_24xrgb_4444argb_pixelmap_alpha_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT           skipcount;
INT           xval;
INT           yval;
USHORT       *getrow;
GX_CONST USHORT *get;
UCHAR         falpha;
GX_UBYTE      combined_alpha;
ULONG         pixel;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    /* calculate how many pixels to skip */
    skipcount = (pixelmap -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    skipcount += (clip -> gx_rectangle_left - xpos);
    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            /* 0x000f- -> b , 0x00f0- -> g , 0x0f00- -> r , 0xf000- -> a */
            /*4444bgra - ->  565rgb*/
            falpha = (UCHAR)(((*get) & 0xf000) >> 8);
            if (falpha)
            {
                falpha = (GX_UBYTE)(falpha | (falpha >> 4));
                pixel = (GX_COLOR)((((*get) & 0x0f00) << 12) | (((*get) & 0x00f0) << 8) | (((*get) & 0x000f) << 4));
                combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, combined_alpha);
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
/*    _gx_display_driver_24xrgb_565rgb_pixelmap_alpha_blend               */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
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
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
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
static VOID _gx_display_driver_24xrgb_565rgb_pixelmap_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                  INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT             skipcount;
INT             xval;
INT             yval;
GX_CONST GX_UBYTE *getalpha;
GX_CONST USHORT   *get;
USHORT         *getrow;
GX_UBYTE       *getrowalpha;
GX_UBYTE        r;
GX_UBYTE        g;
GX_UBYTE        b;
GX_COLOR        pixel;
GX_UBYTE        falpha;
GX_UBYTE        combined_alpha;

GX_RECTANGLE   *clip = context -> gx_draw_context_clip;

    skipcount = (pixelmap -> gx_pixelmap_width) * (clip -> gx_rectangle_top - ypos);
    skipcount += (clip -> gx_rectangle_left - xpos);
    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += skipcount;

    getrowalpha = (GX_UBYTE *)(pixelmap -> gx_pixelmap_aux_data);
    getrowalpha += skipcount;

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;
        getalpha = getrowalpha;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            falpha = *getalpha++;
            if (falpha)
            {
                combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                pixel = *get;
                r = (GX_UBYTE)(((USHORT)pixel & 0xf800) >> 8);
                g = (GX_UBYTE)(((USHORT)pixel & 0x07e0) >> 3);
                b = (GX_UBYTE)(((USHORT)pixel & 0x001f) << 3);
                pixel = (GX_COLOR)ASSEMBLECOLOR_32BPP(r, g, b);
                _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, combined_alpha);
            }
            get++;
        }
        getrow += pixelmap -> gx_pixelmap_width;
        getrowalpha += pixelmap -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_565rgb_pixelmap_raw_blend PORTABLE C      */
/*                                                           6.1          */
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
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixel_blend Display driver basic function */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend                            */
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
static VOID _gx_display_driver_24xrgb_565rgb_pixelmap_raw_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
INT           xval;
INT           yval;
USHORT       *getrow;
GX_CONST USHORT *get;
GX_COLOR      pixel;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;

    getrow = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    for (yval = clip -> gx_rectangle_top; yval <= clip -> gx_rectangle_bottom; yval++)
    {
        get = getrow;

        for (xval = clip -> gx_rectangle_left; xval <= clip -> gx_rectangle_right; xval++)
        {
            pixel = (GX_COLOR)ASSEMBLECOLOR_32BPP(REDVAL_16BPP(*get) << 3,
                                                  GREENVAL_16BPP(*get) << 2,
                                                  BLUEVAL_16BPP(*get) << 3);
            _gx_display_driver_24xrgb_pixel_blend(context, xval, yval, pixel, alpha);
            get++;
        }
        getrow += pixelmap -> gx_pixelmap_width;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_pixelmap_blend            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32xrgb format screen driver pixelmap blending function that         */
/*    handles compressed or uncompress, with or without alpha channel.    */
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
/*    _gx_display_driver_24xrgb_palette_pixelmap_transparent_blend        */
/*                                          Real pixelmap draw function.  */
/*    _gx_display_driver_24xrgb_palette_pixelmap_blend                    */
/*                                          Real pixelmap draw function.  */
/*    _gx_display_driver_24xrgb_4444argb_pixelmap_alpha_blend             */
/*                                          Real pixelmap draw function.  */
/*    _gx_display_driver_24xrgb_565rgb_pixelmap_alpha_blend               */
/*                                          Real pixelmap draw function.  */
/*    _gx_display_driver_24xrgb_565rgb_pixelmap_raw_blend                 */
/*                                          Real pixelmap draw function.  */
/*    _gx_display_driver_24xrgb_pixelmap_alpha_blend                      */
/*                                          Real pixelmap draw function.  */
/*    _gx_display_driver_24xrgb_pixelmap_raw_blend                        */
/*                                          Real pixelmap draw function.  */
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
VOID _gx_display_driver_24xrgb_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                              INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha)
{
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        return;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
        {
            _gx_display_driver_24xrgb_palette_pixelmap_transparent_blend(context, xpos, ypos, pixelmap, alpha);
        }
        else
        {
            _gx_display_driver_24xrgb_palette_pixelmap_blend(context, xpos, ypos, pixelmap, alpha);
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        _gx_display_driver_24xrgb_4444argb_pixelmap_alpha_blend(context, xpos, ypos, pixelmap, alpha);
        break;

    case GX_COLOR_FORMAT_565RGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            _gx_display_driver_24xrgb_565rgb_pixelmap_alpha_blend(context, xpos, ypos, pixelmap, alpha);
        }
        else
        {
            _gx_display_driver_24xrgb_565rgb_pixelmap_raw_blend(context, xpos, ypos, pixelmap, alpha);
        }
        break;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
        if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            _gx_display_driver_24xrgb_pixelmap_alpha_blend(context, xpos, ypos, pixelmap, alpha);

        }
        else
        {
            _gx_display_driver_24xrgb_pixelmap_raw_blend(context, xpos, ypos, pixelmap, alpha);
        }
        break;
    }

    return;
}
