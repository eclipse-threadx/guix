/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_event_dispatch_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area. */
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

#define TEST_LIST_ID  1024
#define TEST_CHILD_ID 1025

int event_count = 0;
UINT test_button_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case 0:
    case GX_EVENT_PEN_DOWN:
        event_count++;
        break;   
    }

    return gx_window_event_process(window, event_ptr);
}
static VOID control_thread_entry(ULONG input)
{
GX_BOOL             test_successed = GX_TRUE;
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Test event displath with event type = 0. */
    gx_widget_event_process_set(&button_screen, test_button_screen_event_process);
    my_event.gx_event_type = 0;
    my_event.gx_event_target = (GX_WIDGET *)&button_screen;
    gx_system_event_send(&my_event);

    _gx_system_event_dispatch(&my_event);
    if(event_count != 0)
    {
        test_successed = GX_FALSE;
    }

    my_event.gx_event_target = GX_NULL;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = -10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = -10;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    if(event_count != 0)
    {
        test_successed = GX_FALSE;
    }

    _gx_system_focus_owner = GX_NULL;
    my_event.gx_event_type = GX_EVENT_CLICKED;
    gx_system_event_send(&my_event);

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
