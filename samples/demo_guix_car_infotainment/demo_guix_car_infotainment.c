/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

#define MEMORY_POOL_BUFFER_SIZE (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION) * 5

/* Define total number animatin steps.  */
#define TOTAL_NUMBER_ANIMATION_STEPS 10

#define CURRENT_NUMBER_VAL_GET(target_val) \
            (target_val * number_animation_step / TOTAL_NUMBER_ANIMATION_STEPS)

#define ARC_RANGE    33
#define ARC_INTERVAL 36
#define ARC_COUNT    8

#define MAX_MPH 150
#define MIN_MPH 0

typedef struct ARC_INFO_STRUCT {
    INT start_angle;
    INT end_angle;
}ARC_INFO;

ARC_INFO arc_info_list[ARC_COUNT + 1];

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY* display);
VOID clock_update();
VOID time_update();

/* Define memory for memory pool. */
GX_UBYTE memory_pool_buffer[MEMORY_POOL_BUFFER_SIZE];
TX_BYTE_POOL memory_pool;
GX_WINDOW_ROOT *root;

/* Define variables.  */
INT local_time_second = 0;
GX_WINDOW *current_screen = (GX_WINDOW *)&main_screen;
INT bottom_button_alpha = 255;
static INT number_animation_step;
static INT target_remain_miles = 382;
static INT target_battery_usage = 86;
static INT MPH_values[] = { 45, 46, 48, 50, 54, 56, 57, 58, 60, 64, 68 };
static INT MPH_value_index = 0;

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/******************************************************************************************/
/* Define memory allocator function.                                                      */
/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
/* Define memory de-allocator function.                                                   */
/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool. */
    tx_byte_pool_create(&memory_pool, "memory pool", memory_pool_buffer, MEMORY_POOL_BUFFER_SIZE);

    guix_setup();
}

/******************************************************************************************/
/* Initialize some information used by main screen.                                       */
/******************************************************************************************/
VOID main_screen_init()
{
    INT index;
    GX_RADIAL_SLIDER_INFO* info = &main_screen.main_screen_radial_slider.gx_radial_slider_info;

    arc_info_list[0].end_angle = info->gx_radial_slider_info_max_angle;
    arc_info_list[0].start_angle = arc_info_list[0].end_angle - ARC_RANGE;

    for (index = 1; index < ARC_COUNT; index++)
    {
        arc_info_list[index].start_angle = arc_info_list[index - 1].start_angle - ARC_INTERVAL;
        arc_info_list[index].end_angle = arc_info_list[index - 1].end_angle - ARC_INTERVAL;
    }

    arc_info_list[ARC_COUNT].start_angle = 0;
    arc_info_list[ARC_COUNT].end_angle = 0;
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID  guix_setup()
{

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* Create the gauge_window */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);
    main_screen_init();

    /* Create vehicle scrreen. */
    gx_studio_named_widget_create("vehicle_screen", GX_NULL, GX_NULL);

    /* Create climate scrreen. */
    gx_studio_named_widget_create("climate_screen", GX_NULL, GX_NULL);

    /* Create phone scrreen. */
    gx_studio_named_widget_create("phone_screen", GX_NULL, GX_NULL);

    /* Create audio screen. */
    gx_studio_named_widget_create("audio_screen", GX_NULL, GX_NULL);

    /* Create video screen. */
    gx_studio_named_widget_create("video_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Start number animation.                                                                */
/******************************************************************************************/
static VOID number_animation_start()
{
    number_animation_step = 0;
    gx_system_timer_start(&main_screen, TIMER_ID_MAIN_SCREEN_NUMBER_ANIMATION, 40 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);
}

/******************************************************************************************/
/* Update number animation.                                                               */
/******************************************************************************************/
static VOID number_animation_update()
{
    INT val;
    number_animation_step++;

    gx_numeric_prompt_value_set(&main_screen.main_screen_remain_miles, CURRENT_NUMBER_VAL_GET(target_remain_miles));
    val = CURRENT_NUMBER_VAL_GET(target_battery_usage);
    gx_numeric_prompt_value_set(&main_screen.main_screen_remain_battery, val);
    gx_progress_bar_value_set(&main_screen.main_screen_battery_progress_bar, val);

    if (number_animation_step == TOTAL_NUMBER_ANIMATION_STEPS)
    {
        gx_system_timer_stop(&main_screen, TIMER_ID_MAIN_SCREEN_NUMBER_ANIMATION);
    }
}

/******************************************************************************************/
/* Update speedometer animation.                                                          */
/******************************************************************************************/
static VOID speedometer_aniomation_update()
{
    GX_RADIAL_SLIDER_INFO *info = &main_screen.main_screen_radial_slider.gx_radial_slider_info;
    INT angle;

    if (MPH_value_index + 1 < sizeof(MPH_values) / sizeof(INT))
    {
        MPH_value_index++;
    }
    else
    {
        MPH_value_index = 0;
    }

    gx_numeric_prompt_value_set(&main_screen.main_screen_MPH_val, MPH_values[MPH_value_index]);
    
    /* Convert MPH value to radial progress bar value. */
    angle = MPH_values[MPH_value_index] * (info->gx_radial_slider_info_max_angle - info->gx_radial_slider_info_min_angle) / MAX_MPH;
    angle = info->gx_radial_slider_info_max_angle - angle;
    gx_radial_slider_angle_set(&main_screen.main_screen_radial_slider, angle);
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        clock_update();

        gx_system_timer_start(window, TIMER_ID_SECOND, GX_TICKS_SECOND, GX_TICKS_SECOND);

        gx_system_timer_start(&main_screen, TIMER_ID_MAIN_SCREEN_SPEEDOMETER_ANIMATION, GX_TICKS_SECOND / 2, GX_TICKS_SECOND / 2);

        /* Start number animation. */
        number_animation_start();

        /* Call default window event process. */
        base_event_process(window, event_ptr);
        break;

    case GX_EVENT_TIMER:
        switch (event_ptr->gx_event_payload.gx_event_timer_id)
        {
        case TIMER_ID_SECOND:
            time_update();
            break;

        case TIMER_ID_MAIN_SCREEN_NUMBER_ANIMATION:
            /* Update number animation. */
            number_animation_update();
            break;

        case TIMER_ID_MAIN_SCREEN_SPEEDOMETER_ANIMATION:
            /* Update speedometer animation. */
            speedometer_aniomation_update();
            break;
        }
        break;

    case GX_SIGNAL(ID_VEHICLE_MODE_BUTTON, GX_EVENT_CLICKED):
        ToggleScreen((GX_WINDOW*)&vehicle_screen);
        break;

    default:
        return base_event_process(window, event_ptr);
    }

    return GX_SUCCESS;
}

/******************************************************************************************/
/* Switch screen.                                                                         */
/******************************************************************************************/
VOID ToggleScreen(GX_WINDOW *new_win)
{
    if (new_win != current_screen)
    {
        if (!new_win->gx_widget_parent)
        {
            gx_widget_attach(root, (GX_WIDGET*)new_win);
        }
        else
        {
            gx_widget_show((GX_WIDGET*)new_win);
        }
        gx_widget_hide((GX_WIDGET*)current_screen);

        if (new_win->gx_widget_id != ID_VIDEO_SCREEN)
        {
            gx_widget_attach(new_win, &main_screen.main_screen_btn_window);
            gx_widget_attach(new_win, &main_screen.main_screen_on_off_btn);
        }
        current_screen = new_win;
    }
}

/******************************************************************************************/
/* Update clock of main screen.                                                           */
/******************************************************************************************/
VOID clock_update()
{
#ifdef WIN32
    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    gx_numeric_prompt_value_set(&main_screen.main_screen_time_hour, local_time.wHour);
    gx_numeric_prompt_value_set(&main_screen.main_screen_time_minute, local_time.wMinute);
    local_time_second = local_time.wSecond;
#else
#endif
}

/******************************************************************************************/
/* Update time.                                                                           */
/******************************************************************************************/
VOID time_update()
{
    GX_RESOURCE_ID color_id;

    if (local_time_second & 0x1)
    {
        if (main_screen.main_screen_weather_win.gx_widget_style & GX_STYLE_ENABLED)
        {
            color_id = GX_COLOR_ID_WHITE;
        }
        else
        {
            color_id = GX_COLOR_ID_DISABLED_FILL;
        }
    }
    else
    {
        color_id = GX_COLOR_ID_GRAY;
    }

    gx_widget_fill_color_set(&main_screen.main_screen_second_dot_up, color_id, color_id, color_id);
    gx_widget_fill_color_set(&main_screen.main_screen_second_dot_down, color_id, color_id, color_id);
    local_time_second++;

    if (local_time_second >= 60)
    {
        clock_update();
    }
}

/******************************************************************************************/
/* Define time value format callback.                                                     */
/******************************************************************************************/
VOID time_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    INT index = 0;

    if (value < 10)
    {
        prompt->gx_numeric_prompt_buffer[index++] = '0';
        prompt->gx_numeric_prompt_buffer[index++] = '0' + value;
    }
    else
    {
        prompt->gx_numeric_prompt_buffer[index++] = '0' + (value / 10);
        prompt->gx_numeric_prompt_buffer[index++] = '0' + (value % 10);
    }

    prompt->gx_numeric_prompt_buffer[index++] = '\0';
}

/******************************************************************************************/
/* Define custom pixelmap button draw function.                                           */
/******************************************************************************************/
VOID custom_pixelmap_button_draw(GX_PIXELMAP_BUTTON* button)
{
    GX_WIDGET* child = button->gx_widget_first_child;
    GX_BRUSH* brush;

    gx_context_brush_get(&brush);

    if (button->gx_widget_id != ID_ON_OFF_BTN)
    {
        brush->gx_brush_alpha = bottom_button_alpha;
    }

    while (child)
    {
        if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            gx_widget_style_add(child, GX_STYLE_DRAW_SELECTED);
        }
        else
        {
            gx_widget_style_remove(child, GX_STYLE_DRAW_SELECTED);
        }

        child = child->gx_widget_next;
    }

    gx_pixelmap_button_draw(button);
}


/******************************************************************************************/
/* Custom defined draw function for main screen radial slider widget.                     */
/******************************************************************************************/
VOID radial_slider_draw(GX_RADIAL_SLIDER* slider)
{
    GX_RADIAL_SLIDER_INFO* info = &slider->gx_radial_slider_info;
    GX_PIXELMAP* map;
    INT xpos;
    INT ypos;
    ARC_INFO* arc = arc_info_list;
    INT start_angle;
    INT radius;
    INT angle = info->gx_radial_slider_info_current_angle;
    GX_POINT inner_pt;
    GX_POINT outer_pt;

    /* Call default widget background draw. */
    gx_widget_background_draw((GX_WIDGET*)slider);

    gx_context_pixelmap_get(info->gx_radial_slider_info_background_pixelmap, &map);

    xpos = slider->gx_widget_size.gx_rectangle_left;
    ypos = slider->gx_widget_size.gx_rectangle_top;

    if (map)
    {
        gx_context_fill_color_set(GX_COLOR_ID_BLUE);

        /* Draw background pixelmap. */
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }

    gx_context_brush_width_set(4);
    gx_context_brush_define(GX_COLOR_ID_LIGHT_GREEN, GX_COLOR_ID_LIGHT_GREEN, GX_BRUSH_ALIAS | GX_BRUSH_ROUND);

    xpos += info->gx_radial_slider_info_xcenter;
    ypos += info->gx_radial_slider_info_ycenter;

    radius = info->gx_radial_slider_info_radius;
    radius += (info->gx_radial_slider_info_track_width / 2);
    radius -= 2;

    /* Fill outer ring with disconnected arcs. */
    while (arc->start_angle < arc->end_angle)
    {
        if (info->gx_radial_slider_info_current_angle < arc->start_angle)
        {
            start_angle = arc->start_angle;
        }
        else
        {
            start_angle = info->gx_radial_slider_info_current_angle;
        }

        if (start_angle < arc->end_angle)
        {
            gx_canvas_arc_draw(xpos, ypos, radius, start_angle, arc->end_angle);
        }

        if (start_angle == info->gx_radial_slider_info_current_angle)
        {
            break;
        }

        arc++;
    }

    /* Get the point on the inner circle of the track with given center point, radius and angle. */
    gx_utility_circle_point_get(xpos,
        ypos,
        info->gx_radial_slider_info_radius - (info->gx_radial_slider_info_track_width >> 1) + 1,
        angle,
        &inner_pt);

    /* Get the point on the outer circle of the track with given center point, radius and angle. */
    gx_utility_circle_point_get(xpos,
        ypos,
        info->gx_radial_slider_info_radius + (info->gx_radial_slider_info_track_width >> 1) - 1,
        angle,
        &outer_pt);

    /* Draw needle. */
    gx_canvas_line_draw(inner_pt.gx_point_x, inner_pt.gx_point_y, outer_pt.gx_point_x, outer_pt.gx_point_y);

    /* Draw children widgets. */
    gx_widget_children_draw((GX_WIDGET*)slider);
}