/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define DASHBOARD_LIST_TOTAL_ROWS 4
#define BRUSH_LIST_TOTAL_ROWS     1
#define SOUND_LIST_TOTAL_ROWS     1

#define SETTING_STATUS_ON         0x01
#define SETTING_STATUS_OFF        0x02

#define CHECKBOX_BUTTON_ID_START  0x01

typedef struct SETTING_LIST_ROW_STRUCT{
    GX_WIDGET          widget;
    CUSTOM_CHECKBOX    checkbox;
    GX_PROMPT          item_name;
    GX_PIXELMAP_BUTTON right_arrow;
}SETTING_LIST_ROW;

SETTING_LIST_ROW setting_list_row_memory[DASHBOARD_LIST_TOTAL_ROWS];

typedef struct SETTING_LIST_INFO_STRUCT{
    GX_RESOURCE_ID name_id;
    USHORT   status;
    USHORT   id;
}SETTING_LIST_INFO;

SETTING_LIST_INFO dashboard_list_info[] = {
    { GX_STRING_ID_SOUNDS, SETTING_STATUS_OFF, CHECKBOX_BUTTON_ID_START },
    { GX_STRING_ID_WIFI, SETTING_STATUS_OFF, CHECKBOX_BUTTON_ID_START + 1 },
    { GX_STRING_ID_BLUETOOTH, SETTING_STATUS_OFF, CHECKBOX_BUTTON_ID_START + 2 },
    { GX_STRING_ID_NOTIFICATIONS, SETTING_STATUS_OFF, CHECKBOX_BUTTON_ID_START + 3 }
};

SETTING_LIST_INFO brush_list_info[] = {
    { GX_STRING_ID_THEMES, SETTING_STATUS_OFF, CHECKBOX_BUTTON_ID_START }
};

SETTING_LIST_INFO sound_list_info[] = {
    { GX_STRING_ID_VOICE_CONTROL, SETTING_STATUS_OFF, CHECKBOX_BUTTON_ID_START }
};

SETTING_LIST_INFO *current_list_info = dashboard_list_info;
INT current_list_count = DASHBOARD_LIST_TOTAL_ROWS;

VOID setting_list_children_create(GX_VERTICAL_LIST *list);
VOID setting_list_children_delete(GX_VERTICAL_LIST *list);
UINT dashboard_list_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr);

GX_VERTICAL_LIST setting_list;

CUSTOM_CHECKBOX_INFO checkbox_info = {
    SWITCH_BTTON_ID,
    GX_PIXELMAP_ID_SWITCH_BG,
    GX_PIXELMAP_ID_SWITCH_ACTIVE,
    GX_PIXELMAP_ID_SWITCH_DISACTIVE,
    4,
    24
};

/*************************************************************************************/
UINT settings_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    INT index;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        if (setting_list.gx_widget_type == 0)
        {
            setting_list_children_create(&setting_list);
            setting_list.gx_widget_event_process_function = (UINT(*)(GX_WIDGET *, GX_EVENT *))dashboard_list_event_handler;
        }
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_DASHBOARD, GX_EVENT_RADIO_SELECT):
        current_list_info = dashboard_list_info;
        current_list_count = DASHBOARD_LIST_TOTAL_ROWS;
        setting_list_children_create(&setting_list);
        setting_list.gx_widget_event_process_function = (UINT(*)(GX_WIDGET *, GX_EVENT *))dashboard_list_event_handler;
        break;
    case GX_SIGNAL(ID_BRUSH, GX_EVENT_RADIO_SELECT):
        current_list_info = brush_list_info;
        current_list_count = BRUSH_LIST_TOTAL_ROWS;
        setting_list_children_create(&setting_list);
        break;
    case GX_SIGNAL(ID_LANGUAGE, GX_EVENT_RADIO_SELECT):
        gx_widget_detach(&setting_list);
        gx_widget_attach((GX_WIDGET *)window, &settings_language);
        break;
    case GX_SIGNAL(ID_LANGUAGE, GX_EVENT_RADIO_DESELECT):
        gx_widget_detach(&settings_language);
        break;
    case GX_SIGNAL(ID_SOUND, GX_EVENT_RADIO_SELECT):
        current_list_info = sound_list_info;
        current_list_count = SOUND_LIST_TOTAL_ROWS;
        setting_list_children_create(&setting_list);
        break;

    case GX_SIGNAL(SWITCH_BTTON_ID, GX_EVENT_TOGGLE_ON):
        gx_vertical_list_selected_index_get(&setting_list, &index);
        current_list_info[index].status = SETTING_STATUS_ON;
        break;

    case GX_SIGNAL(SWITCH_BTTON_ID, GX_EVENT_TOGGLE_OFF):
        gx_vertical_list_selected_index_get(&setting_list, &index);
        current_list_info[index].status = SETTING_STATUS_OFF;
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
UINT template_main_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        if (dashboard_list_info[0].status == SETTING_STATUS_ON)
        {
            gx_widget_fill_color_set(&((TEMPLATE_MAIN_CONTROL_BLOCK *)window)->template_main_sound, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
        }
        else
        {
            gx_widget_fill_color_set(&((TEMPLATE_MAIN_CONTROL_BLOCK *)window)->template_main_sound, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        }

        if (dashboard_list_info[1].status == SETTING_STATUS_ON)
        {
            gx_widget_fill_color_set(&((TEMPLATE_MAIN_CONTROL_BLOCK *)window)->template_main_wifi, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
        }
        else
        {
            gx_widget_fill_color_set(&((TEMPLATE_MAIN_CONTROL_BLOCK *)window)->template_main_wifi, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        }

        if (dashboard_list_info[2].status == SETTING_STATUS_ON)
        {
            gx_widget_fill_color_set(&((TEMPLATE_MAIN_CONTROL_BLOCK *)window)->template_main_bluetooth, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
        }
        else
        {
            gx_widget_fill_color_set(&((TEMPLATE_MAIN_CONTROL_BLOCK *)window)->template_main_bluetooth, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        }
        gx_window_event_process(window, event_ptr);
        break;
    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
UINT settings_language_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_ENGLISH, GX_EVENT_RADIO_SELECT):
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;
    case GX_SIGNAL(ID_SPANISH, GX_EVENT_RADIO_SELECT):
        gx_system_active_language_set(LANGUAGE_SPANISH);
        break;
    case GX_SIGNAL(ID_CHINESE, GX_EVENT_RADIO_SELECT):
        gx_system_active_language_set(LANGUAGE_CHINESE);
        break;
    case GX_SIGNAL(ID_JAPANESE, GX_EVENT_RADIO_SELECT):
        gx_system_active_language_set(LANGUAGE_JAPANESE);
        break;
    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
UINT dashboard_list_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(CHECKBOX_BUTTON_ID_START, GX_EVENT_TOGGLE_ON):
        /* Turn on sound. */
        gx_widget_fill_color_set(&settings_screen.base.template_main_sound, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
        dashboard_list_info[0].status = SETTING_STATUS_ON;
        break;

    case GX_SIGNAL(CHECKBOX_BUTTON_ID_START, GX_EVENT_TOGGLE_OFF):
        /* Turn off sound. */
        gx_widget_fill_color_set(&settings_screen.base.template_main_sound, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        dashboard_list_info[0].status = SETTING_STATUS_OFF;
        break;

    case GX_SIGNAL(CHECKBOX_BUTTON_ID_START + 1, GX_EVENT_TOGGLE_ON):
        /* Turn on wifi. */
        gx_widget_fill_color_set(&settings_screen.base.template_main_wifi, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
        dashboard_list_info[1].status = SETTING_STATUS_ON;
        break;

    case GX_SIGNAL(CHECKBOX_BUTTON_ID_START + 1, GX_EVENT_TOGGLE_OFF):
        /* Turn off wifi. */
        gx_widget_fill_color_set(&settings_screen.base.template_main_wifi, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        dashboard_list_info[1].status = SETTING_STATUS_OFF;
        break;

    case GX_SIGNAL(CHECKBOX_BUTTON_ID_START + 2, GX_EVENT_TOGGLE_ON):
        /* Turn on bluetooth. */
        gx_widget_fill_color_set(&settings_screen.base.template_main_bluetooth, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
        dashboard_list_info[2].status = SETTING_STATUS_ON;
        break;

    case GX_SIGNAL(CHECKBOX_BUTTON_ID_START + 2, GX_EVENT_TOGGLE_OFF):
        /* Turn off bluetooth. */
        gx_widget_fill_color_set(&settings_screen.base.template_main_bluetooth, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        dashboard_list_info[2].status = SETTING_STATUS_OFF;
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID setting_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    SETTING_LIST_ROW *row = (SETTING_LIST_ROW *)widget;
    GX_BOOL result;
    GX_RECTANGLE childsize;

    gx_widget_created_test(widget, &result);

    if (!result)
    {
        gx_utility_rectangle_define(&childsize, 0, 0, 259, 50);
        gx_widget_create(&row->widget, "", list, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, 0, &childsize);
        gx_widget_draw_set(&row->widget, row_widget_draw);

        gx_utility_rectangle_define(&childsize, 12, 2, 70, 35);
        checkbox_info.widget_id = current_list_info[index].id;
        custom_checkbox_create(&row->checkbox, &row->widget, &checkbox_info, &childsize);

        gx_utility_rectangle_define(&childsize, 80, 0, 220, 40);
        gx_prompt_create(&row->item_name, "", &row->widget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT, 0, &childsize);
        gx_prompt_text_color_set(&row->item_name, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_prompt_font_set(&row->item_name, GX_FONT_ID_NORMAL);

        gx_utility_rectangle_define(&childsize, 232, 4, 254, 38);
        gx_pixelmap_button_create(&row->right_arrow, "", &row->widget, GX_PIXELMAP_ID_ARROW_RIGHT, GX_NULL, GX_NULL, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->right_arrow, custom_pixelmap_button_draw);
        gx_widget_fill_color_set(&row->right_arrow, GX_COLOR_ID_GRAY, GX_COLOR_ID_BLUE, GX_COLOR_ID_GRAY);
    }

    if (current_list_info[index].status == SETTING_STATUS_ON)
    {
        gx_widget_style_add((GX_WIDGET *)&row->checkbox, GX_STYLE_BUTTON_PUSHED);
    }
    else
    {
        gx_widget_style_remove((GX_WIDGET *)&row->checkbox, GX_STYLE_BUTTON_PUSHED);
    }

    gx_prompt_text_id_set(&row->item_name, current_list_info[index].name_id);
}

/*************************************************************************************/
VOID setting_list_children_delete(GX_VERTICAL_LIST *list)
{
    GX_WIDGET *child = list->gx_widget_first_child;

    while (child)
    {
        gx_widget_delete(child);
        child = list->gx_widget_first_child;
    }

    gx_widget_delete(list);
}

/*************************************************************************************/
VOID setting_list_children_create(GX_VERTICAL_LIST *list)
{
    GX_RECTANGLE size;
    INT index;

    setting_list_children_delete(list);
    gx_utility_rectangle_define(&size, 190, 150, 448, 360);
    gx_vertical_list_create(list, "", &settings_screen, current_list_count, GX_NULL, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, 0, &size);

    for (index = 0; index < current_list_count; index++)
    {
        setting_list_row_create(list, (GX_WIDGET *)&setting_list_row_memory[index], index);
    }

    gx_vertical_list_children_position(list);
}