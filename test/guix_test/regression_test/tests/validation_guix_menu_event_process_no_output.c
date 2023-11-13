/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_scroll_wheel.h"

TEST_PARAM test_parameter = {
    "guix_menu_event_process_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
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

static VOID control_thread_entry(ULONG input)
{
int      failed_tests = 0;
UINT     status;
GX_MENU  menu;
GX_EVENT myevent;
GX_RECTANGLE size;

    status = gx_menu_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_menu_event_process(&menu, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    memset(&menu, 0, sizeof(GX_MENU));
    status = gx_menu_event_process(&menu, &myevent);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    status = gx_menu_create(&menu, "menu", GX_NULL, 0, 0, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    myevent.gx_event_type = GX_EVENT_DELETE;
    status = gx_menu_event_process(&menu, &myevent);
    EXPECT_EQ(GX_SUCCESS, status);
    
    if(failed_tests == 0) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
