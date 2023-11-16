/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "sl_text_input_8bpp_resources.h"
#include "sl_text_input_8bpp_specifications.h"

WINDOW_CONTROL_BLOCK *pMainScreen;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
GX_RESOURCE_ID font[3][2] = {{GX_FONT_ID_TEXT_INPUT_1BPP_18PIX, GX_FONT_ID_TEXT_INPUT_1BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_4BPP_18PIX, GX_FONT_ID_TEXT_INPUT_4BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_8BPP_18PIX, GX_FONT_ID_TEXT_INPUT_8BPP_24PIX}};

UINT    font_depth = 2;
UINT    font_size = 0;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);

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

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_8bit_palette, 
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

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_SINGLE_LINE_TEXT_INPUT *input;
GX_SINGLE_LINE_TEXT_INPUT *cursor_width_input;
GX_SINGLE_LINE_TEXT_INPUT *cursor_height_input;
GX_CHAR  *input_buffer;
UINT     string_size;
INT      value;

    input = &pMainScreen->window_sl_text_input;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CURSOR_BLINK, GX_EVENT_TOGGLE_ON):
        gx_single_line_text_input_style_add(input, GX_STYLE_CURSOR_BLINK);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_CURSOR_BLINK, GX_EVENT_TOGGLE_OFF):
        gx_single_line_text_input_style_remove(input, GX_STYLE_CURSOR_BLINK);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_CURSOR_ALWAYS, GX_EVENT_TOGGLE_ON):
        gx_single_line_text_input_style_add(input, GX_STYLE_CURSOR_ALWAYS);
        input->gx_widget_status |= GX_STATUS_CURSOR_SHOW;
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_CURSOR_ALWAYS, GX_EVENT_TOGGLE_OFF):
        gx_single_line_text_input_style_remove(input, GX_STYLE_CURSOR_ALWAYS);
        input->gx_widget_status &= ~ GX_STATUS_CURSOR_SHOW;
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_FONT_1BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 0;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_FONT_4BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 1;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_FONT_8BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 2;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_SIZE_18PIX, GX_EVENT_RADIO_SELECT):
        font_size = 0;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_SIZE_24PIX, GX_EVENT_RADIO_SELECT):
        font_size = 1;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_NO_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_NONE);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_THIN_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_THIN);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_RECESSED_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_RECESSED);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_RAISED_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_RAISED);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_THICK_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_THICK);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_CURSOR_WIDTH_INPUT, GX_EVENT_TEXT_EDITED):
        cursor_width_input = &pMainScreen->window_cursor_width_input;

        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(cursor_width_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);
        if(value > 0)
        {
            gx_text_input_cursor_width_set(&input->gx_single_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)input);
        }
        break;
    case GX_SIGNAL(ID_CURSOR_HEIGHT_INPUT, GX_EVENT_TEXT_EDITED):
        cursor_height_input = &pMainScreen->window_cursor_height_input;

        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(cursor_height_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);

        if(value > 0)
        {
            gx_text_input_cursor_height_set(&input->gx_single_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)input);
        }
        break;
    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

