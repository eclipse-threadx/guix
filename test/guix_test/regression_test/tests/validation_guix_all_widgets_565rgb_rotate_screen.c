/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_565rgb_rotate_screen", /* Test name */
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

#include "demo_guix_all_widgets_16bpp.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/

extern INT     angle;
extern GX_BOOL direct;
extern INT     rotate_pixelmap_index;

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;

    ToggleScreen(pRotateScreen, pButtonScreen);

    for(rotate_pixelmap_index = 1; rotate_pixelmap_index < 3; rotate_pixelmap_index++)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
         */
        for(angle = 0; angle < 360; angle++)
        {
            for(index = 0; index < 2; index++)
            {
                if(index)
                {
                    direct = GX_TRUE;
                }
                else
                {
                    direct = GX_FALSE;
                }
            
                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "canvas rotate: %d, angle: %d, rotate_pixelmap_index: %d. ", direct, angle, rotate_pixelmap_index);
                gx_validation_set_frame_comment(test_comment);

                gx_system_dirty_mark(pRotateScreen);

                /* Force a screen refresh. */
                gx_validation_screen_refresh();
            }
        }
    }

    rotate_pixelmap_index = 3;
    direct = GX_TRUE;
    angle = 30;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set alphamap");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





