/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_tree_view_show_event_process", /* Test name */
    460, 100, 628, 355  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
INT           frame_id = 1;
GX_EVENT      my_event;
GX_TREE_VIEW  test_tree;
GX_BUTTON     button;
GX_MENU       menu;
GX_RECTANGLE  size;

    /* Toggle from menu screen to text screen. */
    ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);

    gx_utility_rectangle_define(&size, 486, 247, 565, 270);

    memset(&test_tree, 0, sizeof(GX_TREE_VIEW));
    gx_tree_view_create(&test_tree, "test_tree", GX_NULL, GX_STYLE_ENABLED, 1024, &size);
    gx_menu_insert(&menu_screen.menu_screen_tree_menu_1, (GX_WIDGET *)&test_tree);

    memset(&button, 0, sizeof(GX_BUTTON));
    gx_button_create(&button, "test_button", &test_tree, GX_STYLE_ENABLED, 2014, &size);

    memset(&menu, 0, sizeof(GX_MENU));
    gx_menu_create(&menu, "test_menu", &test_tree, 0, 0, GX_STYLE_ENABLED, 1026, &size);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)&test_tree;
    my_event.gx_event_type = GX_EVENT_SHOW;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("show test tree that is attached to menu_1");
    gx_validation_screen_refresh();

    gx_widget_detach(&test_tree);
    gx_system_event_send(&my_event);

    tx_thread_sleep(2);

    gx_validation_end();

    exit(0);
}

