/* This is a smaable animation, set anchor angle list."ll demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radial_slider_event_process", /* Test name */
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

typedef struct TEST_STRUCT{
int event_type;
int xpos;
int ypos;
}TEST;

TEST test_list[]=
{
{GX_EVENT_PEN_DOWN, 407, 195},
{GX_EVENT_PEN_UP,   446, 175},
{GX_EVENT_PEN_DOWN, 407, 195},
{GX_EVENT_PEN_UP,   400, 195},
{GX_EVENT_PEN_DOWN, 403, 189},
{GX_EVENT_PEN_UP,   405, 157},
{GX_EVENT_PEN_DOWN, 403, 189},
{GX_EVENT_PEN_UP,   403, 200},
{0,                   0,   0}
};
static VOID control_thread_entry(ULONG input)
{
int               frame_id = 1;
GX_EVENT          my_event;
GX_RADIAL_SLIDER *slider;
TEST             *test = test_list;

    memset(&my_event, 0, sizeof(GX_EVENT));

    ToggleScreen((GX_WINDOW *)&radial_slider_screen, (GX_WINDOW *)&button_screen);

    slider = &radial_slider_screen.radial_slider_screen_radial_slider;
    gx_radial_slider_animation_set(slider, 0, 0, 0, GX_NULL);

    my_event.gx_event_target = (GX_WIDGET *)slider;

    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->xpos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->ypos;
        gx_system_event_send(&my_event);
  
        switch(test->event_type)
        {
        case GX_EVENT_PEN_DOWN:
            sprintf(test_comment, "pen down on (%d, %d)", test->xpos, test->ypos);
            break;

        case GX_EVENT_PEN_UP:
            sprintf(test_comment, "pen up on (%d, %d)", test->xpos, test->ypos);
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
        test++;
    }

    gx_widget_style_remove(slider, GX_STYLE_ENABLED);
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 403;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 189;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "removed GX_STYLE_ENABLED style, simulate click on(403, 189)");

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    my_event.gx_event_payload.gx_event_timer_id = GX_RADIAL_SLIDER_TIMER;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("send a timer event to the slider.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
