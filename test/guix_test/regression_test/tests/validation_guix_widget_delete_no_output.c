/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widget_delete_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

static VOID control_thread_entry(ULONG input)
{
    gx_widget_style_add(&menu_screen.menu_screen_ola_menu_1, GX_STYLE_MENU_EXPANDED);
    gx_accordion_menu_position(&menu_screen.menu_screen_one_level_accordion);

    /* Test if dynamic menu item is deleted. */
    gx_widget_delete(&menu_screen);

    /* Test if dynamic text input buffer is deleted. */
    gx_widget_delete(&text_screen);

    if(rotate_pool.tx_byte_pool_list == rotate_pool.tx_byte_pool_search)
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else
    {
        printf("memory leak in menu delete.");
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}

