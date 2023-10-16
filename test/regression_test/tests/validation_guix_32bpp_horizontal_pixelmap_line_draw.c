/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_32bpp_horizontal_pixelmap_line_draw", /* Test name */
    28, 24, 398, 426  /* Define the coordinates of the capture area.
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

static int map_id_list[]={
GX_PIXELMAP_ID_CHECKBOX_OFF_RAW,/*raw*/
GX_PIXELMAP_ID_CHECKBOX_OFF,/*compressed*/
GX_PIXELMAP_ID_RADIOBUTTON_ALPHA,/*alpha*/
GX_PIXELMAP_ID_RADIOBUTTON_ALPHA_COMPRESS,/*compressed, alpha*/
};

char *map_string_list[]=
{
    "raw",
    "compressed",
    "alpha",
    "alpha, compressed"
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int alpha_index;

    draw_shape = PIE;
    start_angle = 139;
    end_angle = 218;
    radius = 317;
    pixelmap_fill = GX_TRUE;

    pixelmap_id[0] = map_id_list[0];
    pixelmap_id[1] = map_id_list[1];
    pixelmap_id[2] = map_id_list[2];
    pixelmap_id[3] = map_id_list[3];

    for(alpha_index = 0; alpha_index < 2; alpha_index++)
    {
        if(alpha_index)
        {
            brush_alpha = 255;
        }
        else
        {
            brush_alpha = 128;
        }

        for(pixelmap_index = 0; pixelmap_index < 4; pixelmap_index++)
        {
            sprintf(test_comment, "alpha = %d, pixelmap = %s", brush_alpha, map_string_list[pixelmap_index]);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);   
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
