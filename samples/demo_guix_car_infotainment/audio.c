/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

/* Define macros.  */
#define RADIAL_PROGRESS_ANIMATION_TOTAL_STEPS 10
#define MAX_PROGRESS_VAL -360

/* Define local variables.  */
static int radial_progress_animation_step = 0;
static int play_seconds = 30;

/******************************************************************************************/
/* Play music.                                                                            */
/******************************************************************************************/
VOID music_play_start()
{
    radial_progress_animation_step = 0;
    gx_system_timer_start(&audio_screen, TIMER_ID_RADIAL_PROGRESS_ANIMATION, GX_TICKS_SECOND, GX_TICKS_SECOND);
}

/******************************************************************************************/
/* Stop music play.                                                                       */
/******************************************************************************************/
VOID music_play_stop()
{
    gx_system_timer_stop(&audio_screen, 0);
}

/******************************************************************************************/
/* Update music play progress.                                                            */
/******************************************************************************************/
VOID music_play_update()
{
    GX_RADIAL_PROGRESS_BAR *bar = &audio_screen.audio_screen_radial_progress_bar;
    GX_VALUE val = bar->gx_radial_progress_bar_info.gx_radial_progress_bar_info_current_val;
    if (val - 10 < MAX_PROGRESS_VAL)
    {
        val = 0;
        play_seconds = 0;
    }
    else
    {
        val -= 3;

        play_seconds++;
    }

    gx_numeric_prompt_value_set(&audio_screen.audio_screen_minute, play_seconds / 60);
    gx_numeric_prompt_value_set(&audio_screen.audio_screen_second, play_seconds % 60);
    gx_radial_progress_bar_value_set(bar, val);
}

/******************************************************************************************/
/* Start animation to slide in music information window.                                  */
/******************************************************************************************/
VOID slide_in_music_info_win()
{
    GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET*)&audio_screen;
    my_event.gx_event_type = USER_EVENT_SLIDE_IN_MUSIC_INFO_WIN;

    gx_system_event_send(&my_event);
}

/******************************************************************************************/
/* Play next music.                                                                       */
/******************************************************************************************/
VOID music_next()
{
    slide_in_music_info_win();

    /* Reset music play progress.  */
    play_seconds = 0;
    gx_numeric_prompt_value_set(&audio_screen.audio_screen_minute, 0);
    gx_numeric_prompt_value_set(&audio_screen.audio_screen_second, 0);
    gx_radial_progress_bar_value_set(&audio_screen.audio_screen_radial_progress_bar, 0);
}

/******************************************************************************************/
/* Override the default event processing of "audio_screen" to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT audio_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Slide music informatin window. */
        slide_in_music_info_win();

        volume_screen_base_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_PLAY_BTN, GX_EVENT_TOGGLE_ON):
        /* Play. */
        music_play_start();
        break;

    case GX_SIGNAL(ID_PLAY_BTN, GX_EVENT_TOGGLE_OFF):
        /* Pause. */
        music_play_stop();
        break;

    case GX_SIGNAL(ID_PREVIOUS, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NEXT, GX_EVENT_CLICKED):
        /* Next. */
        music_next();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TIMER_ID_RADIAL_PROGRESS_ANIMATION)
        {
            /* Update music play progress.  */
            music_play_update();
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