/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_1bpp_pixelmap_draw", /* Test name */
    365, 77, 587, 361  /* Define the coordinates of the capture area.
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

static int x_shift = -50;
static int test_map_id = GX_PIXELMAP_ID_COMPRESSED_TRANSPARENT;
static GX_BOOL test_invalid_format = GX_FALSE;

VOID test_jpeg_to_1bpp_win_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;
GX_PIXELMAP invalid_map;

    xpos = window->gx_window_client.gx_rectangle_left + x_shift;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    if(test_invalid_format)
    {
        memset(&invalid_map, 0, sizeof(GX_PIXELMAP));
        invalid_map.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_PALETTE;
        invalid_map.gx_pixelmap_width = 100;
        invalid_map.gx_pixelmap_height = 100;
        map = &invalid_map;
    }
    else
    {
        gx_widget_pixelmap_get((GX_WIDGET *)window, test_map_id, &map);
    }
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;

    /* Toggle to Image Convert Screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, pSpriteScreen);

    gx_widget_draw_set((GX_WIDGET *)&image_convert_screen.image_convert_screen_jpeg_to_1bpp_win, test_jpeg_to_1bpp_win_draw);    
    for(x_shift = -50; x_shift <= 100; x_shift += 100)
    {
        sprintf(test_comment, "compressed, transparent, x_shift = %d", x_shift);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    x_shift = -10;
    test_map_id = GX_PIXELMAP_ID_COMPRESSED;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("compressed, no transparent, x_shift = -10");
    gx_validation_screen_refresh();

    
    test_invalid_format = GX_TRUE;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("invalid map format");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
