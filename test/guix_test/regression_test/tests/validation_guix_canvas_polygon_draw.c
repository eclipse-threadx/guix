/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_polygon_draw", /* Test name */
    28, 24, 400, 430  /* Define the coordinates of the capture area.
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
#include "demo_guix_graphics_32bpp.c"

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int left, right, top, bottom;
GX_RECTANGLE size;
GX_RECTANGLE original_size;
static GX_WINDOW test_window;

    draw_shape = POLYGON;
    original_size =  pGraphicsWin->gx_widget_size;
    left = pGraphicsWin->gx_widget_size.gx_rectangle_left;
    right = pGraphicsWin->gx_widget_size.gx_rectangle_right;
    top = pGraphicsWin->gx_widget_size.gx_rectangle_top;
    bottom = pGraphicsWin->gx_widget_size.gx_rectangle_bottom;
    
    while(left<= right)
    {
        left += 5;
        top += 10;
        bottom -= 10;
        right -= 20;
        
        size.gx_rectangle_left = left;
        size.gx_rectangle_right = right;
        size.gx_rectangle_top = top;
        size.gx_rectangle_bottom = bottom;
        
        gx_widget_resize(pGraphicsWin, &size);
        
        gx_validation_set_frame_id(frame_id);
        sprintf(test_comment, "Resize graphics window:(%d,%d,%d,%d)",
                left,top,right,bottom);

        gx_validation_set_frame_comment(test_comment);
                    
        /* Mark the window "dirty" */
        gx_system_dirty_mark(pMainWin);
                    
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
                    
        /* Increment frame ID */
        frame_id ++;
    }
    
    gx_widget_resize(pGraphicsWin, &original_size);
    gx_utility_rectangle_define(&size, original_size.gx_rectangle_left + 30,
                                original_size.gx_rectangle_top + 30, 
                                original_size.gx_rectangle_right + 30,
                                original_size.gx_rectangle_bottom + 30);
    gx_window_create(&test_window, "test_window", root, 0, 0, &size);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Create a window to cover the graphics window then draw polygon.");

    gx_validation_set_frame_comment(test_comment);
                        
    /* Mark the window "dirty" */
    gx_system_dirty_mark(pMainWin);
                        
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
