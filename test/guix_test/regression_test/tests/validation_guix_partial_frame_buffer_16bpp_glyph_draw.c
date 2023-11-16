/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_partial_frame_buffer_16bpp_glyph_draw", /* Test name */
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

int test_font_ids[]={
    GX_FONT_ID_NORMAL_FONT,
    GX_FONT_ID_FONT_4BPP,
    GX_FONT_ID_FONT_1BPP,
};

char comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
int font_id;

    /* Toggle to glyph screen.  */
    ToggleScreen((GX_WINDOW *)&glyph_screen, (GX_WINDOW *)&button_screen);
    gx_widget_style_add(&glyph_screen.glyph_screen_view, GX_STYLE_USE_LOCAL_ALPHA);

    for(index = 0; index < (int)(sizeof(test_font_ids)/sizeof(int)); index++)
    {
        font_id = test_font_ids[index];
        sprintf(comment, "font id = %d, brush alpha = 255", font_id);
        glyph_screen.glyph_screen_view.gx_widget_alpha = 255;
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, font_id);
        gx_validation_screen_refresh();
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(comment);
        gx_validation_write_frame_buffer();

        glyph_screen.glyph_screen_view.gx_widget_alpha = 128;
        sprintf(comment, "font id = %d, brush alpha = 128", font_id);
        gx_validation_screen_refresh();
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(comment);
        gx_validation_write_frame_buffer();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





