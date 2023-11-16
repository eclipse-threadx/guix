/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "ml_text_input_16bpp_resources.h"
#include "ml_text_input_16bpp_specifications.h"

WINDOW_CONTROL_BLOCK *pMainScreen;

/* Define the ThreadX demo thread control block and stack.  */

GX_BOOL APP_CTRL_FLAG = GX_FALSE;
GX_BOOL APP_SHIFT_FLAG = GX_FALSE;

TX_BYTE_POOL       memory_pool;

#define SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

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

GX_CHAR line_space[3];
GX_CHAR whitespace[3];

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));


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

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
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
GX_MULTI_LINE_TEXT_INPUT *ml_input;
GX_MULTI_LINE_TEXT_VIEW *ml_view;
GX_SINGLE_LINE_TEXT_INPUT *cursor_width_input;
GX_SINGLE_LINE_TEXT_INPUT *cursor_height_input;
GX_CHAR  *input_buffer;
UINT     string_size;
INT      value;
GX_STRING str;

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

    case GX_SIGNAL(ID_ALIGN_LEFT, GX_EVENT_RADIO_SELECT):
        gx_widget_style_add((GX_WIDGET *)ml_input, GX_STYLE_TEXT_LEFT);
        break;

    case GX_SIGNAL(ID_ALIGN_LEFT, GX_EVENT_RADIO_DESELECT):
        gx_widget_style_remove((GX_WIDGET *)ml_input, GX_STYLE_TEXT_LEFT);
        break;

    case GX_SIGNAL(ID_ALIGN_CENTER, GX_EVENT_RADIO_SELECT):
        gx_widget_style_add((GX_WIDGET *)ml_input, GX_STYLE_TEXT_CENTER);
        break;

    case GX_SIGNAL(ID_ALIGN_CENTER, GX_EVENT_RADIO_DESELECT):
        gx_widget_style_remove((GX_WIDGET *)ml_input, GX_STYLE_TEXT_CENTER);
        break;

    case GX_SIGNAL(ID_ALIGN_RIGHT, GX_EVENT_RADIO_SELECT):
        gx_widget_style_add((GX_WIDGET *)ml_input, GX_STYLE_TEXT_RIGHT);
        break;

    case GX_SIGNAL(ID_ALIGN_RIGHT, GX_EVENT_RADIO_DESELECT):
        gx_widget_style_remove((GX_WIDGET *)ml_input, GX_STYLE_TEXT_RIGHT);
        break;

    case GX_SIGNAL(ID_CURSOR_WIDTH_INPUT, GX_EVENT_TEXT_EDITED):
        cursor_width_input = &pMainScreen->window_cursor_width_input;

        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(cursor_width_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);

        if(value > 0)
        {
            gx_text_input_cursor_width_set(&ml_input->gx_multi_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)ml_input);
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
            gx_text_input_cursor_height_set(&ml_input->gx_multi_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)ml_input);
        }
        break;

    case GX_SIGNAL(ID_SLIDER_WHITESPACE, GX_EVENT_SLIDER_VALUE):
        value = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(value, whitespace, 3);
        str.gx_string_ptr = whitespace;
        str.gx_string_length = strnlen(whitespace, 3);
        gx_prompt_text_set_ext(&pMainScreen->window_prompt_whitespace, &str);
        gx_multi_line_text_view_whitespace_set(ml_view, value);
        break;

    case GX_SIGNAL(ID_SLIDER_LINE_SPACE, GX_EVENT_SLIDER_VALUE):
        value = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(value, line_space, 3);
        str.gx_string_ptr = line_space;
        str.gx_string_length = strnlen(line_space, 3);
        gx_prompt_text_set_ext(&pMainScreen->window_prompt_line_space, &str);
        gx_multi_line_text_view_line_space_set(ml_view, value);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

