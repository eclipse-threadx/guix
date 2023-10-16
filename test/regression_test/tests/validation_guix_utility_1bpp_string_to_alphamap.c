/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_1bpp_string_to_alphamap", /* Test name */
    437, 269, 639, 479  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb
#undef win32_graphics_driver_setup_24xrgb
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_text_rotation.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/

char test_comment[256];
char test_string[]="!'\"";

GX_PIXELMAP *map = GX_NULL;
void test_pixelmap_draw(GX_WINDOW *window)
{
int xpos;
int ypos;

    gx_window_draw(window);

    if(map)
    {
        gx_context_fill_color_set(GX_COLOR_ID_RED);
        xpos = window->gx_widget_size.gx_rectangle_left + 10;
        ypos = window->gx_widget_size.gx_rectangle_top + 10;
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int         frame_id = 1;
GX_WINDOW  *window = &screen_base.screen_base_text_window_1bpp;
GX_FONT    *font;
GX_PIXELMAP destination;

    gx_widget_draw_set(window, test_pixelmap_draw);
    gx_widget_font_get(window, GX_FONT_ID_MONO_FONT, &font);

    gx_utility_string_to_alphamap(test_string, font, &destination);
    map = &destination;

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "convert string \"%s\" to alphamap", test_string);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
