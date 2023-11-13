/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_washing_machine.h"

#define ID_TIMER_WAVE_ANIMATION         1
#define ID_TIMER_PROGRESS_BAR_ANIMATION 2
#define ID_TIMER_TIME                   3

#define PROGRESS_DOT_SPACE   9
#define PROGRESS_ALPHA_SHIFT 10

#define ROTATION_DISTANCE 25
#define WASH_CYCLE_TEMPERATURE_RING_WIDTH 52

/* Define variables.  */
GX_WINDOW_ROOT *root;

INT             wave_rotation_angle = 0; /* Used for wave animation in water cycle and temperature window.  */
INT             wash_cycle_remain_seconds = 3600; /* Wash cyle remain time.  */
INT             progress_alpha_start = 0; /* Used for progress bar animation.  */

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY* display);

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    guix_setup();
}

/******************************************************************************************/
/* Define custom button selection function to remove GX_STYLE_BUTTON_PUSHED style before  */
/* call the default button select route, so that button selected event can be send no     */
/* mater the selected button is pushed already or not.                                    */
/******************************************************************************************/
VOID repeat_selection_style_button_select(GX_WIDGET *button)
{
    button->gx_widget_style &= ~GX_STYLE_BUTTON_PUSHED;

    gx_button_select((GX_BUTTON *)button);
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID  guix_setup()
{

    /* Initialize GUIX. */
    gx_system_initialize();

    /* Configure display. */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb,
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* Create the main screen and attach it to root window. */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Change button select handler of wash cycle button.  */
    main_screen.main_screen_btn_wash_cycle.gx_button_select_handler = repeat_selection_style_button_select;

    /* Create wash cycle window. */
    gx_studio_named_widget_create("wash_cycle_window", (GX_WIDGET *)&main_screen, GX_NULL);

    /* Initialize wash cycle window.  */
    wash_cycle_window_init(wash_cycle_remain_seconds);

    /* Create mode select window. */
    gx_studio_named_widget_create("mode_select_window", GX_NULL, GX_NULL);

    /* Initialize mode select window.  */
    mode_select_window_init();

    /* Create temperature window. */
    gx_studio_named_widget_create("temperature_window", GX_NULL, GX_NULL);

    /* Create water level window. */
    gx_studio_named_widget_create("water_level_window", GX_NULL, GX_NULL);

    /* Initialize water level window.  */
    water_level_window_init();

    /* Show the root window to make it and wash cycle screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run.  */
    gx_system_start();
}

/******************************************************************************************/
/* Mark wave animation parent dirty.                                                      */
/******************************************************************************************/
VOID mark_wave_animation_parent_dirty()
{
    GX_RECTANGLE dirty_area;
    GX_WIDGET *parent;

    if (wash_cycle_window.gx_widget_status & GX_STATUS_VISIBLE)
    {
        parent = (GX_WIDGET *)&wash_cycle_window;
    }
    else if (temperature_window.gx_widget_status & GX_STATUS_VISIBLE)
    {
        parent = (GX_WIDGET *)&temperature_window;
    }
    else
    {
        return;
    }

    /* Calculate wave area. */
    dirty_area = parent->gx_widget_size;
    dirty_area.gx_rectangle_left = (GX_VALUE)(dirty_area.gx_rectangle_left + WASH_CYCLE_TEMPERATURE_RING_WIDTH);
    dirty_area.gx_rectangle_top = (GX_VALUE)(dirty_area.gx_rectangle_top + WASH_CYCLE_TEMPERATURE_RING_WIDTH);
    dirty_area.gx_rectangle_right = (GX_VALUE)(dirty_area.gx_rectangle_right - WASH_CYCLE_TEMPERATURE_RING_WIDTH);
    dirty_area.gx_rectangle_bottom = (GX_VALUE)(dirty_area.gx_rectangle_bottom - WASH_CYCLE_TEMPERATURE_RING_WIDTH);

    /* Mark wave area as dirty. */
    gx_system_dirty_partial_add(parent, &dirty_area);
}

/******************************************************************************************/
/* Update wash cycle remain time that displayed in various windows.                       */
/******************************************************************************************/
VOID wash_cycle_remain_time_update(INT remain_reconds)
{
    /* Update remain time in wash cycle window.  */
    wash_cycle_window_remain_time_update(remain_reconds);

    /* Update remain time in mode select window.  */
    mode_select_window_remain_time_update(remain_reconds);
}

/******************************************************************************************/
/* Set wash cycle remain time.                                                            */
/******************************************************************************************/
VOID remain_time_set(INT seconds)
{
    /* Set current wash cycle remain time.  */
    wash_cycle_remain_seconds = seconds;

    /* Update wash cycle remain time that displayed in various windows.  */
    wash_cycle_remain_time_update(wash_cycle_remain_seconds);
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BTN_WASH_CYCLE, GX_EVENT_RADIO_SELECT):

        if (wash_cycle_window.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Dettach wash cycle window. */
            gx_widget_detach(&wash_cycle_window);

            /* Attach mode select window.  */
            gx_widget_attach((GX_WIDGET*)window, &mode_select_window);
        }
        else
        {
            if (mode_select_window.gx_widget_status & GX_STATUS_VISIBLE)
            {
                /* Dettach mode select window. */
                gx_widget_detach(&mode_select_window);
            }

            /* Attach wash cycle window. */
            gx_widget_attach((GX_WIDGET*)window, &wash_cycle_window);
        }
        break;

    case GX_SIGNAL(ID_BTN_WASH_CYCLE, GX_EVENT_RADIO_DESELECT):

        if (mode_select_window.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Dettach wash cycle window. */
            gx_widget_detach(&mode_select_window);
        }
        else
        {
            /* Dettach mode select window. */
            gx_widget_detach(&wash_cycle_window);
        }
        break;

    case GX_SIGNAL(ID_BTN_TEMPERATURE, GX_EVENT_RADIO_SELECT):

        /* Attach temperature window. */
        gx_widget_attach((GX_WIDGET*)window, &temperature_window);
        break;

    case GX_SIGNAL(ID_BTN_TEMPERATURE, GX_EVENT_RADIO_DESELECT):

        /* Dettach temperature window. */
        gx_widget_detach(&temperature_window);
        break;

    case GX_SIGNAL(ID_BTN_WATER_LEVEL, GX_EVENT_RADIO_SELECT):

        /* Attach water level window. */
        gx_widget_attach((GX_WIDGET*)window, &water_level_window);
        break;

    case GX_SIGNAL(ID_BTN_WATER_LEVEL, GX_EVENT_RADIO_DESELECT):

        /* Dettach water level page. */
        gx_widget_detach(&water_level_window);
        break;

    case GX_SIGNAL(ID_BTN_PLAY, GX_EVENT_TOGGLE_ON):
        gx_system_timer_start(window, ID_TIMER_TIME, GX_TICKS_SECOND, GX_TICKS_SECOND);
        gx_system_timer_start(window, ID_TIMER_PROGRESS_BAR_ANIMATION, 40 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);
        gx_system_timer_start(window, ID_TIMER_WAVE_ANIMATION, 1, 1);
        break;

    case GX_SIGNAL(ID_BTN_PLAY, GX_EVENT_TOGGLE_OFF):
        gx_system_timer_stop(window, 0);

        mark_wave_animation_parent_dirty();
        gx_system_dirty_mark(&main_screen.main_screen_wash_cycle_progress_bar);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ID_TIMER_WAVE_ANIMATION)
        {
            wave_rotation_angle = (358 + wave_rotation_angle) % 360;

            mark_wave_animation_parent_dirty();
        }
        if(event_ptr->gx_event_payload.gx_event_timer_id == ID_TIMER_PROGRESS_BAR_ANIMATION)
        {
            gx_system_dirty_mark(&main_screen.main_screen_wash_cycle_progress_bar);
        }
        if(event_ptr->gx_event_payload.gx_event_timer_id == ID_TIMER_TIME)
        {
            wash_cycle_remain_seconds--;

            if (wash_cycle_remain_seconds >= 0)
            {
                wash_cycle_remain_time_update(wash_cycle_remain_seconds);
            }
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Define custom prompt draw function for child prompts of a button.                      */
/******************************************************************************************/
VOID btn_text_draw(GX_PROMPT *prompt)
{
    if (prompt->gx_widget_parent->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        prompt->gx_widget_style |= GX_STYLE_DRAW_SELECTED;
    }
    else
    {
        prompt->gx_widget_style &= (~GX_STYLE_DRAW_SELECTED);
    }

    gx_prompt_draw(prompt);
}

/******************************************************************************************/
/* Define custom numeric prompt draw function for child numeric prompts of a button.      */
/******************************************************************************************/
VOID btn_numeric_text_draw(GX_NUMERIC_PROMPT* prompt)
{
    if (prompt->gx_widget_parent->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        prompt->gx_widget_style |= GX_STYLE_DRAW_SELECTED;
    }
    else
    {
        prompt->gx_widget_style &= (~GX_STYLE_DRAW_SELECTED);
    }

    gx_prompt_draw((GX_PROMPT *)prompt);
}

/******************************************************************************************/
/* Define format function for numeric prompt to show time.                                */
/******************************************************************************************/
VOID time_format(GX_NUMERIC_PROMPT* prompt, INT value)
{
    if (value < 10)
    {
        prompt->gx_numeric_prompt_buffer[0] = '0';
        prompt->gx_numeric_prompt_buffer[1] = '0' + value;
    }
    else if (value < 100)
    {
        prompt->gx_numeric_prompt_buffer[0] = '0' + (value / 10);
        prompt->gx_numeric_prompt_buffer[1] = '0' + (value % 10);
    }
}

/******************************************************************************************/
/* Draw animation wave in the center of specified window.                                 */
/******************************************************************************************/
VOID animation_wave_draw(GX_WINDOW* window)
{
    GX_RECTANGLE *size = &window->gx_widget_size;
    INT scaled_angle;
    INT xdist;
    INT ydist;
    INT xcenter;
    INT ycenter;
    INT width;
    INT height;
    GX_PIXELMAP *map;

    gx_context_pixelmap_get(GX_PIXELMAP_ID_WAVE, &map);

    width = size->gx_rectangle_right - size->gx_rectangle_left + 1;
    height = size->gx_rectangle_bottom - size->gx_rectangle_top + 1;
    xcenter = size->gx_rectangle_left + (width >> 1);
    ycenter = size->gx_rectangle_top + (height >> 1);

    gx_context_brush_define(GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_BRUSH_ALIAS | GX_BRUSH_SOLID_FILL);
    gx_context_brush_width_set(1);

    scaled_angle = GX_FIXED_VAL_MAKE(wave_rotation_angle);
    xdist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * ROTATION_DISTANCE);
    ydist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * ROTATION_DISTANCE);

    width = (map->gx_pixelmap_width >> 1);
    height = (map->gx_pixelmap_height >> 1);

    gx_canvas_pixelmap_draw(xcenter + xdist - width, ycenter + ydist - height, map);

    scaled_angle = GX_FIXED_VAL_MAKE(wave_rotation_angle + 60);
    xdist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * ROTATION_DISTANCE);
    ydist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * ROTATION_DISTANCE);

    gx_canvas_pixelmap_draw(xcenter + xdist - width, ycenter + ydist - height, map);

    scaled_angle = GX_FIXED_VAL_MAKE(wave_rotation_angle + 120);
    xdist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * ROTATION_DISTANCE);
    ydist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * ROTATION_DISTANCE);

    gx_canvas_pixelmap_draw(xcenter + xdist - width, ycenter + ydist - height, map);
}

/******************************************************************************************/
/* Define custom progress bar draw function.                                              */
/******************************************************************************************/
VOID wash_cycle_progress_bar_draw(GX_PROGRESS_BAR* progress_bar)
{
    GX_PIXELMAP *map;
    INT xpos;
    INT ypos;
    INT selected_dist;
    GX_RECTANGLE *size = &progress_bar->gx_widget_size;
    GX_BRUSH *brush;
    INT alpha_shift;

    gx_context_pixelmap_get(GX_PIXELMAP_ID_CYCLE_PROGRESS_DOT, &map);

    ypos = size->gx_rectangle_top;

    /* Draw background dots. */
    gx_context_brush_get(&brush);
    gx_context_fill_color_set(GX_COLOR_ID_WHITE);

    brush->gx_brush_alpha = 100;
    for (xpos = size->gx_rectangle_left; xpos < size->gx_rectangle_right; xpos += PROGRESS_DOT_SPACE)
    {
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }

    brush->gx_brush_alpha = 255;

    selected_dist = (size->gx_rectangle_right - size->gx_rectangle_left) * progress_bar->gx_progress_bar_info.gx_progress_bar_info_current_val / 100;

    /* Draw selected dots.  */
    if (main_screen.main_screen_btn_play.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        if (selected_dist / PROGRESS_DOT_SPACE)
        {
            alpha_shift = 255 / (selected_dist / PROGRESS_DOT_SPACE);
        }
        else
        {
            alpha_shift = 255;
        }

        brush->gx_brush_alpha = progress_alpha_start;
        for (xpos = size->gx_rectangle_left; xpos < size->gx_rectangle_left + selected_dist; xpos += PROGRESS_DOT_SPACE)
        {
            gx_canvas_pixelmap_draw(xpos, ypos, map);
            if (brush->gx_brush_alpha + alpha_shift < 255)
            {
                brush->gx_brush_alpha += alpha_shift;
            }
            else
            {
                brush->gx_brush_alpha = 0;
            }
        }

        brush->gx_brush_alpha = 255;

        if (progress_alpha_start > PROGRESS_ALPHA_SHIFT)
        {
            progress_alpha_start -= PROGRESS_ALPHA_SHIFT;
        }
        else
        {
            progress_alpha_start = 255;
        }
    }
    else
    {
        for (xpos = size->gx_rectangle_left; xpos < size->gx_rectangle_left + selected_dist; xpos += PROGRESS_DOT_SPACE)
        {
            gx_canvas_pixelmap_draw(xpos, ypos, map);
        }
    }
}