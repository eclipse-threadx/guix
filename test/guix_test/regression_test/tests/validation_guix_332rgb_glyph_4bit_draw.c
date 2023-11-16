/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_332rgb_glyph_4bit_draw", /* Test name */
    0, 0, 114, 29  /* Define the coordinates of the capture area.
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
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_332rgb  
#undef win32_graphics_driver_setup_332rgb  
#endif
#define win32_graphics_driver_setup_332rgb  gx_validation_graphics_driver_setup_332rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_all_widgets_332rgb.c"

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
GX_PROMPT   *prompt = &button_screen.button_screen_prompt_11;
GX_RECTANGLE size;

    gx_utility_rectangle_define(&size,  -16, 6, 114, 29);
    gx_widget_resize(prompt, &size);

    ToggleScreen(pButtonScreen, pShapesScreen);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("toggle to button_screen");
    gx_validation_screen_refresh();

    gx_prompt_font_set(prompt, GX_FONT_ID_FONT_4BPP);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set 4bit font");
    gx_validation_screen_refresh();

    gx_widget_style_add(prompt, GX_STYLE_USE_LOCAL_ALPHA);
    prompt->gx_widget_alpha = 128;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set button alpha = 128");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





