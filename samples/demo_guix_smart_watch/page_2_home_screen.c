#include "demo_guix_smart_watch.h"

/* Extern system time.  */
extern TIME system_time;

/* Define text id list for day of week names.  */
static GX_RESOURCE_ID day_name_ids[] = {
    GX_STRING_ID_SUNDAY,
    GX_STRING_ID_MONDAY,
    GX_STRING_ID_TUESDAY,
    GX_STRING_ID_WEDNESDAY,
    GX_STRING_ID_THURSDAY,
    GX_STRING_ID_FRIDAY,
    GX_STRING_ID_SATURDAY
};

/* Define text id list for month names.  */
static GX_RESOURCE_ID month_name_ids[] = {
    GX_STRING_ID_JANUARY,
    GX_STRING_ID_FEBRUARY,
    GX_STRING_ID_MARCH,
    GX_STRING_ID_APRIL,
    GX_STRING_ID_MAY,
    GX_STRING_ID_JUNE,
    GX_STRING_ID_JULY,
    GX_STRING_ID_AUGUST,
    GX_STRING_ID_SEPTEMBER,
    GX_STRING_ID_OCTOBER,
    GX_STRING_ID_NOVEMBER,
    GX_STRING_ID_DECEMBER
};

/******************************************************************************************/
/* Calculate string length.                                                               */
/******************************************************************************************/
static UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}

/******************************************************************************************/
/* Update clock time.                                                                     */
/******************************************************************************************/
static VOID screen_clock_update()
{
    GX_RESOURCE_ID am_pm_text_id;
    GX_CHAR date_string_buffer[20];
    GX_STRING string;

    if (system_time.second & 0x1)
    {
        gx_widget_fill_color_set(&home_screen.home_screen_time_second_up, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_widget_fill_color_set(&home_screen.home_screen_time_second_down, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    }
    else
    {
        gx_widget_fill_color_set(&home_screen.home_screen_time_second_up, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        gx_widget_fill_color_set(&home_screen.home_screen_time_second_down, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    }

    if (system_time.hour < 12)
    {
        am_pm_text_id = GX_STRING_ID_AM;
    }
    else
    {
        am_pm_text_id = GX_STRING_ID_PM;
    }

    gx_numeric_prompt_value_set(&home_screen.home_screen_time_hour, system_time.hour);
    gx_numeric_prompt_value_set(&home_screen.home_screen_time_minute, system_time.minute);
    gx_prompt_text_id_set(&home_screen.home_screen_am_pm, am_pm_text_id);
    gx_prompt_text_id_set(&home_screen.home_screen_day_of_week, day_name_ids[system_time.day_of_week]);

    gx_widget_string_get_ext(&home_screen, month_name_ids[system_time.month - 1], &string);
    
    if(string.gx_string_ptr)
    {
        sprintf(date_string_buffer, ", %s %d", string.gx_string_ptr, system_time.day);

        string.gx_string_ptr = date_string_buffer;
        string.gx_string_length = string_length_get(date_string_buffer, sizeof(date_string_buffer) - 1);
        gx_prompt_text_set_ext(&home_screen.home_screen_date, &string);
    }
}

/******************************************************************************************/
/* Override the default event processing of "home_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT home_screen_event_handler(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:

        gx_window_event_process(window, event_ptr);
            
        /* Update clock time. */
        screen_clock_update();

        /* Start a timer to update clock.  */
        gx_system_timer_start(window, SCREEN_CLOCK_TIMER_ID, GX_TICKS_SECOND, GX_TICKS_SECOND);

        return GX_SUCCESS;

    case GX_EVENT_HIDE:

        /* Stop clock timer.  */
        gx_system_timer_stop(window, SCREEN_CLOCK_TIMER_ID);

        return gx_window_event_process(window, event_ptr);

    case USER_EVENT_ANIMATION_START:
        gx_sprite_start(&home_screen.home_screen_sprite, 0);
        break;

    case USER_EVENT_ANIMATION_STOP:
        gx_sprite_stop(&home_screen.home_screen_sprite);
        break;

    case GX_EVENT_TIMER:

        if (event_ptr->gx_event_payload.gx_event_timer_id == SCREEN_CLOCK_TIMER_ID)
        {
            /* Update clock time. */
            screen_clock_update();
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Define format function for numeric prompt to show time.                                */
/******************************************************************************************/
VOID time_format(GX_NUMERIC_PROMPT* prompt, INT value)
{
    if (value < 10)
    {
        prompt->gx_numeric_prompt_buffer[0] = '0';
        prompt->gx_numeric_prompt_buffer[1] = '0' + value;
    }
    else if (value < 100)
    {
        prompt->gx_numeric_prompt_buffer[0] = '0' + (value / 10);
        prompt->gx_numeric_prompt_buffer[1] = '0' + (value % 10);
    }
}