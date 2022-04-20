/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_home_automation_resources.h"
#include "demo_guix_home_automation_specifications.h"

/* Define animation IDs. */
#define ANIMATION_ID_DRAG_SLIDE       GX_NEXT_ANIMATION_ID
#define ANIMATION_ID_PAGE_SLIDE       GX_NEXT_ANIMATION_ID + 1
#define ANIMATION_ID_FOOTER_HOME      GX_NEXT_ANIMATION_ID + 2
#define ANIMATION_ID_FOOTER_ADD       GX_NEXT_ANIMATION_ID + 3
#define ANIMATION_ID_FOOTER_SETTING   GX_NEXT_ANIMATION_ID + 4
#define ANIMATION_ID_SCREEN_CHILDREN  GX_NEXT_ANIMATION_ID + 5

/* Define maximum power value. */
#define MAX_POWER 11489

/* Define minimum radial slider angle that correspond to maximum power value. */
#define MIN_ANGLE -50

/* Define maximum radial slider angle that correspond to minumum power value. */
#define MAX_ANGLE 230

/* Define application information structure. */
typedef struct APP_INFO_STRUCT{
    GX_WIDGET *current_screen; /* Pointer to current screen. */
    GX_WIDGET *previous_screen; /* Pointer to previous screen. */
    INT        overall_energy_today; /* Overall energy used today. */
}APP_INFO;

VOID title_animation_start(CONTROLLER_BASE_CONTROL_BLOCK *base);

VOID toggle_screen(GX_WIDGET *new_screen);
VOID on_footer_menu_clicked(INT button_id);

VOID lights_screen_init();
VOID lights_title_init();
VOID lights_screen_reset();
VOID lights_screen_animation_start();
VOID lights_screen_animation_stop();
GX_RESOURCE_ID light_selected_map_id_get(GX_RADIAL_SLIDER *slider);

VOID thermostat_screen_init();
VOID thermostat_title_init();
VOID thermostat_screen_reset();
VOID thermostat_screen_animation_start();
VOID thermostat_screen_animation_stop();

VOID locks_screen_init();
VOID locks_title_init();
VOID locks_screen_reset();
VOID locks_screen_animation_start();
VOID locks_screen_animation_stop();

VOID weather_screen_init();
VOID weather_screen_reset();
VOID weather_screen_animation_start();
VOID weather_screen_animation_stop();

VOID pagination_button_enable_disable(GX_WINDOW *window, GX_BOOL enabled);