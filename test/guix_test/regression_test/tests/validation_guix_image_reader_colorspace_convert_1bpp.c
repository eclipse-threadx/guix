/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_colorspace_convert_1bpp", /* Test name */
    60, 75, 590, 365  /* Define the coordinates of the capture area.
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

char test_comment[256];

INT map_id_list[]={
    GX_PIXELMAP_ID_CHECKBOX_ON_PNG,
    GX_PIXELMAP_ID_WHITEBOX_OPACITY_100_PNG,
    GX_PIXELMAP_ID_BLACKBOX_OPACITY_100_PNG,
    GX_PIXELMAP_ID_BLACKBOX_OPACITY_30_PNG
};

extern GX_PIXELMAP pixelmap_memory[];
extern GX_PIXELMAP *png_to_1bpp_map;
extern GX_IMAGE_READER  image_reader;
extern GX_UBYTE image_reader_mode;
extern void cleanup_output_pixelmaps();

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
GX_PIXELMAP *pixelmap;
int          index;

    /* Toggle to Image Convert Screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, pSpriteScreen);

    for(index = 0; index < (int)(sizeof(map_id_list)/sizeof(INT)); index++)
    {
        cleanup_output_pixelmaps();

        png_to_1bpp_map = &pixelmap_memory[0];

        gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen, map_id_list[index], &pixelmap);

        gx_image_reader_create(&image_reader,
            pixelmap->gx_pixelmap_data,
            pixelmap->gx_pixelmap_data_size,
            GX_COLOR_FORMAT_MONOCHROME, image_reader_mode);

        if (gx_image_reader_start(&image_reader, png_to_1bpp_map) != GX_SUCCESS)
        {
            png_to_1bpp_map = GX_NULL;
        }

        sprintf(test_comment, "map_id = %d", map_id_list[index]);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }


    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
