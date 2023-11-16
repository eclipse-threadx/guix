/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_32argb_resources.h"
#include "all_widgets_32argb_specifications.h"

#define ID_SCROLL_WHEEL_DAY   0xf0
#define ID_SCROLL_WHEEL_MONTH 0xf1
#define ID_SCROLL_WHEEL_YEAR  0xf2

VOID select_day();
VOID day_count_update();

/* Override default event processing of scroll wheel screen. */
UINT scroll_wheel_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_YEAR_WHEEL, GX_EVENT_LIST_SELECT):
        day_count_update();
        break;

    case GX_SIGNAL(ID_DAY_SELECT, GX_EVENT_CLICKED):
        select_day();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

int string_to_int(const char* string)
{
    int value = 0;

    while (string[0])
    {
        value = value * 10 + string[0] - '0';
        string++;
    }

    return value;
}

VOID select_day()
{
    int selected_day;
    GX_SINGLE_LINE_TEXT_INPUT* input = &scroll_wheel_screen.scroll_wheel_screen_text_input;
    GX_NUMERIC_SCROLL_WHEEL* day_wheel = &scroll_wheel_screen.scroll_wheel_screen_day_wheel;

    selected_day = string_to_int(input->gx_single_line_text_input_buffer);
    gx_scroll_wheel_selected_set(day_wheel, selected_day - 1);
}

VOID day_count_update()
{
    int year;
    int month;
    int day_count;
    int new_day_count;

    GX_NUMERIC_SCROLL_WHEEL *day_wheel = &scroll_wheel_screen.scroll_wheel_screen_day_wheel;
    GX_NUMERIC_SCROLL_WHEEL *year_wheel = &scroll_wheel_screen.scroll_wheel_screen_year_wheel;
    GX_STRING_SCROLL_WHEEL *month_wheel = &scroll_wheel_screen.scroll_wheel_screen_month_wheel;

    year = year_wheel->gx_numeric_scroll_wheel_start_val;
    year += year_wheel->gx_scroll_wheel_selected_row;

    month = month_wheel->gx_scroll_wheel_selected_row + 1;
    day_count = GX_ABS(day_wheel->gx_numeric_scroll_wheel_end_val - day_wheel->gx_numeric_scroll_wheel_start_val) + 1;

    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        new_day_count = 31;
        break;

    case 2:
        if ((!(year % 400)) ||
            (!(year % 4) && (year % 400)))
        {
            new_day_count = 29;
        }
        else
        {
            new_day_count = 28;
        }
        break;

    default:
        new_day_count = 30;
        break;
    }

    if (new_day_count != day_count)
    {
        gx_numeric_scroll_wheel_range_set(day_wheel, 1, new_day_count);
    }
}
