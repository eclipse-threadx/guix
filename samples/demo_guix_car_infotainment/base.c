/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

/* Define variables. */
GX_RESOURCE_ID    normal_top_icon_sound = GX_PIXELMAP_ID_TOP_ICON_SOUND_SPEAKER_FULL;
GX_RESOURCE_ID    selected_top_icon_sound = GX_PIXELMAP_ID_TOP_ICON_SOUND_SPEAKER_FULL_ON;

/* Define prototypes. */
VOID speaker_show(GX_WIDGET *parent);
VOID speaker_hide(GX_WIDGET *parent);

/******************************************************************************************/
/* Override default event processing of "screen_base" to handle signals from my child     */
/* widgets.                                                                               */
/******************************************************************************************/
UINT base_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
INT value;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Update top sound button pixelmaps. */
        gx_pixelmap_button_pixelmap_set(&((SCREEN_BASE_CONTROL_BLOCK *)window)->screen_base_top_button_sound,
            normal_top_icon_sound, selected_top_icon_sound, GX_NULL);

        map_delete(&decoded_map);
        map_delete(&resized_map);

        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_PEN_DOWN:
        if (speaker_window.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Slide out speaker window. */
            speaker_hide((GX_WIDGET *)window);
        }
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_TOP_SOUND, GX_EVENT_CLICKED):
        /* Slide in speaker window. */
        speaker_show((GX_WIDGET *)window);
        break;

    case GX_SIGNAL(ID_TOP_CAMERA, GX_EVENT_CLICKED):
        if (window != (GX_WINDOW *)&camera_screen)
        {
            /* Toggle to camera screen. */
            screen_toggle((GX_WINDOW *)&camera_screen, window);
        }
        break;

    case GX_SIGNAL(ID_TOP_HOME, GX_EVENT_CLICKED):
        if (window != (GX_WINDOW *)&main_screen)
        {
            /* Toggle to main screen. */
            screen_toggle((GX_WINDOW *)&main_screen, window);
        }
        break;

    case GX_SIGNAL(ID_SPEAKER_SLIDER, GX_EVENT_SLIDER_VALUE):
        value = event_ptr->gx_event_payload.gx_event_intdata[0];

        if (value < 30)
        {
            normal_top_icon_sound = GX_PIXELMAP_ID_ICON_SPEAKER_MUTE;
            selected_top_icon_sound = GX_PIXELMAP_ID_ICON_SPEAKER_MUTE_ON;
        }
        else if (value < 70)
        {
            normal_top_icon_sound = GX_PIXELMAP_ID_ICON_SPEAKER_HALF;
            selected_top_icon_sound = GX_PIXELMAP_ID_ICON_SPEAKER_HALF_ON;
        }
        else
        {
            normal_top_icon_sound = GX_PIXELMAP_ID_TOP_ICON_SOUND_SPEAKER_FULL;
            selected_top_icon_sound = GX_PIXELMAP_ID_TOP_ICON_SOUND_SPEAKER_FULL_ON;
        }
        /* Reset pixelmap of sound button. */
        gx_pixelmap_button_pixelmap_set(&((SCREEN_BASE_CONTROL_BLOCK *)window)->screen_base_top_button_sound,
            normal_top_icon_sound, selected_top_icon_sound, GX_NULL);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override default event processing of "speaker_window" to handle signals from my child  */
/* widgets.                                                                               */
/******************************************************************************************/
UINT speaker_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_SLIDER_INFO *info;

    /* Pickup speaker slider info. */
    info = (GX_SLIDER_INFO *)&speaker_window.speaker_window_speaker_slider.gx_slider_info;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        break;

    case GX_SIGNAL(ID_SPEAKER_MUTE, GX_EVENT_CLICKED):
        /* Set speaker slider value to min. */
        gx_slider_value_set((GX_SLIDER *)&speaker_window.speaker_window_speaker_slider, info, info->gx_slider_info_min_val);
        break;

    case GX_SIGNAL(ID_SPEAKER_FULL, GX_EVENT_CLICKED):
        /* Set speaker slider vlaue to max. */
        gx_slider_value_set((GX_SLIDER *)&speaker_window.speaker_window_speaker_slider, info, info->gx_slider_info_max_val);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Start an animation to slide in the speaker window.                                     */
/******************************************************************************************/
VOID speaker_show(GX_WIDGET *parent)
{
INT speaker_window_height;
GX_ANIMATION *speaker_animation = GX_NULL;
GX_ANIMATION_INFO speaker_animation_info;

    gx_system_animation_get(&speaker_animation);
    if (speaker_animation)
    {
        speaker_window_height = speaker_window.gx_widget_size.gx_rectangle_bottom - speaker_window.gx_widget_size.gx_rectangle_top + 1;
        memset(&speaker_animation_info, 0, sizeof(GX_ANIMATION_INFO));

        /* Set animation information. */
        speaker_animation_info.gx_animation_frame_interval = 1;
        speaker_animation_info.gx_animation_steps = 10;
        speaker_animation_info.gx_animation_target = (GX_WIDGET *)&speaker_window;
        speaker_animation_info.gx_animation_parent = parent;
        speaker_animation_info.gx_animation_start_position.gx_point_x = parent->gx_widget_size.gx_rectangle_left;
        speaker_animation_info.gx_animation_start_position.gx_point_y = parent->gx_widget_size.gx_rectangle_top - speaker_window_height;
        speaker_animation_info.gx_animation_end_position.gx_point_x = parent->gx_widget_size.gx_rectangle_left;
        speaker_animation_info.gx_animation_end_position.gx_point_y = parent->gx_widget_size.gx_rectangle_top;
        speaker_animation_info.gx_animation_start_alpha = 255;
        speaker_animation_info.gx_animation_end_alpha = 255;

        /* Start animation. */
        gx_animation_start(speaker_animation, &speaker_animation_info);
    }
}

/******************************************************************************************/
/* Start an animation to slide out the speaker window.                                    */
/******************************************************************************************/
VOID speaker_hide(GX_WIDGET *parent)
{
INT            speaker_window_height;
GX_ANIMATION  *speaker_animation = GX_NULL;
GX_ANIMATION_INFO speaker_animation_info;

    gx_system_animation_get(&speaker_animation);
    if (speaker_animation)
    {
        speaker_window_height = speaker_window.gx_widget_size.gx_rectangle_bottom - speaker_window.gx_widget_size.gx_rectangle_top + 1;
        memset(&speaker_animation_info, 0, sizeof(GX_ANIMATION_INFO));

        /* Set animation information. */
        speaker_animation_info.gx_animation_frame_interval = 1;
        speaker_animation_info.gx_animation_steps = 10;
        speaker_animation_info.gx_animation_target = (GX_WIDGET *)&speaker_window;
        speaker_animation_info.gx_animation_parent = parent;
        speaker_animation_info.gx_animation_start_position.gx_point_x = parent->gx_widget_size.gx_rectangle_left;
        speaker_animation_info.gx_animation_start_position.gx_point_y = parent->gx_widget_size.gx_rectangle_top;
        speaker_animation_info.gx_animation_end_position.gx_point_x = parent->gx_widget_size.gx_rectangle_left;
        speaker_animation_info.gx_animation_end_position.gx_point_y = parent->gx_widget_size.gx_rectangle_top - speaker_window_height;
        speaker_animation_info.gx_animation_style = GX_ANIMATION_DETACH;
        speaker_animation_info.gx_animation_start_alpha = 255;
        speaker_animation_info.gx_animation_end_alpha = 255;

        /* Start animation. */
        gx_animation_start(speaker_animation, &speaker_animation_info);
    }
}