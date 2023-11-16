/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1bpp_rotate_screen", /* Test name */
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
#ifdef win32_graphics_driver_setup_monochrome
#undef win32_graphics_driver_setup_monochrome
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1bpp.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/

GX_RESOURCE_ID pixelmap_id_list[] = {
    GX_PIXELMAP_ID_RED_APPLE,
    GX_PIXELMAP_ID_ROTATE_FISH
};

extern GX_RESOURCE_ID rotate_pixelmap_id;
extern INT angle;
extern GX_BOOL direct;

char test_comment[256];
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int pixelmap;
int direct_index;

    ToggleScreen(pRotateScreen, pSpriteScreen);

    for(pixelmap = 0; pixelmap < 2; pixelmap++)
    {
        rotate_pixelmap_id = pixelmap_id_list[pixelmap];

        for(angle= 0; angle < 360; angle++)
        {
            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            for(direct_index = 0; direct_index < 2; direct_index++)
            {
                if(direct_index)
                {
                   direct = GX_TRUE;
                }
                else
                {
                    direct = GX_FALSE;
                }
    
                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "canvas rotate = %d, pixelmap_id = %d, angle = %d", direct, (int)rotate_pixelmap_id, angle);
                gx_validation_set_frame_comment(test_comment);
                gx_system_dirty_mark(pRotateScreen);

                /* Force a screen refresh. */
                gx_validation_screen_refresh();
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
