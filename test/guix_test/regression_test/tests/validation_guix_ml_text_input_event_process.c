/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_event_process", /* Test name */
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

GX_CHAR test_string_2[]="why do the birds";

char test_comment[256];

typedef struct TEST_STRUCT{
INT event_type;
INT sleep_time;
GX_VALUE point_x;
GX_VALUE point_y;
INT      scroll_value;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_PEN_DOWN,                 0, 10,  278,   0, "pen down on (10, 278)(outsize widget area)"},
{GX_EVENT_PEN_DRAG,                 0, 159, 158,   0, "pen drag on (159, 158)(inside widget area)"},
{GX_EVENT_PEN_UP,                   0, 43,  133,   0, "pen up on (43, 133)"},
{GX_EVENT_PEN_DOWN,                 0, 36,  278,   0, "pen down on (36, 278)"},
{GX_EVENT_PEN_DRAG,                22, 54,  324,   0, "pen drag on (54, 324)"},
{GX_EVENT_PEN_DRAG,                22, 54,  325,   0, "pen drag on (54, 325)"},
{GX_EVENT_PEN_DRAG,                 0, 54,  278,   0, "pen drag on (54, 278)"},
{GX_EVENT_PEN_UP,                   0, 54,  278,   0, "pen up on (54, 278)"},
{GX_EVENT_PEN_DOWN,                 0, 54,  241,    0, "pen down on (54, 241)"},
{GX_EVENT_PEN_DRAG,                22, 54,  112,    0, "pen drag on (54, 112)"},
{GX_EVENT_PEN_DRAG,                22, 54,  113,    0, "pen drag on (54, 113)"},
{GX_EVENT_PEN_UP,                   0, 54,  113,    0, "pen up on (54, 113)"},
{GX_EVENT_PEN_DOWN,                 0, 36,  242,    0, "pen down on (36, 242)"},
{GX_EVENT_PEN_DRAG,                 0, 46,  242,    0, "pen drag on (46, 242)"},
{GX_EVENT_PEN_DRAG,                 0, 36,  242,    0, "pen drag on (36, 242)"},
{GX_EVENT_PEN_UP,                   0, 36,  242,    0, "pen up on (36, 242)"},
{0,                                 0,  0,   0,   0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;
TEST                     *test;
int                       index;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;

     /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK | GX_STYLE_CURSOR_ALWAYS);

    /* Start a custom timer. */
    gx_system_timer_start((GX_WIDGET *)text_input, 1001, 1, 0);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    for(index = 0; index < 2; index++)
    {
        if(index == 0)
        {
            gx_widget_hide((GX_WIDGET *)text_input);
            gx_multi_line_text_input_text_set(text_input, test_string);
            gx_widget_show((GX_WIDGET *)text_input);

            sprintf(test_comment, "set text of 40 lines");
        }
        else
        {
            gx_multi_line_text_input_text_set(text_input, test_string_2);
            sprintf(test_comment, "set text of one line");
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        test = test_list;
  
        while(test->event_type)
        {
            my_event.gx_event_type = test->event_type;
            my_event.gx_event_target = (GX_WIDGET *)text_input;

            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
            my_event.gx_event_target = GX_NULL;

            tx_thread_sleep(1);
            gx_system_event_send(&my_event);
            
            if(test->sleep_time)
            {
                tx_thread_sleep(test->sleep_time);
            }

            if(test->event_type != GX_EVENT_PEN_DRAG)
            {
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test->comment);
                gx_validation_screen_refresh();
            }

            test++;
        }
    }

    gx_widget_focus_next((GX_WIDGET *)text_input);
    gx_system_timer_start((GX_WIDGET *)text_input, 2, 1, 0);
    tx_thread_sleep(5);
    sprintf(test_comment, "move focus next, start text input timer when input widget do not have focus");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_system_focus_claim((GX_WIDGET *)text_input);
    gx_multi_line_text_input_text_select(text_input, 0, 10);
    gx_system_timer_start((GX_WIDGET *)text_input, 2, 1, 0);
    tx_thread_sleep(5);
    sprintf(test_comment, "gaim focus, select text[0:10], start text input timer");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_LANGUAGE_CHANGE;
    my_event.gx_event_target = (GX_WIDGET *)text_input;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send GX_EVENT_LANGUAGE_CHANGE event");
    gx_validation_screen_refresh();

    gx_validation_end();

    exit(0);
}
