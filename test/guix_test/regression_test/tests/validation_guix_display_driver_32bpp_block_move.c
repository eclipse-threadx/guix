/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"


TEST_PARAM test_parameter = {
    "guix_display_driver_32bpp_block_move", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"
char test_comment[255];
static INT xshift;
static VOID text_button_draw(GX_TEXT_BUTTON *button)
{
    gx_text_button_draw(button);
    
    gx_canvas_block_move(&button->gx_widget_size, xshift, 0, 0);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT          frame_id = 1;
int          index =0;
GX_TEXT_BUTTON *button;

    button = &button_screen.button_screen_text_button_1;
    gx_widget_draw_set(button, (VOID (*)(GX_WIDGET *))text_button_draw);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Original canvas data (button screen).");
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(pButtonScreen);
    gx_validation_screen_refresh();
    /* Set frame id. */
    xshift = 0;
    while(index++ < 2)
    {
        xshift += 100;
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "Call gx_canvas_block_move to shift, value: %d", xshift);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark(pButtonScreen);
        gx_validation_screen_refresh();
    }

    index = 0;
    xshift = 0;
    while(index++ < 2)
    {
        xshift -= 100;
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "Call gx_canvas_block_move to shift, value: %d", xshift);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark(pButtonScreen);
        gx_validation_screen_refresh();
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
