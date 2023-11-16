/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_standalone_binres_load_16bpp", /* Test name */
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

#include "demo_guix_standalone_binres_load_16bpp.c"

void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *)new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *)new_win);
    }
    gx_widget_hide((GX_WIDGET *)old_win);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int failed_tests = 0; 
UINT status;
UINT buffer_size;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("565rgb screen");
    gx_validation_screen_refresh();

    ToggleScreen(&screen_4444argb, &main_screen);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("444argb screen");
    gx_validation_screen_refresh();

    ToggleScreen(&screen_8bit_palette, &screen_4444argb);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("8bit palette screen");
    gx_validation_screen_refresh();

    ToggleScreen(&screen_alphamap, &screen_8bit_palette);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("alphamap screen");
    gx_validation_screen_refresh();

    // Invalid magic number
    ((USHORT *)root_address_font_4bpp)[0] = 0x00;
    status = gx_binres_font_load(root_address_font_4bpp, 0, font_4bpp_buffer, &buffer_size);
    EXPECT_EQ(GX_INVALID_FORMAT, status);
    ((USHORT *)root_address_font_4bpp)[0] = GX_MAGIC_NUMBER;

    // Insufficient buffer size
    buffer_size = 100;
    status = gx_binres_font_load(root_address_font_4bpp, 0, font_4bpp_buffer, &buffer_size);
    EXPECT_EQ(GX_INVALID_MEMORY_SIZE, status);

    // Invalid font id
    status = gx_binres_font_load(root_address_font_4bpp, 2, font_4bpp_buffer, &buffer_size);
    EXPECT_EQ(GX_NOT_FOUND, status)

    // Invalid font header magic number
    ((USHORT *)root_address_font_4bpp)[2] = 0x00;
    status = gx_binres_font_load(root_address_font_4bpp, 0, font_4bpp_buffer, &buffer_size);
    EXPECT_EQ(GX_INVALID_FORMAT, status);
    ((USHORT *)root_address_font_4bpp)[2] = GX_MAGIC_NUMBER;

    status = gx_binres_font_load(root_address_font_4bpp, 0, font_4bpp_buffer, &buffer_size);
    EXPECT_EQ(GX_SUCCESS, status);

    // Invalid magic number
    ((USHORT *)root_address_pixelmap_raw_alphamap)[0] = 0x00;
    status = gx_binres_pixelmap_load(root_address_pixelmap_raw_alphamap, 0, &pixelmap_raw_alphamap);
    EXPECT_EQ(GX_INVALID_FORMAT, status);
    ((USHORT *)root_address_pixelmap_raw_alphamap)[0] = GX_MAGIC_NUMBER;

    // Invalid pixelmap id
    status = gx_binres_pixelmap_load(root_address_pixelmap_raw_alphamap, 2, &pixelmap_raw_alphamap);
    EXPECT_EQ(GX_NOT_FOUND, status);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





