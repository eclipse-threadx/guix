/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_execute_resources.h"
#include "all_widgets_execute_specifications.h"

extern GX_WINDOW *pGaugeScreen;
INT angle[6] = { -114, -90, 52, -27, 52, -114 };
int angle_index = 0;

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
    GX_CIRCULAR_GAUGE *gauge = &((GAUGE_SCREEN_CONTROL_BLOCK *)pGaugeScreen)->gauge_screen_gauge_1;
    INT tics;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        set_gauge_angle(gauge);
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_GAUGE, GX_EVENT_ANIMATION_COMPLETE):
        set_gauge_angle(gauge);

        tics = gauge->gx_circular_gauge_info.gx_circular_gauge_info_animation_delay;
        gx_system_timer_start((GX_WIDGET *)gauge, GX_CIRCULAR_GAUGE_TIMER, tics, tics);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}