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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_border_style_set                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets the border style flags of the widget.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    Style                                 Border style for widget       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
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
UINT  _gx_widget_border_style_set(GX_WIDGET *widget, ULONG Style)
{
GX_WINDOW *win = (GX_WINDOW *)widget;
GX_VALUE   border_width;
GX_EVENT   my_event;
ULONG      old_style = widget -> gx_widget_style;

    /* Set the widget's border style.  */

    widget -> gx_widget_style &= ~GX_STYLE_BORDER_MASK;
    Style &= GX_STYLE_BORDER_MASK;
    widget -> gx_widget_style |= Style;

    if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        /* Pick up border width.  */
        _gx_widget_border_width_get(widget, &border_width);

        /* Get window client.  */
        _gx_widget_client_get(widget, border_width, &win -> gx_window_client);
    }

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ulongdata = old_style;
    my_event.gx_event_type = GX_EVENT_STYLE_CHANGED;
    my_event.gx_event_target = widget;
    _gx_system_event_fold(&my_event);
    return(GX_SUCCESS);
}

