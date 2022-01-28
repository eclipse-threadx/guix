/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

/* Define macros. */
#define MIN_SEAT_ANGLE 50
#define MAX_SEAT_ANGLE 90
#define PROGRESS_ANIMATION_TOTAL_STEPS 20

/* Define variables. */
GX_RESOURCE_ID fan_mode_text_id_list[] = {
    GX_STRING_ID_AUTO,
    GX_STRING_ID_MED,
    GX_STRING_ID_HI,
    GX_STRING_ID_LOW
};

INT left_fan_mode_index = 0;
INT right_fan_mode_index = 0;
INT target_left_progress_val;
INT target_right_progress_val;
INT progress_animation_step;

/******************************************************************************************/
/* Convert progress bar value to seat angle.                                              */
/******************************************************************************************/
INT progress_val_to_seat_angle(GX_PROGRESS_BAR* bar)
{
    INT seat_angle;
    GX_PROGRESS_BAR_INFO* info = &bar->gx_progress_bar_info;

    seat_angle = info->gx_progress_bar_info_current_val - info->gx_progress_bar_info_min_val;
    seat_angle *= (MAX_SEAT_ANGLE - MIN_SEAT_ANGLE);
    seat_angle /= info->gx_progress_bar_info_max_val - info->gx_progress_bar_info_min_val;
    seat_angle += MIN_SEAT_ANGLE;

    return seat_angle;
}

/******************************************************************************************/
/* Increase fan mode.                                                                     */
/******************************************************************************************/
INT fan_mode_increase(int fan_mode)
{
    fan_mode++;
    if (fan_mode >= sizeof(fan_mode_text_id_list) / sizeof(GX_RESOURCE_ID))
    {
        fan_mode = 0;
    }

    return fan_mode;
}

/******************************************************************************************/
/* Decrease fan mode.                                                                     */
/******************************************************************************************/
INT fan_mode_decrease(int fan_mode)
{
    fan_mode--;
    if (fan_mode < 0)
    {
        fan_mode = sizeof(fan_mode_text_id_list) / sizeof(GX_RESOURCE_ID) - 1;
    }

    return fan_mode;
}

/******************************************************************************************/
/* Start seat angle animation.                                                            */
/******************************************************************************************/
VOID seat_angle_animation_start()
{
    gx_system_timer_start(&climate_screen, TIMER_ID_CLIMATE_SCREEN_PROGRESS_ANIMATION, 20 / GX_SYSTEM_TIMER_MS, 20 / GX_SYSTEM_TIMER_MS);
    target_left_progress_val = climate_screen.climate_screen_left_progress_bar.gx_progress_bar_info.gx_progress_bar_info_current_val;
    target_right_progress_val = climate_screen.climate_screen_right_progress_bar.gx_progress_bar_info.gx_progress_bar_info_current_val;
    progress_animation_step = 0;
    gx_progress_bar_value_set(&climate_screen.climate_screen_left_progress_bar, 0);
    gx_progress_bar_value_set(&climate_screen.climate_screen_right_progress_bar, 0);
}

/******************************************************************************************/
/* Update seat angle animation.                                                           */
/******************************************************************************************/
VOID seat_angle_animation_update()
{
    INT left_progress_val;
    INT right_progress_val;

    progress_animation_step++;

    left_progress_val = target_left_progress_val * progress_animation_step / PROGRESS_ANIMATION_TOTAL_STEPS;
    right_progress_val = target_right_progress_val * progress_animation_step / PROGRESS_ANIMATION_TOTAL_STEPS;
    gx_progress_bar_value_set(&climate_screen.climate_screen_left_progress_bar, left_progress_val);
    gx_progress_bar_value_set(&climate_screen.climate_screen_right_progress_bar, right_progress_val);

    if (progress_animation_step == PROGRESS_ANIMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop(&climate_screen, 0);
    }
}

/******************************************************************************************/
/* Override the default event processing of "climate_screen" to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT climate_screen_event_process(GX_WINDOW *window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        seat_angle_animation_start();
        screen_base_event_process(window, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TIMER_ID_CLIMATE_SCREEN_PROGRESS_ANIMATION)
        {
            seat_angle_animation_update();
        }
        else
        {
            screen_base_event_process(window, event_ptr);
        }
        break;

    case GX_SIGNAL(ID_LEFT_PROGRESS_BAR, GX_EVENT_PROGRESS_VALUE):
        gx_numeric_prompt_value_set(&climate_screen.climate_screen_left_progress_val, progress_val_to_seat_angle(&climate_screen.climate_screen_left_progress_bar));
        break;

    case GX_SIGNAL(ID_RIGHT_PROGRESS_BAR, GX_EVENT_PROGRESS_VALUE):
        gx_numeric_prompt_value_set(&climate_screen.climate_screen_right_progress_val, progress_val_to_seat_angle(&climate_screen.climate_screen_right_progress_bar));
        break;

    case GX_SIGNAL(ID_LEFT_FAN_BUTTON, GX_EVENT_CLICKED):
        left_fan_mode_index = fan_mode_increase(left_fan_mode_index);
        gx_prompt_text_id_set(&climate_screen.climate_screen_left_fan_mode, fan_mode_text_id_list[left_fan_mode_index]);
        break;

    case GX_SIGNAL(ID_LEFT_SMALL_FAN_BUTTON, GX_EVENT_CLICKED):
        left_fan_mode_index = fan_mode_decrease(left_fan_mode_index);
        gx_prompt_text_id_set(&climate_screen.climate_screen_left_fan_mode, fan_mode_text_id_list[left_fan_mode_index]);
        break;

    case GX_SIGNAL(ID_RIGHT_FAN_BUTTON, GX_EVENT_CLICKED):
        right_fan_mode_index = fan_mode_increase(right_fan_mode_index);
        gx_prompt_text_id_set(&climate_screen.climate_screen_right_fan_mode, fan_mode_text_id_list[right_fan_mode_index]);
        break;

    case GX_SIGNAL(ID_RIGHT_SMALL_FAN_BUTTON, GX_EVENT_CLICKED):
        right_fan_mode_index = fan_mode_decrease(right_fan_mode_index);
        gx_prompt_text_id_set(&climate_screen.climate_screen_right_fan_mode, fan_mode_text_id_list[right_fan_mode_index]);
        break;
    default:
        return screen_base_event_process(window, event_ptr);
    }

    return GX_SUCCESS;
}

/******************************************************************************************/
/* Define custom progress event process function to handle pen down event.               */
/******************************************************************************************/
UINT custom_progress_bar_event_process(GX_PROGRESS_BAR *bar, GX_EVENT *event_ptr)
{
    INT val;
    GX_RECTANGLE *size = &bar->gx_widget_size;
    GX_PROGRESS_BAR_INFO *info = &bar->gx_progress_bar_info;

    if (event_ptr->gx_event_type == GX_EVENT_PEN_DOWN)
    {
        /* Calculate progress bar value according to pen down position. */
        val = (size->gx_rectangle_bottom - event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y + 2);
        val *= (info->gx_progress_bar_info_max_val - info->gx_progress_bar_info_min_val);
        val /= (size->gx_rectangle_bottom - size->gx_rectangle_top + 1);
        val += info->gx_progress_bar_info_min_val;

        gx_progress_bar_value_set(bar, val);
        return GX_SUCCESS;
    }
    else
    {
        return gx_progress_bar_event_process(bar, event_ptr);
    }
}