/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_language_table_get_no_output", /* Test name */
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
INT         failed_tests = 0;
UINT        status;
GX_DISPLAY *display = GX_NULL;
GX_STRING **language_table = GX_NULL;
GX_UBYTE    language_count;
UINT        string_count;
    
    display = root->gx_window_root_canvas->gx_canvas_display;
#ifdef GX_ENABLE_DEPRECATED_STRING_API
    status = gx_display_language_table_get(display, 0, 0, 0);
    EXPECT_EQ(GX_SUCCESS, status);
#endif

    status = gx_display_language_table_get_ext(GX_NULL, &language_table, &language_count, &string_count);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_language_table_get_ext(display, &language_table, &language_count, &string_count);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(display->gx_display_language_table_size, language_count);
    EXPECT_EQ(display->gx_display_string_table_size, string_count);

    status = gx_display_language_table_get_ext(display, GX_NULL, GX_NULL, GX_NULL);
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
