
#include "demo_guix_home_automation.h"

extern APP_INFO app_info;

static INT input_code_count = 0;

/* This is a list of passcode icons, one icon correspond to on passcode. */
static GX_ICON *passcode_icon_list[] = {
    &passcode_screen.passcode_screen_passcode_1,
    &passcode_screen.passcode_screen_passcode_2,
    &passcode_screen.passcode_screen_passcode_3,
    &passcode_screen.passcode_screen_passcode_4,
    GX_NULL
};

/******************************************************************************************/
/* Event handler for number button click.                                                 */
/******************************************************************************************/
static VOID on_passcode_input()
{
    INT icon_id;
    INT text_color_id;

    if (input_code_count > 3)
    {
        return;
    }

    switch (input_code_count)
    {
    case 0:
    case 1:
    case 2:
        icon_id = GX_PIXELMAP_ID_PASSCODE_ASTERIC_ACTIVE;
        break;

    case 3:
        icon_id = GX_PIXELMAP_ID_PASSCODE_ASTERIC_WRONG;
        text_color_id = GX_COLOR_ID_RED;

        gx_prompt_text_id_set(&passcode_screen.passcode_screen_title, GX_STRING_ID_WRONG_PASSCODE);
        gx_prompt_text_color_set(&passcode_screen.passcode_screen_title, text_color_id, text_color_id, text_color_id);
        break;
    }

    gx_icon_pixelmap_set(passcode_icon_list[input_code_count], icon_id, icon_id);
    input_code_count++;
}

/******************************************************************************************/
/* Event handler for clear button click.                                                  */
/******************************************************************************************/
static VOID on_clear()
{
    GX_ICON *icon;
    INT index = 0;
    GX_RESOURCE_ID icon_id = GX_PIXELMAP_ID_PASSCODE_ASTERIC;
    INT text_color_id = GX_COLOR_ID_WHITE;
    GX_ANIMATION *animation;
    GX_ANIMATION_INFO info;

    if (!input_code_count)
    {
        /* No code been input yet, just return. */
        return;
    }

    /* Reset input code count to 0. */
    input_code_count = 0;

    memset(&info, 0, sizeof(GX_ANIMATION_INFO));

    info.gx_animation_parent = (GX_WIDGET *)&passcode_screen;
    info.gx_animation_start_position.gx_point_y = -60;
    info.gx_animation_end_position.gx_point_y = 78;
    info.gx_animation_steps = 600 / GX_SYSTEM_TIMER_MS;
    info.gx_animation_frame_interval = 1;
    info.gx_animation_start_alpha = 255;
    info.gx_animation_end_alpha = 255;
    info.gx_animation_style = GX_ANIMATION_CIRC_EASE_OUT;

    while (1)
    {
        icon = passcode_icon_list[index];

        if (icon)
        {
            if ((icon->gx_icon_normal_pixelmap != icon_id) &&
                (gx_system_animation_get(&animation) == GX_SUCCESS))
            {
                info.gx_animation_start_position.gx_point_x = icon->gx_widget_size.gx_rectangle_left;
                info.gx_animation_end_position.gx_point_x = icon->gx_widget_size.gx_rectangle_left;
                info.gx_animation_target = (GX_WIDGET *)icon;
                info.gx_animation_start_delay += 5;

                /* Show cleared passcode icon with animation. */
                gx_animation_start(animation, &info);
            }

            /* Reset icon pixelmaps. */
            gx_icon_pixelmap_set(icon, icon_id, icon_id);

            index++;
        }
        else
        {
            break;
        }
    }

    gx_prompt_text_id_set(&passcode_screen.passcode_screen_title, GX_STRING_ID_ENTER_PASSCODE);
    gx_prompt_text_color_set(&passcode_screen.passcode_screen_title, text_color_id, text_color_id, text_color_id);
}

/******************************************************************************************/
/* Override the default event processing of "passcode_screen" to handle signals from my   */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT passcode_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        on_clear();
        break;

    case GX_SIGNAL(ID_NUM_0, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_1, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_2, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_3, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_4, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_5, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_6, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_7, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_8, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_NUM_9, GX_EVENT_CLICKED):
        on_passcode_input();
        break;

    case GX_SIGNAL(ID_CLEAR, GX_EVENT_CLICKED):
        on_clear();
        break;

    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        gx_widget_attach(window->gx_widget_parent, app_info.current_screen);
        gx_widget_detach(window);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
