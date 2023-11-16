/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_16bpp_rotated_horizontal_pixelmap_line_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_565rgb_rotated  
#undef win32_graphics_driver_setup_565rgb_rotated
#endif
#define win32_graphics_driver_setup_565rgb_rotated  gx_validation_graphics_driver_setup_565rgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp_rotated.c"

char test_comment[256];

static int map_id_list[]={
GX_PIXELMAP_ID_ID_RAW,/*raw*/
GX_PIXELMAP_ID_ID_ALPHA,/*alpha*/
GX_PIXELMAP_ID_ID_COMPRESS,/*compressed*/
GX_PIXELMAP_ID_ID_COMPRESS_ALPHA,/*compressed, alpha*/
};

static int map_id = 0;
static int blend_alpha = 0;
static int test_shape = 0;

VOID test_graphics_draw(GX_WINDOW *window)
{
GX_BRUSH *brush;

    gx_window_draw(window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = blend_alpha;

    gx_context_pixelmap_set(map_id);
    gx_context_brush_define(GX_COLOR_ID_GRAY, GX_COLOR_ID_PURPLE, GX_BRUSH_PIXELMAP_FILL);
    gx_context_brush_width_set(2);

    gx_canvas_pie_draw(213, 230, 317, 60, 120);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;

    ToggleScreen(pShapesScreen, pButtonScreen);

    gx_widget_draw_set((GX_WIDGET *)&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    for(blend_alpha = 128; blend_alpha <= 255; blend_alpha += 127)
    {
        for(index = 0; index < (int)(sizeof(map_id_list)/sizeof(int)); index++)
        {
            map_id = map_id_list[index];
            sprintf(test_comment, "blend_alpha = %d, map_id = %d", blend_alpha, map_id);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
