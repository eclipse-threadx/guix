/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_sl_text_input_buffer_get", /* Test name */
    56, 196, 363, 231  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static char input_text[] = "single line text input";

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
int                        frame_id = 1;
GX_CHAR  *buffer_address = GX_NULL;
UINT      string_size = 0;
UINT      buffer_size = 0;

    ToggleScreen(pTextScreen, pButtonScreen);
    text_input_ptr = &((TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen) -> text_screen_text_input_1;

    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);
    gx_single_line_text_input_character_insert(text_input_ptr, (GX_UBYTE *)input_text, GX_STRLEN(input_text));

    sprintf(test_comment, "insert string: %s", input_text);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_single_line_text_input_buffer_get(text_input_ptr, &buffer_address, &string_size, &buffer_size);

    if((buffer_address != text_input_ptr->gx_single_line_text_input_buffer) ||
       (buffer_size != text_input_ptr->gx_single_line_text_input_buffer_size) ||
       (string_size != text_input_ptr->gx_single_line_text_input_string_size))
    {
        printf("Test Failed!");
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
