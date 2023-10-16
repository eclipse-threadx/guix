/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_button_deselect_no_output", /* Test name */
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

static VOID control_thread_entry(ULONG input)
{
GX_EVENT   my_event;
GX_BOOL    test_successed = GX_TRUE;
GX_WIDGET *test_button;

    test_button = (GX_WIDGET *)&button_screen.button_screen_base_button_1;
    gx_widget_style_add(test_button, GX_STYLE_BUTTON_EVENT_ON_PUSH);

    memset(&my_event, 0, sizeof(GX_EVENT));

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = test_button;
    gx_system_event_send(&my_event);
    if(button_click_count != 1)
    {
        test_successed = GX_FALSE;
    }

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test_button->gx_widget_size.gx_rectangle_left + 5;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test_button->gx_widget_size.gx_rectangle_top + 5;
    gx_system_event_send(&my_event);
    if(button_click_count != 1)
    {
        test_successed = GX_FALSE;
    }

    /* Added toggle style. */
    gx_widget_style_add(test_button, GX_STYLE_BUTTON_TOGGLE);
    gx_widget_style_remove(test_button, GX_STYLE_BUTTON_EVENT_ON_PUSH);

    /* Click on button. */
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    if(!(test_button->gx_widget_style & GX_STYLE_BUTTON_PUSHED))
    {
        test_successed = GX_FALSE;
    }

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    if(test_button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        test_successed = GX_FALSE;
    }

    /* select radio button. */
    test_button = (GX_WIDGET *)&button_screen.button_screen_radio_button1;
    my_event.gx_event_target = test_button;
    my_event.gx_event_type = GX_EVENT_SELECT;
    gx_system_event_send(&my_event);
    
    /* removed pushed style. */
    gx_widget_style_remove(test_button, GX_STYLE_BUTTON_PUSHED);

    /* deselect radio button. */
    my_event.gx_event_type = GX_EVENT_DESELECT;
    gx_system_event_send(&my_event);

    /* select radio button. */
    my_event.gx_event_type = GX_EVENT_SELECT;
    gx_system_event_send(&my_event);

    /* deselect radio button. */
    my_event.gx_event_type = GX_EVENT_DESELECT;
    gx_system_event_send(&my_event);

    if(button_click_count != 1)
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
