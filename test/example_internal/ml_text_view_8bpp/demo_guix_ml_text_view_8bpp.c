/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "ml_text_view_8bpp_resources.h"
#include "ml_text_view_8bpp_specifications.h"

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
GX_MULTI_LINE_TEXT_VIEW *ml_view;

    ml_view = &pMainScreen->window_text_view;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_ON):
        gx_widget_show((GX_WIDGET *)&pMainScreen->window_vertical_scroll_1);
        ml_view->gx_multi_line_text_view_line_index_old = GX_TRUE;;
        gx_system_dirty_mark((GX_WIDGET *)ml_view);
        break;
    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_OFF):
        gx_widget_hide((GX_WIDGET *)&pMainScreen->window_vertical_scroll_1);
        ml_view->gx_multi_line_text_view_line_index_old = GX_TRUE;;
        gx_system_dirty_mark((GX_WIDGET *)ml_view);
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
    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

