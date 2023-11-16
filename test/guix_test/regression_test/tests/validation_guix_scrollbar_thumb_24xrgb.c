/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_scrollbar_thumb_24xrgb", /* Test name */
    347, 43, 567, 187  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the scrollbar
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command scrollbar argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the scrollbar drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual scrollbar routine. */
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

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int vertical_index, horizontal_index;
GX_POINT vertical_value = {550, 75};
GX_POINT horizontal_value = {400, 170};
int frame_id = 1;
GX_EVENT my_event;

    ToggleScreen(pWindowScreen, pButtonScreen);
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    for(vertical_index = 0; vertical_index < 23; vertical_index++)
    {
        if(vertical_index > 0)
        {
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = vertical_value.gx_point_x;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = vertical_value.gx_point_y;
            gx_system_event_send(&my_event);
            if(vertical_index < 12)
            {
                vertical_value.gx_point_y += 5;
            }
            else
            {
                vertical_value.gx_point_y -= 5;
            }
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = vertical_value.gx_point_y;
            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            gx_system_event_send(&my_event);
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
        }

        for(horizontal_index = 0; horizontal_index < 33; horizontal_index++)
        {
            if(horizontal_index > 0)
            {
                my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = horizontal_value.gx_point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = horizontal_value.gx_point_y;
                gx_system_event_send(&my_event);
                if(horizontal_index < 17)
                {
                    horizontal_value.gx_point_x += 5;
                }
                else
                {
                    horizontal_value.gx_point_x -= 5;
                }
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = horizontal_value.gx_point_x;
                my_event.gx_event_type = GX_EVENT_PEN_DRAG;
                gx_system_event_send(&my_event);
                my_event.gx_event_type = GX_EVENT_PEN_UP;
                gx_system_event_send(&my_event);
            }

            gx_validation_set_frame_id(frame_id);

            sprintf(test_comment, "horizontal_scroll ");
            if(horizontal_index > 0 && horizontal_index < 17)
            {
                strcat(test_comment, "right ");
            }
            else if(horizontal_index >= 17)
            {
                strcat(test_comment, "left ");
            }
            else
            {
                strcat(test_comment, "notmove ");
            }
            if(horizontal_index == 0 && vertical_index < 12 && vertical_index > 0)
            {
                strcat(test_comment, "vertical_scroll down ");
            }
            else if(horizontal_index == 0 && vertical_index >= 12)
            {
                strcat(test_comment, "vertical_scroll up ");
            }
            
            gx_validation_set_frame_comment(test_comment);

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pWindowScreen);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
