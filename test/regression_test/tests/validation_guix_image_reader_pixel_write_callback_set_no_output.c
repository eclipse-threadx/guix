/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_image_reader.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_pixel_write_callback_set_no_output", /* Test name */
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
#include "demo_guix_color_convert_16bpp.c"

#define TEST_MEMORY_SIZE_1 312657
#define TEST_MEMORY_SIZE_2 364722

GX_UBYTE test_memory_1[TEST_MEMORY_SIZE_1];
GX_UBYTE test_memory_2[TEST_MEMORY_SIZE_2];

static VOID control_thread_entry(ULONG input)
{
INT             failed_tests = 0;
GX_IMAGE_READER image_reader;
GX_PIXELMAP    *map;
GX_PIXELMAP     outmap;
int             index;
UINT            status;

    gx_widget_pixelmap_get(&window, GX_PIXELMAP_ID_RED_APPLE_PNG, &map);
    status = gx_image_reader_create(&image_reader,
                                    map->gx_pixelmap_data,
                                    map->gx_pixelmap_data_size,
                                    GX_COLOR_FORMAT_4444ARGB,
                                    GX_IMAGE_READER_MODE_ALPHA | GX_IMAGE_READER_MODE_ROTATE_CW );
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_image_reader_start(&image_reader, &outmap);
    EXPECT_EQ(GX_NOT_SUPPORTED, status);

    for(index = 0; index < 2; index++)
    {
        if(index == 0)
        {    
            tx_byte_pool_delete(&decode_pool);
            tx_byte_pool_create(&decode_pool, "test_memory_1", test_memory_1, TEST_MEMORY_SIZE_1);
        }
        else
        {
            tx_byte_pool_delete(&decode_pool);
            tx_byte_pool_create(&decode_pool, "test_memory_2", test_memory_2, TEST_MEMORY_SIZE_2);
        }

        gx_widget_pixelmap_get(&window, GX_PIXELMAP_ID_RED_APPLE_PNG, &map);
        gx_image_reader_create(&image_reader,
                               map->gx_pixelmap_data,
                               map->gx_pixelmap_data_size,
                               GX_COLOR_FORMAT_565RGB,
                               GX_IMAGE_READER_MODE_ALPHA);

        if(gx_image_reader_start(&image_reader, &outmap) == GX_SUCCESS)
        {
            failed_tests++;
        }
    }

    outmap.gx_pixelmap_format = 0;
    status = _gx_image_reader_pixel_write_callback_set(&image_reader, &outmap);
    EXPECT_EQ(GX_NOT_SUPPORTED, status);

    if(failed_tests == 0)
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
