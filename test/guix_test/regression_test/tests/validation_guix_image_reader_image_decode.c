/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_image_decode", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
                    In this test, we only need to capture the prompt
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
    on the prompt drawing example. These operations simulate
    user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual prompt routine. */
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
#include "demo_guix_color_convert_32bpp.c"

char test_comment[256];

GX_UBYTE test_data_0[]={0x00, 0xd8, 0xff};
GX_UBYTE test_data_1[]={0xff, 0x00, 0xff};
GX_UBYTE test_data_2[]={0xff, 0xd8, 0x00};
GX_UBYTE *test_data = 0;

VOID test_pixelmap_draw(GX_WINDOW *window)
{
    gx_window_draw(window);

    gx_image_reader_create(&image_reader, test_data, 100, GX_COLOR_FORMAT_32ARGB, 0);
    image_reader.gx_image_reader_source_data = test_data;

    if(gx_image_reader_start(&image_reader, &pic_outmap) == GX_SUCCESS)
    {
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left + 10,
                                window->gx_widget_size.gx_rectangle_top + 10,
                                &pic_outmap);

        if (pic_outmap.gx_pixelmap_data)
        {
            _gx_system_memory_free((VOID *)pic_outmap.gx_pixelmap_data);
            pic_outmap.gx_pixelmap_data = GX_NULL;
        }

        if (pic_outmap.gx_pixelmap_aux_data)
        {
            _gx_system_memory_free((VOID *)pic_outmap.gx_pixelmap_aux_data);
            pic_outmap.gx_pixelmap_aux_data = GX_NULL;
        }
    }    
}

static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
int index;

    gx_widget_draw_set(&window.window_pixelmap_window, test_pixelmap_draw);
 
    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
           test_data = test_data_0;
           break;

        case 1:
           test_data = test_data_1;
           break;

        case 2:
           test_data = test_data_2;
           break;

        case 3:
           test_data = 0;
           break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("convert with invalid map format");
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
