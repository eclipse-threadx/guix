/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_partial_frame_buffer_16bpp", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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

#include "demo_guix_partial_frame_buffer_16bpp.c"

typedef struct SCREEN_INFO_STRUCT{
GX_WINDOW *window;
GX_CHAR *name;
}SCREEN_INFO;

SCREEN_INFO screen_list[]={
    {&button_screen, "button_screen"},
    {&window_screen, "window_screen"},
    {&indicator_screen, "indicator_screen"},
    {&text_screen, "text_screen"},
    {&gauge_screen, "gauge_screen"},
    {&sprite_screen, "sprite_screen"},
    {&menu_screen, "menu_screen"},
    {&rotate_screen, "rotate_screen"},
    {&radial_slider_screen, "radial_slider_screen"},
    {&rich_text_view_screen, "rich_text_view_screen"},
    {&line_screen, "line_screen"},
    {&pic_screen, "pic_screen"},
    {&shapes_screen, "shapes_screen"},
    {&resize_screen, "resize_screen"},
    {&glyph_screen, "glyph_screen"},
    {GX_NULL, GX_NULL}
};

void draw_to_glyph_screen()
{
GX_CANVAS *canvas;
GX_WIDGET *screen = (GX_WIDGET *)&glyph_screen;
GX_RECTANGLE dirty;
GX_PIXELMAP *map;

    /* pick up the canvas pointer */
    gx_widget_canvas_get(screen, &canvas);

    gx_utility_rectangle_define(&dirty,  500, 10, 600, 60);

    /* Initiate drawing on this canvas.  */
    if (gx_canvas_drawing_initiate(canvas, screen, &dirty) == GX_SUCCESS)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_CHECKBOX_OFF, &map);
        gx_canvas_pixelmap_tile(&dirty, map);

        /* Indicate that drawing on this canvas is complete.  */
        gx_canvas_drawing_complete(canvas, GX_TRUE);
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
SCREEN_INFO *next = screen_list;
SCREEN_INFO *previous;

    while(next->window)
    {
        if(next != screen_list)
        {
            ToggleScreen(next->window, previous->window);
        }

        gx_validation_screen_refresh();
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(next->name);
        gx_validation_write_frame_buffer();

        previous = next;
        next++;
    }

    draw_to_glyph_screen();
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Drawing to glyph screen.");
    gx_validation_write_frame_buffer();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





