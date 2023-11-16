/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_gx_system_string_width_get_no_output", /* Test name */
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
int failed_tests = 0;
GX_FONT *font = GX_NULL;
GX_VALUE width;
UINT     status;

    gx_widget_font_get(root, GX_FONT_ID_NORMAL_FONT, &font);
    if(font == GX_NULL)
    {
        PRINT_ERROR("Retrieved font should be NULL.");
        failed_tests++;
    }  
    
#ifndef GX_DISABLE_ERROR_CHECKING
    status = gx_system_string_width_get(font, "string", GX_STRLEN("string") + 1, &width);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);
#endif

    status = gx_system_string_width_get(font, "string", GX_STRLEN("string"), &width);
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
