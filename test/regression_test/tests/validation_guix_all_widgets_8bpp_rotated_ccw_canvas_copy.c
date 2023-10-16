/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_8bpp_rotated_ccw_canvas_copy", /* Test name */
    38, 23, 323, 161  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_8bit_palette_rotated
#undef win32_graphics_driver_setup_8bit_palette_rotated
#endif
#define win32_graphics_driver_setup_8bit_palette_rotated  gx_validation_graphics_driver_setup_8bit_palette_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets_8bpp_rotated_ccw.c"

extern GX_STUDIO_DISPLAY_INFO all_widgets_8bpp_rotated_ccw_display_table[];
char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = all_widgets_8bpp_rotated_ccw_display_table[MAIN_DISPLAY].theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    /* Toggle to window screen. */
    ToggleScreen(pWindowScreen, pButtonScreen);
    gx_widget_fill_color_set(&window_screen.window_screen_fade_window,
                             GX_COLOR_ID_PURPLE, GX_COLOR_ID_PURPLE, GX_COLOR_ID_PURPLE);

    gx_canvas_alpha_set(&animation_canvas, 0);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set canvas alpha to 0");
    gx_validation_screen_refresh();

    gx_canvas_alpha_set(&animation_canvas, 255);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set canvas alpha to 255");
    gx_validation_screen_refresh();

    gx_canvas_offset_set(&animation_canvas, -100, -100);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set canvas offset to (-100, -100)");
    gx_validation_screen_refresh();
        
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

