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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_radial_progress_bar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_value_calculate             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service calcualtes the radial progress bar value.              */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Radial Progress Bar control   */
/*                                            block                       */
/*    new_position                          New progress bar position     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radial_progress_bar_value_set     Set the value for the         */
/*                                            progress bar                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gx_radial_progress_bar_value_calculate(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_POINT new_position)
{
GX_RADIAL_PROGRESS_BAR_INFO *info;
INT                          new_value;
INT                          dist;
INT                          x_dist;
INT                          y_dist;


    info = &progress_bar -> gx_radial_progress_bar_info;

    x_dist = (INT)(new_position.gx_point_x - info -> gx_radial_progress_bar_info_xcenter);
    y_dist = (INT)(new_position.gx_point_y - info -> gx_radial_progress_bar_info_ycenter);

    dist = (x_dist * x_dist) + (y_dist * y_dist);
    dist = (INT)_gx_utility_math_sqrt((UINT)dist);

    if (!dist)
    {
        return GX_FAILURE;
    }
   
    if (new_position.gx_point_y <= info -> gx_radial_progress_bar_info_ycenter)
    {
        x_dist = GX_FIXED_VAL_MAKE(x_dist) / dist;

        /* New position hit upper track.  */
        new_value = _gx_utility_math_acos(x_dist);
    }
    else
    {
        y_dist = GX_FIXED_VAL_MAKE(y_dist) / dist;

        /* New position hit lower track.  */
        new_value = _gx_utility_math_asin(y_dist);

        if (new_position.gx_point_x < info -> gx_radial_progress_bar_info_xcenter)
        {
            new_value = 180 + new_value;
        }
        else
        {
            new_value = 360 - new_value;
        }
    }

    if (info -> gx_radial_progress_bar_info_current_val <= 0)
    {
        /* Move radial progress bar clockwise.  */
        new_value = new_value - info -> gx_radial_progress_bar_info_anchor_val;

        if (new_value > 0)
        {
            new_value -= 360;
        }
    }
    else
    {
        /* Move radial progress bar anti-clockwise.  */
        new_value = new_value - info -> gx_radial_progress_bar_info_anchor_val;

        if (new_value < 0)
        {
            new_value += 360;
        }
    }

    _gx_radial_progress_bar_value_set(progress_bar, (GX_VALUE)new_value);

    return(GX_SUCCESS);
}

