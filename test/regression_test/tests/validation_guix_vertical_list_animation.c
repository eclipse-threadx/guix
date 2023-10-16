/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_window.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_animation", /* Test name */
    70, 85, 230, 330  /* Define the coordinates of the capture area.
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

#include "demo_guix_vertical_list_32bpp.c"

typedef struct TEST_STRUCT{
INT       event_type;
GX_VALUE  xpos;
GX_VALUE  ypos;
INT       flick_distance;
INT       wait_ticks;
INT       expect_frames;
GX_CHAR  *comments;
}TEST;

#define XPOS 80
#define YPOS 200

TEST test_list[]={
{GX_EVENT_PEN_DOWN,       XPOS, YPOS,       0,      5, 0, "pen down"},/* Drag down. */
{GX_EVENT_PEN_DRAG,       XPOS, YPOS + 100, 0,     11, 1, "drag down"}, 
{GX_EVENT_PEN_UP,         XPOS, YPOS + 100, 0,      5, 5, "pen up"},
{GX_EVENT_VERTICAL_FLICK, 0,    0,          10240,  0, 5, "flick down"},
{GX_EVENT_PEN_DOWN,       XPOS, YPOS,       0,      5, 0, "pen down"},/* Drag up. */
{GX_EVENT_PEN_DRAG,       XPOS, YPOS - 100, 0,     11, 2, "drag up"},
{GX_EVENT_PEN_UP,         XPOS, YPOS - 100, 0,      5, 0, "pen up"},
{GX_EVENT_VERTICAL_FLICK, 0,    0,          -10240, 0, 4, "flick up"},
{GX_EVENT_PEN_DOWN,       XPOS, YPOS,       0,      5, 0, "pen down"},/* Drag up. */
{GX_EVENT_PEN_DRAG,       XPOS, YPOS - 100, 0,     11, 2, "drag up"},
{GX_EVENT_PEN_UP,         XPOS, YPOS - 100, 0,      5, 0, "pen up"},
{GX_EVENT_PEN_DOWN,       XPOS, YPOS,       0,      5, 0, "pen down"},/* Drag up. */
{GX_EVENT_PEN_DRAG,       XPOS, YPOS - 100, 0,     11, 2, "drag up"},
{GX_EVENT_PEN_UP,         XPOS, YPOS - 100, 0,      5, 0, "pen up"},
{GX_EVENT_PEN_DOWN,       XPOS, YPOS,       0,      5, 0, "pen down"},/* Drag up. */
{GX_EVENT_PEN_DRAG,       XPOS, YPOS - 150, 0,     11, 2, "drag up"},
{GX_EVENT_PEN_UP,         XPOS, YPOS - 150, 0,      5, 0, "pen up"},
{GX_EVENT_VERTICAL_FLICK, 0,    0,          -10240, 0, 5, "flick up"},
{0, 0, 0, 0, 0, 0, 0}
};

GX_BOOL timer_find(GX_WIDGET *owner, UINT timer_id)
{
GX_TIMER *found;

    found = _gx_system_active_timer_list;

    while(found)
    {
        if(found -> gx_timer_id == timer_id &&
           found -> gx_timer_owner == owner)
        {
            return GX_TRUE;
        }
        found = found -> gx_timer_next;
    }

    return GX_FALSE;
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                 frame_id = 1;
TEST               *test;
GX_EVENT            my_event;
GX_VERTICAL_LIST *list = &pMainScreen->window_vertical_list_scroll;
GX_CHAR          *test_comment[1];
  
    memset(&my_event, 0, sizeof(GX_EVENT));
  
    my_event.gx_event_display_handle = 1; 

    test = test_list;

    while(test->event_type)
    {
        test_comment[0] = test->comments;
        gx_validation_current_frame_id_get(&frame_id);
        gx_validation_capture_frames(frame_id, 30, test_comment, 1, 1000);

        my_event.gx_event_type = test->event_type;

        if(my_event.gx_event_type == GX_EVENT_VERTICAL_FLICK)
        {
            my_event.gx_event_payload.gx_event_intdata[0] = test->flick_distance;
        }
        else
        {
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->xpos;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->ypos;
        }
        gx_system_event_send(&my_event);

        if(test->wait_ticks)
        {
            tx_thread_sleep(test->wait_ticks);
        }

        if(test->event_type == GX_EVENT_PEN_UP || test->event_type == GX_EVENT_VERTICAL_FLICK)
        {
            while (timer_find((GX_WIDGET *)list, GX_FLICK_TIMER) ||
                   timer_find((GX_WIDGET *)list, GX_SNAP_TIMER))
            {
                /* Wait utill animation complete. */
                tx_thread_sleep(5);
            }
        }
    
        test++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
