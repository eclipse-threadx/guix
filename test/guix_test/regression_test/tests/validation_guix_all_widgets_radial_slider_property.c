/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_radial_slider_property", /* Test name */
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

INT animation_step = 0;
VOID animation_update_callback(GX_RADIAL_SLIDER *slider)
{
    animation_step++;
}

static VOID control_thread_entry(ULONG input)
{
int                    frame_id = 1;
GX_RADIAL_SLIDER      *slider;
GX_RADIAL_SLIDER_INFO *info;
int                    index;
GX_EVENT               my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
    ToggleScreen((GX_WINDOW *)&radial_slider_screen, (GX_WINDOW *)&button_screen);

    slider = &radial_slider_screen.radial_slider_screen_radial_slider;

    gx_radial_slider_animation_set(slider, 15, 2, GX_ANIMATION_CIRC_EASE_IN_OUT, animation_update_callback);

    gx_radial_slider_animation_start(slider, 180);
    while(animation_step != 15)
    {
        tx_thread_sleep(2);
    }

    for(index = 0; index < 6; index++)
    {
        switch(index)
        {
        case 0:
            gx_radial_slider_info_get(slider, &info);
            info->gx_radial_slider_info_needle_pixelmap = GX_PIXELMAP_ID_RADIO_ON;
            gx_radial_slider_info_set(slider, info);
            sprintf(test_comment, "reset radial slider info with new needle pixelmap");
            break;

        case 1:
            gx_radial_slider_pixelmap_set(slider, GX_PIXELMAP_ID_GUAGEMETER, GX_PIXELMAP_ID_SAVE_ICON);
            sprintf(test_comment, "reset radial slider background and needle pixelmap");
            break;

        case 2:
            gx_widget_hide(slider);
            gx_radial_slider_angle_set(slider, 90);
            sprintf(test_comment, "hide widget and set radial slider angle to 90");
            break;

        case 3:
            gx_radial_slider_animation_set(slider, 15, 0, GX_ANIMATION_CIRC_EASE_IN_OUT, GX_NULL);
            gx_radial_slider_animation_start(slider, 180);
            sprintf(test_comment, "set animation interval to 0, and start radial slider animation");
            break;

        case 4:
            animation_step = 0;
            gx_radial_slider_animation_set(slider, 15, 2, GX_ANIMATION_CIRC_EASE_IN_OUT, animation_update_callback);
            gx_radial_slider_animation_start(slider, 270);
            while(animation_step != 15)
            {
                tx_thread_sleep(2);
            }
            sprintf(test_comment, "set animation interval to 2, and start radial slider animation");
            break;

        case 5:
            gx_widget_show(slider);
            gx_radial_slider_pixelmap_set(slider, GX_NULL, GX_NULL);
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 223;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 331;
            my_event.gx_event_target = (GX_WIDGET *)slider;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "show widget, set radial slider background and needle pixelmap null, simulate click on (223, 331)");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
  
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
