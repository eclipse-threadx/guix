/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_view_no_output", /* Test name */
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
int        failed_tests = 0;
UINT       status;
GX_MULTI_LINE_TEXT_VIEW text_view;
GX_RECTANGLE size;
GX_EVENT my_event;
GX_STRING string;
GX_CHAR string_buffer[] = "test string";

    memset(&text_view, 0, sizeof(GX_MULTI_LINE_TEXT_VIEW));
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);

    status = gx_multi_line_text_view_create(&text_view, "test ml view", GX_NULL,
                                            0, GX_STYLE_TEXT_COPY, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    string.gx_string_ptr = string_buffer;
    string.gx_string_length = sizeof(string_buffer) - 1;
    status = gx_multi_line_text_view_text_set(&text_view, &string);
    EXPECT_EQ(GX_SUCCESS, status);

    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_DELETE;
    status = gx_multi_line_text_view_event_process(&text_view, &my_event);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

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
