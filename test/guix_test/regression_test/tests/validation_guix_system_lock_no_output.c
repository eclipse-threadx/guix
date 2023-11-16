/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_lock_no_output", /* Test name */
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

#define ENTER_CRITICAL GX_FIRST_USER_EVENT
#define EXIT_CRITICAL  GX_FIRST_USER_EVENT + 1
UINT test_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case ENTER_CRITICAL:
       GX_ENTER_CRITICAL
       break;

    case EXIT_CRITICAL:
       GX_EXIT_CRITICAL
       break;

    default:
       return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

static VOID control_thread_entry(ULONG input)
{
GX_BOOL                  test_successed = GX_TRUE;
GX_EVENT                 my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    gx_widget_event_process_set(&button_screen, test_event_handler);

    _gx_system_last_error = 0;
    GX_ENTER_CRITICAL

    my_event.gx_event_type = ENTER_CRITICAL;
    my_event.gx_event_target = (GX_WIDGET *)&button_screen;
    gx_system_event_send(&my_event);

    tx_mutex_delete(&_gx_system_protect);
    memset(&_gx_system_protect, 0, sizeof(TX_MUTEX));

    if(_gx_system_last_error != GX_SYSTEM_PROTECTION_ERROR)
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
