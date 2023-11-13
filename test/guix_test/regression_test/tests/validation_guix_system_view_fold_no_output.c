/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_view_fold_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
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

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"



/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_WINDOW window;
GX_VIEW   old_view;
GX_RECTANGLE size;
GX_RECTANGLE new_view;

    memset(&window, 0, sizeof(GX_WINDOW));
    size.gx_rectangle_left = 100;
    size.gx_rectangle_top = 0;
    size.gx_rectangle_right = 200;
    size.gx_rectangle_bottom = 400;
    gx_window_create(&window, "window", GX_NULL, 0, 0, &size);
    window.gx_window_views = &old_view;
    old_view.gx_view_rectangle = size;
    old_view.gx_view_next = GX_NULL;
    
    new_view.gx_rectangle_left = 201;
    new_view.gx_rectangle_top = 0;
    new_view.gx_rectangle_right = 400;
    new_view.gx_rectangle_bottom = 400;
    _gx_system_view_fold(&window, &new_view);
    
    new_view.gx_rectangle_left = 0;
    new_view.gx_rectangle_top = 0;
    new_view.gx_rectangle_right = 99;
    new_view.gx_rectangle_bottom = 400;
    _gx_system_view_fold(&window, &new_view);
    
    gx_validation_print_test_result(TEST_SUCCESS);

    exit(0);
}
