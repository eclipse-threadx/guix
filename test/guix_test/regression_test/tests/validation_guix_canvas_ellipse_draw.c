/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_ellipse_draw", /* Test name */
    10, 9, 401, 428  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);
 
    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_monochrome
#undef win32_graphics_driver_setup_monochrome
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1bpp.c"

char test_comment[256];
extern INT draw_shape;
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_RECTANGLE size;

    /* part1, original screen. */
    gx_widget_detach(pSpriteScreen);
    gx_widget_attach(root, pShapesScreen);
    draw_shape = 4; //4 is ellipse

    /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set brush properties and draw original ellipse.");
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(pShapesScreen);
    gx_validation_screen_refresh();
    
    /* part2, draw area covered by others. */
    while(pSpriteScreen->gx_widget_first_child)
    {
        gx_widget_detach(pSpriteScreen->gx_widget_first_child);
    }
    
    size = shapes_screen.shapes_screen_graphics_window.gx_widget_size;
    size.gx_rectangle_left += 20;
    size.gx_rectangle_right += 20;
    size.gx_rectangle_top += 20;
    size.gx_rectangle_bottom += 20;

    gx_widget_resize(pSpriteScreen, &size);
    gx_widget_attach(root, pSpriteScreen); 
    
    /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Add a window to cover view where the ellipse should be draw.");
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(pShapesScreen);
    gx_validation_screen_refresh();
    
    /* part3, draw area outside the screen. */
    gx_widget_detach(pSpriteScreen);
    size = shapes_screen.shapes_screen_graphics_window.gx_widget_size;
    size.gx_rectangle_left += 380;
    size.gx_rectangle_top += 410;
    gx_widget_resize(pShapesScreen, &size);
    
    /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Draw ellipse outside the window client.");
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(pShapesScreen);
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
