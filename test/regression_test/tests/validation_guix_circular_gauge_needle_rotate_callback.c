/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_circular_gauge_needle_rotate_callback", /* Test name */
    167, 80, 437, 360  /* Define the coordinates of the capture area.
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
#include "demo_guix_all_widgets.c"

char test_comment[255];

static VOID display_driver_callback_assign(UINT callback(void *), void *widget){ callback(widget);}

GX_POINT needle_pos[]={
{100, 120},
{140, 142},
{180, 170}
};

GX_POINT needle_cor[]={
{0, 100},
{18, 88},
{10, 10}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                frame_id = 1;
int                angle;
int                pos;
GX_CIRCULAR_GAUGE *gauge;
GX_CIRCULAR_GAUGE_INFO *gauge_info;
GX_DISPLAY        *display;
    
    display = root->gx_window_root_canvas->gx_canvas_display;
    display->gx_display_driver_callback_assign = display_driver_callback_assign;
    gauge = &gauge_screen.gauge_screen_gauge_1;

    gx_circular_gauge_animation_set(gauge, 0, 1);

    /* Toggle from button screen to gauge screen. */
    ToggleScreen((GX_WINDOW *)&gauge_screen, (GX_WINDOW *)&button_screen);

    for(pos = 0; pos < (int)(sizeof(needle_pos)/sizeof(GX_POINT)); pos++)
    {
        gauge_info = &gauge->gx_circular_gauge_info;

        gauge_info->gx_circular_gauge_info_needle_xpos = needle_pos[pos].gx_point_x;
        gauge_info->gx_circular_gauge_info_needle_ypos = needle_pos[pos].gx_point_y;
        gauge_info->gx_circular_gauge_info_needle_xcor = needle_cor[pos].gx_point_x;
        gauge_info->gx_circular_gauge_info_needle_ycor = needle_cor[pos].gx_point_y;

        for (angle = -180; angle <= 180; angle +=10)
        {
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "angle:%d, needle_pos:(%d, %d), needle_xcor:(%d, %d)",
                    angle,
                    needle_pos[pos].gx_point_x, needle_pos[pos].gx_point_y,
                    needle_pos[pos].gx_point_y, needle_pos[pos].gx_point_y);
            gx_validation_set_frame_comment(test_comment);

            gx_circular_gauge_angle_set(gauge, angle);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
