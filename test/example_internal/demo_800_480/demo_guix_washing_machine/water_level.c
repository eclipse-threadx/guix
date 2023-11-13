/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_washing_machine.h"

#define MIN_WATER_LEVEL 20
#define MAX_WATER_LEVEL 80

/* Define widget angle map structure.  */
typedef struct widget_angle_map_struct {
    INT        angle;
    GX_WIDGET *widget;
}widget_angle_map;

/* Define map list between radial slider angle and water level button widget.  */
widget_angle_map map_list[] = {
    {-40, (GX_WIDGET *) &water_level_window.water_level_window_btn_min},
    {-20, (GX_WIDGET*) &water_level_window.water_level_window_btn_low},
    {0, (GX_WIDGET*) &water_level_window.water_level_window_btn_medium},
    {20, (GX_WIDGET*) &water_level_window.water_level_window_btn_high},
    {40, (GX_WIDGET*) &water_level_window.water_level_window_btn_max},
    {0, GX_NULL}
};

/* Define water level radial slider anchor angle values.  */
GX_VALUE water_level_anchor_angle_values[] = { -40, -20, 0, 20, 40 };

/******************************************************************************************/
/* Initialize radial slider in water level window.                                        */
/******************************************************************************************/
VOID water_level_window_init()
{
    gx_radial_slider_anchor_angles_set(&water_level_window.water_level_window_water_level_slider,
                                       water_level_anchor_angle_values,
                                       sizeof(water_level_anchor_angle_values) / sizeof(GX_VALUE));
}

/******************************************************************************************/
/* Retrieved angle value by selected button widget.                                       */
/******************************************************************************************/
INT get_angle(GX_WIDGET *widget)
{
    widget_angle_map* entry = map_list;

    while (entry->widget)
    {
        if (entry->widget == widget)
        {
            return entry->angle;
        }
        entry++;
    }
    
    return 0;
}

/******************************************************************************************/
/* Retrieved button widget by angle value.                                                */
/******************************************************************************************/
GX_WIDGET *get_button_widget(INT angle)
{
    widget_angle_map* entry = map_list;

    while (entry->widget)
    {
        if (entry->angle == angle)
        {
            return entry->widget;
        }
        entry++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Start animation to move radial slider value from min angle to current angle.           */
/******************************************************************************************/
VOID start_water_level_slider_animation()
{
    GX_RADIAL_SLIDER *slider = &water_level_window.water_level_window_water_level_slider;
    GX_RADIAL_SLIDER_INFO *info = &slider->gx_radial_slider_info;
    INT water_level;

    water_level = info->gx_radial_slider_info_current_angle;
    gx_radial_slider_angle_set(slider, info->gx_radial_slider_info_min_angle);
    gx_radial_slider_animation_start(slider, water_level);
}

/******************************************************************************************/
/* Update numeric water level values.                                                     */
/******************************************************************************************/
VOID update_water_level_numeric_values(INT value)
{
    GX_RADIAL_SLIDER_INFO *info = &water_level_window.water_level_window_water_level_slider.gx_radial_slider_info;
    INT water_level;

    /* Convert angle value to temperature value.  */
    water_level = (info->gx_radial_slider_info_current_angle - info->gx_radial_slider_info_min_angle) *
                  (MAX_WATER_LEVEL - MIN_WATER_LEVEL) /
                  (info->gx_radial_slider_info_max_angle - info->gx_radial_slider_info_min_angle);

    water_level = MIN_WATER_LEVEL + water_level;

    /* Update numeric water level values.  */
    gx_numeric_prompt_value_set(&water_level_window.water_level_window_water_level_value, water_level);
    gx_numeric_prompt_value_set(&main_screen.main_screen_water_level_value, water_level);
}

/******************************************************************************************/
/* Update water level slider values.                                                      */
/******************************************************************************************/
VOID update_water_level_slider_value(INT selected_button_id)
{
    GX_WIDGET *widget;

    gx_widget_find((GX_WIDGET *) &water_level_window, selected_button_id, GX_SEARCH_DEPTH_INFINITE, &widget);

    if (widget)
    {
        gx_radial_slider_animation_start(&water_level_window.water_level_window_water_level_slider, get_angle(widget));
    }
}

/******************************************************************************************/
/* Override the default event processing of "water_level_window" to handle signals from   */
/* my child widgets.                                                                      */
/******************************************************************************************/
UINT water_level_window_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);

        start_water_level_slider_animation();
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_SLIDER, GX_EVENT_SLIDER_VALUE):
        update_water_level_numeric_values(event_ptr->gx_event_payload.gx_event_longdata);
        break;


    case GX_SIGNAL(ID_BTN_MAX, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BTN_HIGH, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BTN_MEDIUM, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BTN_LOW, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BTN_MIN, GX_EVENT_RADIO_SELECT):
        update_water_level_slider_value(event_ptr->gx_event_sender);
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_SLIDER, GX_EVENT_ANIMATION_COMPLETE):
        gx_button_select((GX_BUTTON*)get_button_widget(water_level_window.water_level_window_water_level_slider.
                                                       gx_radial_slider_info.gx_radial_slider_info_current_angle));
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
