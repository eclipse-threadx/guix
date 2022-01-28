#include "demo_guix_smart_watch.h"

#define CHART_SCROLL     4
#define CHART_Y_CENTER   30
#define EKG_COUNT        15
#define EKG_MAP_HEIGHT   50
#define EKG_MAP_WIDTH    255

#define ANIMATION_TOTAL_STEPS 20
#define CAL_ANIMATION_VAL(val) ((val) * animation_step / ANIMATION_TOTAL_STEPS)

extern GX_WINDOW_ROOT* root;

/* Define local variables.  */
static GX_PIXELMAP ekg_map;
static GX_CANVAS   ekg_canvas;
static GX_COLOR    ekg_map_data_buffer[EKG_MAP_WIDTH * EKG_MAP_HEIGHT];

static INT ekg_line_start_y;
static INT ekg_line_end_y;
static INT ekg_index;
static INT ekg_values[EKG_COUNT] = { 0, 0, 0, 0, 0, 5, 7, 4, 1, 0, -16, 25, 18, -6, -1 };

static INT animation_step = 0;

/******************************************************************************************/
/* Initialize fitness screen.                                                             */
/******************************************************************************************/
VOID fitness_screen_initialize()
{
    memset(&ekg_map, 0, sizeof(GX_PIXELMAP));

    ekg_map.gx_pixelmap_data = (GX_UBYTE*)ekg_map_data_buffer;
    ekg_map.gx_pixelmap_data_size = sizeof(ekg_map_data_buffer);
    ekg_map.gx_pixelmap_height = EKG_MAP_HEIGHT;
    ekg_map.gx_pixelmap_width = EKG_MAP_WIDTH;
    ekg_map.gx_pixelmap_format = GX_COLOR_FORMAT_24XRGB;

    memset(&ekg_canvas, 0, sizeof(GX_CANVAS));

    /* Create a canvas for drawing ekg. */
    gx_canvas_create(&ekg_canvas, "ekg",
        root->gx_window_root_canvas->gx_canvas_display, GX_CANVAS_SIMPLE,
        EKG_MAP_WIDTH, EKG_MAP_HEIGHT, (GX_COLOR*)ekg_map.gx_pixelmap_data, ekg_map.gx_pixelmap_data_size);

    ekg_line_start_y = CHART_Y_CENTER;
    ekg_line_end_y = CHART_Y_CENTER;
    ekg_index = 0;
}

/******************************************************************************************/
/* Reset screen data.                                                                     */
/******************************************************************************************/
static void screen_data_reset()
{
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_running_steps, 0);
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_running_distance, 0);
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_calorie_burned_progress_val, 0);
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_running_progress_val, 0);
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_standing_progress_val, 0);
    gx_radial_progress_bar_value_set(&fitness_screen.fitness_screen_calorie_burned_progress, 0);
    gx_radial_progress_bar_value_set(&fitness_screen.fitness_screen_running_progress, 0);
    gx_radial_progress_bar_value_set(&fitness_screen.fitness_screen_standing_progress, 0);
}

/******************************************************************************************/
/* Start a timer for animation.                                                           */
/******************************************************************************************/
static void animation_start(GX_WINDOW* window)
{
    animation_step = 0;

    gx_system_timer_start(window, SCREEN_ANIMATION_TIMER_ID, 40 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);
}

/******************************************************************************************/
/* Update animation.                                                                      */
/******************************************************************************************/
static void animation_update()
{
    INT val;

    animation_step++;

    val = get_running_steps();
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_running_steps, CAL_ANIMATION_VAL(val));

    val = get_running_distance();
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_running_distance, CAL_ANIMATION_VAL(val));

    val = get_calorie_burned_progress_val();
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_calorie_burned_progress_val, CAL_ANIMATION_VAL(val));

    val = val * (-360) / 100;
    gx_radial_progress_bar_value_set(&fitness_screen.fitness_screen_calorie_burned_progress, CAL_ANIMATION_VAL(val));

    val = get_running_progress_val();
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_running_progress_val, CAL_ANIMATION_VAL(val));

    val = val * (-360) / 100;
    gx_radial_progress_bar_value_set(&fitness_screen.fitness_screen_running_progress, CAL_ANIMATION_VAL(val));

    val = get_standing_progress_val();
    gx_numeric_prompt_value_set(&fitness_screen.fitness_screen_standing_progress_val, CAL_ANIMATION_VAL(val));

    val = val * (-360) / 100;
    gx_radial_progress_bar_value_set(&fitness_screen.fitness_screen_standing_progress, CAL_ANIMATION_VAL(val));

    if (animation_step == ANIMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop(&fitness_screen, SCREEN_ANIMATION_TIMER_ID);
    }
}


/******************************************************************************************/
/* Update ekg map.                                                                        */
/******************************************************************************************/
static VOID ekg_map_update()
{
    GX_RECTANGLE line_draw_rect;
    GX_RECTANGLE canvas_size;

    gx_utility_rectangle_define(&canvas_size, 0, 0, EKG_MAP_WIDTH - 1, EKG_MAP_HEIGHT - 1);
    gx_utility_rectangle_define(&line_draw_rect, 0, 0, CHART_SCROLL - 1, EKG_MAP_HEIGHT - 1);

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(&ekg_canvas, GX_NULL, &canvas_size);
    gx_canvas_block_move(&canvas_size, CHART_SCROLL, 0, GX_NULL);

    /* Erase the rectangle ahead of the line */
    gx_context_brush_define(GX_COLOR_ID_RED, GX_COLOR_ID_CANVAS, GX_BRUSH_SOLID_FILL | GX_BRUSH_ROUND | GX_BRUSH_ALIAS);
    gx_context_brush_width_set(0);
    gx_canvas_rectangle_draw(&line_draw_rect);

    gx_context_brush_width_set(2);

    ekg_line_end_y = ekg_line_start_y;
    ekg_line_start_y = CHART_Y_CENTER - ekg_values[ekg_index];

    gx_canvas_line_draw(0, ekg_line_start_y, CHART_SCROLL - 1, ekg_line_end_y);
    gx_widget_shift(&fitness_screen.fitness_screen_ekg_dot_icon, 0, ekg_line_start_y - ekg_line_end_y, GX_TRUE);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(&ekg_canvas, GX_FALSE);

    /* Mark waveform window as dirty.   */
    gx_system_dirty_mark((GX_WIDGET*)&fitness_screen.fitness_screen_ekg_waveform_win);

    ekg_index++;

    if (ekg_index >= EKG_COUNT)
    {
        ekg_index = 0;
    }
}

/******************************************************************************************/
/* Override the default event processing of "fitness_screen" to handle signals from my    */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT fitness_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        screen_data_reset();
        break;

    case USER_EVENT_ANIMATION_START:
        gx_system_timer_start(window, FITNESS_SCREEN_CHART_TIMER_ID, 100 / GX_SYSTEM_TIMER_MS, 100 / GX_SYSTEM_TIMER_MS);
        animation_start(window);
        break;

    case USER_EVENT_ANIMATION_STOP:
        gx_system_timer_stop(window, FITNESS_SCREEN_CHART_TIMER_ID);
        gx_system_timer_stop(window, SCREEN_ANIMATION_TIMER_ID);
        break;

    case GX_EVENT_TIMER:
        switch (event_ptr->gx_event_payload.gx_event_timer_id)
        {
        case FITNESS_SCREEN_CHART_TIMER_ID:
            ekg_map_update();
            break;

        case SCREEN_ANIMATION_TIMER_ID:
            animation_update();
            break;
        }
        break;

    case GX_SIGNAL(ID_CALORIE_BUTTON, GX_EVENT_CLICKED):
        page_screen_slide((GX_WIDGET *)window, (GX_WIDGET *)&calories_screen);
        break;

    case GX_SIGNAL(ID_RUNNING_BUTTON, GX_EVENT_CLICKED):
        page_screen_slide((GX_WIDGET *)window, (GX_WIDGET *)&run_screen);
        break;

    case GX_SIGNAL(ID_STANDING_BUTTON, GX_EVENT_CLICKED):
        page_screen_slide((GX_WIDGET *)window, (GX_WIDGET *)&stand_screen);
        break;

    default:
        break;
    }

    return screen_template_event_process(window, event_ptr);
}

/******************************************************************************************/
/* Override the default drawing of ekg waveform window.                                   */
/******************************************************************************************/
VOID ekg_waveform_win_draw(GX_WINDOW* window)
{
    GX_PIXELMAP *line_cover_gradient;
    GX_RECTANGLE *size = &window->gx_widget_size;

    /* Draw ekg waveform.  */
    gx_canvas_pixelmap_draw(size->gx_rectangle_left, size->gx_rectangle_top, &ekg_map);

    /* Pick line cover gradient map.  */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_LANDING_EKG_LINE_COVER_GRADIENT, &line_cover_gradient);

    /* Draw line cover gradient map.  */
    gx_canvas_pixelmap_draw(size->gx_rectangle_right - line_cover_gradient->gx_pixelmap_width + 1, size->gx_rectangle_top, line_cover_gradient);
}