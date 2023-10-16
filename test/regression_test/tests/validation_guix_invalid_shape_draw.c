/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_invalid_shape_draw", /* Test name */
    28, 24, 400, 425  /* Define the coordinates of the capture area.
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

#include "demo_guix_graphics_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index = 0;

    solid_fill = GX_TRUE;

    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
            draw_shape = CIRCLE;
            radius = 1500;
            sprintf(test_comment, "draw circle with radius = %d", radius);
            break;

        case 1:
            draw_shape = ARC;
            radius = 1500;
            sprintf(test_comment, "draw arc with radius = %d", radius);
            break;

        case 2:
            draw_shape = PIE;
            radius = 1500;
            sprintf(test_comment, "draw pie with radius = %d", radius);
            break;

        case 3:
            draw_shape = ELLIPSE;
            b = 1500;
            sprintf(test_comment, "draw ellipse with b = %d", radius);
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }    

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





