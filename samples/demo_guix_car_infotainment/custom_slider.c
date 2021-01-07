
#include "custom_slider.h"

/* Implement a slider with moving backgrounds. */

#define TICKMARKS_SPACE        10
#define SLIDER_ANIMATION_TIMER 10
#define TOTAL_ANIMATION_STEPS  30
#define TICKMARK_TEXT_WIDTH    35
#define TICKMARKS_SHIFT_SPACE  50

/******************************************************************************************/
/* Create a custom slider widget.                                                         */
/******************************************************************************************/
UINT custom_slider_create(CUSTOM_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent, CUSTOM_SLIDER_INFO *info,
    ULONG style, USHORT id, GX_RECTANGLE *size)
{
UINT status;

    /* Call default widget create function. */
    status = gx_widget_create((GX_WIDGET *)slider, name, parent, style, id, size);

    if (status == GX_SUCCESS)
    {
        slider->info = *info;
        slider->gx_widget_type = USER_TYPE_CUSTOM_SLIDER;
        slider->gx_widget_draw_function = (VOID(*)(GX_WIDGET *))custom_slider_draw;
        slider->gx_widget_event_process_function = (UINT(*)(GX_WIDGET *, GX_EVENT *event_ptr))custom_slider_event_process;

        slider->current_needle_xpos = slider->gx_widget_size.gx_rectangle_left + info->min_travel + info->tickmarks_xoffset;
        slider->current_needle_xpos += (info->current_value - info->min_value) * TICKMARKS_SPACE;
        slider->start_needle_xpos = slider->current_needle_xpos;
        slider->target_needle_xpos = slider->current_needle_xpos;
        slider->start_tickmarks_xoffset = info->tickmarks_xoffset;
        slider->target_tickmarks_xoffset = info->tickmarks_xoffset;
        slider->total_animation_steps = 0;
        slider->remain_animation_steps = 0;
    }

    return status;
}

/******************************************************************************************/
/* Draw a custom slider widget.                                                           */
/******************************************************************************************/
VOID custom_slider_draw(CUSTOM_SLIDER *slider)
{
GX_PIXELMAP        *map;
CUSTOM_SLIDER_INFO *info = &slider->info;
INT                 xpos;
INT                 ypos;
INT                 value;
GX_CHAR             text_buffer[10];
GX_STRING           text;
INT                 tickcount = 0;

    /* Draw tickmarks. */
    gx_context_line_color_set(GX_COLOR_ID_DARK_GRAY_LUM_43);
    gx_context_brush_width_set(1);
    gx_context_font_set(GX_FONT_ID_NORMAL);

    /* Calcualte the smallest slider value that is visible. */
    value = info->min_value;
    value -= info->tickmarks_xoffset / TICKMARKS_SPACE * info->increment;

    tickcount = (value - info->min_value) / info->increment;

    xpos = slider->gx_widget_size.gx_rectangle_left + info->min_travel + info->tickmarks_xoffset;
    xpos += tickcount * TICKMARKS_SPACE;
    ypos = slider->gx_widget_size.gx_rectangle_top + info->tickmarks_yoffset;

    text.gx_string_ptr = text_buffer;

    while (xpos < slider->gx_widget_size.gx_rectangle_right - info->max_travel)
    {
        if (tickcount % 10 == 0)
        {
            /* Draw long tickmark. */
            gx_canvas_line_draw(xpos, ypos, xpos, ypos + 40);

            /* Draw tickmark number value. */
            gx_utility_ltoa(value / 10, text_buffer, 10);
            text.gx_string_length = string_length_get(text_buffer, sizeof(text_buffer) - 1);
            gx_canvas_text_draw_ext(xpos + 5, ypos + 25, &text);
        }
        else
        {
            /* Draw short tickmark. */
            gx_canvas_line_draw(xpos, ypos, xpos, ypos + 10);
        }
        value += info->increment;
        tickcount++;

        if (value > info->max_value)
        {
            break;
        }
        xpos += TICKMARKS_SPACE;
    }

    /* Draw needle pixelmap. */
    gx_context_pixelmap_get(info->needle_pixelmap, &map);

    if (map)
    {
        xpos = slider->current_needle_xpos + info->needle_xoffset;
        ypos = slider->gx_widget_size.gx_rectangle_top - info->needle_yoffset;

        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
}

/******************************************************************************************/
/* Calcualte the target needle and tickmarks offset with the click position.              */
/******************************************************************************************/
VOID custom_slider_target_position_calculate(CUSTOM_SLIDER *slider, INT xpos)
{
GX_RECTANGLE       *size = &slider->gx_widget_size;
CUSTOM_SLIDER_INFO *info = &slider->info;
INT                 tickcount;
INT                 tickmarks_shift = 0;
INT                 tickmarks_length;
INT                 client_width;
INT                 target_value;
INT                 right_xoffset;

    if (xpos < size->gx_rectangle_left + info->min_travel ||
        xpos > size->gx_rectangle_right - info->max_travel)
    {
        return;
    }

    /* Calcualte client width. */
    client_width = size->gx_rectangle_right - size->gx_rectangle_left + 1;
    client_width -= info->min_travel;
    client_width -= info->max_travel;

    if (xpos < size->gx_rectangle_left + TICKMARKS_SHIFT_SPACE)
    {
        /* Need to move tickmarks right. */
        if (GX_ABS(info->tickmarks_xoffset) >= client_width - TICKMARKS_SHIFT_SPACE)
        {
            tickmarks_shift = (client_width - TICKMARKS_SHIFT_SPACE);
        }
        else
        {
            tickmarks_shift = -info->tickmarks_xoffset;
        }
    }
    else if (xpos > size->gx_rectangle_right - TICKMARKS_SHIFT_SPACE)
    {
        /* Need to move tickmarks left. */
        tickcount = (info->max_value - info->min_value) / info->increment;
        tickmarks_length = tickcount * TICKMARKS_SPACE + TICKMARK_TEXT_WIDTH;
        right_xoffset = tickmarks_length - (client_width - info->tickmarks_xoffset);

        if (right_xoffset >= client_width - TICKMARKS_SHIFT_SPACE)
        {
            tickmarks_shift = -(client_width - TICKMARKS_SHIFT_SPACE);
        }
        else
        {
            tickmarks_shift = -right_xoffset - TICKMARK_TEXT_WIDTH;
        }
    }

    /* Calcualte target tickmarks xoffset. */
    slider->target_tickmarks_xoffset = info->tickmarks_xoffset + tickmarks_shift;

    /* Calcualte target needle xpos. */
    if (xpos <= size->gx_rectangle_left + info->min_travel)
    {
        xpos = size->gx_rectangle_left + info->min_travel;
    }
    else if (xpos >= size->gx_rectangle_right - info->max_travel)
    {
        xpos = size->gx_rectangle_right - info->max_travel;
    }

    /* Calcualte target value. */
    tickcount = (xpos - size->gx_rectangle_left - info->min_travel - info->tickmarks_xoffset + (TICKMARKS_SPACE / 2)) / TICKMARKS_SPACE;
    target_value = tickcount * info->increment;
    target_value += info->min_value;

    if (target_value > info->max_value)
    {
        target_value = info->max_value;
    }

    /* Set new slider value. */
    info->current_value = target_value;

    /* Calcualte target needle xpos. */
    tickcount = (target_value - info->min_value) / info->increment;
    slider->target_needle_xpos = slider->gx_widget_size.gx_rectangle_left + info->min_travel + slider->target_tickmarks_xoffset;
    slider->target_needle_xpos +=  tickcount * TICKMARKS_SPACE;

    /* Set start needle xpos. */
    slider->start_needle_xpos = slider->current_needle_xpos;

    /* Set start tickmarks xoffset. */
    slider->start_tickmarks_xoffset = info->tickmarks_xoffset;

    /* Set animation steps. */
    slider->total_animation_steps = TOTAL_ANIMATION_STEPS;
    slider->remain_animation_steps = TOTAL_ANIMATION_STEPS;
}

/******************************************************************************************/
/* Set information from a custom slider widget.                                           */
/******************************************************************************************/
VOID custom_slider_info_set(CUSTOM_SLIDER *slider, CUSTOM_SLIDER_INFO *info)
{
    if (slider && info)
    {
        slider->info = *info;

        gx_system_dirty_mark((GX_WIDGET *)slider);
    }
}

/******************************************************************************************/
/* Get the information of a custom slider widget.                                         */
/******************************************************************************************/
VOID custom_slider_info_get(CUSTOM_SLIDER *slider, CUSTOM_SLIDER_INFO *info)
{
    if (slider && info)
    {
        *info = slider->info;
    }
}

/******************************************************************************************/
/* Caculate target needle and tickmarks offset with the new value, and start an animation */
/* to move the needle and tickmarks offset from current value to target value.            */
/******************************************************************************************/
VOID custom_slider_value_set(CUSTOM_SLIDER *slider, INT value)
{
CUSTOM_SLIDER_INFO *info = &slider->info;
INT                 visible_min_value;
INT                 visible_max_value;
INT                 tickmarks_shift = 0;
INT                 tickmarks_length;
INT                 right_xoffset;
INT                 client_width;
INT                 tickcount;

    /* Calculte min value that is visible. */
    tickcount = (-info->tickmarks_xoffset + (TICKMARKS_SPACE / 2)) / TICKMARKS_SPACE;
    visible_min_value = tickcount * info->increment;
    visible_min_value += info->min_value;

    /* Calculate max value that is visible. */
    client_width = slider->gx_widget_size.gx_rectangle_right - slider->gx_widget_size.gx_rectangle_left + 1;
    client_width -= info->min_travel;
    client_width -= info->max_travel;

    tickcount = (client_width - info->tickmarks_xoffset + (TICKMARKS_SPACE / 2)) / TICKMARKS_SPACE;
    visible_max_value = tickcount * info->increment;
    visible_max_value += info->min_value;

    if (value < visible_min_value)
    {
        /* Need to shift tickmarks right to make setted value visible. */
        if (value > info->min_value + 5 * info->increment)
        {
            tickcount = (visible_min_value - value + 5 * info->increment) / info->increment;
            tickmarks_shift = tickcount * TICKMARKS_SPACE;
        }
        else
        {
            tickmarks_shift = -info->tickmarks_xoffset;
        }
    }
    else if (value > visible_max_value)
    {
        /* Need to shift tickmarks left to make setted value visible. */
        if (value < info->max_value - 5 * info->increment)
        {
            tickcount = (value - visible_max_value + 5 * info->increment) / info->increment;
            tickmarks_shift = -tickcount * TICKMARKS_SPACE;
        }
        else
        {
            tickmarks_length = (info->max_value - info->min_value) * TICKMARKS_SPACE + TICKMARK_TEXT_WIDTH;
            right_xoffset = tickmarks_length - (client_width - info->tickmarks_xoffset);
            tickmarks_shift = -right_xoffset - TICKMARK_TEXT_WIDTH;
        }
    }

    /* Set target tickmarks xoffset value. */
    slider->target_tickmarks_xoffset = info->tickmarks_xoffset + tickmarks_shift;

    /* Set new slider value. */
    info->current_value = value;

    /* Calculate target needle xpos. */
    tickcount = (value - info->min_value) / info->increment;
    slider->target_needle_xpos = slider->gx_widget_size.gx_rectangle_left + info->min_travel + slider->target_tickmarks_xoffset;
    slider->target_needle_xpos += tickcount * TICKMARKS_SPACE;

    /* Set start needle xpos. */
    slider->start_needle_xpos = slider->current_needle_xpos;

    /* Set start tickmarks xoffset. */
    slider->start_tickmarks_xoffset = info->tickmarks_xoffset;

    /* Set animatin steps. */
    slider->total_animation_steps = TOTAL_ANIMATION_STEPS;
    slider->remain_animation_steps = TOTAL_ANIMATION_STEPS;

    /* Start aimation timer. */
    if (slider->remain_animation_steps)
    {
        gx_system_timer_start((GX_SLIDER *)slider, SLIDER_ANIMATION_TIMER, 1, 1);
    }
}

/******************************************************************************************/
/* Update a custom slider animation to move needle and tickmarks offset from current      */
/* value to target value.                                                                 */
/******************************************************************************************/
VOID custom_slider_animation_update(CUSTOM_SLIDER *slider)
{
GX_EVENT slider_value_event;

    slider->remain_animation_steps--;

    /* Udpate current needle pos. */
    slider->current_needle_xpos = (slider->target_needle_xpos - slider->start_needle_xpos);
    slider->current_needle_xpos *= (slider->total_animation_steps - slider->remain_animation_steps);
    slider->current_needle_xpos /= slider->total_animation_steps;
    slider->current_needle_xpos += slider->start_needle_xpos;

    /* Udpate current tickmarks offset. */
    slider->info.tickmarks_xoffset = (slider->target_tickmarks_xoffset - slider->start_tickmarks_xoffset);
    slider->info.tickmarks_xoffset *= (slider->total_animation_steps - slider->remain_animation_steps);
    slider->info.tickmarks_xoffset /= slider->total_animation_steps;
    slider->info.tickmarks_xoffset += slider->start_tickmarks_xoffset;

    if (slider->remain_animation_steps == 0)
    {
        /* Stop animation. */
        gx_system_timer_stop((GX_WIDGET *)slider, SLIDER_ANIMATION_TIMER);

        /* Set value set event. */
        memset(&slider_value_event, 0, sizeof(GX_EVENT));
        slider_value_event.gx_event_sender = slider->gx_widget_id;
        slider_value_event.gx_event_target = slider->gx_widget_parent;
        slider_value_event.gx_event_type = USER_EVENT_SLIDER_VALUE;
        slider_value_event.gx_event_payload.gx_event_intdata[0] = (int)slider->info.current_value;

        gx_system_event_fold(&slider_value_event);
    }

    /* Mark slider widget dirty. */
    gx_system_dirty_mark((GX_WIDGET *)slider);
}

/******************************************************************************************/
/* Handle incoming event for a custom slider widget.                                      */
/******************************************************************************************/
UINT custom_slider_event_process(CUSTOM_SLIDER *slider, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        slider->gx_widget_style &= GX_STYLE_BUTTON_PUSHED;
        custom_slider_target_position_calculate(slider, event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x);
        if (slider->remain_animation_steps)
        {
            gx_system_timer_start((GX_WIDGET *)slider, SLIDER_ANIMATION_TIMER, 1, 1);
        }
        break;

    case GX_EVENT_PEN_DRAG:
        break;

    case GX_EVENT_PEN_UP:
        if (slider->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            slider->gx_widget_style &= ~GX_STYLE_BUTTON_PUSHED;
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SLIDER_ANIMATION_TIMER)
        {
            custom_slider_animation_update(slider);
        }
        break;

    default:
        return gx_widget_event_process(slider, event_ptr);
    }

    return 0;
}
