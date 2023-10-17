/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_industrial.h"

#define COUNTDOWN_TIMER 0xf1
#define COUNTDOWN_VALUE 9

static INT countdown_value;

/******************************************************************************************/
/* Override event processing of "complete_window" to handle signals from my child widgets.*/
/******************************************************************************************/
UINT complete_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        gx_system_timer_start((GX_WIDGET *)window, COUNTDOWN_TIMER, 40, 40);
        countdown_value = COUNTDOWN_VALUE;
        gx_numeric_prompt_value_set(&complete_window.complete_window_countdown, countdown_value);
        break;

    case GX_EVENT_HIDE:
        gx_window_event_process(window, event_ptr);
        gx_system_timer_stop((GX_WIDGET *)window, COUNTDOWN_TIMER);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == COUNTDOWN_TIMER)
        {
            countdown_value--;
            gx_numeric_prompt_value_set(&complete_window.complete_window_countdown, countdown_value);

            if (countdown_value == 0)
            {
                gx_system_timer_stop((GX_WIDGET *)window, COUNTDOWN_TIMER);
                screen_toggle_animation_start((GX_WINDOW *)&sequence_window, (GX_WINDOW *)&complete_window);
            }
        }
        break;
    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
