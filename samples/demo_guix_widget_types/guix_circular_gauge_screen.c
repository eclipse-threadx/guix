/* This is a demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "guix_widget_types_resources.h"
#include "guix_widget_types_specifications.h"

#define MAX_TEXT_LENGTH 10

extern GX_WINDOW_ROOT    *root;

static GX_CHAR prompt_text_animated[MAX_TEXT_LENGTH + 1];
static GX_CHAR prompt_text_not_animated[MAX_TEXT_LENGTH + 1];
extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/******************************************************************************************/
void set_speed_value(int id, int value)
{
    GX_PROMPT *prompt;
    GX_STRING string;

    if (id == 1)
    {
        prompt = &(&Gauge_Screen)->Gauge_Screen_prompt_animated;
        string.gx_string_ptr = prompt_text_animated;
    }
    else
    {
        prompt = &(&Gauge_Screen)->Gauge_Screen_prompt_not_animated;
        string.gx_string_ptr = prompt_text_not_animated;
    }

    gx_utility_ltoa(value, (GX_CHAR *)string.gx_string_ptr, MAX_TEXT_LENGTH + 1);

    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_TEXT_LENGTH);
    gx_prompt_text_set_ext(prompt, &string);
}

/******************************************************************************************/
UINT gauge_window_event_handler(GX_WINDOW *win, GX_EVENT *event_ptr)
{
    int pos;
    GX_CIRCULAR_GAUGE *gauge;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_ANIMATED, GX_EVENT_SLIDER_VALUE):
        /* Change needle position. */
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        set_speed_value(1, pos);
        gx_widget_find(win, ID_GAUGE_ANIMATED, GX_SEARCH_DEPTH_INFINITE, &gauge);
        pos = pos * 22 / 10 - 109;
        gx_circular_gauge_angle_set(gauge, pos);
        break;

    case GX_SIGNAL(ID_SLIDER_NOT_ANIMATED, GX_EVENT_SLIDER_VALUE):
        /* Change needle position of gauge 1*/
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        set_speed_value(2, pos);
        gx_widget_find(win, ID_GAUGE_NOT_ANIMATED, GX_SEARCH_DEPTH_INFINITE, &gauge);
        pos =  pos * 22 / 10 - 109;
        gx_circular_gauge_angle_set(gauge, pos);
        break;

    default:
        return gx_window_event_process(win, event_ptr);
    }
    return 0;
}
