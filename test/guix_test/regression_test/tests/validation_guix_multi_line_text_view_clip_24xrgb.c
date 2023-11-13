/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_line_text_view_clip_24xrgb", /* Test name */
    0, 0, 229, 189  /* Define the coordinates of the capture area.
                      In this test, we only need to capture the 
                      clip window and multi_line_text_view window
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_multi_line_text_view_clip_32bpp.c"

static char text[] = "0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz";

static int clip_height_value[] = {0, 3, 6, 9};

static int vertical_value[] = {150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10};

static int clip_width_value[] = {0, 3, 6, 9};

static int horizontal_value[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int clip_height_index, vertical_index, clip_width_index, horizontal_index;
int clip_height_first, clip_height, vertical, clip_width_first, clip_width, horizontal;
int frame_id = 1;

    gx_multi_line_text_view_text_set(&pMainWin -> main_window_text_view, text);
    clip_height_first = clip_1 -> gx_widget_size.gx_rectangle_bottom - clip_1 -> gx_widget_size.gx_rectangle_top + 1;
    for(clip_height_index = 0; clip_height_index < (INT)(sizeof(clip_height_value) / sizeof(int)); clip_height_index++)
    {
        gx_slider_value_set(v_slider_1, &v_slider_1 -> gx_slider_info, clip_height_value[clip_height_index]);
        clip_height = clip_height_first + (clip_height_value[clip_height_index] << 1);
        for(vertical_index = 0; vertical_index < (INT)(sizeof(vertical_value) / sizeof(int)); vertical_index++)
        {
            vertical = vertical_value[vertical_index];

            gx_validation_set_frame_id(frame_id);
            if(vertical < v_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_1 height %d move down %d", clip_height, v_slider_2 -> gx_slider_info.gx_slider_info_current_val - vertical);
            }
            else if(vertical > v_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_1 height %d move up %d", clip_height, vertical - v_slider_2 -> gx_slider_info.gx_slider_info_current_val);
            }
            else
            {
                sprintf(test_comment, "clip_1 height %d don't move", clip_height);
            }
            gx_validation_set_frame_comment(test_comment);

            gx_slider_value_set(v_slider_2, &v_slider_2 -> gx_slider_info, vertical_value[vertical_index]);

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pMainWin);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }

    clip_width_first = clip_2 -> gx_widget_size.gx_rectangle_right - clip_2 -> gx_widget_size.gx_rectangle_left + 1;
    for(clip_width_index = 0; clip_width_index < (INT)(sizeof(clip_width_value) / sizeof(int)); clip_width_index++)
    {
        gx_slider_value_set(h_slider_1, &h_slider_1 -> gx_slider_info, clip_width_value[clip_width_index]);
        clip_width = clip_width_first + (clip_width_value[clip_width_index] << 1);
        for(horizontal_index = 0; horizontal_index < (INT)(sizeof(horizontal_value) / sizeof(int)); horizontal_index++)
        {
            horizontal = horizontal_value[horizontal_index];

            gx_validation_set_frame_id(frame_id);
            if(horizontal < h_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_2 width %d move left %d", clip_width, h_slider_2 -> gx_slider_info.gx_slider_info_current_val - horizontal);
            }
            else if(horizontal > h_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_2 width %d move right %d", clip_width, horizontal - h_slider_2 -> gx_slider_info.gx_slider_info_current_val);
            }
            else
            {
                sprintf(test_comment, "clip_2 width %d don't move", clip_width);
            }
            gx_validation_set_frame_comment(test_comment);

            gx_slider_value_set(h_slider_2, &h_slider_2 -> gx_slider_info, horizontal_value[horizontal_index]);

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pMainWin);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
