/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "ml_text_view_16bpp_resources.h"
#include "ml_text_view_16bpp_specifications.h"


extern MAIN_SCREEN_CONTROL_BLOCK main_screen;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       memory_pool;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_RESOURCE_ID font[3][2] = {{GX_FONT_ID_TEXT_INPUT_1BPP_18PIX, GX_FONT_ID_TEXT_INPUT_1BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_4BPP_18PIX, GX_FONT_ID_TEXT_INPUT_4BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_8BPP_18PIX, GX_FONT_ID_TEXT_INPUT_8BPP_24PIX}};
UINT    font_depth = 2;
UINT    font_size = 0;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

GX_CHAR line_space[3];
GX_CHAR whitespace[3];

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
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

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
GX_MULTI_LINE_TEXT_VIEW *ml_view;
INT                      slider_value;
GX_STRING                str;

    ml_view = &main_screen.main_screen_text_view;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_ON):

        gx_widget_show((GX_WIDGET *)&main_screen.main_screen_vertical_scroll_1);
        break;

    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_OFF):

        gx_widget_hide((GX_WIDGET *)&main_screen.main_screen_vertical_scroll_1);
        break;

    case GX_SIGNAL(ID_ASCII, GX_EVENT_RADIO_SELECT):

        gx_multi_line_text_view_text_id_set(ml_view, GX_STRING_ID_STRING_ASCII);
        break;

    case GX_SIGNAL(ID_UTF8, GX_EVENT_RADIO_SELECT):

        gx_multi_line_text_view_text_id_set(ml_view, GX_STRING_ID_STRING_UTF8);
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

        gx_widget_style_add((GX_WIDGET *)ml_view, GX_STYLE_TEXT_LEFT);
        break;

    case GX_SIGNAL(ID_ALIGN_LEFT, GX_EVENT_RADIO_DESELECT):

        gx_widget_style_remove((GX_WIDGET *)ml_view, GX_STYLE_TEXT_LEFT);
        break;

    case GX_SIGNAL(ID_ALIGN_CENTER, GX_EVENT_RADIO_SELECT):

        gx_widget_style_add((GX_WIDGET *)ml_view, GX_STYLE_TEXT_CENTER);
        break;

    case GX_SIGNAL(ID_ALIGN_CENTER, GX_EVENT_RADIO_DESELECT):

        gx_widget_style_remove((GX_WIDGET *)ml_view, GX_STYLE_TEXT_CENTER);
        break;

    case GX_SIGNAL(ID_ALIGN_RIGHT, GX_EVENT_RADIO_SELECT):

        gx_widget_style_add((GX_WIDGET *)ml_view, GX_STYLE_TEXT_RIGHT);
        break;

    case GX_SIGNAL(ID_ALIGN_RIGHT, GX_EVENT_RADIO_DESELECT):

        gx_widget_style_remove((GX_WIDGET *)ml_view, GX_STYLE_TEXT_RIGHT);
        break;

    case GX_SIGNAL(ID_SLIDER_WHITESPACE, GX_EVENT_SLIDER_VALUE):
        slider_value = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(slider_value, whitespace, 3);
        str.gx_string_ptr = whitespace;
        str.gx_string_length = 3;
        gx_prompt_text_set_ext(&main_screen.main_screen_prompt_whitespace, &str);
        gx_multi_line_text_view_whitespace_set(ml_view, slider_value);
        break;

    case GX_SIGNAL(ID_SLIDER_LINE_SPACE, GX_EVENT_SLIDER_VALUE):
        slider_value = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(slider_value, line_space, 3);
        str.gx_string_ptr = line_space;
        str.gx_string_length = 3;
        gx_prompt_text_set_ext(&main_screen.main_screen_prompt_line_space, &str);
        gx_multi_line_text_view_line_space_set(ml_view, slider_value);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

