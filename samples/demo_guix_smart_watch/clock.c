/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define AM                     1
#define PM                     2
#define TIME_LIST_VISIBLE_ROWS 3
#define TIME_SCREEN_TIMER_ID   2

void time_update();

extern GX_WINDOW_ROOT    *root;

typedef struct TIME_TABLE_STRUCT{
    GX_RESOURCE_ID city_name_id;
    UINT     time_hour;
    UINT     time_minute;
    GX_CHAR  time_hour_text[MAX_TIME_TEXT_LENGTH + 1];
    GX_CHAR  time_minute_text[MAX_TIME_TEXT_LENGTH + 1];
    UINT     time_am_pm;
}TIME_TABLE;

TIME_TABLE time_table[] = {
    { GX_STRING_ID_LOS_ANGELES, 6, 46, "6", "46", GX_STRING_ID_TIME_PM},
    { GX_STRING_ID_CHICAGO, 8, 59, "8", "59", GX_STRING_ID_TIME_PM },
    { GX_STRING_ID_NEW_YORK, 9, 46, "9", "46", GX_STRING_ID_TIME_PM },
    { GX_STRING_ID_LONDON, 11, 46, "11", "46", GX_STRING_ID_TIME_AM },
    { GX_STRING_ID_MOSCOW, 4, 46, "4", "46", GX_STRING_ID_TIME_AM },
    { GX_STRING_ID_BEIJING, 9, 46, "9", "46", GX_STRING_ID_TIME_AM },
    { 0, 0, 0, "", "", 0}
};

/* Time list row structure. */
typedef struct TIME_LIST_ROW_STRUCT {
    GX_WIDGET widget;
    GX_MULTI_LINE_TEXT_VIEW city_name;
    GX_PROMPT time_hour;
    GX_PROMPT time_minute;
    GX_PROMPT time_am_pm;
    GX_ICON   dot_up;
    GX_ICON   dot_down;
} TIME_LIST_ROW;

TIME_LIST_ROW row_memory[TIME_LIST_VISIBLE_ROWS + 1];

static SHORT time_second = 0;

/*************************************************************************************/
UINT clock_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_system_timer_start(window, TIME_SCREEN_TIMER_ID, 50, 50);
        return template_main_event_handler(window, event_ptr);

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TIME_SCREEN_TIMER_ID)
        {
            time_update();
        }
        break;
    case GX_EVENT_HIDE:
        gx_system_timer_stop(window, TIME_SCREEN_TIMER_ID);
        return gx_window_event_process(window, event_ptr);

    case GX_SIGNAL(ID_ADD, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&clock_add_screen);
        break;

    default:
        return clock_pages_switch_event_handler(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID row_widget_draw(GX_WIDGET *widget)
{
    GX_RECTANGLE dirty;

    gx_widget_background_draw(widget);

    dirty.gx_rectangle_left = widget->gx_widget_size.gx_rectangle_left + 10;
    dirty.gx_rectangle_right = widget->gx_widget_size.gx_rectangle_right - 10;
    dirty.gx_rectangle_top = widget->gx_widget_size.gx_rectangle_bottom - 7;
    dirty.gx_rectangle_bottom = widget->gx_widget_size.gx_rectangle_bottom - 7;

    gx_canvas_drawing_initiate(root->gx_window_root_canvas, widget, &dirty);
    gx_context_fill_color_set(GX_COLOR_ID_DARK_GRAY);
    gx_canvas_rectangle_draw(&dirty);
    gx_canvas_drawing_complete(root->gx_window_root_canvas, GX_FALSE);

    gx_widget_children_draw(widget);
}

/*************************************************************************************/
VOID time_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    TIME_LIST_ROW *row = (TIME_LIST_ROW *)widget;
    GX_BOOL result;
    GX_STRING string;

    gx_widget_created_test(&row->widget, &result);
    if (!result)
    {
        gx_utility_rectangle_define(&childsize, 0, 0, 252, 75);
        gx_widget_create(&row->widget, NULL, list, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, GX_ID_NONE, &childsize);
        gx_widget_draw_set(&row->widget, row_widget_draw);

        gx_utility_rectangle_define(&childsize, 10, 5, 130, 50);
        gx_multi_line_text_view_create(&row->city_name, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_multi_line_text_view_font_set(&row->city_name, GX_FONT_ID_NORMAL);
        gx_multi_line_text_view_text_color_set(&row->city_name, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);
        gx_multi_line_text_view_line_space_set(&row->city_name, 4);

        gx_utility_rectangle_define(&childsize, 130, 5, 175, 43);
        gx_prompt_create(&row->time_hour, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_RIGHT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->time_hour, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->time_hour, GX_FONT_ID_MIDDLE_NUMBER_40);

        gx_utility_rectangle_define(&childsize, 171, 0, 190, 40);
        gx_icon_create(&row->dot_up, NULL, &row->widget, GX_PIXELMAP_ID_DOT, GX_STYLE_TRANSPARENT, 0, 180, 15);
        gx_icon_create(&row->dot_down, NULL, &row->widget, GX_PIXELMAP_ID_DOT, GX_STYLE_TRANSPARENT, 0, 180, 30);

        gx_utility_rectangle_define(&childsize, 190, 5, 245, 43);
        gx_prompt_create(&row->time_minute, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->time_minute, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->time_minute, GX_FONT_ID_MIDDLE_NUMBER_40);

        gx_utility_rectangle_define(&childsize, 200, 40, 240, 70);
        gx_prompt_create(&row->time_am_pm, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_RIGHT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->time_am_pm, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        gx_prompt_font_set(&row->time_am_pm, GX_FONT_ID_NORMAL);
    }
    gx_multi_line_text_view_text_id_set(&row->city_name, time_table[index].city_name_id);
    gx_prompt_text_id_set(&row->time_am_pm, time_table[index].time_am_pm);

    /* Set hour text. */
    string.gx_string_ptr = time_table[index].time_hour_text;
    string.gx_string_length = string_length_get(time_table[index].time_hour_text, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&row->time_hour, &string);

    /* Set minute text. */
    string.gx_string_ptr = time_table[index].time_minute_text;
    string.gx_string_length = string_length_get(time_table[index].time_minute_text, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&row->time_minute, &string);
}

/*************************************************************************************/
VOID time_list_children_create(GX_VERTICAL_LIST *list)
{
    INT count;

    for (count = 0; count < TIME_LIST_VISIBLE_ROWS + 1; count++)
    {
        time_list_row_create(list, (GX_WIDGET *)&row_memory[count], count);
    }
}

/*************************************************************************************/
void time_update()
{
    INT            index = 0;
    TIME_TABLE    *table_ptr = time_table;
    TIME_LIST_ROW *row;
    GX_STRING string;

    while (table_ptr->city_name_id)
    {
        row = &row_memory[index];

        if (index < TIME_LIST_VISIBLE_ROWS + 1)
        {
            if (time_second & 0x1)
            {
                gx_widget_fill_color_set(&row->dot_up, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
                gx_widget_fill_color_set(&row->dot_down, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
            }
            else
            {
                gx_widget_fill_color_set(&row->dot_up, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
                gx_widget_fill_color_set(&row->dot_down, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
            }
        }


        if (time_second == 59)
        {
            if (table_ptr->time_minute == 59)
            {
                table_ptr->time_minute = 0;

                if (table_ptr->time_hour == 11)
                {
                    table_ptr->time_hour = 0;
                    if (table_ptr->time_am_pm == GX_STRING_ID_TIME_AM)
                    {
                        table_ptr->time_am_pm = GX_STRING_ID_TIME_PM;
                    }
                    else
                    {
                        table_ptr->time_am_pm = GX_STRING_ID_TIME_AM;
                    }
                }
                else
                {
                    table_ptr->time_hour++;
                }

                gx_utility_ltoa(table_ptr->time_hour, table_ptr->time_hour_text, 3);
   
                string.gx_string_ptr = table_ptr->time_hour_text;
                string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_TIME_TEXT_LENGTH);
                gx_prompt_text_set_ext(&row->time_hour, &string);
            }
            else
            {
                table_ptr->time_minute++;
            }

            gx_utility_ltoa(table_ptr->time_minute, table_ptr->time_minute_text, 3);

            if (table_ptr->time_minute < 10)
            {
                table_ptr->time_minute_text[2] = '\0';
                table_ptr->time_minute_text[1] = table_ptr->time_minute_text[0];
                table_ptr->time_minute_text[0] = '0';
            }

            gx_system_dirty_mark(&row->time_minute);
            string.gx_string_ptr = table_ptr->time_minute_text;
            string.gx_string_length = 2;
            gx_prompt_text_set_ext(&row->time_minute, &string);
        }

        index++;
        table_ptr++;
    }


    if (time_second == 59)
    {
        time_second = 0;
    }
    else
    {
        time_second++;
    }
}

/*************************************************************************************/
UINT clock_pages_switch_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_TIME, GX_EVENT_CLICKED):
        if (window != (GX_WINDOW *)&clock_screen)
        {
            screen_switch(window->gx_widget_parent, (GX_WIDGET *)&clock_screen);
        }
        break;

    case GX_SIGNAL(ID_ALARM, GX_EVENT_CLICKED):
        if (window != (GX_WINDOW *)&alarm_screen)
        {
            screen_switch(window->gx_widget_parent, (GX_WIDGET *)&alarm_screen);
        }
        break;

    case GX_SIGNAL(ID_STOPWATCH, GX_EVENT_CLICKED):
        if (window != (GX_WINDOW *)&stopwatch_screen)
        {
            screen_switch(window->gx_widget_parent, (GX_WIDGET *)&stopwatch_screen);
        }
        break;

    default:
        return template_main_event_handler(window, event_ptr);
    }

    return 0;
}
