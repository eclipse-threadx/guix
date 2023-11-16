/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "ml_text_view_vcenter_resources.h"
#include "ml_text_view_vcenter_specifications.h"


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


GX_STUDIO_DISPLAY_INFO display_info;

VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

