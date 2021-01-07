/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_simple_resources.h"
#include "guix_simple_specifications.h"

GX_WINDOW *pHelloScreen;

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}


VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);


    /* create the hello world screen */
    gx_studio_named_widget_create("simple_window", (GX_WIDGET *) root, (GX_WIDGET **) &pHelloScreen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}


