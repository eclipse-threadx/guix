/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_scroll_wheel_timer_event_process", /* Test name */
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
INT       event_payload;
INT       expect_frames;
GX_CHAR  *comments;
}TEST;

#define XPOS 270
#define YPOS 235

enum user_events{
USER_EVENT_SELECT_FIRST_ROW = GX_FIRST_APP_EVENT,
USER_EVENT_SELECT_LAST_ROW,
USER_EVENT_ADD_WRAP_STYLE,
USER_EVENT_REMOVE_WRAP_STYLE,
USER_EVENT_SET_WHEEL_SPEED
};

TEST test_list[]={
{USER_EVENT_SELECT_FIRST_ROW,    0, 1, "select first row"},
{GX_EVENT_VERTICAL_FLICK,     8533, 2, "flick down"},
{GX_EVENT_VERTICAL_FLICK,    -8533, 8, "flick up"},
{USER_EVENT_SELECT_LAST_ROW,     0, 1, "select last row"},
{GX_EVENT_VERTICAL_FLICK,    -6826, 2, "flick up"},
{USER_EVENT_ADD_WRAP_STYLE,      0, 1, "add wrap style"},
{GX_EVENT_VERTICAL_FLICK,     8533, 8, "flick down"},
{USER_EVENT_SET_WHEEL_SPEED,     0, 1, "set wheel speed"},
{USER_EVENT_REMOVE_WRAP_STYLE,   0, 1, "remove wrap style"},
{USER_EVENT_SELECT_FIRST_ROW,    0, 1, "select first row"},
{GX_EVENT_VERTICAL_FLICK,    -7680, 3, "flick up"},
{USER_EVENT_SELECT_LAST_ROW,     0, 1, "select last row"},
{GX_EVENT_VERTICAL_FLICK,     7680, 3, "flick down"},
{0, 0, 0, 0}
};

char *test_comment[1];

TX_SEMAPHORE semaphore_memory;

static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
TEST                    *test = test_list;
GX_EVENT                 my_event;
GX_STRING_SCROLL_WHEEL  *month_wheel;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    selected_semaphore = &semaphore_memory;

    tx_semaphore_create(selected_semaphore, "test_semaphore", 0);

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    while(test->event_type)
    {
        my_event.gx_event_target = GX_NULL;

        if(test->event_type == GX_EVENT_VERTICAL_FLICK)
        {
            my_event.gx_event_type = test->event_type;
            my_event.gx_event_payload.gx_event_intdata[0] = test->event_payload;
            my_event.gx_event_target = (GX_WIDGET *)month_wheel;
            gx_system_event_send(&my_event);

            test_comment[0] = test->comments;
            gx_validation_capture_frames(frame_id, test->expect_frames, test_comment, 1, 1000);
            tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);

            gx_validation_current_frame_id_get(&frame_id);
        }
        else
        {
            switch(test->event_type)
            {
            case USER_EVENT_SELECT_FIRST_ROW:
                gx_widget_hide(month_wheel);
                gx_scroll_wheel_selected_set(month_wheel, 0);
                gx_widget_show(month_wheel);
                break;
    
            case USER_EVENT_SELECT_LAST_ROW:
                gx_widget_hide(month_wheel);
                gx_scroll_wheel_selected_set(month_wheel, month_wheel->gx_scroll_wheel_total_rows - 1);
                gx_widget_show(month_wheel);
                break;
    
            case USER_EVENT_ADD_WRAP_STYLE:
                gx_widget_style_add(month_wheel, GX_STYLE_WRAP);
                break;

            case USER_EVENT_REMOVE_WRAP_STYLE:
                gx_widget_style_remove(month_wheel, GX_STYLE_WRAP);
                break;
    
            case USER_EVENT_SET_WHEEL_SPEED:
                gx_scroll_wheel_speed_set(month_wheel, GX_FIXED_VAL_MAKE(1), GX_FIXED_VAL_MAKE(1), 3, 2);
                break;
            }

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test->comments);
            gx_validation_screen_refresh();
        }
        test++;
    }

    /* Delete selected semaphore. */
    tx_semaphore_delete(selected_semaphore);
    selected_semaphore = GX_NULL;

    month_wheel->gx_widget_id = 0;
    gx_system_timer_start(month_wheel, 1024, 1, 0);

    tx_thread_sleep(2);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

