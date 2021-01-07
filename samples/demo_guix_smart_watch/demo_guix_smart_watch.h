
#include <stdio.h>
#include "gx_api.h"

#include "guix_smart_watch_resources.h"
#include "guix_smart_watch_specifications.h"
#include "custom_checkbox.h"

#define SWITCH_BTTON_ID      0xf0
#define MSG_LIST_ID_START    0xf1
#define CLOCK_TIMER          0x01

#define CITY_NEW_YORK        0x01
#define CITY_SAN_FRANCISCO   0x02
#define CITY_LOS_ANGELES     0x03
#define SLIDE_LEFT           0x00
#define SLIDE_RIGHT          0x01

#define TIME_AM              0x01
#define TIME_PM              0x02

#define ANIMATION_ID_SCREEN_LIST_SLIDE 2

extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID *memory_allocate(ULONG size);
void memory_free(VOID *mem);

VOID row_widget_draw(GX_WIDGET *widget);

UINT clock_pages_switch_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr);
VOID time_list_children_create(GX_VERTICAL_LIST *list);
VOID alarm_list_children_create(GX_VERTICAL_LIST *list);
VOID city_list_children_create(GX_VERTICAL_LIST *list);
VOID game_list_children_create(GX_VERTICAL_LIST *list);
VOID contact_list_children_create(GX_VERTICAL_LIST *list);
VOID future_weather_list_children_create(GX_WIDGET *parent, SHORT city);
VOID msg_list_children_create(GX_VERTICAL_LIST *list);
VOID keyboard_children_create();
VOID screen_list_slide(GX_WIDGET **screen_list, GX_WIDGET *parent, INT type);

VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen);
GX_CHAR *expression_evaluate(GX_CHAR *expression, GX_BOOL persent_flag);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

typedef struct SYS_TIME_STRUCT{
    GX_VALUE year;
    GX_VALUE month;
    GX_VALUE day;
    GX_VALUE hour;
    GX_VALUE minute;
    GX_VALUE second;
    USHORT   am_pm;
}SYS_TIME;

#define MAX_FULL_NAME_LENGTH    42
#define MAX_NAME_LENGTH         20
#define MAX_PHONE_NUMBER_LENGTH 15
#define MAX_EMAIL_LENGTH        50
#define MAX_ADDRESS_LENGTH      50
#define MAX_TIME_TEXT_LENGTH    2
#define MAX_CITY_NAME_LENGTH    50
#define MAX_KEY_LENGTH          5
#define MAX_MSG_LENGTH          100
#define MAX_EXPRESSION_LENGTH   100
#define MAX_T_TEXT_LENGTH     2

typedef struct CONTACT_INTO_STRUCT{
    GX_CHAR name[MAX_FULL_NAME_LENGTH + 1];
    GX_CHAR firstname[MAX_NAME_LENGTH + 1];
    GX_CHAR lastname[MAX_NAME_LENGTH + 1];
    GX_CHAR mobile[MAX_PHONE_NUMBER_LENGTH + 1];
    GX_CHAR office[MAX_PHONE_NUMBER_LENGTH + 1];
    GX_CHAR email[MAX_EMAIL_LENGTH + 1];
    GX_CHAR address[MAX_ADDRESS_LENGTH + 1];
    GX_RESOURCE_ID image_id;
}CONTACT_INFO;

#ifdef INTERNAL_BUILD
#define ROAD_MAP_JPG_PATH "..\\graphics\\map\\road_map.jpg"
#else
#define ROAD_MAP_JPG_PATH "..\\..\\graphics\\map\\road_map.jpg"
#endif