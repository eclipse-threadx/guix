#include "demo_guix_smart_watch.h"

#define MSG_COUNT 4

/* Extern message template definition.  */
extern GX_STUDIO_WIDGET message_template_define;

/* Define message information structure.  */
typedef struct MESSAGE_INFO_STRUCT {
    GX_RESOURCE_ID                     day;
    INT                                hour;
    INT                                minute;
    GX_RESOURCE_ID                     am_pm;
    GX_STRING                          msg;
}MSG_INFO;

/* Define message widgets.  */
MESSAGE_TEMPLATE_CONTROL_BLOCK msg_widget_list[MSG_COUNT];

/* Define message content.  */
static GX_CONST CHAR msg_1[] = "Missed Call\r (414) 785-7858";
static GX_CONST CHAR msg_2[] = "Hi Alex! We haven't seen you in a while. Open your BistroFast App for 1 $10 off $20+ reward! Don't wait, it expires in 5 days!";
static GX_CONST CHAR msg_3[] = "Missed Call\r (414) 785-7858";
static GX_CONST CHAR msg_4[] = "Hi Alex,\r We'd like to let you know that an update to your item 2UiKit is now availlable in your Downloads pages.";

/* Define message information.  */
MSG_INFO msg_info_list[MSG_COUNT] = {
    {GX_STRING_ID_TODAY, 7, 52, GX_STRING_ID_AM, {msg_1, sizeof(msg_1) - 1}},
    {GX_STRING_ID_YESTERDAY, 4, 37, GX_STRING_ID_PM, {msg_2, sizeof(msg_2) - 1}},
    {GX_STRING_ID_YESTERDAY, 5, 11, GX_STRING_ID_PM, {msg_3, sizeof(msg_3) - 1}},
    {GX_STRING_ID_YESTERDAY, 5, 40, GX_STRING_ID_PM, {msg_4, sizeof(msg_4) - 1}},
};

/******************************************************************************************/
/* Position message frame window children.                                                */
/******************************************************************************************/
static VOID position_children(GX_WIDGET *parent)
{
    GX_WIDGET *child;
    INT left = parent->gx_widget_size.gx_rectangle_left;
    INT top = parent->gx_widget_size.gx_rectangle_top;

    child = parent->gx_widget_first_child;
    while (child)
    {
        if (!(child->gx_widget_status & GX_STATUS_NONCLIENT))
        {
            
            gx_widget_shift(child, left - child->gx_widget_size.gx_rectangle_left, top - child->gx_widget_size.gx_rectangle_top, GX_FALSE);
            top = child->gx_widget_size.gx_rectangle_bottom + 1;
        }

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Calculate the height of the specified text.                                            */
/******************************************************************************************/
static INT calculate_text_height(GX_STRING *string, GX_VALUE client_width, GX_RESOURCE_ID font_id)
{
    GX_FONT  *font;
    GX_VALUE  text_width = 0;
    GX_VALUE  text_height = 0;
    GX_STRING tmp;
    GX_CHAR   tmp_buffer[2];
    GX_VALUE  tmp_width;
    UINT      index;

    gx_widget_font_get(&main_screen, font_id, &font);

    if (!font)
    {
        return 0;
    }

    tmp.gx_string_ptr = tmp_buffer;
    tmp.gx_string_length = 1;
    tmp_buffer[1] = '\0';

    for (index = 0; index < string->gx_string_length; index++)
    {
        tmp_buffer[0] = string->gx_string_ptr[index];
        
        gx_system_string_width_get_ext(font, &tmp, &tmp_width);
        text_width += tmp_width;

        if ((tmp_buffer[0] == '\r') || (index == string->gx_string_length - 1))
        {
            text_height += font->gx_font_line_height * (1 + text_width / client_width);
            text_width = 0;
        }
    }

    return text_height;
}

/******************************************************************************************/
/* Update message list.                                                                   */
/******************************************************************************************/
static VOID screen_meesage_update()
{
    INT index;
    GX_WIDGET *parent = (GX_WIDGET *)&message_screen.message_screen_message_frame;
    MESSAGE_TEMPLATE_CONTROL_BLOCK *msg_widget;
    GX_MULTI_LINE_TEXT_VIEW *view;
    MSG_INFO *info;
    GX_RECTANGLE size;
    INT text_height;
    GX_VALUE width;
    GX_BOOL created;

    for (index = 0; index < MSG_COUNT; index++)
    {
        msg_widget = &msg_widget_list[index];
        info = &msg_info_list[index];

        /* Test if the widget has been created.  */
        gx_widget_created_test(msg_widget, &created);

        if (!created)
        {
            /* Create message widget.  */
            gx_studio_widget_create((GX_BYTE*)msg_widget, &message_template_define, GX_NULL);

            /* Attach message bar and message text view to the message frame window.  */
            gx_widget_attach(parent, &msg_widget->message_template_message_bar);
            gx_widget_attach(parent, &msg_widget->message_template_text_view);
        }

        /* Set message content. */
        gx_prompt_text_id_set(&msg_widget->message_template_day_of_week, info->day);
        gx_numeric_prompt_value_set(&msg_widget->message_template_hour, info->hour);
        gx_numeric_prompt_value_set(&msg_widget->message_template_minute, info->minute);
        gx_prompt_text_id_set(&msg_widget->message_template_am_pm, info->am_pm);
        gx_multi_line_text_view_text_set_ext(&msg_widget->message_template_text_view, &info->msg);

        /* Resize multi line text view to fit message content.  */
        view = &msg_widget->message_template_text_view;
        gx_widget_width_get(view, &width);
        text_height = calculate_text_height(&info->msg, width, view->gx_multi_line_text_view_font_id);
        size = view->gx_widget_size;
        size.gx_rectangle_bottom = size.gx_rectangle_top + text_height + (view->gx_multi_line_text_view_whitespace << 1);
        gx_widget_resize(view, &size);
    }

    position_children(parent);
}

/******************************************************************************************/
/* Override the default event processing of "message_screen_template" to handle signals   */
/* from my child widgets.                                                                 */
/******************************************************************************************/
UINT message_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:

        /* Update message list.  */
        screen_meesage_update();
        break;

    default:
        break;
    }

    return screen_template_event_process(window, event_ptr);
}