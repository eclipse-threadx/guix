/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_car_infotainment_resources.h"
#include "demo_guix_car_infotainment_specifications.h"

/* Defime timer IDs.  */
#define TIMER_ID_SECOND                    1
#define TIMER_ID_PROGRESS_BAR_ANIMATION    2
#define TIMER_ID_CALL_DURATION             3
#define TIMER_ID_RADIAL_PROGRESS_ANIMATION 4
#define TIMER_ID_VIDEO_PLAY_PROGRESS_ANIMATION 5
#define TIMER_ID_CLIMATE_SCREEN_PROGRESS_ANIMATION 6
#define TIMER_ID_VEHICLE_NUMBER_ANIMATION 7
#define TIMER_ID_MAIN_SCREEN_NUMBER_ANIMATION 8
#define TIMER_ID_MAIN_SCREEN_SPEEDOMETER_ANIMATION 9

VOID ToggleScreen(GX_WINDOW* new_win);