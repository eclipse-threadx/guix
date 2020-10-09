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
/**   Prompt Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_slider.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_slider_needle_position_get                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the slider needle position.                       */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider widget control block   */
/*    slider_info                           Pointer to slider information */
/*                                            structure defining the      */
/*                                            slider limits, needle size  */
/*                                            and offset, and other       */
/*                                            slider parameters.          */
/*    return_position                       Pointer to destination for    */
/*                                            needle position             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_travel_get                 Gets the slider travel        */
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
UINT _gx_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position)
{
INT      mintravel;
INT      maxtravel;
INT      shift = 0;
GX_VALUE hotspot;

    /* get the limits of the slider travel */
    _gx_slider_travel_get(slider, slider_info, &mintravel, &maxtravel);

    /* calculate shift amount based on value */

    if (slider_info -> gx_slider_info_max_val > slider_info -> gx_slider_info_min_val)
    {
        shift = (slider_info -> gx_slider_info_current_val - slider_info -> gx_slider_info_min_val);
        shift *= (maxtravel - mintravel);
        shift /= (slider_info -> gx_slider_info_max_val - slider_info -> gx_slider_info_min_val);
        shift += slider_info -> gx_slider_info_min_travel;
    }

    /* calculate needle dimensions */

    if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
    {
        /* calculate position of the hotspot */
        hotspot = (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_bottom - shift);

        if (return_position)
        {
            return_position -> gx_rectangle_left =
                (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_left + slider_info -> gx_slider_info_needle_inset);
            return_position -> gx_rectangle_top = (GX_VALUE)(hotspot - slider_info -> gx_slider_info_needle_hotspot_offset);
            if (slider_info -> gx_slider_info_needle_width > 0)
            {
                return_position -> gx_rectangle_right = (GX_VALUE)(return_position -> gx_rectangle_left + slider_info -> gx_slider_info_needle_width - 1);
                return_position -> gx_rectangle_bottom = (GX_VALUE)(return_position -> gx_rectangle_top + slider_info -> gx_slider_info_needle_height - 1);
            }
            else
            {
                return_position -> gx_rectangle_right = slider -> gx_widget_size.gx_rectangle_right;
                return_position -> gx_rectangle_bottom = return_position -> gx_rectangle_top;
            }
        }
    }
    else
    {
        /* calculate the position of the hotspot */
        hotspot = (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_left + shift);

        if (return_position)
        {
            return_position -> gx_rectangle_top =
                (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_top + slider_info -> gx_slider_info_needle_inset);
            return_position -> gx_rectangle_left = (GX_VALUE)(hotspot - slider_info -> gx_slider_info_needle_hotspot_offset);

            if (slider_info -> gx_slider_info_needle_width > 0)
            {
                return_position -> gx_rectangle_bottom = (GX_VALUE)(return_position -> gx_rectangle_top + slider_info -> gx_slider_info_needle_height - 1);
                return_position -> gx_rectangle_right = (GX_VALUE)(return_position -> gx_rectangle_left + slider_info -> gx_slider_info_needle_width - 1);
            }
            else
            {
                return_position -> gx_rectangle_bottom = slider -> gx_widget_size.gx_rectangle_bottom;
                return_position -> gx_rectangle_right = return_position -> gx_rectangle_left;
            }
        }
    }
    return(GX_SUCCESS);
}

