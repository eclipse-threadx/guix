
#include "demo_guix_home_automation.h"

/* Define controller information structure. */
typedef struct TITLE_INFO_STRUCT{
    CONTROLLER_BASE_CONTROL_BLOCK *base; /* Pointer of the controller widget. */
    GX_RESOURCE_ID title_id; /* Title of the controller. */
}TITLE_INFO;

/* "controller base" widget is a template that contains only one child to show title text.
   "locks door base" widget is a template that derived from "controller base", which contains
   other child widgets for lock settings. This is a list of lock controller widgets and title text
   to assign to those widgets. */
TITLE_INFO locks_title_list[] = {
    { &locks_page_1.locks_page_1_security_alarm, GX_STRING_ID_SECURITY_ALARM },
    { &locks_page_1.locks_page_1_front_door.base, GX_STRING_ID_FRONT_DOOR },
    { &locks_page_2.locks_page_2_patio_door.base, GX_STRING_ID_PATIO_DOOR },
    { &locks_page_2.locks_page_2_upper_deck_door.base, GX_STRING_ID_UPPER_DECK_DOOR },
    { &locks_page_3.locks_page_3_lower_deck_door.base, GX_STRING_ID_LOWER_DECK_DOOR },
    { &locks_page_3.locks_page_3_add_lock.base, GX_STRING_ID_ADD_LOCK },
    { GX_NULL, 0 }
};

/* "controller base" widget is a template that contains only one child to show title text.
   "lights base" widget is a template that derived from "controller base", which contains
    other child widgets for light settings. This is a list of light controller widgets and title text
    to assign to those widgets. */
TITLE_INFO lights_title_list[] = {
    { &lights_page_1.lights_page_1_kitchen.base, GX_STRING_ID_KITCHEN},
    { &lights_page_1.lights_page_1_master_bedroom.base, GX_STRING_ID_MASTER_BEDROOM },
    { &lights_page_2.lights_page_2_kids_bedroom.base, GX_STRING_ID_KIDS_BEDRROM },
    { &lights_page_2.lights_page_2_living_room.base, GX_STRING_ID_LIVING_ROOM },
    { &lights_page_3.lights_page_3_dinning_room.base, GX_STRING_ID_DINNING_ROOM },
    { &lights_page_3.lights_page_3_outdoor_patio.base, GX_STRING_ID_OUTDOOR_PATIO },
    { &lights_page_4.lights_page_4_office.base, GX_STRING_ID_OFFICE },
    { &lights_page_4.lights_page_4_add_light.base, GX_STRING_ID_ADD_LIGHT },
    { GX_NULL, 0 }
};

/* "controller base" widget is a template that contains only one child to show title text.
   "thermostat base" widget is a template that derived from "controller base", which contains
    other child widgets for thermostat settings. This is a list of thermostat controller widgets and title text
    to assign to those widgets. */
TITLE_INFO thermostat_title_list[] = {
    { &thermostat_page_1.thermostat_page_1_kitchen.base, GX_STRING_ID_KITCHEN },
    { &thermostat_page_1.thermostat_page_1_master_bedroom.base, GX_STRING_ID_MASTER_BEDROOM },
    { &thermostat_page_2.thermostat_page_2_kids_bedroom.base, GX_STRING_ID_KIDS_BEDRROM },
    { &thermostat_page_2.thermostat_page_2_living_room.base, GX_STRING_ID_LIVING_ROOM },
    { &thermostat_page_3.thermostat_page_3_dinning_room.base, GX_STRING_ID_DINNING_ROOM },
    { &thermostat_page_3.thermostat_page_3_outdoor_patio.base, GX_STRING_ID_OUTDOOR_PATIO },
    { &thermostat_page_4.thermostat_page_4_office.base, GX_STRING_ID_OFFICE },
    { &thermostat_page_4.thermostat_page_4_add_room.base, GX_STRING_ID_ADD_ROOM },
    { GX_NULL, 0 }
};

/******************************************************************************************/
/* Initiate title of templates based on "controller_base".                                */
/******************************************************************************************/
static VOID controller_title_init(TITLE_INFO *info)
{
    while (info->base)
    {
        /* Set title text for the specified controller widget. */
        gx_prompt_text_id_set(&info->base->controller_base_title, info->title_id);
        info++;
    }
}

/******************************************************************************************/
/* Initiate title of lights.                                                              */
/******************************************************************************************/
VOID lights_title_init()
{
    controller_title_init(lights_title_list);
}

/******************************************************************************************/
/* Initiate title of rooms.                                                               */
/******************************************************************************************/
VOID thermostat_title_init()
{
    controller_title_init(thermostat_title_list);
}

/******************************************************************************************/
/* Initiate title of locks.                                                               */
/******************************************************************************************/
VOID locks_title_init()
{
    controller_title_init(locks_title_list);
}

