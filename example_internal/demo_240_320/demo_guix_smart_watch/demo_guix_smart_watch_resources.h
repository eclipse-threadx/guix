/*******************************************************************************/
/*  This file is auto-generated by Azure RTOS GUIX Studio. Do not edit this    */
/*  file by hand. Modifications to this file should only be made by running    */
/*  the Azure RTOS GUIX Studio application and re-generating the application   */
/*  resource file(s). For more information please refer to the Azure RTOS GUIX */
/*  Studio User Guide, or visit our web site at azure.com/rtos                 */
/*                                                                             */
/*  GUIX Studio Revision 6.1.12.0                                              */
/*  Date (dd.mm.yyyy): 25. 8.2022   Time (hh:mm): 18:33                        */
/*******************************************************************************/


#ifndef _DEMO_GUIX_SMART_WATCH_MAIN_SCREEN_RESOURCES_H_
#define _DEMO_GUIX_SMART_WATCH_MAIN_SCREEN_RESOURCES_H_

#include "gx_api.h"

#if !defined(GX_UTF8_SUPPORT)
#error "The symbol GX_UTF8_SUPPORT must be defined to support the Studio project settings".
#endif

/* Display and theme definitions                                               */

#define MAIN_SCREEN 0
#define MAIN_SCREEN_COLOR_FORMAT GX_COLOR_FORMAT_24XRGB
#define MAIN_SCREEN_X_RESOLUTION 240
#define MAIN_SCREEN_Y_RESOLUTION 320
#define MAIN_SCREEN_THEME_DEFAULT 0
#define MAIN_SCREEN_THEME_TABLE_SIZE 1

/* Language definitions                                                        */

#define LANGUAGE_ENGLISH 0
#define LANGUAGE_SPANISH 1
#define LANGUAGE_CHINESE 2
#define LANGUAGE_JAPANESE 3
#define MAIN_SCREEN_LANGUAGE_TABLE_SIZE 4

/* Color ID definitions                                                        */

#define GX_COLOR_ID_GRAY 29
#define GX_COLOR_ID_WHITE 30
#define GX_COLOR_ID_BLUE 31
#define GX_COLOR_ID_GREEN 32
#define GX_COLOR_ID_LIGHT_GRAY 33
#define GX_COLOR_ID_BLACK 34
#define GX_COLOR_ID_DARK_GRAY 35
#define GX_COLOR_ID_SILVERY_GRAY 36
#define GX_COLOR_ID_MILK_WHITE 37
#define GX_COLOR_ID_CURSOR 38
#define GX_COLOR_ID_HEALTHY_ICON 39
#define GX_COLOR_ID_HINT_TEXT 40
#define GX_COLOR_ID_BUTTON 41
#define GX_COLOR_ID_NAME 42
#define MAIN_SCREEN_COLOR_TABLE_SIZE 43

/* Font ID definitions                                                         */

#define GX_FONT_ID_NORMAL 4
#define GX_FONT_ID_BIG_NUMBER_70 5
#define GX_FONT_ID_BIG_NUMBER_50 6
#define GX_FONT_ID_MIDDLE 7
#define GX_FONT_ID_SMALL 8
#define GX_FONT_ID_MIDDLE_30 9
#define GX_FONT_ID_MIDDLE_NUMBER 10
#define MAIN_SCREEN_FONT_TABLE_SIZE 11

/* Pixelmap ID definitions                                                     */

#define GX_PIXELMAP_ID_ABOUT_ON 5
#define GX_PIXELMAP_ID_ABOUT_PUSHED 6
#define GX_PIXELMAP_ID_ALARM 7
#define GX_PIXELMAP_ID_ARROW 8
#define GX_PIXELMAP_ID_ARROW_DOWN 9
#define GX_PIXELMAP_ID_ARROW_LEFT 10
#define GX_PIXELMAP_ID_ARROW_RIGHT 11
#define GX_PIXELMAP_ID_ARROW_UP 12
#define GX_PIXELMAP_ID_BATTERY 13
#define GX_PIXELMAP_ID_BLUETOOTH 14
#define GX_PIXELMAP_ID_BRUSH 15
#define GX_PIXELMAP_ID_BUTTON_AM_PM 16
#define GX_PIXELMAP_ID_BUTTON_CONTACT_PLUS 17
#define GX_PIXELMAP_ID_BUTTON_CONTACT_PLUS_ON 18
#define GX_PIXELMAP_ID_BUTTON_EQUALS 19
#define GX_PIXELMAP_ID_BUTTON_LEFT 20
#define GX_PIXELMAP_ID_BUTTON_MIDDLE 21
#define GX_PIXELMAP_ID_BUTTON_RIGHT 22
#define GX_PIXELMAP_ID_BUTTON_SEND 23
#define GX_PIXELMAP_ID_BUTTON_SEND_ON 24
#define GX_PIXELMAP_ID_BUTTON_START_RESET 25
#define GX_PIXELMAP_ID_CALCULATOR_CLEAR 26
#define GX_PIXELMAP_ID_CALCULATOR_FIELD 27
#define GX_PIXELMAP_ID_CALCULATOR_MINUS 28
#define GX_PIXELMAP_ID_CALCULATOR_MULTIPLY 29
#define GX_PIXELMAP_ID_CALCULATOR_ON 30
#define GX_PIXELMAP_ID_CALCULATOR_PERCENTAGE 31
#define GX_PIXELMAP_ID_CALCULATOR_PLUS 32
#define GX_PIXELMAP_ID_CALCULATOR_PLUS_MINUS 33
#define GX_PIXELMAP_ID_CALCULATOR_PUSHED 34
#define GX_PIXELMAP_ID_CALCULATOR_SUBTRACT 35
#define GX_PIXELMAP_ID_CLOCK_ON 36
#define GX_PIXELMAP_ID_CLOCK_PUSHED 37
#define GX_PIXELMAP_ID_CONTACTS_ON 38
#define GX_PIXELMAP_ID_CONTACTS_PUSHED 39
#define GX_PIXELMAP_ID_CONTACT_INFO_BG 40
#define GX_PIXELMAP_ID_DASHBOARD 41
#define GX_PIXELMAP_ID_DOT 42
#define GX_PIXELMAP_ID_EKG_WAVE_DOT 43
#define GX_PIXELMAP_ID_GAMES_ON 44
#define GX_PIXELMAP_ID_GAMES_PUSHED 45
#define GX_PIXELMAP_ID_GAME_FOUR 46
#define GX_PIXELMAP_ID_GAME_ONE 47
#define GX_PIXELMAP_ID_GAME_THREE 48
#define GX_PIXELMAP_ID_GAME_TWO 49
#define GX_PIXELMAP_ID_GLOB 50
#define GX_PIXELMAP_ID_GUIX_BG 51
#define GX_PIXELMAP_ID_GUIX_TRANSPERENT_COVER 52
#define GX_PIXELMAP_ID_GX_CONTACT_COMMENT_OFF 53
#define GX_PIXELMAP_ID_HALF_TRANSPARENT_COVER 54
#define GX_PIXELMAP_ID_HEALTH_HEART_LARGE 55
#define GX_PIXELMAP_ID_HEALTH_HEART_SMALL 56
#define GX_PIXELMAP_ID_HEALTH_ON 57
#define GX_PIXELMAP_ID_HEALTH_PUSHED 58
#define GX_PIXELMAP_ID_HEALTH_RUN 59
#define GX_PIXELMAP_ID_HORIZONTAL_LINE 60
#define GX_PIXELMAP_ID_ICON_PENCIL 61
#define GX_PIXELMAP_ID_KEYBOARD_BG 62
#define GX_PIXELMAP_ID_MAP_MARKER_RED 63
#define GX_PIXELMAP_ID_MAP_ON 64
#define GX_PIXELMAP_ID_MAP_PUSHED 65
#define GX_PIXELMAP_ID_MESSAGES_ON 66
#define GX_PIXELMAP_ID_MESSAGES_PUSHED 67
#define GX_PIXELMAP_ID_MICROPHONE 68
#define GX_PIXELMAP_ID_PAGINATION_DOT 69
#define GX_PIXELMAP_ID_PENCIL_EDIT 70
#define GX_PIXELMAP_ID_PRELOADER 71
#define GX_PIXELMAP_ID_SEARCH 72
#define GX_PIXELMAP_ID_SEARCH_BUTTON_LEFT 73
#define GX_PIXELMAP_ID_SEARCH_BUTTON_RIGHT 74
#define GX_PIXELMAP_ID_SETTINGS_ON 75
#define GX_PIXELMAP_ID_SETTINGS_PUSHED 76
#define GX_PIXELMAP_ID_SOUND 77
#define GX_PIXELMAP_ID_STOP_WATCH 78
#define GX_PIXELMAP_ID_SWITCH_ACTIVE 79
#define GX_PIXELMAP_ID_SWITCH_ACTIVE_BIG 80
#define GX_PIXELMAP_ID_SWITCH_BG 81
#define GX_PIXELMAP_ID_SWITCH_BG_BIG 82
#define GX_PIXELMAP_ID_SWITCH_DISACTIVE 83
#define GX_PIXELMAP_ID_SWITCH_DISACTIVE_BIG 84
#define GX_PIXELMAP_ID_TIME 85
#define GX_PIXELMAP_ID_UP_ARROW 86
#define GX_PIXELMAP_ID_USER_FEMALE 87
#define GX_PIXELMAP_ID_USER_MALE 88
#define GX_PIXELMAP_ID_WEATHER_CLOUDS_LARGE 89
#define GX_PIXELMAP_ID_WEATHER_CLOUD_SMALL 90
#define GX_PIXELMAP_ID_WEATHER_ON 91
#define GX_PIXELMAP_ID_WEATHER_PUSHED 92
#define GX_PIXELMAP_ID_WEATHER_SUN_LARGE 93
#define GX_PIXELMAP_ID_WEATHER_SUN_SMALL 94
#define GX_PIXELMAP_ID_WIFI 95
#define GX_PIXELMAP_ID_ZOOM_BG 96
#define GX_PIXELMAP_ID_ZOOM_IN 97
#define GX_PIXELMAP_ID_ZOOM_OUT 98
#define MAIN_SCREEN_PIXELMAP_TABLE_SIZE 99

/* String Ids                                                                  */

#define GX_STRING_ID_ABOUT_GUIX 1
#define GX_STRING_ID_ADDRESS 2
#define GX_STRING_ID_BLUETOOTH 3
#define GX_STRING_ID_CHARACTER_O 4
#define GX_STRING_ID_CHINESE 5
#define GX_STRING_ID_COMPANY 6
#define GX_STRING_ID_COMPANY_CONTACT 7
#define GX_STRING_ID_COMPANY_NAME 8
#define GX_STRING_ID_COMPANY_WEBSITE 9
#define GX_STRING_ID_DATE 10
#define GX_STRING_ID_DOUBLE_ZERO 11
#define GX_STRING_ID_EDIT_ALARM 12
#define GX_STRING_ID_EMAIL 13
#define GX_STRING_ID_ENGLISH 14
#define GX_STRING_ID_FIRST_NAME 15
#define GX_STRING_ID_FRIDAY 16
#define GX_STRING_ID_JAPANESE 17
#define GX_STRING_ID_LAST_NAME 18
#define GX_STRING_ID_MOBILE 19
#define GX_STRING_ID_MONDAY 20
#define GX_STRING_ID_NOTIFICATIONS 21
#define GX_STRING_ID_OFFICE 22
#define GX_STRING_ID_REPLAY 23
#define GX_STRING_ID_RESET 24
#define GX_STRING_ID_SATURDAY 25
#define GX_STRING_ID_SAVE 26
#define GX_STRING_ID_SENT 27
#define GX_STRING_ID_SEPARATOR 28
#define GX_STRING_ID_SOUNDS 29
#define GX_STRING_ID_SPANISH 30
#define GX_STRING_ID_START 31
#define GX_STRING_ID_STATIC 32
#define GX_STRING_ID_STOP 33
#define GX_STRING_ID_STOPWATCH 34
#define GX_STRING_ID_STRING_1 35
#define GX_STRING_ID_STRING_10 36
#define GX_STRING_ID_STRING_11 37
#define GX_STRING_ID_STRING_12 38
#define GX_STRING_ID_STRING_13 39
#define GX_STRING_ID_STRING_14 40
#define GX_STRING_ID_STRING_15 41
#define GX_STRING_ID_STRING_16 42
#define GX_STRING_ID_STRING_17 43
#define GX_STRING_ID_STRING_18 44
#define GX_STRING_ID_STRING_19 45
#define GX_STRING_ID_STRING_2 46
#define GX_STRING_ID_STRING_20 47
#define GX_STRING_ID_STRING_21 48
#define GX_STRING_ID_STRING_22 49
#define GX_STRING_ID_STRING_23 50
#define GX_STRING_ID_STRING_24 51
#define GX_STRING_ID_STRING_25 52
#define GX_STRING_ID_STRING_26 53
#define GX_STRING_ID_STRING_27 54
#define GX_STRING_ID_STRING_28 55
#define GX_STRING_ID_STRING_29 56
#define GX_STRING_ID_STRING_3 57
#define GX_STRING_ID_STRING_30 58
#define GX_STRING_ID_STRING_31 59
#define GX_STRING_ID_STRING_32 60
#define GX_STRING_ID_STRING_33 61
#define GX_STRING_ID_STRING_34 62
#define GX_STRING_ID_STRING_35 63
#define GX_STRING_ID_STRING_36 64
#define GX_STRING_ID_STRING_37 65
#define GX_STRING_ID_STRING_38 66
#define GX_STRING_ID_STRING_39 67
#define GX_STRING_ID_STRING_4 68
#define GX_STRING_ID_STRING_40 69
#define GX_STRING_ID_STRING_41 70
#define GX_STRING_ID_STRING_42 71
#define GX_STRING_ID_STRING_43 72
#define GX_STRING_ID_STRING_44 73
#define GX_STRING_ID_STRING_45 74
#define GX_STRING_ID_STRING_46 75
#define GX_STRING_ID_STRING_47 76
#define GX_STRING_ID_STRING_48 77
#define GX_STRING_ID_STRING_49 78
#define GX_STRING_ID_STRING_5 79
#define GX_STRING_ID_STRING_50 80
#define GX_STRING_ID_STRING_51 81
#define GX_STRING_ID_STRING_52 82
#define GX_STRING_ID_STRING_53 83
#define GX_STRING_ID_STRING_54 84
#define GX_STRING_ID_STRING_55 85
#define GX_STRING_ID_STRING_56 86
#define GX_STRING_ID_STRING_57 87
#define GX_STRING_ID_STRING_58 88
#define GX_STRING_ID_STRING_59 89
#define GX_STRING_ID_STRING_6 90
#define GX_STRING_ID_STRING_60 91
#define GX_STRING_ID_STRING_61 92
#define GX_STRING_ID_STRING_62 93
#define GX_STRING_ID_STRING_63 94
#define GX_STRING_ID_STRING_64 95
#define GX_STRING_ID_STRING_65 96
#define GX_STRING_ID_STRING_66 97
#define GX_STRING_ID_STRING_67 98
#define GX_STRING_ID_STRING_68 99
#define GX_STRING_ID_STRING_69 100
#define GX_STRING_ID_STRING_7 101
#define GX_STRING_ID_STRING_70 102
#define GX_STRING_ID_STRING_71 103
#define GX_STRING_ID_STRING_72 104
#define GX_STRING_ID_STRING_73 105
#define GX_STRING_ID_STRING_8 106
#define GX_STRING_ID_STRING_9 107
#define GX_STRING_ID_SUNDAY 108
#define GX_STRING_ID_THEMES 109
#define GX_STRING_ID_THUESDAY 110
#define GX_STRING_ID_THURSDAY 111
#define GX_STRING_ID_TIME_AM 112
#define GX_STRING_ID_TIME_PM 113
#define GX_STRING_ID_VOICE_CONTROL 114
#define GX_STRING_ID_WEDNESDAY 115
#define GX_STRING_ID_WIFI 116
#define GX_STRING_ID_YESTERDAY 117
#define GX_STRING_ID_ZERO 118
#define GX_STRING_ID_LOS_ANGELES 119
#define GX_STRING_ID_CHICAGO 120
#define GX_STRING_ID_NEW_YORK 121
#define GX_STRING_ID_LONDON 122
#define GX_STRING_ID_MOSCOW 123
#define GX_STRING_ID_BEIJING 124
#define MAIN_SCREEN_STRING_TABLE_SIZE 125

#endif                                       /* sentry                         */