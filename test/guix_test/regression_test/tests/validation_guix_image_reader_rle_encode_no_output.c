/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_image_reader.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_rle_encode_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_color_convert_32bpp.c"

UINT test_pixel_read(GX_IMAGE_READER *image_reader, INT index, GX_PIXEL *pixel)
{
    pixel->gx_pixel_red = 0;
    pixel->gx_pixel_green = 0;
    pixel->gx_pixel_blue = 0;
    pixel->gx_pixel_alpha = 0;

    return GX_SUCCESS;
}

UINT test_pixel_write(GX_IMAGE_READER *image_reader, GX_PIXEL *pixel)
{
    return GX_SUCCESS;
}

static VOID control_thread_entry(ULONG input)
{
GX_BOOL         test_successed = GX_TRUE;
UINT            status;
GX_IMAGE_READER image_reader;
GX_PIXELMAP     outmap;
INT             index;

    memset(&image_reader, 0, sizeof(GX_IMAGE_READER));

    image_reader.gx_image_reader_color_format = 0;
    image_reader.gx_image_reader_image_width = 200;
    image_reader.gx_image_reader_image_height = 2;
    image_reader.gx_image_reader_pixel_read = test_pixel_read;
    image_reader.gx_image_reader_pixel_write = test_pixel_write;

    outmap.gx_pixelmap_width = 200;
    outmap.gx_pixelmap_height = 2;

    for(index = 0; index < 3; index++)
    {    
        switch(index)
        {
        case 0:
            status = _gx_image_reader_rle_encode(&image_reader, &outmap);
            break;

        case 1:
            image_reader.gx_image_reader_image_width = 2;
            outmap.gx_pixelmap_width = 2;
            status = _gx_image_reader_rle_encode(&image_reader, &outmap);
            break;

        case 2:
            status = _gx_image_reader_rle_encode_size_get(&image_reader, GX_NULL, GX_NULL);
            break;
        }
 
        if(status != GX_NOT_SUPPORTED)
        {
            test_successed = GX_FALSE;       
        }
    }

    if(test_successed)
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
