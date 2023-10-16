/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_event_remove_no_output", /* Test name */
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

static GX_BOOL test_fail = GX_FALSE;
static int count = 0;

GX_VALUE ypos[] = {110, 150, 180, 260, 330, 360};

static UINT button_screen_event_process(GX_WINDOW *win, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
        case GX_EVENT_PEN_DOWN:
        case GX_EVENT_PEN_UP:
        test_fail = GX_TRUE;
        break;
        
        case GX_EVENT_TIMER:
        count++;
        break;
    }
    
    return gx_window_event_process(win, event_ptr);
}
static GX_WIDGET *list[] = 
{
    (GX_WIDGET *)&button_screen.button_screen_base_button_1,
    (GX_WIDGET *)&button_screen.button_screen_text_button_1,
    (GX_WIDGET *)&button_screen.button_screen_multi_line_button_1,
    (GX_WIDGET *)&button_screen.button_screen_pixelmap_button1,
    (GX_WIDGET *)&button_screen.button_screen_checkbox,
    (GX_WIDGET *)&button_screen.button_screen_radio_button1,
    (GX_WIDGET *)&button_screen.button_screen_base_button_1
};

static VOID control_thread_entry(ULONG input)
{
int index = 0;
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    GX_ENTER_CRITICAL
    gx_widget_event_process_set(pButtonScreen, (UINT (*)(GX_WIDGET *, GX_EVENT *))button_screen_event_process);
    
    for(index = 0; index < (int)(sizeof(ypos)/sizeof(int)); index++)
    {
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 150;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos[index];
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_target = list[index];
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
    }
    
    for(index = 0; index < (int)(sizeof(list)/sizeof(GX_WIDGET *)); index++)
    {
        gx_widget_delete(list[index]);
    }
    GX_EXIT_CRITICAL
    
    gx_system_timer_start(pButtonScreen, 10, 1, 1);
    
    while(count < 0xff);
   
    if(test_fail == GX_FALSE) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }

    exit(0);

}
