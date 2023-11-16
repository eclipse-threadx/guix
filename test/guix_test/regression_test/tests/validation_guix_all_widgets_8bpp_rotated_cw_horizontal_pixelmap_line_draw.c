/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_8bpp_rotated_cw_horizontal_pixelmap_line_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_8bit_palette_rotated
#undef win32_graphics_driver_setup_8bit_palette_rotated
#endif
#define win32_graphics_driver_setup_8bit_palette_rotated  gx_validation_graphics_driver_setup_8bit_palette_rotated

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_8bpp_rotated_cw.c"

extern GX_STUDIO_DISPLAY_INFO all_widgets_8bpp_rotated_cw_display_table[];

char test_comment[256];

static int map_id_list[]={
    GX_PIXELMAP_ID__RAW,
    GX_PIXELMAP_ID__TRANS, 
    GX_PIXELMAP_ID__COMPRESS,
    GX_PIXELMAP_ID__COMPRESS_TRANS
};

static int map_id_index = 0;
VOID test_graphics_draw(GX_WINDOW *window)
{
    gx_window_draw(window);

    gx_context_pixelmap_set(map_id_list[map_id_index]);
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
GX_DRAW_CONTEXT context;
GX_FILL_PIXELMAP_INFO info;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = all_widgets_8bpp_rotated_cw_display_table[MAIN_DISPLAY].theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    ToggleScreen(pShapesScreen, pButtonScreen);

    gx_widget_draw_set((GX_WIDGET *)&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    for(map_id_index = 0; map_id_index < (int)(sizeof(map_id_list)/sizeof(int)); map_id_index++)
    {
        sprintf(test_comment, "map_id = %d", map_id_list[map_id_index]);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    memset(&context, 0, sizeof(GX_DRAW_CONTEXT));
    memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));
    context.gx_draw_context_display = root->gx_window_root_canvas->gx_canvas_display;
    context.gx_draw_context_canvas = root->gx_window_root_canvas;

    /* call horizontal pixelmap line draw with info->pixelmap = GX_NULL.  */
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_horizontal_pixelmap_line_draw(&context, 40, 43, 1, &info);

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
