/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_widget_block_move", /* Test name */
    24, 10, 578, 464  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

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

static GX_CHAR test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_DISPLAY *display;
GX_WINDOW *window;
GX_RECTANGLE size;

    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pWindowScreen);
    gx_widget_hide((GX_WIDGET *)&nested_root);
    gx_canvas_delete(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Original window screen");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();    
    
    display = root->gx_window_root_canvas->gx_canvas_display;
       
    window = &window_screen.window_screen_window_4;
    size = window->gx_widget_size;
    size.gx_rectangle_right = window_screen.gx_widget_size.gx_rectangle_right;
    size.gx_rectangle_bottom = window_screen.gx_widget_size.gx_rectangle_bottom;
    gx_validation_set_frame_id(frame_id++);
    gx_widget_block_move(window, &size, 10, 10);
   
    sprintf(test_comment, "Call gx_widget_block_move, xshift: 10, yshift: 10.");
    gx_validation_set_frame_comment(test_comment);
    
    gx_widget_resize(window, &size);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Resize window_4 to original size.");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    gx_validation_set_frame_id(frame_id++);
    display -> gx_display_driver_block_move = 0;
    /* block move driver is null, so no screen will be captured here.*/
    gx_widget_block_move(window, &size, 10, 10);
   
    sprintf(test_comment, "Set gx_display_driver_block_move NULL and call block move again:  xshift: 10, yshift: 10.");
    gx_validation_set_frame_comment(test_comment);
        
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
