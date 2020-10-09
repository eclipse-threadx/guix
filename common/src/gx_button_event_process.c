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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_button_event_process                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified button.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Pointer to button control     */
/*                                            block                       */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [_gx_button_select_handler]           Button select callback        */
/*    _gx_system_input_capture              Assign system input widget    */
/*    _gx_system_input_release              Release system input widget   */
/*    [_gx_button_deselect_handler]         Widget-provided deselect      */
/*                                            handler routine             */
/*    _gx_widget_event_generate             Generate event to notify      */
/*                                             parent widget              */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*    _gx_widget_event_process              Default widget event process  */
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
UINT  _gx_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr)
{
UINT       status;
GX_WIDGET *widget = (GX_WIDGET *)button;
GX_BOOL    generate_event = GX_FALSE;

    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
    case GX_EVENT_SELECT:
        if (widget -> gx_widget_style & GX_STYLE_ENABLED)
        {
            button -> gx_button_select_handler(widget);
            _gx_system_input_capture(widget);
        }

        if (event_ptr -> gx_event_type == GX_EVENT_PEN_DOWN)
        {
            /* Delivery event to parent here, _gx_widget_event_process handle GX_EVENT_PEN_DOWN to
               produce a GX_EVENT_CLICK event, if call _gx_widget_event_process here, 2 GX_EVENT_CLICK events will
               be sent.*/
            status = _gx_widget_event_to_parent(widget, event_ptr);
        }
        break;

    case GX_EVENT_PEN_UP:
        if (widget -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release(widget);

            if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
            {
                if (_gx_utility_rectangle_point_detect(&widget -> gx_widget_size, event_ptr -> gx_event_payload.gx_event_pointdata))
                {
                    generate_event = GX_TRUE;
                }
                if (!(widget -> gx_widget_style & GX_STYLE_BUTTON_RADIO))
                {
                    button -> gx_button_deselect_handler(widget, generate_event);
                }
            }
        }
        status = _gx_widget_event_to_parent(widget, event_ptr);
        break;

    case GX_EVENT_DESELECT:
    case GX_EVENT_INPUT_RELEASE:
        if (widget -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release(widget);

            if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
            {
                if (event_ptr -> gx_event_type == GX_EVENT_DESELECT)
                {
                    generate_event = GX_TRUE;
                }
                if (!(widget -> gx_widget_style & GX_STYLE_BUTTON_RADIO))
                {
                    button -> gx_button_deselect_handler(widget, generate_event);
                }
            }
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_BUTTON_TIMER)
        {
            if (widget -> gx_widget_style & GX_STYLE_BUTTON_REPEAT)
            {
                _gx_widget_event_generate(widget, GX_EVENT_CLICKED, widget -> gx_widget_id);
            }
            else
            {
                _gx_system_timer_stop(widget, GX_BUTTON_TIMER);
            }
        }
        break;

    default:

        /* Call the widget default processing.  */
        status =  _gx_widget_event_process(widget, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

