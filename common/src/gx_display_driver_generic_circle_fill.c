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
#include "gx_system.h"
#include "gx_utility.h"

#if defined(GX_ARC_DRAWING_SUPPORT)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_circle_fill              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver fill circle.                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          Basic display driver          */
/*                                            horizontal line draw routine*/
/*    [gx_display_driver_horizontal_pixelmap_line_draw]                   */
/*                                          Basic display driver          */
/*                                            horizontal pixelmap line    */
/*                                            draw function               */
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
VOID _gx_display_driver_generic_circle_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r)
{
GX_DISPLAY           *display;
GX_BRUSH             *brush;
GX_RECTANGLE         *clip;
INT                   xpos;
INT                   curx;
INT                   cury;
INT                   ymin;
INT                   ymax;
INT                   yi;
INT                   x1;
INT                   x2;
INT                   y1;
INT                   height;
INT                   loop;
GX_VALUE              format;
INT                   skip_line;
GX_PIXELMAP          *pixelmap = GX_NULL;
GX_FILL_PIXELMAP_INFO info;
INT                  *pLineEnds;
INT                   Index;
INT                   sign[4][2] = {{1, 1}, {1, -1}, {1, 1}, {1, -1}};
INT                   error;
GX_COLOR              fill_color;

    display = context -> gx_draw_context_display;
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;

    if (brush -> gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        pixelmap = brush -> gx_brush_pixelmap;
        if (pixelmap == GX_NULL)
        {
            /* Nothing should be drawn if pixelmap isn't set with GX_BRUSH_PIXELMAP_FILL style. */
            return;
        }
        if (pixelmap -> gx_pixelmap_height <= 0 ||
            pixelmap -> gx_pixelmap_width <= 0)
        {
            return;
        }
        format = display -> gx_display_color_format;
        memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));

        /*Check the pixelmap format is supported in this driver or not.*/
        if (format >= GX_COLOR_FORMAT_24XRGB)
        {
            /*32ARGB FORMAT use 24xrgb driver for now. So this is a special case.*/
            if (pixelmap -> gx_pixelmap_format < GX_COLOR_FORMAT_24XRGB)
            {
                /* Display driver only support its native format pixelmap.*/
                /* Nothing should be drawn if pixelmap format isn't support. */
                return;
            }
        }
        else
        {
            if (pixelmap -> gx_pixelmap_format != format)
            {
                /* Display driver only support its native format pixelmap.*/
                /* Nothing should be drawn if pixelmap format isn't support. */
                return;
            }
        }

        if (format == GX_COLOR_FORMAT_4BIT_GRAY)
        {
            info.mask = 0xf0;
        }

        /* If filling pixelmap, set fill pixelmap info. */
        info.pixelmap = pixelmap;
        info.current_pixel_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
        if (pixelmap -> gx_pixelmap_aux_data_size)
        {
            info.current_aux_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
        }
    }

    xpos = xcenter - (INT)r;
    ymin = ycenter - (INT)r;
    ymax = ycenter + (INT)r;

    /* Calculate minimum y line. */
    if (clip -> gx_rectangle_top > ymin)
    {
        skip_line = clip -> gx_rectangle_top - ymin;
        ymin = clip -> gx_rectangle_top;
    }
    else
    {
        skip_line = 0;
    }

    /* Calculat maximum y line. */
    if (clip -> gx_rectangle_bottom < ymax)
    {
        ymax = clip -> gx_rectangle_bottom;
    }

    height = ymax - ymin + 1;
    pLineEnds = _gx_system_scratchpad;

    /* default the point array to being off the screen on both sides: */
    for (loop = 0; loop < height * 2; loop += 2)
    {
        pLineEnds[loop] = 2000;
        pLineEnds[loop + 1] = 0;
    }

    curx = 0;
    cury = (INT)r;
    error = 0;

    /* For shapes pixelmap-fill driver function, it must fill shapes line by line from up to bottom.
       So this contains two steps here: first, calculate the boudary point of circle; Second, draw pixelmap line by line. */
    while (curx <= cury)
    {
        for (loop = 0; loop < 4; loop++)
        {
            /* Upper half part. */
            if (loop < 2)
            {
                x1 = curx * sign[loop][0];
                y1 = (cury - 1) * sign[loop][1];
            }
            /* lower half part. */
            else
            {
                x1 = (cury - 1) * sign[loop][0];
                y1 = curx * sign[loop][1];
            }

            x2 = -x1;
            x1 += xcenter;
            x2 += xcenter;
            y1 += ycenter;

            if ((y1 >= ymin) && (y1 <= ymax))
            {
                if (x1 > x2)
                {
                    GX_SWAP_VALS(x1, x2);
                }

                if (x1 < clip -> gx_rectangle_left)
                {
                    x1 = clip -> gx_rectangle_left;
                }

                if (x2 > clip -> gx_rectangle_right)
                {
                    x2 = clip -> gx_rectangle_right;
                }

                Index = (y1 - ymin) << 1;
                pLineEnds[Index] = x1;
                pLineEnds[Index + 1] = x2;
            }
        }

        curx++;
        yi = (INT)(r * r) - curx * curx;
        error = (cury << 8) - (INT)(_gx_utility_math_sqrt((UINT)(yi << 10)) << 3);

        while (error >= 255)
        {
            error -= 255;
            cury--;
        }
    }

    Index = 0;

    if (pixelmap)
    {
        skip_line = (skip_line % info.pixelmap -> gx_pixelmap_height);
        if (skip_line)
        {
            info.draw = GX_FALSE;
            while (skip_line--)
            {
                display -> gx_display_driver_horizontal_pixelmap_line_draw(context, 0, 0, 0, &info);
            }
        }

        info.draw = GX_TRUE;

        for (cury = ymin; cury <= ymax; cury++)
        {
            if (pLineEnds[Index] <= pLineEnds[Index + 1])
            {
                info.x_offset = pLineEnds[Index] - xpos;
                display -> gx_display_driver_horizontal_pixelmap_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], cury, &info);
            }
            Index += 2;
        }
    }
    else
    {
        fill_color = brush -> gx_brush_fill_color;

        for (cury = ymin; cury <= ymax; cury++)
        {
            if (pLineEnds[Index] <= pLineEnds[Index + 1])
            {
                display -> gx_display_driver_horizontal_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], cury, 1, fill_color);
            }
            Index += 2;
        }
    }
}
#endif

