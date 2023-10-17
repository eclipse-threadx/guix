/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "focus_management_resources.h"
#include "focus_management_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

VOID start_guix(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY* display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    gx_widget_status_add(&main_screen.main_screen_window, GX_STATUS_DEFAULT_FOCUS);
    gx_widget_status_add(&main_screen.main_screen_prompt_3, GX_STATUS_DEFAULT_FOCUS);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}