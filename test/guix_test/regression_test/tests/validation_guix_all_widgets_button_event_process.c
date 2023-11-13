/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_button_event_process", /* Test name */
    111, 90, 490, 150  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

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


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"
GX_CHAR test_comments[256];

static UINT custom_button_select(GX_BUTTON *button)
{
    return GX_SUCCESS;
}

static int button_clicked_count = 0;

UINT test_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(GX_EVENT_CLICKED, ID_BUTTON):
        button_clicked_count++;
        break;

    default:
        gx_window_event_process(window, event_ptr);
        break;
    }

    return 0;
}

static VOID control_thread_entry(ULONG input)
{
int        frame_id = 1;
GX_EVENT   my_event;
GX_BUTTON *button;
int        index;

    gx_widget_find((GX_WIDGET *)&button_screen, ID_BUTTON, ID_BUTTON, &button);

    gx_widget_style_remove((GX_WIDGET *)button, GX_STYLE_BUTTON_REPEAT);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)button;

    for(index = 0; index < 5; index++)
    {
        switch(index)
        {
        case 0:
           button->gx_button_select_handler = (VOID (*)(GX_WIDGET *))custom_button_select;
           sprintf(test_comments, "set custom button selecter that do nothing");
           break;

        case 1:
           my_event.gx_event_type = GX_EVENT_PEN_DOWN;
           gx_system_event_send(&my_event);
           sprintf(test_comments, "pen down");
           break;

        case 2:
           my_event.gx_event_type = GX_EVENT_PEN_UP;
           gx_system_event_send(&my_event);
           sprintf(test_comments, "pen up");
           break;

        case 3:
           gx_widget_style_remove((GX_WIDGET *)button, GX_STYLE_ENABLED);
           sprintf(test_comments, "removed enabled style");
           break;

        case 4:
           my_event.gx_event_type = GX_EVENT_PEN_DOWN;
           gx_system_event_send(&my_event);
           sprintf(test_comments, "pen down");
           break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comments);
        gx_validation_screen_refresh();
    }
 
    gx_widget_event_process_set(&button_screen, test_event_handler);

    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_payload.gx_event_timer_id = 1000;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_payload.gx_event_timer_id = GX_BUTTON_TIMER;
    gx_system_event_send(&my_event);
   
    if(button_click_count != 0)
    {
        exit(1);
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
