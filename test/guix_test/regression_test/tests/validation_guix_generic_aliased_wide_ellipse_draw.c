/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_aliased_wide_ellipse_draw", /* Test name */
    24, 43, 370, 402  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_graphics_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
INT index;
GX_CANVAS *canvas;

    ToggleScreen((GX_WINDOW *)pEllipseWin, (GX_WINDOW *)pMainWin);
    
    draw_shape = ELLIPSE;
    brush_width = 10;
    anti_aliased = GX_TRUE;
 
    for(index = 0; index < 3; index++)
    {
        switch(index)
        {
        case 0:
            a = 100;
            b = 2;
            sprintf(test_comment, "brush_width = %d, a = %d, b = %d", brush_width, a, b);
            break;

        case 1:
            a = 100;
            b = 300;
            sprintf(test_comment, "brush_width = %d, a = %d, b = %d", brush_width, a, b);
            break;

        case 2:
           gx_widget_canvas_get(pEllipseWin, &canvas);
           canvas->gx_canvas_display->gx_display_driver_ellipse_fill = GX_NULL;
           sprintf(test_comment, "brush_width = %d, a = %d, b = %d, set gx_display_driver_ellipse_fill NULL", brush_width, a, b);
           break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
   
    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
