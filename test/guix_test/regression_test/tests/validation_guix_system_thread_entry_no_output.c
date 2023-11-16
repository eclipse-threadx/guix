/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "tx_thread.h"

TEST_PARAM test_parameter = {
    "guix_system_thread_entry_no_output", /* Test name */
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

GX_BOOL broken_tx_thread = GX_FALSE;

VOID test_button_screen_draw(GX_WINDOW *window)
{
    gx_window_draw(window);

    if(broken_tx_thread)
    {
        _tx_thread_preempt_disable = 1;
        broken_tx_thread = GX_FALSE;
    }
}

VOID  _gx_system_error_process(UINT error_code)
{

    /* Increment the total number of system errors.  */
    _gx_system_error_count++;

    /* Remember the last system error code.  */
    _gx_system_last_error =  error_code;

    _tx_thread_preempt_disable = 0;
}

static VOID control_thread_entry(ULONG input)
{
GX_BOOL                  test_successed = GX_TRUE;

    gx_widget_draw_set(&button_screen, test_button_screen_draw);

    broken_tx_thread = GX_TRUE;
    gx_validation_screen_refresh();
    if(_gx_system_last_error != GX_SYSTEM_EVENT_RECEIVE_ERROR)
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
