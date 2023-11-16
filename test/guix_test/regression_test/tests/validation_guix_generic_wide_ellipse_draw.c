/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_wide_ellipse_draw", /* Test name */
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
int index;

    anti_aliased = GX_FALSE;
    draw_shape = ELLIPSE;
    pixelmap_fill = GX_FALSE;
    brush_width = 10;

    for(index = 0; index < 6; index ++)
    {
        switch(index)
        {
        case 0:
            xcenter = 213;
            ycenter = 230;
            a = 100;
            b = 5;
            break;

        case 1:
            xcenter = 213;
            ycenter = 400;
            a = 200;
            b = 100;
            break;

        case 2:
            xcenter = 213;
            ycenter = 230;
            a = 67;
            b = 275;
            brush_width = 10;
            break;

        case 3:
            a = brush_width;
            b = (brush_width - 1) >> 1;
            break;

        case 4:
            xcenter = 213;
            ycenter = 628;
            a = 100;
            b = 200;
            brush_width = 10;
            break;

        case 5:
            ycenter = -176;
            break;
        }

        sprintf(test_comment,  "xcenter = %d, ycenter = %d, a = %d, b = %d, brush_width = %d",
                xcenter, ycenter, a, b, brush_width);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
