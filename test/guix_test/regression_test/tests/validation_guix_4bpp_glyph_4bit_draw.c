/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_4bpp_glyph_4bit_draw", /* Test name */
    -66, 18, 241, 42  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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
#ifdef win32_graphics_driver_setup_4bpp_grayscale
#undef win32_graphics_driver_setup_4bpp_grayscale
#endif
#define win32_graphics_driver_setup_4bpp_grayscale  gx_validation_graphics_driver_setup_4bpp_grayscale


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets_4bpp.c"

char test_comment[256];
static char test_string[]="ABCDEFGHIJK";

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_RECTANGLE size;
GX_PROMPT *prompt;

    ToggleScreen((GX_WINDOW *)&text_screen, (GX_WINDOW *)&sprite_screen);

    prompt = &text_screen.text_screen_prompt_3;

    gx_utility_rectangle_define(&size, -66, 18, 241, 42);

    /* Resize to make the prompt outside of window view. */
    gx_widget_resize((GX_WIDGET *)prompt, &size);

    /* Set test string. */
    gx_prompt_text_set(prompt, test_string);

    /* Set text font. */
    gx_prompt_font_set(prompt, GX_FONT_ID_FONT_4BIT);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("test 4bpp glyph 4bit draw");
    gx_validation_screen_refresh();
 
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

