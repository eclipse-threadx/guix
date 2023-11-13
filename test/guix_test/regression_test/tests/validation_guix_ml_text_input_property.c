/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_property", /* Test name */
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
"0 do you remember the day we used to be\r\r1\r2\r3\r4ABCDEFGHIJKLMN\r5\r6\r7\r8\r9\r\r\
0\r1\r2\r3\r4\r5OPQRSTUVWXYZ\r6\r7\r8\r9\r\
0\r1\r2\r3\r4\r5abcdefghijklmn\r6\r7\r8\r9\r0\r\
31\r32\r33\r34\r35\r36\r37ABCDEFG\r38abcdefghijk\r39ABCDEF\r40";

char test_comment[256];
char input_buffer[3];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT *text_input;
int                       index;
GX_EVENT                  my_event;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;
    
     /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);

    for(index = 1; index <= 10; index++)
    {
        switch(index)
        {
        case 1:
            gx_multi_line_text_input_text_set(text_input, test_string);
            gx_system_focus_claim((GX_WIDGET *)text_input);
            sprintf(test_comment, "set text and gain focus");
            break;

	case 2:
            gx_widget_focus_next((GX_WIDGET *)text_input);
            sprintf(test_comment, "move focus next");
            break;

	case 3:
            memset(&my_event, 0, sizeof(GX_EVENT));
            my_event.gx_event_display_handle = 1;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 205;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 310;
            gx_system_event_send(&my_event);
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "simulate click on (205, 310)");
            break;

        case 4:
            gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, GX_FONT_ID_TEXT_INPUT_8BPP_24PIX);
            sprintf(test_comment, "set font: 8bpp 24pix height");
            break;
   
        case 5:
            gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, GX_FONT_ID_TEXT_INPUT_8BPP_18PIX);
            sprintf(test_comment, "set font: 8bpp 18pix height");
            break;

        case 6:
            gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 5);
            sprintf(test_comment, "set whitespace to 5");
            break;

        case 7:
            gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 5);
            sprintf(test_comment, "set line space to 5");
            break;

        case 8:
            gx_widget_style_remove((GX_WIDGET *)text_input, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add((GX_WIDGET *)text_input, GX_STYLE_TEXT_CENTER);
            sprintf(test_comment, "set alignment center");
            break;

        case 9:
            gx_widget_style_remove((GX_WIDGET *)text_input, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add((GX_WIDGET *)text_input, GX_STYLE_TEXT_RIGHT);
            sprintf(test_comment, "set alignment right");
            break;

	case 10:
	    text_input->gx_multi_line_text_view_text.gx_string_ptr = input_buffer;
            text_input->gx_multi_line_text_input_buffer_size = 3;
            gx_multi_line_text_input_text_set(text_input, "1234");
            sprintf(test_comment, "set a new input buffer with size equal to 3, set a new text \"1234\"");
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    gx_validation_end();

    exit(0);
}
