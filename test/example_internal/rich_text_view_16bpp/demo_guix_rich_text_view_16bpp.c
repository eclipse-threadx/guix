/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "rich_text_view_16bpp_resources.h"
#include "rich_text_view_16bpp_specifications.h"

extern MAIN_SCREEN_CONTROL_BLOCK main_screen;

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

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT* event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SCROLLBAR_CHECK, GX_EVENT_TOGGLE_ON):
        /* Show text view scrollbar. */
        gx_widget_show((GX_WIDGET*)&main_screen.main_screen_vertical_scroll);
        break;

    case GX_SIGNAL(ID_SCROLLBAR_CHECK, GX_EVENT_TOGGLE_OFF):
        /* Hide text view scrollbar. */
        gx_widget_hide((GX_WIDGET*)&main_screen.main_screen_vertical_scroll);
        break;

    case GX_SIGNAL(ID_WHITESPACE_SLIDER, GX_EVENT_SLIDER_VALUE):
        /* Set whitespace value. */
        gx_numeric_prompt_value_set(&main_screen.main_screen_whitespace_value, (GX_UBYTE)event_ptr->gx_event_payload.gx_event_longdata);

        /* Set rich text view whitespace. */
        gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)&main_screen.main_screen_rich_text_view,
                                               (GX_UBYTE)event_ptr->gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_LINE_SPACE_SLIDER, GX_EVENT_SLIDER_VALUE):
        /* Set line space value. */
        gx_numeric_prompt_value_set(&main_screen.main_screen_line_space_value, (GX_UBYTE)event_ptr->gx_event_payload.gx_event_longdata);

        /* Set rich tet view line space. */
        gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW*)&main_screen.main_screen_rich_text_view,
                                               (GX_BYTE)event_ptr->gx_event_payload.gx_event_longdata);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return GX_SUCCESS;
}
