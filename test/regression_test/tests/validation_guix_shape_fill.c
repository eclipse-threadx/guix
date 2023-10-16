/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_shape_fill", /* Test name */
    28, 24, 400, 430  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_graphics_32bpp.c"

char test_comment[255];

GX_CONST UINT test_pixelmap_data[256];

GX_CONST GX_PIXELMAP test_pixelmap_1=
{
    0x00000001,
    0x00000000,
    0,
    GX_COLOR_FORMAT_32ARGB,
    (GX_UBYTE *)test_pixelmap_data,
    sizeof(test_pixelmap_data),
    NULL,
    0,
    0x00,
    16,
    0
};

GX_CONST GX_PIXELMAP test_pixelmap_2=
{
    0x00000001,
    0x00000000,
    0,
    GX_COLOR_FORMAT_32ARGB,
    (GX_UBYTE *)test_pixelmap_data,
    sizeof(test_pixelmap_data),
    NULL,
    0,
    0x00,
    0,
    16
};

GX_CONST GX_PIXELMAP *test_pixelmap_table[]=
{
    GX_NULL,
    &test_pixelmap_1,
    &test_pixelmap_2
};

#define TEST_MAP_ID_1 1
#define TEST_MAP_ID_2 2

INT shape_ids[]=
{
CIRCLE,
ARC,
PIE,
POLYGON,
ELLIPSE
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT index;

    gx_display_pixelmap_table_set(root->gx_window_root_canvas->gx_canvas_display, (GX_PIXELMAP **)test_pixelmap_table, 3);

    pixelmap_id[0] = TEST_MAP_ID_1;
    pixelmap_id[1] = TEST_MAP_ID_2;

    solid_fill = GX_TRUE;
    pixelmap_fill = GX_TRUE;

    for(pixelmap_index = 0; pixelmap_index < 2; pixelmap_index++)
    {
        for(index = 0; index < sizeof(shape_ids)/sizeof(int); index++)
        {
            draw_shape = shape_ids[index];

            sprintf(test_comment, "pixelmap id = %d, shape id = %d", pixelmap_index, draw_shape);

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
