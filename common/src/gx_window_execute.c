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
/*    _gx_window_execute                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function modally executes a window.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Window's widget control block */
/*    return_ptr                            return value                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
UINT  _gx_window_execute(GX_WINDOW *window, ULONG *return_ptr)
{
UINT status = GX_SUCCESS;
UINT return_code = 0;

#ifdef GX_THREADX_BINDING
ULONG    event_memory[GX_EVENT_ULONGS];
#else
GX_EVENT event_memory;
#endif

GX_EVENT  *event_ptr;
GX_WIDGET *widget;

    if (window -> gx_widget_status & GX_STATUS_MODAL)
    {
        return GX_NO_CHANGE;
    }

    _gx_widget_status_add((GX_WIDGET *)window, GX_STATUS_MODAL);

    /* Loop to process GUIX events.  */
    while (!return_code)
    {
#ifdef GX_THREADX_BINDING
        /* Pickup event from event queue.  */
        status =  tx_queue_receive(&_gx_system_event_queue, &event_memory[0], TX_NO_WAIT);

        /* Was there an event?  */
        if (status == TX_QUEUE_EMPTY)
        {
            /* No event to process, so re-draw dirty widgets */
            _gx_system_canvas_refresh();

            /* Now block this thread until an event is received.  */
            status = tx_queue_receive(&_gx_system_event_queue, &event_memory[0], TX_WAIT_FOREVER);
            if (status == TX_SUCCESS)
            {
                status = GX_SUCCESS;
            }
        }
#else
        /* here for generic RTOS binding */
        status = GX_EVENT_POP(&event_memory, GX_FALSE);
        if (status == GX_FAILURE)
        {
            _gx_system_canvas_refresh();
            status = GX_EVENT_POP(&event_memory, GX_TRUE);
        }
#endif

        /* Check for a successful status.  */
        if (status == GX_SUCCESS)
        {
            /* Setup a pointer to the event.  */
            event_ptr =  (GX_EVENT *)(&event_memory);

            switch (event_ptr -> gx_event_type)
            {
            /* Determine if a redraw event is present.  */
            case GX_EVENT_REDRAW:
                /* Yes, a redraw event is present, detect and process the dirty area(s).  */
                widget = (GX_WIDGET *)_gx_system_root_window_created_list;
                while (widget)
                {
                    _gx_system_dirty_mark(widget);
                    widget = widget -> gx_widget_next;
                }
                break;

            case GX_EVENT_TIMER:
                if (event_ptr -> gx_event_target == GX_NULL)
                {
                    /* the event is from gx_system_timer_expiration */
                    _gx_system_timer_update(event_ptr -> gx_event_payload.gx_event_ulongdata);
                }
                else
                {
                    return_code = _gx_system_event_dispatch(event_ptr);
                }
                break;

            case GX_EVENT_PEN_DOWN:
            case GX_EVENT_PEN_UP:
            case GX_EVENT_PEN_DRAG:
                if (_gx_system_capture_count > 0 ||
                    _gx_utility_rectangle_point_detect(&window -> gx_widget_size, event_ptr -> gx_event_payload.gx_event_pointdata))
                {
                    return_code = _gx_system_event_dispatch(event_ptr);
                }
                break;

            case GX_EVENT_TERMINATE:
                _gx_system_event_send(event_ptr);
                return_code = GX_EVENT_TERMINATE;
                break;

            case GX_EVENT_HIDE:
                _gx_system_event_dispatch(event_ptr);
                return_code = GX_EVENT_HIDE;
                break;

            case GX_EVENT_CLOSE:
                _gx_system_event_dispatch(event_ptr);
                return_code = GX_EVENT_CLOSE;
                break;

            case 0:
                /* event has been purged */
                break;

            default:
                /* Dispatch the event to GUIX proper window/widget.  */
                return_code = _gx_system_event_dispatch(event_ptr);
                break;
            }
        }
        else
        {
            /* Error receiving event - call system error handler.  */
            _gx_system_error_process(GX_SYSTEM_EVENT_RECEIVE_ERROR);

            /* Return to exit the system thread.  */

            _gx_widget_status_remove((GX_WIDGET *)window, GX_STATUS_MODAL);

            if (return_ptr)
            {
                *return_ptr = 0;
            }
            return GX_SYSTEM_EVENT_RECEIVE_ERROR;
        }
    }

    _gx_widget_status_remove((GX_WIDGET *)window, GX_STATUS_MODAL);

    if (return_ptr)
    {
        *return_ptr = return_code;
    }

    _gx_widget_detach((GX_WIDGET *)window);
    return GX_SUCCESS;
}

