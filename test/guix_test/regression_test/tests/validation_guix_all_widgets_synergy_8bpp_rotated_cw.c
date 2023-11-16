/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_synergy_8bpp_rotated_cw", /* Test name */
    0, 0, 479, 639  /* Define the coordinates of the capture area.
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
#define _gx_synergy_display_driver_setup gx_validation_graphics_driver_setup_8bit_palette_rotated

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_synergy_8bpp_rotated_cw.c"

extern GX_STUDIO_DISPLAY_INFO all_widgets_synergy_8bpp_rotated_cw_display_table[];
extern VOID _gx_display_driver_8bpp_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

GX_WINDOW *screen_list[]={
(GX_WINDOW *)&gauge_screen,
(GX_WINDOW *)&indicator_screen,
(GX_WINDOW *)&lines_screen,
(GX_WINDOW *)&menu_screen,
(GX_WINDOW *)&resize_screen,
(GX_WINDOW *)&rotate_screen,
(GX_WINDOW *)&scroll_wheel_screen,
(GX_WINDOW *)&shapes_screen,
(GX_WINDOW *)&text_screen,
(GX_WINDOW *)&window_screen,
(GX_WINDOW *)&button_screen,
GX_NULL
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_WINDOW **screen = screen_list;
GX_WINDOW *current = (GX_WINDOW *)&button_screen;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = all_widgets_synergy_8bpp_rotated_cw_display_table[MAIN_DISPLAY].theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_4bit_glyph_draw = _gx_display_driver_8bpp_rotated_glyph_4bit_draw;

    while(*screen)
    {
        ToggleScreen(*screen, current);
        current = *screen;

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment((char *)current->gx_widget_name);
        gx_validation_screen_refresh();

        screen++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





