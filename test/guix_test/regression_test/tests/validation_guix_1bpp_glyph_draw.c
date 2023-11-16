/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_1bpp_glyph_draw", /* Test name */
   200, 30, 468, 80  /* Define the coordinates of the capture area.
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

#define TEST_1BPP_BLACK          0x00
#define TEST_1BPP_WHITE          0x01
#define TEST_1BPP_INVALID_COLOR  0x02


/* User-defined color table. */
static GX_COLOR test_color_table[] = 
{
    TEST_1BPP_BLACK,
    TEST_1BPP_WHITE,
    TEST_1BPP_INVALID_COLOR
};

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
#ifdef win32_graphics_driver_setup_monochrome  
#undef win32_graphics_driver_setup_monochrome  
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1bpp.c"

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_DRAW_CONTEXT context;

    ToggleScreen((GX_WINDOW *)&button_screen, (GX_WINDOW *)&sprite_screen); 
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Screen shot without changing");
    gx_validation_screen_refresh();

    gx_display_color_table_set(root->gx_window_root_canvas->gx_canvas_display, test_color_table, sizeof(test_color_table) / sizeof(GX_COLOR));

    /* Reset line wind draw function because line color is hard-coded in original draw function in demo file,
       Rewrite this draw function to change line color. */
    gx_widget_style_remove((GX_WIDGET *)&button_screen.button_screen_title_1, GX_STYLE_TRANSPARENT);
    gx_prompt_text_color_set(&button_screen.button_screen_title_1, TEST_1BPP_INVALID_COLOR, TEST_1BPP_INVALID_COLOR, TEST_1BPP_INVALID_COLOR);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set title prompt text color id to an invalid id");
    gx_validation_screen_refresh();

    memset(&context, 0, sizeof(GX_DRAW_CONTEXT));
    context.gx_draw_context_pitch = 640;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_write(&context, 201, 35, 2);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Call 1bpp pixel write with color set to 2");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}









