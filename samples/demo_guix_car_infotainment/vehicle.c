/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

/* Define total number animatin steps.  */
#define TOTAL_NUMBER_ANIMATION_STEPS 10

#define CURRENT_NUMBER_VAL_GET(target_val) \
            (target_val * number_animation_step / TOTAL_NUMBER_ANIMATION_STEPS)

/* Define sprite frame lists for light animation.  */
GX_SPRITE_FRAME light_frame_list_1[] = {
    {GX_PIXELMAP_ID_LLIGHT01, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT02, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT03, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT04, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT05, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT06, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT07, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT08, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT09, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT10, 0, 0, 5, 0, 255},
    {GX_NULL, 0, 0, 5, 0, 255}
};

GX_SPRITE_FRAME light_frame_list_2[] = {
    {GX_NULL, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT10, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT09, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT08, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT07, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT06, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT05, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT04, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT03, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT02, 0, 0, 5, 0, 255},
    {GX_PIXELMAP_ID_LLIGHT01, 0, 0, 5, 0, 255}
};

/* Define variables.  */
static INT number_animation_step;
static INT target_battery_usage = 86;
static INT target_miles = 382;
static INT target_washer_level = 54;
static INT target_light_level = 77;
static INT target_air_filter_usage = 74;
static INT target_front_left_brake_pad_usage = 61;
static INT target_front_right_brake_pad_usage = 65;
static INT target_rear_left_brake_pad_usage = 71;
static INT target_rear_right_brake_pad_usage = 75;
static INT target_front_left_tire_pressure = 35;
static INT target_front_right_tire_pressure = 35;
static INT target_rear_left_tire_pressure = 38;
static INT target_rear_right_tire_pressure = 38;

/******************************************************************************************/
/* Start number animation.                                                                */
/******************************************************************************************/
static VOID number_animation_start()
{
    number_animation_step = 0;
    gx_system_timer_start(&vehicle_screen, TIMER_ID_VEHICLE_NUMBER_ANIMATION, 40 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);
}

/******************************************************************************************/
/* Update number animation.                                                               */
/******************************************************************************************/
static VOID number_animation_update()
{
    number_animation_step++;

    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_battery_usage, CURRENT_NUMBER_VAL_GET(target_battery_usage));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_miles, CURRENT_NUMBER_VAL_GET(target_miles));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_windshield_washer_level, CURRENT_NUMBER_VAL_GET(target_washer_level));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_lights_bulbs, CURRENT_NUMBER_VAL_GET(target_light_level));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_air_filter_usage, CURRENT_NUMBER_VAL_GET(target_air_filter_usage));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_front_left_brake_pad, CURRENT_NUMBER_VAL_GET(target_front_left_brake_pad_usage));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_front_right_brake_pad, CURRENT_NUMBER_VAL_GET(target_front_right_brake_pad_usage));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_rear_left_brake_pad, CURRENT_NUMBER_VAL_GET(target_rear_left_brake_pad_usage));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_rear_right_brake_pad, CURRENT_NUMBER_VAL_GET(target_rear_right_brake_pad_usage));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_front_left_tire_pressure, CURRENT_NUMBER_VAL_GET(target_front_left_tire_pressure));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_front_right_tire_pressure, CURRENT_NUMBER_VAL_GET(target_front_right_tire_pressure));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_rear_left_tire_pressure, CURRENT_NUMBER_VAL_GET(target_rear_left_tire_pressure));
    gx_numeric_prompt_value_set(&vehicle_screen.vehicle_screen_rear_right_tire_pressure, CURRENT_NUMBER_VAL_GET(target_rear_right_tire_pressure));

    if (number_animation_step == TOTAL_NUMBER_ANIMATION_STEPS)
    {
        gx_system_timer_stop(&vehicle_screen, TIMER_ID_VEHICLE_NUMBER_ANIMATION);
    }
}

/******************************************************************************************/
/* Override the default event processing of "vehicle_screen" to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT vehicle_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_SPRITE *sprite;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Start number animation. */
        number_animation_start();

        screen_base_event_process(window, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TIMER_ID_VEHICLE_NUMBER_ANIMATION)
        {
            /* Update number animation. */
            number_animation_update();
        }
        else
        {
            screen_base_event_process(window, event_ptr);
        }
        break;

    case GX_SIGNAL(ID_LIGHT_SPRITE_LEFT, GX_EVENT_SPRITE_COMPLETE):
    case GX_SIGNAL(ID_LIGHT_SPRITE_RIGHT, GX_EVENT_SPRITE_COMPLETE):
        gx_widget_find(window, event_ptr->gx_event_sender, 2, &sprite);

        /* Toggle light frame list. */
        if (sprite->gx_sprite_frame_list[0].gx_sprite_frame_pixelmap == light_frame_list_1[0].gx_sprite_frame_pixelmap)
        {
            gx_sprite_frame_list_set(sprite, light_frame_list_2, sizeof(light_frame_list_2) / sizeof(GX_SPRITE_FRAME));
        }
        else
        {
            gx_sprite_frame_list_set(sprite, light_frame_list_1, sizeof(light_frame_list_1) / sizeof(GX_SPRITE_FRAME));
        }

        gx_sprite_start(sprite, 0);
        break;

    case GX_SIGNAL(ID_MODE_ECO_BTN, GX_EVENT_RADIO_SELECT):
        /* Update vechile mode to ECO. */
        gx_prompt_text_id_set(&main_screen.main_screen_vehicle_mode_text, GX_STRING_ID_ECO);
        gx_icon_pixelmap_set(&main_screen.main_screen_vehicle_mode_icon, GX_PIXELMAP_ID_V_MODE_BTN_ICON_ECO, GX_PIXELMAP_ID_V_MODE_BTN_ICON_ECO);
        break;

    case GX_SIGNAL(ID_MODE_COMFORT_BTN, GX_EVENT_RADIO_SELECT):
        /* Update vechile mode to Comfort. */
        gx_prompt_text_id_set(&main_screen.main_screen_vehicle_mode_text, GX_STRING_ID_COMFORT);
        gx_icon_pixelmap_set(&main_screen.main_screen_vehicle_mode_icon, GX_PIXELMAP_ID_HOME_MODE_BTN_ICON_COMFORT, GX_PIXELMAP_ID_HOME_MODE_BTN_ICON_COMFORT);
        break;

    case GX_SIGNAL(ID_MODE_SPORT_BTN, GX_EVENT_RADIO_SELECT):
        /* Update vechile mode to Sport. */
        gx_prompt_text_id_set(&main_screen.main_screen_vehicle_mode_text, GX_STRING_ID_SPORT);
        gx_icon_pixelmap_set(&main_screen.main_screen_vehicle_mode_icon, GX_PIXELMAP_ID_HOME_MODE_BTN_ICON_SPORT, GX_PIXELMAP_ID_HOME_MODE_BTN_ICON_SPORT);
        break;

    default:
        return screen_base_event_process(window, event_ptr);
    }

    return 0;
}