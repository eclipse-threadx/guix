/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define CITY_LIST_VISIBLE_ROWS 10

GX_CHAR *city_table[] = {
     "Abadan, Iran",
     "AbaKan, Russia",
     "Aberdeen, United Kingdom",
     "Abuja, Nigeria",
     "Agdam, Azerbaijan",
     "Agra, India",
     "Aguascalientes, Mexico",
     "Adamklisi, Romania",
     "Adana, Turkey",
     "Adelaide, Australia",
     "Aden, Yemen",
     "Addis Ababa, Ethiopia",
     "Ajmer, India",
     "Adygeysk, Russia",
     "Azov, Russia",
     "Aqaba, Jordan",
     "Acapulco, Mexico",
     "Akashi, Japan",
     "Accra, Ghana",
     GX_NULL
};

/* CITY list row structure. */
typedef struct CITY_LIST_ROW_STRUCT {
    GX_PROMPT bg;
    GX_PROMPT city_name;
} CITY_LIST_ROW;

CITY_LIST_ROW city_list_row_memory[CITY_LIST_VISIBLE_ROWS + 1];

/*************************************************************************************/
UINT clock_add_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&clock_screen);
        break;

    default:
        return clock_pages_switch_event_handler(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID city_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    CITY_LIST_ROW *row = (CITY_LIST_ROW *)widget;
    GX_BOOL result;
    GX_STRING string;

    gx_widget_created_test(&row->bg, &result);
    if (!result)
    {
        gx_utility_rectangle_define(&childsize, 0, 5, 215, 35);
        gx_prompt_create(&row->bg, NULL, (GX_WIDGET *)list, 0, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_widget_fill_color_set(&row->bg, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY);

        gx_utility_rectangle_define(&childsize, 5, 5, 215, 35);
        gx_prompt_create(&row->city_name, NULL, (GX_WIDGET *)&row->bg, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->city_name, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->city_name, GX_FONT_ID_SYSTEM);
    }

    string.gx_string_ptr = city_table[index];
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_CITY_NAME_LENGTH);
    gx_prompt_text_set_ext(&row->city_name, &string);

    if (index & 1)
    {
        gx_widget_style_add(&row->bg, GX_STYLE_TRANSPARENT);
    }
    else
    {
        gx_widget_style_remove(&row->bg, GX_STYLE_TRANSPARENT);
    }
}

/*************************************************************************************/
VOID city_list_children_create(GX_VERTICAL_LIST *list)
{
    INT count;

    for (count = 0; count < CITY_LIST_VISIBLE_ROWS + 1; count++)
    {
        city_list_row_create(list, (GX_WIDGET *)&city_list_row_memory[count], count);
    }
}
