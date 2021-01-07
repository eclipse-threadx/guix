/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_keyboard_resources.h"
#include "guix_keyboard_specifications.h"

/* Define prototypes. */
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/* Define constants specific to this implemenation.  */
#define KEY_PRESS_EVENT GX_FIRST_APP_EVENT

extern GX_CONST GX_THEME *display_1_theme_table[];

GX_WINDOW_ROOT    *root;

/* declare a keyboard_key control block, derived from GX_PIXELMAP_BUTTON */

typedef struct KEY_WIDGET_STRUCT {
    GX_PIXELMAP_BUTTON_MEMBERS_DECLARE
    GX_RESOURCE_ID icon;
    char *text;
    USHORT key_val;
} KEY_WIDGET;

/* declare a structure to define one keyboard key */
typedef struct KEY_LAYOUT_ENTRY_STRUCT {
    GX_VALUE   xoffset;
    GX_VALUE   yoffset;
    GX_RESOURCE_ID normal_background;
    GX_RESOURCE_ID selected_background;
    GX_RESOURCE_ID icon;
    char      *text;
    USHORT     key_val;
} KEY_LAYOUT_ENTRY;

/* define an array of keys. This array can easily be modified or new versions added to define
   alternative keyboard layouts and keyboard key values
*/
#define MAX_KEY_LENGTH     5
#define NUM_DEFAULT_KEYS  33
KEY_WIDGET key_control_blocks[NUM_DEFAULT_KEYS];

KEY_LAYOUT_ENTRY key_layout_1[NUM_DEFAULT_KEYS + 1] = {
    {  0,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "A",     'A'},
    { 67,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "B",     'B'}, 
    {134,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "C",     'C'}, 
    {201,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "D",     'D'}, 
    {268,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "E",     'E'}, 
    {335,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "F",     'F'}, 
    {402,    0,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,             0,   "G",     'G'}, 
    {  0,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "H",     'H'},
    { 67,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "I",     'I'}, 
    {134,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "J",     'J'}, 
    {201,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "K",     'K'}, 
    {268,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "L",     'L'}, 
    {335,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "M",     'M'}, 
    {402,    37,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "N",     'N'}, 
    {  0,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "O",     'O'},
    { 67,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "P",     'P'}, 
    {134,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "Q",     'Q'}, 
    {201,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "R",     'R'}, 
    {268,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "S",     'S'}, 
    {335,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "T",     'T'}, 
    {402,    74,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "U",     'U'}, 
    {  0,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "@",     '@'},
    { 67,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "V",     'V'}, 
    {134,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "W",     'W'}, 
    {201,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "X",     'X'}, 
    {268,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "Y",     'Y'}, 
    {335,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "Z",     'Z'}, 
    {402,   111,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "Enter",  GX_KEY_SELECT}, 
    {  0,   148,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "Home",   GX_KEY_HOME},
    { 67,   148,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            GX_PIXELMAP_ID_I_LEFTARROW,   NULL,     GX_KEY_LEFT_ARROW}, 
    {134,   148,  GX_PIXELMAP_ID_B_SPACE,     GX_PIXELMAP_ID_B_SPACE_H,               0,   NULL,     GX_KEY_SPACE}, 
    {335,   148,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            GX_PIXELMAP_ID_I_RIGHTARROW,   NULL,     GX_KEY_RIGHT_ARROW}, 
    {402,   148,  GX_PIXELMAP_ID_B_KEYBOARD,  GX_PIXELMAP_ID_B_KEYBOARD_H,            0,   "End",    GX_KEY_END}, 
    {  0,    0,                          0,                            0,             0,  NULL,       0}
};
    
/* Define prototypes.   */

VOID start_guix(VOID);
VOID  PopulateKeyboardButtons(GX_WINDOW *frame, KEY_LAYOUT_ENTRY *layout);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
		LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the keyboard screen */
    gx_studio_named_widget_create("keyboard_screen", (GX_WIDGET *) root, GX_NULL);

    /* populate buttons into keyboard frame */
    PopulateKeyboardButtons(&keyboard_screen.keyboard_screen_keyboard_frame,
                            key_layout_1);

    gx_single_line_text_input_style_set(&keyboard_screen.keyboard_screen_keyboard_input_field, 
                                        GX_STYLE_ENABLED | GX_STYLE_CURSOR_BLINK | GX_STYLE_CURSOR_ALWAYS);


    /* Show the root window to make it visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}


VOID key_widget_draw(KEY_WIDGET *key)
{
    INT x_offset = 0;
    INT y_offset = 1;
    GX_RESOURCE_ID font_id = GX_FONT_ID_MIDSIZE;
    GX_PIXELMAP *map;
    GX_STRING string;

    gx_pixelmap_button_draw((GX_PIXELMAP_BUTTON *) key);
        
    if (key->text)
    {
        string.gx_string_ptr = key->text;
        string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_KEY_LENGTH);
        if (string.gx_string_length > 1)
        {
            font_id = GX_FONT_ID_DEFAULT;
        }
        if (key ->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            x_offset++;
            y_offset++;
        }

        gx_widget_text_draw_ext(key, GX_COLOR_ID_WHITE,
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
                x_offset -= map ->gx_pixelmap_width;
                x_offset /=2;
                x_offset += key->gx_widget_size.gx_rectangle_left;

                y_offset = key->gx_widget_size.gx_rectangle_bottom - key->gx_widget_size.gx_rectangle_top + 1;
                y_offset -= map ->gx_pixelmap_height;
                y_offset /=2;
                y_offset += key->gx_widget_size.gx_rectangle_top;
            
                gx_canvas_pixelmap_draw(x_offset, y_offset, map);
            }
        }
    }
}

VOID key_widget_select(GX_WIDGET *widget)
{
GX_BUTTON *button = (GX_BUTTON *) widget;
KEY_WIDGET *key = (KEY_WIDGET *) widget;

GX_EVENT notification;

    /* Set style for pen down.  */
    button -> gx_widget_style |=  GX_STYLE_BUTTON_PUSHED;
    gx_widget_front_move(widget, NULL);
    
    /* Mark as dirty.  */
    gx_system_dirty_mark(widget);

    /* send notification to parent keyboard frame */
    notification.gx_event_type = KEY_PRESS_EVENT;
    notification.gx_event_payload.gx_event_ushortdata[0] = key->key_val;
    notification.gx_event_target = 0;
    widget->gx_widget_parent->gx_widget_event_process_function(widget->gx_widget_parent, &notification);
}

VOID key_widget_deselect(GX_WIDGET *key, GX_BOOL generate_event)
{
    /* Set style for pen down.  */
    key -> gx_widget_style &=  ~GX_STYLE_BUTTON_PUSHED;
    
    /* Mark as dirty.  */
    gx_system_dirty_mark(key);
}

VOID key_widget_create(KEY_WIDGET *key_widget, USHORT id, KEY_LAYOUT_ENTRY *entry, GX_WIDGET *frame)
{
    GX_RECTANGLE size;
    INT left;
    INT top;
    const GX_THEME *theme_ptr;

    GX_PIXELMAP *map;
    theme_ptr = display_1_theme_table[0];

    map = theme_ptr->theme_pixelmap_table[entry->selected_background];
    left = frame->gx_widget_size.gx_rectangle_left + entry->xoffset;
    top = frame->gx_widget_size.gx_rectangle_top + entry->yoffset;

    gx_utility_rectangle_define(&size, left, top,
        left + map->gx_pixelmap_width - 1,
        top + map->gx_pixelmap_height - 1);
   
    /* first create the base pixelmap button */
    gx_pixelmap_button_create((GX_PIXELMAP_BUTTON *) key_widget,
            NULL, frame,
            entry->normal_background,
            entry->selected_background,
            0, GX_STYLE_HALIGN_CENTER | GX_STYLE_VALIGN_CENTER | GX_STYLE_ENABLED,
            id, &size);
        
    /* now initialize the extended fields */

    key_widget->icon = entry->icon;
    key_widget->text = entry->text;
    key_widget->key_val = entry->key_val;
    gx_widget_draw_set(key_widget, key_widget_draw);
    key_widget->gx_button_select_handler = key_widget_select;
    key_widget->gx_button_deselect_handler = key_widget_deselect;
}

VOID PopulateKeyboardButtons(GX_WINDOW *frame, KEY_LAYOUT_ENTRY *entry)
{
    INT index = 0;
    
    while(entry ->key_val)
    {
        key_widget_create(&key_control_blocks[index], index + 1, entry, (GX_WIDGET *) frame);
        entry++;
        index++;
    }
}

UINT keyboard_frame_event_handler(GX_WINDOW *frame, GX_EVENT *event_ptr)
{
GX_EVENT key_event;

    switch(event_ptr->gx_event_type)
    {
    case KEY_PRESS_EVENT:
        /* these events arrive from the key buttons. generate keyboard input
           events sent to the input field. The key value is held in the gx_event_ushortdata[0] field
        */
        key_event.gx_event_target = (GX_WIDGET *)&keyboard_screen.keyboard_screen_keyboard_input_field;
        key_event.gx_event_sender = frame ->gx_widget_id;
        key_event.gx_event_payload.gx_event_ushortdata[0] = event_ptr->gx_event_payload.gx_event_ushortdata[0];
        key_event.gx_event_type = GX_EVENT_KEY_DOWN;
        gx_system_event_send(&key_event);
        key_event.gx_event_type = GX_EVENT_KEY_UP;
        gx_system_event_send(&key_event);
        return 0;

    default:
        return gx_window_event_process(frame, event_ptr);
    }
}

UINT input_field_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
    if (event_ptr ->gx_event_type == GX_SIGNAL(IDB_BACKSPACE, GX_EVENT_CLICKED))
    {
        gx_single_line_text_input_backspace(text_input);
        return 0;
    }

    return gx_single_line_text_input_event_process(text_input, event_ptr);
}

UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}