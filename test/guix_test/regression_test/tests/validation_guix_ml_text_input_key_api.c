/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_key_api", /* Test name */
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
GX_CONST GX_CHAR test_string[] = "first line\rsecond line";

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
int                       index;
GX_MULTI_LINE_TEXT_INPUT *text_input;

    text_input = &window.window_ml_text_input;

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(text_input, GX_STYLE_CURSOR_ALWAYS);
    
    gx_system_focus_claim(text_input);

    gx_multi_line_text_input_text_set(text_input, test_string);

    for(index = 0; index < 9; index++)
    {
        switch(index)
        {
        case 0:
            gx_multi_line_text_input_char_insert(text_input, (GX_UBYTE *)test_string, 1);
            sprintf(test_comment, "insert");
            break;

        case 1:
            gx_multi_line_text_input_home(text_input);
            sprintf(test_comment, "home");
            break;

        case 2:
            gx_multi_line_text_input_end(text_input);
            sprintf(test_comment, "end");
            break;

        case 3:
            gx_multi_line_text_input_backspace(text_input);
            sprintf(test_comment, "backspace");
            break;

        case 4:
            gx_multi_line_text_input_left_arrow(text_input);
            sprintf(test_comment, "left arrow");
            break;

        case 5:
            gx_multi_line_text_input_right_arrow(text_input);
            sprintf(test_comment, "right arrow");
            break;

        case 6:
            gx_multi_line_text_input_down_arrow(text_input);
            sprintf(test_comment, "down arrow");
            break;

        case 7:
            gx_multi_line_text_input_up_arrow(text_input);
            sprintf(test_comment, "up arrow");
            break;

        case 8:
            gx_multi_line_text_input_delete(text_input);
            sprintf(test_comment, "delete");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_validation_end();

    exit(0);
}
