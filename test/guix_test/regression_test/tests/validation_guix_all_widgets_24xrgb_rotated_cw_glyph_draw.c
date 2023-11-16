/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_24xrgb_rotated_cw_glyph_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_24xrgb_rotated  
#undef win32_graphics_driver_setup_24xrgb_rotated
#endif
#define win32_graphics_driver_setup_24xrgb_rotated  gx_validation_graphics_driver_setup_24xrgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_24xrgb_rotated_cw.c"

static char test_comment[255];
static const char test_string[] = "Te";
static int test_alpha_index = 0;
static int test_font_index = 0;
static int font_id_list[]={
    GX_FONT_ID_NORMAL_4BPP,
    GX_FONT_ID_NORMAL_1BPP
};
static int alpha_list[]={255, 128, 0};

VOID test_button_screen_draw(GX_WINDOW *window)
{
GX_RECTANGLE *size;
GX_STRING string;
GX_BRUSH *brush;
int xoffset;
int yoffset;

    gx_window_draw(window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = alpha_list[test_alpha_index];

    size = &window->gx_window_client;


    gx_context_font_set(font_id_list[test_font_index]);
    gx_context_line_color_set(GX_COLOR_ID_PURPLE);

    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 1;

    yoffset = size->gx_rectangle_top - 10;
    for(xoffset = 0; xoffset < 20; xoffset++)
    {
        gx_canvas_text_draw_ext(size->gx_rectangle_left - xoffset, yoffset, &string);
        gx_canvas_text_draw_ext(size->gx_rectangle_right - xoffset, yoffset, &string);

        yoffset += 30;
    }

    gx_context_line_color_set(GX_COLOR_ID_ORANGE);

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
int frame_id = 1;

    gx_widget_draw_set(&button_screen, test_button_screen_draw);

    for(test_alpha_index = 0; test_alpha_index < sizeof(alpha_list)/sizeof(int); test_alpha_index++)
    {
        for(test_font_index = 0; test_font_index < sizeof(font_id_list)/sizeof(int); test_font_index++)
        {
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "font id = %d, brush alpha = %d", font_id_list[test_font_index], alpha_list[test_alpha_index]);

            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





