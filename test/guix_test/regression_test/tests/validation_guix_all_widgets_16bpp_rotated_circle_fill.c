/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_16bpp_rotated_circle_fill", /* Test name */
    51, 9, 442, 400  /* Define the coordinates of the capture area.
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

typedef struct CIRCLE_INFO_STRUCT{
int xcenter;
int ycenter;
int radius;
}CIRCLE_INFO;

CIRCLE_INFO circle_info_list[]={
{213, 230, 300},
{323, 230, 120},
{0, 0, 0}
};

static int test_xcenter;
static int test_ycenter;
static int test_radius;
static int test_fill_map = GX_FALSE;

VOID test_graphics_draw(GX_WINDOW *window)
{
UINT style = GX_BRUSH_SOLID_FILL | GX_BRUSH_ALIAS;

    gx_window_draw((GX_WINDOW*)window);

    if(test_fill_map)
    {
        style |= GX_BRUSH_PIXELMAP_FILL;
        gx_context_pixelmap_set(GX_PIXELMAP_ID_ID_RAW);
    }

    gx_context_brush_define(GX_COLOR_ID_GRAY, GX_COLOR_ID_PURPLE, style);
    gx_context_brush_width_set(1);

    gx_canvas_circle_draw(test_xcenter, test_ycenter, test_radius);
}

VOID test_invalid_graphics_draw(GX_WINDOW *window)
{
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = 255;

    gx_context_brush_define(GX_COLOR_ID_GRAY, GX_COLOR_ID_PURPLE, GX_BRUSH_SOLID_FILL | GX_BRUSH_PIXELMAP_FILL);
    gx_context_brush_width_set(0);

    test_xcenter = 213;
    test_ycenter = 300;
    test_radius = 100;

    /* Fill pixelmap is not set. */
    gx_canvas_circle_draw(test_xcenter, test_ycenter, test_radius);

    /* Fill pixelmap format is not match display format. */
    gx_context_pixelmap_set(GX_PIXELMAP_ID__4444ARGB_A);
    gx_canvas_circle_draw(test_xcenter, test_ycenter, test_radius);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
int index;
CIRCLE_INFO *info; 

    ToggleScreen((GX_WINDOW *)&shapes_screen, (GX_WINDOW *)&button_screen);
    gx_widget_draw_set(&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    for(test_fill_map = 0; test_fill_map < 2; test_fill_map++)
    {
        info = circle_info_list; 

        while(info->radius)
        {
            test_xcenter = info->xcenter;
            test_ycenter = info->ycenter;
            test_radius = info->radius;

            sprintf(test_comment, "fill map = %d, xcenter = %d, ycenter = %d, radius = %d",
                    test_fill_map, info->xcenter, info->ycenter, info->radius);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();

            info++;
        }
    }

    gx_widget_draw_set(&shapes_screen.shapes_screen_graphics_window, test_invalid_graphics_draw);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test invalid properties.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
