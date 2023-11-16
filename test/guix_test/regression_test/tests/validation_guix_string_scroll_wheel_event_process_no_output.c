/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_string_scroll_wheel_event_process_no_output", /* Test name */
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

#define STRING_COUNT 3

GX_CONST GX_CHAR row_1[] = "row 1";
GX_CONST GX_CHAR row_2[] = "row 2";
GX_CONST GX_CHAR row_3[] = "row 3";

GX_STRING test_string_list[STRING_COUNT]=
{
    {row_1, sizeof(row_1) - 1},
    {row_2, sizeof(row_2) - 1},
    {row_3, sizeof(row_3) - 1}
};

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
GX_CONST GX_CHAR *test_deprecated_string_list[STRING_COUNT]=
{
    row_1,
    row_2,
    row_3
};
#endif

static VOID control_thread_entry(ULONG input)
{
int                    failed_tests = 0;
UINT                   status;
GX_STRING_SCROLL_WHEEL wheel;
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
GX_STRING_SCROLL_WHEEL wheel_2;
#endif
GX_RECTANGLE           size;
GX_WIDGET              parent;
GX_EVENT               my_event;

    gx_utility_rectangle_define(&size, 0, 0, 100, 100);

    status = gx_string_scroll_wheel_event_process(GX_NULL, &my_event);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_string_scroll_wheel_event_process(&wheel, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));

    status = gx_string_scroll_wheel_event_process(&wheel, &my_event);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, 0, GX_NULL, GX_STYLE_TEXT_COPY, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_string_list, STRING_COUNT);
    EXPECT_EQ(GX_SUCCESS, status);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    memset(&wheel_2, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    status = gx_string_scroll_wheel_create(&wheel_2, "", GX_NULL, 0, GX_NULL, GX_STYLE_TEXT_COPY, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_string_scroll_wheel_string_list_set(&wheel_2, test_deprecated_string_list, STRING_COUNT);
    EXPECT_EQ(GX_SUCCESS, status);
#endif
    
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);

    my_event.gx_event_type = GX_EVENT_DELETE;
    status = gx_string_scroll_wheel_event_process(&wheel, &my_event);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    status = gx_string_scroll_wheel_event_process(&wheel_2, &my_event);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
#endif

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
