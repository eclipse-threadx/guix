/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_alphamap_draw", /* Test name */
    382, 139, 580, 290  /* Define the coordinates of the capture area.
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

#include "demo_alphamap_16bpp.c"

char test_comment[256];

static int map_id_list[]={
GX_PIXELMAP_ID_RED_APPLE_RAW,/*alpha*/
GX_PIXELMAP_ID_FOOT_COMPRESSED,/*compress, alpha*/
GX_PIXELMAP_ID_CHECKBOX_ON_RAW,/*raw*/
GX_PIXELMAP_ID_CHECKBOX_OFF_COMPRESSED/*compress*/
};
static int map_id = 0;
static int x_shift = 0;
static int blend_alpha = 255;

VOID test_pixelmap_wnd_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;
GX_BRUSH *brush;

    gx_context_brush_get(&brush);
    brush->gx_brush_fill_color = COLOR_ID_BLUE;
    brush->gx_brush_alpha = blend_alpha;

    xpos = window->gx_window_client.gx_rectangle_left + x_shift;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    gx_context_pixelmap_get(map_id, &map);
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_WINDOW *window2;
GX_BOOL set_blend_null;
int index;

    window2 = &main_window.main_window_window_2;
   
    gx_widget_draw_set(window2, test_pixelmap_wnd_draw);

    for(set_blend_null = 0; set_blend_null < 2; set_blend_null++)
    {
        if(set_blend_null)
        {
            root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
        }

        for(blend_alpha = 128; blend_alpha <= 255; blend_alpha += 127)
        {
            for(index = 0; index < (int)(sizeof(map_id_list) / sizeof(int)); index++)
            {
                map_id = map_id_list[index];
                for(x_shift = -10; x_shift <= 190; x_shift += 200)
                {
                    sprintf(test_comment, "set_blend_null = %d, blend_alpha = %d, map_id = %d, x_shift = %d", set_blend_null, blend_alpha, map_id, x_shift);
                    gx_validation_set_frame_id(frame_id++);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();
                }
            }
        } 
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





