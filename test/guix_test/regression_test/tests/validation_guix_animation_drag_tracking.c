/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_animation.h"

TEST_PARAM test_parameter = {
    "guix_animation_drag_tracking", /* Test name */
    69, 80, 348, 468  /* Define the coordinates of the capture area.
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

#include "demo_screen_drag_animation.c"
typedef struct TEST_STRUCT{
int event_type;
int point_x;
int point_y;
GX_BOOL horizontal;
char *comment;
}TEST;

#define XPOS 150
#define YPOS 100
#define V_XPOS 270
#define V_YPOS 300

TEST test_list[]={
{GX_EVENT_PEN_DOWN,         XPOS,         YPOS,       1, "pen down"},/* Drag left. */
{GX_EVENT_PEN_DRAG,         XPOS - 150,   YPOS,       1, "drag left"},
{GX_EVENT_PEN_UP,           XPOS - 150,   YPOS,       1, "pen up"},
{GX_EVENT_PEN_DOWN,         XPOS,         YPOS,       1, "pen down"},/* Drag left. */
{GX_EVENT_PEN_DRAG,         XPOS - 150,   YPOS,       1, "drag left"},
{GX_EVENT_PEN_UP,           XPOS - 150,   YPOS,       1, "pen up"},
{GX_EVENT_PEN_DOWN,         XPOS,         YPOS,       1, "pen down"},/* Drag left. */
{GX_EVENT_PEN_DRAG,         XPOS - 150,   YPOS,       1, "drag left"},
{GX_EVENT_PEN_DRAG,         XPOS - 150,   YPOS,       1, "send same drag event as previous"},
{GX_EVENT_PEN_DRAG,         XPOS + 150,   YPOS,       1, "drag right"},
{GX_EVENT_PEN_UP,           XPOS + 150,   YPOS,       1, "pen up"},
{GX_EVENT_PEN_DOWN,         V_XPOS,     V_YPOS,       0, "pen down"},/* Drag up. */
{GX_EVENT_PEN_DRAG,         V_XPOS,     V_YPOS - 160, 0, "drag up"},
{GX_EVENT_PEN_DRAG,         V_XPOS,     V_YPOS + 160, 0, "drag down"},
{GX_EVENT_PEN_UP,           V_XPOS,     V_YPOS + 160, 0, "pen up"},
{0,                         0,             0,         0,  ""}
};

TX_SEMAPHORE semaphore_memory[2];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                 frame_id = 1;
TEST               *test;
GX_EVENT            my_event;

    /* Add wrap stlyle for horizontal and vertical drag animation. */
    v_screen_drag_semaphore = &semaphore_memory[0];
    h_screen_drag_semaphore = &semaphore_memory[1];

    memset(v_screen_drag_semaphore, 0, sizeof(TX_SEMAPHORE));
    memset(h_screen_drag_semaphore, 0, sizeof(TX_SEMAPHORE));

    tx_semaphore_create(v_screen_drag_semaphore, "", 0);
    tx_semaphore_create(h_screen_drag_semaphore, "", 0);

    memset(&my_event, 0, sizeof(GX_EVENT));
  
    my_event.gx_event_display_handle = 1; 

    test = test_list;

    while(test->event_type)
    {

        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;

        if(test->horizontal)
        {
            my_event.gx_event_target = (GX_WIDGET *)&main_screen.main_screen_h_menu_window;
        }
        else
        {
            my_event.gx_event_target = (GX_WIDGET *)&main_screen.main_screen_v_menu_window;
        }
        tx_thread_sleep(1);
        gx_system_event_send(&my_event);

        if(test->event_type == GX_EVENT_PEN_UP)
        {
            if(test->horizontal)
            {
                tx_semaphore_get(h_screen_drag_semaphore, TX_WAIT_FOREVER);
            }
            else
            {
                tx_semaphore_get(v_screen_drag_semaphore, TX_WAIT_FOREVER);
            }
        }
        else if(test->event_type == GX_EVENT_PEN_DRAG)
        {
            tx_thread_sleep(11);
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();
        test++;
    }

    /* Test flick event when direction is not left/rigth/up/down. */
    v_screen_drag_animation.gx_animation_status = GX_ANIMATION_SLIDE_LANDING;
    v_screen_drag_animation.gx_animation_slide_direction = 0;
    my_event.gx_event_payload.gx_event_intdata[0] = 0;
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    gx_system_event_send(&my_event);

    /* Test pen up event when direction is not left/right/up/down. */
    v_screen_drag_animation.gx_animation_status = GX_ANIMATION_SLIDE_TRACKING;
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    /* Test landing when target is not available. */
    v_screen_drag_animation.gx_animation_status = GX_ANIMATION_SLIDE_LANDING;
    gx_system_timer_start((GX_WIDGET *)&main_screen.main_screen_v_menu_window, GX_ANIMATION_SLIDE_TIMER, 1, 0);
    tx_thread_sleep(3);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
