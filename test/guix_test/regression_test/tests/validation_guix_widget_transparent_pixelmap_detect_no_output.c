/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "tx_thread.h"

TEST_PARAM test_parameter = {
    "guix_widget_transparent_pixelmap_detect_no_output", /* Test name */
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
GX_BOOL  test_successed = GX_TRUE;
GX_EVENT my_event;
GX_PIXELMAP_BUTTON *button = &button_screen.button_screen_pixelmap_button1;
    
    memset(&my_event, 0, sizeof(GX_EVENT));

    gx_widget_detach(button);
    gx_widget_status_remove(button, GX_STATUS_TRANSPARENT);

    my_event.gx_event_type = GX_EVENT_SHOW;
    my_event.gx_event_target = (GX_WIDGET *)button;
    gx_system_event_send(&my_event);

    if(button->gx_widget_status & GX_STATUS_TRANSPARENT)
    {
        test_successed = GX_FALSE;
    }

    gx_widget_status_add(button, GX_STATUS_VISIBLE);
    gx_system_event_send(&my_event);
    
    if(button->gx_widget_status & GX_STATUS_TRANSPARENT)
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
