/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <math.h>
#include "tx_api.h"
#include "gx_api.h"
#include "non_linear_animation_resources.h"
#include "non_linear_animation_specifications.h"
#include "gx_utility.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

GX_ANIMATION       animation;
GX_ANIMATION_INFO  animation_info;
USHORT             animation_style = 0;

/* Define prototypes.   */
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

#define XPOS_START 492
#define XPOS_END   492
#define YPOS_START 341
#define YPOS_END   170
#define TOTAL_STEPS 40

#define ID_ANIMATION 1

INT chart_data[TOTAL_STEPS + 1];
int num_data_vals;

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Create animation control block. */
    memset(&animation, 0, sizeof(GX_ANIMATION));
    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    gx_animation_create(&animation);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

void animation_start()
{

    animation_info.gx_animation_start_position.gx_point_x = XPOS_START;
    animation_info.gx_animation_start_position.gx_point_y = YPOS_START;
    animation_info.gx_animation_end_position.gx_point_x = XPOS_END;
    animation_info.gx_animation_end_position.gx_point_y = YPOS_END;
    animation_info.gx_animation_id = ID_ANIMATION;
    animation_info.gx_animation_frame_interval = 2;
    animation_info.gx_animation_steps = TOTAL_STEPS;
    animation_info.gx_animation_start_alpha = 0xff;
    animation_info.gx_animation_end_alpha = 0xff;
    animation_info.gx_animation_target = (GX_WIDGET *)&main_screen.main_screen_icon_button;
    animation_info.gx_animation_parent = (GX_WIDGET *)&main_screen;
    animation_info.gx_animation_style = animation_style;
    gx_animation_start(&animation, &animation_info);
}

void generate_line_chart_data()
{
    INT ypos;

    if (animation.gx_animation_info.gx_animation_style != animation_style)
    {
        return;
    }

    for (num_data_vals = 0; num_data_vals <= TOTAL_STEPS; num_data_vals++)
    {
        _gx_utility_easing_function_calculate(animation_style, YPOS_START, YPOS_END, num_data_vals, TOTAL_STEPS, &ypos);
        chart_data[num_data_vals] = 256 * (ypos - YPOS_START) / (YPOS_END - YPOS_START);
    }
    gx_line_chart_update((GX_LINE_CHART *)&main_screen.main_screen_line_chart, chart_data, TOTAL_STEPS + 1);
}

UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_LINEAR, GX_EVENT_RADIO_SELECT):
        animation_style = 0;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_BACK_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_BACK_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_BACK_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_BACK_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_BACK_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_BACK_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_BOUNCE_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_BOUNCE_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_BOUNCE_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_BOUNCE_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_BOUNCE_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_BOUNCE_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_CIRC_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_CIRC_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_CIRC_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_CIRC_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_CIRC_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_CIRC_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_CUBIC_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_CUBIC_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_CUBIC_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_CUBIC_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_CUBIC_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_CUBIC_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_ELASTIC_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_ELASTIC_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_ELASTIC_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_ELASTIC_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_ELASTIC_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_ELASTIC_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_EXPO_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_EXPO_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_EXPO_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_EXPO_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_EXPO_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_EXPO_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUAD_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUAD_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUAD_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUAD_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUAD_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUAD_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUART_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUART_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUART_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUART_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUART_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUART_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUINT_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUINT_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUINT_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUINT_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_QUINT_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_QUINT_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_SINE_EASE_IN, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_SINE_EASE_IN;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_SINE_EASE_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_SINE_EASE_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_SINE_EASE_IN_OUT, GX_EVENT_RADIO_SELECT):
        animation_style = GX_ANIMATION_SINE_EASE_IN_OUT;
        animation_start();
        generate_line_chart_data();
        break;
    case GX_SIGNAL(ID_ANIMATION_START, GX_EVENT_CLICKED):
        animation_start();
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        if (animation.gx_animation_info.gx_animation_style != animation_style)
        {
            animation_start();
            generate_line_chart_data();
        }
        break;
    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}