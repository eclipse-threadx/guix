
#include <stdio.h>
#include "gx_api.h"

#include "guix_smart_watch_resources.h"
#include "guix_smart_watch_specifications.h"

#define SYSTEM_CLOCK_TIMER_ID              1
#define SCREEN_CLOCK_TIMER_ID               2
#define FITNESS_SCREEN_CHART_TIMER_ID      3
#define SCREEN_ANIMATION_TIMER_ID          4

#define SCREEN_DRAG_ANIMATION_ID 1
#define CLOCK_SLIDE_ANIMATION_ID  2
#define SCREEN_SLIDE_ANIMATION_ID 3

#define USER_EVENT_ANIMATION_START GX_FIRST_APP_EVENT
#define USER_EVENT_ANIMATION_STOP  GX_FIRST_APP_EVENT + 1

typedef struct SCREEN_INFO_STRUCT {
    GX_WIDGET* screen;
    GX_RESOURCE_ID title_icon_id;
    GX_RESOURCE_ID title_text_id;
    GX_BOOL title_transparency;
}SCREEN_INFO;

typedef struct TIME_STRUCT {
    INT month;
    INT day;
    INT day_of_week;
    INT hour;
    INT minute;
    INT second;
} TIME;

VOID *memory_allocate(ULONG size);
VOID  memory_free(VOID *mem);

VOID clear_screen_clock_record();
VOID screen_clock_update(GX_NUMERIC_PROMPT* hour, GX_NUMERIC_PROMPT* minute, GX_PROMPT* second);
VOID page_screen_slide(GX_WIDGET *current_screen, GX_WIDGET *target_screen);
VOID container_screen_slide(GX_WIDGET *current_screen, GX_WIDGET *target_screen);
VOID screens_initialize();
VOID fitness_screen_initialize();
VOID watch_screen_initialize();
VOID music_play_on();
VOID music_play_off();
VOID music_play_progress_update();
VOID weather_screen_music_progress_update();

int get_calorie_burned_progress_val();
int get_running_progress_val();
int get_running_steps();
int get_running_distance();
int get_standing_progress_val();
int get_ekg_progress_val();