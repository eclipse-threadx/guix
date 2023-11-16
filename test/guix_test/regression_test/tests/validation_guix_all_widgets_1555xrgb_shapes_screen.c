/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1555xrgb_shapes_screen", /* Test name */
    10, 10, 400, 402  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_1555xrgb  
#undef win32_graphics_driver_setup_1555xrgb
#endif
#define win32_graphics_driver_setup_1555xrgb  gx_validation_graphics_driver_setup_1555xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1555xrgb.c"

extern GX_RESOURCE_ID pixelmap_id[];
extern INT fill_pixelmap_index;
extern GX_BOOL pixelmap_fill;

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT        frame_id = 1;
GX_WINDOW *pGraphicsWin = &(shapes_screen.shapes_screen_graphics_window);

    /* Arc */
    ToggleScreen(pShapesScreen, (GX_WINDOW *)&rotate_screen);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Test raw color set.");
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(pGraphicsWin);
    gx_validation_screen_refresh();

    pixelmap_fill = GX_TRUE;

    for(fill_pixelmap_index = 0; fill_pixelmap_index < 4; fill_pixelmap_index++)
    {
        switch(pixelmap_id[fill_pixelmap_index])
        {
            case GX_PIXELMAP_ID_ID_RAW:
                sprintf(test_comment, "Test shape pixelmap fill: without alpha, without compression");
                break;
            case GX_PIXELMAP_ID_ID_ALPHA:
                sprintf(test_comment, "Test shape pixelmap fill: with alpha, without compression");
                break;
            case GX_PIXELMAP_ID_ID_COMPRESS:
                sprintf(test_comment, "Test shape pixelmap fill: without alpha, with compression");
                break;
            case GX_PIXELMAP_ID_ID_COMPRESS_ALPHA:
                sprintf(test_comment, "Test shape pixelmap fill: with alpha, with compression");
                break;
        }
        gx_system_dirty_mark(pGraphicsWin);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
