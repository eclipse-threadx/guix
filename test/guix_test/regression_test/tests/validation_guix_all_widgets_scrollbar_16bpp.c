/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_scrollbar_16bpp", /* Test name */
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int vertical_index, horizontal_index;
int vertical_count = 0;
int horizontal_count = 0;
int frame_id = 1;
int vertical_flag = 0;
GX_EVENT my_event;

    ToggleScreen(pWindowScreen, pButtonScreen);
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    for(vertical_index = 0; vertical_index <= 16; vertical_index++)
    {
        horizontal_count = 0;
        my_event.gx_event_target =(GX_WIDGET *)&window_screen.window_screen_vertical_scroll_1;

        if(vertical_count == 1)
        {
            my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
            my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
            gx_system_event_send(&my_event);
        }
        else if(vertical_count > 1 && vertical_count <= 9)
        {
            my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED);
            my_event.gx_event_sender = GX_ID_SCROLL_DOWN_RIGHT;
            gx_system_event_send(&my_event);
        }
        else if(vertical_count > 9)
        {
            my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
            my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
            gx_system_event_send(&my_event);
        }
        vertical_count++;
        if(my_event.gx_event_sender == GX_ID_SCROLL_UP_LEFT)
        {
            vertical_flag = 1;
        }
        else if(my_event.gx_event_sender == GX_ID_SCROLL_DOWN_RIGHT)
        {
            vertical_flag = 2;
        }
        else
        {
            vertical_flag = 0;
        }

        for(horizontal_index = 0; horizontal_index <= 12; horizontal_index++)
        {
            my_event.gx_event_target =(GX_WIDGET *)&window_screen.window_screen_hscroll_1;
            if(horizontal_count == 0)
            {
                my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
                my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
                gx_system_event_send(&my_event);
            }
            else if(horizontal_count > 0 && horizontal_count <= 6)
            {
                my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED);
                my_event.gx_event_sender = GX_ID_SCROLL_DOWN_RIGHT;
                gx_system_event_send(&my_event);
            }
            else if(horizontal_count > 6)
            {
                my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
                my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
                gx_system_event_send(&my_event);
            }
            horizontal_count++;

            gx_validation_set_frame_id(frame_id);

            sprintf(test_comment, "vertical_scroll ");
            if(vertical_flag == 1)
            {
                strcat(test_comment, "up ");
            }
            else if(vertical_flag == 2)
            {
                strcat(test_comment, "down ");
            }
            else if(vertical_flag == 0)
            {
                strcat(test_comment, "not_move ");
            }

            strcat(test_comment, "horizontal_scroll ");
            if(my_event.gx_event_sender == GX_ID_SCROLL_UP_LEFT)
            {
                strcat(test_comment, "left ");
            }
            else
            {
                strcat(test_comment, "right ");
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
