
#include "demo_guix_washing_machine.h"

/* Define variables. */

#define WASHER_ON_POWER_OFF_TIMER 0x2
#define TIME_ANIMATION_TOTAL_STEPS 10

/* Define angle value of each wash mode. */
GX_VALUE washer_mode_angles[] = { -63, -34, -10, 13, 38, 67, 112, 142, 168, 191, 214, 242 };

/* Define time needed for each wash mode. */
GX_VALUE washer_mode_remain_minutes[] = { 14, 80, 10, 150, 34, 65, 40, 50, 90, 75, 65, 55 };

/* Define label widget that each wash mode correspond to. */
GX_WIDGET *washer_mode_label_widgets[] = {
    (GX_WIDGET *)&main_screen.main_screen_mode_rinse_spin,
    (GX_WIDGET *)&main_screen.main_screen_mode_no_spin,
    (GX_WIDGET *)&main_screen.main_screen_mode_spin,
    (GX_WIDGET *)&main_screen.main_screen_mode_soak,
    (GX_WIDGET *)&main_screen.main_screen_mode_quick_wash,
    (GX_WIDGET *)&main_screen.main_screen_mode_perm_press,
    (GX_WIDGET *)&main_screen.main_screen_mode_very_fast,
    (GX_WIDGET *)&main_screen.main_screen_mode_fast,
    (GX_WIDGET *)&main_screen.main_screen_mode_normal,
    (GX_WIDGET *)&main_screen.main_screen_mode_medium,
    (GX_WIDGET *)&main_screen.main_screen_mode_light,
    (GX_WIDGET *)&main_screen.main_screen_mode_very_light
};

/* "selected_index" is used to record current selected wash mode index. */
static INT selected_index = 4;

INT ani_target_val;
INT ani_start_val;
INT ani_target_remain_minutes;
INT ani_start_remain_minutes;

/* "ani_current_remain_minutes" is used to record the needed time of each wash mode */
INT ani_current_remain_minutes;
INT time_animation_step = TIME_ANIMATION_TOTAL_STEPS;

/* Declare prototypes. */
VOID washer_on_page_init();
VOID washer_on_page_power_off();
VOID washer_mode_label_widgets_update(int old_selected_index);
VOID washer_mode_remain_minutes_update(int current_value);

/******************************************************************************************/
/* Override default event processing of "washer on" window to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT washer_on_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    GX_WIDGET *widget;
    int index;
    int old_selected_index;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_radial_slider_anchor_angles_set(&main_screen.main_screen_radial_slider, washer_mode_angles, sizeof(washer_mode_angles) / sizeof(GX_VALUE));
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_WASHER_MODE_RINSE_SPIN, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_NO_SPIN, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_SPIN, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_SOAK, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_QUICK_WASH, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_PERM_PRESS, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_VERY_FAST, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_FAST, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_NORMAL, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_MEDIUM, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_LIGHT, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WASHER_MODE_VERY_LIGHT, GX_EVENT_CLICKED):
        gx_widget_find((GX_WIDGET *)window, (USHORT)(event_ptr->gx_event_type >> 8), GX_SEARCH_DEPTH_INFINITE, &widget);
        if (widget)
        {
            for (index = 0; index < (int)(sizeof(washer_mode_angles) / sizeof(GX_VALUE)); index++)
            {
                if (washer_mode_label_widgets[index] == widget)
                {
                    /* Find selected widget index, and start animation to move the needle to the
                    selected angle. */

                    gx_radial_slider_animation_start(&main_screen.main_screen_radial_slider, washer_mode_angles[index]);
                }
            }
        }
        break;

    case GX_SIGNAL(ID_WASHER_ON_RADIAL_SLIDER, GX_EVENT_ANIMATION_START):
        for (index = 0; index < (int)(sizeof(washer_mode_angles) / sizeof(GX_VALUE)); index++)
        {
            if (washer_mode_angles[index] == event_ptr->gx_event_payload.gx_event_intdata[0])
            {
                /* Get the remain minutes of target mode. */
                ani_start_val = washer_mode_angles[selected_index];
                ani_target_val = washer_mode_angles[index];
                ani_start_remain_minutes = washer_mode_remain_minutes[selected_index];
                ani_target_remain_minutes = washer_mode_remain_minutes[index];
                break;
            }
        }
        break;

    case GX_SIGNAL(ID_WASHER_ON_RADIAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        for (index = 0; index < (int)(sizeof(washer_mode_angles) / sizeof(GX_VALUE)); index++)
        {
            if (washer_mode_angles[index] == event_ptr->gx_event_payload.gx_event_intdata[0])
            {
                old_selected_index = selected_index;
                selected_index = index;

                /* Update selection mode lable. */
                washer_mode_label_widgets_update(old_selected_index);
            }
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == WASHER_ON_POWER_OFF_TIMER)
        {
            time_animation_step--;
            if (time_animation_step)
            {
                ani_current_remain_minutes = (ani_start_remain_minutes - ani_target_remain_minutes) * time_animation_step / TIME_ANIMATION_TOTAL_STEPS;
            }
            else
            {
                gx_system_timer_stop(&main_screen.main_screen_washer_on_window, WASHER_ON_POWER_OFF_TIMER);
                ani_current_remain_minutes = 0;
            }

            washer_mode_remain_minutes_update(ani_current_remain_minutes);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Update some values when "waher on" button is selected.                                 */
/******************************************************************************************/
VOID washer_on_page_init()
{
    INT animation_start_index = 11;

    /* Change text id to "STRING_ID_PAUSE" in washer on button. */
    gx_prompt_text_id_set(&main_screen.main_screen_washer_on_label, GX_STRING_ID_PAUSE);

    /* Change icon to "ICON_PAUSE" in washer on button. */
    gx_icon_pixelmap_set(&main_screen.main_screen_washer_on_icon, GX_PIXELMAP_ID_BUTTON_ICON_PAUSE, GX_NULL);

    /* Set page name to "Washer On". */
    gx_prompt_text_id_set(&main_screen.main_screen_page_name, GX_STRING_ID_WASHER_ON);

    /* Set page name color to green. */
    gx_prompt_text_color_set(&main_screen.main_screen_page_name, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);

    /* Init remain minutes, the value is larged by 256 for precision needs.
    remain minutes is the time needed for each mode. */
    ani_current_remain_minutes = washer_mode_remain_minutes[animation_start_index];

    /* Set radial slider position to mode 11. */
    gx_radial_slider_angle_set(&main_screen.main_screen_radial_slider, washer_mode_angles[animation_start_index]);

    /* Update selection mode labels. */
    washer_mode_label_widgets_update(selected_index);

    /* Start animation to move from mode 11 to select mode. */
    gx_radial_slider_animation_start(&main_screen.main_screen_radial_slider, washer_mode_angles[selected_index]);

    widget_enable_disable((GX_WIDGET *)&main_screen.main_screen_washer_on_window, POWER_ON);
}

/******************************************************************************************/
/* Update some values when "washer on" button is de-selected.                             */
/******************************************************************************************/
VOID washer_on_page_power_off()
{
    ani_start_remain_minutes = ani_current_remain_minutes;
    ani_target_remain_minutes = 0;
    ani_start_val = ani_current_remain_minutes;
    ani_target_val = 0;
    time_animation_step = TIME_ANIMATION_TOTAL_STEPS;
    gx_system_timer_start(&main_screen.main_screen_washer_on_window, WASHER_ON_POWER_OFF_TIMER, 1, 1);

    widget_enable_disable((GX_WIDGET *)&main_screen.main_screen_washer_on_window, POWER_OFF);
}

/******************************************************************************************/
/* Update text color of selected and de-selected mode labels.                             */
/******************************************************************************************/
VOID washer_mode_label_widgets_update(int old_selected_index)
{
    /* Set old selected mode label color to light gray. */
    gx_prompt_text_color_set((GX_PROMPT *)washer_mode_label_widgets[old_selected_index], GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);

    /* Set new selected mode label color to green. */
    gx_prompt_text_color_set((GX_PROMPT *)washer_mode_label_widgets[selected_index], GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);

    /* Update time value in the center of the radial slider. */
    gx_numeric_prompt_value_set(&main_screen.main_screen_remain_hour, washer_mode_remain_minutes[selected_index] / 60);
    gx_numeric_prompt_value_set(&main_screen.main_screen_remain_minute, washer_mode_remain_minutes[selected_index] % 60);
}

/******************************************************************************************/
/* Washer on mode radial slider animatio update callback, this function is called after   */
/* each animation step.                                                                   */
/******************************************************************************************/
VOID washer_mode_animation_update(GX_RADIAL_SLIDER *slider)
{
    INT current_angle = slider->gx_radial_slider_info.gx_radial_slider_info_current_angle;

    /* Move remain minutes toward target remain minutes step by step. */
    washer_mode_remain_minutes_update(current_angle);
}

/******************************************************************************************/
/* Update remaining time in the middle of the radial slider widget.                       */
/******************************************************************************************/
VOID washer_mode_remain_minutes_update(int current_value)
{
    GX_VALUE hour;
    GX_VALUE minute;

    /* Move remain minutes toward target remain minutes step by step. */
    if (ani_target_val == ani_start_val)
    {
        return;
    }

    ani_current_remain_minutes = (ani_target_remain_minutes - ani_start_remain_minutes) *
        (current_value - ani_start_val) / (ani_target_val - ani_start_val);
    ani_current_remain_minutes += ani_start_remain_minutes;

    hour = ani_current_remain_minutes / 60;
    minute = ani_current_remain_minutes % 60;

    /* Update time value in the center of the radial slider. */
    gx_numeric_prompt_value_set(&main_screen.main_screen_remain_hour, hour);
    gx_numeric_prompt_value_set(&main_screen.main_screen_remain_minute, minute);
}

/******************************************************************************************/
/* Format minute numeric prompt value.                                                    */
/******************************************************************************************/
VOID minute_prompt_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    int index = 0;

    if (value < 10)
    {
        prompt->gx_numeric_prompt_buffer[index++] = '0';
    }
    gx_utility_ltoa(value, prompt->gx_numeric_prompt_buffer + index, GX_NUMERIC_PROMPT_BUFFER_SIZE);
}