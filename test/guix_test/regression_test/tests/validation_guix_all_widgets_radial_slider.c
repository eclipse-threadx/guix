/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_radial_slider", /* Test name */
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

char *test_comment[1];

typedef struct TEST_STRUCT{
    int event_type;
    GX_POINT point;
    char *comment;
}TEST;

TEST test_list[]={
    {GX_EVENT_PEN_DOWN, {406, 273}, "pen down on (406, 273)"},
    {GX_EVENT_PEN_UP, {406, 273}, "pen up on (406, 273)"},
    {GX_EVENT_PEN_DOWN, {400, 198}, "pen down on (400, 198)"},
    {GX_EVENT_PEN_UP,  {400, 198}, "pen up on (400, 198)"},
    {GX_EVENT_PEN_DOWN, {311, 133}, "pen down on (311, 133)"},
    {GX_EVENT_PEN_UP, {311, 133}, "pen up on (311, 133)"},
    {GX_EVENT_PEN_DOWN, {211, 176}, "pen down on(211, 176)"},
    {GX_EVENT_PEN_UP, {211, 176}, "pen up on (211, 176)"},
    {GX_EVENT_PEN_DOWN, {199, 241}, "pen down on (199, 241)"},
    {GX_EVENT_PEN_UP, {199, 241}, "pen up on (199, 241)"},
    {GX_EVENT_PEN_DOWN, {280, 335}, "pen down on (280, 335)"},
    {GX_EVENT_PEN_UP, {280, 335}, "pen up on (280, 335)"},
    {GX_EVENT_PEN_DOWN, {335, 343}, "pen down on (335, 243)"},
    {GX_EVENT_PEN_UP, {335, 343}, "pen up on (335, 343)"},
    {GX_EVENT_PEN_DOWN, {357, 299}, "pen down on (355, 299), not inside track"},
    {GX_EVENT_PEN_UP, {357, 299}, "pen up on (355, 299), not inside track"},
    {GX_EVENT_PEN_DOWN, {419, 318}, "pen down on (419, 318), not inside track)"},
    {GX_EVENT_PEN_UP, {419, 318}, "pen up on (419, 318), not inside track"},
    {GX_EVENT_PEN_DOWN, {357, 332}, "pen down on (357, 332)"},
    {GX_EVENT_PEN_DRAG, {404, 278}, "pen drag to (404, 278)"},
    {GX_EVENT_PEN_UP, {404, 278}, "pen up on (404, 278)"},
    {0, {0, 0}, ""}
};

static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
TEST    *test;
GX_EVENT my_event;
GX_RADIAL_SLIDER *slider;
GX_RADIAL_SLIDER_INFO *info;
INT x_center, y_center;

    memset(&my_event, 0, sizeof(GX_EVENT));
    test_comment[0] = (char *)malloc(256);

    ToggleScreen((GX_WINDOW *)&radial_slider_screen, (GX_WINDOW *)&button_screen);

    test = test_list;
    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_pointdata = test->point;

        if(test->event_type == GX_EVENT_PEN_UP)
        {
            sprintf(test_comment[0], "%s",  test->comment);
            gx_validation_current_frame_id_get(&frame_id);
            frame_id++;
            /* start_id, expected_frames, comments, num_comments, max_time. */
            gx_validation_capture_frames(frame_id, 15, (char **)test_comment, 1, 1000);
            gx_system_event_send(&my_event);

            gx_validation_capture_frames_wait();

            tx_thread_sleep(2);
        }
        else
        {
            gx_system_event_send(&my_event);
        }
        test++;
    }    

    gx_radial_slider_animation_set(&radial_slider_screen.radial_slider_screen_radial_slider, 0, 0, 0, GX_NULL);
    gx_radial_slider_anchor_angles_set(&radial_slider_screen.radial_slider_screen_radial_slider, radial_slider_anchor_list, sizeof(radial_slider_anchor_list) / sizeof(GX_VALUE));

    gx_validation_current_frame_id_get(&frame_id);
    frame_id++;

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment[0], "Diable animation, set anchor angle list.");
    gx_validation_set_frame_comment(test_comment[0]);
    gx_validation_screen_refresh();

    test = test_list;
    while(test->event_type)
    {
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_pointdata = test->point;
        gx_system_event_send(&my_event);
        test++;
    }

    slider = &radial_slider_screen.radial_slider_screen_radial_slider;
    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_display_handle = 1;

    /* Process unknown timer event. */
    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    my_event.gx_event_payload.gx_event_timer_id = 0;
    gx_system_event_send(&my_event);

    /* Send pen down event to a radial slider without GX_STATUS_ANIMATION_NONE flag. */
    gx_widget_status_remove((GX_WIDGET *)slider, GX_STATUS_ANIMATION_NONE);
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    /* Send pen drag event to a radial slider without GX_STATUS_TRACKING_PEN flag. */
    gx_widget_status_remove((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    gx_radial_slider_info_get(slider, &info);
    info -> gx_radial_slider_info_min_angle = 90;
    info -> gx_radial_slider_info_max_angle = 90;
    my_event.gx_event_display_handle = 1;
    gx_radial_slider_info_set(slider, info);

    /* new_value(270) > gx_radial_slider_info_max_val. */
    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center - 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center - 1;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    /* new_value(-90) < gx_radial_slider_info_min_val. */
    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center + 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center - 1;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    gx_radial_slider_info_get(slider, &info);
    info -> gx_radial_slider_info_min_angle = -90;
    info -> gx_radial_slider_info_max_angle = 270;
    gx_radial_slider_info_set(slider, info);

    /* previous_value < new_value but new_value - previous_value >= 180 */
    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center + 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center - 1;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    /* new_value(90) > previous_value(0) && current_angle(120) < new_value */
    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center - 1;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;

    gx_radial_slider_info_get(slider, &info);
    info -> gx_radial_slider_info_current_angle = 120;
    gx_radial_slider_info_set(slider, info);

    gx_system_event_send(&my_event);

    /* previous_value > new_value && current_angle > new_value */
    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center + 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    /* previous_value < new_value && new_value - previous_value >= 180 */
    gx_radial_slider_info_get(slider, &info);
    info -> gx_radial_slider_info_current_angle = -90;
    gx_radial_slider_info_set(slider, info);

    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center - 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event);

    /* previous_value > new_value && info_current_angle <= new_value */
    gx_radial_slider_info_get(slider, &info);
    info -> gx_radial_slider_info_current_angle = -90;
    gx_radial_slider_info_set(slider, info);

    x_center = slider -> gx_widget_size.gx_rectangle_left + info -> gx_radial_slider_info_xcenter;
    y_center = slider -> gx_widget_size.gx_rectangle_top + info -> gx_radial_slider_info_ycenter;
    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRACKING_PEN);

    gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_ANIMATION_NONE);
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center - 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event); /* previous_value == 90 */

    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_center + 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_center;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)slider;
    gx_system_event_send(&my_event); /* new_value == 0 */

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
