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
#include "gx_widget.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_style_add                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service adds a style to the widget.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*    style                                 New style to add              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark     Mark widget dirty if visible              */
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
UINT _gx_widget_style_add(GX_WIDGET *widget, ULONG style)
{
ULONG    old_style = widget -> gx_widget_style;
GX_EVENT my_event;

    /* Update the widget status.  */
    widget -> gx_widget_style |= style;

    if (style & GX_STYLE_TRANSPARENT)
    {
        _gx_widget_status_add(widget, GX_STATUS_TRANSPARENT);
    }

    if (style & GX_STYLE_ENABLED)
    {
        _gx_widget_status_add(widget, GX_STATUS_SELECTABLE);
    }

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark(widget);
    }

    /* notify widget of style modification */

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ulongdata = old_style;
    my_event.gx_event_type = GX_EVENT_STYLE_CHANGED;
    my_event.gx_event_target = widget;
    _gx_system_event_fold(&my_event);

    /* Return the completion status.  */
    return(GX_SUCCESS);
}

