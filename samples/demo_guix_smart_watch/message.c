/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define MSG_SEND_SCREEN_TIMER  2
#define TEXT_INPUT_ANIMATION_TIMER  3
#define MSG_LIST_VISIBLE_ROWS  3
#define MSG_MAX_NUM           10
#define MSG_STATUS_RECEIVED 0x01
#define MSG_STATUS_SEND     0x02
#define MSG_TITLE_HEIGHT      30
#define MSG_VIEW_HEIGHT       35

#define MAX_MSG_INFO_COUNT     6
#define MAX_MSG_COUNT          5
#define TEXT_BOX_WIDTH       210
#define TEXT_BOX_SPACE        25
#define TIME_BOX_WIDTH        70
#define TIME_BOX_HEIGHT       24

#define ANIMATION_LENGTHEN  0
#define ANIMATION_SHORTEN   1

/* Define message information structure. */
typedef struct MSG_INFO_STRUCT{
    GX_CHAR        text[MAX_MSG_LENGTH + 1];
    USHORT         status;
    GX_RESOURCE_ID time_id;
}MSG_INFO;

/* Predefined message information.  */
MSG_INFO msg_info_memory[MAX_MSG_INFO_COUNT][MAX_MSG_COUNT] = {
    { { "Hi Fiona, I sent you a draft with materials selection. Hope you like it this time", MSG_STATUS_SEND, GX_STRING_ID_FRIDAY },
      { "We're running a bit late, getting in the car now, see you soon!", MSG_STATUS_RECEIVED, GX_STRING_ID_FRIDAY },
      { "Hi Fiona, I sent you a draft with materials selection. Hope you like it this time", MSG_STATUS_SEND, GX_STRING_ID_SATURDAY }},
    { { "Hi Robert, I sent you a draft with materials selection. Hope you like it this time", MSG_STATUS_SEND, GX_STRING_ID_YESTERDAY },
      { "Awesome! Looks greate, Done!", MSG_STATUS_RECEIVED, GX_STRING_ID_YESTERDAY } },
    { { "Hi Margarita, I sent you a draft with materials selection. Hope you like it this time", MSG_STATUS_SEND, GX_STRING_ID_FRIDAY },
      { "I can talk now if you are free, if not how about around 1pm tomorrow?", MSG_STATUS_RECEIVED, GX_STRING_ID_FRIDAY } },
    { { "Hi James, I sent you a draft with materials selection. Hope you like it this time", MSG_STATUS_SEND, GX_STRING_ID_THUESDAY },
      { "Hello my friend.", MSG_STATUS_RECEIVED, GX_STRING_ID_THUESDAY } }
};

/* Define message data structure. */
typedef struct MSG_DATA_STRUCT{
    MSG_INFO *msg_info;
    INT       contact_id;
    INT       msg_count;
}MSG_DATA;

/* Predefined message data.  */
MSG_DATA msg_data[MAX_MSG_INFO_COUNT + 1] = {
    { msg_info_memory[0], 0, 3 },
    { msg_info_memory[1], 4, 2 },
    { msg_info_memory[2], 6, 2 },
    { msg_info_memory[3], 7, 2 },
    { GX_NULL, 0, 0}
};

extern CONTACT_INFO contact_info[];

/* Define message row structure. */
typedef struct MSG_ROW_STRUCT{
    GX_WIDGET widget;
    GX_WIDGET title_bar;
    GX_PROMPT name;
    GX_PROMPT time;
    GX_MULTI_LINE_TEXT_VIEW content;
}MSG_ROW;

typedef struct DIALOG_ROW_STRUCT{
    GX_PROMPT time;
    GX_MULTI_LINE_TEXT_VIEW content;
}DIALOG_ROW;

MSG_ROW       msg_row_memory[MSG_LIST_VISIBLE_ROWS + 1];
DIALOG_ROW    dialog_row_memory[MAX_MSG_COUNT];

VOID msg_send_screen_info_set(INT info_id);
VOID dialog_create(MSG_DATA *data);
VOID dialog_update(MSG_DATA *data);
VOID dialog_delete();
VOID msg_send(MSG_DATA *data, GX_CHAR *text);
VOID new_msg_data_add(INT contact_id);
VOID msg_search_window_animation_start(USHORT type);
VOID msg_search_window_animation_update();

INT selected_msg_data_id = -1;
GX_BOOL msg_sending = GX_FALSE;
INT preloader_draw_angle = 0;
INT animation_shift = 10;

/*************************************************************************************/
/* Handle message screen                                                             */
/*************************************************************************************/
UINT msg_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Process default event process. */
        template_main_event_handler(window, event_ptr);

        /* Set selected item of message abstract list to NULL. */
        gx_vertical_list_selected_set(&msg_screen.msg_screen_msg_list, -1);
        break;

    case GX_SIGNAL(ID_MSG_LIST, GX_EVENT_LIST_SELECT):
        /* Retrieve selected item of message abstract list. */
        gx_vertical_list_selected_index_get(&msg_screen.msg_screen_msg_list, &selected_msg_data_id);

        /* Switch to message send screen. */
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&msg_send_screen);
        break;

    case GX_SIGNAL(ID_NEW_MSG, GX_EVENT_CLICKED):
        /* Add contact 10 to message data array, and switch to message send screen
           to send messsage to contact 10.  */
        new_msg_data_add(10);
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&msg_send_screen);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
UINT msg_screen_text_input_event_handler(GX_SINGLE_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_FOCUS_GAINED:
        msg_search_window_animation_start(ANIMATION_LENGTHEN);

        /* Process default event process. */
        gx_single_line_text_input_event_process((GX_WIDGET *)input, event_ptr);
        break;

    case GX_EVENT_FOCUS_LOST:
        msg_search_window_animation_start(ANIMATION_SHORTEN);

        /* Process default event process. */
        gx_single_line_text_input_event_process((GX_WIDGET *)input, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TEXT_INPUT_ANIMATION_TIMER)
        {
            msg_search_window_animation_update();
        }
        else
        {
            gx_single_line_text_input_event_process(input, event_ptr);
        }
        break;
    default:
        return gx_single_line_text_input_event_process(input, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID msg_search_window_animation_start(USHORT type)
{
    if (type == ANIMATION_LENGTHEN)
    {
        animation_shift = -10;
        gx_widget_fill_color_set(&msg_screen.msg_screen_text_input_bg, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);
        gx_system_timer_start(&msg_screen.msg_screen_text_input, TEXT_INPUT_ANIMATION_TIMER, 1, 1);
    }
    else
    {
        animation_shift = 10;
        gx_widget_fill_color_set(&msg_screen.msg_screen_text_input_bg, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY);
        gx_system_timer_start(&msg_screen.msg_screen_text_input, TEXT_INPUT_ANIMATION_TIMER, 1, 1);
        gx_single_line_text_input_buffer_clear(&msg_screen.msg_screen_text_input);
    }
}

/*************************************************************************************/
VOID msg_search_window_animation_update()
{
    GX_RECTANGLE size;

    size = msg_screen.msg_screen_text_input_bg.gx_widget_size;

    if (((animation_shift < 0) && (size.gx_rectangle_left <= 199)) ||
        ((animation_shift > 0) && (size.gx_rectangle_left >= 375)))
    {
        gx_system_timer_stop(&msg_screen.msg_screen_text_input, TEXT_INPUT_ANIMATION_TIMER);
        return;
    }

    size.gx_rectangle_left += animation_shift;

    /* Resize text input background window. */
    gx_widget_resize(&msg_screen.msg_screen_text_input_bg, &size);

    size = msg_screen.msg_screen_text_input.gx_widget_size;

    if (animation_shift < 0)
    {
        size.gx_rectangle_left = msg_screen.msg_screen_text_input_bg.gx_widget_size.gx_rectangle_left + 5;
    }
    else
    {
        size.gx_rectangle_left = msg_screen.msg_screen_text_input_bg.gx_widget_size.gx_rectangle_left;
    }

    /* Resize text input widget. */
    gx_widget_resize(&msg_screen.msg_screen_text_input, &size);

}

/*************************************************************************************/
VOID msg_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    MSG_ROW *row = (MSG_ROW *)widget;
    GX_RECTANGLE size;
    GX_BOOL result;
    INT     contact_id;
    INT     msg_count;
    GX_STRING string;

    gx_widget_created_test(&row->widget, &result);
    if (!result)
    {
        gx_utility_rectangle_define(&size, 0, 0, 245, 80);
        gx_widget_create(&row->widget, "", list, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &size);

        gx_utility_rectangle_define(&size, 0, 0, 245, 30);
        gx_widget_create(&row->title_bar, "", &row->widget, GX_STYLE_BORDER_NONE, 0, &size);
        gx_widget_fill_color_set(&row->title_bar, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_BLUE, GX_COLOR_ID_DARK_GRAY);

        /* Create prompt to show contact name. */
        gx_utility_rectangle_define(&size, 8, 5, 160, MSG_TITLE_HEIGHT);
        gx_prompt_create(&row->name, "", &row->widget, 0, GX_STYLE_BORDER_NONE | GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT, 0, &size);
        gx_prompt_text_color_set(&row->name, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

        /* Create prompt to show message time. */
        gx_utility_rectangle_define(&size, 160, 3, 240, 30);
        gx_prompt_create(&row->time, "", &row->widget, 0, GX_STYLE_BORDER_NONE | GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_RIGHT, 0, &size);
        gx_prompt_text_color_set(&row->time, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);
        gx_prompt_font_set(&row->time, GX_FONT_ID_SMALL);

        /* Create text view to show message. */
        gx_utility_rectangle_define(&size, 3, MSG_TITLE_HEIGHT + 8, 240, MSG_TITLE_HEIGHT + 8 + MSG_VIEW_HEIGHT);
        gx_multi_line_text_view_create(&row->content, "", &row->widget, 0, GX_STYLE_BORDER_NONE | GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT, 0, &size);
        gx_multi_line_text_view_font_set(&row->content, GX_FONT_ID_SYSTEM);
        gx_multi_line_text_view_text_color_set(&row->content, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);
    }

    contact_id = msg_data[index].contact_id;
    msg_count = msg_data[index].msg_count;

    string.gx_string_ptr = contact_info[contact_id].name;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_FULL_NAME_LENGTH);
    gx_prompt_text_set_ext(&row->name, &string);
    gx_prompt_text_id_set(&row->time, msg_data[index].msg_info[msg_count - 1].time_id);

    string.gx_string_ptr = msg_data[index].msg_info[msg_count - 1].text;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_MSG_LENGTH);
    gx_multi_line_text_view_text_set_ext(&row->content, &string);
}


/*************************************************************************************/
VOID msg_list_children_create(GX_VERTICAL_LIST *list)
{
    INT index;

    for (index = 0; index < MSG_LIST_VISIBLE_ROWS + 1; index++)
    {
        msg_list_row_create(list, (GX_WIDGET *)&msg_row_memory[index], index);
    }
}

/*************************************************************************************/
/* Handle message send screen                                                        */
/*************************************************************************************/
UINT msg_send_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        template_main_event_handler(window, event_ptr);

        /* Initiate the screen with the information of selected contact. */
        msg_send_screen_info_set(selected_msg_data_id);
        break;

    case GX_EVENT_HIDE:
        gx_window_event_process(window, event_ptr);
        dialog_delete();
        break;

    case GX_SIGNAL(ID_BACK, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&msg_screen);

        gx_single_line_text_input_buffer_clear(&keyboard_screen.keyboard_screen_input_field);

        /* Update message list.  */
        msg_list_children_create(&msg_screen.msg_screen_msg_list);
        break;

    case GX_SIGNAL(ID_REPLAY, GX_EVENT_CLICKED):
        gx_widget_attach(window, (GX_WIDGET *)&keyboard_screen);
        break;

    case GX_EVENT_PEN_DOWN:
        if (keyboard_screen.gx_widget_status & GX_STATUS_VISIBLE)
        {
            if (!gx_utility_rectangle_point_detect(&keyboard_screen.gx_widget_size, event_ptr->gx_event_payload.gx_event_pointdata))
            {
                gx_widget_detach((GX_WIDGET *)&keyboard_screen);
            }
        }
        break;

    case GX_SIGNAL(ID_SEND, GX_EVENT_CLICKED):
        if (msg_data[selected_msg_data_id].msg_count < MAX_MSG_COUNT)
        {
            msg_sending = GX_TRUE;
            gx_system_timer_start(window, MSG_SEND_SCREEN_TIMER, 10, 10);
            gx_widget_front_move(&msg_send_screen.msg_send_screen_preloader_window, GX_NULL);
            gx_system_dirty_mark((GX_WIDGET *)&msg_send_screen.msg_send_screen_preloader_window);
        }
        
        gx_widget_detach(&keyboard_screen);
        break;

    case GX_EVENT_TIMER:
        preloader_draw_angle += 30;
        gx_system_dirty_mark((GX_WIDGET *)&msg_send_screen.msg_send_screen_preloader_window);

        if (preloader_draw_angle > 180)
        {
            preloader_draw_angle = 0;
            msg_sending = GX_FALSE;
            msg_send(&msg_data[selected_msg_data_id], keyboard_screen.keyboard_screen_input_field.gx_single_line_text_input_buffer);
            gx_single_line_text_input_buffer_clear(&keyboard_screen.keyboard_screen_input_field);
            gx_system_timer_stop(window, MSG_SEND_SCREEN_TIMER);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID msg_send_screen_info_set( INT id)
{
    INT contact_id = msg_data[id].contact_id;
    GX_STRING string;

    string.gx_string_ptr = contact_info[contact_id].lastname;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_NAME_LENGTH);

    gx_prompt_text_set_ext(&msg_send_screen.msg_send_screen_name, &string);
    dialog_create(&msg_data[id]);
}

/*************************************************************************************/
VOID msg_send(MSG_DATA *data, GX_CHAR *text)
{
    INT count;
    INT string_length;
    GX_VERTICAL_LIST *list;
    INT total_rows;

    if (!data->msg_info)
    {
        list = &msg_screen.msg_screen_msg_list;

        total_rows = list->gx_vertical_list_total_rows;

        if (total_rows < MAX_MSG_INFO_COUNT)
        {
            gx_vertical_list_total_rows_set(list, total_rows + 1);
        }
        else
        {
            total_rows--;
        }

        msg_data[total_rows].msg_info = msg_info_memory[total_rows];
    }

    count = data->msg_count;

    /* Save text to msg date struct. */
    data->msg_count++;
    data->msg_info[count].status = MSG_STATUS_SEND;
    data->msg_info[count].time_id = GX_STRING_ID_SENT;
    string_length = string_length_get(text, MAX_MSG_LENGTH);
    memcpy((void *)data->msg_info[count].text, (void *)text, string_length + 1);

    /* Update message dialog window.  */
    dialog_update(data);

    return;
}

/*************************************************************************************/
VOID dialog_row_create(GX_WIDGET *parent, GX_WIDGET *widget, INT index, MSG_DATA *data, INT top)
{
    GX_RECTANGLE   size;
    DIALOG_ROW    *row = (DIALOG_ROW *)widget;
    GX_FONT       *font;
    GX_VALUE       text_width;
    GX_VALUE       font_height;
    GX_VALUE       text_view_height;
    GX_STRING      string;

    gx_widget_font_get(parent, GX_FONT_ID_SYSTEM, &font);
    string.gx_string_ptr = data->msg_info[index].text;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_MSG_LENGTH);
    gx_system_string_width_get_ext(font, &string, &text_width);
    font_height = font->gx_font_line_height;
    text_view_height = font_height + text_width / TEXT_BOX_WIDTH * font_height + 10;

    top += TEXT_BOX_SPACE;

    gx_utility_rectangle_define(&size, parent->gx_widget_size.gx_rectangle_left + 5,
        top,
        parent->gx_widget_size.gx_rectangle_left + TEXT_BOX_WIDTH, top + text_view_height);

    if (data->msg_info[index].status & MSG_STATUS_SEND)
    {
        gx_utility_rectangle_shift(&size, 35, 0);

        if (text_width < TEXT_BOX_WIDTH)
        {
            size.gx_rectangle_left = size.gx_rectangle_right - text_width - 10;
        }
    }
    else
    {
        if (text_width < TEXT_BOX_WIDTH)
        {
            size.gx_rectangle_right = size.gx_rectangle_left + text_width + 10;
        }
    }

    /* Create a multi line text view to show message text.  */
    gx_multi_line_text_view_create(&row->content, GX_NULL, parent, GX_NULL, GX_STYLE_TEXT_LEFT, GX_NULL, &size);
    gx_multi_line_text_view_font_set(&row->content, GX_FONT_ID_SYSTEM);
    gx_multi_line_text_view_line_space_set(&row->content, 2);
    gx_multi_line_text_view_whitespace_set(&row->content, 2);
    string.gx_string_ptr = data->msg_info[index].text;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_MSG_LENGTH);
    gx_multi_line_text_view_text_set_ext(&row->content, &string);
    gx_widget_fill_color_set(&row->content, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY);

    /* Create a prompt to show the sending or receiving time to a message. */

    size.gx_rectangle_top -= TIME_BOX_HEIGHT;
    size.gx_rectangle_bottom = size.gx_rectangle_top + TIME_BOX_HEIGHT;

    if (data->msg_info[index].status & MSG_STATUS_SEND)
    {
        size.gx_rectangle_left = size.gx_rectangle_right - TEXT_BOX_WIDTH;
        gx_prompt_create(&row->time, GX_NULL, parent, GX_NULL, GX_STYLE_TEXT_RIGHT | GX_STYLE_TRANSPARENT, GX_NULL, &size);
    }
    else
    {
        size.gx_rectangle_right = size.gx_rectangle_left + TEXT_BOX_WIDTH;
        gx_prompt_create(&row->time, GX_NULL, parent, GX_NULL, GX_STYLE_TEXT_LEFT | GX_STYLE_TRANSPARENT, GX_NULL, &size);
    }

    gx_prompt_text_id_set(&row->time, data->msg_info[index].time_id);
    gx_prompt_text_color_set(&row->time, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    gx_prompt_font_set(&row->time, GX_FONT_ID_SMALL);
}

/*************************************************************************************/
VOID dialog_client_update(GX_WINDOW *dialog_frame, GX_WINDOW *dialog, INT dialog_bottom)
{
    GX_RECTANGLE size;
    if (dialog_bottom > dialog_frame->gx_window_client.gx_rectangle_bottom)
    {
        gx_widget_show(&msg_send_screen.msg_send_screen_scroll);

        size = dialog->gx_widget_size;
        size.gx_rectangle_bottom = dialog_bottom;
        gx_widget_resize(dialog, &size);

        gx_widget_shift(dialog, 0, dialog_frame->gx_widget_size.gx_rectangle_bottom - dialog->gx_widget_size.gx_rectangle_bottom, GX_TRUE);
        gx_scrollbar_reset(&msg_send_screen.msg_send_screen_scroll, GX_NULL);
    }
    else
    {
        gx_widget_hide(&msg_send_screen.msg_send_screen_scroll);
    }
}

/*************************************************************************************/
VOID dialog_create(MSG_DATA *data)
{
GX_WINDOW  *dialog = &msg_send_screen.msg_send_screen_dialog;
GX_WINDOW  *dialog_frame = &msg_send_screen.msg_send_screen_dialog_frame;
INT         count;
INT         top;

    top = dialog->gx_widget_size.gx_rectangle_top;

    for (count = 0; count < data->msg_count; count++)
    {
        dialog_row_create((GX_WIDGET *)dialog, (GX_WIDGET *)&dialog_row_memory[count], count, data, top);
        top = dialog_row_memory[count].content.gx_widget_size.gx_rectangle_bottom + 1;
    }

    /* Resize dailog window. */
    dialog_client_update(dialog_frame, dialog, top);
}

/*************************************************************************************/
VOID dialog_update(MSG_DATA *data)
{
GX_WINDOW  *dialog = &msg_send_screen.msg_send_screen_dialog;
GX_WINDOW  *dialog_frame = &msg_send_screen.msg_send_screen_dialog_frame;
INT         count =data->msg_count - 1;
INT         top;
DIALOG_ROW *row = (DIALOG_ROW *)&dialog_row_memory[count];

    if (count == 0)
    {
        top = dialog->gx_widget_size.gx_rectangle_top;
    }
    else
    {
        top = dialog_row_memory[count - 1].content.gx_widget_size.gx_rectangle_bottom + 1;
    }

    dialog_row_create((GX_WIDGET *)dialog, (GX_WIDGET *)row, count, data, top);

    gx_prompt_text_color_set(&row->time, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE);
    top = dialog_row_memory[count].content.gx_widget_size.gx_rectangle_bottom + 1;

    dialog_client_update(dialog_frame, dialog, top);
}

/*************************************************************************************/
VOID dialog_delete()
{
    GX_WINDOW  *dialog = &msg_send_screen.msg_send_screen_dialog;
    GX_WINDOW  *dialog_frame = &msg_send_screen.msg_send_screen_dialog_frame;
    GX_WIDGET  *child;
    GX_WIDGET  *next_child;
    GX_RECTANGLE size;

    child = dialog->gx_widget_first_child;
    while (child)
    {
        next_child = child->gx_widget_next;
        gx_widget_delete(child);
        child = next_child;
    }

    size = dialog->gx_widget_size;
    size.gx_rectangle_top = dialog_frame->gx_widget_size.gx_rectangle_top;
    size.gx_rectangle_bottom = dialog_frame->gx_widget_size.gx_rectangle_bottom;

    gx_widget_resize(dialog, &size);
}

/*************************************************************************************/
VOID preloader_windown_draw(GX_WINDOW *window)
{
    GX_PIXELMAP *map;

    /* Call default window draw.  */
    gx_window_draw(window);

    if (msg_sending)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_PRELOADER, &map);
        gx_canvas_pixelmap_rotate(window->gx_widget_size.gx_rectangle_left, window->gx_widget_size.gx_rectangle_top, map, preloader_draw_angle,
            -1, -1);
    }
}

/*************************************************************************************/
VOID new_msg_data_add(INT contact_id)
{
    GX_VERTICAL_LIST *list = &msg_screen.msg_screen_msg_list;

    INT total_rows = list->gx_vertical_list_total_rows;

    INT index;

    for (index = 0; index < total_rows; index++)
    {
        if (msg_data[index].contact_id == contact_id)
        {
            /* Conversation already exist.  */
            selected_msg_data_id = index;
            return;
        }
    }

    if (total_rows >= MAX_MSG_INFO_COUNT)
    {
        total_rows--;
    }

    /* Create a new conversation.  */
    msg_data[total_rows].contact_id = contact_id;
    msg_data[total_rows].msg_count = 0;

    selected_msg_data_id = total_rows;
}
