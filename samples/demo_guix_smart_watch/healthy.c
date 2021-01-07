/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define CHART_TIMER_ID   2
#define RATE_TIMER_ID    3
#define CHART_SCROLL     4
#define CHART_Y_CENTER   25
#define EKG_COUNT        15
#define MIN_HEART_RATE   60
#define MAX_HEART_RATE   150

#define        EKG_MAP_HEIGHT    50
#define        EKG_MAP_WIDTH     255
GX_COLOR       scrachpad[EKG_MAP_WIDTH * EKG_MAP_HEIGHT];
GX_PIXELMAP    ekg_map;
GX_CANVAS      ekg_canvas;

GX_WIDGET *healthy_screen_list[] = {
    (GX_WIDGET *)&healthy_screen.healthy_screen_healthy_heart_rate,
    (GX_WIDGET *)&healthy_pace,
    GX_NULL
};

int ekg_values[EKG_COUNT] = {
    0, 0, 0, 0, 0, 5, 7, 4, 1, 0, -16, 25, 18, -6, -1
};

GX_RECTANGLE chart_area;
int line_y_coords[3];
int ekg_index = 0;

int heart_rate_step = 1;                        // simulation, HR value delta
int current_heart_rate = MIN_HEART_RATE;        // simulation current HR value
int retrace_xpos;

extern GX_ANIMATION slide_animation;
extern GX_WINDOW_ROOT  *root;

VOID rate_value_update();
VOID ekg_canvas_map_create();
VOID ekg_map_update();

/*************************************************************************************/
static VOID start_slide_animation(GX_WINDOW *window)
{
    GX_ANIMATION_INFO slide_animation_info;

    memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
    slide_animation_info.gx_animation_parent = (GX_WIDGET *)&healthy_screen;
    slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_WRAP | GX_ANIMATION_HORIZONTAL;
    slide_animation_info.gx_animation_frame_interval = 1;
    slide_animation_info.gx_animation_slide_screen_list = healthy_screen_list;
    slide_animation_info.gx_animation_id = ANIMATION_ID_SCREEN_LIST_SLIDE;

    gx_animation_drag_enable(&slide_animation, (GX_WIDGET *)window, &slide_animation_info);
}

/*************************************************************************************/
UINT healthy_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        start_slide_animation(window);

        gx_system_timer_start((GX_WIDGET *)window, RATE_TIMER_ID, GX_TICKS_SECOND * 3, GX_TICKS_SECOND * 3);

        line_y_coords[0] = line_y_coords[1] = line_y_coords[2] = CHART_Y_CENTER;
        ekg_index = 0;
        retrace_xpos = 0;
        chart_area = healthy_screen.healthy_screen_waveform_window.gx_window_client;

        gx_system_timer_start(window, CHART_TIMER_ID, 5, 5);

        ekg_canvas_map_create();
        return template_main_event_handler(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)window);
        gx_system_timer_stop((GX_WIDGET *)window, RATE_TIMER_ID);
        gx_system_timer_stop(window, CHART_TIMER_ID);

        gx_canvas_delete(&ekg_canvas);
        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == RATE_TIMER_ID)
        {
            rate_value_update();
        }
        else if (event_ptr->gx_event_payload.gx_event_timer_id == CHART_TIMER_ID)
        {
            ekg_map_update();
        }
        break;

    case GX_SIGNAL(ID_LEFT, GX_EVENT_CLICKED):
        screen_list_slide((GX_WIDGET **)healthy_screen_list, (GX_WIDGET *)&healthy_screen, SLIDE_LEFT);
        break;

    case GX_SIGNAL(ID_RIGHT, GX_EVENT_CLICKED):
        screen_list_slide((GX_WIDGET **)healthy_screen_list, (GX_WIDGET *)&healthy_screen, SLIDE_RIGHT);
        break;

    case GX_SIGNAL(ID_STOPWATCH, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&stopwatch_screen);
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        gx_widget_front_move(&healthy_screen.healthy_screen_left_arrow, GX_NULL);
        gx_widget_front_move(&healthy_screen.healthy_screen_right_arrow, GX_NULL);
        gx_widget_front_move(&healthy_screen.healthy_screen_stopwatch, GX_NULL);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID rate_value_update()
{
    GX_CHAR  string_buffer[5];
    GX_STRING string;

    string.gx_string_ptr = string_buffer;

    if (healthy_pace.gx_widget_status & GX_STATUS_VISIBLE)
    {
        gx_utility_ltoa(rand() % 10, string_buffer, 4);
        string_buffer[1] = '.';
        gx_utility_ltoa(rand() % 10, string_buffer + 2, 2);

        string.gx_string_length = 3;

        gx_prompt_text_set_ext(&healthy_pace.healthy_pace_pace, &string);
    }
    else if (healthy_screen.healthy_screen_healthy_heart_rate.gx_widget_status & GX_STATUS_VISIBLE)
    {
        current_heart_rate += heart_rate_step;

        gx_utility_ltoa(current_heart_rate, string_buffer, 5);
        string.gx_string_length = string_length_get(string_buffer, sizeof(string_buffer) - 1);
        gx_prompt_text_set_ext(&healthy_screen.healthy_screen_heart_rate, &string);

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
}

/*************************************************************************************/
VOID ekg_canvas_map_create()
{
    memset(&ekg_map, 0, sizeof(GX_PIXELMAP));

    ekg_map.gx_pixelmap_data = (GX_UBYTE *)scrachpad;
    ekg_map.gx_pixelmap_data_size = EKG_MAP_WIDTH * EKG_MAP_HEIGHT * sizeof(GX_COLOR);
    ekg_map.gx_pixelmap_height = EKG_MAP_HEIGHT;
    ekg_map.gx_pixelmap_width = EKG_MAP_WIDTH;
    ekg_map.gx_pixelmap_format = GX_COLOR_FORMAT_24XRGB;

    memset(&ekg_canvas, 0, sizeof(GX_CANVAS));

    /* Create a canvas for drawing ekg. */
    gx_canvas_create(&ekg_canvas, "ekg",
        root->gx_window_root_canvas->gx_canvas_display, GX_CANVAS_SIMPLE,
        EKG_MAP_WIDTH, EKG_MAP_HEIGHT, (GX_COLOR *)ekg_map.gx_pixelmap_data, ekg_map.gx_pixelmap_data_size);
}

/*************************************************************************************/
VOID ekg_map_update()
{
    GX_WIDGET *waveform;
    GX_PIXELMAP *wave_dot;
    GX_RECTANGLE block_rect;

    waveform = (GX_WIDGET *)&healthy_screen.healthy_screen_waveform_window;
    gx_widget_pixelmap_get(waveform, GX_PIXELMAP_ID_EKG_WAVE_DOT, &wave_dot);

    ekg_index++;

    if (ekg_index >= EKG_COUNT)
    {
        ekg_index = 0;
    }

    retrace_xpos += CHART_SCROLL;

    if (retrace_xpos > EKG_MAP_WIDTH - 1)
    {
        retrace_xpos = 0;
    }

    block_rect.gx_rectangle_top = 0;
    block_rect.gx_rectangle_bottom = EKG_MAP_HEIGHT - 1;
    block_rect.gx_rectangle_left = retrace_xpos;
    block_rect.gx_rectangle_right = retrace_xpos + (CHART_SCROLL * 5);

    if (block_rect.gx_rectangle_right > EKG_MAP_WIDTH - 1)
    {
        block_rect.gx_rectangle_right = EKG_MAP_WIDTH - 1;
    }

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(&ekg_canvas, GX_NULL, &block_rect);

    /* erase the rectangle ahead of the line */
    gx_context_brush_define(GX_COLOR_ID_GREEN, GX_COLOR_ID_BLACK, GX_BRUSH_SOLID_FILL | GX_BRUSH_ROUND | GX_BRUSH_ALIAS);
    gx_context_brush_width_set(0);
    gx_canvas_rectangle_draw(&block_rect);

    gx_context_brush_width_set(1);

    line_y_coords[1] = line_y_coords[0];
    line_y_coords[0] = CHART_Y_CENTER - ekg_values[ekg_index];

    gx_canvas_line_draw(retrace_xpos,
        line_y_coords[1],
        retrace_xpos + CHART_SCROLL,
        line_y_coords[0]);

    gx_canvas_pixelmap_draw(retrace_xpos + CHART_SCROLL,
        line_y_coords[0] - wave_dot->gx_pixelmap_height / 2, wave_dot);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(&ekg_canvas, GX_FALSE);


    gx_utility_rectangle_shift(&block_rect, waveform->gx_widget_size.gx_rectangle_left,
        waveform->gx_widget_size.gx_rectangle_top);

    gx_system_dirty_partial_add(waveform, &block_rect);
}

/*************************************************************************************/
VOID waveform_window_draw(GX_WINDOW *window)
{
    gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left, window->gx_widget_size.gx_rectangle_top, &ekg_map);
}