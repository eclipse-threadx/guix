/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_widget_text_blend_no_output", /* Test name */
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
#include "demo_guix_all_widgets.c"

static VOID control_thread_entry(ULONG input)
{
int        failed_tests = 0;
UINT       status;
GX_BUTTON *button = &button_screen.button_screen_base_button_1;
#ifdef GX_ENABLE_DEPRECATED_STRING_API
GX_WIDGET  widget;
GX_CHAR    invalid_string[GX_MAX_STRING_LENGTH + 1];

    memset(&widget, 0, sizeof(GX_WIDGET));
    status = gx_widget_text_blend(&widget, 0, 0, GX_NULL, 0, 0, 128);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(invalid_string, 'x', GX_MAX_STRING_LENGTH + 1);
    status = gx_widget_text_blend(button, 0, 0, invalid_string, 0, 0, 128);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);
#endif

    status = gx_widget_text_blend_ext(button, 0, 0, GX_NULL, 0, 0, 128);
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
