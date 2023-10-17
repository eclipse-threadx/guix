/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "medical_resources.h"
#include "medical_specifications.h"

extern char current_patient_name[128];
void ToggleScreen(GX_WIDGET *new_win, GX_WIDGET *old_win);
extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

#define CHART_TIMER      2
#define RATE_TIMER       3
#define CHART_Y_CENTER   44
#define CHART_TYPE_SCROLLING 0  /* chart continuously scrolls */
#define CHART_TYPE_RETRACE   1  /* chart traces left to right */

#define CHART_SCROLL     5
#define CHART_LINE_WIDTH 1
#define CHART_AMPLITUDE  1
#define EKG_COUNT        15

int ekg_values[EKG_COUNT] = {
    0, 0, 0, 0, 0, 5, 7, 4, 1, 0, -16, 36, 18, -6, -1
};

int heart_rates[] = {
    57, 61, 63, 65, 69, 72, 77, 74, 71, 79, 83, 88, 91, 86, 92, 93, 94, 91, 94
};

GX_RECTANGLE chart_area;
int line_y_coords[3];
int ekg_index = 0;

int heart_rate_index = 0;
int heart_rate_step;
int chart_type = CHART_TYPE_RETRACE;

int x_pos;

/*****************************************************************************/
void draw_scrolling_chart_lines(GX_CANVAS *canvas, GX_WINDOW *win)
{
GX_RECTANGLE chart_update_rectangle;
GX_DRAW_CONTEXT *context;
 /*GX_PIXELMAP *map;*/

    chart_update_rectangle = chart_area;
    chart_update_rectangle.gx_rectangle_right = chart_update_rectangle.gx_rectangle_left + (CHART_SCROLL * 2) - 1;

    /* Initiate drawing on this canvas, clipped to just the leftmost two shift slots.  */
    gx_canvas_drawing_initiate(canvas, (GX_WIDGET *) win, &chart_update_rectangle);      
    gx_system_draw_context_get(&context);

    /* paint a slice of the background bitmap to erase the previous line */
    gx_context_brush_define(GX_COLOR_ID_CHART_LINE, GX_COLOR_ID_WINDOW_FILL, GX_BRUSH_SOLID_FILL|GX_BRUSH_ROUND|GX_BRUSH_ALIAS);
    gx_context_brush_width_set(0);

    /* draw a rectangle to erase the old lines: */
    gx_canvas_rectangle_draw(&chart_update_rectangle);

    /* draw the most recent two lines
       one line is the newly drawn value, and the second
       line is re-drawn so that it is not clipped
    */
    context ->gx_draw_context_brush.gx_brush_width = CHART_LINE_WIDTH;
    /* context ->gx_draw_context_brush.gx_brush_style = GX_BRUSH_ROUND|GX_BRUSH_ALIAS; */

    gx_canvas_line_draw(chart_update_rectangle.gx_rectangle_left,
        chart_update_rectangle.gx_rectangle_top + line_y_coords[0],
        chart_update_rectangle.gx_rectangle_left + CHART_SCROLL,
        chart_update_rectangle.gx_rectangle_top + line_y_coords[1]);

    gx_canvas_line_draw(chart_update_rectangle.gx_rectangle_left + CHART_SCROLL,
        chart_update_rectangle.gx_rectangle_top + line_y_coords[1],
        chart_update_rectangle.gx_rectangle_left + (CHART_SCROLL * 2),
        chart_update_rectangle.gx_rectangle_top + line_y_coords[2]);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(canvas, GX_FALSE);
}


/*****************************************************************************/
VOID update_scrolling_chart(GX_WINDOW *win)
{
GX_CANVAS *canvas;
GX_RECTANGLE block_rect; 
    
    /* pick up the canvas pointer */
    gx_widget_canvas_get(win, &canvas);

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(canvas, win, &chart_area);        

    /* shift down are previous values */
    line_y_coords[2] = line_y_coords[1];
    line_y_coords[1] = line_y_coords[0];

    ekg_index++;

    if (ekg_index >= EKG_COUNT)
    {
        ekg_index = 0;
    }

    line_y_coords[0] = CHART_Y_CENTER - ekg_values[ekg_index];       
    gx_slider_value_set((GX_SLIDER *)&vitals_screen.vitals_screen_chart_pen,
        &vitals_screen.vitals_screen_chart_pen.gx_slider_info, ekg_values[ekg_index]);

    /* shift the existing data to the right
       skip the first segment on the left, because it has
       been clipped at the chart left edge. So start one
       segment in, and shift all remaining segments to the right
    */
    block_rect = chart_area;
    block_rect.gx_rectangle_left += CHART_SCROLL;

    gx_canvas_block_move(&block_rect, CHART_SCROLL, 0, GX_NULL);

    /* draw the two previous lines
       the second value will not be unclipped,
       and the first value is our newest line
    */
    draw_scrolling_chart_lines(canvas, win);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(canvas, GX_TRUE);

    if (ekg_index == 0)
    {
        gx_widget_hide(&vitals_screen.vitals_screen_heart_icon);
    }
    else
    {
        if (ekg_index == EKG_COUNT - 2)
        {
            gx_widget_show(&vitals_screen.vitals_screen_heart_icon);
        }
    }
 }

/*****************************************************************************/
void update_retrace_chart(GX_WINDOW *win)
{
GX_CANVAS *canvas;
GX_PIXELMAP *cardio_dot;
GX_RECTANGLE block_rect; 

    gx_widget_pixelmap_get(win, GX_PIXELMAP_ID_SMALL_BALL, &cardio_dot);
    
    ekg_index++;

    if (ekg_index >= EKG_COUNT)
    {
        ekg_index = 0;
    }

    x_pos += CHART_SCROLL;

    if (x_pos + CHART_SCROLL > chart_area.gx_rectangle_right)
    {
        x_pos = 0;
    }

    block_rect.gx_rectangle_top = chart_area.gx_rectangle_top;
    block_rect.gx_rectangle_bottom = chart_area.gx_rectangle_bottom;
    block_rect.gx_rectangle_left = chart_area.gx_rectangle_left + x_pos;
    block_rect.gx_rectangle_right = chart_area.gx_rectangle_left + x_pos + (CHART_SCROLL * 5);

    /* pick up the canvas pointer */
    gx_widget_canvas_get(win, &canvas);

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(canvas, win, &chart_area);        

    /* erase the rectangle ahead of the line */
    gx_context_brush_define(GX_COLOR_ID_CHART_LINE, GX_COLOR_ID_WINDOW_FILL, GX_BRUSH_SOLID_FILL|GX_BRUSH_ROUND|GX_BRUSH_ALIAS);
    gx_context_brush_width_set(0);
    gx_canvas_rectangle_draw(&block_rect);

    gx_context_brush_width_set(1);

    line_y_coords[1] = line_y_coords[0];
    line_y_coords[0] = CHART_Y_CENTER - ekg_values[ekg_index];

    gx_canvas_line_draw(chart_area.gx_rectangle_left + x_pos,
        chart_area.gx_rectangle_top + line_y_coords[1],
        chart_area.gx_rectangle_left + x_pos + CHART_SCROLL,
        chart_area.gx_rectangle_top + line_y_coords[0]);

    gx_canvas_pixelmap_draw(chart_area.gx_rectangle_left + x_pos + CHART_SCROLL,
        chart_area.gx_rectangle_top + line_y_coords[0] - cardio_dot->gx_pixelmap_height / 2, cardio_dot);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(canvas, GX_TRUE);
}

/*****************************************************************************/
VOID update_heart_rate(GX_WINDOW *win)
{
INT heart_rate;
static GX_CHAR heart_rate_string[5];
GX_STRING string;

    heart_rate = heart_rates[heart_rate_index];
    gx_utility_ltoa(heart_rate, heart_rate_string, 5);
    string.gx_string_ptr = heart_rate_string;
    string.gx_string_length = string_length_get(heart_rate_string, sizeof(heart_rate_string) - 1);
    gx_prompt_text_set_ext(&vitals_screen.vitals_screen_current_rate, &string);

    if (heart_rate >= 90)
    {
        gx_prompt_text_color_set(&vitals_screen.vitals_screen_current_rate, GX_COLOR_ID_YELLOW, GX_COLOR_ID_YELLOW, GX_COLOR_ID_YELLOW);
    }
    else
    {
        gx_prompt_text_color_set(&vitals_screen.vitals_screen_current_rate, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN);
    }
    heart_rate_index += heart_rate_step;

    if (heart_rate_step > 0)
    {
        if ((UINT)heart_rate_index >= sizeof(heart_rates) / sizeof(int))
        {
            heart_rate_step = -1;
            heart_rate_index--;
        }
    }
    else
    {
        if (heart_rate_step < 0)
        {
            if (heart_rate_index < 0)
            {
                heart_rate_index = 0;
                heart_rate_step = 1;
            }
        }
    }
}

/*****************************************************************************/
UINT chart_window_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = GX_SUCCESS;
GX_WIDGET *widget = (GX_WIDGET *) window;

    switch(myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        heart_rate_step = 1;
        heart_rate_index = 0;
        line_y_coords[0] = line_y_coords[1] = line_y_coords[2] = 50;
        ekg_index = 0;
        chart_area.gx_rectangle_top = window->gx_widget_size.gx_rectangle_top;
        chart_area.gx_rectangle_bottom = window->gx_widget_size.gx_rectangle_bottom;
        chart_area.gx_rectangle_right = vitals_screen.vitals_screen_current_rate.gx_widget_size.gx_rectangle_left - 10;

        gx_window_event_process(window, myevent);

        if (chart_type == CHART_TYPE_SCROLLING)
        {
            chart_area.gx_rectangle_left = vitals_screen.vitals_screen_chart_pen.gx_widget_size.gx_rectangle_right + 1;
        }
        else
        {
            chart_area.gx_rectangle_left = vitals_screen.gx_widget_size.gx_rectangle_left + 4;
            gx_widget_hide(&vitals_screen.vitals_screen_chart_pen);
        }

        x_pos = chart_area.gx_rectangle_left;

        gx_widget_hide(&vitals_screen.vitals_screen_heart_icon);
        gx_system_timer_start(widget, CHART_TIMER, 80 / GX_SYSTEM_TIMER_MS, 80 / GX_SYSTEM_TIMER_MS);
        gx_system_timer_start(widget, RATE_TIMER, GX_TICKS_SECOND * 3, GX_TICKS_SECOND * 3);
        break;

    case GX_EVENT_HIDE:
        gx_system_timer_stop(widget, CHART_TIMER);
        gx_window_event_process(window, myevent);
        break;

    case GX_EVENT_PEN_DOWN:
        if (chart_type == CHART_TYPE_SCROLLING)
        {
            chart_type = CHART_TYPE_RETRACE;
            chart_area.gx_rectangle_left = window->gx_widget_size.gx_rectangle_left + 4;
            gx_widget_hide(&vitals_screen.vitals_screen_chart_pen);
        }
        else
        {
            chart_type = CHART_TYPE_SCROLLING;
            gx_widget_show(&vitals_screen.vitals_screen_chart_pen);
            chart_area.gx_rectangle_left = vitals_screen.vitals_screen_chart_pen.gx_widget_size.gx_rectangle_right + 1;
        }
        x_pos = 0;
        gx_system_dirty_mark(window);
        break;
         

    case GX_EVENT_TIMER:
        if (myevent ->gx_event_payload.gx_event_timer_id == CHART_TIMER)
        {
            if (chart_type == CHART_TYPE_SCROLLING)
            {
                update_scrolling_chart(window);
            }
            else
            {
                update_retrace_chart(window);
            }
        }
        else
        {
            update_heart_rate(window);
        }
        break;

    default:
        status = gx_window_event_process(window, myevent);
    }
    return status;
}

/*****************************************************************************/
void chart_window_draw(GX_WINDOW *window)
{
    gx_window_draw(window);
}

/*****************************************************************************/
UINT vitals_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
GX_BUTTON        *vitals_button;
GX_WIDGET       *widget = (GX_WIDGET *) window;

    switch(myevent ->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, myevent);
        gx_widget_find(widget, ID_VITALS_BUTTON, 0, (GX_WIDGET **) &vitals_button);
        vitals_button->gx_button_select_handler((GX_WIDGET *) vitals_button);
        break;

    case GX_SIGNAL(ID_MEDS_BUTTON, GX_EVENT_RADIO_SELECT):
        ToggleScreen((GX_WIDGET *) &meds_screen, widget);
        break;

    case GX_SIGNAL(ID_PATIENTS_BUTTON, GX_EVENT_RADIO_SELECT):
        ToggleScreen((GX_WIDGET *) &patients_screen, widget);
        break;

    default:
        return gx_window_event_process(window, myevent);
    }
    return 0;
}
