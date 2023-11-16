/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "tx_thread.h"

TEST_PARAM test_parameter = {
    "guix_system_timer_update_no_output", /* Test name */
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

enum timer_ids{
TEST_TIMER_1 = 1024,
TEST_TIMER_2,
TEST_TIMER_3,
TEST_TIMER_4,
TEST_TIMER_5,
TEST_TIMER_6,
TEST_TIMER_7
};

static VOID control_thread_entry(ULONG input)
{
GX_BOOL                  test_successed = GX_TRUE;
GX_WIDGET *button_1 = (GX_WIDGET *)&button_screen.button_screen_base_button_1;
GX_WIDGET *button_2 = (GX_WIDGET *)&button_screen.button_screen_text_button_1;
GX_WIDGET *button_3 = (GX_WIDGET *)&button_screen.button_screen_multi_line_button_1;

    gx_system_timer_start(button_1, TEST_TIMER_1, 2, 2);
    gx_system_timer_start(button_2, TEST_TIMER_2, 2, 2);
    gx_system_timer_start(button_2, TEST_TIMER_3, 2, 2);
    gx_system_timer_start(button_2, TEST_TIMER_4, 2, 2);
    gx_system_timer_start(button_3, TEST_TIMER_5, 2, 2);
    gx_system_timer_start(button_1, TEST_TIMER_6, 2, 2);
    gx_system_timer_start(button_1, TEST_TIMER_7, 2, 3);

    gx_system_timer_stop(button_2, 0);
    gx_system_timer_stop(button_1, 0);

    /* Make sure timer is the one owned by button_3, and list is correctly null terminated. */
    if(_gx_system_active_timer_list == GX_NULL ||
       _gx_system_active_timer_list->gx_timer_owner != button_3 ||
       _gx_system_active_timer_list->gx_timer_next != GX_NULL)
    {
        test_successed = GX_FALSE;
    }

    gx_system_timer_stop(button_3, 0);
    button_screen.gx_widget_event_process_function = GX_NULL;

    gx_system_timer_start(&button_screen, TEST_TIMER_1, 2, 2);
    _gx_system_active_timer_list->gx_timer_owner = GX_NULL;
    gx_system_timer_start(&button_screen, TEST_TIMER_2, 2, 2);
    gx_system_timer_start(&button_screen, TEST_TIMER_3, 2, 2);
  
    tx_thread_sleep(10);

    gx_system_timer_stop(&button_screen, 0);

    if(_gx_system_active_timer_list->gx_timer_id != TEST_TIMER_1 ||
       _gx_system_active_timer_list->gx_timer_next != GX_NULL)
    {
        test_successed = GX_FALSE;
    }

    if(test_successed)
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
