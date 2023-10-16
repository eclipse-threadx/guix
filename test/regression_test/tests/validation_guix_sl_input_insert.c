/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_insert", /* Test name */
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

static GX_CONST GX_UBYTE SINGLE_LINE_TEXT[] = "12345";

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
char *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_KEY_DOWN, GX_KEY_END, "end key down"},
{GX_EVENT_MARK_PREVIOUS, 0, "mark previous"},
{GX_EVENT_KEY_DOWN, '6', "insert characer '6'"},
{GX_EVENT_COPY, 0, "copy"},
{GX_EVENT_PASTE, 0, "paste"},
{GX_EVENT_CUT, 0, "cut"},
{GX_EVENT_PASTE, 0, "paste"},
{0, 0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
int                        frame_id = 1;
GX_EVENT                   my_event;
TEST                      *test = test_list;

    memset(&my_event, 0, sizeof(GX_EVENT));
    text_input_ptr = &pMainScreen->window_sl_text_input;

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_TEXT_ALIGNMENT_MASK);

    /* Set input test. */
    gx_single_line_text_input_text_set(text_input_ptr, (GX_CONST GX_CHAR *)SINGLE_LINE_TEXT);

    gx_system_focus_claim((GX_WIDGET *)text_input_ptr);

    my_event.gx_event_display_handle = 1;

    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();
        test++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
