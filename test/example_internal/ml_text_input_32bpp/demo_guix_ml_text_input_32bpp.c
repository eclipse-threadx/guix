/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "ml_text_input_32bpp_resources.h"
#include "ml_text_input_32bpp_specifications.h"

WINDOW_CONTROL_BLOCK *pMainScreen;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
GX_RESOURCE_ID font[3][2] = {{GX_FONT_ID_TEXT_INPUT_1BPP_18PIX, GX_FONT_ID_TEXT_INPUT_1BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_4BPP_18PIX, GX_FONT_ID_TEXT_INPUT_4BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_8BPP_18PIX, GX_FONT_ID_TEXT_INPUT_8BPP_24PIX}};
GX_BOOL scrollbar = GX_TRUE;
GX_BOOL cursor_blink = GX_TRUE;
GX_BOOL cursor_always = GX_FALSE;
UINT    cursor_height;
UINT    cursor_width;
UINT    font_depth = 2;
UINT    font_size = 0;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


GX_STUDIO_DISPLAY_INFO display_info;

VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainScreen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

void string_to_int(char *str, int len, int *value)
{
int i;
    
    *value = 0;

    for(i = 0; i < len; i++)
    {
        *value = *value * 10;
        *value += str[i] - '0';
    }
}

UINT ml_input_handler(GX_MULTI_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr)
{
    USHORT   key_value;
    GX_STRING str;

    if (event_ptr->gx_event_type == GX_EVENT_KEY_DOWN)
    {
        switch (event_ptr->gx_event_payload.gx_event_ushortdata[0])
        {
        case GX_KEY_BACKSPACE:
            return gx_multi_line_text_input_backspace(input);

        case GX_KEY_DELETE:
            return gx_multi_line_text_input_delete(input);

        case GX_KEY_HOME:
            return gx_multi_line_text_input_home(input);

        case GX_KEY_END:
            return gx_multi_line_text_input_end(input);

        case GX_KEY_LEFT_ARROW:
            return gx_multi_line_text_input_left_arrow(input);

        case GX_KEY_RIGHT_ARROW:
            return gx_multi_line_text_input_right_arrow(input);

        case GX_KEY_UP_ARROW:
            return gx_multi_line_text_input_up_arrow(input);

        case GX_KEY_DOWN_ARROW:
            return gx_multi_line_text_input_down_arrow(input);

        default:
            key_value = event_ptr->gx_event_payload.gx_event_ushortdata[0];

            if (key_value > 0x20 && key_value <= 0x7e)
            {
                str.gx_string_ptr = (GX_CHAR*)&key_value;
                str.gx_string_length = 1;
                return gx_multi_line_text_input_char_insert_ext(input, &str);
            }
        }
    }

    return gx_multi_line_text_input_event_process(input, event_ptr);
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_MULTI_LINE_TEXT_INPUT *ml_input;
GX_MULTI_LINE_TEXT_VIEW *ml_view;
GX_CHAR  *input_buffer;
UINT     string_size;
INT      value;

    ml_input = &pMainScreen->window_ml_text_input;
    ml_view = (GX_MULTI_LINE_TEXT_VIEW *)ml_input;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_ON):
        gx_widget_show((GX_WIDGET *)&pMainScreen->window_input_scroll);
        break;

    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_OFF):
        gx_widget_hide((GX_WIDGET *)&pMainScreen->window_input_scroll);
        break;

    case GX_SIGNAL(ID_CURSOR_BLINK, GX_EVENT_TOGGLE_ON):
        gx_multi_line_text_input_style_add(ml_input, GX_STYLE_CURSOR_BLINK);
        break;

    case GX_SIGNAL(ID_CURSOR_BLINK, GX_EVENT_TOGGLE_OFF):
        gx_multi_line_text_input_style_remove(ml_input, GX_STYLE_CURSOR_BLINK);
        break;

    case GX_SIGNAL(ID_CURSOR_ALWAYS, GX_EVENT_TOGGLE_ON):
        gx_multi_line_text_input_style_add(ml_input, GX_STYLE_CURSOR_ALWAYS);
        break;

    case GX_SIGNAL(ID_CURSOR_ALWAYS, GX_EVENT_TOGGLE_OFF):
        gx_multi_line_text_input_style_remove(ml_input, GX_STYLE_CURSOR_ALWAYS);
        break;

    case GX_SIGNAL(ID_FONT_1BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 0;
        gx_multi_line_text_view_font_set(ml_view, font[font_depth][font_size]);
        break;

    case GX_SIGNAL(ID_FONT_4BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 1;
        gx_multi_line_text_view_font_set(ml_view, font[font_depth][font_size]);
        break;

    case GX_SIGNAL(ID_FONT_8BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 2;
        gx_multi_line_text_view_font_set(ml_view, font[font_depth][font_size]);
        break;

    case GX_SIGNAL(ID_SIZE_18PIX, GX_EVENT_RADIO_SELECT):
        font_size = 0;
        gx_multi_line_text_view_font_set(ml_view, font[font_depth][font_size]);
        break;

    case GX_SIGNAL(ID_SIZE_24PIX, GX_EVENT_RADIO_SELECT):
        font_size = 1;
        gx_multi_line_text_view_font_set(ml_view, font[font_depth][font_size]);
        break;

    case GX_SIGNAL(ID_CURSOR_WIDTH_INPUT, GX_EVENT_TEXT_EDITED):
        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(&pMainScreen->window_cursor_width_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);

        if(value > 0)
        {
            gx_text_input_cursor_width_set(&ml_input->gx_multi_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)ml_input);
        }
        break;

    case GX_SIGNAL(ID_CURSOR_HEIGHT_INPUT, GX_EVENT_TEXT_EDITED):
        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(&pMainScreen->window_cursor_height_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);

        if(value > 0)
        {
            gx_text_input_cursor_height_set(&ml_input->gx_multi_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)ml_input);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

