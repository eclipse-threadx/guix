/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "synergy_565rgb_dave2d_disabled_resources.h"
#include "synergy_565rgb_dave2d_disabled_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);


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


    gx_studio_display_configure(DISPLAY_1, _gx_synergy_display_driver_setup,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}