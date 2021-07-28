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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_animation.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_event_check                      PORTABLE C      */
/*                                                           6.1.8        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that handles incoming events for screen    */
/*    drag animation.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*    event_ptr                             Event to process              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_slide_landing           Move animation screens one    */
/*                                            step toward target position */
/*    _gx_animation_slide_landing_start     Prepare for screen landing    */
/*    _gx_animation_drag_tracking_start     Prepare for screen slide      */
/*                                            animation                   */
/*    _gx_animation_drag_tracking           Shift animation screens       */
/*                                            accordion to pen pos        */
/*    _gx_system_event_send                 Send an event for processing  */
/*    _gx_system_input_capture              Temporarily direct all input  */
/*                                            events to specified widget  */
/*    _gx_system_input_release              Release captured input events */
/*    _gx_system_timer_stop                 Stop a timer for a widget     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_drag_event_process                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  08-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            replaced abs with GX_ABS,   */
/*                                            resulting in version 6.1.8  */
/*                                                                        */
/**************************************************************************/
static UINT  _gx_animation_drag_event_check(GX_ANIMATION *animation, GX_EVENT *event_ptr)
{
GX_ANIMATION_INFO *info = &animation -> gx_animation_info;
GX_RECTANGLE      *size;
INT                delta;
INT                temp;
GX_WIDGET        **stackptr;
GX_EVENT           input_release_event;
INT                shift;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        if (animation -> gx_animation_status == GX_ANIMATION_IDLE)
        {
            _gx_system_input_capture(info -> gx_animation_parent);

            /* Initiate animation information. */
            animation -> gx_animation_status = GX_ANIMATION_SLIDE_TRACKING;

            if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
            {
                animation -> gx_animation_slide_tracking_start_pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y;
                animation -> gx_animation_slide_tracking_current_pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y;
            }
            else
            {
                animation -> gx_animation_slide_tracking_start_pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;
                animation -> gx_animation_slide_tracking_current_pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;
            }
            animation -> gx_animation_slide_target_index_1 = -1;
            animation -> gx_animation_slide_target_index_2 = -1;
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if (animation -> gx_animation_status == GX_ANIMATION_SLIDE_TRACKING)
        {
            if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
            {
                delta = GX_ABS(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y - animation -> gx_animation_slide_tracking_start_pos);
            }
            else
            {
                delta = GX_ABS(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x - animation -> gx_animation_slide_tracking_start_pos);
            }

            if ((animation -> gx_animation_slide_target_index_1 == -1) &&
                (delta > GX_ANIMATION_MIN_SLIDING_DIST))
            {
                /* Start swiping, remove other widgets from input capture stack.  */
                stackptr = _gx_system_input_capture_stack;
                memset(&input_release_event, 0, sizeof(GX_EVENT));
                input_release_event.gx_event_type = GX_EVENT_INPUT_RELEASE;

                while (*stackptr)
                {
                    if (*stackptr != info -> gx_animation_parent)
                    {
                        input_release_event.gx_event_target = *stackptr;
                        _gx_system_event_send(&input_release_event);
                    }
                    stackptr++;
                }

                _gx_animation_drag_tracking_start(animation, event_ptr -> gx_event_payload.gx_event_pointdata);
            }

             _gx_animation_drag_tracking(animation, event_ptr -> gx_event_payload.gx_event_pointdata);
        }
        break;

    case GX_EVENT_PEN_UP:
        if (animation -> gx_animation_status == GX_ANIMATION_SLIDE_TRACKING)
        {
            _gx_system_input_release(info -> gx_animation_parent);
            animation -> gx_animation_status = GX_ANIMATION_IDLE;

            size = &info -> gx_animation_parent -> gx_widget_size;

            delta = animation -> gx_animation_slide_tracking_current_pos - animation -> gx_animation_slide_tracking_start_pos;
            if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
            {
                shift = (size -> gx_rectangle_bottom - size -> gx_rectangle_top + 1) >> 1;
            }
            else
            {
                shift = (size -> gx_rectangle_right - size -> gx_rectangle_left + 1) >> 1;
            }

            if ((GX_ABS(delta) < shift) || (animation -> gx_animation_slide_target_index_2 == -1))
            {
                /* slide back to original when slide distance is less than half screen width/height. */
                if (animation -> gx_animation_slide_target_index_2 >= 0)
                {
                    temp = animation -> gx_animation_slide_target_index_1;
                    animation -> gx_animation_slide_target_index_1 = animation -> gx_animation_slide_target_index_2;
                    animation -> gx_animation_slide_target_index_2 = (GX_VALUE)temp;
                }

                switch (animation -> gx_animation_slide_direction)
                {
                case GX_ANIMATION_SLIDE_LEFT:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_RIGHT;
                    break;

                case GX_ANIMATION_SLIDE_RIGHT:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_LEFT;
                    break;

                case GX_ANIMATION_SLIDE_UP:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_DOWN;
                    break;

                case GX_ANIMATION_SLIDE_DOWN:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_UP;
                    break;
                }
            }

            if (delta)
            {
                _gx_animation_slide_landing_start(animation);
            }
        }
        break;

    case GX_EVENT_HORIZONTAL_FLICK:
    case GX_EVENT_VERTICAL_FLICK:
        if (animation -> gx_animation_status == GX_ANIMATION_SLIDE_LANDING)
        {
            delta = event_ptr -> gx_event_payload.gx_event_intdata[0];

            if (((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_LEFT) && (delta > 0)) ||
                ((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_RIGHT) && (delta < 0)) ||
                ((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_UP) && (delta > 0)) ||
                ((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_DOWN) && (delta < 0)))
            {
                /* landing direction is different to flick direction
                   exchange targets */
                if (animation -> gx_animation_slide_target_index_2 >= 0)
                {
                    temp = animation -> gx_animation_slide_target_index_1;
                    animation -> gx_animation_slide_target_index_1 = animation -> gx_animation_slide_target_index_2;
                    animation -> gx_animation_slide_target_index_2 = (GX_VALUE)temp;
                }

                animation -> gx_animation_status = GX_ANIMATION_IDLE;

                switch (animation -> gx_animation_slide_direction)
                {
                case GX_ANIMATION_SLIDE_LEFT:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_RIGHT;
                    break;

                case GX_ANIMATION_SLIDE_RIGHT:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_LEFT;
                    break;

                case GX_ANIMATION_SLIDE_UP:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_DOWN;
                    break;

                default:
                    animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_UP;
                    break;
                }
            }

            if (delta)
            {
                _gx_animation_slide_landing_start(animation);
            }
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_ANIMATION_SLIDE_TIMER)
        {
            if (animation -> gx_animation_status != GX_ANIMATION_SLIDE_LANDING)
            {
                _gx_system_timer_stop(info -> gx_animation_parent, GX_ANIMATION_SLIDE_TIMER);
                break;
            }

            _gx_animation_slide_landing(animation);
        }
        break;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_event_process                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handles incoming events for screen drag animation.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget control     */
/*                                            block                       */
/*    event_ptr                             Event to process              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_slide_event_check       Handle incoming events for    */
/*                                            screen drag animation       */
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
UINT  _gx_animation_drag_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr)
{
GX_ANIMATION *animation;

    animation = _gx_system_animation_list;

    while (animation)
    {
        if ((animation -> gx_animation_info.gx_animation_parent == widget) &&
            (animation -> gx_animation_original_event_process_function))
        {
            _gx_animation_drag_event_check(animation, event_ptr);

            /* Call original event process function.  */
            animation -> gx_animation_original_event_process_function(widget, event_ptr);
        }

        animation = animation -> gx_animation_next;
    }

    return GX_SUCCESS;
}

