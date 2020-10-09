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
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_blend       */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles blending of uncompressed      */
/*    pixelmap file with alpha channel by lines.                          */
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
/*    _gx_display_driver_32argb_pixel_blend Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                          INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
GX_COLOR           color;
GX_CONST GX_COLOR *get;
GX_PIXELMAP       *pixelmap;
GX_VALUE           pic_width;
GX_VALUE           offset;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;

        /* Calculate the map offset in x-axis. */
        offset = (GX_VALUE)(info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /*get points to the start postion of this row. So we need to calculate its position.*/
            color = *(get + offset);

            if (color & 0xff000000)
            {
                _gx_display_driver_32argb_pixel_blend(context, xval, y, color, alpha);
            }
            offset++;
            if (offset >= pic_width)
            {
                offset = (GX_VALUE)(offset - pic_width);
            }
        }
    }

    /*This line is drawn. Update the pointer position for next row.*/
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_COLOR);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_blend         */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of raw pixlemap file  */
/*      with brush alpha by line.                                         */
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
/*    _gx_display_driver_32argb_pixel_blend Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_blend(GX_DRAW_CONTEXT *context,
                                                                        INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
INT                offset;
INT                pic_width;
GX_CONST GX_COLOR *get;
GX_COLOR           pixel;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;

        /* Calculate map offset in x-axis. */
        offset = (info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /* get points to the start postion of this row. So we need to calculate its position. */
            pixel = *(get + offset);
            _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, alpha);
            offset++;
            if (offset >= pic_width)
            {
                offset -= pic_width;
            }
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_COLOR);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_blend  */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file without alpha channel with brush alpha by line.       */
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
/*    _gx_display_driver_32argb_pixel_blend Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_blend(GX_DRAW_CONTEXT *context,
                                                                               INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_PIXELMAP       *pixelmap;
GX_CONST GX_COLOR *get = GX_NULL;
GX_CONST GX_UBYTE *get_count = GX_NULL;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* This means it's the draw operation. */
        /* Skip the invisible pixels.*/
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /* Repeat the draw operation to fill the whole dirty area. */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
            get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get_count++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);

                    pixel = *get++;
                    while (count--)
                    {
                        if (xval >= xstart && xval <= xend)
                        {
                            _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, alpha);
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
                            _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, alpha);
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
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
        get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;
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
    info -> current_aux_ptr = (GX_UBYTE *)get_count;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_c_a_blend         */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file with alpha channel with brush alpha for one line.     */
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
/*    _gx_display_driver_32argb_pixel_blend Basic display driver pixel    */
/*                                            blend function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_alpha_blend(GX_DRAW_CONTEXT *context,
                                                                                     INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info, GX_UBYTE alpha)
{
INT                xval;
GX_UBYTE           count;
INT                start_pos;
GX_COLOR           pixel;
GX_CONST GX_COLOR *get = GX_NULL;
GX_CONST GX_UBYTE *get_count = GX_NULL;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /* Repeat the draw operation to fill the whole dirty area. */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
            get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get_count++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    pixel = *get++;
                    while (count--)
                    {
                        if (pixel & 0xff000000)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, alpha);
                            }
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
                        if (pixel & 0xff000000)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, alpha);
                            }
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
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
        get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;
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
    info -> current_aux_ptr = (GX_UBYTE *)get_count;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_write       */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of uncompressed       */
/*    pixlemap file with alpha channel for one line.                      */
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
/*    _gx_display_driver_32argb_pixel_blend Basic display driver pixel    */
/*                                            blend function              */
/*    _gx_display_driver_32argb_pixel_write Basic display driver pixel    */
/*                                            write function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_write(GX_DRAW_CONTEXT *context,
                                                                          INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
GX_COLOR           color;
GX_CONST GX_COLOR *get;
UCHAR              alpha_value;
GX_PIXELMAP       *pixelmap;
GX_VALUE           pic_width;
GX_VALUE           offset;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;

        /* calculate the offset. */
        offset = (GX_VALUE)(info -> x_offset % pic_width);

        for (xval = xstart; xval <= xend; xval++)
        {
            /* get points to the start postion of this row. So we need to calculate its position. */
            color = *(get + offset);
            alpha_value = (GX_UBYTE)((color >> 24) & 0xff);
            if (alpha_value)
            {
                if (alpha_value == 255)
                {
                    _gx_display_driver_32bpp_pixel_write(context, xval, y, color);
                }
                else
                {
                    _gx_display_driver_32argb_pixel_blend(context, xval, y, color, 0xff);
                }
            }
            offset++;
            if (offset >= pic_width)
            {
                offset = (GX_VALUE)(offset - pic_width);
            }
        }
    }

    /* This line is drawn. Update the pointer position for next row. */
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_COLOR);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_write         */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of raw pixlemap file  */
/*    for one line.                                                       */
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
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_write(GX_DRAW_CONTEXT *context,
                                                                        INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
INT                offset;
INT                pic_width;
GX_CONST GX_COLOR *get;
GX_COLOR          *put;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    pic_width = pixelmap -> gx_pixelmap_width;

    if ((info -> draw) && (xstart <= xend))
    {
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
        put = (GX_COLOR *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + xstart;

        /* calculate the offset. */
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
    info -> current_pixel_ptr += (UINT)pic_width * sizeof(GX_COLOR);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_write  */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles writing of compressed         */
/*    pixlemap file without alpha channel for one line.                   */
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
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_write(GX_DRAW_CONTEXT *context,
                                                                               INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                start_pos;
INT                xval;
GX_UBYTE           count;
GX_COLOR           pixel;
GX_CONST GX_COLOR *get = GX_NULL;
GX_COLOR          *put;
GX_CONST GX_UBYTE *get_count = GX_NULL;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        put = (GX_COLOR *)context -> gx_draw_context_memory;
        put += y * context -> gx_draw_context_pitch + start_pos;

        /* Repeat the draw operation to fill the whole dirty area. */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
            get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get_count++;
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
        /* Just do skip operation here. */
        xval = 0;
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
        get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;
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
    info -> current_aux_ptr = (GX_UBYTE *)get_count;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_c_a_write         */
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
/*    _gx_display_driver_32argb_pixel_blend Basic display driver pixel    */
/*                                            blend function              */
/*    _gx_display_driver_32argb_pixel_write Basic display driver pixel    */
/*                                            write function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
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
static VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_alpha_write(GX_DRAW_CONTEXT *context,
                                                                                     INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info)
{
INT                xval;
GX_UBYTE           count;
INT                start_pos;
GX_UBYTE           alpha_value;
GX_COLOR           pixel;
GX_CONST GX_COLOR *get = GX_NULL;
GX_CONST GX_UBYTE *get_count = GX_NULL;
GX_PIXELMAP       *pixelmap;

    pixelmap = info -> pixelmap;

    if ((info -> draw) && (xstart <= xend))
    {
        /* Calculate draw start position. */
        start_pos = xstart - (info -> x_offset % pixelmap -> gx_pixelmap_width);

        /* Repeat the draw operation to fill the whole dirty area. */
        while (start_pos <= xend)
        {
            xval = start_pos;

            /* Start from where we need to repeat. */
            get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
            get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;

            while (xval < start_pos + pixelmap -> gx_pixelmap_width)
            {
                count = *get_count++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    pixel = *get++;
                    alpha_value = (GX_UBYTE)((pixel >> 24) & 0xff);
                    while (count--)
                    {
                        if (alpha_value)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, 0xff);
                            }
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
                        alpha_value = (GX_UBYTE)((pixel >> 24) & 0xff);
                        if (alpha_value)
                        {
                            if (xval >= xstart && xval <= xend)
                            {
                                if (alpha_value == 0xff)
                                {
                                    _gx_display_driver_32bpp_pixel_write(context, xval, y, pixel);
                                }
                                else
                                {
                                    _gx_display_driver_32argb_pixel_blend(context, xval, y, pixel, 0xff);
                                }
                            }
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
        get = (GX_CONST GX_COLOR *)info -> current_pixel_ptr;
        get_count = (GX_CONST GX_UBYTE *)info -> current_aux_ptr;
        while (xval < pixelmap -> gx_pixelmap_width)
        {
            count = *get_count++;
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
    info -> current_aux_ptr = (GX_UBYTE *)get_count;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32bpp_horizontal_pixelmap_line_draw              */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32bpp screen driver horizontal pixelmap line drawing function that  */
/*    handles compressed or uncompress, with or without alpha channel.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of line left          */
/*    xend                                  x-coord of line right         */
/*    y                                     y-coord of line top           */
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_c_a_write        */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_write      */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_write */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_write        */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_c_a_blend        */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_a_blend          */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_c_blend          */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
/*     _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_blend        */
/*                                          Real display driver pixelmap  */
/*                                            line draw function          */
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
VOID _gx_display_driver_32bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
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
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /* compressed*/
                _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_alpha_blend(context, xstart, xend, y, info, alpha);
            }
            else
            {
                /* compressed*/
                _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_blend(context, xstart, xend, y, info, alpha);
            }
        }
        else
        {
            if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /* no compression */
                _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_blend(context, xstart, xend, y, info, alpha);
            }
            else
            {
                /* no compression */
                _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_blend(context, xstart, xend, y, info, alpha);
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

#endif

    if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            /* compressed*/
            _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_alpha_write(context, xstart, xend, y, info);
        }
        else
        {
            /* compressed*/
            _gx_display_driver_32bpp_horizontal_pixelmap_line_compressed_write(context, xstart, xend, y, info);
        }
    }
    else
    {
        if (info -> pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
        {
            /* no compression */
            _gx_display_driver_32bpp_horizontal_pixelmap_line_alpha_write(context, xstart, xend, y, info);
        }
        else
        {
            /* no compression */
            _gx_display_driver_32bpp_horizontal_pixelmap_line_raw_write(context, xstart, xend, y, info);
        }
    }

    /*Current pixelmap has gone over, so the offset pointer should be reset.*/
    if (info -> current_pixel_ptr >= info -> pixelmap -> gx_pixelmap_data + info -> pixelmap -> gx_pixelmap_data_size)
    {
        info -> current_pixel_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_data;
        info -> current_aux_ptr = (GX_UBYTE *)info ->  pixelmap -> gx_pixelmap_aux_data;
    }
}

