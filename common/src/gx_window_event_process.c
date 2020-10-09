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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_event_process                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified window.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Window's widget control block */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_shift                      Shift a widget                */
/*    _gx_window_view_update_detect         Detect changes to the window  */
/*                                            viewport                    */
/*    _gx_widget_event_process              Call widget event processing  */
/*    _gx_window_cilent_scroll              Scroll window client area     */
/*    _gx_widget_front_move                 Move window to front          */
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
UINT  _gx_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{

UINT       status;
GX_WIDGET *widget;
GX_WIDGET *parent;
GX_VALUE   xShift;
GX_VALUE   yShift;

    status = GX_SUCCESS;
    widget = (GX_WIDGET *)window;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        /* if this window is moveable, go into move mode */
        if (window -> gx_widget_status & GX_STATUS_MOVABLE)
        {
            window -> gx_window_move_mode = GX_TRUE;
            window -> gx_window_move_start = event_ptr -> gx_event_payload.gx_event_pointdata;
            /* Return successful status.  */
            return(GX_SUCCESS);
        }
        else
        {
            status =  _gx_widget_event_process(widget, event_ptr);
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if (window -> gx_window_move_mode)
        {
            xShift = (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x -
                                window -> gx_window_move_start.gx_point_x);
            yShift = (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y -
                                window -> gx_window_move_start.gx_point_y);

            window -> gx_window_move_start = event_ptr -> gx_event_payload.gx_event_pointdata;

            _gx_widget_shift(widget, xShift, yShift, GX_TRUE);
        }
        else
        {
            status = _gx_widget_event_process(widget, event_ptr);
        }
        return(status);

    case GX_EVENT_PEN_UP:
        if (window -> gx_window_move_mode)
        {
            window -> gx_window_move_mode = GX_FALSE;
        }
        else
        {
            status = _gx_widget_event_process(widget, event_ptr);
        }
        break;

    case GX_EVENT_FOCUS_GAINED:
        /* first do default handling */

        if (widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS)
        {
            _gx_widget_event_process(widget, event_ptr);

            /* If this window owns focus, try to assign focus to client child */
            if (_gx_system_focus_owner == GX_NULL ||
                _gx_system_focus_owner == widget)
            {
                _gx_widget_child_focus_assign(widget);
            }

            /* Now move window to front.  */
            if (window -> gx_widget_type != GX_TYPE_ROOT_WINDOW)
            {
                if (_gx_widget_front_move(widget, GX_NULL) != GX_NO_CHANGE)
                {
                    /* if I am a top-level window and I moved in front, then
                       the viewport lists need to be updated.
                     */
                    _gx_window_view_update_detect(window);
                }
            }
        }
        break;

    case GX_EVENT_HIDE:
        /* do the default handling */
        status = _gx_widget_event_process(widget, event_ptr);

        /* if this window is child of root, viewports must be updated */
        _gx_window_view_update_detect(window);
        break;

    case GX_EVENT_CLOSE:
        _gx_widget_detach(widget);
        return GX_EVENT_CLOSE;

    case GX_EVENT_SHOW:
        /* do the default handling */
        status = _gx_widget_event_process(widget, event_ptr);

        _gx_widget_nav_order_initialize(widget);

        /* if this window is child of root, viewports must be updated */
        _gx_window_view_update_detect(window);

        /* if this window is in front, claim focus */
        if (widget -> gx_widget_type != GX_TYPE_ROOT_WINDOW &&
            !(widget -> gx_widget_status & GX_STATUS_HAS_FOCUS))
        {
            parent = widget;
            while (parent && widget -> gx_widget_next == GX_NULL)
            {
                parent = widget -> gx_widget_parent;

                if (parent)
                {
                    if (parent -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
                    {
                        _gx_system_focus_claim((GX_WIDGET *)window);
                        break;
                    }
                    widget = parent;
                    parent = widget -> gx_widget_parent;
                }
            }
        }

        break;

    case GX_EVENT_VERTICAL_SCROLL:
        _gx_window_scroll(window, 0, (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_intdata[1] - event_ptr -> gx_event_payload.gx_event_intdata[0]));
        break;

    case GX_EVENT_HORIZONTAL_SCROLL:
        _gx_window_scroll(window, (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_intdata[1] - event_ptr -> gx_event_payload.gx_event_intdata[0]), 0);
        break;

    default:

        /* Call the widget default processing.  */
        status =  _gx_widget_event_process(widget, event_ptr);
    }
    /* Return widget event processing status.  */
    return(status);
}

