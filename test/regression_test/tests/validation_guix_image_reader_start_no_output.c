/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_image_reader.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_start_no_output", /* Test name */
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

VOID *test_memory_allocate(ULONG size)
{
    VOID *memptr;

    if(size == 125808)
    {
        /* for compress. */
        return NULL;
    }

    if(size == 4728)
    {
        /* for dither. */
        return NULL;
    }

    if (tx_byte_allocate(&decode_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

VOID test_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

static VOID control_thread_entry(ULONG input)
{
int             failed_tests = 0;
UINT            status;
UINT            correct_status;
GX_PIXELMAP    *pixelmap;
GX_PIXELMAP     outmap;
int             outformat;
int             index;

    gx_widget_pixelmap_get(&window, GX_PIXELMAP_ID_BLACK_BAR_2_16400_JPG, &pixelmap);
    gx_image_reader_create(&image_reader, pixelmap->gx_pixelmap_data, pixelmap->gx_pixelmap_data_size, GX_COLOR_FORMAT_8BIT_ALPHAMAP, image_reader_mode);
    status = gx_image_reader_start(&image_reader, &outmap);
    EXPECT_EQ(GX_INVALID_HEIGHT, status);
    
    gx_widget_pixelmap_get(&window, GX_PIXELMAP_ID_BLACK_BAR_2_16400_PNG, &pixelmap);
    gx_image_reader_create(&image_reader, pixelmap->gx_pixelmap_data, pixelmap->gx_pixelmap_data_size, GX_COLOR_FORMAT_8BIT_ALPHAMAP, image_reader_mode);
    status = gx_image_reader_start(&image_reader, &outmap);
    EXPECT_EQ(GX_INVALID_HEIGHT, status);

    gx_widget_pixelmap_get(&window, GX_PIXELMAP_ID_RED_BAR_16400_2_JPG, &pixelmap);
    gx_image_reader_create(&image_reader, pixelmap->gx_pixelmap_data, pixelmap->gx_pixelmap_data_size, GX_COLOR_FORMAT_8BIT_ALPHAMAP, image_reader_mode);
    status = gx_image_reader_start(&image_reader, &outmap);
    EXPECT_EQ(GX_INVALID_WIDTH, status);
    
    gx_widget_pixelmap_get(&window, GX_PIXELMAP_ID_RED_BAR_16400_2_PNG, &pixelmap);
    gx_image_reader_create(&image_reader, pixelmap->gx_pixelmap_data, pixelmap->gx_pixelmap_data_size, GX_COLOR_FORMAT_8BIT_ALPHAMAP, image_reader_mode);
    status = gx_image_reader_start(&image_reader, &outmap);
    EXPECT_EQ(GX_INVALID_WIDTH, status);

    gx_widget_pixelmap_get((GX_WIDGET *)&window, GX_PIXELMAP_ID_RED_APPLE_PNG, &pixelmap);

    for(index = 0; index < 3; index++)
    {

        switch(index)
        {
        case 0:
            outformat = 0;
            correct_status = GX_NOT_SUPPORTED;
            break;
        
        case 1:
            image_reader_mode = GX_IMAGE_READER_MODE_COMPRESS|GX_IMAGE_READER_MODE_ALPHA;
            gx_system_memory_allocator_set(test_memory_allocate, test_memory_free);
            outformat = GX_COLOR_FORMAT_565RGB;
            correct_status = GX_SYSTEM_MEMORY_ERROR;
            break;

        case 2:
            /* Reset memory buffer with memory size not enough for color converti(dither). */
            image_reader_mode = GX_IMAGE_READER_MODE_ALPHA | GX_IMAGE_READER_MODE_DITHER;
            break;
        }           

        /* Output map format is not valid. */
        gx_image_reader_create(&image_reader,
            pixelmap->gx_pixelmap_data,
            pixelmap->gx_pixelmap_data_size,
            outformat, image_reader_mode);

        status = gx_image_reader_start(&image_reader, &outmap);
        EXPECT_EQ(correct_status, status);
    }

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
