/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_line_text_view_font_24xrgb", /* Test name */
    64, 50, 243, 189  /* Define the coordinates of the capture area.
                      In this test, we only need to capture the 
                      multi_line_text_view window
                      drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command canvas_text_view argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
    on the canvas_text_view drawing example. These operations 
    simulate user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual canvas_text_view routine. */
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

#include "demo_guix_multi_line_text_view_font_32bpp.c"

static char text[] = "0123456789 abcdefghijklmnopqrstuvwxyz";

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    gx_multi_line_text_view_text_set(&pMainWin -> main_window_text_view, text);

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "small font");
    gx_validation_set_frame_comment(test_comment);
    gx_multi_line_text_view_font_set(&pMainWin -> main_window_text_view, 4);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(pMainWin);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Increment frame ID */
    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "large font");
    gx_validation_set_frame_comment(test_comment);
    gx_multi_line_text_view_font_set(&pMainWin -> main_window_text_view, 5);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(pMainWin);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Increment frame ID */
    frame_id ++;

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
