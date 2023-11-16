/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_polygon_fill", /* Test name */
    28, 24, 400, 430  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_graphics_32bpp.c"

char test_comment[255];

VOID test_graphics_draw(GX_WINDOW *window)
{
ULONG brush_style = 0;
GX_POINT rectangle[4] = {{18, 50}, {84, 50}, {84, 150}, {18, 150}};
GX_POINT pentagon[5] = {{290, 20}, {335, -20}, {380, 20}, {360, 70}, {310, 70}};
GX_POINT concave[6] = {{330, 50}, {370, 80}, {410, 50}, {410, 150}, {370, 110}, {330, 150}};
GX_POINT star[11] = { { 212, 232 }, { 223, 192 }, { 237, 232 }, { 273, 232 }, { 244, 258 }, { 256, 299 }, { 226, 275 }, { 192, 298 }, { 203, 258 }, { 180, 250 }, { 173, 232 } };
GX_POINT self_intersection[8] = {{110, 406}, {189, 500}, {266, 406}, {334, 500}, {334, 406}, {264, 500}, {189, 406}, {110, 500}};
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*) window);
    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = 128;

    if(anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    if(solid_fill)
    {
        brush_style |= GX_BRUSH_SOLID_FILL;
    }

    if(pixelmap_fill)
    {
        brush_style |= GX_BRUSH_PIXELMAP_FILL;
        gx_context_pixelmap_set(pixelmap_id[pixelmap_index]);
    }

    gx_context_brush_define(line_color, fill_color, brush_style | GX_BRUSH_ROUND);
    gx_context_brush_width_set(brush_width);

    gx_canvas_polygon_draw(rectangle, 4);
    gx_canvas_polygon_draw(pentagon, 5);
    gx_canvas_polygon_draw(concave, 6);
    gx_canvas_polygon_draw(star, 11);
    gx_canvas_polygon_draw(self_intersection, 8);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
GX_CANVAS *canvas;

    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_graphics_window, test_graphics_draw);

    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
            brush_width = 1;
            sprintf(test_comment,  "blend_alpha = 128, brush_width = %d, pixelmap_fill = %d", brush_width, pixelmap_fill);
            break;

        case 1:
            brush_width = 4;
            sprintf(test_comment,  "blend_alpha = 128, brush_width = %d, pixelmap_fill = %d", brush_width, pixelmap_fill);
            break;

        case 2:
            pixelmap_fill = GX_TRUE;
            sprintf(test_comment,  "blend_alpha = 128, brush_width = %d, pixelmap_fill = %d", brush_width, pixelmap_fill);
            break;

        case 3:
            gx_widget_canvas_get(&main_window, &canvas);
            canvas->gx_canvas_display->gx_display_driver_polygon_draw = GX_NULL;
            sprintf(test_comment,  "blend_alpha = 128, brush_width = %d, pixelmap_fill = %d, set gx_display_driver_polygon_draw = GX_NULL", brush_width, pixelmap_fill);
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
