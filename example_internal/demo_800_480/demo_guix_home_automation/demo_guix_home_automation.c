/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_home_automation.h"

/* Define the ThreadX demo thread control block and stack.  */
TX_BYTE_POOL       memory_pool;

#define CLOCK_TIMER         20

#define SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 2)

/* Define memory for memory pool. */
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID clock_update();
VOID fade_in_home_window();
VOID fade_out_home_window();
VOID toggle_screen(GX_WIDGET *new_screen);

/* Define animation variable for screen slide animation.  */
GX_ANIMATION slide_animation;

/* Define GX_PIXELMAP type variable to receive pixelmap data decoded from background jpeg. */
GX_PIXELMAP main_screen_bg;

/* Define application varaible to record current screen and overrall energy used today. */
APP_INFO app_info = { (GX_WIDGET *)&main_screen.main_screen_home_window, GX_NULL, 8746594 };

const GX_CHAR *day_names[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

const GX_CHAR *month_names[12] = {
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

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/******************************************************************************************/
/* Define memory allocator function.                                                     */
/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
/* Define memory de-allocator function.                                                   */
/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{

    /* create byte pool. */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    guix_setup();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID  guix_setup()
{
    /* Initialize GUIX. */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* Create the main screen and attach it to root window. */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* The screen control blocks in this case are statically defined, so
       we can create them all at startup. It is also an option to dynamically
       allocate the control block memory.
    */

    gx_studio_named_widget_create("lights_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("lights_page_1", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("lights_page_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("lights_page_3", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("lights_page_4", GX_NULL, GX_NULL);
    lights_screen_init();

    gx_studio_named_widget_create("locks_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("locks_page_1", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("locks_page_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("locks_page_3", GX_NULL, GX_NULL);
    locks_screen_init();

    gx_studio_named_widget_create("thermostat_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("thermostat_page_1", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("thermostat_page_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("thermostat_page_3", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("thermostat_page_4", GX_NULL, GX_NULL);
    thermostat_screen_init();

    gx_studio_named_widget_create("weather_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("win_Las_Vegas", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("win_New_York", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("win_San_Diego", GX_NULL, GX_NULL);
    weather_screen_init();

    gx_studio_named_widget_create("passcode_screen", GX_NULL, GX_NULL);

    /* Create slide animation control block.  */
    gx_animation_create(&slide_animation);

    /* Initialize "main_screen_by" varaible. */
    memset(&main_screen_bg, 0, sizeof(GX_PIXELMAP));

    /* Show the root window to make it and main screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        clock_update();

        /* Start a timer to update current time. */
        gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);

        /* Call default event process. */
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_LIGHTS, GX_EVENT_CLICKED):
        on_footer_menu_clicked(ID_FOOTER_LIGHTS);
        break;

    case GX_SIGNAL(ID_THERMOSTAT, GX_EVENT_CLICKED):
        on_footer_menu_clicked(ID_FOOTER_THERMOSTAT);
        break;

    case GX_SIGNAL(ID_LOCKS, GX_EVENT_CLICKED):
        on_footer_menu_clicked(ID_FOOTER_LOCKS);
        break;

    case GX_SIGNAL(ID_WEATHER, GX_EVENT_CLICKED):
        on_footer_menu_clicked(ID_FOOTER_WEATHER);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_TIMER)
        {
            clock_update();
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Decode main screen background jpeg.                                                    */
/* The background jpeg is saved in the resource data in "raw" format, meaning it is still */
/* a jpeg encoded graphic image. We need to decode and color space convert the raw jpeg   */
/* data to create a GUIX pixelmap before we can draw the image.                           */ 
/******************************************************************************************/
static VOID decode_main_screen_jpeg()
{
    GX_IMAGE_READER reader;
    GX_PIXELMAP *map;

    /* get a pointer to the raw jpeg data */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_HOME_BG, &map);

    /* create an image reader object */
    gx_image_reader_create(&reader, map->gx_pixelmap_data, map->gx_pixelmap_data_size, GX_COLOR_FORMAT_24XRGB, 0);

    /* decode and color space convert the jpeg to produce a GUIX compatible pixelmap image */
    gx_image_reader_start(&reader, &main_screen_bg);
}

/******************************************************************************************/
/* Override the default draw function of "main_screen".                                   */
/******************************************************************************************/
VOID main_screen_draw(GX_WINDOW *window)
{
    gx_window_background_draw(window);

    /* If this is the first time drawing, then we need to decompress the raw jpeg
       image we use to paint the screen background
    */
    if (!main_screen_bg.gx_pixelmap_data)
    {
        decode_main_screen_jpeg();
    }

    /* unless something went wrong, the pixelmap data should now be populated in 
       GUIX pixelmap format
    */
    if (main_screen_bg.gx_pixelmap_data)
    {
        /* Draw background map. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left,
            window->gx_widget_size.gx_rectangle_top, &main_screen_bg);
    }
    gx_widget_children_draw(window);
}


/******************************************************************************************/
/* Fade out home window from main screen.                                                 */
/******************************************************************************************/
VOID fade_out_home_window()
{
    GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* "USER_EVENT_FADE_OUT_HOME_WINDOW" is an user defined event that will trigger a
       set of actions. You are able to define those events and actions in studio. */
    my_event.gx_event_type = USER_EVENT_FADE_OUT_HOME_WINDOW;
    my_event.gx_event_target = (GX_WIDGET *)&main_screen;
    gx_system_event_send(&my_event);
}

/******************************************************************************************/
/* Fade out home window to main screen.                                                   */
/******************************************************************************************/
VOID fade_in_home_window(int event_type)
{
    GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* "USER_EVENT_FADE_IN_HOME_WINDOW" is an user defined event that will trigger a
    set of actions. You are able to define those events and actions in studio. */
    my_event.gx_event_type = USER_EVENT_FADE_IN_HOME_WINDOW;
    my_event.gx_event_target = (GX_WIDGET *)&main_screen;
    gx_system_event_send(&my_event);
}

/******************************************************************************************/
/* Switch to a new screen.                                                                */
/******************************************************************************************/
VOID toggle_screen(GX_WIDGET *new_screen)
{
    if (!new_screen || new_screen->gx_widget_id == app_info.current_screen->gx_widget_id)
    {
        /* no change. */
        return;
    }

    /* Reset status of current screen. */
    switch (app_info.current_screen->gx_widget_id)
    {
    case ID_LIGHTS_SCREEN:
        lights_screen_reset();
        break;

    case ID_THERMOSTAT_SCREEN:
        thermostat_screen_reset();
        break;

    case ID_LOCKS_SCREEN:
        locks_screen_reset();
        break;

    case ID_WEATHER_SCREEN:
        weather_screen_reset();
        break;
    }

    if (app_info.current_screen == (GX_WIDGET *)&main_screen.main_screen_home_window)
    {
        /* If current screen is home window, fade out home window. */
        fade_out_home_window();
    }
    else
    {
        /* Detach current screen. */
        gx_widget_detach(app_info.current_screen);
    }
    
    /* Attach new screen. */
    gx_widget_attach((GX_WIDGET *)&main_screen, new_screen);

    /* Start child widget animations for new screen. */
    switch (new_screen->gx_widget_id)
    {
    case ID_LIGHTS_SCREEN:
        lights_screen_animation_start();
        break;

    case ID_THERMOSTAT_SCREEN:
        thermostat_screen_animation_start();
        break;

    case ID_LOCKS_SCREEN:
        locks_screen_animation_start();
        break;

    case ID_WEATHER_SCREEN:
        weather_screen_animation_start();
        break;
    }

    app_info.previous_screen = app_info.current_screen;
    app_info.current_screen = new_screen;
}

/******************************************************************************************/
/* Update clock of main screen.                                                           */
/******************************************************************************************/
VOID clock_update()
{
#ifdef WIN32
    GX_RESOURCE_ID am_pm_id;
    GX_CHAR date_string[20];
    GX_STRING string;

    SYSTEMTIME local_time;
    GetLocalTime(&local_time);
    if (local_time.wHour < 12)
    {
        am_pm_id = GX_STRING_ID_AM;
    }
    else
    {
        am_pm_id = GX_STRING_ID_PM;
    }
    gx_numeric_prompt_value_set(&main_screen.main_screen_hour, (GX_VALUE)local_time.wHour);
    gx_numeric_prompt_value_set(&main_screen.main_screen_minute, (GX_VALUE)local_time.wMinute);
    gx_prompt_text_id_set(&main_screen.main_screen_am_pm, am_pm_id);

    sprintf(date_string, "%s, %s %02d", day_names[local_time.wDayOfWeek], month_names[local_time.wMonth - 1], local_time.wDay);
    string.gx_string_ptr = date_string;
    string.gx_string_length = sizeof(date_string) - 1;
    gx_prompt_text_set_ext(&main_screen.main_screen_date, &string);

    if (local_time.wSecond & 0x1)
    {
        gx_widget_fill_color_set(&main_screen.main_screen_lower_dot, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_widget_fill_color_set(&main_screen.main_screen_upper_dot, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    }
    else
    {
        gx_widget_fill_color_set(&main_screen.main_screen_lower_dot, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        gx_widget_fill_color_set(&main_screen.main_screen_upper_dot, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    }

#else

    /* add embedded target RTC retrieval here */
#endif

    app_info.overall_energy_today += 5;

    /* Display the overall energy in use. */
    gx_numeric_prompt_value_set(&lights_screen.lights_screen_overall_today, app_info.overall_energy_today);
    gx_numeric_prompt_value_set(&thermostat_screen.thermostat_screen_overall_today, app_info.overall_energy_today);
}

/******************************************************************************************/
/* Custom defined draw function for radial slider widget.                                 */
/* This custom radial slider draw function is to support filling selected circular        */
/* track with pixelmap.                                                                   */
/******************************************************************************************/
VOID custom_radial_slider_draw(GX_RADIAL_SLIDER *slider)
{
    GX_PIXELMAP           *map = GX_NULL;
    GX_RESOURCE_ID         map_id;
    GX_RADIAL_SLIDER_INFO *info = &slider->gx_radial_slider_info;
    INT                    xpos;
    INT                    ypos;
    INT                    angle = info->gx_radial_slider_info_current_angle;
    GX_POINT               inner_pt;
    GX_POINT               outer_pt;
    GX_RECTANGLE           dirty[3];
    GX_CANVAS             *canvas;
    GX_RECTANGLE          *size;
    GX_VALUE               xcenter;
    GX_VALUE               ycenter;

    /* Call default widget draw. */
    gx_widget_background_draw((GX_WIDGET *)slider);

    size = &slider->gx_widget_size;

    /* Pick up the pointer to the background pixelmap. */
    gx_context_pixelmap_get(info->gx_radial_slider_info_background_pixelmap, &map);

    if (map)
    {
        /* Draw background pixelmap. */
        gx_canvas_pixelmap_draw(size->gx_rectangle_left, size->gx_rectangle_top, map);
    }

    /* Calculate radial slider center coordinates. */
    xcenter = (GX_VALUE)(info->gx_radial_slider_info_xcenter + size->gx_rectangle_left);
    ycenter = (GX_VALUE)(info->gx_radial_slider_info_ycenter + size->gx_rectangle_top);

    /* Get the point on the inner circle of the track with given center point, radius and angle. */
    gx_utility_circle_point_get(xcenter,
        ycenter,
        info->gx_radial_slider_info_radius - (info->gx_radial_slider_info_track_width >> 1),
        angle,
        &inner_pt);

    /* Get the point on the outer circle of the track with given center point, radius and angle. */
    gx_utility_circle_point_get(xcenter,
        ycenter,
        info->gx_radial_slider_info_radius + (info->gx_radial_slider_info_track_width >> 1),
        angle,
        &outer_pt);

    /* Pickup drawing canvas. */
    gx_widget_canvas_get(slider, &canvas);

    if (angle != info->gx_radial_slider_info_max_angle)
    {
        map_id = light_selected_map_id_get(slider);

        if (!map_id)
        {
            map_id = GX_PIXELMAP_ID_WHEEL_THERMOSTAT;
        }

        /* Get the pointer of the pixelmap for filling the selected circular track. */
        gx_context_pixelmap_get(map_id, &map);

        if (map)
        {
            xpos = size->gx_rectangle_left + 5;
            ypos = size->gx_rectangle_top + 3;

            memset(dirty, 0, sizeof(dirty));

            /* Caculate clipping areas of the selected circular track. */
            if (angle >= 180)
            {
                dirty[0].gx_rectangle_left = size->gx_rectangle_left;
                dirty[0].gx_rectangle_top = outer_pt.gx_point_y;
                dirty[0].gx_rectangle_right = xcenter;
                dirty[0].gx_rectangle_bottom = size->gx_rectangle_bottom;

                dirty[1].gx_rectangle_left = (inner_pt.gx_point_x + outer_pt.gx_point_x) >> 1;
                dirty[1].gx_rectangle_top = inner_pt.gx_point_y;
                dirty[1].gx_rectangle_right = xcenter;
                dirty[1].gx_rectangle_bottom = outer_pt.gx_point_y - 1;
            }
            else if (angle >= 90)
            {
                dirty[0].gx_rectangle_left = size->gx_rectangle_left;
                dirty[0].gx_rectangle_top = inner_pt.gx_point_y;
                dirty[0].gx_rectangle_right = xcenter;
                dirty[0].gx_rectangle_bottom = size->gx_rectangle_bottom;

                dirty[1].gx_rectangle_left = size->gx_rectangle_left;
                dirty[1].gx_rectangle_top = outer_pt.gx_point_y;
                dirty[1].gx_rectangle_right = (inner_pt.gx_point_x + outer_pt.gx_point_x) >> 1;
                dirty[1].gx_rectangle_bottom = inner_pt.gx_point_y - 1;
            }
            else if (angle >= 0)
            {
                dirty[0].gx_rectangle_left = size->gx_rectangle_left;
                dirty[0].gx_rectangle_top = size->gx_rectangle_top;
                dirty[0].gx_rectangle_right = xcenter;
                dirty[0].gx_rectangle_bottom = size->gx_rectangle_bottom;

                dirty[1].gx_rectangle_left = xcenter + 1;
                dirty[1].gx_rectangle_top = size->gx_rectangle_top;
                dirty[1].gx_rectangle_right = size->gx_rectangle_right;
                dirty[1].gx_rectangle_bottom = outer_pt.gx_point_y;

                dirty[2].gx_rectangle_left = xcenter + 1;
                dirty[2].gx_rectangle_top = outer_pt.gx_point_y + 1;
                dirty[2].gx_rectangle_right = (inner_pt.gx_point_x + outer_pt.gx_point_x) >> 1;
                dirty[2].gx_rectangle_bottom = inner_pt.gx_point_y;
            }
            else
            {
                dirty[0].gx_rectangle_left = size->gx_rectangle_left;
                dirty[0].gx_rectangle_top = size->gx_rectangle_top;
                dirty[0].gx_rectangle_right = xcenter;
                dirty[0].gx_rectangle_bottom = size->gx_rectangle_bottom;

                dirty[1].gx_rectangle_left = xcenter + 1;
                dirty[1].gx_rectangle_top = size->gx_rectangle_top;
                dirty[1].gx_rectangle_right = size->gx_rectangle_right;
                dirty[1].gx_rectangle_bottom = inner_pt.gx_point_y;

                dirty[2].gx_rectangle_left = (inner_pt.gx_point_x + outer_pt.gx_point_x) >> 1;
                dirty[2].gx_rectangle_top = inner_pt.gx_point_y + 1;
                dirty[2].gx_rectangle_right = size->gx_rectangle_right;
                dirty[2].gx_rectangle_bottom = outer_pt.gx_point_y;
            }

            /* Draw selected circular track with clipping. */
            if (dirty[0].gx_rectangle_right > dirty[0].gx_rectangle_left &&
                dirty[0].gx_rectangle_bottom > dirty[0].gx_rectangle_top)
            {
                gx_canvas_drawing_initiate(canvas, slider, &dirty[0]);
                gx_canvas_pixelmap_draw(xpos, ypos, map);
                gx_canvas_drawing_complete(canvas, GX_FALSE);
            }

            if (dirty[1].gx_rectangle_right > dirty[1].gx_rectangle_left &&
                dirty[1].gx_rectangle_bottom > dirty[1].gx_rectangle_top)
            {
                gx_canvas_drawing_initiate(canvas, slider, &dirty[1]);
                gx_canvas_pixelmap_draw(xpos, ypos, map);
                gx_canvas_drawing_complete(canvas, GX_FALSE);
            }

            if (dirty[2].gx_rectangle_right > dirty[2].gx_rectangle_left &&
                dirty[2].gx_rectangle_bottom > dirty[2].gx_rectangle_top)
            {
                gx_canvas_drawing_initiate(canvas, slider, &dirty[2]);
                gx_canvas_pixelmap_draw(xpos, ypos, map);
                gx_canvas_drawing_complete(canvas, GX_FALSE);
            }
        }
    }

    /* Pick up the pointer of the needle pixelmap. */
    gx_context_pixelmap_get(slider->gx_radial_slider_info.gx_radial_slider_info_needle_pixelmap, &map);

    if (map)
    {
        /* Calculate start coordicates for needle draw. */
        xpos = (inner_pt.gx_point_x + outer_pt.gx_point_x) >> 1;
        ypos = (inner_pt.gx_point_y + outer_pt.gx_point_y) >> 1;

        xpos -= 15;
        ypos -= 15;

        /* Define needle draw area. */
        gx_utility_rectangle_define(&dirty[0], xpos, ypos, xpos + map->gx_pixelmap_width - 1, ypos + map->gx_pixelmap_height - 1);

        /* Initiates drawing on the specified canvas. */
        gx_canvas_drawing_initiate(canvas, slider, &dirty[0]);

        /* Draw needle pixelmap. */
        gx_canvas_pixelmap_draw(xpos, ypos, map);

        /* Completes drawing on the specified canvas. */
        gx_canvas_drawing_complete(canvas, GX_FALSE);
    }

    /* Draw children widgets. */
    gx_widget_children_draw((GX_WIDGET *)slider);
}

/******************************************************************************************/
/* Value format function for "hour" and "minute" prompt.                                  */
/******************************************************************************************/
VOID time_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    INT index = 0;

    sprintf(prompt->gx_numeric_prompt_buffer, "%02d", value);
    if (value < 10)
    {
        prompt->gx_numeric_prompt_buffer[index++] = '0';
    }

    /* Convert an integer value to ASCII string. */
    gx_utility_ltoa(value, prompt->gx_numeric_prompt_buffer + index, GX_NUMERIC_PROMPT_BUFFER_SIZE);
}

/******************************************************************************************/
/* Calculate string length.                                                               */
/******************************************************************************************/
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
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
