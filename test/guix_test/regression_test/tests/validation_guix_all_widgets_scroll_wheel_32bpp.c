/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_scroll_wheel_32bpp", /* Test name */
    100, 117, 525, 336  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

char test_comment[256];

GX_CONST GX_CHAR *string_list_1[12] = {
"Jan",
"Feb",
"Mar",
"Apr",
"May",
"Jun",
"Jul",
"Aug",
"Sep",
"Oct",
"Nov",
"Dec"
};

GX_CONST GX_CHAR *string_list_2[20] = {
"one",
"two",
"three",
"four",
"five",
"six",
"seven",
"eight",
"nine",
"ten",
"eleven",
"twelve",
"thirteen",
"fourteen",
"fifteen",
"sisteen",
"seventeen",
"eighteen",
"nineteen",
"twenty"
};

GX_CONST GX_CHAR *string_list_3[7] = {
"Monday",
"Tuesday",
"Wednesday",
"Thursday",
"Friday",
"Satday",
"Sunday"
};

GX_RESOURCE_ID id_list[7] = {
    GX_STRING_ID_STRING_1,
    GX_STRING_ID_STRING_10,
    GX_STRING_ID_STRING_11,
    GX_STRING_ID_STRING_12,
    GX_STRING_ID_STRING_13,
    GX_STRING_ID_STRING_14,
    GX_STRING_ID_STRING_15
};

TX_SEMAPHORE semaphore_memory;

static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_EVENT my_event;
GX_NUMERIC_SCROLL_WHEEL *day_wheel;
GX_NUMERIC_SCROLL_WHEEL *year_wheel;
GX_STRING_SCROLL_WHEEL *month_wheel;
INT drag_distance;
INT row_height;
INT index;

    memset(&my_event, 0, sizeof(GX_EVENT));
    selected_semaphore = &semaphore_memory;

    tx_semaphore_create(selected_semaphore, "test_semaphore", 0);

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    /* Search 3 wheel widget. */
    gx_widget_find(pScrollWheelScreen, ID_DAY_WHEEL, GX_SEARCH_DEPTH_INFINITE, &day_wheel);
    gx_widget_find(pScrollWheelScreen, ID_YEAR_WHEEL, GX_SEARCH_DEPTH_INFINITE, &year_wheel);
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "create scroll wheel widgets");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_widget_style_remove((GX_WIDGET *)month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "remove \"round\" style for month wheel");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    row_height = day_wheel->gx_scroll_wheel_row_height;

    /* Drag day wheel one item up. */

    for(drag_distance = row_height; drag_distance < row_height * 4; drag_distance += row_height/5)
    {
        /* Simulate pen down event. */
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (day_wheel->gx_widget_size.gx_rectangle_left +
                                                                   day_wheel->gx_widget_size.gx_rectangle_right) / 2;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (day_wheel->gx_widget_size.gx_rectangle_top +
                                                                   day_wheel->gx_widget_size.gx_rectangle_bottom) / 2;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);

        tx_thread_sleep(5);

        /* Simulate pen drag event. */
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= drag_distance;
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        gx_system_event_send(&my_event);
    
        tx_thread_sleep(11);

        /* Simulate pen up event. */
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        tx_thread_sleep(5);
   
        tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);
 
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "drag day wheel up by %d", drag_distance);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Drag month wheel down. */

    for(drag_distance = row_height; drag_distance < row_height * 4; drag_distance += row_height / 5)
    {
        /* Simulate pen down event. */
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (month_wheel->gx_widget_size.gx_rectangle_left +
                                                                   month_wheel->gx_widget_size.gx_rectangle_right) / 2;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (month_wheel->gx_widget_size.gx_rectangle_top +
                                                                   month_wheel->gx_widget_size.gx_rectangle_bottom) / 2;
        gx_system_event_send(&my_event);
        GX_EXIT_CRITICAL
        tx_thread_sleep(5);

        /* Simulate pen drag event. */
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y += drag_distance;
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        gx_system_event_send(&my_event);
        tx_thread_sleep(11);

        /* Simulate pen up event. */
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
        tx_thread_sleep(5);

        tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "drag month wheel down by %d", drag_distance);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Simulate flick event. */
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -GX_FIXED_VAL_MAKE(18);
    my_event.gx_event_target = (GX_WIDGET *)month_wheel;
    gx_system_event_send(&my_event);

    tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "flick to scroll, flick speed = 4608");

    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    for(index = 0; index < 23; index++)
    {
        switch(index)
        {
        case 0:
            /* Test numeric scroll wheel range set. */
            gx_numeric_scroll_wheel_range_set(year_wheel, 2300, 1900);
            sprintf(test_comment, "set range value of year wheel to [2300, 1900]");
            break;
        case 1:
            /* Text scroll wheel total rows set. */
            gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *)year_wheel, 10);
            sprintf(test_comment, "set total rows of year wheel to 10.");
            break;
        case 2:
            gx_widget_style_remove((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_RIGHT);
            sprintf(test_comment, "set text alighment of year wheel to right");
            break;
        case 3:
            gx_widget_style_remove((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_LEFT);
            sprintf(test_comment, "set text alighment of year wheel to left");
            break;
        case 4:
            /* Test text scroll wheel flat draw. */
            gx_widget_style_remove((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
            sprintf(test_comment, "remove \"round\" style of year wheel");
            break;
        case 5:
            gx_widget_style_remove((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_RIGHT);
            sprintf(test_comment, "set text alighment of year wheel to right");
            break;
        case 6:
            gx_widget_style_remove((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add((GX_WIDGET *)year_wheel, GX_STYLE_TEXT_LEFT);
            sprintf(test_comment, "set text alighment of year wheel to left");
            break;
        case 7:
            /* test scroll wheel selected background set. */
            gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *)year_wheel, GX_NULL);
            sprintf(test_comment, "remove selected background year wheel");
            break;
        case 8:
            /* Test gradient alpha set. */
            gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)year_wheel, 200, 100);
            sprintf(test_comment, "Set fading alpha of year wheel to [200, 100]");
            break;
        case 9:
            gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)year_wheel, 0, 0);
            sprintf(test_comment, "Set fading alpha of year wheel to [0, 0]");
            break;
        case 10:
            gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)year_wheel, 0, 200);
            sprintf(test_comment, "Set fading alpha of year wheel to [0, 200]");
            break;
        case 11:
            gx_scroll_wheel_row_height_set((GX_SCROLL_WHEEL *)year_wheel, 35);
            sprintf(test_comment, "Set row height of year wheel to 35");
            break;
        case 12:
            /* Test background set. */
            gx_window_wallpaper_set((GX_WINDOW *)year_wheel, GX_PIXELMAP_ID_FISH, GX_FALSE);
            sprintf(test_comment, "Set year wheel background");
            break;
        case 13:
            /* Test text copy style. */
            gx_widget_style_add((GX_WIDGET *)month_wheel, GX_STYLE_TEXT_COPY);
            gx_string_scroll_wheel_string_list_set(month_wheel, string_list_1, 12);
            sprintf(test_comment, "Test text copy, set a string list that have shorter strings.");
            break;
        case 14:
            gx_string_scroll_wheel_string_list_set(month_wheel, string_list_2, 14);
            sprintf(test_comment, "Test text copy, set a string list that have more string count");
            break;
        case 15:
            gx_string_scroll_wheel_string_list_set(month_wheel, string_list_3, 7);
            sprintf(test_comment, "Test text copy, set a string list that have less string count");
            break;
        case 16:
            gx_string_scroll_wheel_string_id_list_set(month_wheel, id_list, 7);
            sprintf(test_comment, "Set string id list");
            break;
        case 17:
            gx_widget_hide((GX_WIDGET *)month_wheel);
            gx_widget_style_remove((GX_WIDGET *)month_wheel, GX_STYLE_TEXT_COPY);
            gx_string_scroll_wheel_string_list_set(month_wheel, string_list_3, 7);
            gx_widget_show((GX_WIDGET *)month_wheel);
            sprintf(test_comment, "Remove text copy style, and set a new string list");
            break;
        case 18:
            gx_text_scroll_wheel_font_set((GX_TEXT_SCROLL_WHEEL *)month_wheel, GX_FONT_ID_SELECTED_FONT, GX_FONT_ID_NORMAL_FONT);
            sprintf(test_comment, "Set new font for month wheel");
            break;
        case 19:
            gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *)month_wheel, GX_COLOR_ID_ORANGE, GX_COLOR_ID_BLACK, GX_COLOR_ID_ORANGE);
            sprintf(test_comment, "Set new color for month wheel");
            break;
        case 20:
            gx_numeric_scroll_wheel_range_set(year_wheel, 2012, 2020);
            sprintf(test_comment, "set range value of year wheel to [2012, 2020]");
            break;
        case 21:
            gx_numeric_scroll_wheel_range_set(year_wheel, 2020, 2012);
            sprintf(test_comment, "set range value of year wheel to [2020, 2012]");
            break;
        case 22:
            gx_scroll_wheel_total_rows_set(year_wheel, 1);
            sprintf(test_comment, "set total rows of year wheel to 1");
            while((year_wheel->gx_scroll_wheel_animation_steps) ||
                  (year_wheel->gx_scroll_wheel_selected_yshift))
            {
                tx_thread_sleep(20);
            
            }
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    tx_semaphore_delete(selected_semaphore);

    gx_widget_hide((GX_WIDGET *)month_wheel);
    gx_widget_style_add((GX_WIDGET *)month_wheel, GX_STYLE_TEXT_COPY);
    gx_string_scroll_wheel_string_list_set(month_wheel, string_list_1, 12);
    gx_widget_delete((GX_WIDGET *)month_wheel);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

