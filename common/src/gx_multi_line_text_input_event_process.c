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
/**   Multi Line Text Input Management (Multi Line Text Input)            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_pen_down_process          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handles pen down event for multi line text input      */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi-line text input widget  */
/*                                            control block               */
/*    event_ptr                             Pointer to GX_EVENT structure */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_input_capture              Direct all input events to    */
/*                                            specified widget            */
/*    _gx_multi_line_text_input_highlight_rectangle_get                   */
/*                                          Retrieve rectangle of         */
/*                                            highlight text              */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Retrieve cursor rectangle     */
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*                                          Update text insert position   */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_system_dirty_mark                 Mark specified widget dirty   */
/*    _gx_window_event_process              Default window event process  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_event_process                             */
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
static UINT _gx_multi_line_text_input_pen_down_process(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
INT                   scroll_shift;
GX_RECTANGLE          cursor_rect;
UINT                  start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_multi_line_text_input_end_mark;

    _gx_system_input_capture((GX_WIDGET *)text_input);

    scroll_shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

    if (start_mark != end_mark)
    {
        _gx_multi_line_text_input_highlight_rectangle_get(text_input, &cursor_rect);
    }
    else
    {
        /* Record cursor rectangle and scroll shift value before recaculate cursor position. */
        _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &cursor_rect);
    }

    /* Calculate cursor position according to click position. */
    _gx_multi_line_text_input_cursor_pos_calculate(text_input, (*event_ptr).gx_event_payload.gx_event_pointdata);

    /* Set highlight start/end mark to insert position. */
    text_input -> gx_multi_line_text_input_start_mark = text_input -> gx_multi_line_text_input_text_insert_position;
    text_input -> gx_multi_line_text_input_end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

    if (scroll_shift != text_input -> gx_multi_line_text_view_text_scroll_shift)
    {
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }
    else
    {
        if (start_mark != end_mark)
        {
            /* Mark highlight area as dirty. */
            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
        }
        else
        {
            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);

            /* Get current cursor rectangle. */
            _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &cursor_rect);

            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
        }
    }

    return _gx_window_event_process((GX_WINDOW *)text_input, event_ptr);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_pen_drag_process          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handles pen drag event for multi line text input      */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi-line text input widget  */
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
/*    _gx_multi_line_text_input_text_rectangle_get                        */
/*                                          Retrieve text rectangle       */
/*                                            between specified position  */
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*                                          Update text insert position   */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_system_timer_start                Start a timer                 */
/*    _gx_system_timer_stop                 Stop a timer                  */
/*    _gx_windwo_event_process              Default window event process  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_event_process                             */
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
static UINT _gx_multi_line_text_input_pen_drag_process(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
INT                   scroll_shift;
GX_RECTANGLE          cursor_rect;
GX_POINT              start_pos;
UINT                  old_end_mark;
GX_RECTANGLE          client;
GX_POINT              click_pos;
GX_VALUE              click_y;

    if (text_input -> gx_widget_status & GX_STATUS_OWNS_INPUT)
    {
        client = text_input -> gx_window_client;

        click_pos = event_ptr -> gx_event_payload.gx_event_pointdata;
        click_y = click_pos.gx_point_y;

        if (text_input -> gx_multi_line_text_input_start_mark == text_input -> gx_multi_line_text_input_end_mark)
        {
            _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &cursor_rect);

            /* Mark cursor area as dirty. */
            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
        }

        start_pos = cursor_ptr -> gx_text_input_cursor_pos;

        scroll_shift = text_input -> gx_multi_line_text_view_text_scroll_shift;
        old_end_mark = text_input -> gx_multi_line_text_input_end_mark;

        if (click_pos.gx_point_y < client.gx_rectangle_top)
        {
            click_pos.gx_point_y = client.gx_rectangle_top;
        }
        else if (click_pos.gx_point_y > client.gx_rectangle_bottom)
        {
            click_pos.gx_point_y = client.gx_rectangle_bottom;
        }
        _gx_multi_line_text_input_cursor_pos_calculate(text_input, click_pos);

        text_input -> gx_multi_line_text_input_end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

        if ((click_y < client.gx_rectangle_top) &&
            (text_input -> gx_multi_line_text_input_text_cursor_line > 1))
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
        else if ((click_y > client.gx_rectangle_bottom) &&
                 (text_input -> gx_multi_line_text_input_text_cursor_line < text_input -> gx_multi_line_text_view_text_total_rows))
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

        if (scroll_shift != text_input -> gx_multi_line_text_view_text_scroll_shift)
        {
            _gx_system_dirty_mark((GX_WIDGET *)text_input);
        }
        else
        {
            if ((text_input -> gx_multi_line_text_input_start_mark != text_input -> gx_multi_line_text_input_end_mark) ||
                (text_input -> gx_multi_line_text_input_start_mark != old_end_mark))
            {
                _gx_multi_line_text_input_text_rectangle_get(text_input,
                                                             start_pos,
                                                             cursor_ptr -> gx_text_input_cursor_pos, &cursor_rect);

                /* Mark highlight area as dirty. */
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
            }
        }
    }
    else
    {
        _gx_window_event_process((GX_WINDOW *)text_input, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_event_process             PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified text input widget. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input         */
/*                                            control block               */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process              Default widget event process  */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                          activates it                  */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*    _gx_multi_line_text_input_keydown_process                           */
/*                                          Process the keydown event     */
/*    _gx_widget_event_generate             Generate an event             */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*                                          Calculate cursor position     */
/*                                            according to click positin  */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*                                          Update cursor position        */
/*                                            according to insert position*/
/*    _gx_system_dirty_partial_add          Add a dirty area to the       */
/*                                            specified widget            */
/*    _gx_multi_line_text_view_event_process                              */
/*                                          Invoke the text view event    */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added logic to release      */
/*                                            dynamic input buffer,       */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_multi_line_text_input_event_process(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
GX_TEXT_INPUT_CURSOR    *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)text_input;
GX_WIDGET               *widget = (GX_WIDGET *)text_input;
GX_VALUE                 blink_interval = text_input -> gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_blink_interval;
UINT                     status;
GX_RECTANGLE             cursor_rect;
INT                      scroll_shift;
ULONG                    old_style;

    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        _gx_window_event_process((GX_WINDOW *)text_input, event_ptr);

        /* Update cursor position. */
        _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

        if ((text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS) &&
            (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK))
        {
            /* Start the timer.  */
            _gx_system_timer_start(widget, ID_TEXT_INPUT_TIMER, GX_CURSOR_BLINK_INTERVAL, GX_CURSOR_BLINK_INTERVAL);
        }

        /* Get visible rows. */
        _gx_multi_line_text_view_visible_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)text_input);
        break;

    case GX_EVENT_STYLE_CHANGED:
        if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_multi_line_text_view_event_process((GX_MULTI_LINE_TEXT_VIEW *)text_input, event_ptr);

            old_style = event_ptr -> gx_event_payload.gx_event_ulongdata;
            if ((old_style & (GX_STYLE_BORDER_MASK | GX_STYLE_TEXT_ALIGNMENT_MASK)) !=
                (widget -> gx_widget_style & (GX_STYLE_BORDER_MASK | GX_STYLE_TEXT_ALIGNMENT_MASK)))
            {
                text_input -> gx_multi_line_text_view_line_index_old = GX_TRUE;
            }
        }
        break;

    case GX_EVENT_FOCUS_GAINED:
        /* Call the widget default processing. */
        _gx_widget_event_process(widget, event_ptr);

        /* Do not do anything if the CURSOR_ALWAYS flag is set. */
        if (!(text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS))
        {
            text_input -> gx_widget_status |= (GX_STATUS_CURSOR_SHOW | GX_STATUS_CURSOR_DRAW);

            if (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK)
            {
                /* Start the timer.  */
                _gx_system_timer_start(widget, ID_TEXT_INPUT_TIMER, (UINT)blink_interval, (UINT)blink_interval);
            }
        }

        _gx_multi_line_text_input_text_select(text_input, 0, text_input -> gx_multi_line_text_view_text.gx_string_length - 1);
        break;

    case GX_EVENT_KEY_DOWN:
        _gx_multi_line_text_input_keydown_process(text_input, event_ptr);
        if (widget -> gx_widget_style & GX_STYLE_TEXT_INPUT_NOTIFY_ALL)
        {
            _gx_widget_event_generate(widget, GX_EVENT_TEXT_EDITED, 0);
        }
        break;

    case GX_EVENT_PEN_DOWN:
        _gx_multi_line_text_input_pen_down_process(text_input, event_ptr);
        break;

    case GX_EVENT_PEN_DRAG:
        _gx_multi_line_text_input_pen_drag_process(text_input, event_ptr);
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
        else
        {
            _gx_window_event_process((GX_WINDOW *)text_input, event_ptr);
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_MARK_TIMER)
        {
            if (text_input -> gx_widget_status & GX_STATUS_MARK_PREVIOUS)
            {
                _gx_multi_line_text_input_mark_up(text_input);
            }
            else
            {
                _gx_multi_line_text_input_mark_down(text_input);
            }
        }
        else if ((event_ptr -> gx_event_payload.gx_event_timer_id == ID_TEXT_INPUT_TIMER) &&
                 (text_input -> gx_widget_status & GX_STATUS_CURSOR_SHOW) &&
                 (text_input -> gx_multi_line_text_input_start_mark == text_input -> gx_multi_line_text_input_end_mark))
        {
            if (text_input -> gx_widget_status & GX_STATUS_CURSOR_DRAW)
            {
                text_input -> gx_widget_status &= (ULONG)(~GX_STATUS_CURSOR_DRAW);
            }
            else
            {
                text_input -> gx_widget_status |= GX_STATUS_CURSOR_DRAW;
            }
            /* Define a cursor rectangle for the cursor. */
            _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cursor_rect);
            _gx_system_dirty_partial_add(widget, &cursor_rect);
        }
        break;

    case GX_EVENT_FOCUS_LOST:
        _gx_widget_event_process(widget, event_ptr);

        /* Do not do anything if the CURSOR_ALWAYS flag is set. */
        if (!(text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS))
        {
            text_input ->  gx_widget_status &= (ULONG)(~GX_STATUS_CURSOR_SHOW);

            /* Stop the timer if the cursor style is BLINK. */
            if (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK)
            {
                /* Stop the timer. */
                _gx_system_timer_stop(widget, 0);
            }

            /* Define a cursor rectangle for the cursor. */
            _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cursor_rect);
            _gx_system_dirty_partial_add(widget, &cursor_rect);
        }

        if (text_input -> gx_multi_line_text_input_start_mark != text_input -> gx_multi_line_text_input_end_mark)
        {
            _gx_multi_line_text_input_highlight_rectangle_get(text_input, &cursor_rect);

            text_input -> gx_multi_line_text_input_start_mark = 0;
            text_input -> gx_multi_line_text_input_end_mark = 0;

            _gx_system_dirty_partial_add(widget, &cursor_rect);
        }

        if (text_input -> gx_multi_line_text_input_text_was_modified)
        {
            _gx_widget_event_generate(widget, GX_EVENT_TEXT_EDITED, 0);
            text_input -> gx_multi_line_text_input_text_was_modified = GX_FALSE;
        }
        break;

    case GX_EVENT_VERTICAL_SCROLL:
        /* Pick up scroll shift value. */
        scroll_shift = event_ptr -> gx_event_payload.gx_event_intdata[1] - event_ptr -> gx_event_payload.gx_event_intdata[0];

        /* Move cursor position according to shift value. */
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_y = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y + scroll_shift);

        /* Call the multi-line text view event processing. */
        _gx_multi_line_text_view_event_process(view, event_ptr);
        break;

    case GX_EVENT_LANGUAGE_CHANGE:
        break;

    case GX_EVENT_COPY:
        _gx_multi_line_text_input_copy(text_input);
        break;

    case GX_EVENT_CUT:
        _gx_multi_line_text_input_cut(text_input);
        break;

    case GX_EVENT_PASTE:
        _gx_multi_line_text_input_paste(text_input);
        break;

    case GX_EVENT_MARK_NEXT:
        _gx_multi_line_text_input_mark_next(text_input);
        break;

    case GX_EVENT_MARK_PREVIOUS:
        _gx_multi_line_text_input_mark_previous(text_input);
        break;

    case GX_EVENT_MARK_UP:
        _gx_multi_line_text_input_mark_up(text_input);
        break;

    case GX_EVENT_MARK_DOWN:
        _gx_multi_line_text_input_mark_down(text_input);
        break;

    case GX_EVENT_MARK_END:
        _gx_multi_line_text_input_mark_end(text_input);
        break;

    case GX_EVENT_MARK_HOME:
        _gx_multi_line_text_input_mark_home(text_input);
        break;

    case GX_EVENT_DELETE:
        if (text_input -> gx_widget_status & GX_STATUS_DYNAMIC_BUFFER)
        {
            if (!_gx_system_memory_free)
            {
                return GX_SYSTEM_MEMORY_ERROR;
            }

            _gx_system_memory_free((void *)text_input -> gx_multi_line_text_view_text.gx_string_ptr);
            text_input -> gx_multi_line_text_view_text.gx_string_ptr = GX_NULL;
        }
        break;

    default:
        /* Call the multi-line text view event processing.  */
        status = _gx_multi_line_text_view_event_process(view, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

