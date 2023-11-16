/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_8bpp_horizontal_pixelmap_line_draw", /* Test name */
     28, 24, 370, 402  /* Define the coordinates of the capture area.
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
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_8bit_palette  
#undef win32_graphics_driver_setup_8bit_palette  
#endif
#define win32_graphics_driver_setup_8bit_palette  gx_validation_graphics_driver_setup_8bit_palette


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_graphics_palette.c"

char test_comment[255];
extern GX_STUDIO_DISPLAY_INFO graphics_palette_display_table[];

enum SHAPE_TYPES{
    TEST_CIRCLE = 0,
    TEST_PIE,
    TEST_SHAPE_COUNTS
};

int test_shape = TEST_CIRCLE;

VOID test_graphics_draw(GX_WINDOW *window)
{
    gx_window_draw((GX_WINDOW*)window);

    gx_context_pixelmap_set(GX_PIXELMAP_ID_CMOPRESSED_TRANSPARENT);
    gx_context_brush_define(line_color, fill_color, GX_BRUSH_PIXELMAP_FILL);
    gx_context_brush_width_set(1);

    switch(test_shape)
    {
    case TEST_CIRCLE:
        gx_canvas_circle_draw(213, 100, 160);
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
GX_CONST GX_THEME *theme_ptr;
GX_DRAW_CONTEXT context;
GX_FILL_PIXELMAP_INFO info;

    /* Save palette info. */
    theme_ptr = graphics_palette_display_table[DISPLAY_1].theme_table[DISPLAY_1_DEFAULT_THEME];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);
    
    gx_widget_draw_set(&main_window.main_window_graphics_window, test_graphics_draw);
    
    for(test_shape = 0; test_shape < TEST_SHAPE_COUNTS; test_shape++)
    {
        sprintf(test_comment, "shape type = %d", test_shape);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh(); 
    }

    memset(&context, 0, sizeof(GX_DRAW_CONTEXT));
    memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));
    context.gx_draw_context_brush.gx_brush_alpha = 255;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_horizontal_pixelmap_line_draw(&context, 40, 43, 1, &info);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("call horizontal pixelmap line draw with info->pixelmap = GX_NULL");   
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

