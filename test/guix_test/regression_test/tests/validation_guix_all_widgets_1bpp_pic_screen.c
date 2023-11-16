/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1bpp_pic_screen", /* Test name */
   55, 40, 455, 445  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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

static VOID control_thread_entry(ULONG input)
{
int pixelmap_id_index;
int frame_id = 1;


    ToggleScreen(pPicScreen, pSpriteScreen);

    for(pixelmap_id_index = 0; pixelmap_id_index < (INT)(sizeof(pixelmap_id) / sizeof(int)); pixelmap_id_index++)
    {
        /* Inform the validation system 
       (1) Frame ID, which identifies a specific test configuration;
       (2) Start recording frame on the next toggle operation.
       */
       gx_validation_set_frame_id(frame_id);

       sprintf(test_comment, "id: %d", pixelmap_id_index);

       gx_validation_set_frame_comment(test_comment);
       gx_window_wallpaper_set(pic_window, pixelmap_id[pixelmap_id_index], GX_FALSE);

       /* Force a screen refresh. */
       gx_validation_screen_refresh();

       /* Increment frame ID */
       frame_id ++;



    }


    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}









