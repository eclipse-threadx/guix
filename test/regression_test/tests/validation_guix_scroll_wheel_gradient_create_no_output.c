/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_scroll_wheel_gradient_create_no_output", /* Test name */
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
int                  failed_tests = 0;
UINT                 status;
GX_NUMERIC_SCROLL_WHEEL wheel;
GX_RECTANGLE size;
GX_EVENT myevent;

    memset(&wheel, 0, sizeof(GX_NUMERIC_SCROLL_WHEEL));
    gx_utility_rectangle_define(&size, 0, 0, 100, 400);

    status = gx_numeric_scroll_wheel_create(&wheel, "", GX_NULL, 1980, 2020, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)&wheel, 10, 100);
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&myevent, 0, sizeof(GX_EVENT));
    myevent.gx_event_type = GX_EVENT_SHOW;
    myevent.gx_event_target = (GX_WIDGET *)&wheel;
    gx_system_event_send(&myevent);

    EXPECT_EQ(wheel.gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_data, GX_NULL);
   
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
