/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_single_line_text_input_clip_32xrgb", /* Test name */
    20, 25, 230, 200  /* Define the coordinates of the capture area.
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

#include "demo_guix_single_line_text_input_clip_32bpp.c"

static int clip_height_value[] = {0, 3, 6, 9};

static int clip_width_value[] = {0, 3, 6, 9};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int clip_height_index, clip_width_index;
int clip_height_first, clip_height, clip_width_first, clip_width;
int frame_id = 1;
GX_SLIDER_INFO *slider_info;
int slider_value;

    clip_height_first = clip_1 -> gx_widget_size.gx_rectangle_bottom - clip_1 -> gx_widget_size.gx_rectangle_top + 1;
    for(clip_height_index = 0; clip_height_index < (INT)(sizeof(clip_height_value) / sizeof(int)); clip_height_index++)
    {
        gx_slider_value_set(v_slider_1, &v_slider_1 -> gx_slider_info, clip_height_value[clip_height_index]);
        clip_height = clip_height_first + (clip_height_value[clip_height_index] << 1);

        /* Get slider info. */
        slider_info = &v_slider_2 -> gx_slider_info;

        for(slider_value = slider_info -> gx_slider_info_max_val; slider_value >= slider_info -> gx_slider_info_min_val; slider_value -= 10)
        {
            gx_validation_set_frame_id(frame_id);
            if(slider_value < v_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_1 height %d move down", clip_height);
            }
            else if(slider_value > v_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_1 height %d move up", clip_height);
            }
            else
            {
                sprintf(test_comment, "clip_1 height %d don't move", clip_height);
            }
            gx_validation_set_frame_comment(test_comment);

            gx_slider_value_set(v_slider_2, slider_info, slider_value);

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

        /* Get slider info. */
        slider_info = &h_slider_2 -> gx_slider_info;

        for(slider_value = slider_info -> gx_slider_info_min_val; slider_value <= slider_info -> gx_slider_info_max_val; slider_value += 10)
        {
            gx_validation_set_frame_id(frame_id);
            if(slider_value < h_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_2 width %d move left", clip_width);
            }
            else if(slider_value > h_slider_2 -> gx_slider_info.gx_slider_info_current_val)
            {
                sprintf(test_comment, "clip_2 width %d move right", clip_width);
            }
            else
            {
                sprintf(test_comment, "clip_2 width %d don't move", clip_width);
            }
            gx_validation_set_frame_comment(test_comment);

            gx_slider_value_set(h_slider_2, slider_info, slider_value);

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
