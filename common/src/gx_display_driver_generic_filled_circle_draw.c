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
/*    _gx_display_driver_generic_filled_circle_draw       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw filled circals.                              */
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
/*                                          Driver-level horizontal line  */
/*                                            draw function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_simple_wide_line_draw                    */
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
VOID _gx_display_driver_generic_filled_circle_draw(GX_DRAW_CONTEXT *context, GX_FIXED_VAL xcenter, GX_FIXED_VAL ycenter, GX_FIXED_VAL r)
{
/* The circle draw function is implemented from midpoint circle algorithm. */

GX_DISPLAY   *display;
GX_RECTANGLE *clip;
GX_BRUSH     *brush;
INT           x;
INT           y;
GX_BYTE       ysign[2] = {1, -1};
INT           index;
INT           yi;
GX_VALUE      xstart;
GX_VALUE      xend;
GX_FIXED_VAL  xfraction;
GX_FIXED_VAL  yfraction;
INT           decision;
INT           half_shift;

    display = context -> gx_draw_context_display;
    clip = context -> gx_draw_context_clip;
    brush = &context -> gx_draw_context_brush;

    xfraction = (xcenter & GX_FIXED_VAL_FRACTION_MASK);
    yfraction = (ycenter & GX_FIXED_VAL_FRACTION_MASK);

    r -= GX_FIXED_VAL_HALF;

    if (xfraction)
    {
        x = GX_FIXED_VAL_ONE - xfraction;
    }
    else
    {
        x = 0;
    }
    y = GX_FIXED_VAL_RND_UP(r + yfraction);
    y = GX_FIXED_VAL_MAKE(y) - yfraction;

    half_shift = GX_FIXED_VAL_SHIFT >> 1;

    if ((x == 0) && (y == r))
    {
        decision = 256 - r;
    }
    else
    {
        decision = (x >> half_shift) * (x >> half_shift);
        decision += ((y - GX_FIXED_VAL_HALF) >> half_shift) * ((y - GX_FIXED_VAL_HALF) >> half_shift);
        decision -= ((r >> half_shift) * (r >> half_shift));
    }

    while (1)
    {
        if (decision < 0)
        {
            decision += 2 * x + GX_FIXED_VAL_ONE;
        }
        else
        {
            decision += 2 * (x - y) + GX_FIXED_VAL_MAKE(3);
            y -= GX_FIXED_VAL_ONE;
        }

        if (x > y)
        {
            break;
        }

        for (index = 0; index < 2; index++)
        {
            xstart = (GX_VALUE)GX_FIXED_VAL_TO_INT(xcenter - x);
            xend = (GX_VALUE)GX_FIXED_VAL_TO_INT(xcenter + x);
            yi = (GX_VALUE)GX_FIXED_VAL_TO_INT(ycenter + y * ysign[index]);

            if (xstart < clip -> gx_rectangle_left)
            {
                xstart = clip -> gx_rectangle_left;
            }

            if (xend > clip -> gx_rectangle_right)
            {
                xend = clip -> gx_rectangle_right;
            }

            if (xstart <= xend &&
                yi >= clip -> gx_rectangle_top &&
                yi <= clip -> gx_rectangle_bottom)
            {
                display -> gx_display_driver_horizontal_line_draw(context, xstart, xend, yi, 1, brush -> gx_brush_line_color);
            }
        }

        x += GX_FIXED_VAL_ONE;
    }

    x = GX_FIXED_VAL_RND_UP(r + xfraction);
    x = GX_FIXED_VAL_MAKE(x) - xfraction;

    if (yfraction)
    {
        y = yfraction;
    }
    else
    {
        y = 0;
    }

    if ((y == 0) && (x == r))
    {
        decision = 256 - r;
    }
    else
    {
        decision = (((x - GX_FIXED_VAL_HALF) >> half_shift) * ((x - GX_FIXED_VAL_HALF) >> half_shift));
        decision += ((y >> half_shift) * (y >> half_shift));
        decision -=  ((r >> half_shift) * (r >> half_shift));
    }

    while (1)
    {
        if (decision < 0)
        {
            decision += 2 * y + GX_FIXED_VAL_ONE;
        }
        else
        {
            decision += 2 * (y - x) + GX_FIXED_VAL_MAKE(3);
            x -= GX_FIXED_VAL_ONE;
        }

        if (x <= y)
        {
            break;
        }

        for (index = 0; index < 2; index++)
        {
            xstart = (GX_VALUE)GX_FIXED_VAL_TO_INT(xcenter - x);
            xend  = (GX_VALUE)GX_FIXED_VAL_TO_INT(xcenter + x);
            yi = (GX_VALUE)GX_FIXED_VAL_TO_INT(ycenter + y * ysign[index]);

            if (xstart < clip -> gx_rectangle_left)
            {
                xstart = clip -> gx_rectangle_left;
            }

            if (xend > clip -> gx_rectangle_right)
            {
                xend = clip -> gx_rectangle_right;
            }

            if (xstart <= xend &&
                yi >= clip -> gx_rectangle_top &&
                yi <= clip -> gx_rectangle_bottom)
            {
                display -> gx_display_driver_horizontal_line_draw(context, xstart, xend, yi, 1, brush -> gx_brush_line_color);
            }
        }

        y += GX_FIXED_VAL_ONE;
    }
}

