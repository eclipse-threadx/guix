/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define ALARM_LIST_VISIBLE_ROWS 3
#define ALARM_ON                0x00
#define ALARM_OFF               0xff

VOID select_am_pm(UINT event_type);
VOID edit_hour(UINT event_type);
VOID edit_minute(UINT event_type);
VOID string_to_int(char *str, int len, int *value);

static INT hour = 12;
static INT minute = 0;

typedef struct ALARM_TABLE_STRUCT{
    GX_CHAR  alarm_hour_text[MAX_TIME_TEXT_LENGTH + 1];
    GX_CHAR  alarm_minute_text[MAX_TIME_TEXT_LENGTH + 1];
    UINT     alarm_hour;
    UINT     alarm_minute;
    UINT     alarm_am_pm;
    USHORT   alarm_status;
}ALARM_TABLE;

ALARM_TABLE alarm_table[] = {
    { "5", "30", 5, 30, GX_STRING_ID_TIME_AM, ALARM_OFF },
    { "6", "45", 6, 45, GX_STRING_ID_TIME_AM, ALARM_ON },
    { "10", "30", 10, 30, GX_STRING_ID_TIME_AM, ALARM_OFF },
    { "10", "35", 10, 35, GX_STRING_ID_TIME_PM, ALARM_OFF },
    { "10", "50", 10, 50, GX_STRING_ID_TIME_PM, ALARM_ON },
    { "11", "50", 11, 50, GX_STRING_ID_TIME_PM, ALARM_ON },
    { "", "", 0, 0, 0, 0}
};

/* Alarm list row structure. */
typedef struct TIME_LIST_ROW_STRUCT {
    GX_WIDGET widget;
    CUSTOM_CHECKBOX checkbox;
    GX_PROMPT time_hour;
    GX_PROMPT time_minute;
    GX_PROMPT time_am_pm;
    GX_ICON   dot_up;
    GX_ICON   dot_down;
} ALARM_LIST_ROW;

ALARM_LIST_ROW alarm_list_row_memory[ALARM_LIST_VISIBLE_ROWS + 1];

extern CUSTOM_CHECKBOX_INFO checkbox_info;
/*************************************************************************************/
/* Handler alarm screen                                                              */
/*************************************************************************************/
UINT alarm_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    INT index;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_ADD, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&alarm_add_screen);
        break;

    case GX_SIGNAL(SWITCH_BTTON_ID, GX_EVENT_TOGGLE_ON):
        gx_vertical_list_selected_index_get(&alarm_screen.alarm_screen_alarm_list, &index);
        alarm_table[index].alarm_status = ALARM_ON;
        break;

    case GX_SIGNAL(SWITCH_BTTON_ID, GX_EVENT_TOGGLE_OFF):
        gx_vertical_list_selected_index_get(&alarm_screen.alarm_screen_alarm_list, &index);
        alarm_table[index].alarm_status = ALARM_OFF;
        break;

    default:
        return clock_pages_switch_event_handler(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID alarm_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    ALARM_LIST_ROW *row = (ALARM_LIST_ROW *)widget;
    GX_BOOL result;
    GX_STRING string;

    gx_widget_created_test(&row->widget, &result);
    if (!result)
    {
        gx_utility_rectangle_define(&childsize, 0, 0, 252, 75);
        gx_widget_create(&row->widget, NULL, list, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, GX_ID_NONE, &childsize);
        gx_widget_draw_set(&row->widget, row_widget_draw);

        gx_utility_rectangle_define(&childsize, 20, 15, 77, 47);
        custom_checkbox_create(&row->checkbox, &row->widget, &checkbox_info, &childsize);

        gx_utility_rectangle_define(&childsize, 130, 5, 175, 43);
        gx_prompt_create(&row->time_hour, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_RIGHT | GX_STYLE_BORDER_NONE, SWITCH_BTTON_ID, &childsize);
        gx_prompt_text_color_set(&row->time_hour, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->time_hour, GX_FONT_ID_MIDDLE_NUMBER_40);

        gx_utility_rectangle_define(&childsize, 171, 0, 190, 40);
        gx_icon_create(&row->dot_up, NULL, &row->widget, GX_PIXELMAP_ID_DOT, GX_STYLE_TRANSPARENT, 0, 180, 15);
        gx_icon_create(&row->dot_down, NULL, &row->widget, GX_PIXELMAP_ID_DOT, GX_STYLE_TRANSPARENT, 0, 180, 30);
        gx_widget_fill_color_set((GX_WIDGET  *)&row->dot_up, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_widget_fill_color_set((GX_WIDGET  *)&row->dot_down, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

        gx_utility_rectangle_define(&childsize, 190, 5, 245, 43);
        gx_prompt_create(&row->time_minute, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->time_minute, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->time_minute, GX_FONT_ID_MIDDLE_NUMBER_40);

        gx_utility_rectangle_define(&childsize, 200, 40, 240, 70);
        gx_prompt_create(&row->time_am_pm, NULL, &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_RIGHT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->time_am_pm, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        gx_prompt_font_set(&row->time_am_pm, GX_FONT_ID_NORMAL);
    }

    /* Set AM/PM label. */
    gx_prompt_text_id_set(&row->time_am_pm, alarm_table[index].alarm_am_pm);

    /* Set hour. */
    string.gx_string_ptr = alarm_table[index].alarm_hour_text;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&row->time_hour, &string);

    /* Set minute. */
    string.gx_string_ptr = alarm_table[index].alarm_minute_text;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&row->time_minute, &string);

    if (alarm_table[index].alarm_status == ALARM_ON)
    {
        gx_widget_style_add((GX_WIDGET *)&row->checkbox, GX_STYLE_BUTTON_PUSHED);
    }
    else
    {
        gx_widget_style_remove((GX_WIDGET *)&row->checkbox, GX_STYLE_BUTTON_PUSHED);
    }
}

/*************************************************************************************/
VOID alarm_list_children_create(GX_VERTICAL_LIST *list)
{
    INT count;

    for (count = 0; count < ALARM_LIST_VISIBLE_ROWS + 1; count++)
    {
        alarm_list_row_create(list, (GX_WIDGET *)&alarm_list_row_memory[count], count);
    }
}

/*************************************************************************************/
/* Handle alarm add screen                                                           */
/*************************************************************************************/
UINT alarm_add_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    GX_SINGLE_LINE_TEXT_INPUT *text_input;
    GX_STRING string;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_widget_find(&alarm_add_screen, ID_HOUR, GX_SEARCH_DEPTH_INFINITE, &text_input);

        string.gx_string_ptr = "12";
        string.gx_string_length = 2;
        gx_single_line_text_input_text_set_ext(text_input, &string);
        gx_widget_find(&alarm_add_screen, ID_MINUTE, GX_SEARCH_DEPTH_INFINITE, &text_input);

        string.gx_string_ptr = "00";
        string.gx_string_length = 2;
        gx_single_line_text_input_text_set_ext(text_input, &string);
        template_main_event_handler(window, event_ptr);
        break;

    case GX_SIGNAL(ID_AM, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_PM, GX_EVENT_CLICKED):
        select_am_pm(event_ptr->gx_event_type);
        break;

    case GX_SIGNAL(ID_HOUR_UP, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_HOUR_DOWN, GX_EVENT_CLICKED):
        edit_hour(event_ptr->gx_event_type);
        break;

    case GX_SIGNAL(ID_MINUTE_UP, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_MINUTE_DOWN, GX_EVENT_CLICKED):
        edit_minute(event_ptr->gx_event_type);
        break;

    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&alarm_screen);
        break;
    default:
        return clock_pages_switch_event_handler(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID select_am_pm(UINT event_type)
{
    GX_PROMPT *am;
    GX_PROMPT *pm;

    gx_widget_find(&alarm_add_screen, ID_AM, GX_SEARCH_DEPTH_INFINITE, &am);
    gx_widget_find(&alarm_add_screen, ID_PM, GX_SEARCH_DEPTH_INFINITE, &pm);

    switch (event_type)
    {
    case GX_SIGNAL(ID_AM, GX_EVENT_CLICKED):
        gx_prompt_text_color_set(am, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_prompt_text_color_set(pm, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        break;
    case GX_SIGNAL(ID_PM, GX_EVENT_CLICKED):
        gx_prompt_text_color_set(pm, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_prompt_text_color_set(am, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        break;
    }
}

/*************************************************************************************/
VOID edit_hour(UINT event_type)
{
    GX_SINGLE_LINE_TEXT_INPUT *text_input;
    GX_CHAR string[3];

    gx_widget_find(&alarm_add_screen, ID_HOUR, GX_SEARCH_DEPTH_INFINITE, &text_input);

    switch (event_type)
    {
    case GX_SIGNAL(ID_HOUR_UP, GX_EVENT_CLICKED):
        hour++;
        break;
    case GX_SIGNAL(ID_HOUR_DOWN, GX_EVENT_CLICKED):
        if (hour == 0)
        {
            return;
        }
        hour--;
        break;
    }

    if (hour > 12)
    {
        hour = 0;
    }

    gx_utility_ltoa(hour, string, 3);
    gx_single_line_text_input_buffer_clear(text_input);

    if (hour < 10)
    {
        gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)"0", 1);
        gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)string, 1);
    }
    else
    {
        gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)string, 2);
    }
}

/*************************************************************************************/
VOID edit_minute(UINT event_type)
{
    GX_SINGLE_LINE_TEXT_INPUT *text_input;
    GX_CHAR string[3];

    gx_widget_find(&alarm_add_screen, ID_MINUTE, GX_SEARCH_DEPTH_INFINITE, &text_input);

    switch (event_type)
    {
    case GX_SIGNAL(ID_MINUTE_UP, GX_EVENT_CLICKED):
        minute++;
        break;
    case GX_SIGNAL(ID_MINUTE_DOWN, GX_EVENT_CLICKED):
        if (minute == 0)
        {
            return;
        }
        minute--;
        break;
    }

    if (minute >= 60)
    {
        minute = 0;
    }

    gx_utility_ltoa(minute, string, 3);
    gx_single_line_text_input_buffer_clear(text_input);

    if (minute < 10)
    {
        gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)"0", 1);
        gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)string, 1);
    }
    else
    {
        gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)string, 2);
    }
}

/*************************************************************************************/
UINT custom_text_input_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
    USHORT key_value;
    INT int_value;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_KEY_DOWN:
        key_value = event_ptr->gx_event_payload.gx_event_ushortdata[0];

        if ((key_value >= 0x21) && (key_value <= 0x7e))
        {
            if (key_value >= '0' && key_value <= '9')
            {
                gx_single_line_text_input_event_process(text_input, event_ptr);

                string_to_int(text_input->gx_single_line_text_input_buffer, text_input->gx_single_line_text_input_string_size, &int_value);

                if (text_input->gx_widget_id == ID_HOUR)
                {
                    if (int_value < 0 || int_value > 12)
                    {
                        gx_single_line_text_input_backspace(text_input);
                    }
                    else
                    {
                        hour = int_value;
                    }
                }
                else if (text_input->gx_widget_id == ID_MINUTE)
                {
                    if (int_value < 0 || int_value > 59)
                    {
                        gx_single_line_text_input_backspace(text_input);
                    }
                    else
                    {
                        minute = int_value;
                    }
                }
            }
            return 0;
        }
        break;

    case GX_EVENT_FOCUS_LOST:

        string_to_int(text_input->gx_single_line_text_input_buffer, text_input->gx_single_line_text_input_string_size, &int_value);

        if (text_input->gx_widget_id == ID_HOUR)
        {
            if (text_input->gx_single_line_text_input_string_size)
            {
                hour = int_value;
            }
            edit_hour(0);
        }
        else if (text_input->gx_widget_id == ID_MINUTE)
        {
            if (text_input->gx_single_line_text_input_string_size)
            {
                minute = int_value;
            }
            edit_minute(0);
        }
        break;
    }

    return gx_single_line_text_input_event_process(text_input, event_ptr);
}

/*************************************************************************************/
VOID string_to_int(char *str, int len, int *value)
{
    int i;

    *value = 0;

    for (i = 0; i < len; i++)
    {
        *value = *value * 10;
        *value += str[i] - '0';
    }
}
