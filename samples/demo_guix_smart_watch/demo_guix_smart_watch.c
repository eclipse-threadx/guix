/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define MEMORY_POOL_BUFFER_SIZE  DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 8
#define ANIMATION_MIN_SLIDING_DIST 5

extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY* display);

/* Define prototypes.  */
VOID start_guix(VOID);
VOID root_window_draw(GX_WINDOW* root);
UINT root_window_event_process(GX_WINDOW* window, GX_EVENT* event_ptr);
VOID demo_thread_entry(ULONG thread_input);

/* Create stack and control block for demo thread. */
static TX_THREAD demo_thread;
static ULONG demo_thread_stack[1024];

/* Define variables.  */
GX_WINDOW_ROOT *root;
TIME system_time = {12, 9, 4, 15, 35, 0};
static INT screen_clock_record_hour = -1;
static INT screen_clock_record_minute = -1;
static TX_BYTE_POOL memory_pool;
static GX_UBYTE memory_pool_buffer[MEMORY_POOL_BUFFER_SIZE];

static GX_ANIMATION slide_animation;
static GX_EVENT slide_pen_down_event;
static GX_BOOL slide_pen_down_valid = GX_FALSE;

/* Define screen lists for slide animation. */
static GX_WIDGET *container_screen_list[] = {
    (GX_WIDGET*)&page_1_container_screen,
    (GX_WIDGET*)&page_2_container_screen,
    (GX_WIDGET*)&page_3_container_screen,
    GX_NULL
};

static GX_WIDGET* page_1_screen_list[] = {
    (GX_WIDGET*)&message_screen,
    (GX_WIDGET*)&music_screen,
    (GX_WIDGET*)&weather_screen,
    (GX_WIDGET*)&SanDiego_weather_screen,
    (GX_WIDGET*)&LosAngeles_weather_screen,
    (GX_WIDGET*)&SanFrancisco_weather_screen,
    GX_NULL
};

static GX_WIDGET* page_2_screen_list[] = {
    (GX_WIDGET*)&clock_1_screen,
    (GX_WIDGET*)&clock_2_screen,
    (GX_WIDGET*)&home_screen,
    (GX_WIDGET*)&clock_3_screen,
    (GX_WIDGET*)&clock_4_screen,
    (GX_WIDGET*)&clock_5_screen,
    GX_NULL
};

static GX_WIDGET* page_3_screen_list[] = {
    (GX_WIDGET*)&yoga_screen,
    (GX_WIDGET*)&ekg_screen,
    (GX_WIDGET*)&fitness_screen,
    (GX_WIDGET*)&calories_screen,
    (GX_WIDGET*)&run_screen,
    (GX_WIDGET*)&stand_screen,
    GX_NULL
};

#ifndef WIN32
#define USE_CANVAS_FOR_SCREEN_SLIDE_ANIMATION
#endif

#ifdef USE_CANVAS_FOR_SCREEN_SLIDE_ANIMATION
#define ANIMATION_CANVAS_WIDTH (258 * 2)
#define ANIMATION_CANVAS_HEIGHT (289 * 2)

GX_COLOR animation_canvas_memory[ANIMATION_CANVAS_WIDTH * ANIMATION_CANVAS_HEIGHT];
GX_CANVAS              animation_canvas;
GX_WINDOW_ROOT         animation_root;
#endif

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char** argv)
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
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    tx_thread_create(&demo_thread, "Demo Thread", demo_thread_entry, 0,
        demo_thread_stack, sizeof(demo_thread_stack),
        GX_SYSTEM_THREAD_PRIORITY + 1,
        GX_SYSTEM_THREAD_PRIORITY + 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID demo_thread_entry(ULONG thread_input)
{
#ifdef USE_CANVAS_FOR_SCREEN_SLIDE_ANIMATION
GX_RECTANGLE size;
#endif
      
    /* Create byte pool*/
    tx_byte_pool_create(&memory_pool, "memory_pol", memory_pool_buffer, MEMORY_POOL_BUFFER_SIZE);
    
    /* Initialize GUIX. */
    gx_system_initialize();

    /* Assign memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* Create main screens. */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);
    gx_studio_named_widget_create("home_button", (GX_WIDGET*)root, GX_NULL);
    gx_studio_named_widget_create("page_1_container_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("page_2_container_screen", (GX_WIDGET *)&main_screen, GX_NULL);
    gx_studio_named_widget_create("page_3_container_screen", GX_NULL, GX_NULL);

    /* Create page 1 screens. */
    gx_studio_named_widget_create("message_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("music_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("weather_screen", (GX_WIDGET *)&page_1_container_screen, GX_NULL);
    gx_studio_named_widget_create("SanDiego_weather_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("LosAngeles_weather_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("SanFrancisco_weather_screen", GX_NULL, GX_NULL);

    /* Create page 2 screens. */
    gx_studio_named_widget_create("home_screen", (GX_WIDGET *)&page_2_container_screen, GX_NULL);
    gx_studio_named_widget_create("clock_1_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("clock_2_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("clock_3_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("clock_4_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("clock_5_screen", GX_NULL, GX_NULL);

    /* Create page 3 screens.  */
    gx_studio_named_widget_create("calories_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("ekg_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("fitness_screen", (GX_WIDGET *)&page_3_container_screen, GX_NULL);
    gx_studio_named_widget_create("run_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("stand_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("yoga_screen", GX_NULL, GX_NULL);
    screens_initialize();

    gx_studio_named_widget_create("screen_slide_parent", GX_NULL, GX_NULL);

    /* Reset root window draw function.  */
    gx_widget_draw_set((GX_WIDGET *)root, root_window_draw);

    /* Reset root window event process function.  */
    gx_widget_event_process_set((GX_WIDGET *)root, root_window_event_process);

    /* Create slide animation control block.  */
    gx_animation_create(&slide_animation);

#ifdef USE_CANVAS_FOR_SCREEN_SLIDE_ANIMATION
    gx_canvas_create(&animation_canvas, GX_NULL,
        root->gx_window_root_canvas->gx_canvas_display,
        GX_CANVAS_SIMPLE,
        ANIMATION_CANVAS_WIDTH, ANIMATION_CANVAS_HEIGHT,
        animation_canvas_memory, ANIMATION_CANVAS_WIDTH * ANIMATION_CANVAS_HEIGHT * sizeof(GX_COLOR));

    gx_utility_rectangle_define(&size, 0, 0, ANIMATION_CANVAS_WIDTH - 1, ANIMATION_CANVAS_HEIGHT - 1);
    gx_window_root_create(&animation_root, "animation_root", &animation_canvas, GX_STYLE_BORDER_NONE, GX_NULL, &size);
    gx_canvas_hide(&animation_canvas);

    gx_canvas_hardware_layer_bind(&animation_canvas, 2);
#endif
    
    /* Show the root window.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

/******************************************************************************************/
/* Find visible screen from screen list.                                                  */
/******************************************************************************************/
static GX_WIDGET *find_visible_screen_of_screen_list(GX_WIDGET **screen_list)
{
    INT index = 0;
    while (screen_list[index])
    {
        if (screen_list[index]->gx_widget_status & GX_STATUS_VISIBLE)
        {
            return screen_list[index];
        }

        index++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Get page list by specified container screen.                                           */
/******************************************************************************************/
static GX_WIDGET **get_page_screen_list(GX_WIDGET *screen_container)
{
    GX_WIDGET** page_list = GX_NULL;

    switch (screen_container->gx_widget_id)
    {
    case ID_PAGE_1_CONTAINER_SCREEN:
        page_list = page_1_screen_list;
        break;

    case ID_PAGE_2_CONTAINER_SCREEN:
        page_list = page_2_screen_list;
        break;

    case ID_PAGE_3_CONTAINER_SCREEN:
        page_list = page_3_screen_list;
        break;
    }

    return page_list;
}

/******************************************************************************************/
/* Get screen container by specified screen.                                              */
/******************************************************************************************/
static GX_WIDGET *get_screen_container(GX_WIDGET* screen)
{
    GX_WIDGET** screen_list;
    INT index = 0;
    INT j;

    while (container_screen_list[index])
    {
        screen_list = get_page_screen_list(container_screen_list[index]);
        j = 0;

        while (screen_list[j])
        {
            if (screen_list[j] == screen)
            {
                return container_screen_list[index];
            }
            j++;
        }
        index++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Enable vertical slide animation.                                                       */
/******************************************************************************************/
static GX_BOOL enable_vertical_slide_animation()
{
    GX_ANIMATION_INFO slide_animation_info;

    memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
    slide_animation_info.gx_animation_parent = (GX_WIDGET*)&main_screen;
    slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_VERTICAL | GX_ANIMATION_WRAP | GX_ANIMATION_BLOCK_MOVE;
    slide_animation_info.gx_animation_id = SCREEN_DRAG_ANIMATION_ID;
    slide_animation_info.gx_animation_frame_interval = 20 / GX_SYSTEM_TIMER_MS;
    slide_animation_info.gx_animation_slide_screen_list = container_screen_list;
    slide_animation_info.gx_animation_start_alpha = 255;
    slide_animation_info.gx_animation_end_alpha = 255;
#ifdef USE_CANVAS_FOR_SCREEN_SLIDE_ANIMATION
    gx_animation_canvas_define(&slide_animation, &animation_canvas);
#endif
        
    gx_animation_drag_enable(&slide_animation, (GX_WIDGET*)&main_screen, &slide_animation_info);

    return GX_SUCCESS;
}

/******************************************************************************************/
/* Enable horizontal slide animation.                                                     */
/******************************************************************************************/
static GX_BOOL enable_horizontal_slide_animation()
{
    GX_ANIMATION_INFO slide_animation_info;

    memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));

    /* Find the visible page container screen.  */
    slide_animation_info.gx_animation_parent = find_visible_screen_of_screen_list(container_screen_list);

    if (slide_animation_info.gx_animation_parent)
    {
        slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL | GX_ANIMATION_WRAP | GX_ANIMATION_BLOCK_MOVE;
        slide_animation_info.gx_animation_id = SCREEN_DRAG_ANIMATION_ID;
        slide_animation_info.gx_animation_frame_interval = 20 / GX_SYSTEM_TIMER_MS;
        slide_animation_info.gx_animation_slide_screen_list = get_page_screen_list(slide_animation_info.gx_animation_parent);
        slide_animation_info.gx_animation_start_alpha = 255;
        slide_animation_info.gx_animation_end_alpha = 255;
#ifdef USE_CANVAS_FOR_SCREEN_SLIDE_ANIMATION
        gx_animation_canvas_define(&slide_animation, &animation_canvas);
#endif
    
        /* Enable slide animation for the visible page container screen.  */
        gx_animation_drag_enable(&slide_animation, slide_animation_info.gx_animation_parent, &slide_animation_info);

        return GX_SUCCESS;
    }

    return GX_FALSE;
}

/******************************************************************************************/
/* Update system clock.                                                                   */
/******************************************************************************************/
static VOID system_clock_update()
{
#ifdef WIN32
    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    system_time.month = local_time.wMonth;
    system_time.day = local_time.wDay;
    system_time.day_of_week = local_time.wDayOfWeek;
    system_time.hour = local_time.wHour;
    system_time.minute = local_time.wMinute;
    system_time.second = local_time.wSecond;
#else
    system_time.second++;
    if (system_time.second >= 60)
    {
        system_time.second = 0;
        system_time.minute++;

        if (system_time.minute >= 60)
        {
            system_time.minute = 0;
        }
    }
#endif
}

/******************************************************************************************/
/* Clear time record of screens.                                                          */
/******************************************************************************************/
VOID clear_screen_clock_record()
{
    screen_clock_record_hour = -1;
    screen_clock_record_minute = -1;
}

/******************************************************************************************/
/* Update screen clock.                                                                   */
/******************************************************************************************/
VOID screen_clock_update(GX_NUMERIC_PROMPT *hour, GX_NUMERIC_PROMPT *minute, GX_PROMPT *second)
{
    GX_RESOURCE_ID text_color_id;

    if (screen_clock_record_hour != system_time.hour)
    {
        gx_numeric_prompt_value_set(hour, system_time.hour);
        screen_clock_record_hour = system_time.hour;
    }

    if (screen_clock_record_minute != system_time.minute)
    {
        gx_numeric_prompt_value_set(minute, system_time.minute);
        screen_clock_record_minute = system_time.minute;
    }

    if (second)
    {
        if (system_time.second & 0x1)
        {
            text_color_id = GX_COLOR_ID_WHITE;
        }
        else
        {
            text_color_id = GX_COLOR_ID_GRAY;
        }

        gx_prompt_text_color_set(second, text_color_id, text_color_id, text_color_id);
    }
}

/******************************************************************************************/
/* Send event to current screen.                                                          */
/******************************************************************************************/
static VOID send_event_to_current_screen(INT event_type)
{
    GX_EVENT myevent;
    GX_WIDGET *parent;

    /* Find visible screen container.  */
    parent = find_visible_screen_of_screen_list(container_screen_list);

    /* Find visible screen.  */
    parent = find_visible_screen_of_screen_list(get_page_screen_list(parent));

    if (parent)
    {
        memset(&myevent, 0, sizeof(GX_EVENT));
        myevent.gx_event_type = event_type;
        myevent.gx_event_target = parent;
        parent->gx_widget_event_process_function(parent, &myevent);
    }
}

/******************************************************************************************/
/* Slide to home screen.                                                                  */
/******************************************************************************************/
static VOID slide_to_home_screen()
{
    GX_WIDGET *current_screen_container;
    GX_WIDGET *target_screen_container;
    GX_WIDGET *current_screen;
    GX_WIDGET *target_screen = (GX_WIDGET *)&home_screen;

    /* Find the visible page container screen.  */
    current_screen_container = find_visible_screen_of_screen_list(container_screen_list);

    /* Find visible screen.  */
    current_screen = find_visible_screen_of_screen_list(get_page_screen_list(current_screen_container));

    if (current_screen == target_screen)
    {
        return;
    }

    target_screen_container = get_screen_container(target_screen);

    if (current_screen_container == target_screen_container)
    {
        page_screen_slide(current_screen, target_screen);
    }
    else
    {
        container_screen_slide(current_screen, target_screen);
    }
}

/******************************************************************************************/
/* Start animation to slide from current screen to the specified screen of the same page. */
/******************************************************************************************/
VOID page_screen_slide(GX_WIDGET *current_screen, GX_WIDGET *target_screen)
{
    GX_WIDGET *screen_container;
    GX_WIDGET **screen_list;
    INT current_screen_index = 0;
    INT target_screen_index = 0;
    INT index = 0;
    GX_ANIMATION *animation;
    GX_ANIMATION_INFO animation_info;
    GX_RECTANGLE *parent_size;
    GX_RECTANGLE size;
    INT width;
    INT distance;

    if (slide_animation.gx_animation_status != GX_ANIMATION_IDLE)
    {
        return;
    }

    send_event_to_current_screen(USER_EVENT_ANIMATION_STOP);

    /* Find the visible page container screen.  */
    screen_container = find_visible_screen_of_screen_list(container_screen_list);

    /* Find screen list for the visible screen container.  */
    screen_list = get_page_screen_list(screen_container);

    while (screen_list[index])
    {
        if (screen_list[index] == current_screen)
        {
            current_screen_index = index;
        }
        else if (screen_list[index] == target_screen)
        {
            target_screen_index = index;
        }

        index++;
    }

    parent_size = &screen_container->gx_widget_size;
    width = parent_size->gx_rectangle_right - parent_size->gx_rectangle_left + 1;
    distance = width * (target_screen_index - current_screen_index);

    size = *parent_size;

    if (target_screen_index > current_screen_index)
    {
        size.gx_rectangle_right = size.gx_rectangle_left + distance + width - 1;
    }
    else
    {
        size.gx_rectangle_left = size.gx_rectangle_right + distance - width + 1;
    }

    gx_widget_resize(&screen_slide_parent, &size);

    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    animation_info.gx_animation_frame_interval = 1;
    animation_info.gx_animation_id = SCREEN_SLIDE_ANIMATION_ID;
    animation_info.gx_animation_steps = 300 / GX_SYSTEM_TIMER_MS;
    animation_info.gx_animation_start_alpha = 255;
    animation_info.gx_animation_end_alpha = 255;
    animation_info.gx_animation_parent = screen_container;
    animation_info.gx_animation_target = (GX_WIDGET *)&screen_slide_parent;
    animation_info.gx_animation_start_position.gx_point_x = size.gx_rectangle_left;
    animation_info.gx_animation_start_position.gx_point_y = size.gx_rectangle_top;
    animation_info.gx_animation_end_position.gx_point_x= size.gx_rectangle_left - distance;
    animation_info.gx_animation_end_position.gx_point_y = size.gx_rectangle_top;
    animation_info.gx_animation_style = GX_ANIMATION_TRANSLATE | GX_ANIMATION_BLOCK_MOVE;

    index = current_screen_index;
    while (1)
    {
        gx_widget_attach(&screen_slide_parent, screen_list[index]);

        size.gx_rectangle_left = parent_size->gx_rectangle_left + (width * (index - current_screen_index));
        size.gx_rectangle_right = size.gx_rectangle_left + width - 1;
        gx_widget_resize(screen_list[index], &size);

        if (index == target_screen_index)
        {
            break;
        }

        if (current_screen_index < target_screen_index)
        {
            index++;
        }
        else
        {
            index--;
        }
    }

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        gx_animation_start(animation, &animation_info);
    }
}

/******************************************************************************************/
/* Start animation to slide from current screen to the specified screen of another page.  */
/******************************************************************************************/
VOID container_screen_slide(GX_WIDGET* current_screen, GX_WIDGET* target_screen)
{
    GX_WIDGET *current_screen_container;
    GX_WIDGET *target_screen_container;
    GX_WIDGET **screen_list;
    INT current_screen_index = 0;
    INT target_screen_index = 0;
    INT index = 0;
    GX_ANIMATION* animation;
    GX_ANIMATION_INFO animation_info;
    GX_RECTANGLE *parent_size;
    GX_RECTANGLE size;
    INT height;
    INT distance;
    GX_WIDGET* child = GX_NULL;

    if (slide_animation.gx_animation_status != GX_ANIMATION_IDLE)
    {
        return;
    }

    send_event_to_current_screen(USER_EVENT_ANIMATION_STOP);

    /* Find the visible page container screen.  */
    current_screen_container = find_visible_screen_of_screen_list(container_screen_list);
    target_screen_container = get_screen_container(target_screen);

    gx_widget_first_child_get(target_screen_container, &child);
    if (child != target_screen)
    {
        gx_widget_detach(child);
        gx_widget_attach(target_screen_container, target_screen);
        gx_widget_shift(target_screen,
            target_screen_container->gx_widget_size.gx_rectangle_left - target_screen->gx_widget_size.gx_rectangle_left,
            target_screen_container->gx_widget_size.gx_rectangle_top - target_screen->gx_widget_size.gx_rectangle_top, GX_FALSE);
    }

    /* Find screen list for the visible screen container.  */
    screen_list = container_screen_list;

    while (screen_list[index])
    {
        if (screen_list[index] == current_screen_container)
        {
            current_screen_index = index;
        }
        else if (screen_list[index] == target_screen_container)
        {
            target_screen_index = index;
        }

        index++;
    }

    parent_size = &main_screen.gx_widget_size;
    height = parent_size->gx_rectangle_bottom - parent_size->gx_rectangle_top + 1;
    distance = height * (target_screen_index - current_screen_index);

    size = *parent_size;

    if (target_screen_index > current_screen_index)
    {
        size.gx_rectangle_bottom = size.gx_rectangle_top + distance + height - 1;
    }
    else
    {
        size.gx_rectangle_top = size.gx_rectangle_bottom + distance - height + 1;
    }

    gx_widget_resize(&screen_slide_parent, &size);

    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    animation_info.gx_animation_frame_interval = 1;
    animation_info.gx_animation_id = SCREEN_SLIDE_ANIMATION_ID;
    animation_info.gx_animation_steps = 300 / GX_SYSTEM_TIMER_MS;
    animation_info.gx_animation_start_alpha = 255;
    animation_info.gx_animation_end_alpha = 255;
    animation_info.gx_animation_parent = (GX_WIDGET *)&main_screen;
    animation_info.gx_animation_target = (GX_WIDGET *)&screen_slide_parent;
    animation_info.gx_animation_start_position.gx_point_x = size.gx_rectangle_left;
    animation_info.gx_animation_start_position.gx_point_y = size.gx_rectangle_top;
    animation_info.gx_animation_end_position.gx_point_x = size.gx_rectangle_left;
    animation_info.gx_animation_end_position.gx_point_y = size.gx_rectangle_top - distance;
    animation_info.gx_animation_style = GX_ANIMATION_TRANSLATE | GX_ANIMATION_BLOCK_MOVE;

    index = current_screen_index;

    while(1)
    {
        gx_widget_attach(&screen_slide_parent, screen_list[index]);

        size.gx_rectangle_top = parent_size->gx_rectangle_top + (height * (index - current_screen_index));
        size.gx_rectangle_bottom = size.gx_rectangle_top + height - 1;
        gx_widget_resize(screen_list[index], &size);

        if (index == target_screen_index)
        {
            break;
        }

        if (current_screen_index < target_screen_index)
        {
            index++;
        }
        else
        {
            index--;
        }
    }

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        gx_animation_start(animation, &animation_info);
    }
}

/******************************************************************************************/
/* Override the default drawing of the home button.                                       */
/******************************************************************************************/
VOID root_home_button_draw(GX_PIXELMAP_BUTTON* widget)
{
    if (widget->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_pixelmap_button_draw(widget);
    }
}

/******************************************************************************************/
/* Override the default drawing of the root window.                                       */
/******************************************************************************************/
VOID root_window_draw(GX_WINDOW* root)
{
    GX_PIXELMAP* map = GX_NULL;

    /* Call default root window draw. */
    gx_window_draw(root);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_MICROSOFT_AZURE_LOGO, &map);

    if (map)
    {
        /* Draw logo. */
        gx_canvas_pixelmap_draw(15, 15, map);
    }
    
    gx_context_pixelmap_get(GX_PIXELMAP_ID_WATCH, &map);

    if (map)
    {
        /* Draw smart watch frame.  */
        gx_context_fill_color_set(GX_COLOR_ID_WHITE);
        gx_canvas_pixelmap_draw((DISPLAY_1_X_RESOLUTION - map->gx_pixelmap_width) >> 1, 0, map);
    }
}

/******************************************************************************************/
/* Override the default event processing of the root window to handle signals from my     */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT root_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_HOME_BUTTON, GX_EVENT_CLICKED):
        slide_to_home_screen();
        break;

    default:
        return gx_window_root_event_process((GX_WINDOW_ROOT*)window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override the default event processing of the "main_screen" to handle signals from my   */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    UINT status = GX_SUCCESS;
    INT horizontal_dist;
    INT vertical_dist;
    GX_WIDGET *parent;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:

        /* Update system time. */
        system_clock_update();

        /* Start a timer to update time. */
        gx_system_timer_start(window, SYSTEM_CLOCK_TIMER_ID, GX_TICKS_SECOND, GX_TICKS_SECOND);
        
        status = gx_window_event_process(window, event_ptr);

        if(home_screen.gx_widget_status & GX_STATUS_VISIBLE)
        {
            gx_sprite_start(&home_screen.home_screen_sprite, 0);
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SYSTEM_CLOCK_TIMER_ID)
        {
            system_clock_update();
            music_play_progress_update();
        }
        break;

    case GX_EVENT_PEN_DOWN:
        if (slide_animation.gx_animation_status == GX_ANIMATION_IDLE)
        {
            /* Reserve pen down event for later use.  */
            slide_pen_down_event = *event_ptr;
            slide_pen_down_valid = GX_TRUE;
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if (slide_animation.gx_animation_status == GX_ANIMATION_IDLE && slide_pen_down_valid)
        {
            horizontal_dist = GX_ABS(slide_pen_down_event.gx_event_payload.gx_event_pointdata.gx_point_x - event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x);
            vertical_dist = GX_ABS(slide_pen_down_event.gx_event_payload.gx_event_pointdata.gx_point_y - event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y);

            if ((horizontal_dist > ANIMATION_MIN_SLIDING_DIST) || vertical_dist > ANIMATION_MIN_SLIDING_DIST)
            {
                if (horizontal_dist > vertical_dist)
                {
                    /* Horizontal slide direction. */
                    status = enable_horizontal_slide_animation();
                }
                else
                {
                    /* Vertical slide direction. */
                    status = enable_vertical_slide_animation();
                }

                if (status == GX_SUCCESS)
                {
                    send_event_to_current_screen(USER_EVENT_ANIMATION_STOP);

                    parent = slide_animation.gx_animation_info.gx_animation_parent;

                    /* Pass pen down and pen drag event to slide animation parent to process sliding animation.  */
                    parent->gx_widget_event_process_function(parent, &slide_pen_down_event);
                    parent->gx_widget_event_process_function(parent, event_ptr);
                }
            }
        }
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        if (event_ptr->gx_event_sender == SCREEN_DRAG_ANIMATION_ID)
        {
            /* Disable slide animation.  */
            gx_animation_drag_disable(&slide_animation, slide_animation.gx_animation_info.gx_animation_parent);

            send_event_to_current_screen(USER_EVENT_ANIMATION_START);
        }
        else if (event_ptr->gx_event_sender == SCREEN_SLIDE_ANIMATION_ID)
        {
            gx_widget_attach(screen_slide_parent.gx_widget_parent, screen_slide_parent.gx_widget_last_child);
            parent = screen_slide_parent.gx_widget_first_child;
            while (parent)
            {
                gx_widget_detach(parent);
                parent = parent->gx_widget_next;
            }
            gx_widget_detach(&screen_slide_parent);
            send_event_to_current_screen(USER_EVENT_ANIMATION_START);
        }
        break;

    case GX_EVENT_PEN_UP:
        if (slide_animation.gx_animation_status == GX_ANIMATION_IDLE)
        {
            /* Disable slide animation.  */
            gx_animation_drag_disable(&slide_animation, slide_animation.gx_animation_info.gx_animation_parent);
        }
        slide_pen_down_valid = GX_FALSE;
        break;

    default:
        return gx_window_event_process((GX_WINDOW*)window, event_ptr);
    }

    return status;
}