/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_block_move", /* Test name */
    30, 120, 245, 330  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text input drawing area.  */
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
       on the multi line text input example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text input routine. */
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

#include "demo_guix_ml_text_input_16bpp.c"

GX_CHAR test_string[]=
"0\r1\r2\r3\r4\r5\r6\r7\r8\r9\r\
0\r1\r2\r3\r4\r5\r6\r7\r8\r9\r\
0\r1\r2\r3\r4\r5\r6\r7\r8\r9\r0\r\
31\r32\r33\r34\r35\r36\r37\r38\r39\r40";

char test_comment[256];

typedef struct TEST_STRUCT{
INT event_type;
GX_VALUE point_x;
GX_VALUE point_y;
INT      scroll_value;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_PEN_DOWN,        38, 176,   0, "pen down on (38, 176<F5>)"},
{GX_EVENT_VERTICAL_SCROLL,  0,   0, 768, "scroll up"},
{0,                         0,   0,   0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;
TEST                     *test = test_list;
GX_SCROLLBAR             *scroll;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;
    gx_window_scrollbar_find((GX_WINDOW *)text_input, GX_TYPE_VERTICAL_SCROLL, &scroll);

     /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
 
    gx_multi_line_text_input_text_set(text_input, test_string);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set input string");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 1);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set whitespace to 1");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
  
    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_target = (GX_WIDGET *)text_input;

        if(test->event_type == GX_EVENT_PEN_DOWN)
        {
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
            my_event.gx_event_target = GX_NULL;
        }
        else if(test->event_type == GX_EVENT_VERTICAL_SCROLL)
        {
            /* New scroll value. */
            my_event.gx_event_payload.gx_event_intdata[0] = test->scroll_value;
            /* Old scroll value. */
            my_event.gx_event_payload.gx_event_intdata[1] = scroll->gx_scrollbar_info.gx_scroll_value;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_system_event_send(&my_event);
        
        test++;
    }

    gx_validation_end();

    exit(0);
}
