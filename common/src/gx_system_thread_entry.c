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

#if !defined(GX_THREAD_SLEEPING)
#define GX_THREAD_SLEEPING
#endif

#if !defined(GX_THREAD_AWAKE)
#define GX_THREAD_AWAKE
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_thread_entry                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This is the main processing thread for GUIX. All events and drawing */
/*    are done from the context of this thread.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    id                                    Thread ID                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    tx_mutex_get                          Get protection mutex          */
/*    tx_mutex_put                          Release protection mutex      */
/*    tx_queue_receive                      Receive GUIX events from queue*/
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
/*    _gx_system_error_process              Process system errors         */
/*    _gx_system_event_dispatch             Dispatch GUIX events          */
/*    _gx_system_canvas_refresh             Refresh the canvas            */
/*    _gx_system_timer_update               Update active system timers   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    ThreadX                                                             */
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
VOID  _gx_system_thread_entry(ULONG id)
{

UINT status = GX_FAILURE;

#ifdef GX_THREADX_BINDING
ULONG    event_memory[GX_EVENT_ULONGS];
#else
GX_EVENT event_memory;
#endif

GX_EVENT  *event_ptr;
GX_WIDGET *widget;

    GX_PARAMETER_NOT_USED(id);

    /* Loop to process GUIX events.  */
    while (1)
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
            GX_THREAD_SLEEPING
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
            GX_THREAD_SLEEPING
            status = GX_EVENT_POP(&event_memory, GX_TRUE);
        }
#endif

        /* Check for a successful status.  */
        if (status == GX_SUCCESS)
        {
            GX_THREAD_AWAKE
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

            case GX_EVENT_TERMINATE:
                return;

            case GX_EVENT_TIMER:
                if (event_ptr ->gx_event_target == GX_NULL)
                {
                    /* the event is from gx_system_timer_expiration */
                    _gx_system_timer_update(event_ptr -> gx_event_payload.gx_event_ulongdata);
                }
                else
                {
                    _gx_system_event_dispatch(event_ptr);
                }
                break;

            case 0:
                /* the event has been purged */
                break;

            default:
                /* Dispatch the event to GUIX proper window/widget.  */
                _gx_system_event_dispatch(event_ptr);
                break;
            }
        }
        else
        {
            /* Error receiving event - call system error handler.  */
            _gx_system_error_process(GX_SYSTEM_EVENT_RECEIVE_ERROR);

            /* Return to exit the system thread.  */
            return;
        }
    }
}

