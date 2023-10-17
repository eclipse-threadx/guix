/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_defined_5_4_0_compatibility_resources.h"
#include "all_widgets_defined_5_4_0_compatibility_specifications.h"

#define DATA_COUNT 513
INT chart_data[DATA_COUNT + 1];
INT chart_data_min;
INT chart_data_max;

INT curve_id = ID_SIN;

void generate_line_chart_data()
{
    int num_data_vals;

    for (num_data_vals = chart_data_min; num_data_vals <= chart_data_max; num_data_vals++)
    {
        switch (curve_id)
        {
        case ID_SIN:
            chart_data[num_data_vals - chart_data_min] = gx_utility_math_sin(num_data_vals << 8);
            break;
            
        case ID_COS:
            chart_data[num_data_vals - chart_data_min] = gx_utility_math_cos(num_data_vals << 8);
            break;

        case ID_ASIN:
            chart_data[num_data_vals - chart_data_min] = gx_utility_math_asin(num_data_vals);
            break;

        case ID_ACOS:
            chart_data[num_data_vals - chart_data_min] = gx_utility_math_acos(num_data_vals);
            break;
        }
    }
    gx_line_chart_update((GX_LINE_CHART *)&math_screen.math_screen_line_chart_math, chart_data, chart_data_max - chart_data_min + 1);
}

void draw_sin()
{
    chart_data_min = 0;
    chart_data_max = 360;
    curve_id = ID_SIN;
    generate_line_chart_data();
}

void draw_cos()
{
    chart_data_min = 0;
    chart_data_max = 360;
    curve_id = ID_COS;
    generate_line_chart_data();
}

void draw_asin()
{
    chart_data_min = -256;
    chart_data_max = 256;
    curve_id = ID_ASIN;
    generate_line_chart_data();
}

void draw_acos()
{
    chart_data_min = -256;
    chart_data_max = 256;
    curve_id = ID_ACOS;
    generate_line_chart_data();
}

UINT math_screen_event_handle(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        draw_sin();
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_SIN, GX_EVENT_RADIO_SELECT):
        draw_sin();
        break;

    case GX_SIGNAL(ID_COS, GX_EVENT_RADIO_SELECT):
        draw_cos();
        break;

    case GX_SIGNAL(ID_ASIN, GX_EVENT_RADIO_SELECT):
        draw_asin();
        break;

    case GX_SIGNAL(ID_ACOS, GX_EVENT_RADIO_SELECT):
        draw_acos();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
