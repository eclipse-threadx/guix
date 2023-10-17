
#include "demo_guix_washing_machine.h"

/* Define variables. */

/* Define angles of each garments mode. */
GX_VALUE garments_mode_angles[] = { -63, -34, -10, 13, 38, 67, 112, 142, 168, 191, 214, 242 };

/* Define icon angles of each garments mode, the icon is in the center of the radial slider, 
which rotate together with the slider needle. */
GX_VALUE garments_mode_icon_angles[] = { 300, 270, 240, 210, 190, 160, 110, 70, 50, 30, 0, -20 };

/* Define prompt widget that each mode correspond to. */
GX_WIDGET *garments_mode_label_widgets[] = {
    (GX_WIDGET *)&garments_window.garments_window_mode_workout_clothes,
    (GX_WIDGET *)&garments_window.garments_window_mode_hand_wash,
    (GX_WIDGET *)&garments_window.garments_window_mode_wool,
    (GX_WIDGET *)&garments_window.garments_window_mode_silk,
    (GX_WIDGET *)&garments_window.garments_window_mode_bedding,
    (GX_WIDGET *)&garments_window.garments_window_mode_baby_care,
    (GX_WIDGET *)&garments_window.garments_window_mode_cotton,
    (GX_WIDGET *)&garments_window.garments_window_mode_synthetics,
    (GX_WIDGET *)&garments_window.garments_window_mode_denim,
    (GX_WIDGET *)&garments_window.garments_window_mode_linen,
    (GX_WIDGET *)&garments_window.garments_window_mode_dark_colors,
    (GX_WIDGET *)&garments_window.garments_window_mode_light_colors
};

/* Define current selection mode index. */
static INT selected_index = 11;
static INT target_index = 6;

extern INT blend_alpha;
GX_PIXELMAP rotated_map;

/* Define prototypes. */
VOID garments_page_init();
VOID garments_mode_label_widgets_update(int old_selected_index);
VOID garments_mode_icon_angle_update(int remain_step);
void rotated_map_create(int angle);

/******************************************************************************************/
/* Override default event processing of "garments on" window to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT garments_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    GX_WIDGET *widget;
    int index;
    int old_selected_index;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);

        gx_radial_slider_anchor_angles_set(&garments_window.garments_window_radial_slider,
                                           garments_mode_angles, sizeof(garments_mode_angles) / sizeof(GX_VALUE));
        memset(&rotated_map, 0, sizeof(GX_PIXELMAP));
        rotated_map_create(garments_mode_icon_angles[selected_index]);
        break;

    case GX_SIGNAL(ID_GARMENTS_MODE_WORKOUT_CLOTHES, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_HAND_WASH, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_WOOL, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_SILK, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_BEDDING, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_BABY_CARE, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_COTTON, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_SYNTHETICS, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_DENIM, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_LINEN, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_DARK_COLORS, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_GARMENTS_MODE_LIGHT_COLORS, GX_EVENT_CLICKED):
        gx_widget_find((GX_WIDGET *)window, (USHORT)(event_ptr->gx_event_type >> 8), GX_SEARCH_DEPTH_INFINITE, &widget);
        if (widget)
        {
            for (index = 0; index < (int)(sizeof(garments_mode_angles) / sizeof(GX_VALUE)); index++)
            {
                if (garments_mode_label_widgets[index] == widget)
                {
                    /* Start animation to move slider needle from current mode to target mode. */
                    gx_radial_slider_animation_start(&garments_window.garments_window_radial_slider, garments_mode_angles[index]);
                }
            }
        }
        break;

    case GX_SIGNAL(ID_GARMENTS_ON_RADIAL_SLIDER, GX_EVENT_ANIMATION_START):
        for (index = 0; index < (int)(sizeof(garments_mode_angles) / sizeof(GX_VALUE)); index++)
        {
            if (garments_mode_angles[index] == event_ptr->gx_event_payload.gx_event_intdata[0])
            {
                target_index = index;
                break;
            }
        }
        break;

    case GX_SIGNAL(ID_GARMENTS_ON_RADIAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        for (index = 0; index < (int)(sizeof(garments_mode_angles) / sizeof(GX_VALUE)); index++)
        {
            if (garments_mode_angles[index] == event_ptr->gx_event_payload.gx_event_intdata[0])
            {
                old_selected_index = selected_index;
                selected_index = index;

                /* Update selection mode label. */
                garments_mode_label_widgets_update(old_selected_index);
            }
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Update some values when "garments on" button is selected.                              */
/******************************************************************************************/
VOID garments_page_init()
{
    GX_VALUE old_selected_index;

    /* Change page name to "Garments". */
    gx_prompt_text_id_set(&main_screen.main_screen_page_name, GX_STRING_ID_GARMENTS);

    /* Change page name color to orange. */
    gx_prompt_text_color_set(&main_screen.main_screen_page_name, GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE);

    /* Record old selection mode index. */
    old_selected_index = selected_index;

    /* Init selection index to mode 11. */
    selected_index = 11;

    /* Set slider value to mode 11. */
    gx_radial_slider_angle_set(&garments_window.garments_window_radial_slider, garments_mode_angles[selected_index]);

    /* Update selection mode labels. */
    garments_mode_label_widgets_update(old_selected_index);

    gx_radial_slider_animation_set(&garments_window.garments_window_radial_slider, 15, 2,
                                   GX_ANIMATION_CIRC_EASE_IN_OUT, garments_mode_animation_update);

    /* Start animation to move slider needle from mode 11 to mode 6*/
    gx_radial_slider_animation_start(&garments_window.garments_window_radial_slider, garments_mode_angles[target_index]);

    widget_enable_disable((GX_WIDGET *)&garments_window, POWER_ON);
}

/******************************************************************************************/
/* Update some values when "garments on" button is de-selected.                           */
/******************************************************************************************/
void garments_page_power_off()
{
    widget_enable_disable((GX_WIDGET *)&garments_window, POWER_OFF);
}

/******************************************************************************************/
/* Update text color of selected and de-selected mode labels.                             */
/******************************************************************************************/
VOID garments_mode_label_widgets_update(int old_selected_index)
{
    /* Change old selected mode label color to light gray. */
    gx_prompt_text_color_set((GX_PROMPT *)garments_mode_label_widgets[old_selected_index], GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);

    /* Change new selected mode label color to orange. */
    gx_prompt_text_color_set((GX_PROMPT *)garments_mode_label_widgets[selected_index], GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE);


    rotated_map_create(garments_mode_icon_angles[selected_index]);

    /* Mark icon window as dirty. */
    gx_system_dirty_mark((GX_WIDGET *)&garments_window.garments_window_icon_window);
}

VOID garments_mode_animation_update(GX_RADIAL_SLIDER *slider)
{
    INT current_angle = slider->gx_radial_slider_info.gx_radial_slider_info_current_angle;

    garments_mode_icon_angle_update(current_angle);
}

/******************************************************************************************/
/* Update rotating angle of the icon in the middle of the radial slider widget.           */
/******************************************************************************************/
VOID garments_mode_icon_angle_update(int current_val)
{
    INT icon_target_angle = garments_mode_icon_angles[target_index];
    INT icon_start_angle = garments_mode_icon_angles[selected_index];
    INT target_val = garments_mode_angles[target_index];
    INT start_val = garments_mode_angles[selected_index];
    INT angle;

    if (target_index == selected_index)
    {
        return;
    }

    angle = (icon_target_angle - icon_start_angle) * (current_val - start_val)
                                  / (target_val - start_val);
    angle += icon_start_angle;

    rotated_map_create(angle);

    /* Mark icon window as dirty. */
    gx_system_dirty_mark((GX_WIDGET *)&garments_window.garments_window_icon_window);
}

/******************************************************************************************/
/* Override default drawing of the icon in the center of the radial slider to draw the    */
/* icon with specified angle.                                                             */
/******************************************************************************************/
VOID icon_window_draw(GX_WINDOW *window)
{
    GX_PIXELMAP *map;
    int xpos;
    int ypos;

    if (rotated_map.gx_pixelmap_data)
    {
        map = &rotated_map;
    }
    else
    {
        /* Draw icon with the current icon angle. */
        gx_context_pixelmap_get(GX_PIXELMAP_ID_ICON_SOCK, &map);
    }

    xpos = (window->gx_widget_size.gx_rectangle_left + window->gx_widget_size.gx_rectangle_right) >> 1;
    xpos -= map->gx_pixelmap_width >> 1;

    ypos = (window->gx_widget_size.gx_rectangle_top + window->gx_widget_size.gx_rectangle_bottom) >> 1;
    ypos -= map->gx_pixelmap_height >> 1;

    gx_context_fill_color_set(GX_COLOR_ID_ORANGE);

    if (blend_alpha != GX_ALPHA_VALUE_OPAQUE)
    {
        gx_canvas_pixelmap_blend(xpos, ypos, map, blend_alpha);
    }
    else
    {
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
}

/******************************************************************************************/
/* Rotate a pixelmap.                                                                     */
/******************************************************************************************/
void rotated_map_create(int angle)
{
    GX_PIXELMAP *map = GX_NULL;

    if (rotated_map.gx_pixelmap_data)
    {
        memory_free((void *)rotated_map.gx_pixelmap_data);
        rotated_map.gx_pixelmap_data = GX_NULL;
    }

    if (rotated_map.gx_pixelmap_aux_data)
    {
        memory_free((void *)rotated_map.gx_pixelmap_aux_data);
        rotated_map.gx_pixelmap_aux_data = GX_NULL;
    }

    if (angle == 0)
    {
        return;
    }

    gx_widget_pixelmap_get(&garments_window.garments_window_icon_window, GX_PIXELMAP_ID_ICON_SOCK, &map);

    if (map)
    {
        gx_utility_pixelmap_rotate(map, angle, &rotated_map, GX_NULL, GX_NULL);
    }
}
