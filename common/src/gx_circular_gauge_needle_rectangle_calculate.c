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
/**   Circular Gauge Management (Circular Gauge)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_circular_gauge.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_needle_rectangle_calculate       PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates needle rectangle.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    circular_gauge                        Circular gauge control block  */
/*    angle                                 The rotation angle            */
/*    rect                                  return rectangle pointer      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_sin                  Compute sin value             */
/*    _gx_utility_math_cos                  Compute cos value             */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            corrected logic,            */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_circular_gauge_needle_rectangle_calculate(GX_CIRCULAR_GAUGE *gauge, INT angle, GX_RECTANGLE *rect)
{
GX_CIRCULAR_GAUGE_INFO *info;
INT                    mx[4] = { -1, 1, 1, -1 };
INT                    my[4] = { 1, 1, -1, -1 };
INT                    idxminx;
INT                    idxmaxx;
INT                    idxmaxy;
GX_PIXELMAP            *needle_map;
INT                    srcxres;
INT                    srcyres;
INT                    cosv;
INT                    sinv;
INT                    xres;
INT                    yres;
INT                    width;
INT                    height;
INT                    rot_cx;
INT                    rot_cy;
INT                    x;
INT                    y;

    /* Pick up needle pixelmap.  */
    needle_map = gauge -> gx_circular_gauge_needle_source;

    if (needle_map == GX_NULL)
    {
        return GX_FAILURE;
    }

    info = &gauge -> gx_circular_gauge_info;

    while (angle < 0)
    {
        angle += 360;
    }

    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = needle_map -> gx_pixelmap_width >> 1;
    srcyres = needle_map -> gx_pixelmap_height >> 1;

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT(mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv);
    yres = GX_FIXED_VAL_TO_INT(my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv);

    /* Calculate destination width and height. */
    width = (xres << 1);
    height = (yres << 1);

    rot_cx = info -> gx_circular_gauge_info_needle_xcor;
    rot_cy = info -> gx_circular_gauge_info_needle_ycor;

    /* Calculate the new rotation axis. */
    x = (rot_cx - srcxres) * cosv - (rot_cy - srcyres) * sinv;
    y = (rot_cy - srcyres) * cosv + (rot_cx - srcxres) * sinv;

    x = GX_FIXED_VAL_TO_INT(x) + xres;
    y = GX_FIXED_VAL_TO_INT(y) + yres;

    /* Calculate new needle rectangle */
    rect -> gx_rectangle_left = (GX_VALUE)(gauge -> gx_widget_size.gx_rectangle_left +
                                           info -> gx_circular_gauge_info_needle_xpos - x);
    
    rect -> gx_rectangle_top = (GX_VALUE)(gauge -> gx_widget_size.gx_rectangle_top +
                                          info -> gx_circular_gauge_info_needle_ypos - y);

    rect -> gx_rectangle_right = (GX_VALUE)(rect -> gx_rectangle_left + width);
    rect -> gx_rectangle_bottom = (GX_VALUE)(rect -> gx_rectangle_top + height);

    /* Mark the combine area as dirty.  */
    return(GX_SUCCESS);
}

