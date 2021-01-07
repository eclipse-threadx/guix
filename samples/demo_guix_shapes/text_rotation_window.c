/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_shapes_resources.h"
#include "guix_shapes_specifications.h"

extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

#define           ROTATION_TIMER 1
#define           SHIFT_TIMER    2
#define           COUNT_TIMER    3
#define           MAX_DYNAMIC_TEXT_LENGTH 20

GX_RESOURCE_ID    text_color = GX_COLOR_ID_RED;
GX_BOOL           auto_rotate = GX_FALSE;
GX_BOOL           auto_shift = GX_FALSE;
GX_PIXELMAP       Alphamap_8bit_font;
GX_PIXELMAP       Alphamap_4bit_font;
GX_PIXELMAP       Alphamap_1bit_font;
GX_PIXELMAP       Alphamap_8bit_dynamic;
INT               text_angle = 30;
INT               xshift_8bpp = 10;
INT               yshift_8bpp = 10;
INT               xshift_4bpp = 5;
INT               yshift_4bpp = 5;
INT               xshift_1bpp = 5;
INT               yshift_1bpp = 5;
INT               xshift_dynamic = 1;
INT               yshift_dynamic = 1;
INT               dynamic_count = 123456789;
GX_CHAR           dynamic_text[MAX_DYNAMIC_TEXT_LENGTH + 1];
extern GX_WINDOW *pTextWindow;
extern VOID       memory_free(VOID *mem);

VOID RenderTextToAlphamaps(GX_WIDGET *widget)
{
    GX_FONT *font;
    GX_STRING string;

    gx_widget_font_get(widget, GX_FONT_ID_SCREEN_LABEL, &font);
    gx_widget_string_get_ext(widget, GX_STRING_ID_HELLO_WORLD, &string);
    gx_utility_string_to_alphamap_ext(&string, font, &Alphamap_8bit_font);

    gx_widget_font_get(widget, GX_FONT_ID_SMALL_BOLD, &font);
    gx_widget_string_get_ext(widget, GX_STRING_ID_FONT_4BPP, &string);
    gx_utility_string_to_alphamap_ext(&string, font, &Alphamap_4bit_font);

    gx_widget_font_get(widget, GX_FONT_ID_MONO_FONT, &font);
    gx_widget_string_get_ext(widget, GX_STRING_ID_FONT_1BPP, &string);
    gx_utility_string_to_alphamap_ext(&string, font, &Alphamap_1bit_font);
}

void text_window_draw(GX_WINDOW *window)
{
    INT xpos = 0;
    INT ypos = 0;

    GX_PIXELMAP rotated_text;
    GX_PIXELMAP *source_map = GX_NULL;
    GX_STRING string;

    gx_window_draw(window);

    gx_context_line_color_set(text_color);
    gx_context_fill_color_set(text_color);

    switch (window->gx_widget_id)
    {
    case TEXT_WIN_8BPP:
        source_map = &Alphamap_8bit_font;
        break;

    case TEXT_WIN_4BPP:
        source_map = &Alphamap_4bit_font;
        break;

    case TEXT_WIN_1BPP:
        source_map = &Alphamap_1bit_font;
        break;
    }

    xpos = (window -> gx_widget_size.gx_rectangle_left + window -> gx_widget_size.gx_rectangle_right) / 2;
    ypos = (window -> gx_widget_size.gx_rectangle_top + window -> gx_widget_size.gx_rectangle_bottom) / 2;

    if (source_map)
    {
        if (text_angle)
        {
            if (gx_utility_pixelmap_rotate(source_map, text_angle, &rotated_text, &xpos, &ypos) == GX_SUCCESS)
            {
                xpos = (window -> gx_widget_size.gx_rectangle_left + window -> gx_widget_size.gx_rectangle_right) / 2;
                ypos = (window -> gx_widget_size.gx_rectangle_top + window -> gx_widget_size.gx_rectangle_bottom) / 2;
                xpos -= rotated_text.gx_pixelmap_width >> 1;
                ypos -= rotated_text.gx_pixelmap_height >> 1;
                gx_canvas_pixelmap_draw(xpos, ypos, &rotated_text);
                memory_free((VOID *)rotated_text.gx_pixelmap_data);
            }
        }
        else
        {
            xpos -= source_map->gx_pixelmap_width >> 1;
            ypos -= source_map->gx_pixelmap_height >> 1;
            gx_canvas_pixelmap_draw(xpos, ypos, source_map);
        }
    }
    else
    {
        gx_context_font_set(GX_FONT_ID_SMALL_BOLD);
        gx_utility_ltoa(dynamic_count, dynamic_text, MAX_DYNAMIC_TEXT_LENGTH + 1);
        string.gx_string_ptr = dynamic_text;
        string.gx_string_length = string_length_get(dynamic_text, MAX_DYNAMIC_TEXT_LENGTH);
        gx_canvas_rotated_text_draw_ext(&string, xpos, ypos, text_angle);
    }
}


VOID ShiftText(GX_WINDOW *window)
{
INT         *xshift;
INT         *yshift;
GX_RECTANGLE size = Text_Rotaion_Window.Text_Rotaion_Window_Text_Window.gx_widget_size;

    switch (window -> gx_widget_id)
    {
    case TEXT_WIN_8BPP:
        xshift = &xshift_8bpp;
        yshift = &yshift_8bpp;
        break;

    case TEXT_WIN_4BPP:
        xshift = &xshift_4bpp;
        yshift = &yshift_4bpp;
        break;

    case TEXT_WIN_1BPP:
        xshift = &xshift_1bpp;
        yshift = &yshift_1bpp;
        break;

    case DYNAMIC_TEXT_WINDOW:
        xshift = &xshift_dynamic;
        yshift = &yshift_dynamic;
        gx_system_dirty_mark(&Text_Rotaion_Window.Text_Rotaion_Window_dynamic_text_window);
        break;
    }

    if (*xshift || *yshift)
    {
        gx_widget_shift(window, *xshift, *yshift, GX_TRUE);

        if (*xshift > 0)
        {
            if (window->gx_widget_size.gx_rectangle_right >= size.gx_rectangle_right)
            {
                *xshift = -*xshift;
            }
        }
        else
        {
            if (window->gx_widget_size.gx_rectangle_left <= 0)
            {
                *xshift = -*xshift;
            }
        }
        if (*yshift > 0)
        {
            if (window->gx_widget_size.gx_rectangle_bottom >= size.gx_rectangle_bottom)
            {
                *yshift = -*yshift;
            }
        }
        else
        {
            if (window->gx_widget_size.gx_rectangle_top <= 0)
            {
                *yshift = -*yshift;
            }
        }
    }
}

VOID ShiftAll(VOID)
{
    if (auto_shift)
    {
        ShiftText(&Text_Rotaion_Window.Text_Rotaion_Window_text_window_8bpp);
        ShiftText(&Text_Rotaion_Window.Text_Rotaion_Window_text_window_4bpp);
        ShiftText(&Text_Rotaion_Window.Text_Rotaion_Window_text_window_1bpp);
        ShiftText(&Text_Rotaion_Window.Text_Rotaion_Window_dynamic_text_window);
    }
}

VOID update_text_value(INT value)
{
static GX_CHAR str_value[8];
GX_STRING string;
GX_PROMPT *prompt;

    prompt = (GX_PROMPT *)&Text_Rotaion_Window.Text_Rotaion_Window_rotation_angle_value;
    gx_utility_ltoa(value, str_value, 10);
    string.gx_string_ptr = str_value;
    string.gx_string_length = string_length_get(str_value, sizeof(str_value) - 1);
    gx_prompt_text_set_ext((GX_PROMPT *)prompt, &string);
}

UINT Text_Rotation_Window_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
INT pos;
GX_PIXELMAP_SLIDER *slider;
GX_SLIDER_INFO     *info;

    switch (myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_system_timer_start(&Text_Rotaion_Window.Text_Rotaion_Window_dynamic_text_window, COUNT_TIMER, 5, 5);
        gx_window_event_process(window, myevent);
        RenderTextToAlphamaps((GX_WIDGET *)window);
        break;

    case GX_EVENT_HIDE:
        gx_system_timer_stop(&Text_Rotaion_Window.Text_Rotaion_Window_dynamic_text_window, COUNT_TIMER);
        gx_window_event_process(window, myevent);
        break;

    case GX_SIGNAL(ID_RED_TEXT, GX_EVENT_RADIO_SELECT):
        text_color = GX_COLOR_ID_RED;
        gx_system_dirty_mark(&Text_Rotaion_Window.Text_Rotaion_Window_Text_Window);
        break;

    case GX_SIGNAL(ID_BLUE_TEXT, GX_EVENT_RADIO_SELECT):
        text_color = GX_COLOR_ID_BLUE;
        gx_system_dirty_mark(&Text_Rotaion_Window.Text_Rotaion_Window_Text_Window);
        break;

    case GX_SIGNAL(ID_GREEN_TEXT, GX_EVENT_RADIO_SELECT):
        text_color = GX_COLOR_ID_GREEN;
        gx_system_dirty_mark(&Text_Rotaion_Window.Text_Rotaion_Window_Text_Window);
        break;

    case GX_SIGNAL(ID_AUTO_ROTATE, GX_EVENT_TOGGLE_ON):
        auto_rotate= GX_TRUE;
        gx_system_timer_start(window, ROTATION_TIMER, 5, 5);
        break;

    case GX_SIGNAL(ID_AUTO_ROTATE, GX_EVENT_TOGGLE_OFF):
        auto_rotate = GX_FALSE;
        gx_system_timer_stop(window, ROTATION_TIMER);
        break;

    case GX_SIGNAL(ID_AUTO_SHIFT, GX_EVENT_TOGGLE_ON):
        auto_shift = GX_TRUE;
        gx_system_timer_start(window, SHIFT_TIMER, 5, 5);
        break;

    case GX_SIGNAL(ID_AUTO_SHIFT, GX_EVENT_TOGGLE_OFF):
        auto_shift = GX_FALSE;
        gx_system_timer_stop(window, SHIFT_TIMER);
        break;

    case GX_SIGNAL(ID_ROTATION_ANGLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        pos = myevent->gx_event_payload.gx_event_longdata;
        text_angle = pos;
        update_text_value(pos);
        gx_system_dirty_mark(&Text_Rotaion_Window.Text_Rotaion_Window_Text_Window);
        break;

    case GX_EVENT_TIMER:
        switch (myevent->gx_event_payload.gx_event_timer_id)
        {
        case COUNT_TIMER:
            dynamic_count += 17;
            break;

        case ROTATION_TIMER:
            text_angle += 5;
            if (text_angle >= 360)
            {
                text_angle = 0;
            }
            slider = &Text_Rotaion_Window.Text_Rotaion_Window_Rotation_Angle_slider;
            info = &slider -> gx_slider_info;
            update_text_value(text_angle);
            gx_slider_value_set((GX_SLIDER *)slider, info, text_angle);

            if (auto_shift)
            {
                ShiftAll();
            }
            break;

        case SHIFT_TIMER:
            if (auto_rotate) return 0;
            else             ShiftAll();
            break;
        }
        gx_system_dirty_mark(&Text_Rotaion_Window.Text_Rotaion_Window_Text_Window);
        break;

    default:
        return gx_window_event_process(window, myevent);
    }
    return 0;
}
