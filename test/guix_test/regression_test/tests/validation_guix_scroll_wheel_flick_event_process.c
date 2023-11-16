/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_scroll_wheel_flick_event_process", /* Test name */
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

char *test_comment[1];

TX_SEMAPHORE semaphore_memory;

static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
GX_EVENT                 my_event;
GX_STRING_SCROLL_WHEEL  *month_wheel;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    selected_semaphore = &semaphore_memory;

    tx_semaphore_create(selected_semaphore, "test_semaphore", 0);

    test_comment[0] = malloc(255);

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    /********************************************************************/
    month_wheel->gx_scroll_wheel_selected_yshift = -43;
    
    tx_thread_sleep(1);

    my_event.gx_event_target = (GX_WIDGET *)month_wheel;
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = 7168;
    gx_system_event_send(&my_event);

    sprintf(test_comment[0], "flick event, yshift = %d, flick speed = %d",
            month_wheel->gx_scroll_wheel_selected_yshift,
            my_event.gx_event_payload.gx_event_intdata[0]);

    gx_validation_current_frame_id_get(&frame_id);
    gx_validation_capture_frames(frame_id, 30, test_comment, 1, 1000);

    tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);

    /********************************************************************/
    month_wheel->gx_scroll_wheel_selected_yshift = 12;
    
    tx_thread_sleep(1);

    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -15360;
    gx_system_event_send(&my_event);

    sprintf(test_comment[0], "flick event, yshift = %d, flick speed = %d",
            month_wheel->gx_scroll_wheel_selected_yshift,
            my_event.gx_event_payload.gx_event_intdata[0]);

    gx_validation_current_frame_id_get(&frame_id);
    gx_validation_capture_frames(frame_id, 30, test_comment, 1, 1000);

    tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);

    /********************************************************************/
    gx_scroll_wheel_speed_set(month_wheel, 0, 0, 10, 1);
    tx_thread_sleep(1);
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -15360;
    gx_system_event_send(&my_event);

    sprintf(test_comment[0], "flick event, yshift = %d, flick speed = %d, flick speed = 0",
            month_wheel->gx_scroll_wheel_selected_yshift,
            my_event.gx_event_payload.gx_event_intdata[0]);

    gx_validation_current_frame_id_get(&frame_id);
    gx_validation_capture_frames(frame_id, 30, test_comment, 1, 1000);

    tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);

    /********************************************************************/
    month_wheel->gx_widget_id = 0;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = XPOS;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = YPOS;
    gx_system_event_send(&my_event);
    
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
    
    sprintf(test_comment[0], "pen down, pen drag, pen up on the same point");
    gx_validation_current_frame_id_get(&frame_id);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment[0]);      
    gx_validation_screen_refresh();

    /********************************************************************/
    /* Set scroll wheel total rows 0*/
    gx_scroll_wheel_total_rows_set(month_wheel, 0);

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = 7168;
    gx_system_event_send(&my_event);

    my_event.gx_event_payload.gx_event_intdata[0] = 5;
    gx_system_event_send(&my_event);
  
    sprintf(test_comment[0], "set total rows 0, send pen down and flick event");
    gx_validation_current_frame_id_get(&frame_id);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment[0]);      
    gx_validation_screen_refresh();

    /* Delete selected semaphore. */
    tx_semaphore_delete(selected_semaphore);
    selected_semaphore = GX_NULL;

    free(test_comment[0]);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

