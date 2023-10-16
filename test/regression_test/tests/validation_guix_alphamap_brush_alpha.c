/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_alphamap_brush_alpha", /* Test name */
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_alphamap_16bpp.c"

char test_comment[256];


extern GX_UBYTE alpha_value;
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_WINDOW *window1;
GX_WINDOW *window2;
GX_RECTANGLE size;
int alpha;

    window1 = &main_window.main_window_window_1;
    window2 = &main_window.main_window_window_2;
    
    size = window1->gx_widget_size;
    size.gx_rectangle_top = main_window.gx_widget_size.gx_rectangle_top - 10;
    gx_widget_resize(window1, &size);
    size = window2->gx_widget_size;
    size.gx_rectangle_top = main_window.gx_widget_size.gx_rectangle_top - 10;
    gx_widget_resize(window2, &size);
    
    for(alpha = 255; alpha >= 0; alpha -= 51)
    {
         gx_validation_set_frame_id(frame_id);
         alpha_value = alpha;
         printf(test_comment, "alpha_value: %d ", alpha_value);
         gx_validation_set_frame_comment(test_comment);

         /* Mark the window "dirty" */
         gx_system_dirty_mark((GX_WIDGET *)&main_window);

         /* Force a screen refresh. */
         gx_validation_screen_refresh();

         /* Increment frame ID */
         frame_id ++;
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





