/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_non_linear_animation", /* Test name */
    490, 118, 785, 390  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_non_linear_animation.c"

char *test_comment[1];

typedef struct TEST_STRUCT{
    int            animation_style;
    char          *comment;
}TEST;

TEST test_list[]={
    {GX_ANIMATION_BACK_EASE_IN, "back ease in"},
    {GX_ANIMATION_BACK_EASE_OUT, "back ease out"},
    {GX_ANIMATION_BACK_EASE_IN_OUT, "back ease in out"},
    {GX_ANIMATION_BOUNCE_EASE_IN, "bounce ease in"},
    {GX_ANIMATION_BOUNCE_EASE_OUT, "bounce ease out"},
    {GX_ANIMATION_BOUNCE_EASE_IN_OUT, "bounce ease in out"},
    {GX_ANIMATION_CIRC_EASE_IN, "cric ease in"},
    {GX_ANIMATION_CIRC_EASE_OUT, "circ ease out"},
    {GX_ANIMATION_CIRC_EASE_IN_OUT, "circ ease in out"},
    {GX_ANIMATION_CUBIC_EASE_IN, "cubic ease in"},
    {GX_ANIMATION_CUBIC_EASE_OUT, "cubic ease out"},
    {GX_ANIMATION_CUBIC_EASE_IN_OUT, "cubic ease in out"},
    {GX_ANIMATION_ELASTIC_EASE_IN, "elastic ease in"},
    {GX_ANIMATION_ELASTIC_EASE_OUT, "elastic ease out"},
    {GX_ANIMATION_ELASTIC_EASE_IN_OUT, "elastic ease in out"},
    {GX_ANIMATION_EXPO_EASE_IN, "expo ease in"},
    {GX_ANIMATION_EXPO_EASE_OUT, "expo ease out"},
    {GX_ANIMATION_EXPO_EASE_IN_OUT, "expo ease in out"},
    {GX_ANIMATION_QUAD_EASE_IN, "quad ease in"},
    {GX_ANIMATION_QUAD_EASE_OUT, "quad ease out"},
    {GX_ANIMATION_QUAD_EASE_IN_OUT, "quad ease in out"},
    {GX_ANIMATION_QUART_EASE_IN, "quart ease in"},
    {GX_ANIMATION_QUART_EASE_OUT, "quart ease out"},
    {GX_ANIMATION_QUART_EASE_IN_OUT, "quart ease in out"},
    {GX_ANIMATION_QUINT_EASE_IN, "quint ease in"},
    {GX_ANIMATION_QUINT_EASE_OUT, "quint ease out"},
    {GX_ANIMATION_QUINT_EASE_IN_OUT, "quint ease in out"},
    {GX_ANIMATION_SINE_EASE_IN, "sin ease in"},
    {GX_ANIMATION_SINE_EASE_OUT, "sin ease out"},
    {GX_ANIMATION_SINE_EASE_IN_OUT, "sin ease in out"},
    {GX_ANIMATION_EASING_FUNC_MASK, " no animation operation."}
};

static VOID control_thread_entry(ULONG input)
{
INT   frame_id;
TEST *test = test_list;
int  index;

    for(index = 0; index < (int)(sizeof(test_list)/sizeof(TEST)); index++)
    {
        test_comment[0] = test->comment;

        gx_validation_current_frame_id_get(&frame_id);
        frame_id ++;

GX_ENTER_CRITICAL
        animation_style = test->animation_style;
        animation_start();
        generate_line_chart_data();

        /* start_id, expected_frames, comments, num_comments, max_time. */
        gx_validation_capture_frames(frame_id, TOTAL_STEPS, (char **)test_comment, 1, 1000);
GX_EXIT_CRITICAL
        gx_validation_capture_frames_wait();

        tx_thread_sleep(2);
        test++;
    }
    gx_validation_end();

    exit(0);
}

