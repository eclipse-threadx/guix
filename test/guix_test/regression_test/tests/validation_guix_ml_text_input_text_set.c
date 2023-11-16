/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_text_set", /* Test name */
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


char test_comment[256];
char test_string[]="1\r2\r3\r4\r5\r6\r7\r8\r9\r10\r11\r12\r13\r14\r15\r16\r17\r18\r19\r20\r21\r22\r23\r24\r25\r26\r27\r28\r29\r30\r31\r32\r33\r34\r35";

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
int                       index;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;


    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;
    memset(&my_event, 0, sizeof(GX_EVENT));

    gx_widget_detach((GX_WIDGET *)&window.window_input_scroll);

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(text_input, GX_STYLE_CURSOR_ALWAYS);

    /* Send pen down event to gain input focus. */
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 100;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;
    gx_system_event_send(&my_event);

    for(index = 0; index < 11; index++)
    {
        switch(index)
        {
        case 0:
            gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 20);
            sprintf(test_comment, "set invalid font");
            break;

        case 1:
            gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 1);
            sprintf(test_comment, "set valid font");
            break;

        case 2:
            gx_multi_line_text_input_text_set(text_input, test_string);
            sprintf(test_comment, "set test string");
            break;

        case 3:
            gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 20);
            sprintf(test_comment, "set invalid font");
            break;

        case 4:
           sprintf(test_comment, "mark up");
           my_event.gx_event_type = GX_EVENT_MARK_UP;
           gx_system_event_send(&my_event);
           break;

        case 5:
           sprintf(test_comment, "mark previous");
           my_event.gx_event_type = GX_EVENT_MARK_PREVIOUS;
           gx_system_event_send(&my_event);
           break;

        case 6:
           sprintf(test_comment, "mark down");
           my_event.gx_event_type = GX_EVENT_MARK_DOWN;
           gx_system_event_send(&my_event);
           break;

        case 7:
          gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 0);
          gx_multi_line_text_input_buffer_clear(text_input);
          sprintf(test_comment, "valid font, clear buffer");
          break;

        case 8:
          gx_widget_hide(text_input);
          gx_multi_line_text_input_text_set(text_input, test_string);
          gx_widget_show(text_input);
          sprintf(test_comment, "hide input widget, set input string, show input widget");
          break;
        
        case 9:
          gx_multi_line_text_input_text_select(text_input, 0, 10);
          sprintf(test_comment, "select string[0:10]]");
          break;

        case 10:
          gx_widget_style_remove(text_input, GX_STYLE_ENABLED);
          sprintf(test_comment, "disable input widget");
          break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_validation_end();

    exit(0);
}
