/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_scroll_wheel.h"

TEST_PARAM test_parameter = {
    "guix_string_scroll_wheel_create_no_output", /* Test name */
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

GX_STRING test_string_list_1[]=
{
    {"row 1", sizeof("row 1") - 1},
    {GX_NULL, 0},
    {"row 2", sizeof("row 2") - 2}  
};

GX_STRING test_string_list_2[]=
{
    {"row 1", sizeof("row 1")}
};

GX_STRING test_string_list_3[]=
{
    {"row 1", sizeof("row 1") - 1}
};

static VOID control_thread_entry(ULONG input)
{
int                    failed_tests = 0;
UINT                   status;
GX_STRING_SCROLL_WHEEL wheel;
GX_RECTANGLE           size;
GX_WIDGET              parent;

    status = gx_string_scroll_wheel_create_ext(GX_NULL, "", GX_NULL, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    status = gx_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    memset(&parent, 0, sizeof(GX_WIDGET));
    status = gx_string_scroll_wheel_create_ext(&wheel, "", &parent, 0, GX_NULL, 0, 0, &size);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = _gxe_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, 0, GX_NULL, 0, 0, &size, 0);
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, sizeof(test_string_list_1)/sizeof(GX_STRING), test_string_list_1, 0, 0, &size);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);
    
    status = gx_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, sizeof(test_string_list_2)/sizeof(GX_STRING), test_string_list_2, 0, 0, &size);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    status = gx_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, sizeof(test_string_list_3)/sizeof(GX_STRING), test_string_list_3, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_string_scroll_wheel_create_ext(&wheel, "", GX_NULL, 0, GX_NULL, 0, 0, &size);
    EXPECT_EQ(GX_ALREADY_CREATED, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    memset(&wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    status = gx_string_scroll_wheel_create(&wheel, "", (GX_WIDGET *)root, 0, GX_NULL, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

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
