/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_display.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_32argb_pixelmap_draw", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_32argb
#undef win32_graphics_driver_setup_32argb
#endif
#define win32_graphics_driver_setup_32argb  gx_validation_graphics_driver_setup_32argb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_32argb.c"

char test_comment[256];

#define TEST_MAP_VALID          1
#define TEST_MAP_INVALID_FLAG   2
#define TEST_MAP_INVALID_FORMAT 3

GX_RESOURCE_ID test_map_id_list[]=
{
GX_PIXELMAP_ID_BLACK_PAUSE,/* Compressed, Alpha. */
GX_PIXELMAP_ID_ROTATE_FOOT,/* Alpha. */
GX_PIXELMAP_ID_SAVE_ICON,/* Compress. */
GX_PIXELMAP_ID_ROTATE_FISH/* Raw */
};

INT test_brush_alpha = 0xff;
INT test_map_id = 0;
INT test_map_type = TEST_MAP_VALID;
ULONG test_canvas_fill_color = 0xffecb0ff;
 
#define   TEST_CANVAS_WIDTH  400
#define   TEST_CANVAS_HEIGHT 300
GX_COLOR  test_canvas_memory[TEST_CANVAS_WIDTH * TEST_CANVAS_HEIGHT];
GX_CANVAS test_canvas;

VOID test_canvas_pixelmap_get(GX_PIXELMAP *canvas_pixelmap)
{
GX_RECTANGLE rect;
GX_PIXELMAP *map = GX_NULL;
GX_PIXELMAP  test_map;
GX_BRUSH    *brush;
INT          xpos;
INT          ypos;

    gx_utility_rectangle_define(&rect, 0, 0, TEST_CANVAS_WIDTH - 1, TEST_CANVAS_HEIGHT - 1);

    gx_canvas_drawing_initiate(&test_canvas, NULL, &rect);

    gx_context_brush_width_set(0);
    gx_context_brush_get(&brush);
    brush->gx_brush_style |= GX_BRUSH_SOLID_FILL;

    
    gx_context_raw_fill_color_set(test_canvas_fill_color);
    gx_canvas_rectangle_draw(&rect);

    brush->gx_brush_alpha = test_brush_alpha;
    
    switch(test_map_type)
    {
    case TEST_MAP_VALID:
        gx_context_pixelmap_get(test_map_id, &map);
        break;

    case TEST_MAP_INVALID_FLAG:
        memset(&test_map, 0, sizeof(GX_PIXELMAP));
        test_map.gx_pixelmap_format = GX_COLOR_FORMAT_32ARGB;
        test_map.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
        test_map.gx_pixelmap_width = 100;
        test_map.gx_pixelmap_height = 100;
        map = &test_map;

        _gx_display_driver_32argb_pixelmap_blend(GX_NULL, 0, 0, map, 128);

        test_map.gx_pixelmap_flags = GX_PIXELMAP_TRANSPARENT;
        _gx_display_driver_32argb_pixelmap_blend(GX_NULL, 0, 0, map, 128);
        break;

    case TEST_MAP_INVALID_FORMAT:
        memset(&test_map, 0, sizeof(GX_PIXELMAP));
        test_map.gx_pixelmap_format = GX_COLOR_FORMAT_565RGB;
        test_map.gx_pixelmap_width = 100;
        test_map.gx_pixelmap_height = 100;
        map = &test_map;

        _gx_display_driver_32argb_pixelmap_blend(GX_NULL, 0, 0, map, 128);
        break;
    }

    if(map)
    {
        xpos = -10;
        ypos = -10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        xpos = -10;
        ypos = TEST_CANVAS_HEIGHT - map->gx_pixelmap_height + 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        xpos = TEST_CANVAS_WIDTH - map->gx_pixelmap_width + 10;
        ypos = - 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        xpos = TEST_CANVAS_WIDTH - map->gx_pixelmap_width + 10;
        ypos = TEST_CANVAS_HEIGHT - map->gx_pixelmap_height + 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        xpos = (TEST_CANVAS_WIDTH - map->gx_pixelmap_width) >> 1;
        ypos = (TEST_CANVAS_WIDTH - map->gx_pixelmap_height) >> 1;
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
    
    gx_canvas_pixelmap_get(canvas_pixelmap);
    canvas_pixelmap->gx_pixelmap_flags |= GX_PIXELMAP_ALPHA;
    gx_canvas_drawing_complete(&test_canvas, GX_FALSE);

}

VOID pixelmap_window_draw(GX_WINDOW *window)
{
GX_PIXELMAP canvas_map;

     gx_window_draw(window);

     test_canvas_pixelmap_get(&canvas_map);
     gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left,
                         window->gx_widget_size.gx_rectangle_top,
                         &canvas_map);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT index;
UINT canvas_fill_index;
    
    /* Create test canvas. */
    gx_canvas_create(&test_canvas, "test_canvas",
        root->gx_window_root_canvas->gx_canvas_display, GX_CANVAS_SIMPLE,
        TEST_CANVAS_WIDTH, TEST_CANVAS_HEIGHT, test_canvas_memory, TEST_CANVAS_WIDTH * TEST_CANVAS_HEIGHT * sizeof(GX_COLOR));

    /* Reset draw function of button screen to test 32argb pixelmap draw. */
    gx_widget_draw_set(&button_screen, pixelmap_window_draw);

    for(canvas_fill_index = 0; canvas_fill_index < 2; canvas_fill_index++)
    {
        if(canvas_fill_index)
        {
            test_canvas_fill_color = 0x80ecb0ff;
        }

        for(test_brush_alpha = 255; test_brush_alpha > 0; test_brush_alpha -= 128)
        {
            for(index = 0; index < sizeof(test_map_id_list)/sizeof(GX_RESOURCE_ID); index++)
            {
                test_map_id = test_map_id_list[index];

                sprintf(test_comment, "test canvas fill color = 0x%08x, brush alpha = %d, map id = %d",
                        (unsigned int)test_canvas_fill_color, test_brush_alpha, test_map_id);

                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }
        }
    }

    test_brush_alpha = 0;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush alpha = 0");
    gx_validation_screen_refresh();

    test_brush_alpha = 128;
    test_map_type = TEST_MAP_INVALID_FLAG;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("draw map with un-supported flag");
    gx_validation_screen_refresh();

    test_map_type = TEST_MAP_INVALID_FORMAT;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("draw map with un-supported format");
    gx_validation_screen_refresh();

    /* Delete test canvas. */
    gx_canvas_delete(&test_canvas);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
