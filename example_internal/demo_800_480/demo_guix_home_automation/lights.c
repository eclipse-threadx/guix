
#include "demo_guix_home_automation.h"

/* Define light information structure. */
typedef struct LIGHTS_INFO_STRUCT{
    LIGHTS_BASE_CONTROL_BLOCK *light; /* Pointer to the light controller. */
    GX_RESOURCE_ID color_text_id; /* Description of the light color. */
    INT radial_angle; /* Current value of the light control radial slider. */
    INT power; /* Energy use of the light. */
    GX_RESOURCE_ID selected_map_id; /* ID of the pixelmap for filling the selected part of the radial slider. */
    GX_BOOL enabled; /* On/Off status of the light. */
}LIGHT_INFO;

/* Information about light in various rooms. */
static LIGHT_INFO light_info_list[] = {
    { &lights_page_1.lights_page_1_kitchen, GX_STRING_ID_COLD_WHITE, 37, 0, GX_PIXELMAP_ID_WHEEL_WHITE_COLD, GX_TRUE},
    { &lights_page_1.lights_page_1_master_bedroom,  GX_STRING_ID_RGB_LIGHT, 100, 0, GX_PIXELMAP_ID_WHEEL_RED, GX_TRUE },
    { &lights_page_2.lights_page_2_kids_bedroom,  GX_STRING_ID_RGB_LIGHT, 29, 0,GX_PIXELMAP_ID_WHEEL_GREEN, GX_TRUE },
    { &lights_page_2.lights_page_2_living_room,  GX_STRING_ID_RGB_LIGHT, 29, 0, GX_PIXELMAP_ID_WHEEL_BLUE_DARK, GX_TRUE },
    { &lights_page_3.lights_page_3_dinning_room,  GX_STRING_ID_WARM_WHITE, 37, 0, GX_PIXELMAP_ID_WHEEL_WHITE_WARM, GX_TRUE },
    { &lights_page_3.lights_page_3_outdoor_patio,  GX_STRING_ID_RGB_LIGHT, 99, 0, GX_PIXELMAP_ID_WHEEL_BLUE_LIGHT, GX_TRUE },
    { &lights_page_4.lights_page_4_office,  GX_STRING_ID_RGB_LIGHT, 28, 0, GX_PIXELMAP_ID_WHEEL_PURPLE, GX_TRUE },
    { GX_NULL, 0, 0, 0, 0, 0}
};

extern GX_ANIMATION slide_animation;
extern int screen_animation_count;

UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/******************************************************************************************/
/* Retrieve light information with the specified light controller pointer.                */
/******************************************************************************************/
static LIGHT_INFO *light_info_get(LIGHTS_BASE_CONTROL_BLOCK *light)
{
    LIGHT_INFO *info = light_info_list;

    while (info->light)
    {
        if (info->light == light)
        {
            return info;
        }

        info++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Retrieve selected pixelmap id for slider in light screen.                              */
/******************************************************************************************/
GX_RESOURCE_ID light_selected_map_id_get(GX_RADIAL_SLIDER *slider)
{
    LIGHT_INFO *info = light_info_list;

    while (info->light)
    {
        if (&info->light->lights_base_radial_slider == slider)
        {
            return info->selected_map_id;
        }

        info++;
    }

    return 0;
}

/******************************************************************************************/
/* Update bottom information of light screen.                                             */
/******************************************************************************************/
static VOID update_bottom_info_of_lights()
{
    LIGHT_INFO *info = light_info_list;
    int energy_in_use = 0;
    int enabled = 0;
    int disabled = 0;

    /* Calculate the total enery in use and the number of enabled/disabled lights. */
    while (info->light)
    {
        if (info->enabled)
        {
            energy_in_use += info->power;
            enabled++;
        }
        else
        {
            disabled++;
        }

        info++;
    }

    /* Display the number of enabled lights. */
    gx_numeric_prompt_value_set(&lights_screen.lights_screen_num_on, enabled);

    /* Display the number of disabled lights. */
    gx_numeric_prompt_value_set(&lights_screen.lights_screen_num_off, disabled);

    /* Display the total energy in use. */
    gx_numeric_prompt_value_set(&lights_screen.lights_screen_energy_in_use, energy_in_use);
}

/******************************************************************************************/
/* Update information for specified light.                                                */
/******************************************************************************************/
static VOID power_value_update(LIGHT_INFO *info, int radial_angle)
{
    INT angle;
    INT angle_range = (MAX_ANGLE - MIN_ANGLE);
    INT value;
    INT xpos;

    /* Calcualte the light intensity. */
    angle = MAX_ANGLE - radial_angle;
    value = angle * 100 / angle_range;

    /* Display the light intensity. */
    gx_numeric_prompt_value_set(&info->light->lights_base_power_percent_value, value);

    if (value < 10)
    {
        xpos = 165;
    }
    else  if (value < 100)
    {
        xpos = 179;
    }
    else
    {
        xpos = 192;
    }

    xpos += info->light->base.gx_widget_size.gx_rectangle_left;

    /* Shift light intensity widget to make it in the center of the radial slider. */
    gx_widget_shift(&info->light->lights_base_precent_lable, xpos - info->light->lights_base_precent_lable.gx_widget_size.gx_rectangle_left, 0, GX_TRUE);

    /* Calcualte the energy of the light. */
    value = angle * MAX_POWER / angle_range;
    info->power = value;

    /* Display the energy of the light. */
    gx_numeric_prompt_value_set(&info->light->lights_base_power_value, value);
}

/******************************************************************************************/
/* Event handler for light close button click.                                            */
/* This function detaches light control radial slider, attaches light open button,        */
/* and updates light information in bottom area.                                          */
/******************************************************************************************/
static VOID on_close_button_clicked(LIGHTS_BASE_CONTROL_BLOCK *base)
{
    LIGHT_INFO *info = light_info_get(base);

    if (info && info->enabled)
    {
        info->enabled = GX_FALSE;

        /* detach radial slider. */
        gx_widget_detach(&base->lights_base_radial_slider);

        /* detach icon ruller. */
        gx_widget_detach(&base->lights_base_icon_ruller);

        gx_widget_detach(&base->lights_base_button_close);

        /* attach open button. */
        gx_widget_attach((GX_WIDGET *)base, &base->lights_base_button_open);

        update_bottom_info_of_lights();
    }
}

/******************************************************************************************/
/* Event handler for light open button click.                                             */
/* This function attaches light control radial slider, attaches light close button,       */
/* and updates light information in bottom area.                                          */
/******************************************************************************************/
static VOID on_open_button_clicked(LIGHTS_BASE_CONTROL_BLOCK *base)
{
    LIGHT_INFO *info = light_info_get(base);

    if (info && (!info->enabled))
    {
        info->enabled = GX_TRUE;

        /* attach icon ruller. */
        gx_widget_attach((GX_WIDGET *)base, &base->lights_base_icon_ruller);

        gx_widget_attach((GX_WIDGET *)base, &base->lights_base_button_close);

        /* attach radial slider. */
        gx_widget_attach((GX_WIDGET *)base, &base->lights_base_radial_slider);

        /* detach open button. */
        gx_widget_detach(&base->lights_base_button_open);

        update_bottom_info_of_lights();
    }
}

/******************************************************************************************/
/* Reset status of light.                                                                 */
/******************************************************************************************/
static VOID light_status_reset(LIGHT_INFO *info)
{
    GX_WIDGET *child;

    /* Hide all children of light controller. */
    child = info->light->base.gx_widget_first_child;
    while (child)
    {
        gx_widget_hide(child);
        child = child->gx_widget_next;
    }

    /* Reset light control radial slider value. */
    gx_radial_slider_angle_set(&info->light->lights_base_radial_slider, MAX_ANGLE);

    /* Reset light intensity value. */
    gx_numeric_prompt_value_set(&info->light->lights_base_power_percent_value, 0);

    /* Reset light energy value. */
    gx_numeric_prompt_value_set(&info->light->lights_base_power_value, 0);
}

/******************************************************************************************/
/* Reset status of light screen.                                                          */
/******************************************************************************************/
VOID lights_screen_reset()
{
    LIGHT_INFO *info = light_info_list;

    while (info->light)
    {
        if (info->light->base.gx_widget_status & GX_STATUS_VISIBLE)
        {
            light_status_reset(info);
        }

        info++;
    }
}

/******************************************************************************************/
/* Start children animations for visible light controller, this function is called when   */
/* light pages switched.                                                                  */
/******************************************************************************************/
VOID lights_screen_animation_start()
{
    LIGHT_INFO *info = light_info_list;
    GX_WIDGET *child;

    while (info->light)
    {
        if (info->light->base.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Light controller is visible. */

            if (!(info->light->base.controller_base_title.gx_widget_status & GX_STATUS_VISIBLE))
            {
                /* Title of the light controller is not visible, which means light page switched.
                   start children animations for the light controller. */

                /* Disable drag slide animation before screen item animations complete. */
                gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)&lights_screen.base.screen_base_slide_win);

                /* Disable pagination buttons to avoid button click while animations is on. */
                pagination_button_enable_disable((GX_WINDOW *)&lights_screen.base, GX_FALSE);

                /* Slide in light controller title. */
                title_animation_start(&info->light->base);

                if (info->enabled)
                {
                    /* If the light controller is on, start animation to move light control radial slider from initial
                       value to current value. */
                    gx_radial_slider_animation_start(&info->light->lights_base_radial_slider, info->radial_angle);
                }

                /* Show children of the light controller. */
                child = info->light->base.gx_widget_first_child;
                while (child)
                {
                    gx_widget_show(child);
                    child = child->gx_widget_next;
                }
            }
        }
        else
        {
            /* Reset status of light controller. */
            light_status_reset(info);
        }

        info++;
    }
}

/******************************************************************************************/
/* Stop all animations in lights screen.                                                  */
/******************************************************************************************/
VOID lights_screen_animation_stop()
{
    LIGHT_INFO *info = light_info_list;

    while (info->light)
    {
        if (info->light->base.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Light controller is visible. */
            gx_animation_delete(GX_NULL, (GX_WIDGET *)&info->light->base);
        }

        info++;
    }
    
    screen_animation_count = 0;
}

/******************************************************************************************/
/* Callback function of radial slider, which is called after every animation step.        */
/******************************************************************************************/
VOID radial_slider_value_update(GX_RADIAL_SLIDER *slider)
{
    LIGHT_INFO *info = light_info_get((LIGHTS_BASE_CONTROL_BLOCK *)slider->gx_widget_parent);

    if (info && (slider->gx_widget_status & GX_STATUS_VISIBLE))
    {
        info->radial_angle = slider->gx_radial_slider_info.gx_radial_slider_info_current_angle;

        /* Update light energy value. */
        power_value_update(info, info->radial_angle);

        /* Update summary light information in bottom. */
        update_bottom_info_of_lights();
    }
}

/******************************************************************************************/
/* Format function of numeric prompt.                                                     */
/******************************************************************************************/
VOID power_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    INT index;

    gx_utility_ltoa(value / 10000, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);

    index = string_length_get(prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    prompt->gx_numeric_prompt_buffer[index++] = '.';

    gx_utility_ltoa(value % 10000, prompt->gx_numeric_prompt_buffer + index, GX_NUMERIC_PROMPT_BUFFER_SIZE - index);
}

/******************************************************************************************/
/* Override the default event processing of templates based on "lights_base" to handle    */
/* signals from my child widgets.                                                         */
/******************************************************************************************/
UINT lights_base_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_RADIAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        radial_slider_value_update(&((LIGHTS_BASE_CONTROL_BLOCK *)window)->lights_base_radial_slider);
        break;

    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        on_close_button_clicked((LIGHTS_BASE_CONTROL_BLOCK *)window);
        break;

    case GX_SIGNAL(ID_OPEN, GX_EVENT_CLICKED):
        on_open_button_clicked((LIGHTS_BASE_CONTROL_BLOCK *)window);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Initiate light screen information.                                                     */
/******************************************************************************************/
VOID lights_screen_init()
{
    LIGHT_INFO *info = light_info_list;

    while (info->light)
    {
        /* Set light color text. */
        gx_prompt_text_id_set(&info->light->lights_base_color_text, info->color_text_id);

        /* detach open button. */
        gx_widget_detach(&info->light->lights_base_button_open);

        light_status_reset(info);

        info++;
    }

    lights_title_init();

    /* Set add button text to "ADD LIGHT". */
    gx_prompt_text_id_set(&lights_screen.base.screen_base_add_text, GX_STRING_ID_ADD_LIGHT);
}