/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"


/* Define variables. */
#define VEHICLE_ANIMATION_TIMER 0x10
#define TOTAL_ANIMATION_STEPS 100

#define TARGET_ENGINE_OIL 987
#define TARGET_TRANSMISSION 959
#define TARGET_POWER_STEERING 965
#define TARGET_COOLANT 914
#define TARGET_BRAKES 981
#define TARGET_WINDSHIELD_WASHER 86
#define TARGET_BATTERY 97
#define TARGET_AIR_FILTER 83
#define TARGET_BRAKES_F 71
#define TARGET_BRAKES_R 75
#define TARGET_TIRE_PRESSURE_1 35
#define TARGET_TIRE_PRESSURE_2 35
#define TARGET_TIRE_PRESSURE_3 40
#define TARGET_TIRE_PRESSURE_4 40
#define START_DAY 13
#define TARGET_DAY 18
#define START_RADIAL_PROGRESS_BAR_VAL -90
#define TARGET_RADOAL_PROGRESS_BAR_VAL -130

int remain_animation_steps = 0;

/* Define prototypes. */
VOID vehicle_animation_start();
VOID vehicle_animation_update();

/******************************************************************************************/
/* Override default drawing of "vehicle_screen" to do custom drawing.                     */
/******************************************************************************************/
VOID vehicle_screen_draw(GX_WINDOW *window)
{
    /* Call default window draw. */
    gx_window_draw(window);

    gx_context_brush_width_set(1);

    /* Set line color to orange. */
    gx_context_line_color_set(GX_COLOR_ID_ORANGE);

    /* Draw a line under "Engine oil" prompt. */
    gx_canvas_line_draw(226, 333, 352, 333);

    /* Set line color to purple. */
    gx_context_line_color_set(GX_COLOR_ID_PURPLE);

    /* Draw a line under "Transmission" prompt. */
    gx_canvas_line_draw(226, 360, 352, 360);

    /* Set line color to silvery. */
    gx_context_line_color_set(GX_COLOR_ID_SILVERY);

    /* Draw a line under "Power Steering" prompt. */
    gx_canvas_line_draw(226, 387, 352, 387);

    /* Set line color to blue. */
    gx_context_line_color_set(GX_COLOR_ID_BLUE);

    /* Draw a line under "Coolant" prompt. */
    gx_canvas_line_draw(226, 414, 352, 414);

    /* Set line color to green. */
    gx_context_line_color_set(GX_COLOR_ID_GREEN);

    /* Draw a line under "Brakes" prompt. */
    gx_canvas_line_draw(226, 441, 352, 441);
}

/******************************************************************************************/
/* Override default event processing of "vehicle_screen" to handle signals from my child  */
/* widgets.                                                                               */
/******************************************************************************************/
UINT vehicle_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Call default event process. */
        base_screen_event_process(window, event_ptr);

        /* Start vehicle screen animation. */
        vehicle_animation_start();
        break;

    case GX_SIGNAL(ID_RELOAD, GX_EVENT_CLICKED):
        /* Start vehicle screen animation. */
        vehicle_animation_start();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == VEHICLE_ANIMATION_TIMER)
        {
            /* Update vehicle screen animation. */
            vehicle_animation_update();
        }
        break;
    default:
        return base_screen_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Set a progress bar value according to current animation step.                          */
/******************************************************************************************/
VOID progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, GX_PROMPT *prompt, INT start_value, INT target_value)
{
INT       value;
GX_CHAR   string_buffer[10];
GX_STRING text;

    /* Calculate progress bar value. */
    value = start_value + target_value * (TOTAL_ANIMATION_STEPS - remain_animation_steps) / TOTAL_ANIMATION_STEPS;

    /* Update progress bar value. */
    gx_progress_bar_value_set(progress_bar, value / 10);

    text.gx_string_ptr = string_buffer;

    /* Convert progres bar value to string. */
    gx_utility_ltoa(value / 10, string_buffer, 10);
    text.gx_string_length = string_length_get(string_buffer, sizeof(string_buffer) - 1);
    string_buffer[text.gx_string_length++] = '.';
    gx_utility_ltoa(value % 10, string_buffer + text.gx_string_length, 5);
    text.gx_string_length++;
    string_buffer[text.gx_string_length++] = '%';
    string_buffer[text.gx_string_length] = '\0';

    /* Set string value. */
    gx_prompt_text_set_ext(prompt, &text);
}

/******************************************************************************************/
/* Set a prompt value according to current animation step.                                */
/******************************************************************************************/
VOID prompt_value_set(GX_PROMPT *prompt, INT start_value, INT target_value)
{
INT       value;
GX_CHAR   string_buffer[10];
GX_STRING string;

    /* Calcualte current value. */
    value = start_value + (target_value - start_value) * (TOTAL_ANIMATION_STEPS - remain_animation_steps) / TOTAL_ANIMATION_STEPS;

    /* Convert integer to string. */
    gx_utility_ltoa(value, string_buffer, 10);

    string.gx_string_ptr = string_buffer;
    string.gx_string_length = string_length_get(string_buffer, sizeof(string_buffer) - 1);

    /* Set prompt string. */
    gx_prompt_text_set_ext(prompt, &string);
}

/******************************************************************************************/
/* Start vehicle screen animation.                                                        */
/******************************************************************************************/
VOID vehicle_animation_start()
{
    gx_system_timer_start((GX_WIDGET *)&vehicle_screen, VEHICLE_ANIMATION_TIMER, 1, 1);
    remain_animation_steps = TOTAL_ANIMATION_STEPS;
}

/******************************************************************************************/
/* Update some informations during one step animation.                                    */
/******************************************************************************************/
VOID vehicle_animation_update()
{
INT value;

    remain_animation_steps--;

    /* Update engine oil value. */
    progress_bar_value_set(&vehicle_screen.vehicle_screen_engine_oil, &vehicle_screen.vehicle_screen_engine_oil_value, 0,  TARGET_ENGINE_OIL);

    /* Update transmission value. */
    progress_bar_value_set(&vehicle_screen.vehicle_screen_transmission, &vehicle_screen.vehicle_screen_transmission_value, 0, TARGET_TRANSMISSION);

    /* Update power steering. */
    progress_bar_value_set(&vehicle_screen.vehicle_screen_power_steering, &vehicle_screen.vehicle_screen_power_steering_value, 0, TARGET_POWER_STEERING);

    /* Update coolant. */
    progress_bar_value_set(&vehicle_screen.vehicle_screen_coolant, &vehicle_screen.vehicle_screen_coolant_value, 0, TARGET_COOLANT);

    /* Update brakes. */
    progress_bar_value_set(&vehicle_screen.vehicle_screen_brakes, &vehicle_screen.vehicle_screen_brakes_value, 0, TARGET_BRAKES);

    /* Update windshield washer value. */
    prompt_value_set(&vehicle_screen.vehicle_screen_windshield_washer_value, 0, TARGET_WINDSHIELD_WASHER);

    /* Update battery value. */
    prompt_value_set(&vehicle_screen.vehicle_screen_battery_value, 0, TARGET_BATTERY);

    /* Update air filter value. */
    prompt_value_set(&vehicle_screen.vehicle_screen_air_filter_value, 0, TARGET_AIR_FILTER);

    /* Update brakes value. */
    prompt_value_set(&vehicle_screen.vehicle_screen_brakes_F_value, 0, TARGET_BRAKES_F);
    prompt_value_set(&vehicle_screen.vehicle_screen_brakes_R_value, 0, TARGET_BRAKES_R);

    /* Update tire pressure value. */
    prompt_value_set(&vehicle_screen.vehicle_screen_tire_pressure_value_1, 0, TARGET_TIRE_PRESSURE_1);
    prompt_value_set(&vehicle_screen.vehicle_screen_tire_pressure_value_2, 0, TARGET_TIRE_PRESSURE_2);
    prompt_value_set(&vehicle_screen.vehicle_screen_tire_pressure_value_3, 0, TARGET_TIRE_PRESSURE_3);
    prompt_value_set(&vehicle_screen.vehicle_screen_tire_pressure_value_4, 0, TARGET_TIRE_PRESSURE_4);

    /* Update day value. */
    prompt_value_set(&vehicle_screen.vehicle_screen_day, START_DAY, TARGET_DAY);

    /* Update radial progress bar value. */
    value = START_RADIAL_PROGRESS_BAR_VAL + 
        (TARGET_RADOAL_PROGRESS_BAR_VAL - START_RADIAL_PROGRESS_BAR_VAL)*(TOTAL_ANIMATION_STEPS - remain_animation_steps) / TOTAL_ANIMATION_STEPS;
    gx_radial_progress_bar_value_set(&vehicle_screen.vehicle_screen_radial_progress_bar, value);

    if (remain_animation_steps == 0)
    {
        /* Stop vehicle animation timer. */
        gx_system_timer_stop((GX_WIDGET *)&vehicle_screen, VEHICLE_ANIMATION_TIMER);
    }
}