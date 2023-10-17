/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_washing_machine.h"

#define MIN_TEMPERATURE 30
#define MAX_TEMPERATURE 90

/******************************************************************************************/
/* Implement radial slider animation callback function.                                   */
/******************************************************************************************/
VOID temperature_slider_animation_callback(GX_RADIAL_SLIDER* slider)
{
    INT temperature;
    GX_RADIAL_SLIDER_INFO* info = &temperature_window.temperature_window_temperature_slider.gx_radial_slider_info;

    /* Convert angle value to temperature value.  */
    temperature = (info->gx_radial_slider_info_current_angle - info->gx_radial_slider_info_min_angle) *
                  (MAX_TEMPERATURE - MIN_TEMPERATURE) /
                  (info->gx_radial_slider_info_max_angle - info->gx_radial_slider_info_min_angle);

    temperature = MAX_TEMPERATURE - temperature;

    gx_numeric_prompt_value_set(&temperature_window.temperature_window_temperature_value, temperature);
    gx_numeric_prompt_value_set(&main_screen.main_screen_temperature_value, temperature);
}

/******************************************************************************************/
/* Override the default event processing of "temperature_window" to handle signals from   */
/* my child widgets.                                                                      */
/******************************************************************************************/
UINT temperature_window_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
    {
        gx_window_event_process(window, event_ptr);
        GX_RADIAL_SLIDER *slider = &temperature_window.temperature_window_temperature_slider;
        GX_RADIAL_SLIDER_INFO *info = &slider->gx_radial_slider_info;
        INT angle = info->gx_radial_slider_info_current_angle;

        /* Set temperature slider angle to max.  */
        gx_radial_slider_angle_set(slider, info->gx_radial_slider_info_max_angle);

        /* Start animation to move temperature slider angle from max to current temperature value.  */
        gx_radial_slider_animation_start(&temperature_window.temperature_window_temperature_slider, angle);
    }
        break;

    case GX_SIGNAL(ID_TEMPERATURE_SLIDER, GX_EVENT_SLIDER_VALUE):

        /* Update temperature value.  */
        temperature_slider_animation_callback(&temperature_window.temperature_window_temperature_slider);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Define custom temperature window draw function.                                        */
/******************************************************************************************/
VOID temperature_window_draw(GX_WINDOW* window)
{
    /* Draw window background.  */
    gx_window_background_draw(window);

    if (main_screen.main_screen_btn_play.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        /* Draw animation wave in the center.  */
        animation_wave_draw(window);
    }

    /* Draw child widgets.  */
    gx_widget_children_draw(window);
}