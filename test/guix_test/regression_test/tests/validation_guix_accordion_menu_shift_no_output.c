/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"

TEST_PARAM test_parameter = {
    "guix_accordion_menu_shift_no_output", /* Test name */
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
GX_BOOL            test_successed = GX_TRUE;
GX_EVENT           my_event;
GX_ACCORDION_MENU *accordion;
GX_MENU_LIST      *menu_list;

    memset(&my_event, 0, sizeof(GX_EVENT));

    accordion = &menu_screen.menu_screen_one_level_accordion;

    /* Send menu item close timer event when collapse item is NULL. */
    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_payload.gx_event_timer_id = GX_MENU_CLOSE_TIMER;
    my_event.gx_event_target = (GX_WIDGET *)accordion;
    gx_system_event_send(&my_event);

    /* Send menu item open timer event when expand item is NULL. */
    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_payload.gx_event_timer_id = GX_MENU_OPEN_TIMER;
    my_event.gx_event_target = (GX_WIDGET *)accordion;
    gx_system_event_send(&my_event);

    /* Accordion parent = GX_NULL. */
    accordion->gx_accordion_menu_collapse_item = (GX_WIDGET *)&menu_screen.menu_screen_ola_menu_1;
    gx_widget_detach(accordion);
    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_payload.gx_event_timer_id = GX_MENU_CLOSE_TIMER;
    my_event.gx_event_target = (GX_WIDGET *)accordion;
    gx_system_event_send(&my_event);

    /* Menu parent type != GX_ACCORDION_MENU. */
    accordion = &menu_screen.menu_screen_mla_menu_1_accordion;
    accordion->gx_accordion_menu_collapse_item = (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_1_2;
    menu_screen.menu_screen_multi_level_accordion.gx_widget_type = 0;
    
    my_event.gx_event_target = (GX_WIDGET *)accordion;
    gx_system_event_send(&my_event);

    /* Menu parent = GX_NULL. */
    gx_widget_detach(&menu_screen.menu_screen_mla_menu_1);
    menu_list = (GX_MENU_LIST *)accordion->gx_widget_parent;

    gx_system_event_send(&my_event);

    /* Invalid menu list owner. */
    menu_list->gx_menu_list_owner = GX_NULL;
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
