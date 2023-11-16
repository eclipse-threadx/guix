/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_no_output", /* Test name */
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

static VOID *test_memory_invalid_allocate(ULONG size)
{
    return NULL;
}

static VOID *test_memory_allocate(ULONG size)
{
    return malloc(size);
}

static VOID test_memory_free(VOID *mem)
{
}

static VOID control_thread_entry(ULONG input)
{
int        failed_tests = 0;
UINT       status;
GX_SINGLE_LINE_TEXT_INPUT text_input;
GX_RECTANGLE size;
GX_EVENT my_event;

    memset(&text_input, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);

    status = gx_single_line_text_input_create(&text_input, "test sl input", GX_NULL,
                                             GX_NULL, 100, 0, 0, &size);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    memset(&text_input, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    gx_system_memory_allocator_set(test_memory_invalid_allocate, test_memory_free);
    status = gx_single_line_text_input_create(&text_input, "test sl input", GX_NULL,
                                             GX_NULL, 100, 0, 0, &size);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    memset(&text_input, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    gx_system_memory_allocator_set(test_memory_allocate, GX_NULL);
    status = gx_single_line_text_input_create(&text_input, "test sl input", GX_NULL,
                                              GX_NULL, 100, 0, 0, &size);

    EXPECT_EQ(GX_SUCCESS, status);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_DELETE;
    status = gx_single_line_text_input_event_process(&text_input, &my_event);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    memset(&text_input, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    status = gx_single_line_text_input_create(&text_input, "test sl input", GX_NULL,
                                              GX_NULL, 0, 0, 0, &size);

    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_single_line_text_input_event_process(&text_input, &my_event);
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
