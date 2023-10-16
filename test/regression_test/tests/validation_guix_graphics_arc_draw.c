/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_graphics_arc_draw", /* Test name */
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
int index;
int alias;

    draw_shape = ARC;
    pixelmap_fill = GX_FALSE;
    brush_width = 5;

    for(alias = 0; alias < 2; alias ++)
    {
        if(alias)
        {
            anti_aliased = GX_TRUE;
        }
        else
        {
            anti_aliased = GX_FALSE;
        }

        for(index = 0; index < 6; index ++)
        {
            switch(index)
            {
            case 0:
                xcenter = 213;
                ycenter = 230;
                start_angle = 90;
                end_angle = 270;
                radius = 213;
                break;

            case 1:
                start_angle = 90;
                end_angle = 330;
                radius = 254;
                break;

            case 2:
                start_angle = 38;
                end_angle = 360;
                radius = 1792;
                break;

            case 3:
                start_angle = 326;
                end_angle = 124;
                radius = 400;
                break;

            case 4:
                xcenter = 400;
                ycenter = 500;
                start_angle = 0;
                end_angle = 83;
                radius = 88;
                break;

            case 5:
                xcenter = 213;
                ycenter = 230;
                start_angle = 350;
                end_angle = 640;
                radius = 254;
                break;
            }

            sprintf(test_comment, "alias = %d, xcenter = %d, ycenter = %d, start_angle = %d, end_angle = %d, radius = %d",
                    alias, xcenter, ycenter, start_angle, end_angle, radius);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
