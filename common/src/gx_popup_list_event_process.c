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
/**   Popup List (List)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_popup_list_event_process                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes an event for the vertical list.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    popup_list                            Popup List widget control     */
/*                                            block                       */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_vertical_list_event_process       Process vertical list events  */
/*    [gx_widget_event_process_function]    Widget-specified event        */
/*                                            process routine             */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*                                                                        */
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
/*                                                                        */
/**************************************************************************/

UINT  _gx_popup_list_event_process(GX_POPUP_LIST *popup_list, GX_EVENT *event_ptr)
{
GX_WIDGET        *owner;
GX_EVENT          send_event;

GX_VERTICAL_LIST *list = &popup_list -> gx_popup_list_list;

    owner = popup_list -> gx_popup_list_owner;

    memset(&send_event, 0, sizeof(GX_EVENT));

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_FOCUS_LOST:
        send_event.gx_event_type = GX_EVENT_CLOSE_POPUP;
        owner -> gx_widget_event_process_function(owner, &send_event);
        break;

    case GX_EVENT_PEN_UP:
        /*Only close popup list after click event. If PEN_UP event is following PEN_DRAG, pen index is -1, and list should not be closed.*/
        if (list -> gx_vertical_list_pen_index >= 0)
        {
            send_event.gx_event_type = GX_EVENT_CLOSE_POPUP;
        }
        _gx_vertical_list_event_process(list, event_ptr);
        if (send_event.gx_event_type)
        {
            owner->gx_widget_event_process_function(owner, &send_event);
            _gx_system_dirty_mark(owner);
        }
        break;

    default:
        _gx_vertical_list_event_process(list, event_ptr);
    }

    return 0;
}

