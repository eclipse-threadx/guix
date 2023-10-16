/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_animation_stop", /* Test name */
    117, 100, 526, 253  /* Define the coordinates of the capture area.
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
#include "gx_animation.h"

char *test_comment[1];

static VOID control_thread_entry(ULONG input)
{
INT                frame_id=1;
GX_ANIMATION_INFO  info_1;
GX_ANIMATION_INFO  info_2;
GX_ANIMATION_INFO  info_3;
GX_ANIMATION      *animation_1;
GX_ANIMATION      *animation_2;
GX_ANIMATION      *animation_3;

    test_comment[0] = (char *)malloc(256);
    gx_system_timer_start((GX_WIDGET *)&button_screen, 1000, 1, 1);
    gx_system_animation_get(&animation_1);
    gx_system_animation_get(&animation_2);
    gx_system_animation_get(&animation_3);

    memset(&info_1, 0, sizeof(GX_ANIMATION_INFO));
    info_1.gx_animation_target = (GX_WIDGET *)&button_screen.button_screen_base_button_1;
    info_1.gx_animation_style = GX_ANIMATION_SINE_EASE_OUT;
    info_1.gx_animation_start_alpha = 128;
    info_1.gx_animation_end_alpha = 255;
    info_1.gx_animation_start_position.gx_point_x = 117;
    info_1.gx_animation_start_position.gx_point_y = 107;
    info_1.gx_animation_end_position.gx_point_x = 400;
    info_1.gx_animation_end_position.gx_point_y = 107;
    info_1.gx_animation_steps = 10;
    info_1.gx_animation_start_delay = 0;
    info_1.gx_animation_frame_interval = 1;
    info_1.gx_animation_id = 1;
    info_1.gx_animation_parent = (GX_WIDGET *)&button_screen;

    memset(&info_2, 0, sizeof(GX_ANIMATION_INFO));
    info_2.gx_animation_target = (GX_WIDGET *)&button_screen.button_screen_text_button_1;
    info_2.gx_animation_style = GX_ANIMATION_SINE_EASE_OUT;
    info_2.gx_animation_start_alpha = 128;
    info_2.gx_animation_end_alpha = 255;
    info_2.gx_animation_start_position.gx_point_x = 117;
    info_2.gx_animation_start_position.gx_point_y = 140;
    info_2.gx_animation_end_position.gx_point_x = 400;
    info_2.gx_animation_end_position.gx_point_y = 117;
    info_2.gx_animation_steps = 6;
    info_2.gx_animation_start_delay = 0;
    info_2.gx_animation_frame_interval = 1;
    info_2.gx_animation_id = 1;
    info_2.gx_animation_parent = (GX_WIDGET *)&button_screen;

    memset(&info_3, 0, sizeof(GX_ANIMATION_INFO));
    info_3.gx_animation_target = (GX_WIDGET *)&button_screen.button_screen_pixelmap_button1_1;
    info_3.gx_animation_style = GX_ANIMATION_SINE_EASE_OUT;
    info_3.gx_animation_start_alpha = 128;
    info_3.gx_animation_end_alpha = 255;
    info_3.gx_animation_start_position.gx_point_x = 117;
    info_3.gx_animation_start_position.gx_point_y = 176;
    info_3.gx_animation_end_position.gx_point_x = 400;
    info_3.gx_animation_end_position.gx_point_y = 176;
    info_3.gx_animation_steps = 3;
    info_3.gx_animation_start_delay = 0;
    info_3.gx_animation_frame_interval = 1;
    info_3.gx_animation_id = 1;
    info_3.gx_animation_parent = (GX_WIDGET *)&button_screen;

GX_ENTER_CRITICAL
 
    gx_animation_start(animation_3, &info_3);
    gx_animation_start(animation_1, &info_1);
    gx_animation_start(animation_2, &info_2);

    sprintf(test_comment[0], "capture animation frames");

    /* start_id, expected_frames, comments, num_comments, max_time. */
    gx_validation_capture_frames(frame_id, 10, (char **)test_comment, 1, 1000);
GX_EXIT_CRITICAL

    gx_validation_capture_frames_wait();

    gx_system_animation_get(&animation_1);
    animation_1->gx_animation_status = GX_ANIMATION_ACTIVE;
    gx_animation_stop(animation_1);

    free(test_comment[0]);
    gx_validation_end();

    exit(0);
}

