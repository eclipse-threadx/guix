/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_cursor", /* Test name */
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

static GX_CHAR text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char test_comment[255];

typedef struct ALIGNMENT_STRUCT{
INT value;
GX_CHAR *text;
}ALIGNMENT;

ALIGNMENT alignment_style_list[]={
{GX_STYLE_TEXT_LEFT, "left alignment"},
{GX_STYLE_TEXT_RIGHT, "right alignment"},
{GX_STYLE_TEXT_CENTER, "center alignment"},
{0, 0}
};

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, "left arrow key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW, "right arrow key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_HOME, "home key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_DELETE, "delete key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_END, "end key down"},
{GX_EVENT_KEY_DOWN, 'd', "insert character \'d\'"},
{GX_EVENT_KEY_DOWN, GX_KEY_BACKSPACE, "backspace key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, "left key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_DELETE, "delete key down"},
{0, 0, ""}
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
ALIGNMENT                 *alignment = alignment_style_list;

    memset(&my_event, 0, sizeof(GX_EVENT));
    text_input_ptr = &pMainScreen->window_sl_text_input;

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_text_input_cursor_width_set(&text_input_ptr->gx_single_line_text_input_cursor_instance, 2);
    gx_single_line_text_input_text_set(text_input_ptr, text);

    while(alignment->value)
    {
        gx_widget_style_remove((GX_WIDGET *)text_input_ptr, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)text_input_ptr, alignment->value);
 
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "set %s", alignment->text);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
        alignment++;

        test = test_list;

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "pen down on (150, 100)");
        gx_validation_set_frame_comment(test_comment);

         /* Simulate a click event. */
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 150;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 100;
        gx_system_event_send(&my_event);

        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        /* Initialize text input buffer.  */
        while(test->event_type)
        {
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test->comment);

            my_event.gx_event_type = test->event_type;
            my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
            gx_system_event_send(&my_event);
            test++;
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
