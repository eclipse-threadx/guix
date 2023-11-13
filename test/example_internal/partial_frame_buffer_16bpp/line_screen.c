/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "partial_frame_buffer_16bpp_resources.h"
#include "partial_frame_buffer_16bpp_specifications.h"


#define LINE_LENGTH 100
#define LINE_CENTER_X_OFFSET 159
#define LINE_CENTER_Y_OFFSET 159

GX_WINDOW *pLineWin = &line_screen.line_screen_line_window;
int        line_angle = 0;
int        line_width = 1;
GX_BOOL    rounded = GX_FALSE;
GX_BOOL    anti_alised_line = GX_FALSE;
int        line_x1;
int        line_x2;
int        line_y1;
int        line_y2;
INT        line_pattern;


VOID calculate_line_ends(VOID)
{
    INT scaled_angle;
    INT x_dist;
    INT y_dist;

    INT x_center = pLineWin->gx_widget_size.gx_rectangle_left + LINE_CENTER_X_OFFSET;
    INT y_center = pLineWin->gx_widget_size.gx_rectangle_top + LINE_CENTER_Y_OFFSET;

    scaled_angle = GX_FIXED_VAL_MAKE(line_angle);
    x_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_LENGTH);
    y_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * LINE_LENGTH);

    line_x1 = x_center - x_dist;
    line_x2 = x_center + x_dist;
    line_y1 = y_center - y_dist;
    line_y2 = y_center + y_dist;
}

UINT line_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_LINE_WIDTH, GX_EVENT_SLIDER_VALUE):
        line_width = myevent->gx_event_payload.gx_event_longdata;
        gx_numeric_prompt_value_set(&line_screen.line_screen_line_width, line_width);
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_LINE_ANGLE, GX_EVENT_SLIDER_VALUE):
        line_angle = myevent->gx_event_payload.gx_event_longdata;
        gx_numeric_prompt_value_set(&line_screen.line_screen_line_angle, line_angle);
        calculate_line_ends();
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ROUNDED, GX_EVENT_TOGGLE_ON):
        rounded = GX_TRUE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ROUNDED, GX_EVENT_TOGGLE_OFF):
        rounded = GX_FALSE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_PATTERN, GX_EVENT_TOGGLE_ON):
        line_pattern = 0x0f0f0f0f;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_PATTERN, GX_EVENT_TOGGLE_OFF):
        line_pattern = 0;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED_LINE, GX_EVENT_TOGGLE_ON):
        anti_alised_line = GX_TRUE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED_LINE, GX_EVENT_TOGGLE_OFF):
        anti_alised_line = GX_FALSE;
        gx_system_dirty_mark(pLineWin);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}



VOID line_win_draw(GX_WINDOW *window)
{
    ULONG brush_style = 0;
    GX_BRUSH *brush = GX_NULL;

    gx_window_draw((GX_WINDOW*)window);


    if (anti_alised_line)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    if (rounded)
    {
        brush_style |= GX_BRUSH_ROUND;
        gx_context_brush_define(GX_COLOR_ID_WHITE, window->gx_widget_normal_fill_color, brush_style);
    }
    else
    {
        /* Use different function to set brush color. But color value is same. */
        gx_context_raw_brush_define(0xffffff, 0x9696ff, brush_style);
    }

    gx_context_brush_width_set(line_width);
    gx_context_brush_get(&brush);
    if (brush)
    {
        brush->gx_brush_line_pattern = line_pattern;
        gx_context_brush_set(brush);
    }
    calculate_line_ends();
    gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
}
