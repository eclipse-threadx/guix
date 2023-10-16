/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_total_columns_set", /* Test name */
   50, 50, 290, 310  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
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
       on the pixelmap drawing example. These operations simulate 
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

#include "demo_guix_horizontal_list_32bpp.c"

char test_comment[256];
int  frame_id = 1;

static void set_total_cols(int cols)
{
    gx_horizontal_list_total_columns_set(&pMainScreen->window_horizontal_list_scroll, cols);
    gx_horizontal_list_total_columns_set(&pMainScreen->window_horizontal_list_wrap, cols);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set total cols to %d", cols);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
}

static void select_col(int col)
{
    gx_horizontal_list_selected_set(&pMainScreen->window_horizontal_list_scroll, col);
    gx_horizontal_list_selected_set(&pMainScreen->window_horizontal_list_wrap, col);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Select col %d", col);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

    set_total_cols(5);
    set_total_cols(20);
    select_col(19);
    set_total_cols(3);
    set_total_cols(25);
    select_col(24);
    set_total_cols(30);
    set_total_cols(0);
    set_total_cols(10);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
