
#include "demo_guix_washing_machine.h"

#define ARC_RANGE    33
#define ARC_INTERVAL 36
#define ARC_COUNT    6

typedef struct ARC_INFO_STRUCT {
    INT start_angle;
    INT end_angle;
}ARC_INFO;

ARC_INFO arc_info_list[ARC_COUNT + 1];

/******************************************************************************************/
/* Initialize some information used by wash cycle window.                                 */
/******************************************************************************************/
VOID wash_cycle_window_init(INT remain_seconds)
{
    INT index;
    GX_RADIAL_SLIDER_INFO* info = &wash_cycle_window.wash_cycle_window_wash_cycle_slider.gx_radial_slider_info;

    arc_info_list[0].end_angle = info->gx_radial_slider_info_max_angle;
    arc_info_list[0].start_angle = arc_info_list[0].end_angle - ARC_RANGE;

    for (index = 1; index < ARC_COUNT; index++)
    {
        arc_info_list[index].start_angle = arc_info_list[index - 1].start_angle - ARC_INTERVAL;
        arc_info_list[index].end_angle = arc_info_list[index - 1].end_angle - ARC_INTERVAL;
    }

    arc_info_list[ARC_COUNT].start_angle = 0;
    arc_info_list[ARC_COUNT].end_angle = 0;

    /* Update remain time.  */
    wash_cycle_window_remain_time_update(remain_seconds);
}

/******************************************************************************************/
/* Update remain time.                                                                    */
/******************************************************************************************/
VOID wash_cycle_window_remain_time_update(INT remain_seconds)
{
    gx_numeric_prompt_value_set(&wash_cycle_window.wash_cycle_window_minute, remain_seconds / 60);
    gx_numeric_prompt_value_set(&wash_cycle_window.wash_cycle_window_second, remain_seconds % 60);
}

/******************************************************************************************/
/* Update wash cycle progress bar value.                                                  */
/******************************************************************************************/
static VOID update_progress_bar_value(INT angle)
{
    GX_RADIAL_SLIDER_INFO* info = &wash_cycle_window.wash_cycle_window_wash_cycle_slider.gx_radial_slider_info;

    INT percent = (info->gx_radial_slider_info_max_angle - angle) * 100 /
                  (info->gx_radial_slider_info_max_angle - info->gx_radial_slider_info_min_angle);


    gx_progress_bar_value_set(&main_screen.main_screen_wash_cycle_progress_bar, percent);
}

/******************************************************************************************/
/* Override the default event processing of "wash_cycle_window" to handle signals from    */
/* my child widgets.                                                                      */
/******************************************************************************************/
UINT wash_cycle_window_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_WASH_CYCLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        update_progress_bar_value(event_ptr->gx_event_payload.gx_event_longdata);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Custom defined draw function for wash cycle window.                                    */
/******************************************************************************************/
VOID wash_cycle_window_draw(GX_WINDOW* window)
{
    gx_window_background_draw(window);

    if (main_screen.main_screen_btn_play.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        /* Draw animation wave in the center. */
        animation_wave_draw(window);
    }

    gx_widget_children_draw(window);
}

/******************************************************************************************/
/* Custom defined draw function for wash cycle slider widget.                             */
/******************************************************************************************/
VOID wash_cycle_slider_draw(GX_RADIAL_SLIDER* slider)
{
    GX_RADIAL_SLIDER_INFO *info = &slider->gx_radial_slider_info;
    GX_PIXELMAP *map;
    INT xpos;
    INT ypos;
    ARC_INFO *arc = arc_info_list;
    INT start_angle;

    /* Call default widget background draw. */
    gx_widget_background_draw((GX_WIDGET*)slider);

    gx_context_pixelmap_get(info->gx_radial_slider_info_background_pixelmap, &map);

    xpos = slider->gx_widget_size.gx_rectangle_left;
    ypos = slider->gx_widget_size.gx_rectangle_top;

    if (map)
    {
        /* Draw background pixelmap. */
        gx_canvas_pixelmap_draw(xpos, ypos, map);
    }

    gx_context_brush_width_set(4);
    gx_context_brush_define(GX_COLOR_ID_GREEN, GX_COLOR_ID_GREEN, GX_BRUSH_ALIAS|GX_BRUSH_ROUND);

    xpos += info->gx_radial_slider_info_xcenter;
    ypos += info->gx_radial_slider_info_ycenter;

    /* Fill selected area with disconnected arcs. */
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
            gx_canvas_arc_draw(xpos, ypos, info->gx_radial_slider_info_radius, start_angle, arc->end_angle);
        }

        if (start_angle == info->gx_radial_slider_info_current_angle)
        {
            break;
        }

        arc++;
    }

    /* Draw children widgets. */
    gx_widget_children_draw((GX_WIDGET*)slider);
}