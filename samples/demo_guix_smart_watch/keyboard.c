/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

/* Define constants specific to this implemenation.  */
#define KEY_PRESS_EVENT     GX_FIRST_APP_EVENT

#define KEY_NUMBER_KEYBOARD 0x1101
#define KEY_SIMBLE_KEYBOARD 0x1102

enum keyboard_type
{
    KEYBOARD_TYPE_CHARACTER = 1,
    KEYBOARD_TYPE_NUMBER,
    KEYBOARD_TYPE_SIMBLE
};

#define KEYBOARD_STYLE_LOWERCASE 1

USHORT keyboard_type = KEYBOARD_TYPE_CHARACTER;
USHORT keyboard_style = 0;

/* declare a structure to define one keyboard key */
typedef struct KEY_LAYOUT_ENTRY_STRUCT {
    GX_VALUE   xoffset;
    GX_VALUE   yoffset;
    GX_VALUE   width;
    GX_VALUE   height;
    GX_RESOURCE_ID icon;
    char      *text;
    USHORT     key_val;
} KEY_LAYOUT_ENTRY;

/* declare a keyboard_key control block, derived from GX_BUTTON */

typedef struct KEY_WIDGET_STRUCT {
    GX_BUTTON_MEMBERS_DECLARE
    GX_RESOURCE_ID icon;
    char          *text;
    USHORT         key_val;
} KEY_WIDGET;

/* define an array of keys. This array can easily be modified or new versions added to define
alternative keyboard layouts and keyboard key values
*/
#define NUM_CHARACTERS    26
#define NUM_DEFAULT_KEYS  32
KEY_WIDGET key_control_blocks[NUM_DEFAULT_KEYS];

KEY_LAYOUT_ENTRY key_layout_uppercase[NUM_DEFAULT_KEYS + 1] = {
    { 25, 31, 14, 17, 0, "A", 'A' },
    { 149, 56, 14, 17, 0, "B", 'B' },
    { 97, 56, 14, 17, 0, "C", 'C' },
    { 71, 31, 16, 17, 0, "D", 'D' },
    { 60, 4, 14, 17, 0, "E", 'E' },
    { 96, 31, 14, 17, 0, "F", 'F' },
    { 119, 31, 14, 17, 0, "G", 'G' },
    { 143, 31, 14, 17, 0, "H", 'H' },
    { 180, 4, 14, 17, 0, "I", 'I' },
    { 167, 29, 16, 21, 0, "J", 'J' },
    { 191, 31, 14, 17, 0, "K", 'K' },
    { 215, 31, 14, 17, 0, "L", 'L' },
    { 201, 56, 14, 17, 0, "M", 'M' },
    { 175, 56, 14, 17, 0, "N", 'N' },
    { 204, 4, 14, 17, 0, "O", 'O' },
    { 228, 4, 14, 17, 0, "P", 'P' },
    { 8, 4, 14, 17, 0, "Q", 'Q' },
    { 84, 4, 14, 17, 0, "R", 'R' },
    { 47, 31, 14, 17, 0, "S", 'S' },
    { 108, 4, 14, 17, 0, "T", 'T' },
    { 156, 4, 14, 17, 0, "U", 'U' },
    { 123, 56, 14, 17, 0, "V", 'V' },
    { 32, 4, 18, 17, 0, "W", 'W' },
    { 71, 56, 14, 17, 0, "X", 'X' },
    { 132, 4, 14, 17, 0, "Y", 'Y' },
    { 47, 56, 14, 17, 0, "Z", 'Z' },
    { 7, 52, 32, 23, 0, "123", KEY_NUMBER_KEYBOARD },
    { 16, 81, 40, 23,  0, "bksp", GX_KEY_BACKSPACE },
    { 59, 81, 26, 23, GX_PIXELMAP_ID_UP_ARROW, 0, GX_KEY_UP_ARROW },
    { 90, 81, 101, 23, 0, 0, GX_KEY_SPACE },
    { 194, 82, 46, 23, 0, "enter", GX_KEY_SELECT },
    { 225, 52, 24, 23, 0, ";)", KEY_SIMBLE_KEYBOARD },
    { 0, 0, 0, 0, 0, 0, 0 },
};

KEY_LAYOUT_ENTRY key_layout_lowercase[NUM_CHARACTERS + 1] = {
    { 25, 31, 14, 17, 0, "a", 'a' },
    { 149, 56, 14, 17, 0, "b", 'b' },
    { 97, 56, 14, 17, 0, "c", 'c' },
    { 71, 31, 14, 17, 0, "d", 'd' },
    { 60, 4, 14, 17, 0, "e", 'e' },
    { 96, 31, 14, 17, 0, "f", 'f' },
    { 119, 26, 14, 22, 0, "g", 'g' },
    { 143, 31, 14, 17, 0, "h", 'h' },
    { 180, 4, 14, 17, 0, "i", 'i' },
    { 167, 29, 14, 21, 0, "j", 'j' },
    { 191, 31, 14, 17, 0, "k", 'k' },
    { 215, 31, 14, 17, 0, "l", 'l' },
    { 201, 56, 18, 17, 0, "m", 'm' },
    { 175, 56, 14, 17, 0, "n", 'n' },
    { 204, 4, 14, 17, 0, "o", 'o' },
    { 228, 4, 14, 17, 0, "p", 'p' },
    { 8, 4, 14, 17, 0, "q", 'q' },
    { 84, 4, 14, 17, 0, "r", 'r' },
    { 47, 31, 14, 17, 0, "s", 's' },
    { 108, 5, 14, 17, 0, "t", 't' },
    { 156, 4, 14, 17, 0, "u", 'u' },
    { 123, 56, 14, 17, 0, "v", 'v' },
    { 32, 4, 18, 17, 0, "w", 'w' },
    { 71, 56, 14, 17, 0, "x", 'x' },
    { 132, 4, 14, 17, 0, "y", 'y' },
    { 47, 56, 14, 17, 0, "z", 'z' },
    {0, 0, 0, 0, 0, 0, 0}};

KEY_LAYOUT_ENTRY key_layout_number[NUM_CHARACTERS + 1] = {
    { 228, 4, 14, 17, 0, "0", 0x30 },
    { 8, 4, 14, 17, 0, "1", 0x31 },
    { 33, 4, 14, 17, 0, "2", 0x32 },
    { 60, 4, 14, 17, 0, "3", 0x33 },
    { 84, 4, 14, 17, 0, "4", 0x34 },
    { 108, 4, 14, 17, 0, "5", 0x35 },
    { 132, 4, 14, 17, 0, "6", 0x36 },
    { 156, 4, 14, 17, 0, "7", 0x37 },
    { 180, 4, 14, 17, 0, "8", 0x38 },
    { 204, 4, 14, 17, 0, "9", 0x39 },
    { 25, 31, 14, 17, 0, ":", 0x3A },
    { 47, 31, 14, 17, 0, ";", 0x3B },
    { 71, 31, 14, 17, 0, "!", 0x21 },
    { 95, 31, 14, 17, 0, "\"", 0x22 },
    { 119, 31, 14, 17, 0, "#", 0x23 },
    { 143, 31, 14, 17, 0, "$", 0x24 },
    { 165, 30, 19, 21, 0, "%", 0x25 },
    { 191, 31, 14, 17, 0, "&", 0x26 },
    { 215, 31, 14, 17, 0, "'", 0x27 },
    { 47, 56, 14, 17, 0, "(", 0x28 },
    { 71, 56, 14, 17, 0, ")", 0x29 },
    { 97, 56, 14, 17, 0, "*", 0x2A },
    { 123, 56, 14, 17, 0, "+", 0x2B },
    { 149, 56, 14, 17, 0, ",", 0x2C },
    { 175, 56, 14, 17, 0, ".", 0x2E },
    { 201, 56, 14, 17, 0, "?", 0x3F },
    { 0, 0, 0, 0, 0, 0, 0 } };

/*************************************************************************************/
VOID key_widget_draw(KEY_WIDGET *key)
{
    INT x_offset = 0;
    INT y_offset = 1;
    GX_RESOURCE_ID font_id = GX_FONT_ID_NORMAL;
    GX_RESOURCE_ID color_id = GX_COLOR_ID_MILK_WHITE;
    GX_PIXELMAP *map;
    GX_STRING string;

    if (key->text)
    {
        string.gx_string_ptr = key->text;
        string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_KEY_LENGTH);
        if (string.gx_string_length > 1)
        {
            font_id = GX_FONT_ID_SMALL;
            color_id = GX_COLOR_ID_GRAY;
        }
        if (key->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            x_offset++;
            y_offset++;
        }

        gx_widget_text_draw_ext(key, color_id,
            font_id, &string,
            x_offset, y_offset);
    }
    else
    {
        if (key->icon)
        {
            gx_context_pixelmap_get(key->icon, &map);

            if (map)
            {
                x_offset = key->gx_widget_size.gx_rectangle_right - key->gx_widget_size.gx_rectangle_left + 1;
                x_offset -= map->gx_pixelmap_width;
                x_offset /= 2;
                x_offset += key->gx_widget_size.gx_rectangle_left;

                y_offset = key->gx_widget_size.gx_rectangle_bottom - key->gx_widget_size.gx_rectangle_top + 1;
                y_offset -= map->gx_pixelmap_height;
                y_offset /= 2;
                y_offset += key->gx_widget_size.gx_rectangle_top;

                gx_context_fill_color_set(key->gx_widget_normal_fill_color);

                if (key->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
                {
                    x_offset++;
                    y_offset++;
                }
                gx_canvas_pixelmap_draw(x_offset, y_offset, map);
            }
        }
    }
}

/*************************************************************************************/
VOID key_widget_select(GX_WIDGET *widget)
{
    GX_BUTTON *button = (GX_BUTTON *)widget;
    KEY_WIDGET *key = (KEY_WIDGET *)widget;

    GX_EVENT notification;

    /* Set style for pen down.  */
    button->gx_widget_style |= GX_STYLE_BUTTON_PUSHED;
    gx_widget_front_move(widget, NULL);

    /* Mark as dirty.  */
    gx_system_dirty_mark(widget);

    /* send notification to parent keyboard frame */
    notification.gx_event_type = KEY_PRESS_EVENT;
    notification.gx_event_payload.gx_event_ushortdata[0] = key->key_val;
    notification.gx_event_target = 0;
    widget->gx_widget_parent->gx_widget_event_process_function(widget->gx_widget_parent, &notification);
}

/*************************************************************************************/
VOID key_widget_create(KEY_WIDGET *key_widget, KEY_LAYOUT_ENTRY *entry, GX_WIDGET *frame)
{
    GX_RECTANGLE size;
    INT left;
    INT top;
    GX_BOOL result;

    gx_widget_created_test(key_widget, &result);

    left = frame->gx_widget_size.gx_rectangle_left + entry->xoffset;
    top = frame->gx_widget_size.gx_rectangle_top + entry->yoffset;

    gx_utility_rectangle_define(&size, left, top,
        left + entry->width - 1,
        top + entry->height - 1);

    if (!result)
    {
        /* first create the base pixelmap button */
        gx_button_create((GX_BUTTON *)key_widget, NULL, frame, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED | GX_STYLE_BUTTON_REPEAT, 0, &size);
        gx_widget_fill_color_set((GX_WIDGET *)key_widget, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);

        gx_widget_draw_set(key_widget, key_widget_draw);
        key_widget->gx_button_select_handler = key_widget_select;
    }
    else
    {
        gx_widget_resize((GX_BUTTON *)key_widget, &size);
    }

    /* now initialize the extended fields */

    key_widget->icon = entry->icon;
    key_widget->text = entry->text;
    key_widget->key_val = entry->key_val;
}

/*************************************************************************************/
VOID keyboard_buttons_create(GX_WINDOW *frame, KEY_LAYOUT_ENTRY *entry)
{
    INT index = 0;

    while (entry->key_val)
    {
        key_widget_create(&key_control_blocks[index], entry, (GX_WIDGET *)frame);
        entry++;
        index++;
    }
}

/*************************************************************************************/
VOID keyboard_children_create()
{
    keyboard_buttons_create(&keyboard_screen.keyboard_screen_keyboard, key_layout_uppercase);
}

/*************************************************************************************/
UINT keyboard_screen_event_handler(GX_WINDOW *frame, GX_EVENT *event_ptr)
{
    GX_EVENT key_event;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(frame, event_ptr);
        gx_system_focus_claim((GX_WIDGET *)frame);
        break;

    case GX_EVENT_FOCUS_GAINED:
        if (keyboard_screen.gx_widget_parent != (GX_WIDGET *)&msg_send_screen)
        {
            gx_widget_attach((GX_WIDGET *)&msg_send_screen, (GX_WIDGET *)&keyboard_screen);
        }
        break;

    case GX_EVENT_FOCUS_LOST:
        if (keyboard_screen.gx_widget_parent)
        {
            gx_widget_detach((GX_WIDGET *)&keyboard_screen);
        }
        break;

    case KEY_PRESS_EVENT:
        /* these events arrive from the key buttons. generate keyboard input
        events sent to the input field. The key value is held in the gx_event_ushortdata[0] field
        */
        key_event.gx_event_target = (GX_WIDGET *)&keyboard_screen.keyboard_screen_input_field;
        key_event.gx_event_sender = frame->gx_widget_id;
        key_event.gx_event_payload.gx_event_ushortdata[0] = event_ptr->gx_event_payload.gx_event_ushortdata[0];
        key_event.gx_event_type = GX_EVENT_KEY_DOWN;
        gx_system_event_send(&key_event);
        key_event.gx_event_type = GX_EVENT_KEY_UP;
        gx_system_event_send(&key_event);
        return 0;

    default:
        return gx_window_event_process(frame, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
UINT input_field_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
    GX_EVENT send_event;

    if (event_ptr->gx_event_type == GX_EVENT_KEY_DOWN)
    {
        switch (event_ptr->gx_event_payload.gx_event_ushortdata[0])
        {
        case KEY_NUMBER_KEYBOARD:
            /* Exchange to number keyboard.  */
            if (keyboard_type == KEYBOARD_TYPE_NUMBER)
            {
                if (keyboard_style & KEYBOARD_STYLE_LOWERCASE)
                {
                    keyboard_buttons_create(&keyboard_screen.keyboard_screen_keyboard, key_layout_lowercase);
                }
                else
                {
                    keyboard_buttons_create(&keyboard_screen.keyboard_screen_keyboard, key_layout_uppercase);
                }
                keyboard_type = KEYBOARD_TYPE_CHARACTER;
            }
            else
            {
                keyboard_buttons_create(&keyboard_screen.keyboard_screen_keyboard, key_layout_number);
                keyboard_type = KEYBOARD_TYPE_NUMBER;
            }
            gx_system_dirty_mark(&keyboard_screen.keyboard_screen_keyboard);
            break;

        case KEY_SIMBLE_KEYBOARD:
            /* Exchange to simble keyboard.  */
            break;

        case GX_KEY_UP_ARROW:
            /* Character case change. */
            if (keyboard_style & KEYBOARD_STYLE_LOWERCASE)
            {
                if (keyboard_type == KEYBOARD_TYPE_CHARACTER)
                {
                    keyboard_buttons_create(&keyboard_screen.keyboard_screen_keyboard, key_layout_uppercase);
                }

                keyboard_style &= (~KEYBOARD_STYLE_LOWERCASE);
            }
            else
            {
                if (keyboard_type == KEYBOARD_TYPE_CHARACTER)
                {
                    keyboard_buttons_create(&keyboard_screen.keyboard_screen_keyboard, key_layout_lowercase);
                }
                keyboard_style |= KEYBOARD_STYLE_LOWERCASE;
            }
            gx_system_dirty_mark(&keyboard_screen.keyboard_screen_keyboard);
            break;

        case GX_KEY_SELECT:
            send_event.gx_event_target = (GX_WIDGET *)&msg_send_screen;
            send_event.gx_event_sender = 0;
            send_event.gx_event_payload.gx_event_ushortdata[0] = event_ptr->gx_event_payload.gx_event_ushortdata[0];
            send_event.gx_event_type = GX_SIGNAL(ID_SEND, GX_EVENT_CLICKED);
            gx_system_event_send(&send_event);
            break;
        }
    }

    return gx_single_line_text_input_event_process(text_input, event_ptr);
}