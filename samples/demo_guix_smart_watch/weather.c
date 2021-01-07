/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define FUTHURE_WEATHER_COUNT 7
#define CHILD_WIDTH           252
#define CHILD_HEIGHT          45

typedef struct WEATHER_INFO_STRUCT{
    GX_RESOURCE_ID day;
    GX_RESOURCE_ID weather;
    GX_CHAR        T_high[MAX_T_TEXT_LENGTH + 1];
    GX_CHAR        T_low[MAX_T_TEXT_LENGTH + 1];
}WEATHER_INFO;

WEATHER_INFO weather_info_NewYork[] = {
    { GX_STRING_ID_THUESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "71", "60" },
    { GX_STRING_ID_FRIDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "70", "60"},
    { GX_STRING_ID_SATURDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "69", "60" },
    { GX_STRING_ID_SUNDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "79", "65" },
    { GX_STRING_ID_MONDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "76", "64" },
    { GX_STRING_ID_THUESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "85", "72" },
    { GX_STRING_ID_WEDNESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "80", "70" }
};

WEATHER_INFO weather_info_SanFrancisco[] = {
    { GX_STRING_ID_THUESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "81", "70" },
    { GX_STRING_ID_FRIDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "80", "70" },
    { GX_STRING_ID_SATURDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "79", "65" },
    { GX_STRING_ID_SUNDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "79", "65" },
    { GX_STRING_ID_MONDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "76", "64" },
    { GX_STRING_ID_THUESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "85", "72" },
    { GX_STRING_ID_WEDNESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "80", "70" }
};

WEATHER_INFO weather_info_LosAngeles[] = {
    { GX_STRING_ID_THUESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "92", "80" },
    { GX_STRING_ID_FRIDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "94", "60" },
    { GX_STRING_ID_SATURDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "79", "65" },
    { GX_STRING_ID_SUNDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "79", "65" },
    { GX_STRING_ID_MONDAY, GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL, "76", "64" },
    { GX_STRING_ID_THUESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "85", "72" },
    { GX_STRING_ID_WEDNESDAY, GX_PIXELMAP_ID_WEATHER_SUN_SMALL, "80", "70" }
};

typedef struct WEATHER_LIST_ROW_STRUCT{
    GX_WIDGET child;
    GX_PROMPT day;
    GX_ICON   weather;
    GX_PROMPT T_high;
    GX_PROMPT T_low;
    GX_PROMPT T_flag_high;
    GX_PROMPT T_flag_low;
}WEATHER_LIST_ROW;

WEATHER_LIST_ROW weather_list_row_memory_NewYork[FUTHURE_WEATHER_COUNT];
WEATHER_LIST_ROW weather_list_row_memory_SanFrancisco[FUTHURE_WEATHER_COUNT];
WEATHER_LIST_ROW weather_list_row_memory_LosAngeles[FUTHURE_WEATHER_COUNT];

GX_WIDGET *weather_screen_list[] = {
    (GX_WIDGET *)&weather_screen.weather_screen_weather_NewYork,
    (GX_WIDGET *)&weather_SanFrancisco,
    (GX_WIDGET *)&weather_LosAngeles,
    GX_NULL
};

extern GX_ANIMATION slide_animation;
extern GX_ANIMATION animation[2];

/*************************************************************************************/
static VOID start_slide_animation(GX_WINDOW *window)
{
    GX_ANIMATION_INFO slide_animation_info;

    memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
    slide_animation_info.gx_animation_parent = (GX_WIDGET *)&weather_screen;
    slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_WRAP | GX_ANIMATION_HORIZONTAL;
    slide_animation_info.gx_animation_frame_interval = 1;
    slide_animation_info.gx_animation_slide_screen_list = weather_screen_list;
    slide_animation_info.gx_animation_id = ANIMATION_ID_SCREEN_LIST_SLIDE;

    gx_animation_drag_enable(&slide_animation, (GX_WIDGET *)window, &slide_animation_info);
}

/*************************************************************************************/
UINT weather_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        start_slide_animation(window);
        return template_main_event_handler(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)window);
        return gx_window_event_process(window, event_ptr);

    case GX_SIGNAL(ID_LEFT, GX_EVENT_CLICKED):
        screen_list_slide((GX_WIDGET **)weather_screen_list, (GX_WIDGET *)&weather_screen, SLIDE_LEFT);
        break;

    case GX_SIGNAL(ID_RIGHT, GX_EVENT_CLICKED):
        screen_list_slide((GX_WIDGET **)weather_screen_list, (GX_WIDGET *)&weather_screen, SLIDE_RIGHT);
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        gx_widget_front_move(&weather_screen.weather_screen_left_arrow, GX_NULL);
        gx_widget_front_move(&weather_screen.weather_screen_right_arrow, GX_NULL);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID future_weather_list_children_create(GX_WIDGET *parent, SHORT city)
{
    INT count;
    WEATHER_LIST_ROW *memory;
    WEATHER_INFO *info;
    WEATHER_LIST_ROW *row;
    GX_RECTANGLE size;
    INT  x_offset = parent->gx_widget_size.gx_rectangle_left;
    INT  y_offset = parent->gx_widget_size.gx_rectangle_top + 116;
    GX_STRING string;

    switch (city)
    {
    case CITY_NEW_YORK:
        memory = weather_list_row_memory_NewYork;
        info = weather_info_NewYork;
        break;

    case CITY_SAN_FRANCISCO:
        memory = weather_list_row_memory_SanFrancisco;
        info = weather_info_SanFrancisco;
        break;

    case CITY_LOS_ANGELES:
        memory = weather_list_row_memory_LosAngeles;
        info = weather_info_LosAngeles;
        break;
    }

    for (count = 0; count < FUTHURE_WEATHER_COUNT; count++)
    {
        row = &memory[count];

        gx_utility_rectangle_define(&size, x_offset, y_offset, x_offset + CHILD_WIDTH, y_offset + CHILD_HEIGHT);
        gx_widget_create(&row->child, "", parent, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &size);

        /* Create prompt to display the date. */
        gx_utility_rectangle_define(&size, x_offset, y_offset + 14, x_offset + 100, y_offset + 34);
        gx_prompt_create(&row->day, "", &row->child, info[count].day, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &size);
        gx_prompt_text_color_set(&row->day, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);
        gx_prompt_font_set(&row->day, GX_FONT_ID_NORMAL);

        /* Create icon to display weather. */
        gx_icon_create(&row->weather, "", &row->child, info[count].weather, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED, 0, x_offset + 100, y_offset + 10);
        gx_widget_fill_color_set(&row->weather, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);

        /* Create prompt to display high temperature. */
        gx_utility_rectangle_define(&size, x_offset + 141, y_offset + 12, x_offset + 178, y_offset + 42);
        gx_prompt_create(&row->T_high, "", &row->child, GX_NULL, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &size);
        string.gx_string_ptr = info[count].T_high;
        string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_T_TEXT_LENGTH);
        gx_prompt_text_set_ext(&row->T_high, &string);
        gx_prompt_text_color_set(&row->T_high, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->T_high, GX_FONT_ID_MIDDLE);

        gx_utility_rectangle_define(&size, x_offset + 178, y_offset + 12, x_offset + 187, y_offset + 29);
        gx_prompt_create(&row->T_flag_high, "", &row->child, GX_STRING_ID_CHARACTER_O, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_TEXT_RIGHT | GX_STYLE_ENABLED, 0, &size);
        gx_prompt_text_color_set(&row->T_flag_high, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);

        /* Create prompt to display low temperature. */
        gx_utility_rectangle_define(&size, x_offset + 195, y_offset + 12, x_offset + 232, y_offset + 42);
        gx_prompt_create(&row->T_low, "", &row->child, GX_NULL, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &size);
        string.gx_string_ptr = info[count].T_low;
        string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_T_TEXT_LENGTH);
        gx_prompt_text_set_ext(&row->T_low, &string);
        gx_prompt_text_color_set(&row->T_low, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        gx_prompt_font_set(&row->T_low, GX_FONT_ID_MIDDLE);

        gx_utility_rectangle_define(&size, x_offset + 227, y_offset + 12, x_offset + 237, y_offset + 29);
        gx_prompt_create(&row->T_flag_low, "", &row->child, GX_STRING_ID_CHARACTER_O, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &size);
        gx_prompt_text_color_set(&row->T_flag_low, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);

        y_offset += CHILD_HEIGHT;
    }
}

/*************************************************************************************/
VOID screen_list_slide(GX_WIDGET **screen_list, GX_WIDGET *parent, INT type)
{
GX_WIDGET *test;
INT        list_size;
INT        target_index_1;
INT        target_index_2;
INT        index;
INT        width;
GX_ANIMATION_INFO animation_info[2];

    test = screen_list[0];
    index = 0;

    while (test)
    {
        if (test->gx_widget_status & GX_STATUS_VISIBLE)
        {
            target_index_1 = index;
        }
        index++;
        test = screen_list[index];
    }

    list_size = index;

    if (type == SLIDE_RIGHT)
    {
        if (target_index_1 == 0)
        {
            target_index_2 = list_size - 1;
        }
        else
        {
            target_index_2 = target_index_1 - 1;
        }

    }
    else
    {
        if (target_index_1 == list_size - 1)
        {
            target_index_2 = 0;
        }
        else
        {
            target_index_2 = target_index_1 + 1;
        }
    }

    width = (screen_list[target_index_1]->gx_widget_size.gx_rectangle_right - screen_list[target_index_1]->gx_widget_size.gx_rectangle_left + 1);

    memset(&animation_info[0], 0, sizeof(GX_ANIMATION_INFO));
    animation_info[0].gx_animation_frame_interval = 1;
    animation_info[0].gx_animation_id = ANIMATION_ID_SCREEN_LIST_SLIDE;
    animation_info[0].gx_animation_steps = 20;
    animation_info[0].gx_animation_start_alpha = 255;
    animation_info[0].gx_animation_end_alpha = 255;
    animation_info[0].gx_animation_parent = parent;
    animation_info[0].gx_animation_start_position.gx_point_x = screen_list[target_index_1]->gx_widget_size.gx_rectangle_left;
    animation_info[0].gx_animation_start_position.gx_point_y = screen_list[target_index_1]->gx_widget_size.gx_rectangle_top;

    if (type == SLIDE_RIGHT)
    {
        animation_info[0].gx_animation_end_position.gx_point_x = screen_list[target_index_1]->gx_widget_size.gx_rectangle_left + width;
    }
    else
    {
        animation_info[0].gx_animation_end_position.gx_point_x = screen_list[target_index_1]->gx_widget_size.gx_rectangle_left - width;
    }
    animation_info[0].gx_animation_end_position.gx_point_y = screen_list[target_index_1]->gx_widget_size.gx_rectangle_top;

    animation_info[0].gx_animation_target = screen_list[target_index_1];
    animation_info[0].gx_animation_style = GX_ANIMATION_TRANSLATE | GX_ANIMATION_DETACH;

    memset(&animation_info[1], 0, sizeof(GX_ANIMATION_INFO));
    animation_info[1].gx_animation_frame_interval = 1;
    animation_info[1].gx_animation_id = ANIMATION_ID_SCREEN_LIST_SLIDE;
    animation_info[1].gx_animation_steps = 20;
    animation_info[1].gx_animation_start_alpha = 255;
    animation_info[1].gx_animation_end_alpha = 255;
    animation_info[1].gx_animation_parent = parent;
    if (type == SLIDE_RIGHT)
    {
        animation_info[1].gx_animation_start_position.gx_point_x = screen_list[target_index_1]->gx_widget_size.gx_rectangle_left - width;
    }
    else
    {
        animation_info[1].gx_animation_start_position.gx_point_x = screen_list[target_index_1]->gx_widget_size.gx_rectangle_left + width;
    }
    animation_info[1].gx_animation_start_position.gx_point_y = screen_list[target_index_1]->gx_widget_size.gx_rectangle_top;
    animation_info[1].gx_animation_end_position.gx_point_x = screen_list[target_index_1]->gx_widget_size.gx_rectangle_left;
    animation_info[1].gx_animation_end_position.gx_point_y = screen_list[target_index_1]->gx_widget_size.gx_rectangle_top;
    animation_info[1].gx_animation_target = screen_list[target_index_2];
    gx_animation_start(&animation[0], &animation_info[0]);
    gx_animation_start(&animation[1], &animation_info[1]);
}
