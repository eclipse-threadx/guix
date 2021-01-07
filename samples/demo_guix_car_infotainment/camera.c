/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

#define NUMBER_ANMATION_TIMER 10
#define CAMERA_ANIMATION_ID  1
/* Define variables. */
INT               lines_animation_stage;
INT               total_number_animation_step;
INT               remain_number_animation_step;
INT               start_number;
INT               target_number;
INT               current_number;

/* Define prototypes. */
VOID lines_animation_start();
VOID number_animation_update();

/******************************************************************************************/
/* Override default drawing of "camera_screen" to do custom drawing.                      */
/******************************************************************************************/
VOID camera_screen_draw(GX_WINDOW *window)
{
    if (decoded_map.gx_pixelmap_data)
    {
        /* Draw camera background map. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left, window->gx_widget_size.gx_rectangle_top,
            &decoded_map);
    }

    /* Call default window draw. */
    gx_window_draw(window);
}

VOID on_reload(GX_WIDGET *widget)
{
    GX_STRING text;

    /* Initiate information for number animation.  */
    total_number_animation_step = 80;
    remain_number_animation_step = 80;
    start_number = 0;
    target_number = 625;

    text.gx_string_ptr = "0";
    text.gx_string_length = 1;
    gx_prompt_text_set_ext(&camera_screen.camera_screen_number, &text);
    gx_system_timer_start(widget, NUMBER_ANMATION_TIMER, 10, 1);

    /* Start line animation. */
    lines_animation_stage = 0;
    lines_animation_start();
}

/******************************************************************************************/
/* Override default event processing of "camera_screen" to handle signals from my child   */
/* widget.                                                                                */
/******************************************************************************************/
UINT camera_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Call base screen event process. */
        base_screen_event_process(window, event_ptr);

        /* Decode camera background jpg. */
        jpg_decode(GX_PIXELMAP_ID_CAMERA_BG, &decoded_map);
        break;

    case GX_EVENT_HIDE:
        /* Call base screen event process. */
        base_screen_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_RELOAD, GX_EVENT_CLICKED):
        on_reload((GX_WIDGET*)window);
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        lines_animation_stage++;
        if (lines_animation_stage < 4)
        {
            lines_animation_start();
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == NUMBER_ANMATION_TIMER)
        {
            number_animation_update();
        }
        break;

    default:
        return base_screen_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Start camera animation.                                                                */
/******************************************************************************************/
VOID lines_animation_start()
{
GX_WIDGET    *target;
INT           start_x;
INT           start_y;
INT           target_x;
INT           target_y;
GX_ANIMATION *camera_animation = GX_NULL;
GX_ANIMATION_INFO lines_animation_info;

    gx_system_animation_get(&camera_animation);

    if (camera_animation)
    {
        if (lines_animation_stage == 0)
        {
            /* Stage 0: slide lines widget down. */
            target = (GX_WIDGET *)&camera_screen.camera_screen_lines;
            start_x = 80;
            start_y = 291;
            target_x = 80;
            target_y = 480;
        }
        else if (lines_animation_stage == 1)
        {
            /* Stage 1: Slide arrow widget down. */
            target = (GX_WIDGET *)&camera_screen.camera_screen_arrow;
            start_x = 301;
            start_y = 317;
            target_x = 301;
            target_y = 480;
        }
        else if (lines_animation_stage == 2)
        {
            /* Stage 2: Slide lines widget up. */
            target = (GX_WIDGET *)&camera_screen.camera_screen_lines;
            start_x = 80;
            start_y = 480;
            target_x = 80;
            target_y = 291;
        }
        else if (lines_animation_stage == 3)
        {
            /* Stage 3: Slide arrow widget up. */
            target = (GX_WIDGET *)&camera_screen.camera_screen_arrow;
            start_x = 301;
            start_y = 480;
            target_x = 301;
            target_y = 317;
        }

        memset(&lines_animation_info, 0, sizeof(lines_animation_info));

        lines_animation_info.gx_animation_frame_interval = 1;
        lines_animation_info.gx_animation_steps = 20;
        lines_animation_info.gx_animation_target = target;
        lines_animation_info.gx_animation_parent = (GX_WIDGET *)&camera_screen;
        lines_animation_info.gx_animation_start_position.gx_point_x = start_x;
        lines_animation_info.gx_animation_start_position.gx_point_y = start_y;
        lines_animation_info.gx_animation_end_position.gx_point_x = target_x;
        lines_animation_info.gx_animation_end_position.gx_point_y = target_y;
        lines_animation_info.gx_animation_start_alpha = 255;
        lines_animation_info.gx_animation_end_alpha = 255;
        lines_animation_info.gx_animation_id = CAMERA_ANIMATION_ID;
        gx_animation_start(camera_animation, &lines_animation_info);

        gx_widget_front_move(&camera_screen.camera_screen_arrow, GX_NULL);
        gx_widget_front_move(&camera_screen.camera_screen_menu_window, GX_NULL);
    }
}

/******************************************************************************************/
/* Update number value.                                                                   */
/******************************************************************************************/
VOID number_animation_update()
{
GX_CHAR   text_buffer[10];
GX_STRING text;
INT       index;

    /* Decrease number animation step. */
    remain_number_animation_step--;

    /* Calcualte current number value. */
    current_number = (target_number - start_number);
    current_number *= (total_number_animation_step - remain_number_animation_step);
    current_number /= total_number_animation_step;
    current_number += start_number;

    if (current_number == target_number)
    {
        /* Stop number animation timer. */
        gx_system_timer_stop((GX_WIDGET *)&camera_screen, NUMBER_ANMATION_TIMER);
    }

    gx_utility_ltoa(current_number / 100, text_buffer, 10);
    index = string_length_get(text_buffer, sizeof(text_buffer) - 1);

    if (current_number % 100)
    {
        text_buffer[index++] = '.';
        gx_utility_ltoa(current_number % 100, text_buffer + index, 5);
    }

    text.gx_string_ptr = text_buffer;
    text.gx_string_length = string_length_get(text_buffer, sizeof(text_buffer) - 1);

    /* Update number value. */
    gx_prompt_text_set_ext(&camera_screen.camera_screen_number, &text);
}