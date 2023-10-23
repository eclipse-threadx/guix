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

#if defined(GX_ARC_DRAWING_SUPPORT)

#if defined(GX_BRUSH_ALPHA_SUPPORT)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_blend        */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file without alpha channel with brush alpha.               */
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
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
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
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_blend(GX_DRAW_CONTEXT *context,
                                                                         INT xstart, INT xend, INT y,
                                                                         GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT              xval;
INT              offset;
INT              pic_width;
GX_CONST USHORT *get;
USHORT           pixel;
GX_PIXELMAP     *pixelmap;
VOID             (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE balpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    if (blend_func == GX_NULL)
    {
        return;
    }

    pixelmap = info -> pixelmap;
    pic_width = pixelmap -> gx_pixelmap_width;

    /* Pick the data pointer to the current row. */
    get = (GX_CONST USHORT *)info -> current_pixel_ptr;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            pixel = *(get + offset);
            blend_func(context, xval, y, pixel, alpha);
            offset++;

            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* Update data pointer for next row.*/
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(USHORT);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_blend      */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
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
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
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
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                           INT xstart, INT xend, INT y,
                                                                           GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
GX_CONST USHORT   *get;
GX_CONST GX_UBYTE *getalpha;
USHORT             color;
GX_UBYTE           falpha;
GX_UBYTE           combined_alpha;
GX_PIXELMAP       *pixelmap;
INT                pic_width;
INT                offset;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    if (blend_func == GX_NULL)
    {
        return;
    }

    pixelmap = info -> pixelmap;
    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Pick the data pointer to the current row. */
        get = (GX_CONST USHORT *)info -> current_pixel_ptr;
        getalpha = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

        /* Calculate the map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            color = *(get + offset);
            falpha = *(getalpha + offset);

            if (falpha)
            {
                combined_alpha = (GX_UBYTE)(falpha * alpha / 255);

                blend_func(context, xval, y, color, combined_alpha);
            }

            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* Update data pointers for next row. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(USHORT);
    info -> current_aux_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_blend */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file without alpha channel with brush_alpha.               */
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
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
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
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_blend(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT              start_pos;
INT              xval;
USHORT           count;
USHORT           pixel;
GX_CONST USHORT *get = GX_NULL;
GX_PIXELMAP     *pixelmap;
VOID             (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);

    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;
    pixelmap = info -> pixelmap;
    if (blend_func == GX_NULL)
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        while (start_pos <= xend)
        {
            xval = start_pos;

            /*Start from where we need to repeat.*/
            get = (GX_CONST USHORT *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get++;
                if (count & 0x8000)
                {
                    count = (USHORT)((count & 0x7fff) + 1);
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

    /* Update data pointer for next row. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_c_a_blend        */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
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
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
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
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                                      INT xstart, INT xend, INT y,
                                                                                      GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
GX_UBYTE           count;
INT                start_pos;
GX_UBYTE           falpha;
GX_UBYTE           combined_alpha;
USHORT             pixel;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_CONST USHORT   *getpixel;
GX_PIXELMAP       *pixelmap;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    pixelmap = info -> pixelmap;
    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate the draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        while (start_pos <= xend)
        {
            xval = start_pos;

            /*Start from where we need to repeat.*/
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1u);
                    falpha = *(get + 1);
                    combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                    if (combined_alpha)
                    {
                        get += 2;
                        getpixel = (GX_CONST USHORT *)get;
                        pixel = *getpixel;
                        get += 2;

                        while (count--)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                blend_func(context, xval, y, pixel, combined_alpha);
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
                else
                {
                    count++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            falpha = *(get + 1);
                            combined_alpha = (GX_UBYTE)(falpha * alpha / 255);
                            get += 2;
                            getpixel = (USHORT *)get;
                            pixel = *getpixel;
                            get += 2;
                            blend_func(context, xval, y, pixel, combined_alpha);
                        }
                        else
                        {
                            get += 4;
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
        /* Skip this line. */
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get;
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

    /* Update data pointer for the next line. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

#endif /* GX_BRUSH_ALPHA_SUPPORT */

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_write        */
/*                                                        PORTABLE C      */
/*                                                           6.X          */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_write(GX_DRAW_CONTEXT *context,
                                                                         INT xstart, INT xend, INT y,
                                                                         GX_FILL_PIXELMAP_INFO *info)
{
INT              xval;
INT              offset;
INT              pic_width;
GX_CONST USHORT *get = GX_NULL;
USHORT          *put;
GX_PIXELMAP     *pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;

    /* Pickup data pointer for the current line. */
    get = (GX_CONST USHORT *)info -> current_pixel_ptr;

    if ((info -> draw) && (xstart <= xend))
    {
        put = (USHORT *)context -> gx_draw_context_memory;
        GX_CALCULATE_PUTROW(put, xstart, y, context);

        /*calculate the offset.*/
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            *put++ = *(get + offset);
            offset++;

            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* Update data pointer for the next line. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(USHORT);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_write      */
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
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
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
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_write(GX_DRAW_CONTEXT *context,
                                                                           INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
GX_CONST USHORT   *get;
GX_CONST GX_UBYTE *getalpha;
USHORT             color;
GX_UBYTE           alpha;
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
        /* Pick up data pointers to the current line. */
        get = (GX_CONST USHORT *)info -> current_pixel_ptr;
        getalpha = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

        /* calculate map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            color = *(get + offset);
            alpha = *(getalpha + offset);

            blend_func(context, xval, y, color, alpha);

            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* Update data pointers for the next line. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(USHORT);
    info -> current_aux_ptr += (UINT)pic_width * sizeof(GX_UBYTE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_write */
/*                                                        PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                                INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT              start_pos;
INT              xval;
USHORT           count;
USHORT           pixel;
GX_CONST USHORT *get = GX_NULL;
USHORT          *put;
GX_PIXELMAP     *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (USHORT *)context -> gx_draw_context_memory;
        GX_CALCULATE_PUTROW(put, start_pos, y, context);

        /*Repeat the draw operation to fill the whole dirty area.*/
        while (start_pos <= xend)
        {
            xval = start_pos;

            /*Start from where we need to repeat.*/
            get = (GX_CONST USHORT *)info -> current_pixel_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get++;
                if (count & 0x8000)
                {
                    count = (USHORT)((count & 0x7fff) + 1);
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

    /* Update data pointer for the next line. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_c_a_write        */
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
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
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
static VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                      INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
GX_UBYTE           count;
INT                start_pos;
GX_UBYTE           alpha;
USHORT             pixel;
GX_CONST GX_UBYTE *get = GX_NULL;
GX_CONST USHORT   *getpixel;
GX_PIXELMAP       *pixelmap;
VOID               (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    pixelmap = info -> pixelmap;
    blend_func = context -> gx_draw_context_display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /* Repeat the draw operation to fill the whole dirty area. */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1u);
                    alpha = *(get + 1);
                    if (alpha)
                    {
                        get += 2;

                        getpixel = (GX_CONST USHORT *)get;
                        pixel = *getpixel;
                        get += 2;

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
                        get += 4;
                        xval += count;
                    }
                }
                else
                {
                    count++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            alpha = *(get + 1);
                            get += 2;
                            getpixel = (USHORT *)get;
                            pixel = *getpixel;
                            get += 2;
                            blend_func(context, xval, y, pixel, alpha);
                        }
                        else
                        {
                            get += 4;
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
        /* Just do skip operation here. */
        xval = 0;
        get = (GX_CONST GX_UBYTE *)info -> current_pixel_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get;
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

    /* Update data pinter for the next line. */
    info -> current_pixel_ptr = (GX_UBYTE *)get;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_horizontal_pixelmap_line_draw             */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb screen driver pixelmap drawing function that handles         */
/*    compressed or uncompress, with or without alpha channel.            */
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
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_c_a_blend       */
/*                                          Real pixelmap blend function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_blend     */
/*                                          Real pixelmap blend function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_c_blend         */
/*                                          Real pixelmap blend function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_blend       */
/*                                          Real pixelmap blend function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_c_a_write       */
/*                                          Real pixelmap write function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_write     */
/*                                          Real pixelmap write function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_c_write         */
/*                                          Real pixelmap write function  */
/*     _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_write       */
/*                                          Real pixelmap write function  */
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
VOID _gx_display_driver_565rgb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                             INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
#if defined GX_BRUSH_ALPHA_SUPPORT
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if ((alpha == 0) || (info -> pixelmap == GX_NULL))
    {
        /* Nothing to drawn. Just return. */
        return;
    }

    if (alpha != 0xff)
    {

        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* has both compression and alpha */
                _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_alpha_blend(context, xstart, xend, y, info, alpha);
            }
            else
            {
                /* alpha, no compression */
                _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_blend(context, xstart, xend, y, info, alpha);
            }
        }
        else
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
            {
                /* compressed with no alpha */
                _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_blend(context, xstart, xend, y, info, alpha);
            }
            else
            {
                /* no compression or alpha */
                _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_blend(context, xstart, xend, y, info, alpha);
            }
        }

        /* Data pointer goes to the end of the fill map, move it to the start again. */
        if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
        {
            info -> current_pixel_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_data;
            info -> current_aux_ptr = (GX_UBYTE *)info -> pixelmap -> gx_pixelmap_aux_data;
        }
        return;
    }
#endif

    if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* has both compression and alpha */
            _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_alpha_write(context, xstart, xend, y, info);
        }
        else
        {
            /* alpha, no compression */
            _gx_display_driver_565rgb_horizontal_pixelmap_line_alpha_write(context, xstart, xend, y, info);
        }
    }
    else
    {
        if (info -> pixelmap ->  gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            /* compressed with no alpha */
            _gx_display_driver_565rgb_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        }
        else
        {
            /* no compression or alpha */
            _gx_display_driver_565rgb_horizontal_pixelmap_line_raw_write(context, xstart, xend, y, info);
        }
    }

    /* Data pointers goes to the end of full map, move it to the start again. */
    if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
    {
        info -> current_pixel_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_data;
        info -> current_aux_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_aux_data;
    }
}
#endif /*GX_ARC_DRAWING_SUPPORT*/

