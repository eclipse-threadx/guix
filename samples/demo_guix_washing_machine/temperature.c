/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_washing_machine.h"

#define NEEDLE_TRANS_TIMER 3

#define MIN_TEMPERATURE 22
#define MAX_TEMPERATURE 90

#define MIN_ANGLE -46
#define MAX_ANGLE 228

INT current_temperature = 0;
INT target_angle = 34;
INT temperature_animation_steps = 0;
INT target_needle_pixelmap = 0;
extern INT  blend_alpha;
VOID temperature_value_set(INT new_value);
VOID temperature_page_init();
VOID temperature_page_power_off();

/******************************************************************************************/
/* Override default drawing of "temperature" window to draw some temperature indicator    */
/* lines.                                                                                 */
/******************************************************************************************/
VOID temperature_window_draw(GX_WINDOW *window)
{
    INT       xpos;
    INT       ypos;
    GX_BRUSH *brush;
    GX_RECTANGLE *size;

    /* Get context brush. */
    gx_context_brush_get(&brush);

    brush->gx_brush_alpha = blend_alpha;

    /* Call default window draw. */
    gx_window_draw(window);

    gx_context_line_color_set(GX_COLOR_ID_GRAY);
    gx_context_brush_width_set(1);
    gx_context_brush_style_set(GX_BRUSH_ALIAS);

    size = &temperature_window.temperature_window_water_t_cold.gx_widget_size;
    xpos = size->gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;


    /* Draw cold indicator line. */
    gx_canvas_line_draw(xpos, ypos, xpos + TEMPERATURE_WINDOW_LONG_LINE_WIDTH, ypos);
    gx_canvas_line_draw(xpos + TEMPERATURE_WINDOW_LONG_LINE_WIDTH + 1,
                        ypos,
                        xpos + TEMPERATURE_WINDOW_LONG_LINE_WIDTH + 1 + TEMPERATURE_WINDOW_SHORT_LINE_WIDTH,
                        ypos - TEMPERATURE_WINDOW_SHORT_LINE_WIDTH);

    size = &temperature_window.temperature_window_water_t_hot.gx_widget_size;
    xpos = size->gx_rectangle_left - 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;

    /* Draw hot indicator line. */
    gx_canvas_line_draw(xpos, ypos, xpos - TEMPERATURE_WINDOW_LONG_LINE_WIDTH, ypos);
    gx_canvas_line_draw(xpos - TEMPERATURE_WINDOW_LONG_LINE_WIDTH - 1,
                        ypos,
                        xpos - TEMPERATURE_WINDOW_LONG_LINE_WIDTH - 1 - TEMPERATURE_WINDOW_SHORT_LINE_WIDTH,
                        ypos - TEMPERATURE_WINDOW_SHORT_LINE_WIDTH);
}

/******************************************************************************************/
/* Override default event processing of "temperature" window to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT temperature_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    INT value;

    switch (event_ptr->gx_event_type)
    {

    case GX_SIGNAL(ID_TEMPERATURE_RADIAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        value = event_ptr->gx_event_payload.gx_event_intdata[0];

        /* Update temperature value. */
        temperature_value_set(MAX_TEMPERATURE - ((value - MIN_ANGLE) >> 2));

        if (is_power_on())
        {
            target_angle = event_ptr->gx_event_payload.gx_event_longdata;
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == NEEDLE_TRANS_TIMER)
        {
            /* Set needle pixelmap. */
            gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                GX_PIXELMAP_ID_BG_TEMPERATURE, target_needle_pixelmap);
        }
        else
        {
            return gx_window_event_process(window, event_ptr);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Set new temperature value.                                                             */
/******************************************************************************************/
VOID temperature_value_set(INT new_value)
{
    GX_CHAR string_buffer[10];
    GX_STRING string;
    GX_RADIAL_SLIDER *slider = &temperature_window.temperature_window_radial_slider;
    GX_RESOURCE_ID needle_pixelmap = slider->gx_radial_slider_info.gx_radial_slider_info_needle_pixelmap;

    /* Set temperature value. */
    gx_utility_ltoa(new_value, string_buffer, 10);

    string.gx_string_ptr = string_buffer;
    string.gx_string_length = string_length_get(string_buffer, sizeof(string_buffer) - 1);
    gx_prompt_text_set_ext(&temperature_window.temperature_window_temperature_value, &string);
    gx_prompt_text_set_ext(&main_screen.main_screen_temperature_value, &string);

    if (new_value > current_temperature)
    {
        if (new_value <= 40)
        {
            /* Change needle color to BLUE. */
            gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_BLUE);
        }
        else if (new_value <= 53)
        {
            if (needle_pixelmap == GX_PIXELMAP_ID_WHEEL_DOT_BLUE)
            {
                target_needle_pixelmap = GX_PIXELMAP_ID_WHEEL_DOT_YELLOW;

                /* Change needle color from BLUE to GREEN. */
                gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                    GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_GREEN);

                /* Start a timer to change needle color from GREEN to YELLOW. */
                gx_system_timer_start(&temperature_window, NEEDLE_TRANS_TIMER, 10, 0);
            }
        }
        else if (new_value <= 63)
        {
            /* Change needle color to YELLOW. */
            gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_YELLOW);
        }
        else if (new_value <= 71)
        {
            if (needle_pixelmap == GX_PIXELMAP_ID_WHEEL_DOT_YELLOW)
            {
                target_needle_pixelmap = GX_PIXELMAP_ID_WHEEL_DOT_RED;

                /* Change needle color from YELLOW to ORANGE. */
                gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                    GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_ORANGE);

                /* Start a timer to change needle color from ORANGE to RED. */
                gx_system_timer_start(&temperature_window, NEEDLE_TRANS_TIMER, 10, 0);
            }
        }
        else
        {
            /* Change needle color to RED. */
            gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_RED);
        }
    }
    else if (new_value < current_temperature)
    {
        if (new_value >= 40 && new_value <= 53)
        {
            if (needle_pixelmap == GX_PIXELMAP_ID_WHEEL_DOT_YELLOW)
            {
                target_needle_pixelmap = GX_PIXELMAP_ID_WHEEL_DOT_BLUE;

                /* Change needle color from YELLOW to GREEN. */
                gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                    GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_GREEN);

                /* Start a timer to change needle color from GREEN to BLUE. */
                gx_system_timer_start(&temperature_window, NEEDLE_TRANS_TIMER, 10, 0);
            }
        }
        else if (new_value >= 63 && new_value <= 71)
        {
            if (needle_pixelmap == GX_PIXELMAP_ID_WHEEL_DOT_RED)
            {
                target_needle_pixelmap = GX_PIXELMAP_ID_WHEEL_DOT_YELLOW;

                /* Change needle color from RED to ORANGE. */
                gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
                    GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_ORANGE);

                /* Start a timer to change needle color from ORANGE to YELLOW. */
                gx_system_timer_start(&temperature_window, NEEDLE_TRANS_TIMER, 10, 0);
            }
        }
    }

    current_temperature = new_value;
}

/******************************************************************************************/
/* Update some values when "temperature" button is selected.                              */
/******************************************************************************************/
VOID temperature_page_init()
{
    /* Change page name to "Temperature". */
    gx_prompt_text_id_set(&main_screen.main_screen_page_name, GX_STRING_ID_TEMPERATURE);

    /* Change page name color to purple. */
    gx_prompt_text_color_set(&main_screen.main_screen_page_name, GX_COLOR_ID_PURPLE, GX_COLOR_ID_PURPLE, GX_COLOR_ID_PURPLE);

    /* Set slider value to max_angle. */
    gx_radial_slider_angle_set(&temperature_window.temperature_window_radial_slider, MAX_ANGLE);

    /* Set needle pixelmap. */
    gx_radial_slider_pixelmap_set(&temperature_window.temperature_window_radial_slider,
        GX_PIXELMAP_ID_BG_TEMPERATURE, GX_PIXELMAP_ID_WHEEL_DOT_BLUE);

    current_temperature = MIN_TEMPERATURE;

    /* Start animation to move slider needle from current position to 34 degree. */
    gx_radial_slider_animation_start(&temperature_window.temperature_window_radial_slider, target_angle);

    widget_enable_disable((GX_WIDGET *)&temperature_window, POWER_ON);
}

/******************************************************************************************/
/* Update some values when "temperature" button is de-selected.                           */
/******************************************************************************************/
VOID temperature_page_power_off()
{
    gx_radial_slider_animation_start(&temperature_window.temperature_window_radial_slider, MAX_ANGLE);

    widget_enable_disable((GX_WIDGET *)&temperature_window, POWER_OFF);
}
