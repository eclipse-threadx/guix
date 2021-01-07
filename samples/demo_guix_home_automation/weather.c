
#include "demo_guix_home_automation.h"

/* Define future weather information structure. */
typedef struct WEATHER_7DAY_INFO_STRUCT{
    GX_RESOURCE_ID icon; /* ID of the weather icon. */
    INT hi_value; /* Highest temperature. */
    INT low_value; /* Lowest temperature. */
}WEATHER_7DAY_INFO;

/* Define weather information structure. */
typedef struct WEATHER_INFO_STRUCT{
    WEATHER_BASE_CONTROL_BLOCK *widget; /* Pointer to the weather display widget. */
    GX_RESOURCE_ID city_name_id; /* City name. */
    INT temperature; /* Current temperature. */
    INT hi_value; /* Highest temperature. */
    INT low_value; /* Lowest temperature. */
    INT rain_value; /* Probability of rain. */
    GX_RESOURCE_ID weather_icon; /* ID of the weather icon. */
    GX_RESOURCE_ID weather_text_id; /* Weather description text. */
    INT wind_speed; /* Wind speed. */
    INT humidity_value; /* Humidity value. */
    WEATHER_7DAY_INFO *weather_7day_info_list; /* Weather information for next 7 days. */
}WEATHER_INFO;

/* This is a list of weather information for next 7 days in San Diego. */
static WEATHER_7DAY_INFO San_Diego_weather_7day_info_list[] = {
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 81, 70},
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 80, 70},
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 79, 65 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 79, 65 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 76, 64 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 85, 72 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 80, 70 }
};

/* This is a list of weather information for next 7 days in New York. */
static WEATHER_7DAY_INFO New_York_weather_7day_info_list[] = {
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_RAIN, 68, 79 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_RAIN, 69, 58 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 69, 55 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 69, 55 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 71, 64 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 65, 52 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY,70, 60 }
};

/* This is a list of weather information for next 7 days in Las Vegas. */
static WEATHER_7DAY_INFO Las_Vegas_weather_7day_info_list[] = {
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 85, 72 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 82, 65 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 82, 68 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 80, 68 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_CLOUDY, 76, 67 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 85, 72 },
    { GX_PIXELMAP_ID_WEATHER_7DAY_ICON_SUN, 80, 70 }
};

/* This is a list of weather information of 3 cities. */
static WEATHER_INFO weather_info_list[] = {
    { &win_San_Diego.base, GX_STRING_ID_SANDIEGO, 75, 82, 70, 0, GX_PIXELMAP_ID_WEATHER_ICON_CLOUDY, GX_STRING_ID_PARTLY_CLOUDY, 19, 58, San_Diego_weather_7day_info_list},
    { &win_New_York.base, GX_STRING_ID_NEW_YORK, 67, 71, 52, 90, GX_PIXELMAP_ID_WEATHER_ICON_RAIN, GX_STRING_ID_RAIN, 11, 64, New_York_weather_7day_info_list },
    { &win_Las_Vegas.base, GX_STRING_ID_LAS_VEGAS, 80, 85, 68, 0, GX_PIXELMAP_ID_WEATHER_ICON_SUN, GX_STRING_ID_SUNNY, 5, 14, Las_Vegas_weather_7day_info_list},
    { GX_NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

/* Define child widget animation information structure. */
typedef struct WEATHER_ANIMATION_INFO_STRUCT{
    INT target_id; /* Widget id of animation target. */
    INT delay; /* Delay time of animation. */
    GX_VALUE start_x_offset; /* The x offset from current position to start position. */
    GX_VALUE end_x_offset; /* The x offset from current position  to end position. */
    GX_VALUE start_y_offset; /* The y offset from current position to start position. */
    GX_VALUE end_y_offset; /* The y offset from current position to end position. */
}WEATHER_ANIMATION_INFO;

/* This is a list of animation informations.
   This list is used to run child widget animations when
   weather information page switches. */
WEATHER_ANIMATION_INFO animation_info_list[] = {
    { ID_WEATHER_WIN, 0, -128, 28, 23, 23 },
    { ID_WIND_WIN, 10, -128, 28, 110, 110 },
    { ID_HUMIDITY_WIN, 20, -128, 28, 197, 197 },
    { ID_THU_WIN, 0, 611, 434, 15, 15 },
    { ID_FRI_WIN, 10, 611, 434, 53, 53 },
    { ID_SAT_WIN, 20, 611, 434, 91, 91 },
    { ID_SUN_WIN, 30, 611, 434, 130, 130 },
    { ID_MON_WIN, 40, 611, 434, 168, 168 },
    { ID_TUE_WIN, 50, 611, 434, 206, 206 },
    { ID_WED_WIN, 60, 611, 434, 243, 243 },
    { ID_TITLE, 0, 200, 200, -42, 24 },
    { 0, 0, 0, 0, 0, 0 }
};

/* This variable is ued to count the number of in process animation in current screen. */
extern int screen_animation_count;

/* This variable is used to run screen slide animation. */
extern GX_ANIMATION slide_animation;

/******************************************************************************************/
/* Initiate weather information.                                                          */
/******************************************************************************************/
static VOID init_weather_info(WEATHER_INFO *info)
{
    int index;
    GX_ICON *icon;
    GX_NUMERIC_PROMPT *hi;
    GX_NUMERIC_PROMPT *low;
    WEATHER_7DAY_INFO *seven_day_info;
    WEATHER_BASE_CONTROL_BLOCK *base = info->widget;

    /* Set city name. */
    gx_prompt_text_id_set(&base->weather_base_city_name, info->city_name_id);

    /* Set current temperature. */
    gx_numeric_prompt_value_set(&base->weather_base_temperature, info->temperature);

    /* Set highest temperature. */
    gx_numeric_prompt_value_set(&base->weather_base_hi_value, info->hi_value);

    /* Set probability of rain. */
    gx_numeric_prompt_value_set(&base->weather_base_rain_value, info->rain_value);

    /* Set weather icon. */
    gx_icon_pixelmap_set(&base->weather_base_weather_icon, info->weather_icon, info->weather_icon);

    /* Set weather text. */
    gx_prompt_text_id_set(&base->weather_base_weather_text, info->weather_text_id);

    /* Set wind speed. */
    gx_numeric_prompt_value_set(&base->weather_base_wind_speed, info->wind_speed);

    /* Set humidity value. */
    gx_numeric_prompt_value_set(&base->weather_base_humidity_value, info->humidity_value);

    /* Set weather information for next 7 days. */
    for (index = 0; index < 7; index++)
    {
        switch (index)
        {
        case 0:
            icon = &base->weather_base_thu_wicon;
            hi = &base->weather_base_thu_hi_value;
            low = &base->weather_base_thu_low_value;
            break;
        case 1:
            icon = &base->weather_base_fri_wicon;
            hi = &base->weather_base_fri_hi_value;
            low = &base->weather_base_fri_low_value;
            break;
        case 2:
            icon = &base->weather_base_sat_wicon;
            hi = &base->weather_base_sat_hi_value;
            low = &base->weather_base_sat_low_value;
            break;
        case 3:
            icon = &base->weather_base_sun_wicon;
            hi = &base->weather_base_sun_hi_value;
            low = &base->weather_base_sun_low_value;
            break;
        case 4:
            icon = &base->weather_base_mon_wicon;
            hi = &base->weather_base_mon_hi_value;
            low = &base->weather_base_mon_low_value;
            break;
        case 5:
            icon = &base->weather_base_tue_wicon;
            hi = &base->weather_base_tue_hi_value;
            low = &base->weather_base_tue_low_value;
            break;
        case 6:
            icon = &base->weather_base_wed_wicon;
            hi = &base->weather_base_wed_hi_value;
            low = &base->weather_base_wed_low_value;
            break;

        }
        seven_day_info = &info->weather_7day_info_list[index];

        gx_icon_pixelmap_set(icon, seven_day_info->icon, seven_day_info->icon);
        gx_numeric_prompt_value_set(hi, seven_day_info->hi_value);
        gx_numeric_prompt_value_set(low, seven_day_info->low_value);
    }
}

/******************************************************************************************/
/* Hide some children of weather window.                                                  */
/******************************************************************************************/
static VOID hide_weather_children(WEATHER_INFO *info)
{
    WEATHER_BASE_CONTROL_BLOCK *base = info->widget;
    GX_WIDGET *child;
    GX_WIDGET *next;

    child = base->gx_widget_first_child;

    /* Hide some child widgets. */
    while (child)
    {
        next = child->gx_widget_next;

        if (child->gx_widget_id != ID_V_LINE &&
            child->gx_widget_id != ID_WEATHER_BG)
        {
            gx_widget_hide(child);
        }
        child = next;
    }
}

/******************************************************************************************/
/* Start children animation for specified weather window.                                 */
/******************************************************************************************/
static VOID start_weather_children_animation(WEATHER_INFO *info)
{
    WEATHER_BASE_CONTROL_BLOCK *base = info->widget;
    GX_ANIMATION *animation;
    GX_ANIMATION_INFO animation_info;
    GX_WIDGET *target;
    WEATHER_ANIMATION_INFO *w_aninfo = animation_info_list;

    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));

    animation_info.gx_animation_parent = (GX_WIDGET *)base;
    animation_info.gx_animation_id = ANIMATION_ID_SCREEN_CHILDREN;
    animation_info.gx_animation_style = 0;
    animation_info.gx_animation_start_alpha = 255;
    animation_info.gx_animation_end_alpha = 255;
    animation_info.gx_animation_steps = 1000 / GX_SYSTEM_TIMER_MS;
    animation_info.gx_animation_frame_interval = 1;

    /* Start animations descriped in animation information list. */
    while (w_aninfo->target_id)
    {
        gx_widget_find(base, w_aninfo->target_id, 1, &target);

        if (target && (gx_system_animation_get(&animation) == GX_SUCCESS))
        {
            animation_info.gx_animation_target = target;
            animation_info.gx_animation_start_delay = w_aninfo->delay;
            animation_info.gx_animation_start_position.gx_point_x = base->gx_widget_size.gx_rectangle_left + w_aninfo->start_x_offset;
            animation_info.gx_animation_start_position.gx_point_y = base->gx_widget_size.gx_rectangle_top + w_aninfo->start_y_offset;
            animation_info.gx_animation_end_position.gx_point_x = base->gx_widget_size.gx_rectangle_left + w_aninfo->end_x_offset;
            animation_info.gx_animation_end_position.gx_point_y = base->gx_widget_size.gx_rectangle_top + w_aninfo->end_y_offset;

            gx_animation_start(animation, &animation_info);
            screen_animation_count++;
        }

        w_aninfo++;
    }

    gx_widget_attach(base, &base->weather_base_temperature);
    gx_widget_attach(base, &base->weather_base_bottom_info_win);
    gx_widget_attach(base, &base->weather_base_degree_lable);
}

/******************************************************************************************/
/* Reset weather screen.                                                                  */
/******************************************************************************************/
VOID weather_screen_reset()
{
    WEATHER_INFO *info = weather_info_list;

    while (info->widget)
    {
        if (info->widget->gx_widget_status & GX_STATUS_VISIBLE)
        {
            hide_weather_children(info);
        }

        info++;
    }
}

/******************************************************************************************/
/* Start animation for weather screen.                                                    */
/******************************************************************************************/
VOID weather_screen_animation_start()
{
    WEATHER_INFO *info = weather_info_list;

    while (info->widget)
    {
        if (info->widget->gx_widget_status & GX_STATUS_VISIBLE)
        {
            if (!(info->widget->weather_base_temperature.gx_widget_status & GX_STATUS_VISIBLE))
            {
                /* Disable drag slide animation before weather screen item animations complete. */
                gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)&weather_screen.base.screen_base_slide_win);

                /* Disable pagination buttons before weather screen item animations complete. */
                pagination_button_enable_disable((GX_WINDOW *)&weather_screen.base, GX_FALSE);

                start_weather_children_animation(info);
            }
        }
        else
        {
            hide_weather_children(info);
        }

        info++;
    }
}

/******************************************************************************************/
/* Initiate weather screen information.                                                   */
/******************************************************************************************/
VOID weather_screen_init()
{
    WEATHER_INFO *info = weather_info_list;

    while (info->widget)
    {
        /* Initialize weather information. */
        init_weather_info(info);

        /* Hide child widgets.
           Next time the weather screen been visible, those child widgets will get visible with animation. */
        hide_weather_children(info);

        info++;
    }

    /* Set "Add City" widget text. */
    gx_prompt_text_id_set(&weather_screen.base.screen_base_add_text, GX_STRING_ID_ADD_CITY);
}
