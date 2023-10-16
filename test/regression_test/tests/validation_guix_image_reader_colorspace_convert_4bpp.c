/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_colorspace_convert_4bpp", /* Test name */
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
#ifdef win32_graphics_driver_setup_4bpp_grayscale
#undef win32_graphics_driver_setup_4bpp_grayscale
#endif
#define win32_graphics_driver_setup_4bpp_grayscale  gx_validation_graphics_driver_setup_4bpp_grayscale


#ifdef WIN32
#undef WIN32
#endif

#define TEST_MEMORY_SIZE 25000
GX_COLOR       test_memory[TEST_MEMORY_SIZE];
TX_BYTE_POOL   test_memory_pool;

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_4bpp.c"

char test_comment[256];

extern GX_PIXELMAP pixelmap_memory[];
extern GX_PIXELMAP *png_to_4bit_gray_map;
extern GX_PIXELMAP *jpeg_to_4bit_gray_map;
extern GX_IMAGE_READER  image_reader;

VOID *test_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&test_memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID test_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

static void test_png_jpeg_to_4bit_gray_convert()
{
GX_PIXELMAP *pixelmap;

    png_to_4bit_gray_map = &pixelmap_memory[0];
    jpeg_to_4bit_gray_map = &pixelmap_memory[1];

    gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen.image_convert_screen_png_to_4bit_gray_win,
        GX_PIXELMAP_ID_RADIOBUTTON_OFF_PNG, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        GX_COLOR_FORMAT_4BIT_GRAY, GX_IMAGE_READER_MODE_ALPHA);

    if (gx_image_reader_start(&image_reader, png_to_4bit_gray_map) != GX_SUCCESS)
    {
        png_to_4bit_gray_map = GX_NULL;
    }

    gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen.image_convert_screen_jpeg_to_4bit_gray_win,
        GX_PIXELMAP_ID_ICON_FOOT_JPEG, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        GX_COLOR_FORMAT_4BIT_GRAY, GX_IMAGE_READER_MODE_ALPHA | GX_IMAGE_READER_MODE_DITHER);

    if (gx_image_reader_start(&image_reader, jpeg_to_4bit_gray_map) != GX_SUCCESS)
    {
        jpeg_to_4bit_gray_map = GX_NULL;
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;

    tx_byte_pool_create(&test_memory_pool, "scratchpad", test_memory,
    TEST_MEMORY_SIZE * sizeof(GX_COLOR));

    gx_system_memory_allocator_set(test_memory_allocate, test_memory_free);

    /* Toggle to Image Convert Screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, pSpriteScreen);
GX_ENTER_CRITICAL
    test_png_jpeg_to_4bit_gray_convert();
GX_EXIT_CRITICAL
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("test image convert to 4bpp gray format pixelmap");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
