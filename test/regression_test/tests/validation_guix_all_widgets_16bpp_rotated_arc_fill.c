/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_16bpp_rotated_arc_fill", /* Test name */
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

typedef struct ARC_INFO_STRUCT{
int xcenter;
int ycenter;
int start_angle;
int end_angle;
int radius;
}ARC_INFO;

ARC_INFO arc_info_list[]={
{213, 230, 90, 270, 213},
{213, 230, 90, 330, 254},
{213, 230, 38, 360, 1792},
{213, 230, 326, 124, 400},
{400, 500, 0, 83, 88},
{213, 230, 350, 640, 254},
{213, 230, 0, 180, 100},
{213, 230, 200, 300, 100},
{213, 230, 80, 430, 100},
{213, 230, 180, 360, 160},
{213, 230, 90, 250, 160},
{213, 230, 5, 270, 121},
{213, 230, 0, 180, 300},
{50, 230, 280, 100, 300},
{50, 230, 80, 270, 200},
{500, 230, 30, 180, 200},
{0, 230, 30, 180, 200},
{0, 0, 0, 0, 0},
};

static int test_xcenter;
static int test_ycenter;
static int test_start_angle;
static int test_end_angle;
static int test_radius;
static int test_brush_width = 5;
static GX_BOOL test_anti_aliased = GX_TRUE;
static GX_BOOL test_round_end = GX_TRUE;
static GX_BOOL test_solid_fill = GX_TRUE;
static GX_BOOL test_pixelmap_fill = GX_FALSE;
static GX_UBYTE test_brush_alpha = 255;
static INT fill_pixelmap_id = GX_PIXELMAP_ID_ID_RAW;

VOID test_graphics_draw(GX_WINDOW *window)
{
ULONG brush_style = 0;
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = test_brush_alpha;

    if (test_anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    if (test_solid_fill)
    {
        brush_style |= GX_BRUSH_SOLID_FILL;
    }

    if (test_pixelmap_fill)
    {
        brush_style |= GX_BRUSH_PIXELMAP_FILL;
        gx_context_pixelmap_set(fill_pixelmap_id);
    }

    if (test_round_end)
    {
        brush_style |= GX_BRUSH_ROUND;
    }

    gx_context_brush_define(GX_COLOR_ID_GRAY, GX_COLOR_ID_PURPLE, brush_style);
    gx_context_brush_width_set(test_brush_width);

    gx_canvas_arc_draw(test_xcenter, test_ycenter, test_radius, test_start_angle, test_end_angle);
}

VOID test_invalid_graphics_draw(GX_WINDOW *window)
{
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = 255;

    gx_context_brush_define(GX_COLOR_ID_GRAY, GX_COLOR_ID_PURPLE, GX_BRUSH_SOLID_FILL | GX_BRUSH_PIXELMAP_FILL);
    gx_context_brush_width_set(10);

    test_xcenter = 213;
    test_ycenter = 300;
    test_start_angle = 30;
    test_end_angle = 120;

    /* Radius < (brush_width / 2). */
    test_radius = 4;
    gx_canvas_arc_draw(test_xcenter, test_ycenter, test_radius, test_start_angle, test_end_angle);

    /* Fill pixelmap is not set. */
    test_radius = 100;
    
    gx_canvas_arc_draw(test_xcenter, test_ycenter, test_radius, test_start_angle, test_end_angle);

    /* Fill pixelmap format is not match display format. */
    gx_context_pixelmap_set(GX_PIXELMAP_ID__4444ARGB_A);
    gx_canvas_arc_draw(test_xcenter, test_ycenter, test_radius, test_start_angle, test_end_angle);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
int index;
ARC_INFO *info; 

    ToggleScreen((GX_WINDOW *)&shapes_screen, (GX_WINDOW *)&button_screen);
    gx_widget_draw_set(&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    for(test_anti_aliased = 0; test_anti_aliased <= 1; test_anti_aliased++)
    {
        for(test_pixelmap_fill = 0; test_pixelmap_fill <= 1; test_pixelmap_fill ++)
        {

            info = arc_info_list; 

            while(info->radius)
            {
                test_xcenter = info->xcenter;
                test_ycenter = info->ycenter;
                test_start_angle = info->start_angle;
                test_end_angle = info->end_angle;
                test_radius = info->radius;

                sprintf(test_comment, "alias = %d, pixelmap fill = %d, xcenter = %d, ycenter = %d, start_angle = %d, end_angle = %d, radius = %d",
                        test_anti_aliased, test_pixelmap_fill,
                        info->xcenter, info->ycenter, info->start_angle, info->end_angle, info-> radius);
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();

                info++;
            }
        }
    }

    test_brush_width = 0;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush width = 0.");
    gx_validation_screen_refresh();

    gx_widget_draw_set(&shapes_screen.shapes_screen_graphics_window, test_invalid_graphics_draw);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test invalid properties.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
