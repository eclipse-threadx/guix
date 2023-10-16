/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_scroll_wheel.h"

TEST_PARAM test_parameter = {
    "guix_generic_scroll_wheel_no_output", /* Test name */
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
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_generic_scroll_wheel.c"

static VOID control_thread_entry(ULONG input)
{
int       failed_tests = 0;
UINT      status;
GX_GENERIC_SCROLL_WHEEL wheel;
GX_RECTANGLE size;
GX_WINDOW parent;
GX_EVENT myevent;
GX_GENERIC_SCROLL_WHEEL *wheel_wrap = &main_screen.main_screen_scroll_wheel_wrap;

    status = gx_generic_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(status, GX_PTR_ERROR);

    memset(&wheel, 0, sizeof(GX_GENERIC_SCROLL_WHEEL));

    status = gx_generic_scroll_wheel_children_position(&wheel);
    EXPECT_EQ(status, GX_INVALID_WIDGET);

    status = gx_generic_scroll_wheel_event_process(&wheel, &myevent);
    EXPECT_EQ(status, GX_INVALID_WIDGET);

    status = gx_generic_scroll_wheel_row_height_set(&wheel, 50);
    EXPECT_EQ(status, GX_INVALID_WIDGET);

    status = gx_generic_scroll_wheel_total_rows_set(&wheel, 50);
    EXPECT_EQ(status, GX_INVALID_WIDGET);

    status = gx_generic_scroll_wheel_create(&wheel, "", GX_NULL, 10, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(status, GX_PTR_ERROR);

    /* Invalid total rows.  */
    status = gx_generic_scroll_wheel_create(&wheel, "", GX_NULL, 0, GX_NULL, 0, 0, &size);
    EXPECT_EQ(status, GX_INVALID_VALUE);

    memset(&parent, 0, sizeof(GX_WINDOW));

    /* Invalid parent widget.  */
    status = gx_generic_scroll_wheel_create(&wheel, "", &parent, 10, GX_NULL, 0, 0, &size);
    EXPECT_EQ(status, GX_INVALID_WIDGET);

    /* Invalid control block size.  */
    status = _gxe_generic_scroll_wheel_create(&wheel, "", GX_NULL, 10, GX_NULL, 0 , 0, &size, 0);
    EXPECT_EQ(status, GX_INVALID_SIZE);

    status = gx_generic_scroll_wheel_create(&wheel, "", GX_NULL, 10, GX_NULL, 0, 0, &size);
    EXPECT_EQ(status, GX_SUCCESS);

    status = gx_generic_scroll_wheel_create(&wheel, "", GX_NULL, 10, GX_NULL, 0, 0, &size);
    EXPECT_EQ(status, GX_ALREADY_CREATED);

    status = gx_generic_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(status, GX_PTR_ERROR);

    status = gx_generic_scroll_wheel_event_process(&wheel, GX_NULL);
    EXPECT_EQ(status, GX_PTR_ERROR);

    memset(&myevent, 0, sizeof(GX_EVENT));
    myevent.gx_event_type = GX_EVENT_SHOW;
    status = gx_generic_scroll_wheel_event_process(&wheel, &myevent);
    EXPECT_EQ(status, GX_SUCCESS);

    status = gx_generic_scroll_wheel_row_height_set(GX_NULL, 10);
    EXPECT_EQ(status, GX_PTR_ERROR);

    status = gx_generic_scroll_wheel_row_height_set(&wheel, 0);
    EXPECT_EQ(status, GX_INVALID_VALUE);

    status = gx_generic_scroll_wheel_total_rows_set(GX_NULL, 10);
    EXPECT_EQ(status, GX_PTR_ERROR);

    status = gx_generic_scroll_wheel_total_rows_set(&wheel, -1);
    EXPECT_EQ(status, GX_INVALID_VALUE);

    status = gx_generic_scroll_wheel_children_position(GX_NULL);
    EXPECT_EQ(status, GX_PTR_ERROR);
    
    status = gx_generic_scroll_wheel_total_rows_set(wheel_wrap, 0);
    EXPECT_EQ(status, GX_SUCCESS);

    wheel_wrap->gx_scroll_wheel_total_rows = 10;
    _gx_generic_scroll_wheel_scroll(wheel_wrap, 10);
    _gx_generic_scroll_wheel_scroll(wheel_wrap, -10);

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
