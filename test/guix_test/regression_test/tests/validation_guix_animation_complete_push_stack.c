/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_animation_complete_push_stack", /* Test name */
    117, 100, 526, 151  /* Define the coordinates of the capture area.
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
#define SCREEN_STACK_SIZE 8
GX_WIDGET *screen_stack_memory[SCREEN_STACK_SIZE];

static VOID control_thread_entry(ULONG input)
{
INT                frame_id=1;
GX_ANIMATION_INFO  info;
GX_ANIMATION       animation;
GX_BUTTON          button;
GX_RECTANGLE       size;
GX_WIDGET         *target;

    gx_system_screen_stack_create(screen_stack_memory, sizeof(GX_WIDGET *) * SCREEN_STACK_SIZE);

    gx_utility_rectangle_define(&size, 0, 0, 100, 50);

    /* Create a button for animation. */
    memset(&button, 0, sizeof(GX_BUTTON));
    gx_button_create(&button, "button", GX_NULL, GX_STYLE_ENABLED, 1000, &size);

    gx_widget_detach((GX_WIDGET *)&NestedWindow);

    test_comment[0] = (char *)malloc(256);

    memset(&info, 0, sizeof(GX_ANIMATION_INFO));
    memset(&animation, 0, sizeof(GX_ANIMATION));
    info.gx_animation_target = (GX_WIDGET *)&button;
    info.gx_animation_style = GX_ANIMATION_SINE_EASE_OUT | GX_ANIMATION_PUSH_STACK;
    info.gx_animation_start_alpha = 255;
    info.gx_animation_end_alpha = 255;
    info.gx_animation_start_position.gx_point_x = 100;
    info.gx_animation_start_position.gx_point_y = 107;
    info.gx_animation_end_position.gx_point_x = 400;
    info.gx_animation_end_position.gx_point_y = 107;
    info.gx_animation_steps = 10;
    info.gx_animation_start_delay = 0;
    info.gx_animation_frame_interval = 1;
    info.gx_animation_id = 1;
    info.gx_animation_parent = (GX_WIDGET *)&button_screen;

GX_ENTER_CRITICAL
    gx_animation_create(&animation);
    gx_canvas_create(&animation_canvas, GX_NULL,
        all_widgets_16bpp_display_table[0].display,
        GX_CANVAS_MANAGED_VISIBLE,
        NESTED_WINDOW_WIDTH, NESTED_WINDOW_HEIGHT,
        animation_canvas_memory, NESTED_WINDOW_WIDTH * NESTED_WINDOW_HEIGHT * sizeof(GX_COLOR));

    gx_animation_canvas_define(&animation, &animation_canvas);
    tx_thread_sleep(1);
    gx_animation_start(&animation, &info);

    sprintf(test_comment[0], "Canvas animation");

    /* start_id, expected_frames, comments, num_comments, max_time. */
    gx_validation_capture_frames(frame_id, 10, (char **)test_comment, 1, 20);
GX_EXIT_CRITICAL

    gx_validation_capture_frames_wait();

    gx_system_screen_stack_get(GX_NULL, &target);
    if(target != (GX_WIDGET *)&button)
    {
        exit(1);
    }
    free(test_comment[0]);
    gx_validation_end();

    exit(0);
}

