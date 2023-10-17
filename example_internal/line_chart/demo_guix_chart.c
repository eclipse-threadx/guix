/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

/* The GUIX line chart widget has not yet been introduced into GUIX library, so temporarily need
   to include this header file seperately
*/
#include <time.h>

#include "line_chart_resources.h"
#include "line_chart_specifications.h"

GX_WINDOW_ROOT    *root;
void start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
VOID generate_chart_data(VOID);

#define ID_PRESSURE_CHART     200
#define ID_CHART_TIMER          1
#define CHART_UPDATE_PERIOD   100
#define MAX_DATA_COUNT        60
#define DATA_MAX_VAL          24
#define DATA_MIN_VAL           0
#define NUM_VERTICAL_GRID_LINES 24

#define AXIS_LINE_WIDTH  3
#define DATA_LINE_WIDTH  2
#define GRID_LINE_WIDTH  1

INT chart_data[MAX_DATA_COUNT];
int num_data_vals;


///////////////////////////////////////////////////////////////////////////////
// TEST line chart appearance- additional setting specific to the appearance
//      of the pressure chart.
typedef struct TEST_LINE_CHART_INFO_STRUCT
{
    GX_RESOURCE_ID high_line_color;
    GX_RESOURCE_ID med_line_color;
    GX_RESOURCE_ID low_line_color;
    GX_RESOURCE_ID grid_color;
    GX_RESOURCE_ID axis_label_color;
    GX_RESOURCE_ID axis_label_font;
    INT            high_line_value;
    INT            med_line_value;
    INT            low_line_value;
    ULONG          dash_line_pattern;
    GX_BOOL        draw_grid;
} TEST_LINE_CHART_INFO;

TEST_LINE_CHART_INFO pressure_chart_info;

///////////////////////////////////////////////////////////////////////////////
// program entry point
int main(int argc, char ** argv)
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
    
    return(0);
}

///////////////////////////////////////////////////////////////////////////////
// Standard ThreadX startup function
VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}


///////////////////////////////////////////////////////////////////////////////
// GUIX startup function
VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* create display driver instance */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb, 
		LANGUAGE_ENGLISH, MAIN_DISPLAY_DEFAULT_THEME, &root);
    
    /* create the thermometer screen */
    gx_studio_named_widget_create("pressure_trend_frame", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}


///////////////////////////////////////////////////////////////////////////////
// TEST line chart drawing of scale along X axis
VOID test_line_chart_scale_draw(GX_LINE_CHART *chart)
{
    INT scale_count;
    INT scale_val;
    INT pos;
    INT step;
    INT offset;
    INT line_start;
    char ScaleString[10];
    GX_RECTANGLE chart_bound;
    GX_LINE_CHART_INFO *gx_info;
    GX_STRING str;

    gx_info = &chart->gx_line_chart_info;
    chart_bound = chart->gx_widget_size;
    chart_bound.gx_rectangle_right -= gx_info->gx_line_chart_right_margin;
    chart_bound.gx_rectangle_bottom -= gx_info->gx_line_chart_bottom_margin;

    // draw the vertical tick markss first
    step = chart_bound.gx_rectangle_right - chart_bound.gx_rectangle_left + 1;
    step = GX_FIXED_VAL_MAKE(step);
    
    /* make each grid value == 2.5, so 24 gridlines? */
    step /= NUM_VERTICAL_GRID_LINES;
    offset = GX_FIXED_VAL_MAKE(chart_bound.gx_rectangle_left);
    gx_context_line_color_set(pressure_chart_info.grid_color);

    pos = GX_FIXED_VAL_TO_INT(offset);
    gx_context_brush_width_set(1);
    line_start = chart_bound.gx_rectangle_bottom + 1;
    scale_count = 0;
    scale_val = 0;

    do {
        if ((scale_count % 4)  == 0)
        {
            gx_utility_ltoa(scale_val, ScaleString, 10);
            gx_canvas_line_draw(pos, line_start, pos, line_start + 14);

            gx_context_line_color_set(pressure_chart_info.axis_label_color);
            str.gx_string_ptr = ScaleString;
            str.gx_string_length = strnlen(ScaleString, sizeof(ScaleString));
            gx_canvas_text_draw_ext(pos + 3, line_start + 6, &str);
            gx_context_line_color_set(pressure_chart_info.grid_color);
            scale_val += 10;
        }
        else
        {
            gx_canvas_line_draw(pos, line_start, pos, line_start + 8);
        }
        scale_count++;
        offset += step;
        pos = GX_FIXED_VAL_TO_INT(offset);

    } while (pos < chart_bound.gx_rectangle_right);
}



///////////////////////////////////////////////////////////////////////////////
// test line chart draw sub function, draw the reference grid.
VOID test_line_chart_grid_draw(GX_LINE_CHART *chart)
{
    INT pos;
    INT step;
    INT offset;
    INT line_start;
    INT line_end;
    GX_RECTANGLE chart_bound;

    GX_LINE_CHART_INFO *gx_info;

    gx_info = &chart->gx_line_chart_info;
    chart_bound = chart->gx_widget_size;
    chart_bound.gx_rectangle_right -= gx_info->gx_line_chart_right_margin;
    chart_bound.gx_rectangle_bottom -= gx_info->gx_line_chart_bottom_margin;

    // draw the vertial gridlines first

    step = chart_bound.gx_rectangle_right - chart_bound.gx_rectangle_left + 1;
    step = GX_FIXED_VAL_MAKE(step);
    
    /* make each grid value == 2.5, so 24 gridlines? */

    step /= NUM_VERTICAL_GRID_LINES;
    offset = GX_FIXED_VAL_MAKE(chart_bound.gx_rectangle_left) + step;

    pos = GX_FIXED_VAL_TO_INT(offset);
    gx_context_brush_width_set(1);
    gx_context_line_color_set(pressure_chart_info.grid_color);
    line_start = chart_bound.gx_rectangle_top;
    line_end = chart_bound.gx_rectangle_bottom - gx_info->gx_line_chart_axis_line_width;

    do {
        gx_canvas_line_draw(pos, line_start, pos, line_end);
        offset += step;
        pos = GX_FIXED_VAL_TO_INT(offset);
    } while (pos < chart_bound.gx_rectangle_right);

    // now draw the horizontal gridlines
    step = chart_bound.gx_rectangle_bottom - chart_bound.gx_rectangle_top + 1;
    step = GX_FIXED_VAL_MAKE(step);
    
    /* 7 gridlines? */
    step /= 7;
    offset = GX_FIXED_VAL_MAKE(chart_bound.gx_rectangle_bottom) - step;
    pos = GX_FIXED_VAL_TO_INT(offset);

    line_start = chart_bound.gx_rectangle_left + gx_info->gx_line_chart_axis_line_width;
    line_end = chart_bound.gx_rectangle_right;

    do {
        gx_canvas_line_draw(line_start, pos, line_end, pos);
        offset -= step;
        pos = GX_FIXED_VAL_TO_INT(offset);
    } while (pos > chart_bound.gx_rectangle_top);
}

///////////////////////////////////////////////////////////////////////////////
// test line chart draw sub function, draw the hi-med-low lines.
VOID test_line_chart_reference_lines_draw(GX_LINE_CHART *chart)
{
    INT y_scale;
    INT y_pos;
    INT bottom_pos;
    INT line_start;
    INT line_end;
    GX_BRUSH *brush;
    GX_FONT  *label_font;
    GX_RECTANGLE chart_bound;
    GX_STRING str;

    GX_LINE_CHART_INFO *gx_info = &chart->gx_line_chart_info;
    chart_bound = chart->gx_widget_size;
    chart_bound.gx_rectangle_right -= gx_info->gx_line_chart_right_margin;
    chart_bound.gx_rectangle_bottom -= gx_info->gx_line_chart_bottom_margin;

    gx_line_chart_y_scale_calculate(chart, &y_scale);
    line_start = chart_bound.gx_rectangle_left + gx_info->gx_line_chart_axis_line_width;
    line_end = chart->gx_widget_size.gx_rectangle_right;
    bottom_pos = chart_bound.gx_rectangle_bottom;
    gx_context_brush_width_set(1);
    gx_context_brush_get(&brush);
    brush->gx_brush_line_pattern = pressure_chart_info.dash_line_pattern;
    brush->gx_brush_pattern_mask = 0x80000000;

    // draw the high line
    y_pos = pressure_chart_info.high_line_value - chart->gx_line_chart_info.gx_line_chart_min_val;
    y_pos *= y_scale;
    y_pos = bottom_pos - GX_FIXED_VAL_TO_INT(y_pos);
    gx_context_line_color_set(pressure_chart_info.high_line_color);
    gx_context_fill_color_set(GX_COLOR_ID_BLACK);
    gx_canvas_line_draw(line_start, y_pos, line_end, y_pos);

    // label the high line:
    gx_context_font_get(pressure_chart_info.axis_label_font, &label_font);
    gx_context_font_set(pressure_chart_info.axis_label_font);
    y_pos -= label_font->gx_font_line_height + 4;
    gx_context_line_color_set(pressure_chart_info.axis_label_color);
    str.gx_string_ptr = "HIGH";
    str.gx_string_length = 4;
    gx_canvas_text_draw_ext(chart_bound.gx_rectangle_right, y_pos, &str);

    // draw the med line
    y_pos = pressure_chart_info.med_line_value - chart->gx_line_chart_info.gx_line_chart_min_val;
    y_pos *= y_scale;
    y_pos = bottom_pos - GX_FIXED_VAL_TO_INT(y_pos);
    gx_context_line_color_set(pressure_chart_info.med_line_color);
    gx_context_fill_color_set(GX_COLOR_ID_BLACK);
    brush->gx_brush_pattern_mask = 0x80000000;
    gx_canvas_line_draw(line_start, y_pos, line_end, y_pos);

    // label the MED line:
    y_pos -= label_font->gx_font_line_height + 4;
    gx_context_line_color_set(pressure_chart_info.axis_label_color);
    str.gx_string_ptr = "MED";
    str.gx_string_length = 3;
    gx_canvas_text_draw_ext(chart_bound.gx_rectangle_right, y_pos, &str);

    // draw the low line
    y_pos = pressure_chart_info.low_line_value - chart->gx_line_chart_info.gx_line_chart_min_val;
    y_pos *= y_scale;
    y_pos = bottom_pos - GX_FIXED_VAL_TO_INT(y_pos);
    gx_context_line_color_set(pressure_chart_info.low_line_color);
    gx_context_fill_color_set(GX_COLOR_ID_BLACK);
    brush->gx_brush_pattern_mask = 0x80000000;
    gx_canvas_line_draw(line_start, y_pos, line_end, y_pos);

    // label the LOW line:
    y_pos -= label_font->gx_font_line_height + 4;
    gx_context_line_color_set(pressure_chart_info.axis_label_color);
    str.gx_string_ptr = "LOW";
    str.gx_string_length = 3;
    gx_canvas_text_draw_ext(chart_bound.gx_rectangle_right, y_pos, &str);
}

///////////////////////////////////////////////////////////////////////////////
// test line chart draw- customize drawing to draw special features of Moog chart.
VOID test_line_chart_draw(GX_LINE_CHART *chart)
{
    gx_window_draw((GX_WINDOW *) chart);

    // draw the axis (done by base chart)
    gx_line_chart_axis_draw(chart);

    // draw the grid if it is enabled:

    if (pressure_chart_info.draw_grid)
    {
        test_line_chart_grid_draw(chart);
    }

    // draw the high, mid, low reference lines
    test_line_chart_reference_lines_draw(chart);

    // draw the scale along x axis
    test_line_chart_scale_draw(chart);

    // draw the chart data line (done by base chart)
    gx_line_chart_data_draw(chart);

    // draw any child widgets
    gx_widget_children_draw(chart);
}

///////////////////////////////////////////////////////////////////////////////
// Create test chart with params specific to the pressure chart.
// We might use different params for other chart types.
void create_test_pressure_chart(GX_WINDOW *parent)
{
    // Initialize params for the Moog specific line chart
    pressure_chart_info.high_line_color   =        GX_COLOR_ID_HIGH_COLOR;
    pressure_chart_info.med_line_color    =        GX_COLOR_ID_MED_COLOR;
    pressure_chart_info.low_line_color    =        GX_COLOR_ID_LOW_COLOR;
    pressure_chart_info.grid_color        =        GX_COLOR_ID_GRID_COLOR;
    pressure_chart_info.axis_label_color  =        GX_COLOR_ID_LABEL_TEXT;
    pressure_chart_info.axis_label_font   =        GX_FONT_ID_SMALL;
    pressure_chart_info.high_line_value = 20;
    pressure_chart_info.med_line_value  = 12;
    pressure_chart_info.low_line_value  =  5;
    pressure_chart_info.dash_line_pattern =        0xffffff00;
    pressure_chart_info.draw_grid         =        GX_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Generate psuedo-random data for the chart to draw
VOID generate_chart_data(VOID)
{
INT index;
INT base_val;

    // generate some random chart data:
    memset(chart_data, 0, MAX_DATA_COUNT * sizeof(INT));

    num_data_vals = 40 +  (rand() * 20 / RAND_MAX) + 1;
    if (num_data_vals > MAX_DATA_COUNT)
    {
        num_data_vals = MAX_DATA_COUNT;
    }

    base_val = DATA_MAX_VAL / 3;
    for (index = 0; index < num_data_vals; index++)
    {
        chart_data[index] = base_val + (rand() * base_val / RAND_MAX);
    }
    gx_line_chart_update((GX_LINE_CHART *) &pressure_trend_frame.pressure_trend_frame_line_chart, chart_data, num_data_vals);
}

///////////////////////////////////////////////////////////////////////////////
// Event processing for the background frame window
UINT trend_frame_event_process(GX_WINDOW *win, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        srand((unsigned) time(NULL));
        gx_window_event_process(win, event_ptr);
        create_test_pressure_chart(win);
        gx_system_timer_start(win, ID_CHART_TIMER, CHART_UPDATE_PERIOD, CHART_UPDATE_PERIOD);
        break;

    case GX_EVENT_TIMER:
        generate_chart_data();
        break;

    default:
        return gx_window_event_process(win, event_ptr);
    }
    return 0;
}

