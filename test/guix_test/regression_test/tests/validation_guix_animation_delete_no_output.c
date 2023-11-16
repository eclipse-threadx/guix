/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_animation_delete_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets_16bpp.c"

static VOID control_thread_entry(ULONG input)
{
int       failed_tests = 0;
UINT      status;
GX_ANIMATION *animation;
GX_ANIMATION *animation2;
GX_ANIMATION  animation3;
GX_ANIMATION_INFO info;

#ifndef GX_DISABLE_ERROR_CHECKING
    status = gx_animation_delete(GX_NULL, GX_NULL);
    EXPECT_EQ(status, GX_PTR_ERROR);
#endif

    memset(&animation3, 0, sizeof(GX_ANIMATION));
    gx_animation_create(&animation3);
    status = gx_animation_delete(&animation3, GX_NULL);
    EXPECT_EQ(status, GX_SUCCESS);

    /* Retrieve one animation from system animation pool.  */
    status = gx_system_animation_get(&animation);
    EXPECT_EQ(status, GX_SUCCESS);

    /* Delete the animation retrieved from system animation pool. */
    status = gx_animation_delete(animation, GX_NULL);
    EXPECT_EQ(status, GX_SUCCESS);

    status = gx_system_animation_get(&animation);
    EXPECT_EQ(status, GX_SUCCESS);

    memset(&info, 0, sizeof(GX_ANIMATION_INFO));
    info.gx_animation_parent = &button_screen;
    info.gx_animation_target = &button_screen.button_screen_title_1;
    info.gx_animation_start_alpha = 255;
    info.gx_animation_end_alpha = 255;
    info.gx_animation_start_position.gx_point_x = button_screen.gx_widget_size.gx_rectangle_left;
    info.gx_animation_start_position.gx_point_y = button_screen.gx_widget_size.gx_rectangle_top;
    info.gx_animation_end_position.gx_point_x = info.gx_animation_start_position.gx_point_x;
    info.gx_animation_end_position.gx_point_y = button_screen.gx_widget_size.gx_rectangle_right;
    info.gx_animation_steps = 200;
    info.gx_animation_frame_interval = 10;
    status = gx_animation_start(animation, &info);
    EXPECT_EQ(status, GX_SUCCESS);

    status = gx_system_animation_get(&animation2);
    EXPECT_EQ(status, GX_SUCCESS);

    info.gx_animation_target = &button_screen.button_screen_icon_button_6;
    info.gx_animation_parent = &button_screen.button_screen_text_button_1;
    status = gx_animation_start(animation2, &info);
    EXPECT_EQ(status, GX_SUCCESS);

    EXPECT_EQ(animation->gx_animation_status, GX_ANIMATION_ACTIVE);

    status = gx_animation_delete(GX_NULL, (GX_WIDGET *)&button_screen);
    EXPECT_EQ(status, GX_SUCCESS);

    EXPECT_EQ(animation->gx_animation_status, 0);
    
    if(failed_tests == 0) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
