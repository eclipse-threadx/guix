/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_window_event_process_no_output", /* Test name */
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
GX_BOOL   test_successed = GX_TRUE;
GX_EVENT  my_event;
GX_WINDOW test_window;
GX_RECTANGLE size;

    gx_widget_delete(&button_screen);

    gx_widget_hide(root);
    memset(&my_event, 0, sizeof(GX_EVENT));

    gx_utility_rectangle_define(&size, 0, 0, 640, 480);
    memset(&test_window, 0, sizeof(GX_WINDOW));
    gx_window_create(&test_window, "test_window", root, 0, 1024, &size);

    my_event.gx_event_type = GX_EVENT_FOCUS_GAINED;
    my_event.gx_event_target = (GX_WIDGET *)&test_window;
    gx_system_event_send(&my_event);

    if(_gx_system_focus_owner == (GX_WIDGET *)&test_window)
    {
        PRINT_ERROR("Focus owner shouldn't be test window!")
        test_successed = GX_FALSE;
    }    

    _gx_system_focus_owner = GX_NULL;
    gx_widget_status_add(&test_window, GX_STATUS_ACCEPTS_FOCUS);
    gx_system_event_send(&my_event);
    if(_gx_system_focus_owner != GX_NULL)
    {
        PRINT_ERROR("Focus owner should be NULL.")
        test_successed = GX_FALSE;
    }
    _gx_system_focus_owner = (GX_WIDGET *)root;

    gx_widget_status_remove(&test_window, GX_STATUS_HAS_FOCUS);
    gx_widget_status_remove(&test_window, GX_STATUS_ACCEPTS_FOCUS);
    gx_system_event_send(&my_event);
    if (test_window.gx_widget_status & GX_STATUS_HAS_FOCUS)
    {
        PRINT_ERROR("Test window shouldn't have GX_STATUS_HAS_fOCUS status.")
        test_successed = GX_FALSE;
    }

    if(_gx_system_focus_owner != (GX_WIDGET *)root)
    {
        PRINT_ERROR("Focus owner should be root.")
        test_successed = GX_FALSE;
    }

    /* Delete root. */
    gx_widget_delete(root);

    if(_gx_system_focus_owner != GX_NULL)
    {
        PRINT_ERROR("Focus owner should be NULL");
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
