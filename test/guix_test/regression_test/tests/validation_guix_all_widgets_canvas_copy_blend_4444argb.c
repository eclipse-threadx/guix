/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_canvas_copy_blend_4444argb", /* Test name */
    55, 43, 343, 184  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_4444argb  
#undef win32_graphics_driver_setup_4444argb  
#endif
#define win32_graphics_driver_setup_4444argb  gx_validation_graphics_driver_setup_4444argb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets_4444argb.c"

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int blend_alpha;

    for(blend_alpha = 0; blend_alpha < 256 ; blend_alpha++ )
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        ToggleScreen(pWindowScreen, pRotateScreen);

        gx_canvas_alpha_set(&animation_canvas, blend_alpha);

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "blend_alpha=%d", blend_alpha);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

    }
 
    gx_canvas_alpha_set(&animation_canvas, 128);
    gx_canvas_offset_set(&animation_canvas, -animation_canvas.gx_canvas_x_resolution, 0);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set animation canvas alpha t0 128, shift animation canvas left");
    gx_validation_screen_refresh();
   
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

