#include "demo_guix_smart_watch.h"

#define ANIMATION_TOTAL_STEPS 20
#define MAX_DIASTOLE_BLOOD_PRESSURE 120
#define CAL_ANIMATION_VAL(val) ((val) * animation_step / ANIMATION_TOTAL_STEPS)

/* Define local variables.  */
static INT systole_blood_pressure = 120;
static INT diastole_blood_pressure = 78;
static INT heart_rate = 68;
static INT animation_step = 0;

/******************************************************************************************/
/* Retrieve ekg progress value.                                                           */
/******************************************************************************************/
int get_ekg_progress_val()
{
    return diastole_blood_pressure * 100 / MAX_DIASTOLE_BLOOD_PRESSURE;
}

/******************************************************************************************/
/* Reset screen data.                                                                     */
/******************************************************************************************/
static void screen_data_reset()
{
    gx_numeric_prompt_value_set(&ekg_screen.ekg_screen_systole_blood_pressure, 0);
    gx_numeric_prompt_value_set(&ekg_screen.ekg_screen_diastole_blood_pressure, 0);
    gx_numeric_prompt_value_set(&ekg_screen.ekg_screen_heart_rate, 0);
    gx_radial_progress_bar_value_set(&ekg_screen.ekg_screen_progress, 0);
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
    animation_step++;

    gx_numeric_prompt_value_set(&ekg_screen.ekg_screen_systole_blood_pressure, CAL_ANIMATION_VAL(systole_blood_pressure));
    gx_numeric_prompt_value_set(&ekg_screen.ekg_screen_diastole_blood_pressure, CAL_ANIMATION_VAL(diastole_blood_pressure));
    gx_numeric_prompt_value_set(&ekg_screen.ekg_screen_heart_rate, CAL_ANIMATION_VAL(heart_rate));
    gx_radial_progress_bar_value_set(&ekg_screen.ekg_screen_progress, CAL_ANIMATION_VAL(diastole_blood_pressure * (-360) / MAX_DIASTOLE_BLOOD_PRESSURE));

    if (animation_step == ANIMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop(&ekg_screen, SCREEN_ANIMATION_TIMER_ID);
    }
}

/******************************************************************************************/
/* Override the default event processing of "ekg_screen" to handle signals from my        */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT ekg_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
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