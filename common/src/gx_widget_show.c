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
/*    _gx_widget_show                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function shows the widget.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_widget_event_process_function]    Call widget event processing  */
/*    _gx_widget_clipping_update            Update the clipping area      */
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
/*    _gx_window_view_update_detect         Detect window view area for   */
/*                                            update                      */
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
UINT  _gx_widget_show(GX_WIDGET *widget)
{
GX_EVENT   show_event;
GX_WINDOW *win;

    show_event.gx_event_target = GX_NULL;
    widget -> gx_widget_status &= ~GX_STATUS_HIDDEN;

    if (!(widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        /* Send a GX_SHOW event to widget and all children of the widget.  */
        show_event.gx_event_type = GX_EVENT_SHOW;

        /* Call widget's event processing.  */
        widget -> gx_widget_event_process_function(widget, &show_event);

        /* update the clipping for this widget */
        _gx_widget_clipping_update(widget);

        _gx_system_dirty_mark(widget);

        if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
        {
            win = (GX_WINDOW *)widget;
            if (win -> gx_window_views)
            {
                _gx_system_views_free(win -> gx_window_views);
                win -> gx_window_views = GX_NULL;
            }
            _gx_window_view_update_detect(win);
        }

        if ((widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS) &&
            widget -> gx_widget_parent)
        {
        	/* if this widget was part of navigation order, re-create nav order list */
            _gx_widget_nav_order_initialize(widget -> gx_widget_parent);
        }
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

