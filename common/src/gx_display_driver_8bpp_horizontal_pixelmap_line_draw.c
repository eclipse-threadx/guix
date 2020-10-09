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

#if defined(GX_BRUSH_ALPHA_SUPPORT)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_blend          */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of uncompressed      */
/*    pixlemap file without alpha channel with brush alpha.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*    alpha                                 Alpha value                   */
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
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_blend(GX_DRAW_CONTEXT *context,
                                                                       INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
INT                offset;
INT                pic_width;
GX_CONST GX_UBYTE *get;
GX_PIXELMAP       *pixelmap;
VOID               (*blend_func)(GX_DRAW_CONTEXT *, INT, INT, GX_COLOR, GX_UBYTE);

    pixelmap = info -> pixelmap;

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    if (blend_func == GX_NULL)
    {
        return;
    }

    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = info -> current_pixel_ptr;

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /* get points to the start postion of this row. So we need to calculate its position. */
            blend_func(context, xval, y, *(get + offset), alpha);
            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_blend   */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of compressed        */
/*    pixlemap file with brush alpha.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*    alpha                                 Alpha value                   */
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
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_blend(GX_DRAW_CONTEXT *context,
                                                                              INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_UBYTE           pixel;
GX_PIXELMAP       *pixelmap;
VOID               (*blend_func)(GX_DRAW_CONTEXT *, INT, INT, GX_COLOR, GX_UBYTE);

    pixelmap = info -> pixelmap;
    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    if (blend_func == GX_NULL)
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        /* This means it's the draw operation. */
        /* Skip the invisible pixels.*/
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /*Repeat the draw operation to fill the whole dirty area.*/
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
                    pixel = *get++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            blend_func(context, xval, y, pixel, alpha);
                        }
                        xval++;
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
                            blend_func(context, xval, y, pixel, alpha);
                        }
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
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_blend        */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of uncompressed      */
/*    pixlemap file with alpha channel with brush alpha.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*    alpha                                 Alpha value                   */
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
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                         INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
GX_CONST GX_UBYTE *get;
GX_CONST GX_UBYTE *get_alpha;
GX_UBYTE           falpha;
GX_UBYTE           pixel;
GX_PIXELMAP       *pixelmap;
INT                pic_width;
INT                offset;
GX_UBYTE           combined_alpha;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    pixelmap = info -> pixelmap;

    if (blend_func == GX_NULL)
    {
        return;
    }

    pic_width = pixelmap -> gx_pixelmap_width;
    if ((info -> draw) && (xstart <= xend))
    {
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        get_alpha = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

        /*calculate the offset.*/
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /*get points to the start postion of this row. So we need to calculate its position.*/
            pixel = *(get + offset);
            falpha = *(get_alpha + offset);
            if (falpha)
            {
                combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                blend_func(context, xval, y, pixel, combined_alpha);
            }

            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
    info -> current_aux_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_alpha   */
/*        _blend                                                          */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of compressed        */
/*    pixlemap file with alpha channel.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*    alpha                                 Alpha value                   */
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
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                                    INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT              xval;
USHORT           count;
INT              start_pos;
GX_UBYTE         falpha;
GX_UBYTE         combined_alpha;
USHORT           pixel;
GX_CONST USHORT *get = GX_NULL;
GX_PIXELMAP     *pixelmap;
VOID             (*blend_func)(GX_DRAW_CONTEXT *, INT, INT, GX_COLOR, GX_UBYTE);

    pixelmap = info -> pixelmap;
    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calcualte draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /*Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;
            get = (GX_CONST USHORT *)info -> current_pixel_ptr;
            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get++;

                if (count & 0x8000)
                {
                    /* repeated value */
                    count = (USHORT)((count & 0x7fff) + 1u);
                    pixel = *get++;
                    falpha = (GX_UBYTE)((pixel & 0xff00) >> 8);
                    combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                    if (combined_alpha)
                    {
                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                blend_func(context, xval, y, (GX_COLOR)(pixel & 0xff), combined_alpha);
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
                    /* string of non-repeated values */
                    count++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            pixel = *get;
                            falpha = (GX_UBYTE)((pixel & 0xff00) >> 8);
                            combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                            blend_func(context, xval, y, (GX_COLOR)(pixel & 0xff), combined_alpha);
                        }
                        get++;
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
        get = (GX_CONST USHORT *)info -> current_pixel_ptr;

        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;
            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1);
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
}

#endif /* GX_BRUSH_ALPHA_SUPPORT */

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_write          */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_write(GX_DRAW_CONTEXT *context,
                                                                       INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
INT                offset;
INT                pic_width;
GX_CONST GX_UBYTE *get;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = info -> current_pixel_ptr;
        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + xstart;

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /* get points to the start postion of this row. So we need to calculate its position. */
            *put++ = *(get + offset);
            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_write   */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                              INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_UBYTE           pixel;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + start_pos;

        /*Repeat the draw operation to fill the whole dirty area.*/
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
                    pixel = *get++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            *put = pixel;
                        }
                        xval++;
                        put++;
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
                            *put = pixel;
                        }
                        xval++;
                        put++;
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

    /*This line is drawn. cache the pointer for next line draw.*/
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_line_pixelmap_transparent_write  */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_transparent_write(GX_DRAW_CONTEXT *context,
                                                                               INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
INT                offset;
INT                pic_width;
GX_CONST GX_UBYTE *get;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;
GX_UBYTE           pixel;

    pixelmap = info -> pixelmap;
    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = info -> current_pixel_ptr;
        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + xstart;

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /* get points to the start postion of this row. So we need to calculate its position. */
            pixel = *(get + offset);
            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }

            if (pixel != pixelmap -> gx_pixelmap_transparent_color)
            {
                *put = pixel;
            }
            put++;
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_        */
/*    transparent_write                                                   */
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
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
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
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_transparent_write(GX_DRAW_CONTEXT *context,
                                                                                          INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_UBYTE           pixel;
GX_UBYTE          *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calcualte draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_UBYTE *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + start_pos;


        /*Repeat the draw operation to fill the whole dirty area.*/
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
                    pixel = *get++;
                    if (pixel != pixelmap -> gx_pixelmap_transparent_color)
                    {
                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                *put = pixel;
                            }
                            xval++;
                            put++;
                        }
                    }
                    else
                    {
                        xval += count;
                        put += count;
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
                            if (pixel != pixelmap -> gx_pixelmap_transparent_color)
                            {
                                *put = pixel;
                            }
                        }
                        xval++;
                        put++;
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

    /* This line is drawn. cache the pointer for next line draw.*/
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_write        */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*    alpha                                 Alpha value                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*    [gx_display_driver_pixel_write]       Basic display driver pixel    */
/*                                            write function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_write(GX_DRAW_CONTEXT *context,
                                                                         INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
GX_CONST GX_UBYTE *get;
GX_CONST GX_UBYTE *get_alpha;
GX_UBYTE           alpha;
GX_UBYTE           pixel;
GX_PIXELMAP       *pixelmap;
INT                pic_width;
INT                offset;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    pixelmap = info -> pixelmap;

    if (blend_func == GX_NULL)
    {
        return;
    }

    pic_width = pixelmap -> gx_pixelmap_width;
    if ((info -> draw) && (xstart <= xend))
    {
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        get_alpha = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

        /*calculate the offset.*/
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /*get points to the start postion of this row. So we need to calculate its position.*/
            pixel = *(get + offset);
            alpha = *(get_alpha + offset);

            blend_func(context, xval, y, pixel, alpha);

            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
    info -> current_aux_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_alpha   */
/*        _write                                                          */
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
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
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
/*    _gx_display_driver_8bpp_horizontal_pixelmap_line_draw               */
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
static VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                    INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT              xval;
USHORT           count;
INT              start_pos;
GX_UBYTE         alpha;
USHORT           pixel;
GX_CONST USHORT *get = GX_NULL;
GX_PIXELMAP     *pixelmap;
VOID             (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    pixelmap = info -> pixelmap;
    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        /* This means it's the draw operation. */
        /* Skip the invisible pixels.*/
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /*Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;
            get = (GX_CONST USHORT *)info -> current_pixel_ptr;
            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get++;

                if (count & 0x8000)
                {
                    /* repeated value */
                    count = (USHORT)((count & 0x7fff) + 1u);
                    pixel = *get++;
                    alpha = (GX_UBYTE)((pixel & 0xff00) >> 8);

                    if (alpha)
                    {
                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                blend_func(context, xval, y, (GX_COLOR)(pixel & 0xff), alpha);
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
                    /* string of non-repeated values */
                    count++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            pixel = *get;
                            alpha = (GX_UBYTE)((pixel & 0xff00) >> 8);
                            if (alpha)
                            {
                                blend_func(context, xval, y, (GX_COLOR)(pixel & 0xff), alpha);
                            }
                        }
                        get++;
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
        get = (GX_CONST USHORT *)info -> current_pixel_ptr;

        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get++;
            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1);
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

    /*This line is drawn. cache the pointer for next line draw.*/
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_draw               PORTABLE C      */
/*                                                           6.1          */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_8bit_horizontal_pixelmap_line_raw_write         */
/*                                          Draw raw pixelmap             */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_write  */
/*                                          Draw pixelmap with            */
/*                                            compression                 */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_transparent_write */
/*                                          Draw pixelmap with            */
/*                                            transparency                */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_       */
/*     transparent_write                                                  */
/*                                          Draw pixelmap with            */
/*                                            transparency and compression*/
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
VOID _gx_display_driver_8bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                           INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{

    if (info -> pixelmap == GX_NULL)
    {
        return;
    }

    if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* has both compression and transparent */
            _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_transparent_write(context, xstart, xend, y, info);
        }
        else
        {
            /* transparent, no compression */
            _gx_display_driver_8bpp_horizontal_pixelmap_line_transparent_write(context, xstart, xend, y, info);
        }
    }
    else
    {
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no transparency */
            _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        }
        else
        {
            /* no compression or transaprency */
            _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_write(context, xstart, xend, y, info);
        }
    }


    /*Current pixelmap has gone over, so the offset pointer should be reset.*/
    if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
    {
        info -> current_pixel_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_data;
        info -> current_aux_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_aux_data;
    }

    return;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_pixelmap_draw               PORTABLE C      */
/*                                                           6.1          */
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
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line end           */
/*    y                                     y-coord of line top           */
/*    info                                  GX_FILL_PIXELMAP_INFO struct  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_       */
/*     alpha_blend                                                        */
/*                                          Blend pixelmap with           */
/*                                            compression and alpha       */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_blend       */
/*                                          Blend pixelmap with alpha     */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_blend  */
/*                                          Blend pixelmap with           */
/*                                            compression                 */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_blend         */
/*                                          Blend raw pixelmap            */
/*     _gx_display_driver_8bit_horizontal_pixelmap_line_raw_write         */
/*                                          Draw draw pixelmap            */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_write       */
/*                                          Draw pixelmap with alpha      */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_write  */
/*                                          Draw pixelmap with            */
/*                                            compression                 */
/*     _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_       */
/*     alpha_write                                                        */
/*                                          DRaw pxielmap with            */
/*                                            compression and alpha       */
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
VOID _gx_display_driver_332rgb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                             INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
#if defined GX_BRUSH_ALPHA_SUPPORT
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (alpha == 0)
    {
        /* Nothing to drawn. Just return. */
        return;
    }

    if (info -> pixelmap == GX_NULL)
    {
        /* No pixelmap info here.So just return. */
        return;
    }

    if (alpha != 0xff)
    {
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_alpha_blend(context, xstart, xend, y, info, alpha);
            }
            else
            {
                /* alpha, no compression */
                _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_blend(context, xstart, xend, y, info, alpha);
            }
        }
        else
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_blend(context, xstart, xend, y, info, alpha);
            }
            else
            {
                _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_blend(context, xstart, xend, y, info, alpha);
            }
        }
    }
    else
    {
#endif
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_alpha_write(context, xstart, xend, y, info);
            }
            else
            {
                /* alpha, no compression */
                _gx_display_driver_8bpp_horizontal_pixelmap_line_alpha_write(context, xstart, xend, y, info);
            }
        }
        else
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                _gx_display_driver_8bpp_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
            }
            else
            {
                _gx_display_driver_8bpp_horizontal_pixelmap_line_raw_write(context, xstart, xend, y, info);
            }
        }
#if defined GX_BRUSH_ALPHA_SUPPORT
    }
#endif

    /*Current pixelmap has gone over, so the offset pointer should be reset.*/
    if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
    {
        info -> current_pixel_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_data;
        info -> current_aux_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_aux_data;
    }

    return;
}

