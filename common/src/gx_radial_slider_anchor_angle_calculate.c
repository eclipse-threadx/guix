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
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_anchor_angle_calculate            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service retrieves the nearest anchor angle for the specified   */
/*    angle value.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    angle_value                           Angle value whose neareast    */
/*                                            anchor angle is calculated  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
UINT _gx_radial_slider_anchor_angle_calculate(GX_RADIAL_SLIDER *slider, GX_VALUE *angle_value)
{
INT                    index;
GX_VALUE               min_value;
GX_VALUE               max_value;
GX_VALUE               neareast_anchor_value;
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;

    index = 0;

    /* Range angle value inside min/max angles. */
    if (*angle_value <= info->gx_radial_slider_info_min_angle)
    {
        neareast_anchor_value = info->gx_radial_slider_info_min_angle;
    }
    else if (*angle_value >= info->gx_radial_slider_info_max_angle)
    {
        neareast_anchor_value = info->gx_radial_slider_info_max_angle;
    }
    else
    {
        neareast_anchor_value = *angle_value;
    }

    if (info -> gx_radial_slider_info_angle_list)
    {
        /* If an angle list is supplied, set the target value to one of the values
           in angle list that is nearest to the given value. */
        while (index < info -> gx_radial_slider_info_list_count)
        {
            if (index == 0)
            {
                min_value = info -> gx_radial_slider_info_angle_list[index];
            }
            else
            {
                min_value = (GX_VALUE)((info -> gx_radial_slider_info_angle_list[index] + info -> gx_radial_slider_info_angle_list[index - 1]) >> 1);
            }

            if (index == info -> gx_radial_slider_info_list_count - 1)
            {
                max_value = info -> gx_radial_slider_info_angle_list[index];
            }
            else
            {
                max_value = (GX_VALUE)((info -> gx_radial_slider_info_angle_list[index] + info -> gx_radial_slider_info_angle_list[index + 1]) >> 1);
            }

            if (min_value > max_value)
            {
                GX_SWAP_VALS(min_value, max_value);
            }

            if ((neareast_anchor_value >= min_value) && (neareast_anchor_value <= max_value))
            {
                neareast_anchor_value = info -> gx_radial_slider_info_angle_list[index];
                break;
            }

            index++;
        }
    }

    (*angle_value) = neareast_anchor_value;

    return(GX_SUCCESS);
}

