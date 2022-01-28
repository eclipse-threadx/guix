#include "demo_guix_smart_watch.h"

/* Define music information structure.  */
typedef struct MUSIC_INFO_STRUCT {
    GX_STRING singer;
    GX_STRING song;
    INT duration;
}MUSIC_INFO;

static GX_CHAR singer_1[] = "Angelo";
static GX_CHAR song_1[] = "Remember Me This Way";
static GX_CHAR singer_2[] = "Mat Steiner";
static GX_CHAR song_2[] = "Samba Do Brazil";
static GX_CHAR singer_3[] = "RageGuitar";
static GX_CHAR song_3[] = "Hope For Quick Escape";

/* Define music information.  */
static MUSIC_INFO music_list[] = {
    {{singer_1, sizeof(singer_1) - 1}, {song_1, sizeof(song_1) - 1}, 170},
    {{singer_2, sizeof(singer_2) - 1}, {song_2, sizeof(song_2) - 1}, 180},
    {{singer_3, sizeof(singer_3) - 1}, {song_3, sizeof(song_3) - 1}, 200}
};

/* Define local variables.  */
static GX_BOOL play_on = GX_FALSE;
static INT play_index = 0;
static INT play_time = 0;

/******************************************************************************************/
/* Update music play progress in weather screen.                                          */
/******************************************************************************************/
VOID weather_screen_music_progress_update()
{
    if (!play_on)
    {
        return;
    }

    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_music_play_progress, play_time * (-360) / music_list[play_index].duration);
}

/******************************************************************************************/
/* Update music play progress in music screen.                                            */
/******************************************************************************************/
static VOID music_screen_music_progress_update()
{
    if (!play_on)
    {
        return;
    }

    gx_numeric_prompt_value_set(&music_screen.music_screen_hour, play_time / 60);
    gx_numeric_prompt_value_set(&music_screen.music_screen_minute, play_time % 60);
    gx_radial_progress_bar_value_set(&music_screen.music_screen_play_progress,
        play_time * (-360) / music_list[play_index].duration);
}

/******************************************************************************************/
/* Update music information.                                                              */
/******************************************************************************************/
static VOID music_info_update()
{
    /* Update music information in weather screen.  */
    gx_prompt_text_set_ext(&weather_screen.weather_screen_singer, &music_list[play_index].singer);
    gx_prompt_text_set_ext(&weather_screen.weather_screen_song, &music_list[play_index].song);
    gx_radial_progress_bar_value_set(&weather_screen.weather_screen_music_play_progress, play_time * (-360) / music_list[play_index].duration);

    /* Update music information in music screen.  */
    gx_prompt_text_set_ext(&music_screen.music_screen_singer, &music_list[play_index].singer);
    gx_prompt_text_set_ext(&music_screen.music_screen_song, &music_list[play_index].song);
    gx_numeric_prompt_value_set(&music_screen.music_screen_hour, play_time / 60);
    gx_numeric_prompt_value_set(&music_screen.music_screen_minute, play_time % 60);
    gx_radial_progress_bar_value_set(&music_screen.music_screen_play_progress, play_time * (-360) / music_list[play_index].duration);
}

/******************************************************************************************/
/* Play next song.                                                                        */
/******************************************************************************************/
static VOID music_play_next()
{
    if (play_index + 1 < (INT)(sizeof(music_list) / sizeof(MUSIC_INFO)))
    {
        play_index++;
    }
    else
    {
        play_index = 0;
    }

    play_time =0;

    music_info_update();
}


/******************************************************************************************/
/* Play previous song.                                                                    */
/******************************************************************************************/
static VOID music_play_previous()
{
    if (play_index > 0)
    {
        play_index--;
    }
    else
    {
        play_index = (INT)(sizeof(music_list) / sizeof(MUSIC_INFO)) - 1;
    }

    play_time = 0;

    music_info_update();
}

/******************************************************************************************/
/* Start music playing.                                                                   */
/******************************************************************************************/
VOID music_play_on()
{
    play_on = GX_TRUE;

    gx_widget_style_add(&weather_screen.weather_screen_play_button, GX_STYLE_BUTTON_PUSHED);
    gx_widget_style_add(&music_screen.music_screen_play_button, GX_STYLE_BUTTON_PUSHED);
}

/******************************************************************************************/
/* Stop music playing.                                                                    */
/******************************************************************************************/
VOID music_play_off()
{
    play_on = GX_FALSE;

    gx_widget_style_remove(&weather_screen.weather_screen_play_button, GX_STYLE_BUTTON_PUSHED);
    gx_widget_style_remove(&music_screen.music_screen_play_button, GX_STYLE_BUTTON_PUSHED);
}

/******************************************************************************************/
/* Update music play progress.                                                            */
/******************************************************************************************/
VOID music_play_progress_update()
{
    if (!play_on)
    {
        return;
    }

    if (play_time < music_list[play_index].duration)
    {
        play_time++;
    }
    else
    {
        music_play_next();
    }
}

/******************************************************************************************/
/* Override the default event processing of "music_screen" to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT music_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        music_info_update();
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SCREEN_CLOCK_TIMER_ID)
        {
            music_screen_music_progress_update();
        }
        break;

    case GX_SIGNAL(ID_PLAY_BUTTON, GX_EVENT_TOGGLE_ON):
        music_play_on();
        break;

    case GX_SIGNAL(ID_PLAY_BUTTON, GX_EVENT_TOGGLE_OFF):
        music_play_off();
        break;

    case GX_SIGNAL(ID_FORWARD_BUTTON, GX_EVENT_CLICKED):
        music_play_next();
        break;

    case GX_SIGNAL(ID_REVERSE_BUTTON, GX_EVENT_CLICKED):
        music_play_previous();
        break;

    default:
        break;
    }

    return screen_template_event_process(window, event_ptr);
}

/******************************************************************************************/
/* Define custom pixelmap button draw function to draw the button as selected when the    */
/* the button is pushed.                                                                  */
/******************************************************************************************/
VOID custom_pixelmap_button_draw(GX_PIXELMAP_BUTTON* button)
{
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        button->gx_widget_style |= GX_STYLE_DRAW_SELECTED;
    }
    else
    {
        button->gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
    }

    gx_pixelmap_button_draw(button);
}