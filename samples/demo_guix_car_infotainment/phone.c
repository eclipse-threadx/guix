/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

/* Define local variables.  */
static int call_duration_minute = 0;
static int call_duration_second = 0;

/******************************************************************************************/
/* Start recording call duration.                                                         */
/******************************************************************************************/
static void call_duration_start()
{
    call_duration_second = 0;
    call_duration_minute = 0;
    gx_numeric_prompt_value_set(&phone_screen.phone_screen_second, call_duration_second);
    gx_numeric_prompt_value_set(&phone_screen.phone_screen_minute, call_duration_minute);
    gx_system_timer_start(&phone_screen, TIMER_ID_CALL_DURATION, GX_TICKS_SECOND, GX_TICKS_SECOND);
}

/******************************************************************************************/
/* Update call duration.                                                                  */
/******************************************************************************************/
static void call_duration_update()
{
    GX_RESOURCE_ID color_id;

    call_duration_second++;

    if (call_duration_second & 0x1)
    {
        color_id = GX_COLOR_ID_WHITE;
    }
    else
    {
        color_id = GX_COLOR_ID_GRAY;
    }

    gx_prompt_text_color_set(&phone_screen.phone_screen_second_colon, color_id, color_id, color_id);

    gx_numeric_prompt_value_set(&phone_screen.phone_screen_second, call_duration_second);

    if (call_duration_second == 60)
    {
        call_duration_second = 0;
        call_duration_minute++;
        gx_numeric_prompt_value_set(&phone_screen.phone_screen_minute, call_duration_minute);
    }
}

/******************************************************************************************/
/* Stop recording call duration.                                                          */
/******************************************************************************************/
static void call_duration_end()
{
    gx_system_timer_stop(&phone_screen, TIMER_ID_CALL_DURATION);
}

/******************************************************************************************/
/* Override the default event processing of "phone_screen" to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT phone_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_widget_style_remove(&phone_screen.phone_screen_end_call_btn, GX_STYLE_BUTTON_PUSHED);
        call_duration_start();
        volume_screen_base_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_BTN_END_CALL, GX_EVENT_TOGGLE_ON):
        call_duration_end();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TIMER_ID_CALL_DURATION)
        {
            call_duration_update();
        }
        else
        {
            volume_screen_base_event_process(window, event_ptr);
        }
        break;

    default:
        return volume_screen_base_event_process(window, event_ptr);
    }

    return 0;
}

