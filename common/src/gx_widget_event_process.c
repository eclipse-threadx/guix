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
/*    _gx_widget_children_show_event_process              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to propagate widget show events to the     */
/*    client children of the specified widget first, and then pass the    */
/*    event to the non-client children.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_widget_event_process_function]    Child widget event processing */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_widget_event_process                                            */
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
static VOID  _gx_widget_children_show_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr)
{

GX_WIDGET *child;
GX_WIDGET *start = GX_NULL;
GX_WIDGET *end = GX_NULL;

    /* Pickup the first child of the widget.  */
    child = widget -> gx_widget_first_child;

    /* Loop to send event to all client children of widget.  */
    while (child)
    {
        if (child -> gx_widget_status & GX_STATUS_NONCLIENT)
        {
            if (start == GX_NULL)
            {
                /* Record first non-client widget. */
                start = child;
            }

            /* Record last non-client widget. */
            end = child;
        }
        else
        {
            /* Call widget's event processing.  */
            child -> gx_widget_event_process_function(child, event_ptr);
        }

        /* Move to next child widget.  */
        child = child -> gx_widget_next;
    }

    if (start)
    {
        /* Loop to send event to all non-client children of widget.  */
        while (start != end)
        {
            if (start -> gx_widget_status & GX_STATUS_NONCLIENT)
            {
                /* Call widget's event processing.  */
                start -> gx_widget_event_process_function(start, event_ptr);
            }

            start = start -> gx_widget_next;
        }

        start -> gx_widget_event_process_function(start, event_ptr);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_event_process                            PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified widget.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_children_event_process     Forward event to children     */
/*    [gx_widget_event_process_function]    Widget-specific event process */
/*                                            routine                     */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
/*    _gx_widget_event_to_parent            Signal the parent             */
/*    _gx_widget_event_generate             Generate an event             */
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
/*                                            added new event entries,    */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            avoid pass widget delete    */
/*                                            event to parent,            */
/*                                            resulting in version 6.1.3  */
/*  04-25-2022     Ting Zhu                 Modified comment(s), modified */
/*                                            system input release logic  */
/*                                            on widget hide event,       */
/*                                            resulting in version 6.1.11 */
/*  12-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved focus lose logic,  */
/*                                            resulting in version 6.4.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr)
{
GX_WIDGET *child;
GX_EVENT   new_event;
UINT       status = GX_SUCCESS;
GX_EVENT   input_release_event;

    /* Process relative to the type of event. */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        if (!(widget -> gx_widget_status & (GX_STATUS_VISIBLE | GX_STATUS_HIDDEN)))
        {
            widget -> gx_widget_status |= GX_STATUS_VISIBLE;
            _gx_widget_children_show_event_process(widget, event_ptr);
        }
        break;

    case GX_EVENT_HIDE:
        if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            widget -> gx_widget_status &= ~GX_STATUS_VISIBLE;

            /* Check if the widget still owns system input. */
            if (widget -> gx_widget_status & GX_STATUS_OWNS_INPUT)
            {
                memset(&input_release_event, 0, sizeof(GX_EVENT));
                input_release_event.gx_event_target = widget;
                input_release_event.gx_event_type = GX_EVENT_INPUT_RELEASE;
                widget -> gx_widget_event_process_function(widget, &input_release_event);
            }
            _gx_widget_children_event_process(widget, event_ptr);
        }
        break;

    case GX_EVENT_FOCUS_GAINED:
        /* if I don't already have focus */
        if (!(widget -> gx_widget_status & GX_STATUS_HAS_FOCUS) &&
            widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS)
        {
            /* then status flag indicating I have focus */

            widget -> gx_widget_status |= GX_STATUS_HAS_FOCUS;

            /* and make sure my parent has focus as well */
            if (widget -> gx_widget_parent)
            {
                if (!(widget -> gx_widget_parent -> gx_widget_status & GX_STATUS_HAS_FOCUS))
                {
                    widget -> gx_widget_parent -> gx_widget_event_process_function(widget -> gx_widget_parent, event_ptr);
                }

                /* test to see if this widget should notify it's parent when it gains focus */
                if (widget -> gx_widget_id && widget -> gx_widget_status & GX_STATUS_NOTIFY_ON_GAIN_FOCUS)
                {
                    _gx_widget_event_generate(widget, GX_EVENT_FOCUS_GAIN_NOTIFY, 0);
                }
            }
            if (widget -> gx_widget_style & GX_STYLE_ENABLED)
            {
                widget -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
                _gx_system_dirty_mark(widget);
            }
        }
        break;

    case GX_EVENT_FOCUS_LOST:
        /* if I previously had focus */
        if (widget -> gx_widget_status & GX_STATUS_HAS_FOCUS)
        {
            /* clear focus status flag */
            widget -> gx_widget_status &= ~GX_STATUS_HAS_FOCUS;

            if (widget -> gx_widget_style & GX_STYLE_ENABLED)
            {
                widget -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
                _gx_system_dirty_mark(widget);
            }
        }

        /* and make sure my children don't think they have focus */

        child = widget -> gx_widget_first_child;

        while (child)
        {
            if (child -> gx_widget_status & GX_STATUS_HAS_FOCUS)
            {
                child -> gx_widget_event_process_function(child, event_ptr);
                break;
            }
            child = child -> gx_widget_next;
        }
        break;

    case GX_EVENT_PEN_DOWN:
        if (widget -> gx_widget_status & GX_STATUS_SELECTABLE)
        {
            if (widget -> gx_widget_id > 0)
            {
                _gx_widget_event_generate(widget, GX_EVENT_CLICKED, 0);
            }
        }
        status = _gx_widget_event_to_parent(widget, event_ptr);
        break;


    case GX_EVENT_LANGUAGE_CHANGE:
    case GX_EVENT_RESOURCE_CHANGE:
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    case GX_EVENT_DYNAMIC_BIDI_TEXT_DISABLE:
    case GX_EVENT_DYNAMIC_BIDI_TEXT_ENABLE:
#endif
        /* pass this event down to all my children */
        child = widget -> gx_widget_first_child;
        while (child)
        {
            child -> gx_widget_event_process_function(child, event_ptr);
            child = child -> gx_widget_next;
        }
        break;

    case GX_EVENT_KEY_DOWN:
        memset(&new_event, 0, sizeof(GX_EVENT));

        switch (event_ptr -> gx_event_payload.gx_event_ushortdata[0])
        {
        case GX_KEY_SELECT:
            if (widget -> gx_widget_style & GX_STYLE_ENABLED)
            {
                /* generate pen-down event */
                new_event.gx_event_type = GX_EVENT_SELECT;
                widget -> gx_widget_event_process_function(widget, &new_event);
            }
            break;

        case GX_KEY_NEXT:
            new_event.gx_event_type = GX_EVENT_FOCUS_NEXT;
            new_event.gx_event_sender = widget -> gx_widget_id;
            widget -> gx_widget_event_process_function(widget, &new_event);
            break;

        case GX_KEY_PREVIOUS:
            new_event.gx_event_type = GX_EVENT_FOCUS_PREVIOUS;
            new_event.gx_event_sender = widget -> gx_widget_id;
            widget -> gx_widget_event_process_function(widget, &new_event);
            break;

        default:
            _gx_widget_event_to_parent(widget, event_ptr);
            break;
        }
        break;

    case GX_EVENT_KEY_UP:
        if (widget -> gx_widget_style & GX_STYLE_ENABLED)
        {
            if (event_ptr -> gx_event_payload.gx_event_ushortdata[0] == GX_KEY_SELECT)
            {
                /* generate de-select event */
                memset(&new_event, 0, sizeof(GX_EVENT));
                new_event.gx_event_type = GX_EVENT_DESELECT;
                widget -> gx_widget_event_process_function(widget, &new_event);
            }
            else
            {
                _gx_widget_event_to_parent(widget, event_ptr);
            }
        }
        break;

    case GX_EVENT_FOCUS_NEXT:
        _gx_widget_focus_next(widget);
        break;

    case GX_EVENT_FOCUS_PREVIOUS:
        _gx_widget_focus_previous(widget);
        break;

    case GX_EVENT_INPUT_RELEASE:
        if (widget -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release(widget);
        }
        break;

    case GX_EVENT_STYLE_CHANGED:
    case GX_EVENT_CLIENT_UPDATED:
    case GX_EVENT_PARENT_SIZED:
    case GX_EVENT_RESIZED:
    case GX_EVENT_DELETE:
        break;

    case GX_EVENT_PEN_UP:
    case GX_EVENT_PEN_DRAG:
    default:
        status = _gx_widget_event_to_parent(widget, event_ptr);
        break;
    }

    return(status);
}

