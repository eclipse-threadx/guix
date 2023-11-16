/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_font_8bpp", /* Test name */
    30, 35, 194, 154  /* Define the coordinates of the capture area.
                      In this test, we only need to capture the 
                      multi_line_text_view window
                      drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command canvas_text_view argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
    on the canvas_text_view drawing example. These operations 
    simulate user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual canvas_text_view routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_8bit_palette  
#undef win32_graphics_driver_setup_8bit_palette  
#endif
#define win32_graphics_driver_setup_8bit_palette  gx_validation_graphics_driver_setup_8bit_palette


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_multi_line_text_view_font_8bpp.c"

char test_comment[256];
extern GX_STUDIO_DISPLAY_INFO multi_line_text_view_font_8bpp_display_table[];

GX_RESOURCE_ID font_id[] = {GX_FONT_ID_SMALL_1BPP, GX_FONT_ID_LARGE_4BPP};
GX_CHAR font_name[][20] = {"1bpp font", "4bpp font"};

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
UINT font_index;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = multi_line_text_view_font_8bpp_display_table[DISPLAY_1].theme_table[DISPLAY_1_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    for(font_index = 0; font_index < 1; font_index ++)
    {
        /* Set frame id. */
        gx_validation_set_frame_id(frame_id);

        /* Set frame comment. */
        gx_validation_set_frame_comment(font_name[font_index]);

        /* Set font for text view widget. */
        gx_multi_line_text_view_font_set(&pMainWin -> main_window_text_view, font_id[font_index]);
    
        /* Mark the window "dirty" */
        gx_system_dirty_mark(pMainWin);
    
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    
        /* Increment frame ID */
        frame_id ++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
