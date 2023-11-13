/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_332rgb_pixelmap_blend", /* Test name */
    20, 30, 420, 400  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_332rgb  
#undef win32_graphics_driver_setup_332rgb  
#endif
#define win32_graphics_driver_setup_332rgb  gx_validation_graphics_driver_setup_332rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_all_widgets_332rgb.c"

char test_comment[256];

static int map_id_list[]={
GX_PIXELMAP_ID_SAVE_ICON_RAW,/*raw*/
GX_PIXELMAP_ID_ROTATE_APPLE,/*alpha*/
};

static int map_id = 0;

VOID test_graphics_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_BRUSH *brush;
GX_PIXELMAP *map;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = 128;

    gx_context_pixelmap_get(map_id, &map);

    xpos = window->gx_window_client.gx_rectangle_left;
    ypos = window->gx_window_client.gx_rectangle_top;

    gx_canvas_pixelmap_draw(xpos, ypos, map);
    
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
GX_PIXELMAP invalid_map;

    ToggleScreen(pShapesScreen, pButtonScreen);

    gx_widget_draw_set((GX_WIDGET *)&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
    for(index = 0; index < (int)(sizeof(map_id_list)/sizeof(int)); index++)
    {
        map_id = map_id_list[index];
        sprintf(test_comment, "set_pixel_blend_null = true, blend_alpha = 128, map_id = %d", map_id);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Set invalid format. */
    invalid_map.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_PALETTE;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixelmap_blend(GX_NULL, 0, 0, &invalid_map, 128);
  
    /* Set invalid flag. */
    invalid_map.gx_pixelmap_flags = GX_PIXELMAP_TRANSPARENT;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixelmap_blend(GX_NULL, 0, 0, &invalid_map, 128);

    /* Set invalid flag. */
    invalid_map.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixelmap_blend(GX_NULL, 0, 0, &invalid_map, 128);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("call pixelmap blend with invalid pixelmap");   
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}





