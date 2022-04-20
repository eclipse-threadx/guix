#include "demo_guix_smart_watch.h"

#define CLOCK_SLIDE_SHIFT 40

#define LINE_LENGTH 25

#define TARGET_HOUR_MIN_XPOS 206
#define TARGET_HOUR_MIN_YPOS 132
#define TARGET_HOUR_MAX_XPOS (TARGET_HOUR_MIN_XPOS + CLOCK_SLIDE_SHIFT)
#define TARGET_HOUR_MAX_YPOS (TARGET_HOUR_MIN_YPOS + CLOCK_SLIDE_SHIFT)

#define TARGET_MINIUTE_MIN_XPOS 211
#define TARGET_MINIUTE_MIN_YPOS 218
#define TARGET_MINIUTE_MAX_XPOS (TARGET_MINIUTE_MIN_XPOS + CLOCK_SLIDE_SHIFT)
#define TARGET_MINIUTE_MAX_YPOS (TARGET_MINIUTE_MIN_YPOS + CLOCK_SLIDE_SHIFT)

/* Define clock slide animation control blocks. */
static GX_ANIMATION clock_slide_animation[2];

/* Define local variable for background wave animation.  */
static INT wave_rotation_angle = 0;

/******************************************************************************************/
/* Start clock slide animation.                                                           */
/******************************************************************************************/
static void clock_slide_animation_start()
{
    GX_ANIMATION_INFO info;
    GX_WIDGET* target_hour = (GX_WIDGET*)&clock_5_screen.clock_5_screen_hour;
    GX_WIDGET* target_minute = (GX_WIDGET*)&clock_5_screen.clock_5_screen_minute;
    GX_POINT target_hour_end_pos;
    GX_POINT target_minute_end_pos;

    if (target_hour->gx_widget_size.gx_rectangle_left < target_minute->gx_widget_size.gx_rectangle_left)
    {
        target_hour_end_pos.gx_point_x = TARGET_HOUR_MAX_XPOS;
        target_hour_end_pos.gx_point_y = TARGET_HOUR_MAX_YPOS;
        target_minute_end_pos.gx_point_x = TARGET_MINIUTE_MIN_XPOS;
        target_minute_end_pos.gx_point_y = TARGET_MINIUTE_MIN_YPOS;
    }
    else
    {
        target_hour_end_pos.gx_point_x = TARGET_HOUR_MIN_XPOS;
        target_hour_end_pos.gx_point_y = TARGET_HOUR_MIN_YPOS;
        target_minute_end_pos.gx_point_x = TARGET_MINIUTE_MAX_XPOS;
        target_minute_end_pos.gx_point_y = TARGET_MINIUTE_MAX_YPOS;
    }

    memset(&info, 0, sizeof(GX_ANIMATION_INFO));
    info.gx_animation_parent = (GX_WIDGET*)&clock_5_screen;
    info.gx_animation_target = target_hour;
    info.gx_animation_start_alpha = 255;
    info.gx_animation_end_alpha = 255;
    info.gx_animation_frame_interval = 40 / GX_SYSTEM_TIMER_MS;
    info.gx_animation_id = CLOCK_SLIDE_ANIMATION_ID;
    info.gx_animation_start_position.gx_point_x = target_hour->gx_widget_size.gx_rectangle_left;
    info.gx_animation_start_position.gx_point_y = target_hour->gx_widget_size.gx_rectangle_top;
    info.gx_animation_end_position = target_hour_end_pos;
    info.gx_animation_steps = GX_ABS(info.gx_animation_end_position.gx_point_x - info.gx_animation_start_position.gx_point_x);

    gx_animation_start(&clock_slide_animation[0], &info);

    info.gx_animation_target = target_minute;
    info.gx_animation_start_position.gx_point_x = target_minute->gx_widget_size.gx_rectangle_left;
    info.gx_animation_start_position.gx_point_y = target_minute->gx_widget_size.gx_rectangle_top;
    info.gx_animation_end_position = target_minute_end_pos;

    gx_animation_start(&clock_slide_animation[1], &info);
}

/******************************************************************************************/
/* Stop clock slide animation.                                                            */
/******************************************************************************************/
static void clock_slide_animation_stop()
{
    gx_animation_stop(&clock_slide_animation[0]);
    gx_animation_stop(&clock_slide_animation[1]);
}

/******************************************************************************************/
/* Override the default event processing of "clock_screen_template" to handle signals     */
/* from my child widgets.                                                                 */
/******************************************************************************************/
UINT clock_5_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_animation_create(&clock_slide_animation[0]);
        gx_animation_create(&clock_slide_animation[1]);
        clear_screen_clock_record();
        screen_clock_update(&clock_5_screen.clock_5_screen_hour, &clock_5_screen.clock_5_screen_minute, GX_NULL);
        gx_system_timer_start(window, SCREEN_CLOCK_TIMER_ID, GX_TICKS_SECOND, GX_TICKS_SECOND);
        return gx_window_event_process(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_system_timer_stop(window, SCREEN_CLOCK_TIMER_ID);
        return gx_window_event_process(window, event_ptr);

    case USER_EVENT_ANIMATION_START:
        gx_system_timer_start(window, SCREEN_ANIMATION_TIMER_ID, 20 / GX_SYSTEM_TIMER_MS, 20 / GX_SYSTEM_TIMER_MS);
        clock_slide_animation_start();
        break;

    case USER_EVENT_ANIMATION_STOP:
        clock_slide_animation_stop();
        gx_system_timer_stop(window, SCREEN_ANIMATION_TIMER_ID);
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        if (event_ptr->gx_event_sender == CLOCK_SLIDE_ANIMATION_ID)
        {
            clock_slide_animation_start();
        }
        break;

    case GX_EVENT_TIMER:
        switch (event_ptr->gx_event_payload.gx_event_timer_id)
        {
        case SCREEN_CLOCK_TIMER_ID:
            screen_clock_update(&clock_5_screen.clock_5_screen_hour, &clock_5_screen.clock_5_screen_minute, GX_NULL);
            break;

        case SCREEN_ANIMATION_TIMER_ID:
            wave_rotation_angle = (358 + wave_rotation_angle) % 360;

            gx_system_dirty_mark(window);
            break;
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Draw animation wave in the center of specified window.                                 */
/******************************************************************************************/
VOID clock_5_screen_draw(GX_WINDOW* window)
{
    GX_RECTANGLE* size = &window->gx_widget_size;
    INT scaled_angle;
    INT xdist;
    INT ydist;
    INT xcenter;
    INT ycenter;
    INT width;
    INT height;
    GX_PIXELMAP* map;

    gx_window_background_draw(window);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_WAVE, &map);

    width = size->gx_rectangle_right - size->gx_rectangle_left + 1;
    height = size->gx_rectangle_bottom - size->gx_rectangle_top + 1;
    xcenter = size->gx_rectangle_left + width * 3 / 5;
    ycenter = size->gx_rectangle_top + height * 3 / 5;

    gx_context_brush_define(GX_COLOR_ID_BLUE, GX_COLOR_ID_BLUE, GX_BRUSH_ALIAS | GX_BRUSH_SOLID_FILL);
    gx_context_brush_width_set(1);

    scaled_angle = GX_FIXED_VAL_MAKE(wave_rotation_angle);
    xdist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_LENGTH);
    ydist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * LINE_LENGTH);

    width >>= 1;
    height >>= 1;

    gx_canvas_pixelmap_draw(xcenter + xdist - width, ycenter + ydist - height, map);

    scaled_angle = GX_FIXED_VAL_MAKE(wave_rotation_angle + 60);
    xdist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_LENGTH);
    ydist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * LINE_LENGTH);

    gx_canvas_pixelmap_draw(xcenter + xdist - width, ycenter + ydist - height, map);

    scaled_angle = GX_FIXED_VAL_MAKE(wave_rotation_angle + 120);
    xdist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_LENGTH);
    ydist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * LINE_LENGTH);

    gx_canvas_pixelmap_draw(xcenter + xdist - width, ycenter + ydist - height, map);

    gx_widget_children_draw(window);
}