/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_medical_mouse_support_resources.h"
#include "guix_medical_mouse_support_specifications.h"
#include "demo_guix_medical_mouse_support.h"

#define CHART_TIMER                2
#define RATE_TIMER                 3
#define HR_WIN_ANIMATION_TIMER     4

#define CHART_Y_CENTER   60
#define CHART_TYPE_SCROLLING 0  /* chart continuously scrolls */
#define CHART_TYPE_RETRACE   1  /* chart traces left to right */

#define HR_PROMPT_VERTICAL_SHIFT 18
#define CHART_SCROLL     4
#define CHART_LINE_WIDTH 1
#define CHART_AMPLITUDE  1

int ekg_values[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 5, 4, 2, 0, 0, 0, 0, -3, 38, -7, 0, 0, 0, 0, 5, 8, 10, 8, 5
};

#define MIN_HEART_RATE  60
#define MAX_HEART_RATE  150

GX_RECTANGLE chart_area;
int line_y_coords[3];
int ekg_index = 0;

int current_heart_rate = MIN_HEART_RATE;        // simulation current HR value
int heart_rate_step = 1;                        // simulation, HR value delta
int chart_type = CHART_TYPE_RETRACE;            // toggle two chart types
GX_BOOL warn_heart = GX_FALSE;                  // simulation, high heart rate warning?

int retrace_xpos;

/*****************************************************************************/
void draw_scrolling_chart_lines(GX_CANVAS *canvas, GX_WINDOW *win)
{
GX_RECTANGLE chart_update_rectangle;
GX_DRAW_CONTEXT *context;
GX_PIXELMAP *map;

    chart_update_rectangle = chart_area;
    chart_update_rectangle.gx_rectangle_right = chart_update_rectangle.gx_rectangle_left + (CHART_SCROLL * 2) - 1;

    /* Initiate drawing on this canvas, clipped to just the leftmost two shift slots.  */
    gx_canvas_drawing_initiate(canvas, (GX_WIDGET *) win, &chart_update_rectangle);      
    gx_system_draw_context_get(&context);

    /* paint a slice of the background bitmap to erase the previous line */
    gx_context_brush_define(GX_COLOR_ID_CHART_LINE, GX_COLOR_ID_WINDOW_FILL, GX_BRUSH_SOLID_FILL|GX_BRUSH_ROUND|GX_BRUSH_ALIAS);
    gx_context_brush_width_set(0);

    /* draw a rectangle to erase the old lines: */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_CHART_ERASE, &map);
    gx_canvas_pixelmap_draw(chart_area.gx_rectangle_left, chart_area.gx_rectangle_top, map);

    /* draw the most recent two lines
       one line is the newly drawn value, and the second
       line is re-drawn so that it is not clipped
    */
    context ->gx_draw_context_brush.gx_brush_width = CHART_LINE_WIDTH;
    context ->gx_draw_context_brush.gx_brush_style = GX_BRUSH_ROUND|GX_BRUSH_ALIAS;

    gx_canvas_line_draw(chart_update_rectangle.gx_rectangle_left + 1,
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
INT ekg_count;
INT ekg_value;
INT slider_value;
GX_ICON *heart_icon;
    
    /* pick up the canvas pointer */
    gx_widget_canvas_get(win, &canvas);

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(canvas, win, &chart_area);        

    /* shift down are previous values */
    line_y_coords[2] = line_y_coords[1];
    line_y_coords[1] = line_y_coords[0];

    ekg_index++;
    ekg_count = sizeof(ekg_values) / sizeof(int);
    if (ekg_index >= ekg_count)
    {
        ekg_index = 0;
    }

    ekg_value = ekg_values[ekg_index];
    line_y_coords[0] = CHART_Y_CENTER - ekg_value;  

    // the slider scale is 0 to 100, so convert ekg value to 
    // approximate slider value:

    slider_value = ekg_value + 45;          
    gx_slider_value_set((GX_SLIDER *) &vitals_screen.vitals_screen_cardio_slider,
        &vitals_screen.vitals_screen_cardio_slider.gx_slider_info, slider_value);

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

    heart_icon = &vitals_screen.vitals_screen_heart_icon;
    
    if (ekg_value == 0)
    {
        if (warn_heart)
        {
            if (heart_icon->gx_icon_normal_pixelmap != GX_PIXELMAP_ID_ICON_HEART_RED_SMALL)
            {
                gx_icon_pixelmap_set(heart_icon, GX_PIXELMAP_ID_ICON_HEART_RED_SMALL, GX_PIXELMAP_ID_ICON_HEART_RED_SMALL);
            }
        }
        else
        {
            if (heart_icon->gx_icon_normal_pixelmap != GX_PIXELMAP_ID_ICON_HEART_GRN_SMALL)
            {
                gx_icon_pixelmap_set(heart_icon, GX_PIXELMAP_ID_ICON_HEART_GRN_SMALL, GX_PIXELMAP_ID_ICON_HEART_GRN_SMALL);
            }
        }
    }
    else
    {
        if (ekg_index >= 19 && ekg_index <= 22)
        {
            if (warn_heart)
            {
                if (heart_icon->gx_icon_normal_pixelmap != GX_PIXELMAP_ID_ICON_HEART_RED_LARGE)
                {
                    gx_icon_pixelmap_set(heart_icon, GX_PIXELMAP_ID_ICON_HEART_RED_LARGE, GX_PIXELMAP_ID_ICON_HEART_RED_LARGE);
                }
            }
            else
            {
                if (heart_icon->gx_icon_normal_pixelmap != GX_PIXELMAP_ID_ICON_HEART_GRN_LARGE)
                {
                    gx_icon_pixelmap_set(heart_icon, GX_PIXELMAP_ID_ICON_HEART_GRN_LARGE, GX_PIXELMAP_ID_ICON_HEART_GRN_LARGE);
                }
            }
        }
        else
        {
            if (warn_heart)
            {
                if (heart_icon->gx_icon_normal_pixelmap != GX_PIXELMAP_ID_ICON_HEART_RED_MED)
                {
                    gx_icon_pixelmap_set(heart_icon, GX_PIXELMAP_ID_ICON_HEART_RED_MED, GX_PIXELMAP_ID_ICON_HEART_RED_MED);
                }
            }
            else
            {
                if (heart_icon->gx_icon_normal_pixelmap != GX_PIXELMAP_ID_ICON_HEART_GRN_MED)
                {
                    gx_icon_pixelmap_set(heart_icon, GX_PIXELMAP_ID_ICON_HEART_GRN_MED, GX_PIXELMAP_ID_ICON_HEART_GRN_MED);
                }
            }
        }
     }
 }

/*****************************************************************************/
void update_retrace_chart(GX_WINDOW *win)
{
GX_CANVAS *canvas;
GX_PIXELMAP *map;
GX_PIXELMAP *ball;
GX_RECTANGLE block_rect; 

    ekg_index++;

    if (ekg_index >= sizeof(ekg_values) / sizeof(int))
    {
        ekg_index = 0;
    }

    retrace_xpos += CHART_SCROLL;

    if (retrace_xpos + CHART_SCROLL > chart_area.gx_rectangle_right)
    {
        retrace_xpos = 0;
    }

    block_rect.gx_rectangle_top = chart_area.gx_rectangle_top;
    block_rect.gx_rectangle_bottom = chart_area.gx_rectangle_bottom;
    block_rect.gx_rectangle_left = chart_area.gx_rectangle_left + retrace_xpos;
    block_rect.gx_rectangle_right = chart_area.gx_rectangle_left + retrace_xpos + (CHART_SCROLL * 8);

    /* pick up the canvas pointer */
    gx_widget_canvas_get(win, &canvas);

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(canvas, win, &chart_area);        
    gx_context_pixelmap_get(GX_PIXELMAP_ID_CARDIO_DOT, &ball);

    /* erase the rectangle ahead of the line */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_CHART_ERASE, &map);
    gx_context_brush_define(GX_COLOR_ID_NEON, GX_COLOR_ID_NEON, GX_BRUSH_SOLID_FILL|GX_BRUSH_ROUND|GX_BRUSH_ALIAS);
    gx_context_brush_width_set(0);
    gx_canvas_pixelmap_tile(&block_rect, map);

    gx_context_brush_width_set(1);

    line_y_coords[1] = line_y_coords[0];
    line_y_coords[0] = CHART_Y_CENTER - ekg_values[ekg_index];

    gx_canvas_line_draw(chart_area.gx_rectangle_left + retrace_xpos,
        chart_area.gx_rectangle_top + line_y_coords[1],
        chart_area.gx_rectangle_left + retrace_xpos + CHART_SCROLL,
        chart_area.gx_rectangle_top + line_y_coords[0]);

    gx_canvas_pixelmap_draw(chart_area.gx_rectangle_left + retrace_xpos + CHART_SCROLL,
        chart_area.gx_rectangle_top + line_y_coords[0] - ball->gx_pixelmap_height / 2, ball);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(canvas, GX_TRUE);
}

/*****************************************************************************/
VOID ToggleChartType(VOID)
{
    if (chart_type == CHART_TYPE_SCROLLING)
    {
        chart_type = CHART_TYPE_RETRACE;
        gx_widget_shift(&vitals_screen.vitals_screen_current_hr_prompt, 0, -HR_PROMPT_VERTICAL_SHIFT, GX_TRUE);
        chart_area.gx_rectangle_left = vitals_screen.vitals_screen_waveform_window.gx_window_client.gx_rectangle_left;
        gx_widget_hide(&vitals_screen.vitals_screen_cardio_slider);
        gx_widget_hide(&vitals_screen.vitals_screen_heart_icon);
    }
    else
    {
        chart_type = CHART_TYPE_SCROLLING;
        chart_area.gx_rectangle_left = vitals_screen.vitals_screen_cardio_slider.gx_widget_size.gx_rectangle_right + 1;
        gx_widget_shift(&vitals_screen.vitals_screen_current_hr_prompt, 0, HR_PROMPT_VERTICAL_SHIFT, GX_TRUE);
        gx_widget_show(&vitals_screen.vitals_screen_cardio_slider);
        gx_widget_show(&vitals_screen.vitals_screen_heart_icon);
    }
    gx_system_dirty_mark(&vitals_screen.vitals_screen_waveform_window);
}

/*****************************************************************************/
VOID update_heart_rate()
{
static GX_CHAR heart_rate_string[5];

    current_heart_rate += heart_rate_step;

    gx_utility_ltoa(current_heart_rate, heart_rate_string, 5);
    gx_prompt_text_set(&vitals_screen.vitals_screen_current_hr_prompt, heart_rate_string);

    if (current_heart_rate >= 130)
    {
        warn_heart = GX_TRUE;
        gx_prompt_text_color_set(&vitals_screen.vitals_screen_current_hr_prompt, GX_COLOR_ID_WARN_YELLOW, GX_COLOR_ID_WARN_YELLOW, GX_COLOR_ID_WARN_YELLOW);
    }
    else
    {
        warn_heart = GX_FALSE;
        gx_prompt_text_color_set(&vitals_screen.vitals_screen_current_hr_prompt, GX_COLOR_ID_NEON, GX_COLOR_ID_NEON, GX_COLOR_ID_NEON);
    }

    if (heart_rate_step > 0)
    {
        if (current_heart_rate >= MAX_HEART_RATE)
        {
            heart_rate_step = -1;
        }
    }
    else
    {
        if (current_heart_rate <= MIN_HEART_RATE)
        {
            heart_rate_step = 1;
        }
    }
}

/*****************************************************************************/
VOID AnimateHRWin(VOID)
{
    INT stop;
    INT left;
    INT distance;

    left = vitals_screen.vitals_screen_hr_win.gx_widget_size.gx_rectangle_left;
    stop = vitals_screen.vitals_screen_waveform_window.gx_widget_size.gx_rectangle_right + 5;

    distance = (left - stop) / 3;
    gx_widget_shift(&vitals_screen.vitals_screen_hr_win, -distance, 0, GX_TRUE);

    if (distance <= 1)
    {
        gx_system_timer_stop(&vitals_screen, HR_WIN_ANIMATION_TIMER);
    }
}


/*****************************************************************************/
UINT vitals_screen_event_process(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = GX_SUCCESS;
GX_WIDGET *widget = (GX_WIDGET *) window;

    switch(myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        heart_rate_step = 1;
        line_y_coords[0] = line_y_coords[1] = line_y_coords[2] = 50;
        ekg_index = 0;
        chart_area = vitals_screen.vitals_screen_waveform_window.gx_window_client;
        med_screen_base_event_handler(window, myevent);
        gx_prompt_text_set(&vitals_screen.vitals_screen_patient_name, GetPatientName());
        gx_prompt_text_set(&vitals_screen.vitals_screen_admit_date, GetPatientAdmitDate());

        // Shift the HR window off the right edge of screen
        gx_widget_shift(&vitals_screen.vitals_screen_hr_win,
            vitals_screen.base.gx_widget_size.gx_rectangle_right - vitals_screen.vitals_screen_hr_win.gx_widget_size.gx_rectangle_left, 0, GX_TRUE);

        retrace_xpos = chart_area.gx_rectangle_left;

        if (chart_type == CHART_TYPE_RETRACE)
        {
            gx_widget_hide(&vitals_screen.vitals_screen_heart_icon);
            gx_widget_hide(&vitals_screen.vitals_screen_cardio_slider);
        }
        else
        {
            chart_area.gx_rectangle_left = vitals_screen.vitals_screen_cardio_slider.gx_widget_size.gx_rectangle_right + 1;
        }
        gx_system_timer_start(widget, CHART_TIMER, 2, 2);
        gx_system_timer_start(widget, RATE_TIMER, GX_TICKS_SECOND * 2, GX_TICKS_SECOND * 2);
        gx_system_timer_start(widget, HR_WIN_ANIMATION_TIMER, 1, 1);
        break;

    case GX_EVENT_HIDE:
        gx_system_timer_stop(widget, 0);
        gx_window_event_process(window, myevent);
        break;

    case GX_EVENT_PEN_DOWN:
        if (gx_utility_rectangle_point_detect(&vitals_screen.vitals_screen_hr_win.gx_widget_size, myevent->gx_event_payload.gx_event_pointdata) ||
            gx_utility_rectangle_point_detect(&vitals_screen.vitals_screen_waveform_window.gx_widget_size, myevent->gx_event_payload.gx_event_pointdata))
        {
            ToggleChartType();
        }
        break;

    case GX_EVENT_TIMER:
        switch(myevent ->gx_event_payload.gx_event_timer_id)
        {
        case CHART_TIMER:
            if (chart_type == CHART_TYPE_SCROLLING)
            {
                update_scrolling_chart(window);
            }
            else
            {
                update_retrace_chart(window);
            }
            break;

        case RATE_TIMER:
            update_heart_rate();
            break;

        case HR_WIN_ANIMATION_TIMER:
            AnimateHRWin();
            break;
        }
        break;

    default:
        status = med_screen_base_event_handler(window, myevent);
    }
    return status;
}


/*****************************************************************************/
void waveform_draw(GX_WINDOW *window)
{
    gx_window_draw(window);
}


