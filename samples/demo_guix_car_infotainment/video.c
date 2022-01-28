/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

static INT video_play_ticks = 0;
static INT video_play_total_seconds = 12;

/******************************************************************************************/
/* Play video.                                                                            */
/******************************************************************************************/
VOID video_play()
{
    gx_widget_detach(&video_screen.video_screen_play_btn_big_win);
    gx_widget_style_add(&video_screen.video_screen_play_btn_small, GX_STYLE_BUTTON_PUSHED);
    gx_system_timer_start(&video_screen, TIMER_ID_VIDEO_PLAY_PROGRESS_ANIMATION, 1, 1);
}

/******************************************************************************************/
/* Pause video play.                                                                      */
/******************************************************************************************/
VOID video_pause()
{
    gx_widget_attach(&video_screen, &video_screen.video_screen_play_btn_big_win);
    gx_system_timer_stop(&video_screen, 0);
}

/******************************************************************************************/
/* Update video play progress.                                                            */
/******************************************************************************************/
VOID video_play_update()
{
    INT progress_val;
    INT seconds;

    video_play_ticks++;

    seconds = video_play_ticks / GX_TICKS_SECOND;

    if (seconds > video_play_total_seconds)
    {
        video_play_ticks = 0;
    }

    gx_numeric_prompt_value_set(&video_screen.video_screen_minute, seconds / 60);
    gx_numeric_prompt_value_set(&video_screen.video_screen_second, seconds % 60);
    
    progress_val = video_play_ticks * 100 / (video_play_total_seconds * GX_TICKS_SECOND);
    gx_progress_bar_value_set(&video_screen.video_screen_video_play_progress_bar, progress_val);
}

/******************************************************************************************/
/* Override the default event processing of "video_screen" to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT video_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_numeric_prompt_value_set(&video_screen.video_screen_minute, 0);
        gx_numeric_prompt_value_set(&video_screen.video_screen_second, 0);
        gx_progress_bar_value_set(&video_screen.video_screen_video_play_progress_bar, 0);
        screen_base_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_PLAY_BTN_BIG, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_PLAY_BTN_SMALL, GX_EVENT_TOGGLE_ON):
        /* Play. */
        video_play();
        break;

    case GX_SIGNAL(ID_PLAY_BTN_SMALL, GX_EVENT_TOGGLE_OFF):
        /* Pause. */
        video_pause();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == TIMER_ID_VIDEO_PLAY_PROGRESS_ANIMATION)
        {
            video_play_update();
        }
        else
        {
            screen_base_event_process(window, event_ptr);
        }
        break;

    default:
        return screen_base_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Define custom pixelmap button draw function.                                           */
/******************************************************************************************/
VOID video_screen_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button)
{
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_widget_fill_color_set(button, GX_COLOR_ID_LIGHT_GREEN, GX_COLOR_ID_LIGHT_GREEN, GX_COLOR_ID_LIGHT_GREEN);
    }
    else
    {
        gx_widget_fill_color_set(button, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    }

    gx_pixelmap_button_draw(button);
}