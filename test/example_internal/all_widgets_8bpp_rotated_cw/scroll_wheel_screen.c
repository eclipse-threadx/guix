/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_8bpp_rotated_cw_resources.h"
#include "all_widgets_8bpp_rotated_cw_specifications.h"

#define ID_SCROLL_WHEEL_DAY   0xf0
#define ID_SCROLL_WHEEL_MONTH 0xf1
#define ID_SCROLL_WHEEL_YEAR  0xf2

extern GX_WINDOW *pScrollWheelScreen;

VOID day_count_update();
VOID animation_speed_set();

/* Define day wheel callback function. */
UINT day_wheel_value_format(GX_NUMERIC_SCROLL_WHEEL* wheel, INT row, GX_STRING* string)
{
    sprintf(wheel->gx_numeric_scroll_wheel_string_buffer, "%02d", row + 1);
    string->gx_string_ptr = wheel->gx_numeric_scroll_wheel_string_buffer;
    string->gx_string_length = 2;
    return GX_SUCCESS;
}

int string_to_int(const char *string)
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
    GX_SINGLE_LINE_TEXT_INPUT *input;
    GX_NUMERIC_SCROLL_WHEEL *day_wheel;

    gx_widget_find(pScrollWheelScreen, ID_DAY_WHEEL, GX_SEARCH_DEPTH_INFINITE, &day_wheel);

    gx_widget_find(pScrollWheelScreen, ID_DAY_INPUT, GX_SEARCH_DEPTH_INFINITE, &input);

    if (input && day_wheel)
    {
        selected_day = string_to_int(input->gx_single_line_text_input_buffer);
        gx_scroll_wheel_selected_set(day_wheel, selected_day - 1);
    }
}

/* Override default event processing of scroll wheel screen. */
UINT scroll_wheel_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        animation_speed_set();
        break;

    case GX_SIGNAL(ID_MONTH_WHEEL, GX_EVENT_LIST_SELECT):
        day_count_update();
        break;

    case GX_SIGNAL(ID_YEAR_WHEEL, GX_EVENT_LIST_SELECT):
        day_count_update();
        break;

    case GX_SIGNAL(ID_DAY_SELECT, GX_EVENT_CLICKED):
        select_day();
        break;

    default:
        return next_button_handler(window, event_ptr);
    }

    return 0;
}

VOID animation_speed_set()
{
    GX_NUMERIC_SCROLL_WHEEL *day_wheel;
    GX_NUMERIC_SCROLL_WHEEL *year_wheel;
    GX_STRING_SCROLL_WHEEL *month_wheel;

    gx_widget_find(pScrollWheelScreen, ID_DAY_WHEEL, GX_SEARCH_DEPTH_INFINITE, &day_wheel);
    gx_widget_find(pScrollWheelScreen, ID_YEAR_WHEEL, GX_SEARCH_DEPTH_INFINITE, &year_wheel);
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    gx_scroll_wheel_speed_set(day_wheel, GX_FIXED_VAL_MAKE(1), 100, 10, 2);
    gx_scroll_wheel_speed_set(month_wheel, (GX_FIXED_VAL_MAKE(1) / 2), 100, 10, 2);
    gx_scroll_wheel_speed_set(year_wheel, GX_FIXED_VAL_MAKE(2), 100, 10, 2);
}

VOID day_count_update()
{
    int year;
    int month;
    int day_count;
    int new_day_count;

    GX_NUMERIC_SCROLL_WHEEL *day_wheel;
    GX_NUMERIC_SCROLL_WHEEL *year_wheel;
    GX_STRING_SCROLL_WHEEL *month_wheel;

    gx_widget_find(pScrollWheelScreen, ID_DAY_WHEEL, GX_SEARCH_DEPTH_INFINITE, &day_wheel);
    gx_widget_find(pScrollWheelScreen, ID_YEAR_WHEEL, GX_SEARCH_DEPTH_INFINITE, &year_wheel);
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

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
