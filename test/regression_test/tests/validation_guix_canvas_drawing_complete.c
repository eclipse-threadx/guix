/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_canvas_drawing_complete", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"
GX_CHAR test_comment[256];

int drawing_complete_count = 0;
int drawing_initiate_count = 0;

VOID drawing_complete(GX_DISPLAY *display, GX_CANVAS *canvas)
{
    drawing_complete_count++;
}

VOID drawing_initiate(GX_DISPLAY *display, GX_CANVAS *canvas)
{
    drawing_initiate_count++;
}

static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
GX_RECTANGLE dirty;
GX_CANVAS   *canvas;
UINT         status;

    /* Test gx_canvas_drawing_complete. */
    gx_widget_canvas_get(&button_screen, &canvas);
    canvas->gx_canvas_display->gx_display_driver_drawing_complete = drawing_complete;
    gx_utility_rectangle_define(&dirty, 0, 0, 640, 480);
    gx_canvas_drawing_initiate(canvas, &button_screen, &dirty);

    gx_context_brush_width_set(1);
    gx_context_brush_style_set(GX_BRUSH_SOLID_FILL);
    gx_context_fill_color_set(GX_COLOR_ID_ORANGE);
    gx_context_line_color_set(GX_COLOR_ID_ORANGE);
    gx_canvas_rectangle_draw(&dirty);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set drawing complete callback");
    gx_canvas_drawing_complete(canvas, GX_TRUE);
    gx_canvas_drawing_complete(canvas, GX_TRUE);

    if(drawing_complete_count != 1)
    {
        exit(1);
    }

    /* Test gx_canvas_drawing_initiate. */
    canvas->gx_canvas_display->gx_display_driver_drawing_initiate = drawing_initiate;
    canvas->gx_canvas_draw_nesting = 1;
    gx_utility_rectangle_define(&dirty, 100, 100, 200, 200);
    status = gx_canvas_drawing_initiate(canvas, GX_NULL, &dirty);

    if((drawing_initiate_count != 1) || (status != GX_NO_VIEWS))
    {
        exit(1);
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
