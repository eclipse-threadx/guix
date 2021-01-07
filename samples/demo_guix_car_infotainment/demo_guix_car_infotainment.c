/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

/* Define the ThreadX demo thread control block and stack.  */
TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       memory_pool;

#define            CAR_LIGHTS_ANIMATION_TIMER 10
#define            ALPHA_ANIMATION_TIMER      20
#define            CLOCK_TIMER                30
#define            LIGHTS_TIMER_VAL           5

#define  SCRATCHPAD_PIXELS (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION) * 5

/* Define memory for memory pool. */
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_WINDOW_ROOT    *root;

FILE              *p_file = GX_NULL;

INT blend_alpha = 0xff;
INT blend_alpha_increment;

GX_PIXELMAP    decoded_map;
GX_PIXELMAP    resized_map;

/* Define car light resize scale, the scale is larged by 256 for precision needs. */
INT car_light_resize_scale = 256;
INT car_light_resize_scale_increment = 0;

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID screen_toggle(GX_WINDOW *new_win, GX_WINDOW *old_win);
VOID car_light_pixelmap_resize();
VOID jpg_decode(GX_RESOURCE_ID res_id, GX_PIXELMAP *decoded_map);
VOID map_delete(GX_PIXELMAP *map);
VOID alpha_animation_update();
VOID widgets_enable_disable();
VOID clock_update();

GX_WIDGET *main_screen_buttons[] = {
    (GX_WIDGET *)&main_screen.main_screen_button_navigation,
    (GX_WIDGET *)&main_screen.main_screen_button_audio,
    (GX_WIDGET *)&main_screen.main_screen_button_video,
    (GX_WIDGET *)&main_screen.main_screen_button_vehicle,
    (GX_WIDGET *)&main_screen.main_screen_button_phone,
    (GX_WIDGET *)&main_screen.main_screen_button_settings,
    (GX_WIDGET *)&main_screen.base.screen_base_top_button_sound,
    (GX_WIDGET *)&main_screen.base.screen_base_top_button_camera,
    (GX_WIDGET *)&main_screen.base.screen_base_top_button_home,
    GX_NULL
};

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
    "January",
    "Febuary",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
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
/* Define memory allocator function.                                                      */
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

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* Create the gauge_window */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Create navigation window. */
    gx_studio_named_widget_create("navigation_screen", GX_NULL, GX_NULL);
    gx_icon_pixelmap_set(&navigation_screen.base.screen_base_logo, GX_NULL, GX_NULL);

    /* Create audio screen. */
    gx_studio_named_widget_create("audio_screen", GX_NULL, GX_NULL);
    wave_slider_create();

    /* Create video screen. */
    gx_studio_named_widget_create("video_screen", GX_NULL, GX_NULL);

    /* Create vehicle scrreen. */
    gx_studio_named_widget_create("vehicle_screen", GX_NULL, GX_NULL);

    /* Create speaker window. */
    gx_studio_named_widget_create("speaker_window", GX_NULL, GX_NULL);

    memset(&decoded_map, 0, sizeof(GX_PIXELMAP));
    memset(&resized_map, 0, sizeof(GX_PIXELMAP));

    /* Create camera window. */
    gx_studio_named_widget_create("camera_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Override the default drawing of "main_screen" to do custom drawing.                    */
/******************************************************************************************/
VOID main_screen_draw(GX_WINDOW *window)
{
    if (decoded_map.gx_pixelmap_data)
    {
        /* Draw background pixelmap. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left, window->gx_widget_size.gx_rectangle_top, &decoded_map);
    }

    gx_window_draw(window);
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

        /* Call default window event process. */
        base_screen_event_process(window, event_ptr);

        jpg_decode(GX_PIXELMAP_ID_ROAD_BG, &decoded_map);

        /* Start car lights animation timer. */
        gx_system_timer_start((GX_WIDGET *)window, CAR_LIGHTS_ANIMATION_TIMER, LIGHTS_TIMER_VAL, LIGHTS_TIMER_VAL);

        /* Start a timer to update current time. */
        gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
        break;

    case GX_EVENT_HIDE:
        /* Stop car lights animation timer. */
        gx_system_timer_stop((GX_WIDGET *)window, CAR_LIGHTS_ANIMATION_TIMER);

        /* Call default window event process. */
        base_screen_event_process(window, event_ptr);
        break;

    case GX_EVENT_TIMER:
        switch(event_ptr->gx_event_payload.gx_event_timer_id)
        {
        case CAR_LIGHTS_ANIMATION_TIMER:
            if (car_light_resize_scale >= 256)
            {
                car_light_resize_scale_increment = -25;
            }
            else if (car_light_resize_scale <= 25)
            {
                car_light_resize_scale_increment = 25;
            }
            
            car_light_resize_scale += car_light_resize_scale_increment;
            car_light_pixelmap_resize();
            break;

        case ALPHA_ANIMATION_TIMER:
            alpha_animation_update();
            break;

        case CLOCK_TIMER:
            /* Update current time. */
            clock_update();
            break;
        }
        break;

    case GX_SIGNAL(ID_BOTTOM_NAVIGATION, GX_EVENT_CLICKED):
        /* Toggle to navigation screen. */
        screen_toggle((GX_WINDOW *)&navigation_screen, window);
        break;

    case GX_SIGNAL(ID_BOTTOM_AUDIO, GX_EVENT_CLICKED):
        /* Toggle to audio screen. */
        screen_toggle((GX_WINDOW *)&audio_screen, window);
        break;

    case GX_SIGNAL(ID_BOTTOM_VIDEO, GX_EVENT_CLICKED):
        /* Toggle to video screen. */
        screen_toggle((GX_WINDOW *)&video_screen, window);
        break;

    case GX_SIGNAL(ID_BOTTOM_VEHICLE, GX_EVENT_CLICKED):
        /* Toggle to vehicle screen. */
        screen_toggle((GX_WINDOW *)&vehicle_screen, window);
        break;

    case GX_SIGNAL(ID_BOTTOM_OFF, GX_EVENT_CLICKED):
        if (main_screen.main_screen_button_off_text.gx_prompt_text_id == GX_STRING_ID_OFF)
        {
            gx_system_timer_stop((GX_WIDGET *)window, CAR_LIGHTS_ANIMATION_TIMER);
            gx_prompt_text_id_set(&main_screen.main_screen_button_off_text, GX_STRING_ID_ON);
            blend_alpha_increment = -20;
        }
        else
        {
            gx_system_timer_start((GX_WIDGET *)window, CAR_LIGHTS_ANIMATION_TIMER, LIGHTS_TIMER_VAL, LIGHTS_TIMER_VAL);
            gx_prompt_text_id_set(&main_screen.main_screen_button_off_text, GX_STRING_ID_OFF);
            
            blend_alpha_increment = 20;
        }
        widgets_enable_disable();
        gx_system_timer_start((GX_WIDGET *)&main_screen, ALPHA_ANIMATION_TIMER, 1, 1);
        break;

    default:
        return base_screen_event_process(window, event_ptr);
    }

    return GX_SUCCESS;
}

/******************************************************************************************/
/* Override default drawing of menu buttons in the bottom of "main_screen" to do some     */
/* custom drawing.                                                                        */
/******************************************************************************************/
VOID custom_bottom_button_draw(GX_PIXELMAP_BUTTON *button)
{
GX_BRUSH    *brush;
INT          yoff = 4;
GX_PIXELMAP *map;
GX_WIDGET   *child;
GX_STRING    text;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = blend_alpha;

    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        yoff = 4;
        gx_context_pixelmap_get(button->gx_pixelmap_button_selected_id, &map);
    }
    else
    {
        yoff = 0;
        gx_context_pixelmap_get(button->gx_pixelmap_button_normal_id, &map);
    }

    if (button->gx_widget_id == ID_BOTTOM_OFF)
    {
        brush->gx_brush_alpha = 0xff;
    }

    if (map)
    {
        /* Offset button pixelmap draw. */
        gx_canvas_pixelmap_blend(button->gx_widget_size.gx_rectangle_left, button->gx_widget_size.gx_rectangle_top + yoff, map, brush->gx_brush_alpha);
    }

    child = button->gx_widget_first_child;
    while (child)
    {
        if (child->gx_widget_type == GX_TYPE_ICON)
        {
            /* Get icon pixelmap/ */
            gx_context_pixelmap_get(((GX_ICON *)child)->gx_icon_normal_pixelmap, &map);

            if (map)
            {
                /* Offset icon draw in button pixelmap. */
                gx_canvas_pixelmap_blend(child->gx_widget_size.gx_rectangle_left, child->gx_widget_size.gx_rectangle_top + yoff, map, brush->gx_brush_alpha);
            }
        }
        else if (child->gx_widget_type == GX_TYPE_PROMPT)
        {
            /* Get prompt text. */
            gx_prompt_text_get_ext((GX_PROMPT *)child, &text);

            if (text.gx_string_ptr)
            {
                /* Set text color. */
                gx_context_line_color_set(((GX_PROMPT *)child)->gx_prompt_normal_text_color);

                /* Set text font. */
                gx_context_font_set(((GX_PROMPT *)child)->gx_prompt_font_id);

                /* Offset text draw in button pixelmap. */
                gx_canvas_text_draw_ext(child->gx_widget_size.gx_rectangle_left, child->gx_widget_size.gx_rectangle_top + yoff, &text);
            }
        }

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Override default drawing of the light icons in "main_screen".                          */
/******************************************************************************************/
VOID car_light_icon_draw(GX_ICON *icon)
{
INT xpos;
INT ypos;

    if (main_screen.main_screen_button_off_text.gx_prompt_text_id == GX_STRING_ID_ON)
    {
        return;
    }

    if (car_light_resize_scale >= 0xff)
    {
        gx_icon_draw(icon);
    }
    else
    {
        xpos = (icon->gx_widget_size.gx_rectangle_left + icon->gx_widget_size.gx_rectangle_right) >> 1;
        xpos -= (resized_map.gx_pixelmap_width >> 1);

        ypos = (icon->gx_widget_size.gx_rectangle_top + icon->gx_widget_size.gx_rectangle_bottom) >> 1;
        ypos -= (resized_map.gx_pixelmap_height >> 1);

        if (resized_map.gx_pixelmap_data)
        {
            gx_canvas_pixelmap_draw(xpos, ypos, &resized_map);
        }
    }
}

/******************************************************************************************/
/* Override the default drawing of some prompts to draw the widget with specified blend   */
/* alpha.                                                                                 */
/******************************************************************************************/
VOID prompt_alpha_draw(GX_PROMPT *prompt)
{
GX_BRUSH *brush;
GX_STRING text;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = blend_alpha;

    gx_prompt_text_get_ext(prompt, &text);

    if (text.gx_string_ptr)
    {
        gx_widget_text_draw_ext(prompt, prompt->gx_prompt_normal_text_color, prompt->gx_prompt_font_id,
                                &text, 0, 0);
    }

    gx_widget_children_draw((GX_WIDGET *)prompt);
}

/******************************************************************************************/
/* Override the defaulting drawing of some icons to draw the widget with specified blend  */
/* alpha.                                                                                 */
/******************************************************************************************/
VOID icon_alpha_draw(GX_ICON *icon)
{
GX_PIXELMAP *map;

    /* Get icon pixelmap/ */
    gx_context_pixelmap_get(icon->gx_icon_normal_pixelmap, &map);

    if (map)
    {
        gx_canvas_pixelmap_blend(icon->gx_widget_size.gx_rectangle_left, icon->gx_widget_size.gx_rectangle_top, map, blend_alpha);
    }

    gx_widget_children_draw((GX_WIDGET *)icon);
}

/******************************************************************************************/
/* Detach one window and attach anothe window to root.                                    */
/******************************************************************************************/
VOID screen_toggle(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *)new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *)new_win);
    }
    gx_widget_detach((GX_WIDGET *)old_win);
}

/******************************************************************************************/
/* Resize the background of the car light icon.                                           */
/******************************************************************************************/
VOID car_light_pixelmap_resize()
{
INT          newwidth;
INT          newheight;
GX_PIXELMAP *map;

    gx_widget_pixelmap_get((GX_WIDGET *)&main_screen.main_screen_car_light_left,
        main_screen.main_screen_car_light_left.gx_icon_normal_pixelmap, &map);

    if (map)
    {
        /* Delete previous resized pixelmap. */
        map_delete(&resized_map);

        newwidth = (map->gx_pixelmap_width * car_light_resize_scale) >> 8;
        newheight = (map->gx_pixelmap_height * car_light_resize_scale) >> 8;

        /* Resize car light pixelmap. */
        gx_utility_pixelmap_resize(map, &resized_map, newwidth, newheight);

        /* Mark car light icon dirty. */
        gx_system_dirty_mark(&main_screen.main_screen_car_light_left);
        gx_system_dirty_mark(&main_screen.main_screen_car_light_right);
    }
}

/******************************************************************************************/
/* Decode a jpg image.                                                                    */
/******************************************************************************************/
VOID jpg_decode(GX_RESOURCE_ID res_id, GX_PIXELMAP *decoded_map)
{
GX_IMAGE_READER   image_reader;
GX_CONST GX_CHAR *jpg_data;
UINT              jpg_data_size;

    /* Delete previous decoded pixelmap. */
    map_delete(decoded_map);

    jpg_data = (GX_CONST GX_CHAR *)main_display_theme_1_pixelmap_table[res_id]->gx_pixelmap_data;
    jpg_data_size = main_display_theme_1_pixelmap_table[res_id]->gx_pixelmap_data_size;
    gx_image_reader_create(&image_reader, (GX_CONST GX_UBYTE *)jpg_data, jpg_data_size, GX_COLOR_FORMAT_24XRGB, GX_IMAGE_READER_MODE_ALPHA);
    gx_image_reader_start(&image_reader, decoded_map);
}

/******************************************************************************************/
/* Free memory for a pixelmap.                                                            */
/******************************************************************************************/
VOID map_delete(GX_PIXELMAP *map)
{
    if (map->gx_pixelmap_data)
    {
        memory_free((VOID *)map->gx_pixelmap_data);
        map->gx_pixelmap_data = GX_NULL;
    }

    if (map->gx_pixelmap_aux_data)
    {
        memory_free((VOID *)map->gx_pixelmap_aux_data);
        map->gx_pixelmap_aux_data = GX_NULL;
    }
}

/******************************************************************************************/
/* Update blend_alpha value for fading animation.                                         */
/******************************************************************************************/
VOID alpha_animation_update()
{
    blend_alpha += blend_alpha_increment;

    if ((blend_alpha < 20) || blend_alpha >= 255)
    {
        if (blend_alpha <= 20)
        {
            blend_alpha = 20;
        }
        else
        {
            blend_alpha = 255;
        }

        gx_system_timer_stop((GX_WIDGET *)&main_screen, ALPHA_ANIMATION_TIMER);
    }

    gx_system_dirty_mark((GX_WIDGET *)&main_screen);
}

/******************************************************************************************/
/* Add or Remove GX_STYLE_ENABLE style for the widgets in "main_screen"                   */
/******************************************************************************************/
VOID widgets_enable_disable()
{
GX_WIDGET **widget;

    widget = main_screen_buttons;

    if (main_screen.main_screen_button_off_text.gx_prompt_text_id == GX_STRING_ID_ON)
    {
        while (*widget)
        {
            gx_widget_style_remove(*widget, GX_STYLE_ENABLED);
            widget++;
        }
    }
    else
    {
        while (*widget)
        {
            gx_widget_style_add(*widget, GX_STYLE_ENABLED);
            widget++;
        }
    }
}

/******************************************************************************************/
/* Update clock of main screen.                                                           */
/******************************************************************************************/
VOID clock_update()
{
#ifdef WIN32
    GX_CHAR hour_string_buffer[4];
    GX_CHAR minute_string_buffer[4];
    GX_CHAR date_string_buffer[40];
    GX_STRING hour_string;
    GX_STRING minute_string;
    GX_STRING date_string;

    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    sprintf_s(hour_string_buffer, 4, "%02d", local_time.wHour);
    sprintf_s(minute_string_buffer, 4, "%02d", local_time.wMinute);
    sprintf_s(date_string_buffer, 40, "%s, %s %02d", day_names[local_time.wDayOfWeek], month_names[local_time.wMonth - 1], local_time.wDay);

    hour_string.gx_string_ptr = hour_string_buffer;
    hour_string.gx_string_length = 2;

    minute_string.gx_string_ptr = minute_string_buffer;
    minute_string.gx_string_length = 2;

    date_string.gx_string_ptr = date_string_buffer;
    date_string.gx_string_length = string_length_get(date_string_buffer, sizeof(date_string_buffer) - 1);

    gx_prompt_text_set_ext(&main_screen.main_screen_time_hour, &hour_string);
    gx_prompt_text_set_ext(&main_screen.main_screen_time_minute, &minute_string);
    gx_prompt_text_set_ext(&main_screen.main_screen_date, &date_string);

#else
#endif
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
