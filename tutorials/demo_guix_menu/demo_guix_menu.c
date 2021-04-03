/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_menu_resources.h"
#include "demo_guix_menu_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

#define SYSTEM_SCREEN_STACK_SIZE  20

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
GX_WINDOW_ROOT    *root;
GX_WIDGET         *system_screen_stack_memory[SYSTEM_SCREEN_STACK_SIZE];

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */
    start_guix();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Configure display. */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* Create main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Create screen a. */
    gx_studio_named_widget_create("screen_a", GX_NULL, GX_NULL);

    /* Create screen b. */
    gx_studio_named_widget_create("screen_b", GX_NULL, GX_NULL);

    /* Create setting_screen. */
    gx_studio_named_widget_create("setting_screen", GX_NULL, GX_NULL);

    /* Create system screen stack that needed for studio auto animation. */
    gx_system_screen_stack_create(system_screen_stack_memory, sizeof(GX_WIDGET *)* SYSTEM_SCREEN_STACK_SIZE);

    /* Show the root window to make it and child screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}