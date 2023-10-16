/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_widget_scroll_shift", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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

#include "demo_widget_find.c"

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    _gx_widget_scroll_shift((GX_WIDGET *)&main_screen.main_screen_window_1, 100, 100, GX_FALSE);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("xshift = 100, yshift = 100, clip = GX_FALSE");
    gx_validation_screen_refresh();

    _gx_widget_scroll_shift((GX_WIDGET *)&main_screen.main_screen_window_1, 100, 100, GX_TRUE);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("xshift = 100, yshift = 100, clip = GX_TRUE");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
