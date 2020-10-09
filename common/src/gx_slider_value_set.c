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
/**   Slider Management (Slider)                                          */
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
/*    _gx_slider_value_set                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the slider value.                                 */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider widget control block   */
/*    info                                  Pointer to slider info        */
/*    new_value                             New slider value              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_needle_position_get        Get the slider needle         */
/*                                            position                    */
/*    _gx_utility_rectangle_combine         Combine the first and second  */
/*                                            rectangle into the first    */
/*                                            rectangle                   */
/*    _gx_system_dirty_partial_add          Mark the partial area of this */
/*                                            widget as dirty             */
/*    _gx_widget_event_generate             Generate an event and send it */
/*                                            to the parent widget        */
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
UINT _gx_slider_value_set(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_value)
{
GX_RECTANGLE oldpos;
GX_RECTANGLE newpos;

    if (new_value != info -> gx_slider_info_current_val)
    {
        /* get the old need position */
        _gx_slider_needle_position_get(slider, info, &oldpos);

        /* assign slider value */
        info -> gx_slider_info_current_val = new_value;

        /* get the new needle position */

        _gx_slider_needle_position_get(slider, info, &newpos);

        /* combine old and new needle positions */
        _gx_utility_rectangle_combine(&newpos, &oldpos);

        if (slider -> gx_widget_type == GX_TYPE_PIXELMAP_SLIDER)
        {
            newpos.gx_rectangle_left = slider -> gx_widget_size.gx_rectangle_left;
            newpos.gx_rectangle_right = slider -> gx_widget_size.gx_rectangle_right;
        }

        /* mark as dirty so that I get redrawn */
        _gx_system_dirty_partial_add((GX_WIDGET *)slider, &newpos);

        /* notify my parent of my new value */
        _gx_widget_event_generate((GX_WIDGET *)slider, GX_EVENT_SLIDER_VALUE, new_value);
    }
    return(GX_SUCCESS);
}

