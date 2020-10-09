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
/**   Scroll Management (Scroll Thumb)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_scrollbar.h"
#include "gx_button.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_thumb_event_process                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for a scrollbar thumb button         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scroll_thumb                          Scroll thumb widget control   */
/*                                            block                       */
/*    event                                 Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_button_event_process              Default button event process  */
/*    _gx_system_input_capture              Default widget event process  */
/*    _gx_system_input_release              Direct all input events       */
/*    _gx_widget_shift                      Shift the widget              */
/*    _gx_widget_height_get                 Get the height of widget      */
/*    _gx_scroll_thumb_shift_limit          Limit the travel of the       */
/*                                            scrollbar thumb button      */
/*    _gx_widget_width_get                  Get the width of widget       */
/*    [_gx_widget_event_process_function]   Widget event process routine  */
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
UINT  _gx_scroll_thumb_event_process(GX_SCROLL_THUMB *thumb, GX_EVENT *event_ptr)
{
GX_EVENT   newevent;
GX_WIDGET *widget = (GX_WIDGET *)thumb;
UINT       status;
INT        shift;
GX_VALUE   widget_height;
GX_VALUE   widget_width;

    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        status = _gx_widget_event_process(widget, event_ptr);
        if (thumb -> gx_scroll_thumb_pixelmap)
        {
            if (_gx_widget_transparent_pixelmap_detect(widget, thumb -> gx_scroll_thumb_pixelmap))
            {
                _gx_widget_status_add(widget, GX_STATUS_TRANSPARENT);
            }
        }
        break;

    case GX_EVENT_PEN_DOWN:
        _gx_system_input_capture(widget);

        thumb -> gx_scroll_thumb_drag_mode = GX_TRUE;

        if (widget -> gx_widget_style & GX_SCROLLBAR_VERTICAL)
        {
            thumb -> gx_scroll_thumb_start_drag = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y;
        }
        else
        {
            thumb -> gx_scroll_thumb_start_drag = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;
        }
        break;

    case GX_EVENT_PEN_UP:
        if (widget ->gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release(widget);
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if (thumb -> gx_widget_style & GX_SCROLLBAR_VERTICAL)
        {
            if (event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y != thumb -> gx_scroll_thumb_start_drag)
            {
                shift = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y - thumb -> gx_scroll_thumb_start_drag;
                shift = _gx_scroll_thumb_shift_limit(thumb, shift);

                if (shift)
                {
                    _gx_widget_shift(widget, 0, (GX_VALUE)shift, GX_TRUE);
                    thumb -> gx_scroll_thumb_start_drag = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y;
                    newevent.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
                    newevent.gx_event_payload.gx_event_intdata[0] = widget -> gx_widget_size.gx_rectangle_top;

                    _gx_widget_height_get(widget, &widget_height);
                    newevent.gx_event_payload.gx_event_intdata[1] = (INT)widget_height;

                    newevent.gx_event_sender = GX_ID_SCROLL_THUMB;

                    /* send the scroll event to my parent scrollbar */
                    thumb -> gx_widget_parent -> gx_widget_event_process_function(thumb -> gx_widget_parent, &newevent);
                }
            }
        }
        else
        {
            if (event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x != thumb -> gx_scroll_thumb_start_drag)
            {
                shift = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x - thumb -> gx_scroll_thumb_start_drag;
                shift = _gx_scroll_thumb_shift_limit(thumb, shift);

                if (shift)
                {
                    _gx_widget_shift(widget, (GX_VALUE)shift, 0, GX_TRUE);
                    thumb -> gx_scroll_thumb_start_drag = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;
                    newevent.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
                    newevent.gx_event_payload.gx_event_intdata[0] = widget -> gx_widget_size.gx_rectangle_left;

                    _gx_widget_width_get(widget, &widget_width);
                    newevent.gx_event_payload.gx_event_intdata[1] = (INT)widget_width;
                    newevent.gx_event_sender = GX_ID_SCROLL_THUMB;

                    /* send the scroll event to my parent scrollbar */
                    widget -> gx_widget_parent -> gx_widget_event_process_function(thumb -> gx_widget_parent, &newevent);
                }
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

