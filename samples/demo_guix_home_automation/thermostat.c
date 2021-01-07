
#include "demo_guix_home_automation.h"

#define MIN_DEGREE 40
#define MAX_DEGREE 120

/* Define thermostat controller information structure. */
typedef struct THERMOSTAT_INFO_STRUCT{
    THERMOSTAT_BASE_CONTROL_BLOCK *controller; /* Pointer to thermostat controller. */
    GX_CHAR *cold_heat_text; /* Temperature description text. */
    GX_RESOURCE_ID cold_heat_icon; /* ID of the temperature icon. */
    INT radial_angle; /* Current value of the thermostat radial slider. */
    INT power; /* Engery in use. */
    INT enabled; /* Flag if the thermostat controller is enabled. */
}THERMOSTAT_INFO;

/* This is a list of thermostat controler widgets and property values applied to those widgets. */
static THERMOSTAT_INFO thermostat_info_list[] = {
    { &thermostat_page_1.thermostat_page_1_kitchen, "cooling", GX_PIXELMAP_ID_BOX_ICON_COLD, 230, 0, GX_TRUE },
    { &thermostat_page_1.thermostat_page_1_master_bedroom, "heating", GX_PIXELMAP_ID_BOX_ICON_HEAT, -50, 0, GX_TRUE },
    { &thermostat_page_2.thermostat_page_2_kids_bedroom, "heading", GX_PIXELMAP_ID_BOX_ICON_HEAT, 90, 0, GX_TRUE },
    { &thermostat_page_2.thermostat_page_2_living_room, "cooling", GX_PIXELMAP_ID_BOX_ICON_COLD, 180, 0, GX_TRUE },
    { &thermostat_page_3.thermostat_page_3_dinning_room, "heating", GX_PIXELMAP_ID_BOX_ICON_HEAT, 0, 0, GX_TRUE },
    { &thermostat_page_3.thermostat_page_3_outdoor_patio, "cooling", GX_PIXELMAP_ID_BOX_ICON_COLD, 180, 0, GX_TRUE },
    { &thermostat_page_4.thermostat_page_4_office, "heading", GX_PIXELMAP_ID_BOX_ICON_HEAT, 0, 0, GX_TRUE },
    { GX_NULL, "", 0, 0, 0, 0 }
};

/* This variable is used for screen slide animation. */
extern GX_ANIMATION slide_animation;

/******************************************************************************************/
/* Retrieve thermostat information with specified thermostat controller.                  */
/******************************************************************************************/
static THERMOSTAT_INFO *thermostat_info_get(THERMOSTAT_BASE_CONTROL_BLOCK *controller)
{
    THERMOSTAT_INFO *info = thermostat_info_list;

    while (info->controller)
    {
        if (info->controller == controller)
        {
            return info;
        }
        info++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Update bottom information of thermostat screen.                                        */
/******************************************************************************************/
static VOID thermostat_screen_bottom_info_update()
{
    THERMOSTAT_INFO *info = thermostat_info_list;
    INT energy_in_use = 0;
    INT enabled = 0;
    INT disabled = 0;

    while (info->controller)
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

    /* Display the number of enabled thermostat controllers. */
    gx_numeric_prompt_value_set(&thermostat_screen.thermostat_screen_num_on, enabled);

    /* Display the number of disabled thermostat controllers. */
    gx_numeric_prompt_value_set(&thermostat_screen.thermostat_screen_num_off, disabled);

    /* Display the value of energy in use. */
    gx_numeric_prompt_value_set(&thermostat_screen.thermostat_screen_energy_in_use, energy_in_use);
}

/******************************************************************************************/
/* Update information of specified thermostat controller.                                 */
/******************************************************************************************/
static VOID thermostat_controller_info_update(THERMOSTAT_INFO *info, int radial_angle)
{
    THERMOSTAT_BASE_CONTROL_BLOCK *controller = info->controller;
    INT angle;
    INT angle_range = (MAX_ANGLE - MIN_ANGLE);
    INT value;
    INT xpos;
    GX_RESOURCE_ID icon;
    GX_RESOURCE_ID text_id;

    angle = MAX_ANGLE - radial_angle;

    value = MIN_DEGREE + angle * (MAX_DEGREE - MIN_DEGREE) / angle_range;

    /* Set thermostat value in center of the thermostat radial slider. */
    gx_numeric_prompt_value_set(&controller->thermostat_base_center_value, value);

    if (value < 100)
    {
        xpos = 185;
    }
    else
    {
        xpos = 200;
    }

    xpos += controller->base.gx_widget_size.gx_rectangle_left;

    gx_widget_shift(&controller->thermostat_base_dot_lable, xpos - controller->thermostat_base_dot_lable.gx_widget_size.gx_rectangle_left, 0, GX_TRUE); 

    if (value < 90)
    {
        icon = GX_PIXELMAP_ID_BOX_ICON_COLD;
        text_id = GX_STRING_ID_COOLING;
    }
    else
    {
        icon = GX_PIXELMAP_ID_BOX_ICON_HEAT;
        text_id = GX_STRING_ID_HEATING;
    }

    /* Set temperature icon. */
    gx_icon_pixelmap_set(&controller->thermostat_base_cold_heat_icon, icon, icon);

    /* Set temperature description text. */
    gx_prompt_text_id_set(&controller->thermostat_base_cold_heat_text, text_id);

    info->power = angle * MAX_POWER / angle_range;
}

/******************************************************************************************/
/* Event handler for close button click.                                                  */
/* Hide thermostat radial slider and show open button.                                    */
/******************************************************************************************/
static VOID on_close_button_clicked(THERMOSTAT_BASE_CONTROL_BLOCK *base)
{
    THERMOSTAT_INFO *info = thermostat_info_get(base);

    if (info && info->enabled)
    {
        info->enabled = GX_FALSE;

        /* detach radial slider. */
        gx_widget_detach(&base->thermostat_base_radial_slider);

        /* detach icon ruller. */
        gx_widget_detach(&base->thermostat_base_ruller_thermostat);

        gx_widget_detach(&base->thermostat_base_button_close);

        /* attach open button. */
        gx_widget_attach((GX_WIDGET *)base, &base->thermostat_base_button_open);

        thermostat_screen_bottom_info_update();
    }
}

/******************************************************************************************/
/* Event handler for open button click.                                                   */
/* Show thermostat radial slider and hide open button.                                    */
/******************************************************************************************/
static VOID on_open_button_clicked(THERMOSTAT_BASE_CONTROL_BLOCK *base)
{
    THERMOSTAT_INFO *info = thermostat_info_get(base);

    if (info && (!info->enabled))
    {
        info->enabled = GX_TRUE;

        /* attach icon ruller. */
        gx_widget_attach((GX_WIDGET *)base, &base->thermostat_base_ruller_thermostat);

        gx_widget_attach((GX_WIDGET *)base, &base->thermostat_base_button_close);

        /* attach radial slider. */
        gx_widget_attach((GX_WIDGET *)base, &base->thermostat_base_radial_slider);

        /* detach open button. */
        gx_widget_detach(&base->thermostat_base_button_open);

        thermostat_screen_bottom_info_update();
    }
}

/******************************************************************************************/
/* Reset status of one thermostat controller.                                             */
/******************************************************************************************/
static VOID thermostat_controller_reset(THERMOSTAT_BASE_CONTROL_BLOCK *widget)
{
    GX_WIDGET *child;

    /* Hide child widgets. */
    child = widget->base.gx_widget_first_child;
    while (child)
    {
        gx_widget_hide(child);
        child = child->gx_widget_next;
    }

    /* Reset the value of thermostat radial slider. */
    gx_radial_slider_angle_set(&widget->thermostat_base_radial_slider, MAX_ANGLE);

    /* Reset the text value in center of the thermostat radial slider. */
    gx_numeric_prompt_value_set(&widget->thermostat_base_center_value, 40);
}

/******************************************************************************************/
/* Reset status of all thermostat controllers.                                            */
/******************************************************************************************/
VOID thermostat_screen_reset()
{
    THERMOSTAT_INFO *info = thermostat_info_list;

    while (info->controller)
    {
        if (info->controller->base.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Reset status of one thermostat controller. */
            thermostat_controller_reset(info->controller);
        }

        info++;
    }
}

/******************************************************************************************/
/* Start animation for thermostat screen.                                                 */
/******************************************************************************************/
VOID thermostat_screen_animation_start()
{
    THERMOSTAT_INFO *info = thermostat_info_list;
    CONTROLLER_BASE_CONTROL_BLOCK *base;
    GX_WIDGET *child;

    while (info->controller)
    {
        if (info->controller->base.gx_widget_status & GX_STATUS_VISIBLE)
        {
            base = &info->controller->base;

            if (!(base->controller_base_title.gx_widget_status & GX_STATUS_VISIBLE))
            {
                /* Disable drag slide animation before child widget animations complete. */
                gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)&thermostat_screen.base.screen_base_slide_win);

                /* Disable pagination buttons before child widget animations complete. */
                pagination_button_enable_disable((GX_WINDOW *)&thermostat_screen.base, GX_FALSE);

                /* Slide in controller title. */
                title_animation_start(&info->controller->base);

                if (info->enabled)
                {
                    /* Start animation to move radial slider from current value to the target value. */
                    gx_radial_slider_animation_start(&info->controller->thermostat_base_radial_slider, info->radial_angle);
                }

                /* Show child widgets. */
                child = base->gx_widget_first_child;
                while (child)
                {
                    gx_widget_show(child);
                    child = child->gx_widget_next;
                }
            }
        }
        else
        {
            thermostat_controller_reset(info->controller);
        }

        info++;
    }
}


/******************************************************************************************/
/* Callback function of radial slider, which is called after every animation step.        */
/******************************************************************************************/
VOID thermostat_radial_slider_value_update(GX_RADIAL_SLIDER *slider)
{
    THERMOSTAT_INFO *info = thermostat_info_get((THERMOSTAT_BASE_CONTROL_BLOCK *)slider->gx_widget_parent);

    if (info && (slider->gx_widget_status & GX_STATUS_VISIBLE))
    {
        info->radial_angle = slider->gx_radial_slider_info.gx_radial_slider_info_current_angle;

        /* Update thermostat information values in thermostat controller. */
        thermostat_controller_info_update(info, info->radial_angle);

        /* Update thermostat information values in bottom of the thermostat screen. */
        thermostat_screen_bottom_info_update();
    }
}

/******************************************************************************************/
/* Override the default event processing of templates based on "thermostat_base" to       */
/* handle signals from my  child widgets.                                                 */
/******************************************************************************************/
UINT thermostat_base_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_RADIAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        thermostat_radial_slider_value_update(&((THERMOSTAT_BASE_CONTROL_BLOCK *)window)->thermostat_base_radial_slider);
        break;

    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        on_close_button_clicked((THERMOSTAT_BASE_CONTROL_BLOCK *)window);
        break;

    case GX_SIGNAL(ID_OPEN, GX_EVENT_CLICKED):
        on_open_button_clicked((THERMOSTAT_BASE_CONTROL_BLOCK *)window);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Initiate thermostat screen information.                                                */
/******************************************************************************************/
VOID thermostat_screen_init()
{
    THERMOSTAT_INFO *info = thermostat_info_list;

    while (info->controller)
    {
        gx_widget_detach(&info->controller->thermostat_base_button_open);

        /* Reset status of thermostat controller. */
        thermostat_controller_reset(info->controller);

        info++;
    }
     
    /* Initialize title text for each thermostat controllers. */
    thermostat_title_init();

    /* Set "Add Thermostat" widget text. */
    gx_prompt_text_id_set(&thermostat_screen.base.screen_base_add_text, GX_STRING_ID_ADD_ROOM);
}