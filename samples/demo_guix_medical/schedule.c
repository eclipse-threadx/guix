/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <time.h>
#include "gx_api.h"

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

#include "demo_guix_medical.h"

/* Define timer ids.  */
#define TIME_TIMER                    100
#define ACTIVE_CIRCLE_ANIMATION_TIMER 101

/* Define constants.  */
#define SCHEDULE_COL_COUNT 7
#define SCHEDULE_ROW_COUNT 11

UINT month_name_ids[] = {
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

UINT week_name_ids[] = {
    GX_STRING_ID_SUNDAY,
    GX_STRING_ID_MONDAY,
    GX_STRING_ID_TUESDAY,
    GX_STRING_ID_WEDNESDAY,
    GX_STRING_ID_THURSDAY,
    GX_STRING_ID_FRIDAY,
    GX_STRING_ID_SATURDAY
};

GX_PROMPT* week_widget_list[SCHEDULE_COL_COUNT] = {
    &schedule_screen.schedule_screen_c1_week,
    &schedule_screen.schedule_screen_c2_week,
    &schedule_screen.schedule_screen_c3_week,
    &schedule_screen.schedule_screen_c4_week,
    &schedule_screen.schedule_screen_c5_week,
    &schedule_screen.schedule_screen_c6_week,
    &schedule_screen.schedule_screen_c7_week
};

GX_NUMERIC_PROMPT* day_widget_list[SCHEDULE_COL_COUNT] = {
    &schedule_screen.schedule_screen_c1_day,
    &schedule_screen.schedule_screen_c2_day,
    &schedule_screen.schedule_screen_c3_day,
    &schedule_screen.schedule_screen_c4_day,
    &schedule_screen.schedule_screen_c5_day,
    &schedule_screen.schedule_screen_c6_day,
    &schedule_screen.schedule_screen_c7_day,
};

VOID hide_active_bar();

/******************************************************************************************/
/* Update date of the schedule table.                                                     */
/******************************************************************************************/
static VOID update_date()
{
INT        index;
time_t     now;
struct tm *ts;

    now = time(NULL);
    ts = localtime(&now);

    gx_prompt_text_id_set(&schedule_screen.schedule_screen_month, month_name_ids[ts->tm_mon]);
    gx_numeric_prompt_value_set(&schedule_screen.schedule_screen_year, ts->tm_year + 1900);

    ts->tm_mday--;

    for (index = 0; index < SCHEDULE_COL_COUNT; index++)
    {
        /* Normalize ts.  */
        mktime(ts);

        gx_prompt_text_id_set(week_widget_list[index], week_name_ids[ts->tm_wday]);
        gx_numeric_prompt_value_set(day_widget_list[index], ts->tm_mday);

        ts->tm_mday++;
    }
}

/******************************************************************************************/
/* Show the active bar to display the patient's information.                              */
/******************************************************************************************/
static VOID show_active_bar()
{
    gx_widget_attach(&schedule_screen.schedule_screen_r5_win, &schedule_screen.schedule_screen_active_bar);
    gx_widget_attach(&schedule_screen.schedule_screen_active_bar, &schedule_screen.schedule_screen_active_ball);

    gx_widget_hide(&schedule_screen.schedule_screen_active_circle);
}

/******************************************************************************************/
/* Hide the active bar.                                                                   */
/******************************************************************************************/
VOID hide_active_bar()
{
    gx_widget_detach(&schedule_screen.schedule_screen_active_bar);
    gx_widget_attach(&schedule_screen.schedule_screen_r5_win, &schedule_screen.schedule_screen_active_ball);

    gx_widget_show(&schedule_screen.schedule_screen_active_circle);
}

/******************************************************************************************/
/* Show/Hide active circle.                                                               */
/******************************************************************************************/
static VOID show_hide_active_circle()
{
    if (schedule_screen.schedule_screen_active_bar.gx_widget_parent == GX_NULL)
    {
        if (schedule_screen.schedule_screen_active_circle.gx_widget_parent)
        {
            gx_widget_detach(&schedule_screen.schedule_screen_active_circle);
        }
        else
        {
            gx_widget_back_attach(&schedule_screen.schedule_screen_r5_win, &schedule_screen.schedule_screen_active_circle);
        }
    }
}

/******************************************************************************************/
/* Update time colon animation.                                                           */
/******************************************************************************************/
static VOID update_time_colon_animation()
{
    GX_RESOURCE_ID text_color;
   
    if (schedule_screen.schedule_screen_colon.gx_prompt_disabled_text_color == GX_COLOR_ID_GRAY)
    {
        text_color = GX_COLOR_ID_DARK_GRAY;
    }
    else
    {
        text_color = GX_COLOR_ID_GRAY;
    }

    gx_prompt_text_color_set(&schedule_screen.schedule_screen_colon, text_color, text_color, text_color);
}

/******************************************************************************************/
/* Override the default event processing of "schedule_screen" to handle signals from my   */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT schedule_screen_event_handler(GX_WINDOW* window, GX_EVENT* myevent)
{
    UINT status = GX_SUCCESS;

    switch (myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        update_date();

        gx_system_timer_start(window, TIME_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);

        template_event_handler(window, myevent);
        break;

    case GX_EVENT_HIDE:

        /* Call the event process of the template on which the vitals screen is based.  */
        template_event_handler(window, myevent);

        /* Stop all timers that belongs to the window. */
        gx_system_timer_stop((GX_WIDGET*)window, 0);
        break;

    case GX_SIGNAL(ID_ACTIVE_BALL, GX_EVENT_CLICKED):
        if (schedule_screen.schedule_screen_active_bar.gx_widget_parent == GX_NULL)
        {
            show_active_bar();
        }
        else
        {
            hide_active_bar();
        }
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        if (myevent->gx_event_sender == ID_R5_WIN_FADE_IN)
        {
            gx_system_timer_start(window, ACTIVE_CIRCLE_ANIMATION_TIMER, GX_TICKS_SECOND / 2, GX_TICKS_SECOND / 2);
        }
        break;

    case GX_EVENT_TIMER:
        switch (myevent->gx_event_payload.gx_event_timer_id)
        {
        case ACTIVE_CIRCLE_ANIMATION_TIMER:
            show_hide_active_circle();
            break;

        case TIME_TIMER:
            update_time_colon_animation();
            break;
        }
        break;

    default:
        status = template_event_handler(window, myevent);
        break;
    }

    return status;
}

/******************************************************************************************/
/* Custom schedule window draw function that draws the grid lines.                        */
/******************************************************************************************/
VOID schedule_win_draw(GX_WINDOW *window)
{
GX_RECTANGLE *client;
GX_RECTANGLE *size;
INT           xpos;
INT           ypos;
INT           xoffset;
INT           yoffset;
INT           index;

    /* Draw window background.  */
    gx_window_background_draw(window);

    client = &window->gx_window_client;

    xoffset = (schedule_screen.schedule_screen_c2_day.gx_widget_size.gx_rectangle_left - 
               schedule_screen.schedule_screen_c1_day.gx_widget_size.gx_rectangle_left);

    yoffset = (schedule_screen.schedule_screen_r2_hour.gx_widget_size.gx_rectangle_top -
               schedule_screen.schedule_screen_r1_hour.gx_widget_size.gx_rectangle_top);

    xpos = schedule_screen.schedule_screen_r1_am_pm.gx_widget_size.gx_rectangle_right + 5;
    ypos = schedule_screen.schedule_screen_r1_am_pm.gx_widget_size.gx_rectangle_bottom - 5;
 
    /* Draw horizontal lines.  */
    gx_context_brush_width_set(1);
    gx_context_line_color_set(GX_COLOR_ID_DARK_GRAY);

    for (index = 0; index < SCHEDULE_ROW_COUNT; index++)
    {
        gx_canvas_line_draw(xpos, ypos, client->gx_rectangle_right, ypos);

        ypos += yoffset;
    }

    xpos = schedule_screen.schedule_screen_c1_week.gx_widget_size.gx_rectangle_left - 10;
    ypos -= yoffset;

    /* Draw vertical lines.  */
    for (index = 0; index < SCHEDULE_COL_COUNT; index++)
    {

        gx_canvas_line_draw(xpos, client->gx_rectangle_top, xpos, ypos);

        xpos += xoffset;
    }

    /* Draw current time line.  */
    gx_context_line_color_set(GX_COLOR_ID_RED);
    size = &schedule_screen.schedule_screen_current_time.gx_widget_size;

    xpos = schedule_screen.schedule_screen_r1_am_pm.gx_widget_size.gx_rectangle_right + 5;
    ypos = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
    gx_canvas_line_draw(xpos, ypos, size->gx_rectangle_left - 1, ypos);

    xpos = size->gx_rectangle_right + 1;
    gx_canvas_line_draw(xpos, ypos, client->gx_rectangle_right, ypos);

    /* Draw child widgets.  */
    gx_widget_children_draw(window);
}
