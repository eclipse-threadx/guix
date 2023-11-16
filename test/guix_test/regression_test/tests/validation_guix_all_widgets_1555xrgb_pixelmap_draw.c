/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1555xrgb_pixelmap_draw", /* Test name */
    367, 75, 590, 365  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

extern void mark_dirty();
extern void convert_start();

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
#ifdef win32_graphics_driver_setup_1555xrgb
#undef win32_graphics_driver_setup_1555xrgb
#endif
#define win32_graphics_driver_setup_1555xrgb  gx_validation_graphics_driver_setup_1555xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1555xrgb.c"

char test_comment[256];

static int brush_alpha = 255;
static int pixelmap_id = GX_PIXELMAP_ID_FISH;
static GX_BOOL test_draw_wrong_map_format = GX_FALSE;
static GX_BOOL test_blend_wrong_map_format = GX_FALSE;
static GX_PIXELMAP test_map;

VOID pixelmap_window_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;
GX_BRUSH *brush;
GX_DRAW_CONTEXT *context;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = brush_alpha;
    
    xpos = window->gx_window_client.gx_rectangle_left + 10;
    ypos = window->gx_window_client.gx_rectangle_top + 10;

    gx_widget_pixelmap_get(window, pixelmap_id, &map);

    if(test_draw_wrong_map_format || test_blend_wrong_map_format)
    {
        memset(&test_map, 0, sizeof(GX_PIXELMAP));
        test_map.gx_pixelmap_format = GX_COLOR_FORMAT_565RGB;
        test_map.gx_pixelmap_width = 100;
        test_map.gx_pixelmap_height = 100;
        map = &test_map;
    }

    if(test_blend_wrong_map_format)
    {
        context = _gx_system_current_draw_context;
        root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixelmap_blend(context, xpos, ypos, map, brush_alpha);
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
int      frame_id = 1;

    /* Toggle to image convert screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, (GX_WINDOW *)&rotate_screen);

    /* Reset draw function. */
    gx_widget_draw_set(&image_convert_screen.image_convert_screen_jpeg_to_1555xrgb_win, pixelmap_window_draw);

    brush_alpha = 0;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush_alpha = 0");
    gx_validation_screen_refresh();

    brush_alpha = 128;
    test_draw_wrong_map_format = GX_TRUE;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("draw wrong map format");
    gx_validation_screen_refresh();

    test_draw_wrong_map_format = GX_FALSE;
    test_blend_wrong_map_format = GX_TRUE;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("blend wrong map format");
    gx_validation_screen_refresh();
   
    test_blend_wrong_map_format = GX_FALSE;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush_alpha = 128, compressed, no alpha");
    gx_validation_screen_refresh();

    pixelmap_id = GX_PIXELMAP_ID_CHECKBOX_OFF; 
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush_alpha = 128, no compression, no alpha");
    gx_validation_screen_refresh();

    pixelmap_id = GX_PIXELMAP_ID_RADIO_OFF;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush_alpha = 128, no compression, alpha");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
