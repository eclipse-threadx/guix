#include "demo_guix_smart_watch.h"

#define ANIMATION_TOTAL_STEPS 20
#define CAL_ANIMATION_VAL(val) ((val) * animation_step / ANIMATION_TOTAL_STEPS)

/* Define local variables.  */
static INT running_distance_target = 637;
static INT running_distance = 465;
static INT running_steps = 11278;
static INT running_time = 3148; /* Seconds. */
static INT animation_step = 0;

/******************************************************************************************/
/* Get running progress value.                                                            */
/******************************************************************************************/
int get_running_progress_val()
{
    return (running_distance * 100 / running_distance_target);
}

/******************************************************************************************/
/* Get running steps.                                                                     */
/******************************************************************************************/
int get_running_steps()
{
    return running_steps;
}

/******************************************************************************************/
/* Get running distance.                                                                  */
/******************************************************************************************/
int get_running_distance()
{
    return running_distance;
}

/******************************************************************************************/
/* Reset screen data.                                                                     */
/******************************************************************************************/
static void screen_data_reset()
{
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_distance, 0);
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_minutes, 0);
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_seconds, 0);
    gx_numeric_prompt_value_set(&run_screen.run_screen_progress_val, 0);
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_distance_fraction_part, 0);
    gx_radial_progress_bar_value_set(&run_screen.run_screen_progress, 0);
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

    gx_numeric_prompt_value_set(&run_screen.run_screen_running_distance, CAL_ANIMATION_VAL(running_distance / 100));
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_distance_fraction_part, CAL_ANIMATION_VAL(running_distance % 100));
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_minutes, CAL_ANIMATION_VAL(running_time / 60));
    gx_numeric_prompt_value_set(&run_screen.run_screen_running_seconds, CAL_ANIMATION_VAL(running_time % 60));
    gx_numeric_prompt_value_set(&run_screen.run_screen_progress_val, CAL_ANIMATION_VAL(running_distance * 100 / running_distance_target));
    gx_radial_progress_bar_value_set(&run_screen.run_screen_progress, CAL_ANIMATION_VAL(running_distance * (-360) / running_distance_target));

    if (animation_step == ANIMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop(&run_screen, SCREEN_ANIMATION_TIMER_ID);
    }
}

/******************************************************************************************/
/* Override the default event processing of "calories_screen" to handle signals from my   */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT run_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
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