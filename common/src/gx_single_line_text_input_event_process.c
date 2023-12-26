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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_single_line_text_input.h"
#include "gx_text_input_cursor.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_pen_down_process         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handles pen down event for single line text input     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    event_ptr                             Pointer to GX_EVENT structure */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_single_line_text_input_text_rectangle_get                       */
/*                                          Retrieve rectangle from cursor*/
/*                                            to specified offset position*/
/*    _gx_single_line_text_input_position_get                             */
/*                                          Update text insert position   */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_widget_event_process              Default widget event process  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_single_line_text_input_event_process                            */
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
static UINT _gx_single_line_text_input_pen_down_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
GX_RECTANGLE dirty_area;
UINT         start_mark = text_input -> gx_single_line_text_input_start_mark;
UINT         end_mark = text_input -> gx_single_line_text_input_end_mark;

    _gx_system_input_capture((GX_WIDGET *)text_input);

    if (start_mark != end_mark)
    {
        /* Retrieve highlight text bounding rectangle. */
        _gx_single_line_text_input_text_rectangle_get(text_input, (INT)(start_mark - end_mark), &dirty_area);

        /* Mark highlight area as dirty. */
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &dirty_area);
    }

    /* Calculate insert position according to click position. */
    _gx_single_line_text_input_position_get(text_input, (UINT)((*event_ptr).gx_event_payload.gx_event_pointdata.gx_point_x));

    /* Set highlight start/end mark to insert position. */
    text_input -> gx_single_line_text_input_start_mark = text_input -> gx_single_line_text_input_insert_pos;
    text_input -> gx_single_line_text_input_end_mark = text_input -> gx_single_line_text_input_insert_pos;

    /* Call the widget default processing.  */
    return _gx_widget_event_process((GX_WIDGET *)text_input, event_ptr);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_pen_drag_process         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handles pen drag event for single line text input     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    event_ptr                             Pointer to GX_EVENT structure */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Retrieve cursor rectangle     */
/*    _gx_single_line_text_input_text_rectangle_get                       */
/*                                          Retrieve rectangle from cursor*/
/*                                            to specified offset position*/
/*    _gx_single_line_text_input_position_get                             */
/*                                          Update text insert position   */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_widget_event_process              Default widget event process  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_single_line_text_input_event_process                            */
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
static UINT _gx_single_line_text_input_pen_drag_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
GX_RECTANGLE dirty_area;
UINT         start_mark = text_input -> gx_single_line_text_input_start_mark;
UINT         end_mark = text_input -> gx_single_line_text_input_end_mark;
GX_VALUE     border_width;
GX_RECTANGLE client;
GX_VALUE     click_x;

    if (text_input -> gx_widget_status & GX_STATUS_OWNS_INPUT)
    {
        click_x = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;

        /* Calculate client rectangle. */
        _gx_widget_border_width_get((GX_WIDGET *)text_input, &border_width);
        _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &client);

        /* Calculate insert position. */
        _gx_single_line_text_input_position_get(text_input, (UINT)click_x);

        /* Update end mark. */
        text_input -> gx_single_line_text_input_end_mark = text_input -> gx_single_line_text_input_insert_pos;

        if ((click_x < client.gx_rectangle_left) && (text_input -> gx_single_line_text_input_end_mark > 0))
        {
            if (!(text_input -> gx_widget_status & (GX_STATUS_MARK_NEXT | GX_STATUS_MARK_PREVIOUS)))
            {
                /* Start a timer to move text right. */
                _gx_system_timer_start((GX_WIDGET *)text_input, GX_MARK_TIMER,
                                       GX_MARK_INTERVAL, GX_MARK_INTERVAL);
            }

            text_input -> gx_widget_status &= ~GX_STATUS_MARK_NEXT;
            text_input -> gx_widget_status |= GX_STATUS_MARK_PREVIOUS;
        }
        else if ((click_x > client.gx_rectangle_right) &&
                 (text_input -> gx_single_line_text_input_end_mark < text_input -> gx_single_line_text_input_string_size))
        {
            if (!(text_input -> gx_widget_status & (GX_STATUS_MARK_NEXT | GX_STATUS_MARK_PREVIOUS)))
            {
                /* Start a timer to move text left. */
                _gx_system_timer_start((GX_WIDGET *)text_input, GX_MARK_TIMER,
                                       GX_MARK_INTERVAL, GX_MARK_INTERVAL);
            }

            text_input -> gx_widget_status &= ~GX_STATUS_MARK_PREVIOUS;
            text_input -> gx_widget_status |= GX_STATUS_MARK_NEXT;
        }
        else
        {
            if (text_input -> gx_widget_status & (GX_STATUS_MARK_NEXT | GX_STATUS_MARK_PREVIOUS))
            {
                _gx_system_timer_stop((GX_WIDGET *)text_input, GX_MARK_TIMER);
                text_input -> gx_widget_status &= ~(GX_STATUS_MARK_NEXT | GX_STATUS_MARK_PREVIOUS);
            }
        }

        if ((start_mark != text_input -> gx_single_line_text_input_end_mark) ||
            (start_mark != end_mark))
        {
            /* Retrieve text bounding rectangle between old and new end mark. */
            _gx_single_line_text_input_text_rectangle_get(text_input, (INT)(end_mark - text_input -> gx_single_line_text_input_end_mark), &dirty_area);

            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &dirty_area);
        }
    }
    else
    {
        _gx_widget_event_process((GX_WIDGET *)text_input, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_event_process            PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes a single line text input event. This function*/
/*    is internally referenced by the gx_single_line_text_input_create    */
/*    function, but is exposed for use by the application in those cases  */
/*    where the application defines a custom single line text input event */
/*    processing function.                                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    event_ptr                             Pointer to GX_EVENT structure */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process              Default widget event process  */
/*    _gx_widget_event_generate             Create a event and send it to */
/*                                            parent                      */
/*    _gx_system_timer_start                Start the system timer        */
/*    _gx_system_timer_stop                 Stop the system timer         */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
/*    _gx_single_line_text_input_keydown_process                          */
/*                                          Keydown event process function*/
/*    _gx_single_line_text_input_position_update                          */
/*                                          Update cursor position by     */
/*                                            insert position             */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added logic to release      */
/*                                            dynamic input buffer,       */
/*                                            resulting in version 6.1.3  */
/*  12-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            modified to always call     */
/*                                            default widget event        */
/*                                            process on a pen up event,  */
/*                                            resulting in version 6.4.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
UINT         status;
GX_WIDGET   *widget = (GX_WIDGET *)text_input;
GX_RECTANGLE dirty_area;
GX_VALUE     blink_interval;
ULONG        old_style;

    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        _gx_widget_event_process(widget, event_ptr);

        if ((text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS) &&
            (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK))
        {
            _gx_system_timer_start((GX_WIDGET *)text_input, ID_TEXT_INPUT_TIMER, GX_CURSOR_BLINK_INTERVAL, GX_CURSOR_BLINK_INTERVAL);
        }

        /* Update cursor position. */
        _gx_single_line_text_input_position_update(text_input);
        break;

    case GX_EVENT_RESIZED:
        _gx_single_line_text_input_position_update(text_input);
        break;

    case GX_EVENT_STYLE_CHANGED:
        if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            old_style = event_ptr -> gx_event_payload.gx_event_ulongdata;
            if ((old_style & (GX_STYLE_BORDER_MASK | GX_STYLE_TEXT_ALIGNMENT_MASK)) !=
                (widget -> gx_widget_style & (GX_STYLE_BORDER_MASK | GX_STYLE_TEXT_ALIGNMENT_MASK)))
            {
                _gx_single_line_text_input_position_update(text_input);
            }
        }
        break;

    case GX_EVENT_KEY_DOWN:
        _gx_single_line_text_input_keydown_process(text_input, event_ptr);

        /*If input was done, send event to parent.  */
        if (widget -> gx_widget_style & GX_STYLE_TEXT_INPUT_NOTIFY_ALL)
        {
            _gx_widget_event_generate(widget, GX_EVENT_TEXT_EDITED, 0);
        }
        break;

    case GX_EVENT_FOCUS_GAINED:
        if (!(text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS))
        {
            text_input -> gx_widget_status |= (GX_STATUS_CURSOR_SHOW | GX_STATUS_CURSOR_DRAW);

            if (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK)
            {
                blink_interval = text_input -> gx_single_line_text_input_cursor_instance.gx_text_input_cursor_blink_interval;
                _gx_system_timer_start(widget, ID_TEXT_INPUT_TIMER, (UINT)blink_interval, (UINT)blink_interval);
            }
        }

        /* Select all text. */
        _gx_single_line_text_input_text_select(text_input, 0, text_input -> gx_single_line_text_input_string_size - 1);

        /* this widget wants to be notified if it is moved or re-sized */
        text_input -> gx_widget_status |= GX_STATUS_RESIZE_NOTIFY;

        /* Call the widget default processing.  */
        status = _gx_widget_event_process(widget, event_ptr);
        break;

    case GX_EVENT_PEN_DOWN:
        _gx_single_line_text_input_pen_down_process(text_input, event_ptr);
        break;

    case GX_EVENT_PEN_DRAG:
        _gx_single_line_text_input_pen_drag_process(text_input, event_ptr);
        break;

    case GX_EVENT_PEN_UP:
        if (text_input -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release((GX_WIDGET *)text_input);

            if (text_input -> gx_widget_status & (GX_STATUS_MARK_NEXT | GX_STATUS_MARK_PREVIOUS))
            {
                _gx_system_timer_stop((GX_WIDGET *)text_input, GX_MARK_TIMER);
                text_input -> gx_widget_status &= ~(GX_STATUS_MARK_NEXT | GX_STATUS_MARK_PREVIOUS);
            }
        }

        _gx_widget_event_process(widget, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_MARK_TIMER)
        {
            if (text_input -> gx_widget_status & GX_STATUS_MARK_PREVIOUS)
            {
                _gx_single_line_text_input_mark_previous(text_input);
            }
            else
            {
                _gx_single_line_text_input_mark_next(text_input);
            }
        }
        else if ((event_ptr -> gx_event_payload.gx_event_timer_id == ID_TEXT_INPUT_TIMER) &&
                 (text_input -> gx_widget_status & GX_STATUS_CURSOR_SHOW))
        {
            if (text_input -> gx_widget_status & GX_STATUS_CURSOR_DRAW)
            {
                text_input -> gx_widget_status &= (ULONG)(~GX_STATUS_CURSOR_DRAW);
            }
            else
            {
                text_input -> gx_widget_status |= GX_STATUS_CURSOR_DRAW;
            }

            _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_single_line_text_input_cursor_instance, &dirty_area);
            _gx_system_dirty_partial_add(widget, &dirty_area);
        }
        break;

    case GX_EVENT_FOCUS_LOST:
        if (!(text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS))
        {
            if (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK)
            {
                _gx_system_timer_stop(widget, ID_TEXT_INPUT_TIMER);
            }
            text_input -> gx_widget_status &= (ULONG)(~GX_STATUS_CURSOR_SHOW);

            if (text_input -> gx_single_line_text_input_start_mark == text_input -> gx_single_line_text_input_end_mark)
            {
                _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_single_line_text_input_cursor_instance, &dirty_area);
                _gx_system_dirty_partial_add(widget, &dirty_area);
            }

            /* If cursor always is not set, do-not receive resize notify when lose focus */
            text_input -> gx_widget_status &= ~GX_STATUS_RESIZE_NOTIFY;
        }

        if (text_input -> gx_single_line_text_input_start_mark != text_input -> gx_single_line_text_input_end_mark)
        {
            _gx_single_line_text_input_text_rectangle_get(text_input,
                                                          (INT)(text_input -> gx_single_line_text_input_start_mark - text_input -> gx_single_line_text_input_end_mark), &dirty_area);

            text_input -> gx_single_line_text_input_start_mark = 0;
            text_input -> gx_single_line_text_input_end_mark = 0;

            _gx_system_dirty_partial_add(widget, &dirty_area);
        }

        /*If string was modified, send event to parent.  */
        if (text_input -> gx_single_line_text_input_was_modified)
        {
            _gx_widget_event_generate(widget, GX_EVENT_TEXT_EDITED, 0);
            text_input -> gx_single_line_text_input_was_modified = GX_FALSE;
        }

        /* Call the widget default processing.  */
        status = _gx_widget_event_process(widget, event_ptr);
        break;

    case GX_EVENT_COPY:
        _gx_single_line_text_input_copy(text_input);
        break;

    case GX_EVENT_CUT:
        _gx_single_line_text_input_cut(text_input);
        break;

    case GX_EVENT_PASTE:
        _gx_single_line_text_input_paste(text_input);
        break;

    case GX_EVENT_MARK_NEXT:
        _gx_single_line_text_input_mark_next(text_input);
        break;

    case GX_EVENT_MARK_PREVIOUS:
        _gx_single_line_text_input_mark_previous(text_input);
        break;

    case GX_EVENT_MARK_HOME:
        _gx_single_line_text_input_mark_home(text_input);
        break;

    case GX_EVENT_MARK_END:
        _gx_single_line_text_input_mark_end(text_input);
        break;

    case GX_EVENT_DELETE:
        if (text_input -> gx_widget_status & GX_STATUS_DYNAMIC_BUFFER)
        {
            if (!_gx_system_memory_free)
            {
                return GX_SYSTEM_MEMORY_ERROR;
            }

            _gx_system_memory_free(text_input -> gx_single_line_text_input_buffer);
            text_input -> gx_single_line_text_input_buffer = GX_NULL;
        }
        break;

    default:
        /* Call the widget default processing.  */
        status = _gx_widget_event_process(widget, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

