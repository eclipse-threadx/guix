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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_event_dispatch                           PORTABLE C      */
/*                                                           6.2.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function dispatches new system events to the appropriate       */
/*    widget(s).                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    in_event                              New event                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_top_root_find              Find the root widget          */
/*    _gx_system_top_widget_find            Find top widget               */
/*    _gx_system_focus_claim                Mark the widget to receive    */
/*                                            GUIX input focus            */
/*    [gx_widget_event_process_function]    Widget's event processing     */
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
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.11 */
/*  03-08-2023     Ting Zhu                 Modified comment(s), fixed    */
/*                                            a gcc warning,              */
/*                                            resulting in version 6.2.1  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_system_event_dispatch(GX_EVENT *in_event)
{

GX_WIDGET      *target = GX_NULL;
GX_WINDOW_ROOT *root_window = GX_NULL;
GX_EVENT        out_event;
GX_POINT        pen_pos;
UINT            return_code = 0;

#if defined(GX_MOUSE_SUPPORT)
GX_DISPLAY *display;
#endif
    /* check for NULL event. This happens when an event is purged */
    if (in_event -> gx_event_type == 0)
    {
        return 0;
    }

    /* copy the event to dispatch */
    out_event = *in_event;

    /* is this event targetted to a particular widget? */
    if (out_event.gx_event_target)
    {
        target = out_event.gx_event_target;
        return_code = target -> gx_widget_event_process_function(target,  &out_event);
    }
    else
    {
        switch (out_event.gx_event_type)
        {
#if defined(GX_MOUSE_SUPPORT)
        case GX_EVENT_PEN_MOVE:
            /* Find the top root window under this click position */
            pen_pos = out_event.gx_event_payload.gx_event_pointdata;
            root_window = _gx_system_top_root_find(&out_event);

            if (root_window)
            {
                /* mouse cursor coordinates are display relative, not canvas relative,
                   so set cursor position before canvas offset is applied
                 */
                display = root_window -> gx_window_root_canvas -> gx_canvas_display;
                if (display -> gx_display_mouse_position_set)
                {
                    display -> gx_display_mouse_position_set(display, &pen_pos);
                }
            }
            break;
#endif

        case GX_EVENT_PEN_DOWN:
        case GX_EVENT_PEN_UP:
        case GX_EVENT_PEN_DRAG:

            /* get absolute click position */
            pen_pos = out_event.gx_event_payload.gx_event_pointdata;

            if (_gx_system_capture_count > 0)
            {
                /* Get the widget that owns the system input.  */
                target = *_gx_system_input_capture_stack;

                if (target)
                {
                    
                    /* Find the root window of the widget that owns the system input. */
                    root_window = (GX_WINDOW_ROOT *)target -> gx_widget_parent;
                    while (root_window && root_window -> gx_widget_parent)
                    {
                        root_window = (GX_WINDOW_ROOT *)root_window -> gx_widget_parent;
                    }
                }
            }
            else
            {
                /* Find the top root window under this click position */
                root_window = _gx_system_top_root_find(&out_event);
            }

            if (root_window)
            {
#if defined(GX_MOUSE_SUPPORT)
                /* mouse cursor coordinates are display relative, not canvas relative,
                   so set cursor position before canvas offset is applied
                 */
                display = root_window -> gx_window_root_canvas -> gx_canvas_display;
                if (display -> gx_display_mouse_position_set)
                {
                    display -> gx_display_mouse_position_set(display, &pen_pos);
                }
#endif

                /* adjust the pen position by the canvas offset */
                pen_pos.gx_point_x =
                    (GX_VALUE)(pen_pos.gx_point_x - root_window -> gx_window_root_canvas -> gx_canvas_display_offset_x);
                pen_pos.gx_point_y =
                    (GX_VALUE)(pen_pos.gx_point_y - root_window -> gx_window_root_canvas -> gx_canvas_display_offset_y);

                if (!target)
                {
                    /* find the child of this root under the click position */
                    target = _gx_system_top_widget_find((GX_WIDGET *)root_window, pen_pos, GX_STATUS_SELECTABLE);
                }
            }

            /* Was a widget found?  */
            if (target)
            {
                out_event.gx_event_payload.gx_event_pointdata = pen_pos;
                out_event.gx_event_target = target;

                if (out_event.gx_event_type == GX_EVENT_PEN_DOWN)
                {
                    _gx_system_focus_claim(target);
                }

                /* Yes, a widget was found, call it's event notification function.  */
                return_code = target -> gx_widget_event_process_function(target, &out_event);
            }
            break;

        default:
            if (_gx_system_focus_owner)
            {
                return_code = _gx_system_focus_owner -> gx_widget_event_process_function(
                        _gx_system_focus_owner, &out_event);
            }
            break;
        }
    }
    return return_code;
}

