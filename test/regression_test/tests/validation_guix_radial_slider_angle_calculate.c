/* This is a smaable animation, set anchor angle list."ll demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radial_slider_angle_calculate", /* Test name */
    47, 100, 542, 370  /* Define the coordinates of the capture area.
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
#include "gx_system.h"

#include "demo_guix_all_widgets.c"

char test_comment[255];

static VOID control_thread_entry(ULONG input)
{
int               frame_id = 1;
GX_EVENT          my_event;
GX_RADIAL_SLIDER *slider;
GX_RADIAL_SLIDER_INFO *info;

    memset(&my_event, 0, sizeof(GX_EVENT));

    ToggleScreen((GX_WINDOW *)&radial_slider_screen, (GX_WINDOW *)&button_screen);

    slider = &radial_slider_screen.radial_slider_screen_radial_slider;
    info = &slider->gx_radial_slider_info;
    info->gx_radial_slider_info_track_width = info->gx_radial_slider_info_radius * 2;
    gx_radial_slider_animation_set(slider, 0, 0, 0, GX_NULL);

    my_event.gx_event_target = (GX_WIDGET *)slider;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 306;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 235;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);
    
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Make track width twice of radius, and click on radial slider center");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
