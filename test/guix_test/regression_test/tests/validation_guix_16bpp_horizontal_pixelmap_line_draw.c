/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_16bpp_horizontal_pixelmap_line_draw", /* Test name */
    24, 43, 370, 402  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the graphics
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

#include "demo_guix_all_widgets_16bpp.c"

char test_comment[256];

static int map_id_list[]={
GX_PIXELMAP_ID_SAVE_ICON_RAW,/*raw*/
GX_PIXELMAP_ID_ROTATE_APPLE,/*alpha*/
GX_PIXELMAP_ID_FISH,/*compressed*/
GX_PIXELMAP_ID_RADIO_ON,/*compressed, alpha*/
};

static int map_id = 0;
static int blend_alpha;
static int test_shape = 0;

#define TEST_CIRCLE 0
#define TEST_PIE    1


VOID test_graphics_draw(GX_WINDOW *window)
{
INT       xcenter = 213;
INT       ycenter = 230;
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = blend_alpha;

    gx_context_pixelmap_set(map_id);
    gx_context_brush_define(GX_COLOR_ID_GRAY, GX_COLOR_ID_PURPLE, GX_BRUSH_PIXELMAP_FILL);
    gx_context_brush_width_set(1);

    switch(test_shape)
    {
    case TEST_CIRCLE:
        gx_canvas_circle_draw(xcenter, ycenter, 160);
        break;

    case TEST_PIE:
        gx_canvas_pie_draw(213, 230, 317, 139, 218);
        break;
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
int set_blend_null;
GX_DRAW_CONTEXT context;
GX_FILL_PIXELMAP_INFO info;

    ToggleScreen(pShapesScreen, pButtonScreen);

    gx_widget_draw_set((GX_WIDGET *)&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    for(set_blend_null = 0; set_blend_null < 2; set_blend_null++)
    {
        if(set_blend_null)
        {
            root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
        }

        for(test_shape = 0; test_shape < 2; test_shape++)
        {
            for(blend_alpha = 128; blend_alpha <= 255; blend_alpha += 127)
            {
                for(index = 0; index < (int)(sizeof(map_id_list)/sizeof(int)); index++)
                {
                    map_id = map_id_list[index];
                    sprintf(test_comment, "set_pixel_blend_null = %d, blend_alpha = %d, map_id = %d, shape_id = %d", set_blend_null, blend_alpha, map_id, test_shape);
                    gx_validation_set_frame_id(frame_id++);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();
                }
            }
        }
    }

    memset(&context, 0, sizeof(GX_DRAW_CONTEXT));
    memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));
    context.gx_draw_context_brush.gx_brush_alpha = 255;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_horizontal_pixelmap_line_draw(&context, 40, 43, 1, &info);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("call horizontal pixelmap line draw with info->pixelmap = GX_NULL");   
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
