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
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_display.h"

#if defined(GX_ARC_DRAWING_SUPPORT)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_ellipse_fill             PORTABLE C      */
/*                                                           6.1.6        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw a filled ellipse.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of ellipse  */
/*    ycenter                               y-coord of center of ellipse  */
/*    a                                     Length of the Semi-major Axis */
/*    b                                     Length of the Semi-minor Axis */
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
/*  04-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            modified algorithm to make  */
/*                                            it fit one-width antialiaed */
/*                                            ellipse outline,            */
/*                                            resulting in version 6.1.6  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_ellipse_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b)
{
/* The ellipse fill function follows midpoint ellipse algorithm while
   connecting the two point that symmetric with respect to y-axis. */

GX_DISPLAY           *display;
GX_RECTANGLE         *clip;
GX_BRUSH             *brush;
INT                   x0;
INT                   x1;
INT                   x;
INT                   y;
INT                   y1;
INT                   sign[2] = {1, -1};
INT                  *pLineEnds;
INT                   index;
INT                   height;
INT                   Index;
INT                   aa;
INT                   bb;
INT                   xpos;
INT                   ymin;
INT                   ymax;
INT                   skip_line;
GX_PIXELMAP          *pixelmap = GX_NULL;
GX_VALUE              format;
GX_COLOR              fill_color;
GX_FILL_PIXELMAP_INFO info;
INT                   error;
INT                   realval;


    display = context -> gx_draw_context_display;
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;

    if (brush -> gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        /* Pick up pixelmap. */
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

        if (format >= GX_COLOR_FORMAT_24XRGB)
        {
            /* 32ARGB FORMAT use 24xrgb driver for now. So this is a special case.*/
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

        info.pixelmap = brush -> gx_brush_pixelmap;
        info.current_pixel_ptr = (GX_UBYTE *)info.pixelmap -> gx_pixelmap_data;

        if (pixelmap -> gx_pixelmap_aux_data_size)
        {
            info.current_aux_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
        }
    }

    xpos = xcenter - a;
    ymin = ycenter - b;
    ymax = ycenter + b;

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

    /* Calculate maximum y line. */
    if (clip -> gx_rectangle_bottom < ymax)
    {
        ymax = clip -> gx_rectangle_bottom;
    }

    height = (ymax - ymin + 1);
    pLineEnds = _gx_system_scratchpad;

    /* default the point array to being off the screen on both sides: */
    for (Index = 0; Index < height * 2; Index += 2)
    {
        pLineEnds[Index] = 2000;
        pLineEnds[Index + 1] = 0;
    }

    aa = a * a;
    bb = b * b;
    x = 0;
    y = b;
    error = 0;

    /* Region I of the first quarter of the ellipse.  */
    while (2 * bb * (x + 1) < aa * (2 * y - 1))
    {
        /* calculate error of next pixel. */
        realval = bb - bb * (x + 1) * (x + 1) / aa;
        error = (y << 8) - (INT)(_gx_utility_math_sqrt((UINT)(realval << 10)) << 3);

        if (error >= 510)
        {
            /* The slope in point(x + 1, y) is greater than -1,
               make point(x, y) the delimit pixel, break here. */
            realval = bb - bb * x * x / aa;
            break;
        }

        x0 = xcenter - x;
        x1 = xcenter + x;

        if (x0 < clip -> gx_rectangle_left)
        {
            x0 = clip -> gx_rectangle_left;
        }

        if (x1 > clip -> gx_rectangle_right)
        {
            x1 = clip -> gx_rectangle_right;
        }

        for (index = 0; index < 2; index++)
        {
            y1 = (y - 1) * sign[index] + ycenter;

            if ((y1 >= ymin) && (y1 <= ymax))
            {
                Index = (y1 - ymin) << 1;
                pLineEnds[Index] = x0;
                pLineEnds[Index + 1] = x1;
            }
        }

        if (error >= 255)
        {
            error -= 255;
            y--;
        }

        x++;
    }

    /* Region II of the first quarter of the ellipse.  */
    while (y > 0)
    {
        y--;

        realval = aa - aa * y * y / bb;
        error = (INT)(_gx_utility_math_sqrt((UINT)(realval << 10)) << 3) - (x << 8);

        while (error >= 255)
        {
            error -= 255;
            x++;
        }

        x0 = xcenter - x;
        x1 = xcenter + x;

        if (x0 < clip -> gx_rectangle_left)
        {
            x0 = clip -> gx_rectangle_left;
        }

        if (x1 > clip -> gx_rectangle_right)
        {
            x1 = clip -> gx_rectangle_right;
        }

        for (index = 0; index < 2; index++)
        {
            y1 = y * sign[index] + ycenter;

            if ((y1 >= ymin) && (y1 <= ymax))
            {
                Index = (y1 - ymin) << 1;
                pLineEnds[Index] = x0;
                pLineEnds[Index + 1] = x1;
            }
        }
    }

    Index = 0;
    if (pixelmap != GX_NULL)
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

        for (y = ymin; y <= ymax; y++)
        {
            info.x_offset = pLineEnds[Index] - xpos;
            display -> gx_display_driver_horizontal_pixelmap_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], y, &info);
            Index += 2;
        }
    }
    else
    {
        fill_color = brush -> gx_brush_fill_color;

        for (y = ymin; y <= ymax; y++)
        {
            display -> gx_display_driver_horizontal_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], y, 1, fill_color);
            Index += 2;
        }
    }
}
#endif

