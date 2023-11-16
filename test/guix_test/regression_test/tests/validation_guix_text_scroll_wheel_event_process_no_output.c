/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_text_scroll_wheel_event_process_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area. */
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
    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static VOID control_thread_entry(ULONG input)
{
INT failed_tests = 0;
UINT status;
GX_TEXT_SCROLL_WHEEL wheel;
GX_EVENT my_event;
GX_RECTANGLE size;

    status = gx_text_scroll_wheel_event_process(GX_NULL, &my_event);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_text_scroll_wheel_event_process(&wheel, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&wheel, 0, sizeof(GX_TEXT_SCROLL_WHEEL));
    status = gx_text_scroll_wheel_event_process(&wheel, &my_event);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    status = gx_text_scroll_wheel_create(&wheel, "", GX_NULL, 10, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_DELETE;
    status = gx_text_scroll_wheel_event_process(&wheel, &my_event);
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
