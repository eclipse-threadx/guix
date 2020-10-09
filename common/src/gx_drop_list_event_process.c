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
/**   Drop List Management (List)                                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_scrollbar.h"
#include "gx_drop_list.h"


#define GX_FLICK_TIMER 1001

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_event_process                         PORTABLE C      */
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
/*    drop_list                             Drop list widget control block*/
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_vertical_list_selected_set        Set the list entry at the     */
/*                                          current list index            */
/*    _gx_drop_list_close                   Close a drop list             */
/*    _gx_drop_list_open                    Open a drop list              */
/*    _gx_first_client_child_get            Get the first client child    */
/*    _gx_widget_event_process              Process events for the        */
/*                                          specified window              */
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

UINT _gx_drop_list_event_process(GX_DROP_LIST *drop_list, GX_EVENT *event_ptr)
{
GX_WIDGET *widget;

    widget = (GX_WIDGET *)drop_list;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        _gx_widget_event_process(widget, event_ptr);
        break;

    case GX_EVENT_HIDE:
        if (drop_list -> gx_drop_list_popup_open)
        {
            _gx_drop_list_close(drop_list);
        }
        _gx_widget_event_process(widget, event_ptr);
        break;

    case GX_SIGNAL(ID_DROP_LIST_BUTTON, GX_EVENT_CLICKED):
        if (drop_list -> gx_drop_list_popup_open)
        {
            _gx_drop_list_close(drop_list);
        }
        else
        {
            _gx_drop_list_open(drop_list);
        }
        break;

    case GX_EVENT_CLOSE_POPUP:
        _gx_drop_list_close(drop_list);
        break;

    default:
        return _gx_widget_event_process(widget, event_ptr);
    }
    return(GX_SUCCESS);
}

