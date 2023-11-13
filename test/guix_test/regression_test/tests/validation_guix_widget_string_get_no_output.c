/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widget_string_get_no_output", /* Test name */
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
int       failed_tests = 0;
UINT      status;
GX_STRING string;

    /* Hide window screen. */
    gx_widget_hide((GX_WIDGET *)&window_screen);

    /* Test string get. */
    status = gx_widget_string_get_ext((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    status = gx_widget_string_get((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string.gx_string_ptr);
    EXPECT_EQ(GX_INVALID_CANVAS, status);
#endif

    /* Show window screen. */
    gx_widget_attach((GX_WIDGET *)root, (GX_WIDGET *)&window_screen);

    /* Test string get. */
    status = gx_widget_string_get_ext((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string);
    EXPECT_EQ(GX_SUCCESS, status);

    if(strcmp(string.gx_string_ptr, "item 1") != 0)
    {
        failed_tests++;
    }

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    status = gx_widget_string_get(&window_screen, GX_STRING_ID_ITEM_1, &string.gx_string_ptr);
    EXPECT_EQ(GX_SUCCESS, status);

    if(strcmp(string.gx_string_ptr, "item 1") != 0)
    {
        failed_tests++;
    }
#endif

    /* Remove GX_STATUS_VISIBLE style. */
    gx_widget_status_remove(&window_screen, GX_STATUS_VISIBLE);

    /* Test string get. */
    status = gx_widget_string_get_ext((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    status = gx_widget_string_get((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string.gx_string_ptr);
    EXPECT_EQ(GX_INVALID_CANVAS, status);
#endif

    /* Detach window screen, and add GX_STATUS_VISIBLE status. */
    gx_widget_detach(&window_screen);
    gx_widget_status_add(&window_screen, GX_STATUS_VISIBLE);
    
    /* Test string get. */
    status = gx_widget_string_get_ext((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    status = gx_widget_string_get((GX_WIDGET *)&window_screen, GX_STRING_ID_ITEM_1, &string.gx_string_ptr);
    EXPECT_EQ(GX_INVALID_CANVAS, status);
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
