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
/**   Scroll Wheel Management (Scroll Wheel)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_timer_event_handler                PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to handle timer event for scroll wheel     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    timer_id                              Id of the handled timer       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_start                Start a timer for a widget    */
/*    _gx_system_timer_stop                 Stop a timer for a widget     */
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
/*    _gx_scroll_wheel_scroll               Scroll a scroll wheel widget  */
/*    _gx_widget_event_generate             Create an event and send it   */
/*                                            to parent                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_scroll_wheel_event_process                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            updated with scroll wheel   */
/*                                            control block members,      */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_scroll_wheel_timer_event_handler(GX_SCROLL_WHEEL *wheel, UINT timer_id)
{
GX_WIDGET *widget = (GX_WIDGET *)wheel;
INT        shift;
INT        increment;
GX_BOOL    generate_event = GX_FALSE;

    switch(timer_id)
    {
    case GX_SNAP_TIMER:
        /* Handle snap timer. */
        if (GX_ABS(wheel -> gx_scroll_wheel_selected_yshift) < GX_ABS(wheel -> gx_scroll_wheel_row_height) / 3)
        {
            _gx_system_timer_stop(widget, GX_SNAP_TIMER);
            generate_event = GX_TRUE;
            shift = wheel -> gx_scroll_wheel_selected_yshift;
        }
        else
        {
            if (wheel -> gx_scroll_wheel_selected_yshift > 0)
            {
                shift = wheel -> gx_scroll_wheel_row_height / 3;
            }
            else
            {
                shift = (GX_VALUE)(-wheel -> gx_scroll_wheel_row_height / 3);
            }
        }

        wheel -> gx_scroll_wheel_scroll(wheel, (GX_VALUE)(-shift));
        break;

    case GX_FLICK_TIMER:
        /* Handle flick timer. */

        wheel -> gx_scroll_wheel_animation_steps--;
        shift = wheel -> gx_scroll_wheel_animation_speed;

        if (wheel -> gx_scroll_wheel_shift_error)
        {
            shift += wheel -> gx_scroll_wheel_shift_error;
            wheel -> gx_scroll_wheel_shift_error = 0;
        }

        wheel -> gx_scroll_wheel_scroll(wheel, (GX_VALUE)shift);

        if (wheel -> gx_scroll_wheel_animation_steps > 0)
        {
            increment = (wheel -> gx_scroll_wheel_animation_end_speed - wheel -> gx_scroll_wheel_animation_speed) /
                wheel -> gx_scroll_wheel_animation_steps;

            wheel -> gx_scroll_wheel_animation_speed = (GX_VALUE)(wheel -> gx_scroll_wheel_animation_speed + increment);
        }

        if (!wheel -> gx_scroll_wheel_wrap_style_check(wheel))
        {
            if ((wheel -> gx_scroll_wheel_selected_row == 0 && wheel -> gx_scroll_wheel_selected_yshift > 0) ||
                (wheel -> gx_scroll_wheel_selected_row == wheel -> gx_scroll_wheel_total_rows - 1 &&
                 wheel -> gx_scroll_wheel_selected_yshift < 0))
            {
                wheel -> gx_scroll_wheel_animation_steps = 0;
            }
        }

        if (wheel -> gx_scroll_wheel_animation_steps == 0)
        {
            _gx_system_timer_stop(widget, GX_FLICK_TIMER);

            if (wheel -> gx_scroll_wheel_selected_yshift)
            {
                _gx_system_timer_start((GX_WIDGET *)wheel, GX_SNAP_TIMER, 1, 1);
            }
            else
            {
                generate_event = GX_TRUE;
            }
        }
        break;

    case GX_ANIMATION_TIMER:
        /* Handle animation timer. */
        wheel -> gx_scroll_wheel_animation_steps--;

        if (wheel -> gx_scroll_wheel_shift_error)
        {
            wheel -> gx_scroll_wheel_scroll(wheel, wheel -> gx_scroll_wheel_shift_error);
            wheel -> gx_scroll_wheel_shift_error = 0;
        }

        wheel -> gx_scroll_wheel_scroll(wheel, wheel -> gx_scroll_wheel_animation_speed);

        if (wheel -> gx_scroll_wheel_animation_steps == 0)
        {
            _gx_system_timer_stop(widget, GX_ANIMATION_TIMER);

            if (wheel -> gx_scroll_wheel_selected_yshift)
            {
                _gx_system_timer_start((GX_WIDGET *)wheel, GX_SNAP_TIMER, 1, 1);
            }
            else
            {
                generate_event = GX_TRUE;
            }
        }
        break;
    }

    if (wheel -> gx_widget_id && generate_event)
    {
        _gx_widget_event_generate((GX_WIDGET *)wheel, GX_EVENT_LIST_SELECT, wheel -> gx_scroll_wheel_selected_row);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_flick_event_handler                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to handle flick event for scrol wheel      */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    flick_speed                           Flick speed                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_start                Start a timer for a widget    */
/*    _gx_system_timer_stop                 Stop a timer for a widget     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_scroll_wheel_event_process                                      */
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
static UINT _gx_scroll_wheel_flick_event_handler(GX_SCROLL_WHEEL *wheel, INT flick_speed)
{
INT shift;
INT start_speed;
INT end_speed;
INT total_steps;
INT speed;

    shift = flick_speed / GX_FIXED_VAL_HALF;

    if ((GX_ABS(shift) > 5) &&
        (wheel -> gx_scroll_wheel_total_rows))
    {
        _gx_system_timer_stop((GX_WIDGET *)wheel, GX_SNAP_TIMER);

        if (GX_ABS(shift) < wheel -> gx_scroll_wheel_row_height / 3)
        {
            shift /= GX_ABS(shift);
            shift *= wheel -> gx_scroll_wheel_row_height / 3;
        }
        start_speed = (wheel -> gx_scroll_wheel_animation_start_speed_rate * shift) >> GX_FIXED_VAL_SHIFT;
        end_speed = (wheel -> gx_scroll_wheel_animation_end_speed_rate * shift) >> GX_FIXED_VAL_SHIFT;

        if (GX_ABS(end_speed) < wheel -> gx_scroll_wheel_row_height / 10)
        {
            end_speed = wheel -> gx_scroll_wheel_row_height / 10;

            if (shift < 0)
            {
                end_speed = -end_speed;
            }
        }

        total_steps = GX_ABS(shift) / 2;

        if (total_steps > wheel -> gx_scroll_wheel_animation_max_steps)
        {
            total_steps = wheel -> gx_scroll_wheel_animation_max_steps;
        }

        wheel -> gx_scroll_wheel_animation_speed = (GX_VALUE)start_speed;
        wheel -> gx_scroll_wheel_animation_end_speed = (GX_VALUE)end_speed;
        wheel -> gx_scroll_wheel_animation_steps = (GX_VALUE)total_steps;

        speed = start_speed;
        shift = start_speed;
        while (total_steps > 1)
        {
            total_steps--;
            speed += (end_speed - speed) / total_steps;

            shift += speed;

            if (GX_ABS(shift) > wheel -> gx_scroll_wheel_row_height)
            {
                shift %= wheel -> gx_scroll_wheel_row_height;
            }
        }

        if (start_speed > 0)
        {
            shift += wheel -> gx_scroll_wheel_selected_yshift;
            shift %= wheel -> gx_scroll_wheel_row_height;
            if (shift < 0)
            {
                wheel -> gx_scroll_wheel_shift_error = (GX_VALUE)(-shift);
            }
            else if (shift > 0)
            {
                wheel -> gx_scroll_wheel_shift_error = (GX_VALUE)(wheel -> gx_scroll_wheel_row_height - shift);
            }
        }
        else if (start_speed < 0)
        {
            shift += wheel -> gx_scroll_wheel_selected_yshift;
            shift %= wheel -> gx_scroll_wheel_row_height;

            if (shift < 0)
            {
                wheel -> gx_scroll_wheel_shift_error = (GX_VALUE)(-shift - wheel -> gx_scroll_wheel_row_height);
            }
            else if (shift > 0)
            {
                wheel -> gx_scroll_wheel_shift_error = (GX_VALUE)(-shift);
            }
        }

        /* Start flick timer. */
        _gx_system_timer_start((GX_WIDGET *) wheel, GX_FLICK_TIMER,
                               (UINT)wheel -> gx_scroll_wheel_animation_delay,
                               (UINT)wheel -> gx_scroll_wheel_animation_delay);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_pen_up_event_handler               PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to handle pen up event for scroll wheel    */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_start                Start a timer for a widget    */
/*    _gx_system_dirty_partial_add          Mark partial area of a widget */
/*                                            as dirty                    */
/*    _gx_widget_event_generate             Create an event and send it to*/
/*                                            parent                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_scroll_wheel_event_process                                      */
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
/*                                            GX_STYLE_SCROLL_WHEEL_DRAG  */
/*                                            to GX_STATUS_TRACKING_PEN,  */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_scroll_wheel_pen_up_event_handler(GX_SCROLL_WHEEL *wheel)
{
GX_RECTANGLE dirty;

    wheel -> gx_widget_status &= (ULONG)(~GX_STATUS_TRACKING_PEN);

    if (wheel -> gx_scroll_wheel_selected_yshift)
    {
        /* Start a timer to move selected item to center. */
        _gx_system_timer_start((GX_WIDGET *)wheel, GX_SNAP_TIMER, 1, 1);
    }
    else
    {
        /* Mark center area dirty. */
        dirty = wheel -> gx_window_client;
        dirty.gx_rectangle_top = (GX_VALUE)(dirty.gx_rectangle_top + (dirty.gx_rectangle_bottom - dirty.gx_rectangle_top + 1) / 2);
        dirty.gx_rectangle_top = (GX_VALUE)(dirty.gx_rectangle_top - (wheel -> gx_scroll_wheel_row_height >> 1));
        dirty.gx_rectangle_bottom = (GX_VALUE)(dirty.gx_rectangle_top + wheel -> gx_scroll_wheel_row_height - 1);
        _gx_system_dirty_partial_add((GX_WIDGET *)wheel, &dirty);

        if (wheel -> gx_widget_id)
        {
            /* Generate a list selected event. */
            _gx_widget_event_generate((GX_WIDGET *)wheel, GX_EVENT_LIST_SELECT, wheel -> gx_scroll_wheel_selected_row);
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_event_process                      PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes the comming events for a scroll wheel       */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Text scroll wheel control     */
/*                                            block                       */
/*    event_ptr                             Event to be processed         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_input_capture              Temporarily direct all inputs */
/*                                            to specified widget         */
/*    _gx_system_input_release              Release captured input events */
/*    _gx_window_event_process              Default window event process  */
/*    _gx_widget_event_to_parent            Send event to widget's parent */
/*    _gx_scroll_wheel_scroll               Scroll a scroll wheel widget  */
/*    _gx_scroll_wheel_pen_up_event_handler Handle pen up event           */
/*    _gx_scroll_wheel_flick_event_handler  Handle flick event            */
/*    _gx_scroll_wheel_timer_event_handler  Handle timer event            */
/*    _gx_scroll_wheel_gradient_create      Create a gradient pixelmap    */
/*    _gx_utility_gradient_delete           Delete a gradient             */
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
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            renamed                     */
/*                                            GX_STYLE_SCROLL_WHEEL_DRAG  */
/*                                            to GX_STATUS_TRACKING_PEN,  */
/*                                            resulting in version 6.1.4  */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            updated with scroll wheel   */
/*                                            control block change,       */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT _gx_scroll_wheel_event_process(GX_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr)
{
GX_VALUE shift;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        if (wheel -> gx_scroll_wheel_total_rows)
        {
            _gx_system_input_capture((GX_WIDGET *)wheel);
            wheel -> gx_window_move_start = event_ptr -> gx_event_payload.gx_event_pointdata;
            wheel -> gx_scroll_wheel_shift_error = 0;
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if (wheel -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            wheel -> gx_widget_status |= GX_STATUS_TRACKING_PEN;
            shift = (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y - wheel -> gx_window_move_start.gx_point_y);
            if (shift)
            {
                wheel -> gx_scroll_wheel_scroll(wheel, shift);

                wheel -> gx_window_move_start = event_ptr -> gx_event_payload.gx_event_pointdata;
            }
        }
        else
        {
            _gx_widget_event_to_parent((GX_WIDGET *)wheel, event_ptr);
        }
        break;

    case GX_EVENT_PEN_UP:
        if (wheel -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release((GX_WIDGET *)wheel);
            _gx_scroll_wheel_pen_up_event_handler(wheel);
        }
        else
        {
            _gx_widget_event_to_parent((GX_WIDGET *)wheel, event_ptr);
        }
        break;

    case GX_EVENT_VERTICAL_FLICK:
        _gx_scroll_wheel_flick_event_handler(wheel, event_ptr -> gx_event_payload.gx_event_intdata[0]);
        break;

    case GX_EVENT_TIMER:
        _gx_scroll_wheel_timer_event_handler(wheel, event_ptr -> gx_event_payload.gx_event_timer_id);
        break;

    case GX_EVENT_SHOW:
        _gx_window_event_process((GX_WINDOW *)wheel, event_ptr);
        _gx_scroll_wheel_gradient_create(wheel);
        break;

    case GX_EVENT_DELETE:
        _gx_utility_gradient_delete(&wheel -> gx_scroll_wheel_gradient);
        break;

    default:
        return _gx_window_event_process((GX_WINDOW *)wheel, event_ptr);
    }

    return GX_SUCCESS;
}

