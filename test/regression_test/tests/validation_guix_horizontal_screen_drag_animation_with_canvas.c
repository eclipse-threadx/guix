/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"


TEST_PARAM test_parameter = {
    "guix_horizontal_screen_drag_animation_with_canvas", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
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
       on the pixelmap drawing example. These operations simulate 
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

#include "demo_screen_drag_animation_with_canvas.c"

typedef struct TEST_STRUCT{
INT       event_type;
GX_VALUE  xpos;
GX_VALUE  ypos;
INT       wait_ticks;
GX_CHAR  *comments;
}TEST;

#define XPOS 150
#define YPOS 100

TEST test_list[]={
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Drag right. */
{GX_EVENT_PEN_DRAG, XPOS + 50,  YPOS, 11, "drag right by 50"}, 
{GX_EVENT_PEN_UP,   XPOS + 50,  YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},
{GX_EVENT_PEN_DRAG, XPOS +  3,  YPOS, 11, "drag right by 3"},
{GX_EVENT_PEN_DRAG, XPOS + 50,  YPOS, 11, "drag right by 47"},
{GX_EVENT_PEN_UP,   XPOS + 50,  YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Drag left, less than half window width. */
{GX_EVENT_PEN_DRAG, XPOS - 50,  YPOS, 11, "drag left by 50"},
{GX_EVENT_PEN_UP,   XPOS - 50,  YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Drag left. */
{GX_EVENT_PEN_DRAG, XPOS - 150, YPOS, 11, "drag left by 150"},
{GX_EVENT_PEN_UP,   XPOS - 150, YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Flick left. */
{GX_EVENT_PEN_DRAG, XPOS - 50,  YPOS,  5, "flick left"},
{GX_EVENT_PEN_UP,   XPOS - 50,  YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Flick left. */
{GX_EVENT_PEN_DRAG, XPOS - 50,  YPOS,  5, "flick left"},
{GX_EVENT_PEN_UP,   XPOS - 50,  YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Drag right, then left. */
{GX_EVENT_PEN_DRAG, XPOS + 50,  YPOS, 11, "drag right by 50"},
{GX_EVENT_PEN_DRAG, XPOS - 80,  YPOS, 11, "drag left by 130"},
{GX_EVENT_PEN_UP,   XPOS - 80,  YPOS,  5, "pen up"},
{GX_EVENT_PEN_DOWN, XPOS,       YPOS,  5, "pen down"},/* Flick right. */
{GX_EVENT_PEN_DRAG, XPOS + 50,  YPOS,  5, "flick right"},
{GX_EVENT_PEN_UP,   XPOS + 50,  YPOS,  5, "pen up"},
{0, 0, 0, 0, 0}
};

TX_SEMAPHORE semaphore_memory;
GX_CHAR *test_comments[1];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT          frame_id = 1;
TEST        *test;
GX_EVENT     my_event;
INT          style_index;

    h_screen_drag_semaphore = &semaphore_memory;

    memset(h_screen_drag_semaphore, 0, sizeof(TX_SEMAPHORE));
    tx_semaphore_create(h_screen_drag_semaphore, "", 0);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1; 

    for(style_index = 0; style_index < 2; style_index++)
    {
        if(style_index)
        {
            gx_animation_drag_disable(&h_screen_drag_animation, &main_screen.main_screen_h_menu_window);
            enable_screen_drag_animation(&h_screen_drag_animation, h_screen_list, &main_screen.main_screen_h_menu_window,
                                         GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL,
                                         ANIMATION_ID_H_SCREEN_DRAG);
        }

        test = test_list;

        while(test->event_type)
        {
            test_comments[0] = test->comments;
            gx_validation_current_frame_id_get(&frame_id);
            gx_validation_capture_frames(frame_id, 30, test_comments, 1, 1000);
            my_event.gx_event_type = test->event_type;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->xpos;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->ypos;
            gx_system_event_send(&my_event);

            if(test->wait_ticks)
            {
                tx_thread_sleep(test->wait_ticks);
            }

            if(test->event_type == GX_EVENT_PEN_UP)
            {
                tx_semaphore_get(h_screen_drag_semaphore, TX_WAIT_FOREVER);
            }
        
            test++;
        }
    }

    tx_semaphore_delete(h_screen_drag_semaphore);
    h_screen_drag_semaphore = GX_NULL;

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
