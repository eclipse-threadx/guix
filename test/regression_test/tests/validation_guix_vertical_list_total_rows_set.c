/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_total_rows_set", /* Test name */
   70, 85, 440, 330  /* Define the coordinates of the capture area.
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

#include "demo_guix_vertical_list_32bpp.c"

char test_comment[256];
int  frame_id = 1;

static void set_total_rows(int rows)
{
    gx_vertical_list_total_rows_set(&pMainScreen->window_vertical_list_scroll, rows);
    gx_vertical_list_total_rows_set(&pMainScreen->window_vertical_list_wrap, rows);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set total rows to %d", rows);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
}

static void select_row(int row)
{
    gx_vertical_list_selected_set(&pMainScreen->window_vertical_list_scroll, row);
    gx_vertical_list_selected_set(&pMainScreen->window_vertical_list_wrap, row);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Select row %d", row);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
    set_total_rows(5);
    set_total_rows(20);
    select_row(19);
    set_total_rows(3);
    set_total_rows(25);
    select_row(24);
    set_total_rows(30);
    set_total_rows(0);
    set_total_rows(10);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
