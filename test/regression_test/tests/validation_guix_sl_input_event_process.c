/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_event_process", /* Test name */
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

static GX_CONST GX_UBYTE SINGLE_LINE_TEXT[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
INT point_x;
INT point_y;
INT sleep_times;
char *comment;
}TEST;

#define SET_INPUT_TEXT GX_FIRST_USER_EVENT

TEST test_list[]={
{GX_EVENT_PEN_DOWN,           0,  43, 28,  0, "pen down on (43, 28)(outsize input area)"},
{GX_EVENT_PEN_DRAG,           0, 141, 86,  0, "pen drag on (141, 86)(inside input area)"},
{GX_EVENT_PEN_UP,             0, 141, 86,  0, "pen up on (141, 86)"},
{GX_EVENT_PEN_DOWN,           0, 119, 92,  0, "pen down on (119, 92)"},
{GX_EVENT_PEN_DRAG,           0, 100, 92,  0, "pen drag on (100, 92)"},
{GX_EVENT_PEN_UP,             0, 100, 92,  0, "pen up on (100, 92)"},
{GX_EVENT_PEN_DOWN,           0, 119, 92,  0, "pen down on (110, 92)"},
{GX_EVENT_PEN_DRAG,           0, 539, 92,  0, "pen drag on (539, 92)"},
{GX_EVENT_PEN_UP,             0, 539, 92,  0, "pen up on (539, 92)"},
{SET_INPUT_TEXT,              0,   0,  0,  0, "set input test"},
{GX_EVENT_KEY_DOWN,  GX_KEY_END,   0,  0,  0, "end"},
{GX_EVENT_PEN_DOWN,           0, 125, 92,  0, "pen down on (125, 92)"},
{GX_EVENT_PEN_DRAG,           0, 100, 92, 22, "pen drag on (100, 92)"},
{GX_EVENT_PEN_DRAG,           0, 101, 92, 22, "pen drag on (101, 92)"},
{GX_EVENT_PEN_UP,             0, 100, 92,  0, "pen up on (100, 92)"},
{GX_EVENT_KEY_DOWN, GX_KEY_HOME,   0,  0,  0, "home"},
{GX_EVENT_PEN_DOWN,           0, 523, 92,  0, "pen down on (523, 92)"},
{GX_EVENT_PEN_DRAG,           0, 539, 92, 22, "pen drag on (539, 92)"},
{GX_EVENT_PEN_DRAG,           0, 540, 92, 22, "pen drag on (540, 92)"},
{GX_EVENT_PEN_UP,             0, 539, 92,  0, "pen up on (539, 92)"},
{GX_EVENT_KEY_DOWN, GX_KEY_HOME,   0,  0,  0, "home"},
{GX_EVENT_PEN_DOWN,           0, 125, 92,  0, "pen down on (125, 92)"},
{GX_EVENT_PEN_DRAG,           0, 135, 92,  0, "pen drag on (135, 92)"},
{GX_EVENT_PEN_DRAG,           0, 125, 92,  0, "pen drag on (125, 92)"},
{GX_EVENT_PEN_UP,             0, 125, 92,  0, "pen up on (125, 92)"},
{0, 0, 0, 0, 0, ""}
};

static char test_comments[255];

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

    gx_widget_hide((GX_WIDGET *)text_input_ptr);

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);

    gx_widget_show((GX_WIDGET *)text_input_ptr);

    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_TEXT_INPUT_NOTIFY_ALL);
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);

    /* Start a custom timer. */
    gx_system_timer_start((GX_WIDGET *)text_input_ptr, 1001, 1, 0);
    my_event.gx_event_display_handle = 1;

    while(test->event_type)
    {
        if(test->event_type == SET_INPUT_TEXT)
        {
            /* Set input text. */
            gx_single_line_text_input_text_set(text_input_ptr, (GX_CONST GX_CHAR *)SINGLE_LINE_TEXT);
        }
        else
        {
            my_event.gx_event_type = test->event_type;
            if(test->event_type == GX_EVENT_KEY_DOWN)
            {
                my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
            }
            else
            {
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
            }
            tx_thread_sleep(1);
            gx_system_event_send(&my_event);
            if(test->sleep_times)
            {
                tx_thread_sleep(test->sleep_times);
            }
        }

        if(test->event_type != GX_EVENT_PEN_DRAG)
        {
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test->comment);
            gx_validation_screen_refresh();
            printf("%s\n", test->comment);
        }
        test++;
    }

    gx_widget_focus_next((GX_WIDGET *)text_input_ptr);

    /* Start ID_TEXT_INPUT_TIMER. */
    gx_system_timer_start((GX_WIDGET *)text_input_ptr, 2, 1, 0);
    tx_thread_sleep(5);
    sprintf(test_comments, "move focus next, and start text input timer when input widget do not have foucs");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comments);
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
