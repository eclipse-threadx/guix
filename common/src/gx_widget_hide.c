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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_window.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_hide                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function hides the widget.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_widget_event_process_function]    Call widget event processing  */
/*    _gx_system_dirty_partial_add          Mark the parent dirty         */
/*    _gx_window_view_update_detect         Update viewports if needed    */
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
UINT _gx_widget_hide(GX_WIDGET *widget)
{
GX_EVENT   hide_event;
GX_WINDOW *win;
GX_WIDGET *next;

    widget -> gx_widget_status |= GX_STATUS_HIDDEN;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Send a GX_EVENT_HIDE event to widget and all children of the widget.  */
        hide_event.gx_event_type = GX_EVENT_HIDE;
        hide_event.gx_event_target = GX_NULL;
        hide_event.gx_event_sender = 0;

        /* Call widget's event processing.  */
        widget -> gx_widget_event_process_function(widget, &hide_event);

        if (widget -> gx_widget_status & GX_STATUS_HAS_FOCUS)
        {
            if(widget -> gx_widget_nav_next == widget)
            {
                /* Current widget is the last widget in navigation order, move focus to its parent. */
                next = widget -> gx_widget_parent;
            }
            else
            {
                /* Move focus to the next widget in navigation order. */
                next  = widget -> gx_widget_nav_next;
            }

            if (next)
            {
                _gx_system_focus_claim(next);
            }
        }

        if (widget -> gx_widget_parent)
        {
            _gx_system_dirty_partial_add(widget -> gx_widget_parent, &widget -> gx_widget_size);

            /* if this widget was part of navigation order, re-create nav order list */
            if (widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS)
            {
                _gx_widget_nav_order_initialize(widget -> gx_widget_parent);
            }
        }

        if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
        {
            win = (GX_WINDOW *)widget;
            if (win -> gx_window_views)
            {
                _gx_system_views_free(win -> gx_window_views);
                win -> gx_window_views = GX_NULL;
            }
            _gx_window_view_update_detect((GX_WINDOW *)widget);
        }

    }


    return(GX_SUCCESS);
}

