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
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_pen_down_event_process            PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes pen down event for radial slider.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radial_slider_needle_rectangle_calculate                        */
/*                                          Calcualte needle bounding     */
/*                                            rectangle                   */
/*    _gx_utility_rectangle_point_detect    Test is a point inside a      */
/*                                            rectangle                   */
/*    _gx_utility_math_sqrt                 Compute sqrt value            */
/*    _gx_system_input_capture              Direct all inputs to the      */
/*                                            specified widget            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_radial_slider_event_process                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            renamed                     */
/*                                            GX_STATUS_TRACKING_START to */
/*                                            GX_STATUS_TRACKING_PEN,     */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
static UINT  _gx_radial_slider_pen_down_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr)
{
INT                    xpos;
INT                    ypos;
INT                    dist;
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;
GX_POINT              *point;
GX_RECTANGLE           rectangle;

    if ((slider -> gx_widget_style & GX_STYLE_ENABLED) &&
        (slider -> gx_widget_status & GX_STATUS_ANIMATION_NONE))
    {
        point = &event_ptr -> gx_event_payload.gx_event_pointdata;

        /* Record pen down point. */
        slider -> gx_radial_slider_pendown_point = *point;

        /* Get current needle bounding rectangle. */
        _gx_radial_slider_needle_rectangle_calculate(slider, &rectangle);

        /* Check if click point is inside needle area. */
        if (_gx_utility_rectangle_point_detect(&rectangle, *point))
        {
            slider -> gx_widget_status |= GX_STATUS_TRACKING_PEN;

            _gx_system_input_capture((GX_WIDGET *)slider);
        }
        else
        {
            /* Check if the click down point is inside tracking area. */
            xpos = point -> gx_point_x - slider -> gx_widget_size.gx_rectangle_left;
            ypos = point -> gx_point_y - slider -> gx_widget_size.gx_rectangle_top;


            /* Calculate the distacnce from click point to tracking center. */
            dist = (INT)_gx_utility_math_sqrt((UINT)((xpos - info -> gx_radial_slider_info_xcenter) * (xpos - info -> gx_radial_slider_info_xcenter) +
                                                     (ypos - info -> gx_radial_slider_info_ycenter) * (ypos - info -> gx_radial_slider_info_ycenter)));

            if ((info -> gx_radial_slider_info_radius - info -> gx_radial_slider_info_track_width / 2 <= dist) &&
                (dist <= info -> gx_radial_slider_info_radius + info -> gx_radial_slider_info_track_width / 2))
            {
                /* Click point is inside tracking area.  */
                _gx_system_input_capture((GX_WIDGET *)slider);
            }
        }
    }
    else
    {
        _gx_widget_event_to_parent((GX_WIDGET *)slider, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_pen_drag_event_process            PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes pen drag event for radial slider.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    event_ptr                             Pointer to event process      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radial_slider_value_calculate     Calculate needle value        */
/*                                            according to point value    */
/*    _gx_system_dirty_mark                 Mark widget as dirty          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_radial_slider_event_process                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            renamed                     */
/*                                            GX_STATUS_TRACKING_START to */
/*                                            GX_STATUS_TRACKING_PEN,     */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
static UINT  _gx_radial_slider_pen_drag_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr)
{
GX_VALUE               new_value;
GX_VALUE               previous_value;
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;
GX_BOOL                update = GX_FALSE;

    /* move my needle */
    if (slider -> gx_widget_status & GX_STATUS_TRACKING_PEN)
    {
        /* Calcualte new slider value. */
        _gx_radial_slider_angle_calculate(slider, event_ptr -> gx_event_payload.gx_event_pointdata, &new_value);

        /* Check if the new value is inside valide range. */
        if ((new_value >= info -> gx_radial_slider_info_min_angle) &&
            (new_value <= info -> gx_radial_slider_info_max_angle))
        {
            /* Calculate previous value. */
            _gx_radial_slider_angle_calculate(slider, slider -> gx_radial_slider_pendown_point, &previous_value);

            if ((previous_value < new_value) && ((new_value - previous_value) < 180))
            {
                //anti-clockwise
                if (info -> gx_radial_slider_info_current_angle < new_value)
                {
                    update = GX_TRUE;
                }
            }
            else if (previous_value > new_value)
            {
                //clockwise
                if (info -> gx_radial_slider_info_current_angle > new_value)
                {
                    update = GX_TRUE;
                }
            }

            if (update)
            {
                _gx_radial_slider_angle_set(slider, new_value);
            }
        }

        slider -> gx_radial_slider_pendown_point = event_ptr -> gx_event_payload.gx_event_pointdata;
    }
    else
    {
        _gx_widget_event_to_parent((GX_WIDGET *)slider, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_pen_up_event_process              PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes pen up event for radial slider.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    event_ptr                             Pointer to event process      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_input_release              Release previously captured   */
/*                                            input events                */
/*                                          Process pen down event        */
/*    _gx_radial_slider_value_calculate     Calculate needle value        */
/*                                            according to point value    */
/*    _gx_radial_slider_value_set           Set new slider value          */
/*    _gx_widget_event_process              Default widget event process  */
/*    _gx_radial_slider_animation_update    Update needle position        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_radial_slider_event_process                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            renamed                     */
/*                                            GX_STATUS_TRACKING_START to */
/*                                            GX_STATUS_TRACKING_PEN,     */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
static UINT  _gx_radial_slider_pen_up_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr)
{
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;
GX_VALUE               new_value;
GX_POINT              *point;

    if (slider -> gx_widget_status & GX_STATUS_OWNS_INPUT)
    {
        _gx_system_input_release((GX_WIDGET *)slider);

        point = &event_ptr -> gx_event_payload.gx_event_pointdata;

        if (slider -> gx_widget_status & GX_STATUS_TRACKING_PEN ||
            ((GX_ABS(point -> gx_point_x - slider -> gx_radial_slider_pendown_point.gx_point_x) < 5) &&
             (GX_ABS(point -> gx_point_y - slider -> gx_radial_slider_pendown_point.gx_point_y) < 5)))
        {
            if (slider -> gx_widget_status & GX_STATUS_TRACKING_PEN)
            {
                new_value = info -> gx_radial_slider_info_current_angle;
            }
            else
            {
                /* Calculate new angle value with current point. */
                _gx_radial_slider_angle_calculate(slider, *point, &new_value);
            }

            _gx_radial_slider_anchor_angle_calculate(slider, &new_value);
            _gx_radial_slider_animation_start(slider, new_value);
        }

        slider -> gx_widget_status |= GX_STATUS_ANIMATION_NONE;
        slider -> gx_widget_status &= ~GX_STATUS_TRACKING_PEN;
    }
    else
    {
        _gx_widget_event_to_parent((GX_WIDGET *)slider, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_event_process                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes a radial slider event.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    event_ptr                             Pointer to event process      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radial_slider_pen_down_event_process                            */
/*                                          Process pen down event        */
/*    _gx_radial_slider_pen_drag_event_process                            */
/*                                          Process pen drag event        */
/*    _gx_radial_slider_pen_up_event_process                              */
/*                                          Process pen up event          */
/*    _gx_widget_event_process              Default widget event process  */
/*    _gx_radial_slider_animation_update    Update needle position        */
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
UINT  _gx_radial_slider_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr)
{
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        _gx_radial_slider_pen_down_event_process(slider, event_ptr);
        break;

    case GX_EVENT_PEN_DRAG:
        _gx_radial_slider_pen_drag_event_process(slider, event_ptr);
        break;

    case GX_EVENT_PEN_UP:
        _gx_radial_slider_pen_up_event_process(slider, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_RADIAL_SLIDER_TIMER)
        {
            /* Update needle position. */
            _gx_radial_slider_animation_update(slider);
        }
        else
        {
            /* Process default event process. */
            _gx_widget_event_process((GX_WIDGET *)slider, event_ptr);
        }
        break;

    default:
        /* Process default event process. */
        return _gx_widget_event_process((GX_WIDGET *)slider, event_ptr);
    }

    return GX_SUCCESS;
}

