/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_24xrgb_rotated_cw_sprite", /* Test name */
    40, 96, 139, 175  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the graphics
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
#ifdef win32_graphics_driver_setup_24xrgb_rotated  
#undef win32_graphics_driver_setup_24xrgb_rotated
#endif
#define win32_graphics_driver_setup_24xrgb_rotated  gx_validation_graphics_driver_setup_24xrgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_24xrgb_rotated_cw.c"

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int        frame_id = 1;
char      *comment[1];

    comment[0] = &test_comment[0];
    sprintf(test_comment, "toggle to sprite screen, and capture 7 frame.");

    gx_validation_capture_frames(frame_id, 7, comment, 1, 1000);
    ToggleScreen((GX_WINDOW *)&sprite_screen, (GX_WINDOW *)&button_screen);
    gx_validation_capture_frames_wait(); 

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
