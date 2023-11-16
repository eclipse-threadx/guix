/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "multi_line_text_view_font_16bpp_resources.h"
#include "multi_line_text_view_font_16bpp_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);


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


VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_MULTI_LINE_TEXT_VIEW *text_view;

    text_view = &pMainWin->main_window_text_view;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTON_1BPP, GX_EVENT_CLICKED):
        gx_multi_line_text_view_font_set(text_view, GX_FONT_ID_MIDDLE_1BPP);
        break;

    case GX_SIGNAL(ID_BUTTON_4BPP, GX_EVENT_CLICKED):
        gx_multi_line_text_view_font_set(text_view, GX_FONT_ID_LARGE_4BPP);
        break;

    case GX_SIGNAL(ID_BUTTON_8BPP, GX_EVENT_CLICKED):
        gx_multi_line_text_view_font_set(text_view, GX_FONT_ID_SMALL_8BPP);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}
