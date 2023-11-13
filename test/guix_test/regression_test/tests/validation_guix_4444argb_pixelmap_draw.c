/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_4444argb_pixelmap_draw", /* Test name */
    18, 178, 102, 216  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_4444argb  
#undef win32_graphics_driver_setup_4444argb  
#endif
#define win32_graphics_driver_setup_4444argb  gx_validation_graphics_driver_setup_4444argb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_pixelmaps_4444argb.c"

char test_comment[256];

static int pixelmap_id = GX_PIXELMAP_ID_COMPRESS_ALPHA_4444ARGB;
static GX_BOOL test_wrong_map_format = GX_FALSE;
static GX_BOOL test_blend_wrong_map_format = GX_FALSE;
static GX_PIXELMAP test_map;

static int x_shift = -50;
static int blend_alpha = 255;
VOID window_1_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;

    xpos = window->gx_window_client.gx_rectangle_left + x_shift;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    gx_widget_pixelmap_get((GX_WIDGET *)window, pixelmap_id, &map);

    if(test_wrong_map_format || test_blend_wrong_map_format)
    {
        xpos = window->gx_window_client.gx_rectangle_left;
        ypos = window->gx_window_client.gx_rectangle_top;

        memset(&test_map, 0, sizeof(GX_PIXELMAP));
        test_map.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_PALETTE;
        test_map.gx_pixelmap_width = 100;
        test_map.gx_pixelmap_height = 100;
        map = &test_map;
    }

    if(test_blend_wrong_map_format)
    {
        /* Test blend pixlemap that is not suported. */
        root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixelmap_blend(GX_NULL, xpos, ypos, map, blend_alpha);

        /* Test blend when pixelmap has alpha. */
        test_map.gx_pixelmap_format = GX_COLOR_FORMAT_4444ARGB;
        test_map.gx_pixelmap_flags = GX_PIXELMAP_ALPHA;
        root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixelmap_blend(GX_NULL, xpos, ypos, map, blend_alpha);
    }
    else
    {
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_window_1, window_1_draw);

    for(x_shift = -50; x_shift <= 50; x_shift += 100)
    {
        sprintf(test_comment, "draw pixelmap, compressed, with alpha, x_shift = %d", x_shift);
    	gx_validation_set_frame_id(frame_id++);
    	gx_validation_set_frame_comment(test_comment);
    	gx_validation_screen_refresh();
    }

    test_wrong_map_format = GX_TRUE;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("draw wrong map format");
    gx_validation_screen_refresh();

    test_blend_wrong_map_format = GX_TRUE;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("blend wrong map format");
    gx_validation_screen_refresh();
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





