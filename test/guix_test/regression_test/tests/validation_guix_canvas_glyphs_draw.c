/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_canvas.h"

TEST_PARAM test_parameter = {
    "guix_canvas_glyphs_draw", /* Test name */
     0, 0, 640, 480      /* Define the coordinates of the capture area.
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
#ifdef win32_dave2d_graphics_driver_setup_565rgb 
#undef win32_dave2d_graphics_driver_setup_565rgb  
#endif
#define win32_dave2d_graphics_driver_setup_565rgb  gx_validation_dave2d_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_synergy_glyph_draw_16bpp.c"

char test_comment[256];

GX_PROMPT *test_prompt = &window.window_prompt_4;
GX_UBYTE test_string[] = {'c', 'h', 0xff, 0x00};
GX_UBYTE test_string_2[] = {'t', 'e', 's', 't', '2', 0xc0, 0x80, 0x00};

void test_window_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_DRAW_CONTEXT context;
GX_STRING string;

   gx_window_background_draw(window);

   xpos = window->gx_widget_size.gx_rectangle_left + 10;
   ypos = window->gx_widget_size.gx_rectangle_top + 100;

   /* Test compressed glyph clip. */
   gx_prompt_draw(test_prompt);

   gx_context_line_color_set(GX_COLOR_ID_TEXT);
   gx_context_font_set(GX_FONT_ID_CHINESE_1BIT_COMPRESS);
   gx_canvas_text_draw(xpos, ypos, "compress", 8);

   /* Test draw width invalid glyph data. */
   ypos += 60;
   gx_canvas_text_draw(xpos, ypos, (char *)test_string_2, -1);

   ypos += 60;
   gx_canvas_text_draw(xpos, ypos, (char *)test_string, -1);

   ypos += 100;
   gx_context_font_set(GX_FONT_ID_SYSTEM);
   gx_canvas_text_draw(xpos, ypos, (char *)test_string, -1);

   /* Test draw width invalid glyph data. */
   ypos += 60;
   gx_canvas_text_draw(xpos, ypos, (char *)test_string_2, -1);

   /* Test draw with invalid context. */
   gx_context_font_set(100);
   context.gx_draw_context_brush.gx_brush_font = GX_NULL;
   string.gx_string_ptr = (char *)test_string;
   string.gx_string_length = sizeof(test_string) - 1;
   _gx_canvas_glyphs_draw(&context, GX_NULL, &string, GX_NULL, GX_NULL);
   _gx_canvas_glyphs_draw(&context, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_RECTANGLE size;

    gx_widget_draw_set(&window, test_window_draw);

    gx_utility_rectangle_define(&size, -71, -93, 444, 12);
    gx_widget_resize(test_prompt, &size);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test compressed glyph draw");
    gx_validation_screen_refresh();
 
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}


