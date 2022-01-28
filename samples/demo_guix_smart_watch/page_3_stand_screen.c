#include "demo_guix_smart_watch.h"

#define ANIMATION_TOTAL_STEPS 20
#define CAL_ANIMATION_VAL(val) ((val) * animation_step / ANIMATION_TOTAL_STEPS)

/* Define local variables.  */
static INT standing_minutes_target = 272;
static INT standing_minutes = 140;
static INT animation_step = 0;

/******************************************************************************************/
/* Get standing progress value.                                                           */
/******************************************************************************************/
int get_standing_progress_val()
{
    return (standing_minutes * 100 / standing_minutes_target);
}

/******************************************************************************************/
/* Reset screen data.                                                                     */
/******************************************************************************************/
static void screen_data_reset()
{
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_hour, 0);
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_minute, 0);
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_hour_target, 0);
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_minute_target, 0);
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_progress_val, 0);
    gx_radial_progress_bar_value_set(&stand_screen.stand_screen_progress, 0);
}

/******************************************************************************************/
/* Start a timer for animation.                                                           */
/******************************************************************************************/
static void animation_start(GX_WINDOW *window)
{
    animation_step = 0;

    gx_system_timer_start(window, SCREEN_ANIMATION_TIMER_ID, 40 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);
}

/******************************************************************************************/
/* Update animation.                                                                      */
/******************************************************************************************/
static void animation_update()
{
    animation_step++;

    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_hour, CAL_ANIMATION_VAL(standing_minutes / 60));
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_minute, CAL_ANIMATION_VAL(standing_minutes % 60));
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_hour_target, CAL_ANIMATION_VAL(standing_minutes_target / 60));
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_stand_minute_target, CAL_ANIMATION_VAL(standing_minutes_target % 60));
    gx_numeric_prompt_value_set(&stand_screen.stand_screen_progress_val, CAL_ANIMATION_VAL(standing_minutes * 100 / standing_minutes_target));
    gx_radial_progress_bar_value_set(&stand_screen.stand_screen_progress, CAL_ANIMATION_VAL(standing_minutes * (-360) / standing_minutes_target));

    if (animation_step == ANIMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop(&stand_screen, SCREEN_ANIMATION_TIMER_ID);
    }
}

/******************************************************************************************/
/* Override the default event processing of "stand_screen" to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT stand_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        screen_data_reset();
        break;

    case USER_EVENT_ANIMATION_START:
        animation_start(window);
        break;

    case USER_EVENT_ANIMATION_STOP:
        gx_system_timer_stop(window, SCREEN_ANIMATION_TIMER_ID);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SCREEN_ANIMATION_TIMER_ID)
        {
            animation_update();
        }
        break;

    default:
        break;
    }

    return screen_template_event_process(window, event_ptr);
}