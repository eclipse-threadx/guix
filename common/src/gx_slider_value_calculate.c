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
/*    _gx_slider_value_calculate                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service calculates the slider value.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider widget control block   */
/*    info                                  Pointer to slider info        */
/*    new_position                          New slider position           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_travel_get                 Get the slider travel         */
/*    _gx_slider_value_set                  Set the slider value          */
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
UINT  _gx_slider_value_calculate(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_position)
{
INT newval;
INT mintravel;
INT maxtravel;

    /* get travel limits */
    _gx_slider_travel_get(slider, info, &mintravel, &maxtravel);

    /* make sure we don't allow travel too far */
    if (new_position < mintravel)
    {
        new_position = mintravel;
    }
    else
    {
        if (new_position > maxtravel)
        {
            new_position = maxtravel;
        }
    }

    /* use position to calculate new value */
    if (maxtravel > mintravel)
    {
        /* do linear interpolation using input position to
           determine new slider value
         */

        /* get distance from starting point */

        if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
        {
            newval = maxtravel - new_position;
        }
        else
        {
            newval = new_position - mintravel;
        }

        newval = GX_FIXED_VAL_MAKE(newval);

        /* multiply by value range */
        newval *= info -> gx_slider_info_max_val - info -> gx_slider_info_min_val;

        /* divide by travel range */
        newval /= (maxtravel - mintravel);
        newval = GX_FIXED_VAL_RND(newval);

        /* offset by min value */
        newval += info -> gx_slider_info_min_val;

        /* has the value changed? */
        if (newval != info -> gx_slider_info_current_val)
        {
            /* yes, re-assign value which will move the needle */
            _gx_slider_value_set(slider, info, newval);
        }
    }
    return(GX_SUCCESS);
}

