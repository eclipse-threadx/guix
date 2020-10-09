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
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_event_process                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified window.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             Scrollbar control block       */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_center_find     Locate the center of a        */
/*                                             rectangle                  */
/*    [gx_widget_event_process_function]    Parent widget event process   */
/*                                            routine                     */
/*    _gx_scrollbar_thumb_position_calculate                              */
/*                                          Calculate the position of the */
/*                                            thumb                       */
/*    _gx_scrollbar_limit_check             Check for scroll limit        */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*    _gx_scrollbar_value_calcualte         Calculate the value of the    */
/*                                            scrollbar                   */
/*    _gx_scrollbar_reset                   Reset the scrollbar           */
/*    _gx_widget_border_width_get           Retrieve widget border width  */
/*    _gx_widget_child_clipping_update      Update widget child clipping  */
/*                                            area                        */
/*    _gx_widget_type_find                  Find the type of the widget   */
/*    _gx_scrollbar_size_update             Update the size of scrollbar  */
/*    _gx_widget_event_process              Invoke the widget event       */
/*                                            process routine             */
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
UINT  _gx_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr)
{

UINT             status;
GX_EVENT         newevent;
GX_WINDOW       *win;
GX_SCROLLBAR    *scroll_ptr;
GX_SCROLL_THUMB *thumb;
GX_POINT         pen_point;
GX_VALUE         parent_border_width;
INT              temp_value;
GX_VALUE         edge;
GX_RECTANGLE     rect;

    status = GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        rect = scrollbar->gx_widget_size;

        if (scrollbar->gx_scrollbar_upleft.gx_widget_size.gx_rectangle_left !=
            scrollbar->gx_scrollbar_upleft.gx_widget_size.gx_rectangle_right)
        {
            if (scrollbar->gx_widget_style & GX_SCROLLBAR_VERTICAL)
            {
                rect.gx_rectangle_top = (GX_VALUE)(scrollbar->gx_scrollbar_upleft.gx_widget_size.gx_rectangle_bottom + 1);
                rect.gx_rectangle_bottom = (GX_VALUE)(scrollbar->gx_scrollbar_downright.gx_widget_size.gx_rectangle_top - 1);
            }
            else
            {
                rect.gx_rectangle_left = (GX_VALUE)(scrollbar->gx_scrollbar_upleft.gx_widget_size.gx_rectangle_right + 1);
                rect.gx_rectangle_right = (GX_VALUE)(scrollbar->gx_scrollbar_downright.gx_widget_size.gx_rectangle_left - 1);
            }
        }

        /* Calculate valid rectangle here to avoid situation that click on scrollbar end button, since we delivery PEN_DOWN event
           to parent, if we click on scrollbar end buttons, the PEN_DOWN event will also be sent to it's parent, which handles scrollbar
           thumb in another way.  */

        /* save the position */
        pen_point = event_ptr -> gx_event_payload.gx_event_pointdata;

        if (_gx_utility_rectangle_point_detect(&rect, pen_point))
        {
            thumb = &scrollbar -> gx_scrollbar_thumb;
            /* find center of thumb button */
            _gx_utility_rectangle_center_find(&thumb -> gx_widget_size,
                                              &event_ptr -> gx_event_payload.gx_event_pointdata);

            /* pass the even to my scroll thumb */
            thumb -> gx_widget_event_process_function((GX_WIDGET *)thumb, event_ptr);

            /* now pass original position in drag message */
            newevent.gx_event_type = GX_EVENT_PEN_DRAG;
            newevent.gx_event_payload.gx_event_pointdata = pen_point;
            thumb -> gx_widget_event_process_function((GX_WIDGET *)thumb, &newevent);
        }
        /* Return successful status.  */
        return(GX_SUCCESS);

    case GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED):
    case GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED):
        temp_value = scrollbar -> gx_scrollbar_info.gx_scroll_value;

        if (event_ptr -> gx_event_sender == GX_ID_SCROLL_UP_LEFT)
        {
            temp_value = temp_value - scrollbar -> gx_scrollbar_info.gx_scroll_increment;
        }
        else
        {
            temp_value = scrollbar -> gx_scrollbar_info.gx_scroll_value + scrollbar -> gx_scrollbar_info.gx_scroll_increment;
        }
        
        _gx_scrollbar_value_set(scrollbar, temp_value);
        break;

    case GX_EVENT_VERTICAL_SCROLL:
    case GX_EVENT_HORIZONTAL_SCROLL:
        /* this event arrives from my thumb button. Calculate new value
           and send event to parent window
         */
        _gx_scrollbar_value_calculate(scrollbar, event_ptr -> gx_event_payload.gx_event_intdata[0],
                                      event_ptr -> gx_event_payload.gx_event_intdata[1]);
        break;

    case GX_EVENT_PEN_UP:
        break;

    case GX_EVENT_PARENT_SIZED:
        _gx_scrollbar_reset(scrollbar, GX_NULL);
        break;

    case GX_EVENT_HIDE:
        status = _gx_widget_event_process((GX_WIDGET *)scrollbar, event_ptr);

        /* adjust my parent window client size */
        win = (GX_WINDOW *)scrollbar -> gx_widget_parent;
        if (win)
        {
            _gx_widget_border_width_get((GX_WIDGET *)win, &parent_border_width);

            if (scrollbar -> gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
            {
                win -> gx_window_client.gx_rectangle_right = (GX_VALUE)(win -> gx_widget_size.gx_rectangle_right - parent_border_width);
                _gx_widget_child_clipping_update(scrollbar -> gx_widget_parent);
                _gx_widget_type_find(scrollbar -> gx_widget_parent,  GX_TYPE_HORIZONTAL_SCROLL, (GX_WIDGET **)&scroll_ptr);

                if (scroll_ptr)
                {
                    if (scroll_ptr -> gx_widget_status & GX_STATUS_VISIBLE)
                    {
                        _gx_scrollbar_size_update(scroll_ptr);
                    }
                }
            }
            else
            {
                win -> gx_window_client.gx_rectangle_bottom = (GX_VALUE)(win -> gx_widget_size.gx_rectangle_bottom - parent_border_width);
                _gx_widget_child_clipping_update(scrollbar -> gx_widget_parent);
                _gx_widget_type_find(scrollbar -> gx_widget_parent, GX_TYPE_VERTICAL_SCROLL, (GX_WIDGET **)&scroll_ptr);

                if (scroll_ptr)
                {
                    if (scroll_ptr -> gx_widget_status & GX_STATUS_VISIBLE)
                    {
                        _gx_scrollbar_size_update(scroll_ptr);
                    }
                }
            }

            /* notify window of client modification */

            if (win -> gx_widget_status & GX_STATUS_VISIBLE)
            {
                newevent.gx_event_type = GX_EVENT_CLIENT_UPDATED;
                newevent.gx_event_target = (GX_WIDGET *)win;
                _gx_system_event_fold(&newevent);
            }
        }
        break;

    case GX_EVENT_SHOW:
        /* do the default handling */
         status = _gx_widget_event_process((GX_WIDGET *)scrollbar, event_ptr);

        /* test to see if I should be marked transparent */
        if (scrollbar -> gx_scrollbar_appearance.gx_scroll_fill_pixelmap)
        {
            if (_gx_widget_transparent_pixelmap_detect((GX_WIDGET *)scrollbar, scrollbar -> gx_scrollbar_appearance.gx_scroll_fill_pixelmap))
            {
                scrollbar -> gx_widget_status |= GX_STATUS_TRANSPARENT;
            }
        }

        /* adjust my parent window client size */
        win = (GX_WINDOW *) (scrollbar -> gx_widget_parent);

        if (win)
        {
            _gx_widget_border_width_get((GX_WIDGET *)win, &parent_border_width);

            if (scrollbar -> gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
            {
                edge = (GX_VALUE)(win -> gx_widget_size.gx_rectangle_right - parent_border_width);

                if (!(scrollbar -> gx_widget_style & GX_STYLE_TRANSPARENT))
                {
                    edge = (GX_VALUE)(win -> gx_window_client.gx_rectangle_right - scrollbar -> gx_scrollbar_appearance.gx_scroll_width);
                }
                if (win -> gx_window_client.gx_rectangle_right != edge)
                {
                    win -> gx_window_client.gx_rectangle_right = edge;
                    _gx_widget_child_clipping_update((GX_WIDGET *) win);
                }

                /* if we have a horizontal scrollbar and it is already visible, it needs to be adjusted */
                _gx_widget_type_find((GX_WIDGET *)win, GX_TYPE_HORIZONTAL_SCROLL, (GX_WIDGET **)&scroll_ptr);

                if (scroll_ptr)
                {
                    if (scroll_ptr -> gx_widget_status & GX_STATUS_VISIBLE)
                    {
                        _gx_scrollbar_reset(scroll_ptr, GX_NULL);
                    }
                }
            }
            else
            {
                edge = (GX_VALUE)(win -> gx_widget_size.gx_rectangle_bottom - parent_border_width);

                if (!(scrollbar -> gx_widget_style & GX_STYLE_TRANSPARENT))
                {
                    edge = (GX_VALUE)(win -> gx_window_client.gx_rectangle_bottom - scrollbar -> gx_scrollbar_appearance.gx_scroll_width);
                }
                if (win -> gx_window_client.gx_rectangle_bottom != edge)
                {
                    win -> gx_window_client.gx_rectangle_bottom = edge;
                    _gx_widget_child_clipping_update((GX_WIDGET *) win);
                }

                /* if we have a vertical scrollbar and it is already visible, it needs to be adjusted */
                _gx_widget_type_find(scrollbar -> gx_widget_parent, GX_TYPE_VERTICAL_SCROLL, (GX_WIDGET **)&scroll_ptr);

                if (scroll_ptr)
                {
                    if (scroll_ptr -> gx_widget_status & GX_STATUS_VISIBLE)
                    {
                        _gx_scrollbar_reset(scroll_ptr, GX_NULL);
                    }
                }
            }

            /* notify window of client modification */

            newevent.gx_event_type = GX_EVENT_CLIENT_UPDATED;
            newevent.gx_event_target = (GX_WIDGET *) win;
            _gx_system_event_fold(&newevent);
        }
        _gx_scrollbar_reset(scrollbar, GX_NULL);
        break;

    default:

        /* Call the widget default processing.  */
        status =  _gx_widget_event_process((GX_WIDGET *)scrollbar, event_ptr);
    }
    /* Return widget event processing status.  */
    return(status);
}

