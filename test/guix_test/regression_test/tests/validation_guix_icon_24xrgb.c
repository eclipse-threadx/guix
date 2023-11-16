/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_icon_24xrgb", /* Test name */
    196, 411, 278, 458  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the icon
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
       on the icon drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual icon routine. */
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

#include "demo_guix_all_widgets.c"

static int pic_id[] = {GX_PIXELMAP_ID_SAVE_ICON, GX_PIXELMAP_ID_FISH, GX_PIXELMAP_ID_I_MEDICATIONSGREEN_LG, GX_PIXELMAP_ID_I_MEDICATIONSRED_LG};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int pic_index;
int frame_id = 1;
GX_ICON *icon;

    gx_widget_find(pButtonScreen, ID_ICON, 0, &icon);


    for(pic_index = 0; pic_index < (INT)(sizeof(pic_id) / sizeof(int)); pic_index++)
    {
        gx_icon_pixelmap_set(icon, pic_id[pic_index], GX_PIXELMAP_ID_SAVE_ICON);

        gx_validation_set_frame_id(frame_id);

        sprintf(test_comment, "change pixelmap ");

        gx_validation_set_frame_comment(test_comment);

        /* Mark the window "dirty" */
        gx_system_dirty_mark(pButtonScreen);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
