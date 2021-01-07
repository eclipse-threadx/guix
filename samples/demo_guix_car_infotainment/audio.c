/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"
#include "custom_slider.h"

#define ID_WAVE_SLIDER 100

/* Define variables. */
GX_PIXELMAP_BUTTON *top_button_list[] = {
    &audio_screen.audio_screen_btn_disc,
    &audio_screen.audio_screen_btn_radio,
    &audio_screen.audio_screen_btn_mp3,
    &audio_screen.audio_screen_btn_bluetooth,
    &audio_screen.audio_screen_btn_aux,
    &audio_screen.audio_screen_btn_pandora,
    GX_NULL
};

typedef struct CHANNEL_INFO_ENTRY_STRUCT{
    GX_STRING  channel_name;
    INT        channel_id;
    GX_BUTTON *button;
    GX_PROMPT *prompt;
    INT        wave_value;
}CHANNEL_INFO_ENTRY;

CHANNEL_INFO_ENTRY fm_channel_info_list[] = {
    { {"KQED", 4},ID_CHANNEL_1, (GX_BUTTON*)&audio_screen.audio_screen_channel_1, &audio_screen.audio_screen_channel_1_label, 885 },
    { {"KRZZ", 4},ID_CHANNEL_2, (GX_BUTTON*)&audio_screen.audio_screen_channel_2, &audio_screen.audio_screen_channel_2_label, 933 },
    { {"KLLC", 4},ID_CHANNEL_3, (GX_BUTTON *)&audio_screen.audio_screen_channel_3, &audio_screen.audio_screen_channel_3_label, 973 },
    { {"KISQ", 4},ID_CHANNEL_4, (GX_BUTTON *)&audio_screen.audio_screen_channel_4, &audio_screen.audio_screen_channel_4_label, 981 },
    { {"KKSF", 4},ID_CHANNEL_5, (GX_BUTTON *)&audio_screen.audio_screen_channel_5, &audio_screen.audio_screen_channel_5_label, 1013 },
    { {0,0}, 0, 0, 0, 0}
};

CHANNEL_INFO_ENTRY am_channel_info_list[] = {
    { {"KSPE", 4},ID_CHANNEL_1, (GX_BUTTON *)&audio_screen.audio_screen_channel_1, &audio_screen.audio_screen_channel_1_label, 5600 },
    { {"KLAC", 4},ID_CHANNEL_2, (GX_BUTTON *)&audio_screen.audio_screen_channel_2, &audio_screen.audio_screen_channel_2_label, 5700 },
    { {"KLAA", 4},ID_CHANNEL_3, (GX_BUTTON *)&audio_screen.audio_screen_channel_3, &audio_screen.audio_screen_channel_3_label, 8300 },
    { {"KLSD", 4},ID_CHANNEL_4, (GX_BUTTON *)&audio_screen.audio_screen_channel_4, &audio_screen.audio_screen_channel_4_label, 13600 },
    { {"KSPE", 4},ID_CHANNEL_5, (GX_BUTTON *)&audio_screen.audio_screen_channel_5, &audio_screen.audio_screen_channel_5_label, 14900 },
    { {0, 0}, 0, 0, 0, 0 }
};

CHANNEL_INFO_ENTRY *channel_info_list = am_channel_info_list;
CUSTOM_SLIDER       custom_slider;
CUSTOM_SLIDER_INFO  fm_slider_info;
CUSTOM_SLIDER_INFO  am_slider_info;

INT fm_current_selected_channel_index = 1;
INT am_current_selected_channel_index = 1;
INT current_selected_channel_index = 1;

/* Define prototypes. */
VOID wave_slider_create();
VOID wave_value_update(int wave);
VOID channel_button_selected(int ID);
VOID channel_button_text_update();
VOID bottom_button_selected();

/******************************************************************************************/
/* Override default drawing of "audio_screen" to do custom drawing.                       */
/******************************************************************************************/
VOID audio_screen_draw(GX_WINDOW *window)
{
INT                 x_start;
INT                 x_end;
INT                 y;
INT                 index = 0;
GX_RECTANGLE       *size;
GX_PIXELMAP_BUTTON *button;

    /* Call default window draw. */
    gx_window_draw(window);

    /* Set line color. */
    gx_context_line_color_set(GX_COLOR_ID_DARK_GRAY_LUM_43);

    /* Set brush width. */
    gx_context_brush_width_set(1);

    button = top_button_list[index++];
    x_start = button->gx_widget_size.gx_rectangle_left;
    y = button->gx_widget_size.gx_rectangle_bottom;

    /* Draw lines according to selected button. */
    while (button)
    {
        size = &button->gx_widget_size;
        x_end = size->gx_rectangle_right;

        if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            gx_canvas_line_draw(x_start, y, size->gx_rectangle_left, y);

            gx_canvas_line_draw(size->gx_rectangle_left, size->gx_rectangle_bottom, size->gx_rectangle_left, size->gx_rectangle_top);
            gx_canvas_line_draw(size->gx_rectangle_left, size->gx_rectangle_top, size->gx_rectangle_right, size->gx_rectangle_top);
            gx_canvas_line_draw(size->gx_rectangle_right, size->gx_rectangle_top, size->gx_rectangle_right, size->gx_rectangle_bottom);
            x_start = size->gx_rectangle_right;
        }

        button = top_button_list[index++];
    }

    gx_canvas_line_draw(x_start, y, x_end, y);
}

/******************************************************************************************/
/* Override default drawing of menu buttons in "audio_screen" to do some custom drawing.  */
/******************************************************************************************/
VOID audio_screen_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button)
{
GX_WIDGET   *child = button->gx_widget_first_child;
GX_PIXELMAP *map;
GX_STRING    text;

    while (child)
    {
        if (child->gx_widget_type == GX_TYPE_ICON)
        {
            if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
            {
                gx_context_pixelmap_get(((GX_ICON *)child)->gx_icon_selected_pixelmap, &map);
            }
            else
            {
                gx_context_pixelmap_get(((GX_ICON *)child)->gx_icon_normal_pixelmap, &map);
            }

            if (map)
            {
                /* Draw button icon. */
                gx_canvas_pixelmap_draw(child->gx_widget_size.gx_rectangle_left, child->gx_widget_size.gx_rectangle_top, map);
            }
        }
        else if (child->gx_widget_type == GX_TYPE_PROMPT)
        {
            if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
            {
                gx_context_line_color_set(((GX_PROMPT *)child)->gx_prompt_selected_text_color);
            }
            else
            {
                gx_context_line_color_set(((GX_PROMPT *)child)->gx_prompt_normal_text_color);
            }

            /* Set text font. */
            gx_context_font_set(((GX_PROMPT *)child)->gx_prompt_font_id);

            /* Get text. */
            gx_prompt_text_get_ext((GX_PROMPT *)child, &text);

            if (text.gx_string_ptr)
            {
                /* Draw button text. */
                gx_canvas_text_draw_ext(child->gx_widget_size.gx_rectangle_left, child->gx_widget_size.gx_rectangle_top, &text);
            }
        }

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Overried default event processing of "audio_screen" to handle signals from my child    */
/* widgets.                                                                               */
/******************************************************************************************/
UINT audio_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BTN_RADIO,GX_EVENT_RADIO_SELECT):
        /* Attach radio window to audio screen. */
        gx_widget_attach((GX_WIDGET *)window, (GX_WIDGET *)&audio_screen.audio_screen_radio_window);
        break;

    case GX_SIGNAL(ID_BTN_RADIO, GX_EVENT_RADIO_DESELECT):
        /* Detach radio window from audio screen. */
        gx_widget_detach((GX_WIDGET *)&audio_screen.audio_screen_radio_window);
        break;
    default:
        return base_screen_event_process(window, event_ptr);
    }

    return 0;
}


/******************************************************************************************/
/* Override default event processing of "radio_window" to handle signals from my child    */
/* widget.                                                                                */
/******************************************************************************************/
UINT radio_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_button_select(channel_info_list[2].button);

        /* Call default event process. */
        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_HIDE:
        gx_button_select(channel_info_list[0].button);

        /* Call default event process. */
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_BUTTOM_AM, GX_EVENT_RADIO_SELECT):
        if (!audio_screen.audio_screen_am_fm_window.gx_widget_parent)
        {
            gx_widget_attach((GX_WIDGET *)window, (GX_WIDGET *)&audio_screen.audio_screen_am_fm_window);
        }

        bottom_button_selected();
        break;

    case GX_SIGNAL(ID_BUTTON_FM, GX_EVENT_RADIO_SELECT):

        if (!audio_screen.audio_screen_am_fm_window.gx_widget_parent)
        {
            gx_widget_attach((GX_WIDGET *)window, (GX_WIDGET *)&audio_screen.audio_screen_am_fm_window);
        }

        bottom_button_selected();
        break;

    case GX_SIGNAL(ID_BUTTON_SAT, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BUTTON_SCAN, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BUTTON_SLIDER, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_BUTTON_INFO, GX_EVENT_RADIO_SELECT):
        gx_widget_detach((GX_WIDGET *)&audio_screen.audio_screen_am_fm_window);
        bottom_button_selected();
        break;

    case GX_SIGNAL(ID_CHANNEL_1, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_CHANNEL_2, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_CHANNEL_3, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_CHANNEL_4, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_CHANNEL_5, GX_EVENT_RADIO_SELECT):
        channel_button_selected(event_ptr->gx_event_type >> 8);
        break;

    case GX_SIGNAL(ID_BUTTON_LEFT, GX_EVENT_CLICKED):
        /* Calcualte current selected channel index.*/
        if (current_selected_channel_index == 0)
        {
            current_selected_channel_index = 4;
        }
        else
        {
            current_selected_channel_index--;
        }

        /* Select channel button manually. */
        gx_button_select(channel_info_list[current_selected_channel_index].button);
        break;

    case GX_SIGNAL(ID_BUTTON_RIGHT, GX_EVENT_CLICKED):
        /* Calulate current selected channel index. */
        if (current_selected_channel_index == 4)
        {
            current_selected_channel_index = 0;
        }
        else
        {
            current_selected_channel_index++;
        }

        /* Select channel button manually. */
        gx_button_select(channel_info_list[current_selected_channel_index].button);
        break;

    case USER_EVENT_SLIDER_VALUE:
        /* Update wave value. */
        wave_value_update(event_ptr->gx_event_payload.gx_event_intdata[0]);
        break;

    default:
        /* Call base seceen event process function. */
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Create a custom wave slider.                                                           */
/******************************************************************************************/
VOID wave_slider_create()
{
GX_RECTANGLE size;

    gx_utility_rectangle_define(&size, 140, 181, 628, 250);

    /* Define AM slider information. */
    am_slider_info.needle_pixelmap = GX_PIXELMAP_ID_MEASURE_WAVES_SLIDER;
    am_slider_info.current_value = 10300;
    am_slider_info.min_value = 5000;
    am_slider_info.max_value = 16000;
    am_slider_info.min_travel = 7;
    am_slider_info.max_travel = 7;
    am_slider_info.tickmarks_xoffset = 0;
    am_slider_info.tickmarks_yoffset = 10;
    am_slider_info.needle_xoffset = -7;
    am_slider_info.needle_yoffset = 0;
    am_slider_info.increment = 50;

    /* Define FM slider information. */
    fm_slider_info.needle_pixelmap = GX_PIXELMAP_ID_MEASURE_WAVES_SLIDER;
    fm_slider_info.current_value = 885;
    fm_slider_info.min_value = 880;
    fm_slider_info.max_value = 1030;
    fm_slider_info.min_travel = 7;
    fm_slider_info.max_travel = 7;
    fm_slider_info.tickmarks_xoffset = 0;
    fm_slider_info.tickmarks_yoffset = 10;
    fm_slider_info.needle_xoffset = -7;
    fm_slider_info.needle_yoffset = 0;
    fm_slider_info.increment = 1;

    custom_slider_create(&custom_slider, "wave slider", (GX_WIDGET *)&audio_screen.audio_screen_am_fm_window,
        &am_slider_info, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, ID_WAVE_SLIDER, &size);
}

/******************************************************************************************/
/* Update wave slider value.                                                              */
/******************************************************************************************/
VOID wave_value_update(int wave)
{
GX_CHAR   text_buffer[10];
GX_STRING text;
INT       index;

    gx_utility_ltoa(wave / 10, text_buffer, 10);
    index = string_length_get(text_buffer, sizeof(text_buffer) - 1);

    if (wave % 10)
    {
        text_buffer[index] = '.';
        gx_utility_ltoa(wave % 10, text_buffer + index + 1, 5);
    }

    text.gx_string_ptr = text_buffer;
    text.gx_string_length = string_length_get(text_buffer, sizeof(text_buffer) - 1);

    gx_prompt_text_set_ext(&audio_screen.audio_screen_wave_value, &text);
}

/******************************************************************************************/
/* Update some values when channel button is selected.                                    */
/******************************************************************************************/
VOID channel_button_selected(int ID)
{
CHANNEL_INFO_ENTRY *info = channel_info_list;

    current_selected_channel_index = 0;

    while (info->channel_name.gx_string_ptr)
    {
        if (info->channel_id == ID)
        {
            /* Update wave value. */
            wave_value_update(info->wave_value);

            /* Update slider value. */
            custom_slider_value_set(&custom_slider, info->wave_value);

            /* Update channel name. */
            gx_prompt_text_set_ext(&audio_screen.audio_screen_channel_name, &info->channel_name);
            break;
        }
        current_selected_channel_index++;
        info++;
    }
}

/******************************************************************************************/
/* Update channel button text value according to current select radio type.               */
/******************************************************************************************/
VOID channel_button_text_update()
{
CHANNEL_INFO_ENTRY *info = channel_info_list;
GX_CHAR             text_buffer[10];
GX_STRING           text;
INT                 index;

    while (info->channel_name.gx_string_ptr)
    {
        /* Convert integer to string. */
        gx_utility_ltoa(info->wave_value / 10, text_buffer, 10);
        index = string_length_get(text_buffer, sizeof(text_buffer) - 1);

        if (info->wave_value % 10)
        {
            text_buffer[index] = '.';
            gx_utility_ltoa(info->wave_value % 10, text_buffer + index + 1, 5);
        }

        text.gx_string_ptr = text_buffer;
        text.gx_string_length = string_length_get(text_buffer, sizeof(text_buffer) - 1);

        /* Reset channel button text value. */
        gx_prompt_text_set_ext(info->prompt, &text);

        info++;
    }

    /* Reset selected channel. */
    gx_button_select(channel_info_list[current_selected_channel_index].button);
}

/******************************************************************************************/
/* Update some values when bottom button is selected.                                     */
/******************************************************************************************/
VOID bottom_button_selected()
{
    if (channel_info_list == fm_channel_info_list)
    {
        /* Save FM selected channel index. */
        fm_current_selected_channel_index = current_selected_channel_index;

        /* Save FM slider information. */
        custom_slider_info_get(&custom_slider, &fm_slider_info);

        gx_widget_style_remove((GX_WIDGET *)channel_info_list[current_selected_channel_index].button, GX_STYLE_BUTTON_PUSHED);
    }
    else
    {
        /* Save AM selected channel index. */
        am_current_selected_channel_index = current_selected_channel_index;

        /* Save AM slider information. */
        custom_slider_info_get(&custom_slider, &am_slider_info);

        gx_widget_style_remove((GX_WIDGET *)channel_info_list[current_selected_channel_index].button, GX_STYLE_BUTTON_PUSHED);
    }

    if (audio_screen.audio_screen_button_am.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        /* Set AM selected channel index. */
        current_selected_channel_index = am_current_selected_channel_index;

        /* Set AM channel information list. */
        channel_info_list = am_channel_info_list;

        /* Set AM slider information. */
        custom_slider_info_set(&custom_slider, &am_slider_info);
    }

    if (audio_screen.audio_screen_button_fm.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        /* Set FM selected channel index. */
        current_selected_channel_index = fm_current_selected_channel_index;

        /* Set FM channel information list. */
        channel_info_list = fm_channel_info_list;

        /* Set FM slider information. */
        custom_slider_info_set(&custom_slider, &fm_slider_info);
    }

    /* Update channel text. */
    channel_button_text_update();
}
