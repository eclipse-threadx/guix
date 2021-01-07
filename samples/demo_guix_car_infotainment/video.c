/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

#define VIDEO_PLAY_TIMER 0x20
#define TOTAL_FRAME      13
#define VIDEO_PLAY_TICKS 50

/* Define variables. */
INT         current_frame = 0;

/* Define prototypes. */
VOID video_start();
VOID video_stop();
VOID slider_value_update();
VOID time_stamp_update();

/******************************************************************************************/
/* Override default drawing of "video_screen" to do custom drawing.                       */
/******************************************************************************************/
VOID video_screen_draw(GX_WINDOW *window)
{
INT           xpos;
INT           ypos;
GX_RECTANGLE *size = &window->gx_widget_size;
GX_PIXELMAP  *pixelmap;

    if (decoded_map.gx_pixelmap_data)
    {
        /* Draw decoded jpg image. */
        gx_canvas_pixelmap_draw(size->gx_rectangle_left,
            size->gx_rectangle_top, &decoded_map);
    }

    /* Pick window filling pixelmap. */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_TRANSPARENT_DOT_50, &pixelmap);

    if (pixelmap)
    {
        /* Tile filling pixelmap. */
        for (ypos = size->gx_rectangle_top; ypos < 57; ypos += pixelmap->gx_pixelmap_height)
        {
            for (xpos = size->gx_rectangle_left; xpos < size->gx_rectangle_right; xpos += pixelmap->gx_pixelmap_width)
            {
                gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
            }
        }
    }

    /* Call default window draw. */
    gx_window_draw(window);
}

/******************************************************************************************/
/* Override default event processing of "video_screen" to handle signals from my child    */
/* widgets.                                                                               */
/******************************************************************************************/
UINT video_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Call base screen event process. */
        base_screen_event_process(window, event_ptr);

        /* Decode a jpg image. */
        jpg_decode(GX_PIXELMAP_ID_INTERNET_BG, &decoded_map);
        break;

    case GX_EVENT_HIDE:
        /* Stop video. */
        video_stop();

        /* Call base screen event process.  */
        base_screen_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_VIDEO_START, GX_EVENT_CLICKED):
        /* Start video. */
        video_start();
        break;

    case GX_SIGNAL(ID_BOTTOM_START, GX_EVENT_CLICKED):
        if (video_screen.video_screen_video_bottom_start.gx_pixelmap_button_normal_id == GX_PIXELMAP_ID_START)
        {
            /* Start video. */
            video_start();
        }
        else
        {
            /* Start_video. */
            video_stop();
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == VIDEO_PLAY_TIMER)
        {
            /* Increase current frame number. */
            current_frame++;

            /* Update slider value. */
            slider_value_update();
        }
        break;

    case GX_SIGNAL(ID_VIDEO_SLIDER, GX_EVENT_SLIDER_VALUE):
        current_frame = event_ptr->gx_event_payload.gx_event_intdata[0];

        /* Update time stamp. */
        time_stamp_update();
        break;

    case GX_SIGNAL(ID_REWIND, GX_EVENT_CLICKED):
        if (current_frame > 0)
        {
            /* Decrease current frame number. */
            current_frame--;

            /* Update slider value. */
            slider_value_update();
        }
        break;

    case GX_SIGNAL(ID_FORWARD, GX_EVENT_CLICKED):
        if (current_frame < TOTAL_FRAME)
        {
            /* Increase curremt frame number. */
            current_frame++;

            /* Update slider value. */
            slider_value_update();
        }
        break;

    default:
        /* Call base screen event process. */
        return base_screen_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Start play.                                                                            */
/******************************************************************************************/
VOID video_start()
{
    /* Start video play timer. */
    gx_system_timer_start((GX_WIDGET *)&video_screen, VIDEO_PLAY_TIMER, VIDEO_PLAY_TICKS, VIDEO_PLAY_TICKS);

    /* Detach big start button from video screen. */
    gx_widget_detach(&video_screen.video_screen_video_start);

    /* Reset pixelmap of start/pause button in bottom. */
    gx_pixelmap_button_pixelmap_set(&video_screen.video_screen_video_bottom_start,
        video_screen.video_screen_video_bottom_start.gx_pixelmap_button_selected_id,
        video_screen.video_screen_video_bottom_start.gx_pixelmap_button_normal_id,
        GX_NULL);
}

/******************************************************************************************/
/* Stop play.                                                                             */
/******************************************************************************************/
VOID video_stop()
{
    /* Stop video play timer. */
    gx_system_timer_stop((GX_WIDGET *)&video_screen, VIDEO_PLAY_TIMER);

    /* Attach big start button to video screen. */
    gx_widget_attach((GX_WIDGET *)&video_screen, (GX_WIDGET *)&video_screen.video_screen_video_start);

    /* Reset pixelmap of start/pause button in bottom. */
    gx_pixelmap_button_pixelmap_set(&video_screen.video_screen_video_bottom_start,
        video_screen.video_screen_video_bottom_start.gx_pixelmap_button_selected_id,
        video_screen.video_screen_video_bottom_start.gx_pixelmap_button_normal_id,
        GX_NULL);
}

/******************************************************************************************/
/* Update slider value with frame number.                                                 */
/******************************************************************************************/
VOID slider_value_update()
{
    if (current_frame > TOTAL_FRAME)
    {
        current_frame = 0;
    }

    gx_slider_value_set((GX_SLIDER *)&video_screen.video_screen_video_slider,
        (GX_SLIDER_INFO *)&video_screen.video_screen_video_slider.gx_slider_info, current_frame);
}

/******************************************************************************************/
/* Update time stamp with frame number.                                                   */
/******************************************************************************************/
VOID time_stamp_update()
{
GX_CHAR   text[3];
GX_STRING string;

    /* Convert from integer to string. */
    gx_utility_ltoa(current_frame, text, 3);

    if (current_frame < 10)
    {
        text[1] = text[0];
        text[0] = '0';
        text[2] = '\0';
    }

    string.gx_string_ptr = text;
    string.gx_string_length = 2;

    gx_prompt_text_set_ext(&video_screen.video_screen_play_minute, &string);
}
