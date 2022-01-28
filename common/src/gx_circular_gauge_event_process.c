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
#include "gx_widget.h"
#include "gx_icon.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_circular_gauge.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_event_process                    PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified circular gauge     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    circular_gauge                        Circular gauge control block  */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_circular_gauge_info_get           Retrieve the circular gauge   */
/*                                            information structure.      */
/*    _gx_circular_gauge_info_set           Set the circular gauge info.  */
/*    _gx_widget_event_process              Process event for specified   */
/*                                            widget.                     */
/*    _gx_widget_pixelmap_get               Get needle pixelmap.          */
/*    _gx_circular_gauge_needle_rectangle_calculate                       */
/*                                          Calculate current needle      */
/*                                            rectangle.                  */
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
/*  01-31-2022     Ting Zhu                 Modified comment(s), improved */
/*                                            logic to recalculate needle */
/*                                            rectangle on resize event,  */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_circular_gauge_event_process(GX_CIRCULAR_GAUGE *gauge, GX_EVENT *event_ptr)
{
UINT status = GX_SUCCESS;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        status = _gx_icon_event_process((GX_ICON *)gauge, event_ptr);

        /* Needle source being GX_NULL indicates it's the first time to show. So initialize the needle rectangle. */
        if (gauge -> gx_circular_gauge_needle_source == GX_NULL)
        {
            _gx_widget_pixelmap_get((GX_WIDGET *)gauge, gauge -> gx_circular_gauge_info.gx_circular_gauge_info_needle_pixelmap, &(gauge -> gx_circular_gauge_needle_source));
            _gx_circular_gauge_needle_rectangle_calculate(gauge, gauge -> gx_circular_gauge_current_angle, &(gauge -> gx_circular_gauge_current_needle_rectangle));
        }

        /* Calculate needle increment angle, if any.  */
        _gx_circular_gauge_angle_increment_calculate(gauge);
        break;

    case GX_EVENT_HIDE:
        if (gauge -> gx_circular_gauge_info.gx_circular_gauge_info_animation_steps > 0)
        {
            /* Stop needle animation timer. */
            _gx_system_timer_stop((GX_WIDGET *)gauge, GX_CIRCULAR_GAUGE_TIMER);
        }
        status = _gx_icon_event_process((GX_ICON *)gauge, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_CIRCULAR_GAUGE_TIMER)
        {

            /* Calculate current angle.  */
            gauge -> gx_circular_gauge_animation_step++;

            if (gauge -> gx_circular_gauge_animation_step < gauge -> gx_circular_gauge_info.gx_circular_gauge_info_animation_steps)
            {
                gauge -> gx_circular_gauge_current_angle = gauge -> gx_circular_gauge_start_angle;
                gauge -> gx_circular_gauge_current_angle += GX_FIXED_VAL_TO_INT(gauge -> gx_circular_gauge_angle_increment *
                                                             gauge -> gx_circular_gauge_animation_step);
            }
            else
            {
                gauge -> gx_circular_gauge_current_angle = gauge -> gx_circular_gauge_target_angle;

                /* Stop timer. */
                _gx_system_timer_stop((GX_WIDGET *)gauge, GX_CIRCULAR_GAUGE_TIMER);
                gauge -> gx_circular_gauge_animation_step = 0;

                /* Inform the parent that the gauge animation is completed.  */
                _gx_widget_event_generate((GX_WIDGET *)gauge, GX_EVENT_ANIMATION_COMPLETE, 0);
            }
            _gx_circular_gauge_needle_dirty_mark(gauge);
        }
        else
        {
            status = _gx_widget_event_process((GX_WIDGET *)gauge, event_ptr);
        }
        break;

    case GX_EVENT_RESIZED:

        /* Recalculate needle rectangle. */
        _gx_circular_gauge_needle_rectangle_calculate(gauge, gauge -> gx_circular_gauge_current_angle, &(gauge -> gx_circular_gauge_current_needle_rectangle));
        break;

    default:

        status = _gx_icon_event_process((GX_ICON *)gauge, event_ptr);
    }

    return status;
}

