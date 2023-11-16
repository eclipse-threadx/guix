/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_scroll_wheel_scroll", /* Test name */
    100, 117, 525, 336  /* Define the coordinates of the capture area.
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

typedef struct TEST_STRUCT{
INT       event_type;
GX_VALUE  xpos;
GX_VALUE  ypos;
INT       wait_ticks;
GX_CHAR  *comments;
}TEST;

#define XPOS 270
#define YPOS 235

char test_comment[255];

static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
GX_EVENT                 my_event;
GX_STRING_SCROLL_WHEEL  *month_wheel;
INT                      index;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    my_event.gx_event_target = (GX_WIDGET *)month_wheel;
    for(index = 0; index < 2; index++)
    {
        if(index)
        {
            gx_widget_hide(month_wheel);
            sprintf(test_comment, "hide widget, scroll up");
        }
        else
        {
            sprintf(test_comment, "scroll up");
        }
        month_wheel->gx_widget_id = 0;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = XPOS;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = YPOS;
        gx_system_event_send(&my_event);
        
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = YPOS - 400;
        gx_system_event_send(&my_event);
                
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);      
        gx_validation_screen_refresh();

        tx_thread_sleep(15);

        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        while(month_wheel->gx_scroll_wheel_selected_yshift)
        {
            tx_thread_sleep(30);
        }
    }
    
    gx_scroll_wheel_selected_set(month_wheel, 0);
    gx_widget_show(month_wheel);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set selected row to 0, show month wheel.");
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = month_wheel->gx_widget_size.gx_rectangle_left + 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = month_wheel->gx_widget_size.gx_rectangle_top + 10;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("pen down, drag up by 10");
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= 10;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("drag down by 9");
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 9;
    gx_system_event_send(&my_event);

    tx_thread_sleep(15);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
    
    /* Wait snap animation complete. */
    while(month_wheel->gx_scroll_wheel_selected_yshift)
    {
        tx_thread_sleep(30);
    }

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("pen up");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

