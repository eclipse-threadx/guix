/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_widget_child_focus_assign_no_output", /* Test name */
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
GX_SCROLLBAR scrollbar;

    memset(&my_event, 0, sizeof(GX_EVENT));

    gx_widget_attach(root, &window_screen);
    gx_system_focus_claim(&window_screen);
    
    if(_gx_system_focus_owner != (GX_WIDGET *)&window_screen.window_screen_vertical_list)
    {
        test_successed = GX_FALSE;
    }

    /* Send GX_EVENT_FOCUS_GAINED to base_button_1. */
    my_event.gx_event_type = GX_EVENT_FOCUS_GAINED;
    my_event.gx_event_target = (GX_WIDGET *)&button_screen.button_screen_base_button_1;
    gx_system_event_send(&my_event);

    gx_system_focus_claim(&button_screen);

    if(_gx_system_focus_owner != (GX_WIDGET *)&button_screen.button_screen_base_button_1)
    {
        test_successed = GX_FALSE;
    }
   
    /* Removed default foucus status, if no child has default focus, focus will be assigned to the first
       widget that accepts foucs. */
    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    gx_horizontal_scrollbar_create(&scrollbar, "test_scrollbar", GX_NULL, GX_NULL, GX_STYLE_ENABLED);
    gx_widget_back_attach(&window_screen, &scrollbar);
    gx_widget_status_add(&scrollbar, GX_STATUS_ACCEPTS_FOCUS);

    my_event.gx_event_type = GX_EVENT_FOCUS_GAINED;
    my_event.gx_event_target = (GX_WIDGET *)&window_screen.window_screen_horizontal_list_1;
    gx_system_event_send(&my_event);

    /* Detach other child widgets that accepts focus. */
    gx_widget_detach(&window_screen.window_screen_vertical_list);
    gx_widget_detach(&window_screen.window_screen_drop_list);
    gx_widget_detach(&window_screen.window_screen_line_chart);
    gx_widget_detach(&window_screen.window_screen_next_button_1);

    /* Removed default focus status.*/ 
    gx_widget_status_remove(&window_screen.window_screen_horizontal_list_1, GX_STATUS_DEFAULT_FOCUS);

    gx_system_focus_claim(&window_screen);
    if(_gx_system_focus_owner != (GX_WIDGET *)&window_screen)
    {
        test_successed = GX_FALSE;
    }

    my_event.gx_event_type = GX_EVENT_HIDE;
    my_event.gx_event_target = (GX_WIDGET *)&window_screen;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_SHOW;
    gx_system_event_send(&my_event);
    if(!(window_screen.window_screen_horizontal_list_1.gx_widget_status & GX_STATUS_DEFAULT_FOCUS))
    {
        test_successed = GX_FALSE;
    }

    _gx_widget_child_focus_assign((GX_WIDGET *)&window_screen);
    _gx_widget_child_focus_assign((GX_WIDGET *)&window_screen);
    if(_gx_system_focus_owner != (GX_WIDGET *)&window_screen.window_screen_horizontal_list_1)
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
