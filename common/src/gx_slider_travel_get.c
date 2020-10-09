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
/*    _gx_slider_travel_get                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the slider travel.                                */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider control block          */
/*    info                                  Pointer to slider info        */
/*                                            structure                   */
/*    return_min_travel                     Pointer to destination for    */
/*                                            for minimum travel value    */
/*    return_max_travel                     Pointer to destination for    */
/*                                            for maximum travel value    */
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
UINT _gx_slider_travel_get(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT *return_min_travel, INT *return_max_travel)
{
INT mintravel = 0;
INT maxtravel = 0;

    if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
    {
        mintravel = slider -> gx_widget_size.gx_rectangle_top;
        maxtravel = slider -> gx_widget_size.gx_rectangle_bottom;

        mintravel += info -> gx_slider_info_max_travel;
        maxtravel -= info -> gx_slider_info_min_travel;
    }
    else
    {
        mintravel = slider -> gx_widget_size.gx_rectangle_left;
        maxtravel = slider -> gx_widget_size.gx_rectangle_right;

        mintravel += info -> gx_slider_info_min_travel;
        maxtravel -= info -> gx_slider_info_max_travel;
    }

    *return_min_travel = mintravel;
    *return_max_travel = maxtravel;

    return(GX_SUCCESS);
}

