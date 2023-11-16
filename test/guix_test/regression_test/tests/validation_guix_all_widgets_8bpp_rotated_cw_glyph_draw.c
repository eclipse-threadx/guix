/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_8bpp_rotated_cw_glyph_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_8bit_palette_rotated
#undef win32_graphics_driver_setup_8bit_palette_rotated
#endif
#define win32_graphics_driver_setup_8bit_palette_rotated  gx_validation_graphics_driver_setup_8bit_palette_rotated

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_8bpp_rotated_cw.c"

extern GX_STUDIO_DISPLAY_INFO all_widgets_8bpp_rotated_cw_display_table[];
extern VOID _gx_display_driver_8bpp_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

static char test_comment[255];
static const char test_string[] = "Te";
static int test_font_id = GX_FONT_ID_NORMAL_4BPP;

VOID test_button_screen_draw(GX_WINDOW *window)
{
GX_RECTANGLE *size;
GX_STRING string;
int xoffset;
int yoffset;

    size = &window->gx_window_client;
    gx_window_background_draw(window);

    gx_context_font_set(test_font_id);
    gx_context_line_color_set(GX_COLOR_ID_TEXT_COLOR);

    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 1;

    yoffset = size->gx_rectangle_top - 10;
    for(xoffset = 0; xoffset < 20; xoffset++)
    {
        gx_canvas_text_draw_ext(size->gx_rectangle_left - xoffset, yoffset, &string);
        gx_canvas_text_draw_ext(size->gx_rectangle_right - xoffset, yoffset, &string);

        yoffset += 30;
    }

    xoffset = size->gx_rectangle_left + 30;

    for(yoffset = 0; yoffset < 20; yoffset++)
    {
        gx_canvas_text_draw_ext(xoffset, size->gx_rectangle_top - yoffset, &string);
        gx_canvas_text_draw_ext(xoffset, size->gx_rectangle_bottom - yoffset, &string);

        xoffset+=30;
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
int      index;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = all_widgets_8bpp_rotated_cw_display_table[MAIN_DISPLAY].theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    gx_widget_draw_set(&button_screen, test_button_screen_draw);

    for(index = 0; index < 3; index++)
    {
        switch(index)
        {
        case 0:
            test_font_id = GX_FONT_ID_NORMAL_4BPP;
            sprintf(test_comment, "Test 3bit glyph draw");
            break;

        case 1:
            test_font_id = GX_FONT_ID_NORMAL_4BPP;
            root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_4bit_glyph_draw = _gx_display_driver_8bpp_rotated_glyph_4bit_draw;
            sprintf(test_comment, "Test 4bit glyph draw");
            break;

        case 2:
            test_font_id = GX_FONT_ID_SYSTEM;
            sprintf(test_comment, "Test 1bit glyph draw");
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





