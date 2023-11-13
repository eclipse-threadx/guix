/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_ellipse_fill", /* Test name */
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_graphics_32bpp.c"


static VOID ellipse_draw_func(GX_WINDOW *window)
{
ULONG brush_style = 0;
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*) window);
    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = brush_alpha;

    if(anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    brush_style |= GX_BRUSH_PIXELMAP_FILL;

    gx_context_brush_define(line_color, fill_color, brush_style);
    gx_context_brush_width_set(brush_width);

    switch(draw_shape)
    {
    case ELLIPSE:
        gx_canvas_ellipse_draw(213, 230, a, b);
        break;
    }
}


char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_RECTANGLE size;
int width, height;

    ToggleScreen((GX_WINDOW *)pEllipseWin, (GX_WINDOW *)pMainWin);
    pGraphicsWin = &pEllipseWin -> ellipse_window_graphics_window;
    pre_shape = draw_shape;
    draw_shape = ELLIPSE;

    /* Loop radius from 5 to 200. */
    solid_fill = GX_FALSE;
    pixelmap_fill = GX_TRUE;
    brush_width  = 0;
    brush_alpha = 255;
    pixelmap_index = 0;
    anti_aliased = GX_FALSE;
    a = 150;
    b = 80;
    /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    
    /* Set test comments. */
    sprintf(test_comment, "Original ellipse win: a:%d, b:%d, brush_width %d, solid_fill:%d, pixelmap_fill:%d, brush_alpha: %d, anti_aiased: %d ",
                            a, b, brush_width, solid_fill, pixelmap_fill, brush_alpha, anti_aliased);

    gx_validation_set_frame_comment(test_comment);

    /* Mark graphics window dirty. */
    gx_system_dirty_mark(pGraphicsWin);

    /* Force a screen refresh.  */
    gx_validation_screen_refresh();
    
    size = pGraphicsWin->gx_widget_size;
    width = size.gx_rectangle_right - size.gx_rectangle_left + 1;
    width >>= 2;
    height = size.gx_rectangle_bottom - size.gx_rectangle_top + 1;
    height >>= 2;
    size.gx_rectangle_left += width + (width >> 1);
    size.gx_rectangle_right -= width;
    size.gx_rectangle_top += height + (height >> 1);
    size.gx_rectangle_bottom -= height;

    gx_widget_resize(pGraphicsWin, &size);
    /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    
    /* Set test comments. */
    sprintf(test_comment, "Resize graphics window:size:(%d, %d, %d, %d), a:%d, b:%d, brush_width %d, solid_fill:%d, pixelmap_fill:%d, brush_alpha: %d, anti_aiased: %d ",
                            size.gx_rectangle_left, size.gx_rectangle_top, size.gx_rectangle_right, size.gx_rectangle_bottom,
                            a, b, brush_width, solid_fill, pixelmap_fill, brush_alpha, anti_aliased);

    gx_validation_set_frame_comment(test_comment);

    /* Mark graphics window dirty. */
    gx_system_dirty_mark(pGraphicsWin);

    /* Force a screen refresh.  */
    gx_validation_screen_refresh();
    
    gx_widget_draw_set(pGraphicsWin, ellipse_draw_func);
    
        /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    
    /* Set test comments. */
    sprintf(test_comment, "Set brush with pixelmap fill style, but not set pixelmap. Nothing should be drawn here.");

    gx_validation_set_frame_comment(test_comment);

    /* Mark graphics window dirty. */
    gx_system_dirty_mark(pGraphicsWin);

    /* Force a screen refresh.  */
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
