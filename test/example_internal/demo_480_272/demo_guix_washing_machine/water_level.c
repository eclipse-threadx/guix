
#include "demo_guix_washing_machine.h"

#define WATER_LEVEL_ANIMATION_TIMER 0x2

/* Define variables. */

/* Define slider values of each water level. */
INT water_level_values[] = { 17, 37, 58, 80, 100 };

/* Define slider label widget of wach water level. */
GX_WIDGET *water_level_label_widgets[] = {
    (GX_WIDGET *)&water_level_window.water_level_window_water_level_very_low,
    (GX_WIDGET *)&water_level_window.water_level_window_water_level_low,
    (GX_WIDGET *)&water_level_window.water_level_window_water_level_medium,
    (GX_WIDGET *)&water_level_window.water_level_window_water_level_high,
    (GX_WIDGET *)&water_level_window.water_level_window_water_level_extra_high
};

/* Define water level animation steps. */
INT water_level_animation_steps = 0;

/* Define current water level value. */
INT current_water_level_value = 17;
INT target_water_level_value = 80;

extern INT blend_alpha;

/* Define prototypes. */
INT  water_level_index_get(INT value);
VOID water_level_animation_start(INT target_value);
VOID water_level_animation_update(GX_WIDGET *widget);
VOID water_level_page_init();
VOID water_level_page_power_off();
VOID water_level_value_set(int new_value);

/******************************************************************************************/
/* Override default drawing of "water level" window to draw 5 water level indicater lines */
/******************************************************************************************/
VOID water_level_window_draw(GX_WINDOW *window)
{
    INT xpos;
    INT ypos;
    INT xend;
    GX_BRUSH *brush;
    GX_RECTANGLE *size;

    /* Get context brush. */
    gx_context_brush_get(&brush);

    /* Set brush alpha. */
    brush->gx_brush_alpha = blend_alpha;

    /* Call default window draw. */
    gx_window_draw(window);

    gx_context_line_color_set(GX_COLOR_ID_GRAY);
    gx_context_brush_width_set(1);

    xend = water_level_window.water_level_window_water_level_slider_background.gx_widget_size.gx_rectangle_left - 5;

    /* Draw extra high line. */
    size = &water_level_window.water_level_window_water_level_extra_high.gx_widget_size;
    xpos = size->gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;

    gx_canvas_line_draw(xpos, ypos, xend, ypos);

    /* Draw high line. */
    size = &water_level_window.water_level_window_water_level_high.gx_widget_size;
    xpos = size->gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;

    gx_canvas_line_draw(xpos, ypos, xend, ypos);

    /* Draw medium line. */
    size = &water_level_window.water_level_window_water_level_medium.gx_widget_size;
    xpos = size->gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;

    gx_canvas_line_draw(xpos, ypos, xend, ypos);

    /* Draw low line. */
    size = &water_level_window.water_level_window_water_level_low.gx_widget_size;
    xpos = size->gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;

    gx_canvas_line_draw(xpos, ypos, xend, ypos);

    /* Draw very low line. */
    size = &water_level_window.water_level_window_water_level_very_low.gx_widget_size;
    xpos = size->gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;

    gx_canvas_line_draw(xpos, ypos, xend, ypos);
}

/******************************************************************************************/
/* Override default event processing of "water level" window to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT water_level_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_WATER_LEVEL_SLIDER, GX_EVENT_SLIDER_VALUE):
        /* New slider value is set, update water level labels. */
        water_level_value_set(event_ptr->gx_event_payload.gx_event_intdata[0]);
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_EXTRA_HIGH, GX_EVENT_CLICKED):
        /* Select water level 4. */
        water_level_animation_start(water_level_values[4]);
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_HIGH, GX_EVENT_CLICKED):
        /* Select water level 3. */
        water_level_animation_start(water_level_values[3]);
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_MEDIUM, GX_EVENT_CLICKED):
        /* Select water level 2. */
        water_level_animation_start(water_level_values[2]);
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_LOW, GX_EVENT_CLICKED):
        /* Select water level 1. */
        water_level_animation_start(water_level_values[1]);
        break;

    case GX_SIGNAL(ID_WATER_LEVEL_VERY_LOW, GX_EVENT_CLICKED):
        /* Selecte water level 0. */
        water_level_animation_start(water_level_values[0]);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == WATER_LEVEL_ANIMATION_TIMER)
        {
            water_level_animation_update((GX_WIDGET *)window);
        }
        else
        {
            gx_window_event_process(window, event_ptr);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Update some values when "water level" button is selected.                              */
/******************************************************************************************/
VOID water_level_page_init()
{
    /* Change page name to "Water Level". */
    gx_prompt_text_id_set(&main_screen.main_screen_page_name, GX_STRING_ID_WATER_LEVEL);

    /* Change page name color to blue. */
    gx_prompt_text_color_set(&main_screen.main_screen_page_name, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE);

    /* Set current water level value to water_level_values[0]. */
    gx_slider_value_set((GX_SLIDER *)&water_level_window.water_level_window_water_level_slider,
        (GX_SLIDER_INFO *)&water_level_window.water_level_window_water_level_slider.gx_slider_info, water_level_values[0]);

    /* Init current water level value. */
    current_water_level_value = 17;

    /* Start animation to move water level from current value to 80. */
    water_level_animation_start(target_water_level_value);

    widget_enable_disable((GX_WIDGET *)&water_level_window, POWER_ON);
}

/******************************************************************************************/
/* Update some values when "water level" button is de-selected.                           */
/******************************************************************************************/
VOID water_level_page_power_off()
{
    water_level_animation_start(17);

    widget_enable_disable((GX_WIDGET *)&water_level_window, POWER_OFF);
}

/******************************************************************************************/
/* Start an animation to move current water level to target water level.                  */
/******************************************************************************************/
VOID water_level_animation_start(INT target_value)
{
    /* Calcualte needed animation steps. */
    water_level_animation_steps = target_value - current_water_level_value;

    /* Start water level sliding animation. */
    gx_system_timer_start(&water_level_window, WATER_LEVEL_ANIMATION_TIMER, 1, 1);
}

/******************************************************************************************/
/* Update water level value during animation.                                             */
/******************************************************************************************/
VOID water_level_animation_update(GX_WIDGET *widget)
{
    INT new_value;

    /* Move water level value toward target value step by step. */
    if (water_level_animation_steps > 0)
    {
        if (water_level_animation_steps >= 2)
        {
            water_level_animation_steps -= 2;
            new_value = current_water_level_value + 2;
        }
        else
        {
            water_level_animation_steps--;
            new_value = current_water_level_value + 1;
        }
    }
    else
    {
        if (water_level_animation_steps <= 2)
        {
            water_level_animation_steps += 2;
            new_value = current_water_level_value - 2;
        }
        else
        {
            water_level_animation_steps++;
            new_value = current_water_level_value - 1;
        }
    }

    gx_slider_value_set((GX_SLIDER *)&water_level_window.water_level_window_water_level_slider,
        (GX_SLIDER_INFO *)&water_level_window.water_level_window_water_level_slider.gx_slider_info, new_value);

    if (water_level_animation_steps == 0)
    {
        gx_system_timer_stop(widget, WATER_LEVEL_ANIMATION_TIMER);
    }
}

/******************************************************************************************/
/* Set new water level value.                                                             */
/******************************************************************************************/
VOID water_level_value_set(int new_value)
{
    GX_CHAR string_buffer[10];
    GX_STRING string;
    INT old_index;
    INT new_index;

    /* Get old water level index. */
    old_index = water_level_index_get(current_water_level_value);

    /* Set new water level value. */
    current_water_level_value = new_value;

    if (is_power_on())
    {
        target_water_level_value = current_water_level_value;
    }

    /* Get new water level index. */
    new_index = water_level_index_get(current_water_level_value);

    /* Transfer water level value to text. */
    gx_utility_ltoa(current_water_level_value, string_buffer, 10);

    string.gx_string_ptr = string_buffer;
    string.gx_string_length = string_length_get(string_buffer, sizeof(string_buffer) - 1);

    /* Set water level value. */
    gx_prompt_text_set_ext(&water_level_window.water_level_window_water_level_value, &string);
    gx_prompt_text_set_ext(&main_screen.main_screen_water_level_value, &string);

    if (old_index != new_index)
    {
        /* Set old water level label color to light gray. */
        gx_prompt_text_color_set((GX_PROMPT *)water_level_label_widgets[old_index], GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_LIGHT_GRAY);

        /* Set new water level label color to blue. */
        gx_prompt_text_color_set((GX_PROMPT *)water_level_label_widgets[new_index], GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE);
    }
}

/******************************************************************************************/
/* Get water level index from water level value.                                          */
/******************************************************************************************/
INT water_level_index_get(INT value)
{
    /* Calculate water level index with slider value. */
    if (value < 22)
    {
        return 0;
    }
    else if (value < 48)
    {
        return 1;
    }
    else if (value < 69)
    {
        return 2;
    }
    else if (value < 90)
    {
        return 3;
    }
    else
    {
        return 4;
    }
}
