#include "demo_guix_smart_watch.h"

#define ANIMATION_TOTAL_STEPS 20
#define CAL_ANIMATION_VAL(val) ((val) * animation_step / ANIMATION_TOTAL_STEPS)

static INT animation_step = 0;

/******************************************************************************************/
/* Reset screen data.                                                                     */
/******************************************************************************************/
static void screen_data_reset()
{
    gx_numeric_prompt_value_set(&weather_screen.weather_screen_calorie_burned_progress_val, 0);
    gx_numeric_prompt_value_set(&weather_screen.weather_screen_running_progress_val, 0);
    gx_numeric_prompt_value_set(&weather_screen.weather_screen_ekg_progress_val, 0);
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_calorie_burned_progress, 0);
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_running_progress, 0);
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_ekg_progress, 0);
}

/******************************************************************************************/
/* Start a timer for animation.                                                           */
/******************************************************************************************/
static void animation_start(GX_WINDOW* window)
{
    animation_step = 0;

    gx_system_timer_start(window, SCREEN_ANIMATION_TIMER_ID, 40 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);
}

/******************************************************************************************/
/* Update animation.                                                                      */
/******************************************************************************************/
static void animation_update()
{
    INT val;

    animation_step++;


    val = get_calorie_burned_progress_val();
    gx_numeric_prompt_value_set(&weather_screen.weather_screen_calorie_burned_progress_val, CAL_ANIMATION_VAL(val));

    val = val * (-360) / 100;
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_calorie_burned_progress, CAL_ANIMATION_VAL(val));

    val = get_running_progress_val();
    gx_numeric_prompt_value_set(&weather_screen.weather_screen_running_progress_val, CAL_ANIMATION_VAL(val));

    val = val * (-360) / 100;
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_running_progress, CAL_ANIMATION_VAL(val));

    val = get_ekg_progress_val();
    gx_numeric_prompt_value_set(&weather_screen.weather_screen_ekg_progress_val, CAL_ANIMATION_VAL(val));

    val = val * (-360) / 100;
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_ekg_progress, CAL_ANIMATION_VAL(val));

    if (animation_step == ANIMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop(&weather_screen, SCREEN_ANIMATION_TIMER_ID);
    }
}

/******************************************************************************************/
/* Override the default event processing of "weather_screen" to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT weather_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        screen_data_reset();
        clear_screen_clock_record();
        screen_clock_update(&weather_screen.weather_screen_hour, &weather_screen.weather_screen_minute, &weather_screen.weather_screen_second);
        weather_screen_music_progress_update();
        gx_system_timer_start(window, SCREEN_CLOCK_TIMER_ID, GX_TICKS_SECOND, GX_TICKS_SECOND);
        return gx_window_event_process(window, event_ptr);

    case USER_EVENT_ANIMATION_START:
        animation_start(window);
        break;

    case USER_EVENT_ANIMATION_STOP:
        gx_system_timer_stop(window, SCREEN_ANIMATION_TIMER_ID);
        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_TIMER:
        switch (event_ptr->gx_event_payload.gx_event_timer_id)
        {
        case SCREEN_ANIMATION_TIMER_ID:
            animation_update();
            break;

        case SCREEN_CLOCK_TIMER_ID:
            screen_clock_update(&weather_screen.weather_screen_hour, &weather_screen.weather_screen_minute, &weather_screen.weather_screen_second);
            weather_screen_music_progress_update();
            break;
        }
        break;

    case GX_SIGNAL(ID_PLAY_BUTTON, GX_EVENT_TOGGLE_ON):
        music_play_on();
        break;

    case GX_SIGNAL(ID_PLAY_BUTTON, GX_EVENT_TOGGLE_OFF):
        music_play_off();
        break;

    case GX_SIGNAL(ID_CALORIE_BUTTON, GX_EVENT_CLICKED):
        container_screen_slide((GX_WIDGET*)window, (GX_WIDGET*)&calories_screen);
        break;

    case GX_SIGNAL(ID_RUNNING_BUTTON, GX_EVENT_CLICKED):
        container_screen_slide((GX_WIDGET*)window, (GX_WIDGET*)&run_screen);
        break;

    case GX_SIGNAL(ID_EKG_BUTTON, GX_EVENT_CLICKED):
        container_screen_slide((GX_WIDGET*)window, (GX_WIDGET*)&ekg_screen);
        break;

    case GX_SIGNAL(ID_BELL_BUTTON, GX_EVENT_CLICKED):
        page_screen_slide((GX_WIDGET *)window, (GX_WIDGET *)&message_screen);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}