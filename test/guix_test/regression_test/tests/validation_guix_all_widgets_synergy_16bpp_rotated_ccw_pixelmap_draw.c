/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_display.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_synergy_16bpp_rotated_ccw_pixelmap_draw", /* Test name */
    13, 16, 463, 424  /* Define the coordinates of the capture area.
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
#define _gx_synergy_display_driver_setup gx_validation_dave2d_graphics_driver_setup_565rgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_synergy_16bpp_rotated_ccw.c"

char test_comment[256];

GX_RESOURCE_ID test_map_id_list[]=
{
GX_PIXELMAP_ID__565RGB,
GX_PIXELMAP_ID__565RGB_C,
GX_PIXELMAP_ID__32ARGB,
GX_PIXELMAP_ID__32ARGB_C,
GX_PIXELMAP_ID__4444ARGB,
GX_PIXELMAP_ID__4444ARGB_C,
GX_PIXELMAP_ID__ALPHAMAP,
GX_PIXELMAP_ID__PALETTE,
GX_PIXELMAP_ID__PALETTE_C
};

INT test_brush_alpha = 0xff;
INT test_map_id = 0;

VOID pixelmap_window_draw(GX_WINDOW *window)
{
GX_PIXELMAP *map = GX_NULL;
GX_PIXELMAP  test_map;
INT          xpos;
INT          ypos;
GX_RECTANGLE *size = &window->gx_widget_size;
GX_BRUSH     *brush;

    gx_window_draw(window);

    gx_context_pixelmap_get(test_map_id, &map);

    gx_context_brush_get(&brush);

    brush->gx_brush_style |= GX_BRUSH_SOLID_FILL;
    brush->gx_brush_alpha = test_brush_alpha;

    if(map)
    {
        gx_context_fill_color_set(GX_COLOR_ID_PURPLE);

        /* Draw map to top-left corner. */
        xpos = size->gx_rectangle_left-10;
        ypos = size->gx_rectangle_top-10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        /* Draw map to bottom-left corner. */
        xpos = size->gx_rectangle_left-10;
        ypos = size->gx_rectangle_bottom - map->gx_pixelmap_height + 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        /* Draw map to top-right corner. */
        xpos = size->gx_rectangle_right - map->gx_pixelmap_width + 10;
        ypos = size->gx_rectangle_top- 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        /* Draw map to bottom-right corner. */
        xpos = size->gx_rectangle_right - map->gx_pixelmap_width + 10;
        ypos = size->gx_rectangle_bottom - map->gx_pixelmap_height + 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        /* Draw map to center. */
        xpos = (size->gx_rectangle_top + size->gx_rectangle_bottom - map->gx_pixelmap_width) >> 1;
        ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom - map->gx_pixelmap_height) >> 1;
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT index;
    
    /* Reset draw function of button screen to test pixelmap draw. */
    gx_widget_draw_set(&button_screen, pixelmap_window_draw);

    for(test_brush_alpha = 255; test_brush_alpha > 0; test_brush_alpha -= 128)
    {
        for(index = 0; index < sizeof(test_map_id_list)/sizeof(GX_RESOURCE_ID); index++)
        {
            test_map_id = test_map_id_list[index];

            sprintf(test_comment, "brush alpha = %d, map id = %d", test_brush_alpha, test_map_id);

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
