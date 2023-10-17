/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_synergy_24xrgb_rotated_ccw_binary_resources.h"
#include "all_widgets_synergy_24xrgb_rotated_ccw_binary_specifications.h"

GX_WINDOW     *pLineWin = &lines_screen.lines_screen_line_window;
int            lines_screen_line_angle = 0;
int            lines_screen_line_width = 1;
GX_BOOL        lines_screen_anti_aliased = GX_FALSE;
GX_BOOL        lines_screen_rounded = GX_FALSE;
GX_RESOURCE_ID lines_screen_line_color = GX_COLOR_ID_BLACK;
GX_UBYTE       lines_screen_brush_alpha = 255;
int            line_x1;
int            line_x2;
int            line_y1;
int            line_y2;
int            lines_screen_line_length = 100;

/* Define prototypes.   */
VOID  PopulateDropList(void);

VOID calculate_line_ends(VOID)
{
    INT scaled_angle;
    INT x_dist;
    INT y_dist;

    INT x_center = (pLineWin->gx_widget_size.gx_rectangle_left + pLineWin->gx_widget_size.gx_rectangle_right) / 2;
    INT y_center = (pLineWin->gx_widget_size.gx_rectangle_top + pLineWin->gx_widget_size.gx_rectangle_bottom) / 2;

    scaled_angle = GX_FIXED_VAL_MAKE(lines_screen_line_angle);
    x_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * lines_screen_line_length);
    y_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * lines_screen_line_length);

    line_x1 = x_center - x_dist;
    line_x2 = x_center + x_dist;
    line_y1 = y_center - y_dist;
    line_y2 = y_center + y_dist;
}

VOID update_prompt(GX_RESOURCE_ID id, INT value)
{
GX_PROMPT *pp;
static GX_CHAR val[10];

    gx_widget_find((GX_WIDGET *)&lines_screen, (USHORT)id, 0, &pp);
    
    if (pp)
    {
        gx_utility_ltoa(value, val, 10);
        gx_prompt_text_set(pp, val);
    }
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_BRUSH_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        lines_screen_brush_alpha = (GX_UBYTE)myevent->gx_event_payload.gx_event_longdata;
        update_prompt(ID_BRUSH_ALPHA_PROMPT, lines_screen_brush_alpha);
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_LINE_WIDTH, GX_EVENT_SLIDER_VALUE):
        lines_screen_line_width = myevent ->gx_event_payload.gx_event_longdata;
        update_prompt(ID_WIDTH_DISPLAY, lines_screen_line_width);
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_LINE_ANGLE, GX_EVENT_SLIDER_VALUE):
        lines_screen_line_angle = myevent ->gx_event_payload.gx_event_longdata;
        update_prompt(ID_ANGLE_DISPLAY, lines_screen_line_angle);
        calculate_line_ends();
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_ON):
        lines_screen_anti_aliased = GX_TRUE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_OFF):
        lines_screen_anti_aliased = GX_FALSE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ROUNDED, GX_EVENT_TOGGLE_ON):
        lines_screen_rounded = GX_TRUE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ROUNDED, GX_EVENT_TOGGLE_OFF):
        lines_screen_rounded = GX_FALSE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_COLOR_BLACK, GX_EVENT_RADIO_SELECT):
        lines_screen_line_color = GX_COLOR_ID_BLACK;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_COLOR_WHITE, GX_EVENT_RADIO_SELECT):
        lines_screen_line_color = GX_COLOR_ID_WHITE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_COLOR_ORANGE, GX_EVENT_RADIO_SELECT):
        lines_screen_line_color = GX_COLOR_ID_ORANGE;
        gx_system_dirty_mark(pLineWin);
        break;

    default:
        status = next_button_handler(window, myevent);
        break;
    }
    return status;
}

VOID line_win_draw(GX_WINDOW *window)
{
ULONG brush_style = 0;
GX_BRUSH *brush;
GX_RECTANGLE *size;

    gx_window_draw((GX_WINDOW*) window);

    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = lines_screen_brush_alpha;

    if (lines_screen_anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }
    if (lines_screen_rounded)
    {
        brush_style |= GX_BRUSH_ROUND;
    }
    gx_context_brush_define(lines_screen_line_color, window ->gx_widget_normal_fill_color, brush_style);
    gx_context_brush_width_set(lines_screen_line_width);

    /* Draw pattern lines. */
    size = &window->gx_widget_size;

    gx_context_brush_pattern_set(0xf8fcfeff);
    gx_canvas_line_draw(size->gx_rectangle_left + 5, size->gx_rectangle_top + 5, size->gx_rectangle_left + 5, size->gx_rectangle_bottom - 5);
    gx_context_brush_pattern_set(0xf0f0f0f0);
    gx_canvas_line_draw(size->gx_rectangle_left + 5, size->gx_rectangle_top + 5, size->gx_rectangle_right - 5, size->gx_rectangle_top + 5);
    gx_context_brush_pattern_set(0xaaaaaaaa);
    gx_canvas_line_draw(size->gx_rectangle_left + 5, size->gx_rectangle_bottom - 5, size->gx_rectangle_right - 5, size->gx_rectangle_bottom - 5);
    gx_context_brush_pattern_set(0x33333333);
    gx_canvas_line_draw(size->gx_rectangle_right - 5, size->gx_rectangle_top + 5, size->gx_rectangle_right - 5, size->gx_rectangle_bottom - 5);

    gx_context_brush_pattern_set(0);
    calculate_line_ends();
    gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
}
