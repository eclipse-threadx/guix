/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_widget_border_draw", /* Test name */
    117, 100, 253, 170  /* Define the coordinates of the capture area.
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
GX_CHAR test_comment[256];

void test_button_screen_draw(GX_WINDOW *window)
{
    gx_window_background_draw(window);
    gx_button_draw(&button_screen.button_screen_base_button_1);
}
static VOID control_thread_entry(ULONG input)
{
int        frame_id = 1;
GX_WIDGET *button = (GX_WIDGET *)&button_screen.button_screen_base_button_1;

    gx_widget_draw_set(&button_screen, test_button_screen_draw);

    gx_widget_detach(button);
    sprintf(test_comment, "detach base_button from its parent, and draw base_button in button_screen");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    _gx_widget_back_link(GX_NULL, GX_NULL);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
