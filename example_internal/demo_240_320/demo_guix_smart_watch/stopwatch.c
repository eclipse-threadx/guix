/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define STOPWATCH_TIMER 2

static GX_VALUE    minute = 0;
static GX_VALUE    second = 0;
static GX_VALUE    ticks = 0;
GX_RESOURCE_ID     start_stop_text_id = GX_STRING_ID_START;

void stopwatch_reset();
void stopwatch_update();

/*************************************************************************************/
UINT stopwatch_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch(event_ptr -> gx_event_type)
    {
    case GX_SIGNAL(ID_STOPWATCH_START_STOP, GX_EVENT_CLICKED):
        if (start_stop_text_id == GX_STRING_ID_START)
        {
            start_stop_text_id = GX_STRING_ID_STOP;

            /* Create a stopwatch timer. */
            gx_system_timer_start(window, STOPWATCH_TIMER, 1, 1);
        }
        else
        {
            start_stop_text_id = GX_STRING_ID_START;

            /* Stop stopwatch timer. */
            gx_system_timer_stop(window, STOPWATCH_TIMER);
        }
        break;

    case GX_SIGNAL(ID_STOPWATCH_RESET, GX_EVENT_CLICKED):

        /* Stop timer and reset stopwatch. */
        gx_system_timer_stop(window, STOPWATCH_TIMER);
        stopwatch_reset();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == STOPWATCH_TIMER)
        {
            stopwatch_update();
        }
        break;

    case GX_EVENT_HIDE:

        /* Stop stopwatch timer. */
        gx_system_timer_stop(window, STOPWATCH_TIMER);
        start_stop_text_id = GX_STRING_ID_START;
        gx_window_event_process(window, event_ptr);
        break;

    default:
        return clock_pages_switch_event_handler(window, event_ptr);
    }
    return 0;
}

/*************************************************************************************/
VOID stopwatch_pixelmap_button_draw(GX_PIXELMAP_BUTTON *widget)
{
    INT             x_offset = 0;
    INT             y_offset = 0;
    GX_PIXELMAP    *map;
    GX_RESOURCE_ID  text_id;

    gx_context_pixelmap_get(widget->gx_pixelmap_button_normal_id, &map);

    if (widget->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        x_offset++;
        y_offset++;
    }

    if (map)
    {
        gx_canvas_pixelmap_draw(widget->gx_widget_size.gx_rectangle_left + x_offset,
            widget->gx_widget_size.gx_rectangle_top + y_offset, map);
    }

    switch (widget->gx_widget_id)
    {
    case ID_STOPWATCH_START_STOP:
        text_id = start_stop_text_id;
        break;

    case ID_STOPWATCH_RESET:
        text_id = GX_STRING_ID_RESET;
        break;
    }

    gx_widget_text_id_draw((GX_WIDGET *)widget, widget->gx_widget_normal_fill_color,
        GX_FONT_ID_NORMAL, text_id, x_offset - 2, y_offset - 2);
}

/*************************************************************************************/
void stopwatch_reset()
{

    ticks = 0;
    second = 0;
    minute = 0;

    gx_prompt_text_id_set(&stopwatch_screen.stopwatch_screen_prompt_micro_second, GX_STRING_ID_DOUBLE_ZERO);
    gx_prompt_text_id_set(&stopwatch_screen.stopwatch_screen_prompt_second, GX_STRING_ID_DOUBLE_ZERO);
    gx_prompt_text_id_set(&stopwatch_screen.stopwatch_screen_prompt_minute, GX_STRING_ID_DOUBLE_ZERO);

    start_stop_text_id = GX_STRING_ID_START;
    gx_system_dirty_mark((GX_WIDGET *)&stopwatch_screen.stopwatch_screen_stopwatch_start_stop);
}

/*************************************************************************************/
void stopwatch_update()
{
    GX_CHAR string_buffer[3];
    GX_STRING string;
    int ms;

    string.gx_string_ptr = string_buffer;
    string.gx_string_length = 2;

    if (ticks < GX_TICKS_SECOND)
    {
        ticks++;
        ms = ticks * GX_SYSTEM_TIMER_MS / 10;
        gx_utility_ltoa(ms / 10, string_buffer, 2);
        gx_utility_ltoa(ms % 10, string_buffer + 1, 2);

        gx_prompt_text_set_ext(&stopwatch_screen.stopwatch_screen_prompt_micro_second, &string);
    }
    else if(second < 59)
    {
        ticks = 0;
        second ++;
        gx_prompt_text_id_set(&stopwatch_screen.stopwatch_screen_prompt_micro_second, GX_STRING_ID_DOUBLE_ZERO);

        gx_utility_ltoa(second / 10, string_buffer, 2);
        gx_utility_ltoa(second % 10, string_buffer + 1, 2);

        gx_prompt_text_set_ext(&stopwatch_screen.stopwatch_screen_prompt_second, &string);
    }
    else if(minute < 59)
    {
        second = 0;
        minute ++;
        gx_prompt_text_id_set(&stopwatch_screen.stopwatch_screen_prompt_second, GX_STRING_ID_DOUBLE_ZERO);

        gx_utility_ltoa(minute / 10, string_buffer, 2);
        gx_utility_ltoa(minute % 10, string_buffer + 1, 2);

        gx_prompt_text_set_ext(&stopwatch_screen.stopwatch_screen_prompt_minute, &string);
    }
    else 
    {
        minute = 0;

        gx_prompt_text_id_set(&stopwatch_screen.stopwatch_screen_prompt_minute, GX_STRING_ID_DOUBLE_ZERO);
    }
}
