/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_partial_frame_buffer_16bpp_rotate_screen", /* Test name */
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

#include "demo_guix_partial_frame_buffer_16bpp.c"

typedef struct MAP_INFO_STRUCT{
GX_RESOURCE_ID id;
GX_CHAR *name;
GX_BOOL canvas_rotate;
}MAP_INFO;

MAP_INFO resize_pixelmap_id_list[]={
    {GX_PIXELMAP_ID_ROTATE_FOOT, "GX_PIXELMAP_ID_ROTATE_FOOT", GX_FALSE},
    {GX_PIXELMAP_ID_ROTATE_FOOT, "GX_PIXELMAP_ID_ROTATE_FOOT", GX_TRUE},
    {GX_PIXELMAP_ID_ROTATE_FISH, "GX_PIXELMAP_ID_ROTATE_FISH", GX_FALSE},
    {GX_PIXELMAP_ID_ROTATE_FISH, "GX_PIXELMAP_ID_ROTATE_FISH", GX_TRUE},
    {0, GX_NULL, GX_FALSE}
};

extern INT     rotate_angle;
extern INT     rotate_pixelmap_id;
extern GX_BOOL canvas_rotate;

static char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
MAP_INFO *entry = resize_pixelmap_id_list;

    ToggleScreen((GX_WINDOW *)&rotate_screen, &button_screen);

    while(entry->id)
    {
        rotate_pixelmap_id = entry->id;
        canvas_rotate = entry->canvas_rotate;

        for(rotate_angle = 1; rotate_angle < 360; rotate_angle++)
        {
            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "canvas rotate: %d, angle: %d, pixelmap_id: %d. ", canvas_rotate, rotate_angle, rotate_pixelmap_id);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_write_frame_buffer();
        }

        entry++;
    }
   
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





