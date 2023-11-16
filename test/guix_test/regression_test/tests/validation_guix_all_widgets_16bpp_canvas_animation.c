/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_16bpp_canvas_animation", /* Test name */
    50, 40, 345, 190  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets_16bpp.c"

char *test_comment[1];

static VOID control_thread_entry(ULONG input)
{
INT                frame_id=1;
GX_ANIMATION_INFO  info;
GX_ANIMATION       animation;
UINT               status;

    ToggleScreen((GX_WINDOW *)&window_screen, (GX_WINDOW *)&button_screen);

    gx_widget_shift((GX_WIDGET *)&NestedWindow, 10, 0, GX_FALSE);

    test_comment[0] = (char *)malloc(256);
GX_ENTER_CRITICAL
    memset(&info, 0, sizeof(GX_ANIMATION_INFO));
    memset(&animation, 0, sizeof(GX_ANIMATION));
    info.gx_animation_target = (GX_WIDGET *)&NestedWindow;
    info.gx_animation_style = 0;
    info.gx_animation_start_alpha = 128;
    info.gx_animation_end_alpha = 255;
    info.gx_animation_start_position.gx_point_x = animation_canvas.gx_canvas_display_offset_x;
    info.gx_animation_start_position.gx_point_y = animation_canvas.gx_canvas_display_offset_y;
    info.gx_animation_end_position.gx_point_x = animation_canvas.gx_canvas_display_offset_x + 10;
    info.gx_animation_end_position.gx_point_y = animation_canvas.gx_canvas_display_offset_y + 10;
    info.gx_animation_steps = 10;
    info.gx_animation_start_delay = 8;
    info.gx_animation_frame_interval = 10;
    info.gx_animation_id = 1;
    info.gx_animation_parent = (GX_WIDGET *)&window_screen;

    status = gx_animation_create(&animation);

    if(status == GX_SUCCESS)
    {
       status = gx_animation_canvas_define(&animation, &animation_canvas);
    }
    else
    {
        printf("\nanimation create failed!\n");
    }

    if(status == GX_SUCCESS)
    {
       status = gx_animation_start(&animation, &info);
    }
    else
    {
        printf("\ncanvas define failed!\n");
    }

    if(status == GX_SUCCESS)
    {
        /* start_id, expected_frames, comments, num_comments, max_time. */
        gx_validation_capture_frames(frame_id, 11, (char **)test_comment, 1, 1000);
    }
    else
    {
        printf("\nanimation start failed!\n");
    }

GX_EXIT_CRITICAL
    if(status == GX_SUCCESS)
    {
        gx_validation_capture_frames_wait();
    }

    gx_widget_show((GX_WIDGET *)&nested_root);
    gx_widget_shift((GX_WIDGET *)&NestedWindow, -NestedWindow.gx_widget_size.gx_rectangle_left, 0, GX_TRUE);
    sprintf(test_comment[0], "start animation again");
    gx_validation_current_frame_id_get(&frame_id);
    gx_animation_start(&animation, &info);
    gx_validation_capture_frames(frame_id, 11, (char **)test_comment, 1, 1000);
    gx_validation_capture_frames_wait();

    gx_widget_shift((GX_WIDGET *)&NestedWindow, -NestedWindow.gx_widget_size.gx_rectangle_left,
                    -NestedWindow.gx_widget_size.gx_rectangle_top, GX_TRUE);
    nested_root.gx_window_root_canvas = GX_NULL;

    if(gx_animation_start(&animation, &info) != GX_SUCCESS)
    {
        gx_canvas_show(&animation_canvas);
        sprintf(test_comment[0], "shift Nested window to left-top corner, and start an invalid animation");
        gx_validation_current_frame_id_get(&frame_id);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment[0]);
        gx_validation_screen_refresh();
    }

    free(test_comment[0]);
    gx_validation_end();

    exit(0);
}

