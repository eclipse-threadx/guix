#include "demo_guix_smart_watch.h"

/* Define weather information structure.  */
typedef struct WEATHER_INFO_STRUCT {
    GX_RESOURCE_ID day_of_week_text_id;
    GX_RESOURCE_ID weather_icon_id;
    INT high_temperature;
    INT low_temperature;
}WEATHER_INFO;

/* Define weather information.  */
static WEATHER_INFO SanDiegoWeatherInfoList[]={
    {GX_STRING_ID_THURSDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 79, 73},
    {GX_STRING_ID_FRIDAY, GX_PIXELMAP_ID_WEATHER_ICON_SUNNY, 81, 70},
    {GX_STRING_ID_SATURDAY, GX_PIXELMAP_ID_WEATHER_ICON_CLOUDY, 76, 67},
    {GX_STRING_ID_SUNDAY, GX_PIXELMAP_ID_WEATHER_ICON_RAIN, 70, 65},
    {GX_STRING_ID_MONDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 76, 64},
    {GX_STRING_ID_TUESDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 77, 68},
    {GX_STRING_ID_WEDNESDAY, GX_PIXELMAP_ID_WEATHER_ICON_SUNNY, 82, 74}
};

static WEATHER_INFO LosAngelesWeatherInfoList[] = {
    {GX_STRING_ID_THURSDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 76, 69},
    {GX_STRING_ID_FRIDAY, GX_PIXELMAP_ID_WEATHER_ICON_SUNNY, 81, 70},
    {GX_STRING_ID_SATURDAY, GX_PIXELMAP_ID_WEATHER_ICON_SUNNY, 82, 71},
    {GX_STRING_ID_SUNDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 75, 67},
    {GX_STRING_ID_MONDAY, GX_PIXELMAP_ID_WEATHER_ICON_CLOUDY, 76, 64},
    {GX_STRING_ID_TUESDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 79, 71},
    {GX_STRING_ID_WEDNESDAY, GX_PIXELMAP_ID_WEATHER_ICON_SUNNY, 81, 72}
};

static WEATHER_INFO SanFranciscoWeatherInfoList[] = {
    {GX_STRING_ID_THURSDAY, GX_PIXELMAP_ID_WEATHER_ICON_SUNNY, 70, 61},
    {GX_STRING_ID_FRIDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 72, 65},
    {GX_STRING_ID_SATURDAY, GX_PIXELMAP_ID_WEATHER_ICON_CLOUDY, 68, 62},
    {GX_STRING_ID_SUNDAY, GX_PIXELMAP_ID_WEATHER_ICON_CLOUDY, 65, 60},
    {GX_STRING_ID_MONDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 63, 59},
    {GX_STRING_ID_TUESDAY, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY, 67, 61},
    {GX_STRING_ID_WEDNESDAY, GX_PIXELMAP_ID_WEATHER_ICON_CLOUDY, 70, 62}
};

/******************************************************************************************/
/* Update one day weather information.                                                    */
/******************************************************************************************/
static VOID update_one_day_weather_info(WEATHER_INFO_TEMPLATE_CONTROL_BLOCK* template, WEATHER_INFO* info)
{
    gx_prompt_text_id_set(&template->weather_info_template_day_of_week, info->day_of_week_text_id);
    gx_icon_pixelmap_set(&template->weather_info_template_weather_icon, info->weather_icon_id, info->weather_icon_id);
    gx_numeric_prompt_value_set(&template->weather_info_template_high_temperature, info->high_temperature);
    gx_numeric_prompt_value_set(&template->weather_info_template_low_temperature, info->low_temperature);
}

/******************************************************************************************/
/* Update screen weather information.                                                     */
/******************************************************************************************/
static VOID update_screen_weather_info(WEATHER_SCREEN_TEMPLATE_CONTROL_BLOCK *template)
{
    WEATHER_INFO *info_list;

    switch (template->base.gx_widget_id)
    {
    case ID_SAN_DIEGO_SCREEN:
        info_list = SanDiegoWeatherInfoList;
        break;

    case ID_LOS_ANGELES_SCREEN:
        info_list = LosAngelesWeatherInfoList;
        break;

    case ID_SAN_FRANCISCO_SCREEN:
        info_list = SanFranciscoWeatherInfoList;
        break;

    default:
        return;
    }

    update_one_day_weather_info(&template->weather_screen_template_day_win_1, &info_list[0]);
    update_one_day_weather_info(&template->weather_screen_template_day_win_2, &info_list[1]);
    update_one_day_weather_info(&template->weather_screen_template_day_win_3, &info_list[2]);
    update_one_day_weather_info(&template->weather_screen_template_day_win_4, &info_list[3]);
    update_one_day_weather_info(&template->weather_screen_template_day_win_5, &info_list[4]);
    update_one_day_weather_info(&template->weather_screen_template_day_win_6, &info_list[5]);
    update_one_day_weather_info(&template->weather_screen_template_day_win_7, &info_list[6]);
}

/******************************************************************************************/
/* Override the default event processing of "weather_screen_template" to handle signals   */
/* from my child widgets.                                                                 */
/******************************************************************************************/
UINT weather_screen_template_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        update_screen_weather_info((WEATHER_SCREEN_TEMPLATE_CONTROL_BLOCK *)window);
        return screen_template_event_process(window, event_ptr);

    default:
        return screen_template_event_process(window, event_ptr);
    }

    return 0;
}