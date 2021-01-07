/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_industrial.h"

/* Define the ThreadX demo thread control block and stack.  */
TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       memory_pool;

#define CLOCK_TIMER                   20

#define SCRATCHPAD_PIXELS (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)

/* Define memory for memory pool. */
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];
GX_WINDOW_ROOT    *root;

USHORT toggle_animation_flag = ANIMATION_NONE;
GX_WINDOW *current_screen = (GX_WINDOW *)&main_screen;

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID clock_update();
UINT root_win_event_process(GX_WINDOW *root, GX_EVENT *event_ptr);

static GX_CONST GX_CHAR day_name_sun[] = "Sunday";
static GX_CONST GX_CHAR day_name_mon[] = "Monday";
static GX_CONST GX_CHAR day_name_tue[] = "Tuesday";
static GX_CONST GX_CHAR day_name_wed[] = "Wednesday";
static GX_CONST GX_CHAR day_name_thu[] = "Thursday";
static GX_CONST GX_CHAR day_name_fri[] = "Friday";
static GX_CONST GX_CHAR day_name_sat[] = "Saturday";
static GX_CONST GX_CHAR string_turn_on[] = "Turn On";
static GX_CONST GX_CHAR string_shut_off[] = "Shut Off";

const GX_STRING day_names[7] = {
    {day_name_sun, sizeof(day_name_sun) - 1},
    {day_name_mon, sizeof(day_name_mon) - 1},
    {day_name_tue, sizeof(day_name_tue) - 1},
    {day_name_wed, sizeof(day_name_wed) - 1},
    {day_name_thu, sizeof(day_name_thu) - 1},
    {day_name_fri, sizeof(day_name_fri) - 1},
    {day_name_sat, sizeof(day_name_sat) - 1}
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
VOID memory_free(VOID *mem)
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

    gx_widget_event_process_set(root, root_win_event_process);

    /* Create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Create sequence window. */
    gx_studio_named_widget_create("sequence_window", GX_NULL, GX_NULL);

    /* Create sequence number window. */
    gx_studio_named_widget_create("sequence_number", GX_NULL, GX_NULL);

    /* Create complete window. */
    gx_studio_named_widget_create("complete_window", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Override default event processing of root window to handle screen animation            */ 
/* complete events.                                                                       */
/******************************************************************************************/
UINT root_win_event_process(GX_WINDOW *root, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_ANIMATION_COMPLETE:
        switch (event_ptr->gx_event_sender)
        {
        case ANI_ID_SEQ_WIN_FADE_IN:
            current_screen = (GX_WINDOW *)&sequence_window;
            sequence_animation_start();
            toggle_animation_flag = ANIMATION_NONE;
            break;

        case ANI_ID_START_BTN_SLIDE_IN:
            current_screen = (GX_WINDOW *)&main_screen;
            toggle_animation_flag = ANIMATION_NONE;
            break;

        case ANI_ID_COMPLETE_WIN_FADE_IN:
            current_screen = (GX_WINDOW *)&complete_window;
            toggle_animation_flag = ANIMATION_NONE;
            break;
        }
        break;

    default:
        gx_window_event_process(root, event_ptr);
        break;
    }
    return GX_SUCCESS;
}

/******************************************************************************************/
/* Override default event processing of "main_screen" to handle signals from my child     */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    GX_STRING text;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        clock_update();
        gx_system_timer_start((GX_WIDGET *)&main_screen, CLOCK_TIMER, 20, 20);
        break;

    case GX_SIGNAL(ID_START, GX_EVENT_CLICKED):
        screen_toggle_animation_start((GX_WINDOW *)&sequence_window, current_screen);
        break;

    case GX_SIGNAL(ID_ON_OFF, GX_EVENT_CLICKED):
        gx_prompt_text_get_ext(&main_screen.main_screen_prompt_on_off, &text);
        if (strncmp(text.gx_string_ptr, "Turn On", text.gx_string_length) == 0)
        {
            screen_toggle_animation_start((GX_WINDOW *)&sequence_window, current_screen);
        }
        else
        {
            screen_toggle_animation_start((GX_WINDOW *)&main_screen, current_screen);
        }
        break;

    case GX_SIGNAL(ID_HOME, GX_EVENT_CLICKED):
        if (current_screen != (GX_WINDOW *)&main_screen)
        {
            screen_toggle_animation_start((GX_WINDOW *)&main_screen, current_screen);
        }
        break;

    case GX_SIGNAL(ID_INSPECTING, GX_EVENT_CLICKED):
        if (current_screen == (GX_WINDOW *)&sequence_window)
        {
            sequence_window.gx_widget_event_process_function((GX_WIDGET *)window, event_ptr);
        }
        else
        {
            button_indicator_attach(&main_screen.main_screen_button_inspecting);
        }
        break;

    case GX_SIGNAL(ID_ASSEMBLING, GX_EVENT_CLICKED):
        if (current_screen == (GX_WINDOW *)&sequence_window)
        {
            sequence_window.gx_widget_event_process_function((GX_WIDGET *)window, event_ptr);
        }
        else
        {
            button_indicator_attach(&main_screen.main_screen_button_assembling);
        }
        break;

    case GX_SIGNAL(ID_WELDING, GX_EVENT_CLICKED):
        if (current_screen == (GX_WINDOW *)&sequence_window)
        {
            sequence_window.gx_widget_event_process_function((GX_WIDGET *)window, event_ptr);
        }
        else
        {
            button_indicator_attach(&main_screen.main_screen_button_welding);
        }
        break;

    case GX_SIGNAL(ID_PALLETIZING, GX_EVENT_CLICKED):
        if (current_screen == (GX_WINDOW *)&sequence_window)
        {
            sequence_window.gx_widget_event_process_function((GX_WIDGET *)window, event_ptr);
        }
        else
        {
            button_indicator_attach(&main_screen.main_screen_button_palletizing);
        }
        break;

    case GX_SIGNAL(ID_REFRESH, GX_EVENT_CLICKED):
        if (sequence_window.gx_widget_status & GX_STATUS_VISIBLE)
        {
            sequence_window.gx_widget_event_process_function((GX_WIDGET *)window, event_ptr);
        }
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
/* Attach the indicator icon to the specified button.                                     */
/******************************************************************************************/
VOID button_indicator_attach(GX_PIXELMAP_BUTTON *button)
{
    GX_WIDGET *indicator = (GX_WIDGET *)&main_screen.main_screen_button_indicator_dot;
    INT x_shift;
    INT y_shift;

    if ((GX_WIDGET *)button != indicator->gx_widget_parent)
    {
        gx_widget_attach((GX_WIDGET *)button, indicator);
    }

    x_shift = button->gx_widget_size.gx_rectangle_right - 12 - indicator->gx_widget_size.gx_rectangle_left;
    y_shift = button->gx_widget_size.gx_rectangle_top + 4 - indicator->gx_widget_size.gx_rectangle_top;
    gx_widget_shift(indicator, x_shift, y_shift, GX_TRUE);
}

/******************************************************************************************/
/* Start an animation to toggle between screens.                                          */
/******************************************************************************************/
VOID screen_toggle_animation_start(GX_WINDOW *show, GX_WINDOW *hide)
{
    GX_EVENT  my_event;
    GX_STRING string;

    memset(&my_event, 0, sizeof(GX_EVENT));

    if (toggle_animation_flag != ANIMATION_NONE)
    {
        return;
    }

    toggle_animation_flag = ANIMATION_ACTIVE;

    if (show == (GX_WINDOW *)&main_screen)
    {
        /* Fade in start window. */
        my_event.gx_event_type = USER_EVENT_START_WIN_FADE_IN;
        my_event.gx_event_target = (GX_WIDGET *)&main_screen;
        gx_system_event_send(&my_event);

        /* Set button status to "Turn On". */
        gx_pixelmap_button_pixelmap_set(&main_screen.main_screen_button_on_off, GX_PIXELMAP_ID_MENU_BUTTON_LONG,
            GX_PIXELMAP_ID_MENU_BUTTON_LONG, GX_NULL);

        string.gx_string_ptr = string_turn_on;
        string.gx_string_length = sizeof(string_turn_on) - 1;
        gx_prompt_text_set_ext(&main_screen.main_screen_prompt_on_off, &string);

        /* Detach sequence number prompt. */
        gx_widget_detach(&sequence_number);

        /* Attach time prompt. */
        gx_widget_attach((GX_WIDGET *)&main_screen, (GX_WIDGET *)&main_screen.main_screen_window_time);
    }
    else if (show == (GX_WINDOW *)&sequence_window)
    {
        /* Fade in sequence window. */
        my_event.gx_event_type = USER_EVENT_SEQ_WIN_FADE_IN;
        my_event.gx_event_target = (GX_WIDGET *)&sequence_window;
        gx_system_event_send(&my_event);

        if (main_screen.main_screen_button_on_off.gx_pixelmap_button_normal_id !=
            GX_PIXELMAP_ID_MENU_BUTTON_LONG_PUSHED)
        {
            /* Set button status to "Shut Off". */
            gx_pixelmap_button_pixelmap_set(&main_screen.main_screen_button_on_off, GX_PIXELMAP_ID_MENU_BUTTON_LONG_PUSHED,
                GX_PIXELMAP_ID_MENU_BUTTON_LONG_PUSHED, GX_NULL);

            string.gx_string_ptr = string_shut_off;
            string.gx_string_length = sizeof(string_shut_off) - 1;
            gx_prompt_text_set_ext(&main_screen.main_screen_prompt_on_off, &string);
            gx_widget_detach((GX_WIDGET *)&main_screen.main_screen_window_time);
            gx_widget_attach((GX_WIDGET *)&main_screen, &sequence_number);
        }
    }
    else if (show == (GX_WINDOW *)&complete_window)
    {
        /* Fade in complete window. */
        my_event.gx_event_type = USER_EVENT_COMPLETE_WIN_FADE_IN;
        my_event.gx_event_target = (GX_WIDGET *)&complete_window;
        gx_system_event_send(&my_event);

        /* Detach button window. */
        gx_widget_detach((GX_WIDGET *)&main_screen.main_screen_button_window);
    }

    if (hide == (GX_WINDOW *)&main_screen)
    {
        /* Fade out main screen. */
        my_event.gx_event_type = USER_EVENT_START_WIN_FADE_OUT;
        my_event.gx_event_target = (GX_WIDGET *)&main_screen;
        gx_system_event_send(&my_event);
    }
    else if (hide == (GX_WINDOW *)&sequence_window)
    {
        /* Fade out sequence screen. */
        my_event.gx_event_type = USER_EVENT_SEQ_WIN_FADE_OUT;
        my_event.gx_event_target = (GX_WIDGET *)&sequence_window;
        gx_system_event_send(&my_event);

        /* Stop animations in sequence window. */
        sequence_animation_stop();
    }
    else if (hide == (GX_WINDOW *)&complete_window)
    {
        /* Fade out complete screen. */
        my_event.gx_event_type = USER_EVENT_COMPLETE_WIN_FADE_OUT;
        my_event.gx_event_target = (GX_WIDGET *)&complete_window;
        gx_system_event_send(&my_event);

        /* Attach button window. */
        gx_widget_attach((GX_WIDGET *)&main_screen, (GX_WIDGET *)&main_screen.main_screen_button_window);
    }
}

/******************************************************************************************/
/* Draw a widget with specifie off set in y coordinate.                                   */
/******************************************************************************************/
VOID custom_widget_children_draw(GX_WIDGET *child, INT yoff)
{
    GX_PIXELMAP *map;
    GX_STRING    text;
    GX_BRUSH    *brush;

    gx_context_brush_get(&brush);

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
                gx_widget_text_draw_ext(child,
                    ((GX_PROMPT *)child)->gx_prompt_normal_text_color,
                    ((GX_PROMPT *)child)->gx_prompt_font_id,
                    &text, 0, yoff);
            }
        }
        if (child->gx_widget_first_child)
        {
            custom_widget_children_draw(child->gx_widget_first_child, yoff);
        }

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Defines a custom pixlemap button draw function to do some custom drawing.              */
/******************************************************************************************/
VOID custom_pixlemap_button_draw(GX_PIXELMAP_BUTTON *button)
{
    INT          yoff = 4;
    GX_PIXELMAP *map;
    GX_BRUSH    *brush;

    gx_context_brush_get(&brush);

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

    if (map)
    {
        /* Offset button pixelmap draw. */
        gx_canvas_pixelmap_blend(button->gx_widget_size.gx_rectangle_left, button->gx_widget_size.gx_rectangle_top + yoff, map, brush->gx_brush_alpha);
    }

    custom_widget_children_draw(button->gx_widget_first_child, yoff);
}


/******************************************************************************************/
/* Update clock of main screen.                                                           */
/******************************************************************************************/
VOID clock_update()
{
#ifdef WIN32
    GX_CHAR time_string_buffer[6];
    GX_CHAR date_string_buffer[20];
    GX_STRING time_string;
    GX_STRING date_string;
    GX_STRING string;

    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    if (local_time.wHour < 12)
    {
        sprintf(time_string_buffer, "%02d:%02d", local_time.wHour, local_time.wMinute);

        string.gx_string_ptr = "AM";
        string.gx_string_length = 2;
        gx_prompt_text_set_ext(&main_screen.main_screen_am_pm, &string);
    }
    else
    {
        sprintf(time_string_buffer, "%02d:%02d", local_time.wHour - 12, local_time.wMinute);

        string.gx_string_ptr = "PM";
        string.gx_string_length = 2;
        gx_prompt_text_set_ext(&main_screen.main_screen_am_pm, &string);
    }

    sprintf_s(date_string_buffer, 20, "%s %02d, %d", month_names[local_time.wMonth - 1], local_time.wDay, local_time.wYear);

    time_string.gx_string_ptr = time_string_buffer;
    time_string.gx_string_length = string_length_get(time_string_buffer, sizeof(time_string_buffer) - 1);

    date_string.gx_string_ptr = date_string_buffer;
    date_string.gx_string_length = string_length_get(date_string_buffer, sizeof(date_string_buffer) - 1);

    gx_prompt_text_set_ext(&main_screen.main_screen_time, &time_string);
    gx_prompt_text_set_ext(&main_screen.main_screen_day_of_week, &day_names[local_time.wDayOfWeek]);
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
