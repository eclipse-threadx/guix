/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"


TEST_PARAM test_parameter = {
    "guix_all_widgets_1bpp_block_move", /* Test name */
    50, 40, 567, 400  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_monochrome
#undef win32_graphics_driver_setup_monochrome
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1bpp.c"

typedef struct TEST_STRUCT{
    GX_WINDOW *window;
    INT        x_shift;
    INT        y_shift;
    GX_CHAR   *test_comment;
}TEST;

TEST test_list[] = {
    {(GX_WINDOW *)&(window_screen.window_screen_scroll_frame_1), 4, 0, "block move from left to right, shift value > client width"},
    {(GX_WINDOW *)&(window_screen.window_screen_scroll_frame_1), -4, 0, "block move from right to left, shift value > client width"},
    {(GX_WINDOW *)&(window_screen.window_screen_scroll_frame_1), 1, 0, "block move from left to right, shift value < client width"},
    {(GX_WINDOW *)&(window_screen.window_screen_scroll_frame_1), -1, 0, "block move from right to left, shift value < client width"},
    {(GX_WINDOW *)&(window_screen.window_screen_vertical_list), 0, 4, "block move from  top to bottom, shift value > client height"},
    {(GX_WINDOW *)&(window_screen.window_screen_vertical_list), 0, -4, "block move from bottom to top, shift value > client height"},
    {(GX_WINDOW *)&(window_screen.window_screen_vertical_list), 0, 1, "block move from top to bottom, shift value < client height"},
    {(GX_WINDOW *)&(window_screen.window_screen_vertical_list), 0, -1, "block move from bottom to top, shift value < client height"},
    {GX_NULL, 0, 0, 0}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT          frame_id = 1;
GX_VALUE     width;
GX_VALUE     height;
TEST        *test = test_list;

    ToggleScreen(pWindowScreen, pSpriteScreen);
    
    while(test->window)
    {
        gx_widget_width_get((GX_WIDGET *)test->window, &width);
        gx_widget_height_get((GX_WIDGET *)test->window, &height);
        
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->test_comment);
        gx_widget_block_move((GX_WIDGET *)test->window, &test->window->gx_window_client, width * test->x_shift / 2, height * test->y_shift / 2);

        if(GX_ABS(test->x_shift) > 1 || GX_ABS(test->y_shift) > 1)
        {
            gx_validation_screen_refresh();
        }
        test++;
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
