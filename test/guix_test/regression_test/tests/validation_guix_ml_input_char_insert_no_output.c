/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_scroll_wheel.h"

TEST_PARAM test_parameter = {
    "guix_ml_input_char_insert_no_output", /* Test name */
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
int                      failed_tests = 0;
UINT                     status;
GX_STRING                string;
GX_CONST GX_CHAR         test_string[] = "test";
GX_MULTI_LINE_TEXT_INPUT ml_input;

    memset(&ml_input, 0, sizeof(GX_MULTI_LINE_TEXT_INPUT));

    status = gx_multi_line_text_input_char_insert_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_char_insert_ext(&ml_input, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&string, 0, sizeof(GX_STRING));
    status = gx_multi_line_text_input_char_insert_ext(&ml_input, &string);
    EXPECT_EQ(GX_PTR_ERROR, status);

    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 2;
    status = gx_multi_line_text_input_char_insert_ext(&ml_input, &string);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_length = sizeof(test_string);
    status = gx_multi_line_text_input_char_insert_ext(&ml_input, &string);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);
 
    string.gx_string_length = sizeof(test_string) - 1;
    status = gx_multi_line_text_input_char_insert_ext(&ml_input, &string);
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
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
