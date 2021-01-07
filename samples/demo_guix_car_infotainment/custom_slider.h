/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

#define USER_TYPE_CUSTOM_SLIDER GX_FIRST_USER_WINDOW_TYPE
#define USER_EVENT_SLIDER_VALUE GX_FIRST_APP_EVENT

/* Define custom slider info structure. */
typedef struct CUSTOM_SLIDER_INFO_STRUCT{
    GX_RESOURCE_ID needle_pixelmap;
    INT current_value;
    INT tickmarks_xoffset;
    INT tickmarks_yoffset;
    INT needle_xoffset;
    INT needle_yoffset;
    INT min_value;
    INT max_value;
    INT min_travel;
    INT max_travel;
    INT increment;
}CUSTOM_SLIDER_INFO;

/* Define custom slider structure. */
typedef struct CUSTOM_SLIDER_STRUCT{
    GX_WIDGET_MEMBERS_DECLARE
    CUSTOM_SLIDER_INFO info;
    INT current_needle_xpos;
    INT start_needle_xpos;
    INT target_needle_xpos;
    INT start_tickmarks_xoffset;
    INT target_tickmarks_xoffset;
    INT total_animation_steps;
    INT remain_animation_steps;
}CUSTOM_SLIDER;

/* Declare prototypes. */
UINT custom_slider_create(CUSTOM_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent, CUSTOM_SLIDER_INFO *info,
    ULONG style, USHORT id, GX_RECTANGLE *size);
VOID custom_slider_value_set(CUSTOM_SLIDER *slider, INT value);
VOID custom_slider_info_get(CUSTOM_SLIDER *slider, CUSTOM_SLIDER_INFO *info);
VOID custom_slider_info_set(CUSTOM_SLIDER *slider, CUSTOM_SLIDER_INFO *info);
VOID custom_slider_draw(CUSTOM_SLIDER *slider);
UINT custom_slider_event_process(CUSTOM_SLIDER *slider, GX_EVENT *event_ptr);