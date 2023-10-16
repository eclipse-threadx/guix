/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_wide_arc_draw", /* Test name */
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

    draw_shape = ARC;
    brush_width = 3;
    anti_aliased = GX_TRUE;

    for(index = 0; index < 9; index++)
    {
        switch(index)
        {
        case 0:
            radius = 300;
            start_angle = 0;
            end_angle = 90;
            break;
        case 1:
            radius = 230;
            start_angle = 0;
            end_angle = -90;
            break;
        case 2:
            anti_aliased = GX_FALSE;
            start_angle = 49;
            end_angle = 41;
            brush_width = 1;
            break;

         case 3:
            brush_alpha = 128;
            start_angle = 131;
            end_angle = 124;
            pixelmap_fill = GX_TRUE;
            break;

         case 4:
            radius = 100;
            start_angle = 170;
            end_angle = 160;
            break;

         case 5:
            anti_aliased = GX_FALSE;
            start_angle = 0;
            end_angle = 90;
            radius = 129;
            brush_width = 3;
            break;

         case 6:
            radius = 230;
            break;

         case 7:
            anti_aliased = GX_TRUE;
            radius = 100;
            start_angle = 0;
            end_angle = 4;
            brush_width = 20;
            xcenter = 213;
            ycenter = 30;
            break;

         case 8:
            anti_aliased = GX_FALSE;
            break;
        }
        sprintf(test_comment, "anti_aliased = %d, brush_width = %d, radius = %d, start_angle = %d, end_angle = %d, pixelmap_fill = %d, fill_map = %d",  
                               anti_aliased, brush_width, radius, start_angle, end_angle, pixelmap_fill, (int)pixelmap_id[pixelmap_index]);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
