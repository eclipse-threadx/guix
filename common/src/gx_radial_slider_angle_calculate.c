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
/**   Radial Slider Management (Slider)                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_angle_calculate                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service calculates the slider angle according to a point value.*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    point                                 Point value on radial track   */
/*    return_value                          Retrieved angle value         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_acos                 Compute acos value            */
/*    _gx_utility_math_asin                 Compute asin value            */
/*    _gx_utility_math_sqrt                 Compute sqrt value            */
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
UINT _gx_radial_slider_angle_calculate(GX_RADIAL_SLIDER *slider, GX_POINT point, GX_VALUE *return_value)
{
GX_RADIAL_SLIDER_INFO *info;
INT                    new_value;
INT                    dist;
INT                    x_dist;
INT                    y_dist;
INT                    x_center;
INT                    y_center;

    info = &slider -> gx_radial_slider_info;

    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;

    x_dist = (INT)(point.gx_point_x - x_center);
    y_dist = (INT)(point.gx_point_y - y_center);

    dist = (x_dist * x_dist) + (y_dist * y_dist);
    dist = (INT)_gx_utility_math_sqrt((UINT)dist);

    if (!dist)
    {
        *return_value = 0;
        return GX_FAILURE;
    }

    if (point.gx_point_y <= y_center)
    {
        x_dist = GX_FIXED_VAL_MAKE(x_dist) / dist;

        /* The needle is between [0, 180].  */
        new_value = _gx_utility_math_acos(x_dist);
    }
    else
    {
        y_dist = (GX_FIXED_VAL_MAKE(y_dist)) / dist;

        /* The needle is between [180, 360]  */
        new_value = _gx_utility_math_asin(y_dist);

        if (point.gx_point_x < x_center)
        {
            new_value = 180 + new_value;
        }
        else
        {
            new_value = 360 - new_value;
        }
    }

    /* Range value inside specified [-90, 270]*/
    if (new_value > 270)
    {
        new_value = new_value - 360;
    }

    *return_value = (GX_VALUE)new_value;

    return(GX_SUCCESS);
}

