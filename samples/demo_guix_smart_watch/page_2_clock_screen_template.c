#include "demo_guix_smart_watch.h"

/* Extern system time.  */
extern TIME system_time;

/* Define local variables.  */
static INT minute_needle_angle = 30;
static INT hour_needle_angle = 60;

/******************************************************************************************/
/* Update watch hands.                                                                    */
/******************************************************************************************/
static VOID clock_gauge_update(CLOCK_SCREEN_TEMPLATE_CONTROL_BLOCK* template)
{
    INT hour_angle;
    INT minute_angle;
    GX_BOOL mark_dirty = GX_FALSE;

    minute_angle = system_time.minute * 6;

    if (minute_angle != minute_needle_angle)
    {
        minute_needle_angle = minute_angle;
        mark_dirty = GX_TRUE;
    }

    hour_angle = system_time.hour * 30 + system_time.minute / 2;

    if (hour_angle != hour_needle_angle)
    {
        hour_needle_angle = hour_angle;
        mark_dirty = GX_TRUE;
    }

    gx_circular_gauge_angle_set(&template->clock_screen_template_gauge, system_time.second * 6);

    if (mark_dirty)
    {
        gx_system_dirty_mark(&template->clock_screen_template_gauge);
    }
}

/******************************************************************************************/
/* Draw a needle in specified position.                                                   */
/******************************************************************************************/
static VOID draw_needle(GX_PIXELMAP *map, INT angle, INT xcenter, INT ycenter)
{
    GX_PIXELMAP needle_rotated;
    INT xcor;
    INT ycor;

    xcor = (map->gx_pixelmap_width >> 1);
    ycor = map->gx_pixelmap_height - xcor;

    /* Rotate the second needle pixelmap. */
    if (gx_utility_pixelmap_rotate(map, angle, &needle_rotated, &xcor, &ycor) == GX_SUCCESS)
    {
        /* Draw needle .  */
        gx_canvas_pixelmap_draw((GX_VALUE)(xcenter - xcor), (GX_VALUE)(ycenter - ycor), &needle_rotated);

        /* Free rotate needle pixelmap memory.  */
        if (memory_free)
        {
            if (needle_rotated.gx_pixelmap_aux_data)
            {
                memory_free((VOID*)needle_rotated.gx_pixelmap_aux_data);
            }
            memory_free((VOID*)needle_rotated.gx_pixelmap_data);
        }
    }
    else
    {
        /* Draw needle .  */
        gx_canvas_pixelmap_draw((GX_VALUE)(xcenter - xcor), (GX_VALUE)(ycenter - ycor), map);
    }
}

/******************************************************************************************/
/* Draw specified gauge with extra needles.                                               */
/******************************************************************************************/
static VOID clock_gauge_draw(GX_CIRCULAR_GAUGE *gauge, GX_RESOURCE_ID hands_color_id, GX_RESOURCE_ID hands_center_map_id)
{
    GX_CIRCULAR_GAUGE_INFO *info;
    INT xcenter;
    INT ycenter;
    GX_PIXELMAP *map;

    /* Call default circular draw. */
    gx_circular_gauge_background_draw(gauge);

    /* Pick up pointer to the gauge information structure.  */
    info = &gauge->gx_circular_gauge_info;

    xcenter = gauge->gx_widget_size.gx_rectangle_left + info->gx_circular_gauge_info_needle_xpos;
    ycenter = gauge->gx_widget_size.gx_rectangle_top + info->gx_circular_gauge_info_needle_ypos;

    gx_context_fill_color_set(hands_color_id);

    /* Retrieve the map for the minute needle. */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_WATCH_HAND_HOUR, &map);

    if (map)
    {
        draw_needle(map, hour_needle_angle, xcenter, ycenter);
    }

    /* Retrieve the map for the second needle. */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_WATCH_HAND_MINUTE, &map);

    if (map)
    {
        draw_needle(map, minute_needle_angle, xcenter, ycenter);
    }

    /* Draw watch hands center. */
    gx_context_pixelmap_get(hands_center_map_id, &map);
    if (map)
    {
        gx_canvas_pixelmap_draw((GX_VALUE)(xcenter - (map->gx_pixelmap_width >> 1)),
            (GX_VALUE)(ycenter - (map->gx_pixelmap_height >> 1)), map);
    }
}

/******************************************************************************************/
/* Define custom drawing function for the circular gauge in clock 1 screen.               */
/******************************************************************************************/
static VOID clock_1_gauge_draw(GX_CIRCULAR_GAUGE *gauge)
{
    /* Draw the gauge with black watch hands.  */
    clock_gauge_draw(gauge, GX_COLOR_ID_CANVAS, GX_PIXELMAP_ID_WATCH_HANDS_CENTER);
}

/******************************************************************************************/
/* Define custom drawing function for the circular gauge in clock 2 screen.               */
/******************************************************************************************/
static VOID clock_2_gauge_draw(GX_CIRCULAR_GAUGE *gauge)
{
    /* Draw the gauge with white watch hands.  */
    clock_gauge_draw(gauge, GX_COLOR_ID_WHITE, GX_PIXELMAP_ID_WATCH_HANDS_CENTER_WHITE);
}

/******************************************************************************************/
/* Initialize watch screen.                                                               */
/******************************************************************************************/
VOID watch_screen_initialize()
{
    gx_widget_fill_color_set(&clock_1_screen.base, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    gx_widget_fill_color_set(&clock_2_screen.base, GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS);

    gx_icon_pixelmap_set((GX_ICON*)&clock_1_screen.base.clock_screen_template_gauge, GX_PIXELMAP_ID_W_DIAL_BLACK, GX_PIXELMAP_ID_W_DIAL_BLACK);
    gx_icon_pixelmap_set((GX_ICON*)&clock_2_screen.base.clock_screen_template_gauge, GX_PIXELMAP_ID_W_DIAL_WHITE, GX_PIXELMAP_ID_W_DIAL_WHITE);

    gx_widget_draw_set((GX_ICON*)&clock_1_screen.base.clock_screen_template_gauge, clock_1_gauge_draw);
    gx_widget_draw_set((GX_ICON*)&clock_2_screen.base.clock_screen_template_gauge, clock_2_gauge_draw);
}

/******************************************************************************************/
/* Override the default event processing of "clock_screen_template" to handle signals     */
/* from my child widgets.                                                                 */
/******************************************************************************************/
UINT clock_screen_template_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        clock_gauge_update((CLOCK_SCREEN_TEMPLATE_CONTROL_BLOCK*)window);
        gx_system_timer_start(window, SCREEN_CLOCK_TIMER_ID, GX_TICKS_SECOND, GX_TICKS_SECOND);
        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_system_timer_stop(window, SCREEN_CLOCK_TIMER_ID);
        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SCREEN_CLOCK_TIMER_ID)
        {
            clock_gauge_update((CLOCK_SCREEN_TEMPLATE_CONTROL_BLOCK*)window);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
