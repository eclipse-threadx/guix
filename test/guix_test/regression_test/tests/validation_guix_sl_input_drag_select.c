/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_drag_select", /* Test name */
    114, 69, 538, 119  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_565rgb
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_sl_text_input_16bpp.c"

static GX_CHAR english_text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
static GX_CHAR *test_comment[1];

typedef struct TEST_STRUCT{
INT event_type;
GX_VALUE point_x;
GX_VALUE point_y;
INT      expected_frames;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"},/* Drag left*/
{GX_EVENT_PEN_DRAG, 236, 100, 1, "pen drag to (236, 100)"},
{GX_EVENT_PEN_UP,   236, 100, 1, "pen up on (236, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"}, /* Drag left and right */
{GX_EVENT_PEN_DRAG, 236, 100, 1, "pen drag to (236, 100)"},
{GX_EVENT_PEN_DRAG, 245, 100, 1, "pen drag to (245, 100)"},
{GX_EVENT_PEN_DRAG, 335, 100, 1, "pen drag to (335, 100)"},
{GX_EVENT_PEN_UP,   335, 100, 1, "pen up on (335, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"},/* Drag right*/
{GX_EVENT_PEN_DRAG, 335, 100, 1, "pen drag to (335, 100)"},
{GX_EVENT_PEN_UP,   335, 100, 1, "pen up on (335, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"},/* Drag right and left*/
{GX_EVENT_PEN_DRAG, 335, 100, 1, "pen drag to (335, 100)"},
{GX_EVENT_PEN_DRAG, 322, 100, 1, "pen drag to (322, 100)"},
{GX_EVENT_PEN_DRAG, 245, 100, 1, "pen drag to (245, 100)"},
{GX_EVENT_PEN_UP,   245, 100, 1, "pen up on (245, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"},/* Drag to client right*/
{GX_EVENT_PEN_DRAG, 557, 100, 10, "pen drag to (557, 100)"},
{GX_EVENT_PEN_UP,   517, 100, 1, "pen up on (517, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"}, /* Drag to client left*/
{GX_EVENT_PEN_DRAG, 115, 100, 10, "pen drag to (115, 100)"},
{GX_EVENT_PEN_UP,   151, 100, 1, "pen up on (151, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"},/* Drag to client right and left. */
{GX_EVENT_PEN_DRAG, 557, 100, 10, "pen drag to (557, 100)"},
{GX_EVENT_PEN_DRAG, 517, 100, 1,  "pen drag to (715, 100)"},
{GX_EVENT_PEN_UP,   517, 100, 1,  "pen up on (715, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (286, 100)"},/* Drag to client left and right*/
{GX_EVENT_PEN_DRAG, 115, 100, 10, "pen drag to (115, 100)"},
{GX_EVENT_PEN_DRAG, 151, 100, 1, "pen drag to (151, 100)"},
{GX_EVENT_PEN_UP,   151, 100, 1, "pen up on (151, 100)"},
{GX_EVENT_PEN_DOWN, 286, 100, 1, "pen down on (186, 100)"},/* Drag left*/
{GX_EVENT_PEN_DRAG, 236, 100, 1, "pen drag to (236, 100)"},
{GX_EVENT_PEN_UP,   236, 100, 1, "pen up on (236, 100)"},
{0,                   0,   0, 0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
GX_EVENT                   my_event;
int                        frame_id = 1;
TEST                      *test;

    text_input_ptr = &pMainScreen->window_sl_text_input;

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_text_set(text_input_ptr, english_text);

    memset(&my_event, 0, sizeof(GX_EVENT));

    test_comment[0] = (GX_CHAR *)malloc(256);

    test = test_list;

    while(test->event_type)
    {

        tx_thread_sleep(1);
        gx_validation_current_frame_id_get(&frame_id);
        frame_id++;
        if(test->expected_frames == 1)
        {
            gx_validation_set_frame_id(frame_id);
            gx_validation_set_frame_comment(test->comment);
        }
GX_ENTER_CRITICAL
        if(test->expected_frames > 1)
        {
            sprintf(test_comment[0], "%s, start capture", test->comment);
            /* start_id, expected frames, comments, num_comments, max_time. */
            gx_validation_capture_frames(frame_id, test->expected_frames, (GX_CHAR **)test_comment, 1, 100);
        }

        my_event.gx_event_type = test->event_type;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;

        gx_system_event_send(&my_event);

GX_EXIT_CRITICAL
        gx_validation_capture_frames_wait();
        test++;
    }

    free(test_comment[0]);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
