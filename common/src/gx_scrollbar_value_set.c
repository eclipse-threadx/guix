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
/**   Scrollbar Management (Scrollbar)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_value_set                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service resets the scrollbar.                                  */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             Scrollbar control block       */
/*    value                                 New scrollbar value           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_scrollbar_limit_check             Check value limits            */
/*    _gx_scrollbar_thumb_position_calculate                              */
/*                                          Calculate the scrollbar thumb */
/*                                          position                      */
/*    _gx_system_dirty_mark                 Mark system block dirty       */
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
UINT _gx_scrollbar_value_set(GX_SCROLLBAR *scrollbar, INT value)
{
GX_EVENT newevent;
INT old_value;

    old_value = scrollbar -> gx_scrollbar_info.gx_scroll_value;

    scrollbar -> gx_scrollbar_info.gx_scroll_value = value;
    _gx_scrollbar_limit_check(scrollbar);

    if (old_value != scrollbar ->gx_scrollbar_info.gx_scroll_value)
    {
        _gx_scrollbar_thumb_position_calculate(scrollbar);

        newevent.gx_event_payload.gx_event_intdata[0] = scrollbar -> gx_scrollbar_info.gx_scroll_value;
        newevent.gx_event_payload.gx_event_intdata[1] = old_value;
        newevent.gx_event_sender = scrollbar -> gx_widget_id;

        if (scrollbar -> gx_widget_style & GX_SCROLLBAR_VERTICAL)
        {
            newevent.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
        }
        else
        {
            newevent.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
        }
        newevent.gx_event_target = scrollbar -> gx_widget_parent;
        _gx_system_event_send(&newevent);

        if (scrollbar -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_system_dirty_mark((GX_WIDGET *)scrollbar);
        }
    }
    return(GX_SUCCESS);
}

