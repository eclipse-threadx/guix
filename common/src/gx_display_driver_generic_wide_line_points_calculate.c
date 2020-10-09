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
#include "gx_display.h"


/* Internal scratch area. */
static GX_FIXED_POINT LinePoints[5];

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_wide_line_points_calculate               */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Calculate corners of wide line. Used by all versions (anti-aliased  */
/*    or not, square or round ends) of wide line drawinggeneric display   */
/*    driver wide line drawing function.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xStart                                x-coord of endpoint           */
/*    yStart                                y-coord of endpoint           */
/*    xEnd                                  x-coord of endpoint           */
/*    yEnd                                  y-coord of endpoint           */
/*    outline                               Whether or not to draw        */
/*                                            outline                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_POINT*                             Calculated end points         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GX_ABS                                Compute the absolute value    */
/*    GX_SWAP_VALS                          Swap two values               */
/*    GX_FIXED_VAL_MAKE                                                   */
/*    GX_FIXED_VAL_RND                                                    */
/*    _gx_utility_math_sqrt                 Compute the square root value */
/*    [gx_display_driver_anti_aliased_line_draw                           */
/*                                          Driver function that draws    */
/*                                            anti-aliased lines.         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_simple_wide_line_draw                    */
/*    _gx_display_driver_generic_aliased_wide_line_draw                   */
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
GX_FIXED_POINT *_gx_display_driver_generic_wide_line_points_calculate(GX_DRAW_CONTEXT *context, INT xStart, INT yStart,
                                                                      INT xEnd, INT yEnd, INT brush_width, GX_BOOL outline)
{
GX_FIXED_VAL   distance;
GX_FIXED_VAL   brush_distance;
GX_FIXED_POINT FixedPoints[5];
INT            LineDirection[4];
int            xsign;
int            ysign;
int            xside;
int            yside;
VOID           (*aliased_line)(GX_DRAW_CONTEXT *context, GX_FIXED_VAL x1, GX_FIXED_VAL y1, GX_FIXED_VAL x2, GX_FIXED_VAL y2);

int            dx = GX_ABS(xEnd - xStart);
int            dy = GX_ABS(yEnd - yStart);

    if (((dx >= dy && (xStart > xEnd)) || ((dy > dx) && yStart > yEnd)))
    {
        GX_SWAP_VALS(xEnd, xStart);
        GX_SWAP_VALS(yEnd, yStart);
    }

    distance = (GX_FIXED_VAL)_gx_utility_math_sqrt((UINT)(GX_FIXED_VAL_MAKE(dx * dx) + GX_FIXED_VAL_MAKE(dy * dy)));
    distance <<= (GX_FIXED_VAL_SHIFT >> 1);
    brush_distance = GX_FIXED_VAL_MAKE(brush_width - 1);
    brush_distance >>= 1;
    xsign = ysign = 1;

    if (dx)
    {
        xsign = (xEnd - xStart) / dx;
    }
    if (dy)
    {
        ysign = (yEnd - yStart) / dy;
    }

    xside = dy;
    xside *= brush_distance;
    xside /= (distance >> GX_FIXED_VAL_SHIFT);
    xside *= xsign;

    yside = dx;
    yside *= brush_distance;
    yside /= (distance >> GX_FIXED_VAL_SHIFT);
    yside *= ysign;

    LineDirection[0] = 0;
    LineDirection[1] = 1;
    LineDirection[2] = 2;
    LineDirection[3] = 3;

    if (yEnd < yStart)
    {
        LineDirection[1] = 3;
        LineDirection[3] = 1;
    }

    if (xEnd < xStart)
    {
        LineDirection[0] = 2;
        LineDirection[2] = 0;
    }

    FixedPoints[LineDirection[0]].x = GX_FIXED_VAL_MAKE(xStart) - xside;
    FixedPoints[LineDirection[0]].y = GX_FIXED_VAL_MAKE(yStart) + yside;
    FixedPoints[LineDirection[1]].x = GX_FIXED_VAL_MAKE(xStart) + xside;
    FixedPoints[LineDirection[1]].y = GX_FIXED_VAL_MAKE(yStart) - yside;
    FixedPoints[LineDirection[2]].x = GX_FIXED_VAL_MAKE(xEnd) + xside;
    FixedPoints[LineDirection[2]].y = GX_FIXED_VAL_MAKE(yEnd) - yside;
    FixedPoints[LineDirection[3]].x = GX_FIXED_VAL_MAKE(xEnd) - xside;
    FixedPoints[LineDirection[3]].y = GX_FIXED_VAL_MAKE(yEnd) + yside;

    LinePoints[0] = FixedPoints[0];
    LinePoints[1] = FixedPoints[1];
    LinePoints[2] = FixedPoints[2];
    LinePoints[3] = FixedPoints[3];

    if (outline)
    {
        aliased_line = _gx_display_driver_generic_aliased_fixed_point_line_draw;

        aliased_line(context, FixedPoints[0].x,
                     FixedPoints[0].y,
                     FixedPoints[1].x,
                     FixedPoints[1].y);

        aliased_line(context, FixedPoints[1].x,
                     FixedPoints[1].y,
                     FixedPoints[2].x,
                     FixedPoints[2].y);

        aliased_line(context, FixedPoints[2].x,
                     FixedPoints[2].y,
                     FixedPoints[3].x,
                     FixedPoints[3].y);

        aliased_line(context, FixedPoints[3].x,
                     FixedPoints[3].y,
                     FixedPoints[0].x,
                     FixedPoints[0].y);
    }

    /* close the polygon */
    LinePoints[4] = LinePoints[0];

    return(LinePoints);
}

