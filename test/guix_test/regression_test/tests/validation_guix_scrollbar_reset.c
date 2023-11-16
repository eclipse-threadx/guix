/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_scrollbar_reset", /* Test name */
    351, 45, 562, 180  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

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

char test_comment[255];

static GX_SCROLL_INFO scroll_info = {5, 0, 10, 1, 1};

static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
GX_SCROLLBAR *hscroll;

    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, (GX_WIDGET *)&window_screen);
    hscroll = &window_screen.window_screen_hscroll_1;
    gx_validation_set_frame_id(frame_id++); 
    gx_validation_set_frame_comment("Original scollbar in window_screen.");
    /* Force a screen refresh. */
    gx_validation_screen_refresh();    /* Signal the end of the test case.  Verify the output. */
    
    gx_scrollbar_reset(hscroll, &scroll_info);
    gx_validation_set_frame_id(frame_id++); 
    sprintf(test_comment, "Set scroll info: gx_scroll_value: %d, gx_scroll_minimum: %d, gx_scroll_maximum: %d, gx_scroll_visible: %d, gx_scroll_increment: %d ",
                                                     scroll_info.gx_scroll_value, scroll_info.gx_scroll_minimum,scroll_info.gx_scroll_maximum,scroll_info.gx_scroll_visible,scroll_info.gx_scroll_increment);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();    /* Signal the end of the test case.  Verify the output. */
    
    gx_scrollbar_reset(hscroll, GX_NULL);
    gx_validation_set_frame_id(frame_id++); 
    gx_validation_set_frame_comment("Call function gx_scrollbar_reset, and the scroll_info parameter is null.");
    /* Force a screen refresh. */
    gx_validation_screen_refresh();    /* Signal the end of the test case.  Verify the output. */
    
    gx_validation_end();

    exit(0);
}
