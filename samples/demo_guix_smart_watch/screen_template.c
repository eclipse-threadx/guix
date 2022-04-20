#include "demo_guix_smart_watch.h"

/* Define screen properties.  */
static SCREEN_INFO screen_info_list[] = {
    {(GX_WIDGET*)&message_screen, GX_PIXELMAP_ID_ICON_BELL, 0, GX_TRUE},
    {(GX_WIDGET*)&music_screen, GX_PIXELMAP_ID_ICON_NOTE, 0, GX_TRUE},
    {(GX_WIDGET*)&SanDiego_weather_screen, GX_PIXELMAP_ID_ICON_LOCATION, GX_STRING_ID_SAN_DIEGO, GX_FALSE},
    {(GX_WIDGET*)&LosAngeles_weather_screen, GX_PIXELMAP_ID_ICON_LOCATION, GX_STRING_ID_LOS_ANGELES, GX_FALSE},
    {(GX_WIDGET*)&SanFrancisco_weather_screen, GX_PIXELMAP_ID_ICON_LOCATION, GX_STRING_ID_SAN_FRANCISCO, GX_FALSE},
    {(GX_WIDGET*)&yoga_screen, 0, GX_STRING_ID_YOGA, GX_TRUE},
    {(GX_WIDGET*)&ekg_screen, 0, GX_STRING_ID_EKG, GX_TRUE},
    {(GX_WIDGET*)&fitness_screen, 0, GX_STRING_ID_FITNESS, GX_FALSE},
    {(GX_WIDGET*)&calories_screen, 0, GX_STRING_ID_CALORIES, GX_TRUE},
    {(GX_WIDGET*)&run_screen, 0, GX_STRING_ID_RUN, GX_TRUE},
    {(GX_WIDGET*)&stand_screen, 0, GX_STRING_ID_STAND, GX_TRUE},
    {GX_NULL, 0, 0, 0}
};

/******************************************************************************************/
/* Update title bar clock.                                                                */
/******************************************************************************************/
static VOID update_title_clock(SCREEN_TEMPLATE_CONTROL_BLOCK *template)
{
    screen_clock_update(&template->screen_template_hour, &template->screen_template_minute, &template->screen_template_second);
}

/******************************************************************************************/
/* Initialize screen properties.                                                          */
/******************************************************************************************/
VOID screens_initialize()
{
    SCREEN_INFO* info = screen_info_list;
    SCREEN_TEMPLATE_CONTROL_BLOCK* screen;

    while (info->screen)
    {
        screen = (SCREEN_TEMPLATE_CONTROL_BLOCK*)info->screen;

        /* Set title text.  */
        gx_prompt_text_id_set(&screen->screen_template_title, info->title_text_id);

        /* Set title icon.  */
        if (info->title_icon_id)
        {
            gx_icon_pixelmap_set(&screen->screen_template_icon, info->title_icon_id, info->title_icon_id);
        }
        else
        {
            gx_widget_detach(&screen->screen_template_icon);
        }

        /* Set transparency style.  */
        if (!info->title_transparency)
        {
            gx_widget_style_remove(&screen->screen_template_title_bar, GX_STYLE_TRANSPARENT);
        }

        info++;
    }

    fitness_screen_initialize();
    watch_screen_initialize();
}

/******************************************************************************************/
/* Override the default event processing of "screen_template" to handle signals from my   */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT screen_template_event_process(GX_WINDOW *window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:

        clear_screen_clock_record();

        /* Update title bar clock.  */
        update_title_clock((SCREEN_TEMPLATE_CONTROL_BLOCK *)window);

        /* Start a timer to update title bar clock.  */
        gx_system_timer_start(window, SCREEN_CLOCK_TIMER_ID, GX_TICKS_SECOND, GX_TICKS_SECOND);

        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_HIDE:

        /* Stop clock update timer.  */
        gx_system_timer_stop(window, SCREEN_CLOCK_TIMER_ID);

        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SCREEN_CLOCK_TIMER_ID)
        {
            /* Update title bar clock.  */
            update_title_clock((SCREEN_TEMPLATE_CONTROL_BLOCK *)window);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}