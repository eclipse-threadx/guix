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
/*    _gx_display_driver_generic_rotated_circle_fill      PORTABLE C      */
/*                                                           6.1.3        */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_rotated_circle_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r)
{
GX_DISPLAY           *display;
GX_BRUSH             *brush;
GX_RECTANGLE         *clip;
INT                   ypos;
INT                   curx;
INT                   cury;
INT                   xmin;
INT                   xmax;
INT                   yi;
INT                   x1;
INT                   y1;
INT                   y2;
INT                   width;
INT                   loop;
INT                   skip_line;
GX_PIXELMAP          *pixelmap = GX_NULL;
GX_FILL_PIXELMAP_INFO info;
INT                  *pLineEnds;
INT                   Index;
GX_BYTE               sign[4][2] = {{1, 1}, {-1, 1}, {1, 1}, {-1, 1}};
INT                   error;
GX_COLOR              fill_color;
GX_BYTE               xsign;

    display = context -> gx_draw_context_display;
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;

    if (brush -> gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        if (brush -> gx_brush_pixelmap == GX_NULL)
        {
            /* Nothing should be drawn if pixelmap isn't set with GX_BRUSH_PIXELMAP_FILL style. */
            return;
        }

        pixelmap = brush -> gx_brush_pixelmap;
        if (pixelmap -> gx_pixelmap_format != display -> gx_display_color_format)
        {
            /* Display driver only support its native format pixelmap.*/
            /* Nothing should be drawn if pixelmap format isn't support. */
            return;
        }

        memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));

        /* If filling pixelmap, set fill pixelmap info. */
        info.pixelmap = pixelmap;
        info.current_pixel_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_data;
        if (pixelmap -> gx_pixelmap_aux_data_size)
        {
            info.current_aux_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
        }
    }

    xmin = xcenter - (INT)r;
    xmax = xcenter + (INT)r;

    /* Calculate minimum y line. */
    if (clip -> gx_rectangle_left > xmin)
    {
        xmin = clip -> gx_rectangle_left;
    }

    /* Calculat maximum y line. */
    if (clip -> gx_rectangle_right < xmax)
    {
        xmax = clip -> gx_rectangle_right;
    }

    width = xmax - xmin + 1;
    pLineEnds = _gx_system_scratchpad;

    /* default the point array to being off the screen on both sides: */
    for (loop = 0; loop < width * 2; loop += 2)
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

            y2 = -y1;
            y1 += ycenter;
            y2 += ycenter;
            x1 += xcenter;

            if ((x1 >= xmin) && (x1 <= xmax))
            {
                if (y1 > y2)
                {
                    GX_SWAP_VALS(y1, y2);
                }

                if (y1 < clip -> gx_rectangle_top)
                {
                    y1 = clip -> gx_rectangle_top;
                }

                if (y2 > clip -> gx_rectangle_bottom)
                {
                    y2 = clip -> gx_rectangle_bottom;
                }

                Index = (x1 - xmin) << 1;
                pLineEnds[Index] = y1;
                pLineEnds[Index + 1] = y2;
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

    if (pixelmap)
    {
        if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            skip_line = (xmax - (xcenter - (INT)r) + 1) % pixelmap -> gx_pixelmap_width;

            if (skip_line)
            {
                skip_line = pixelmap -> gx_pixelmap_width - skip_line;
            }

            x1 = xmax;
            xsign = -1;
            Index = (width - 1) * 2;
        }
        else
        {
            skip_line = (xmin - clip -> gx_rectangle_left);

            x1 = xmin;
            xsign = 1;
            Index = 0;
        }

        if (skip_line)
        {

            info.draw = GX_FALSE;
            while (skip_line--)
            {
                display -> gx_display_driver_horizontal_pixelmap_line_draw(context, 0, 0, 0, &info);
            }
        }

        info.draw = GX_TRUE;
        ypos = ycenter - (INT)r;

        for (curx = xmin; curx <= xmax; curx++)
        {
            if (pLineEnds[Index] <= pLineEnds[Index + 1])
            {
                info.x_offset = pLineEnds[Index] - ypos;
                display -> gx_display_driver_horizontal_pixelmap_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], x1, &info);
            }

            x1 += xsign;
            Index += xsign;
            Index += xsign;
        }
    }
    else
    {
        Index = 0;
        fill_color = brush -> gx_brush_fill_color;

        for (curx = xmin; curx <= xmax; curx++)
        {
            if (pLineEnds[Index] <= pLineEnds[Index + 1])
            {
                display -> gx_display_driver_vertical_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], curx, 1, fill_color);
            }
            Index += 2;
        }
    }
}
#endif

