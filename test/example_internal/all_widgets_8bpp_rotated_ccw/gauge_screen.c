/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_8bpp_rotated_ccw_resources.h"
#include "all_widgets_8bpp_rotated_ccw_specifications.h"

extern GX_WINDOW *pGaugeScreen;
static INT angle[6] = { -114, -90, 52, -27, 52, -114 };
static int angle_index = 0;

VOID set_gauge_angle(GX_CIRCULAR_GAUGE *gauge)
{
    gx_circular_gauge_angle_set(gauge, angle[angle_index++]);
    if (angle_index >= (int)(sizeof(angle) / sizeof(INT)))
    {
        angle_index = 0;
    }
}

UINT gauge_event_handle(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    GX_CIRCULAR_GAUGE *gauge = &gauge_screen.gauge_screen_gauge_1;
    INT tics;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        set_gauge_angle(gauge);
        break;

    case GX_SIGNAL(ID_GAUGE, GX_EVENT_ANIMATION_COMPLETE):
        set_gauge_angle(gauge);

        tics = gauge->gx_circular_gauge_info.gx_circular_gauge_info_animation_delay;
        gx_system_timer_start((GX_WIDGET *)gauge, GX_CIRCULAR_GAUGE_TIMER, tics, tics);
        break;

    default:
        return next_button_handler(window, event_ptr);
    }
    return 0;
}