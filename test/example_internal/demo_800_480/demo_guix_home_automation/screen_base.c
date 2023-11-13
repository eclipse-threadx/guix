
#include "demo_guix_home_automation.h"

extern GX_ANIMATION slide_animation;

/* Define screen information structure. */
typedef struct SCREEN_INFO_STRUCT{
    SCREEN_BASE_CONTROL_BLOCK *widget; /* Poniter to screen widget. */
    GX_WIDGET **window_list; /* Page list for screen slide animation. */
    GX_WIDGET **pagination_list; /* Pagination list to indicate current page. */
}SCREEN_INFO;

/* Define footer menu information structure. */
typedef struct FOOTER_MENU_INFO_STRUCT{
    INT            footer_menu_id; /* Footer menu widget id. */
    GX_RESOURCE_ID footer_normal_icon; /* Normal pixelmap id. */
    GX_RESOURCE_ID footer_selected_icon; /* Selected pixelmap id. */
}FOOTER_MENU_INFO;

/* Page list for light screen. */
static GX_WIDGET *lights_win_list[] = {
    (GX_WIDGET *)&lights_page_1,
    (GX_WIDGET *)&lights_page_2,
    (GX_WIDGET *)&lights_page_3,
    (GX_WIDGET *)&lights_page_4,
    GX_NULL
};

/* Page list for thermostat screen. */
static GX_WIDGET *thermostat_win_list[] = {
    (GX_WIDGET *)&thermostat_page_1,
    (GX_WIDGET *)&thermostat_page_2,
    (GX_WIDGET *)&thermostat_page_3,
    (GX_WIDGET *)&thermostat_page_4,
    GX_NULL
};

/* Page list for lock screen. */
static GX_WIDGET *locks_win_list[] = {
    (GX_WIDGET *)&locks_page_1,
    (GX_WIDGET *)&locks_page_2,
    (GX_WIDGET *)&locks_page_3,
    GX_NULL
};

/* Page list for weather screen. */
static GX_WIDGET *weather_win_list[] = {
    (GX_WIDGET *)&win_San_Diego,
    (GX_WIDGET *)&win_New_York,
    (GX_WIDGET *)&win_Las_Vegas,
    GX_NULL
};

/* Pagination list for light screen. */
static GX_WIDGET *lights_pagination_list[] = {
    (GX_WIDGET *)&lights_screen.lights_screen_pagination_1,
    (GX_WIDGET *)&lights_screen.lights_screen_pagination_2,
    (GX_WIDGET *)&lights_screen.lights_screen_pagination_3,
    (GX_WIDGET *)&lights_screen.lights_screen_pagination_4,
    GX_NULL
};

/* Pagination list for lock screen. */
static GX_WIDGET *locks_pagination_list[] = {
    (GX_WIDGET *)&locks_screen.locks_screen_pagination_1,
    (GX_WIDGET *)&locks_screen.locks_screen_pagination_2,
    (GX_WIDGET *)&locks_screen.locks_screen_pagination_3,
    GX_NULL
};

/* Pagination list for weather screen. */
static GX_WIDGET *weather_pagination_list[] = {
    (GX_WIDGET *)&weather_screen.weather_screen_pagination_1,
    (GX_WIDGET *)&weather_screen.weather_screen_pagination_2,
    (GX_WIDGET *)&weather_screen.weather_screen_pagination_3,
    GX_NULL
};

/* Pagination list for thermostat screen. */
static GX_WIDGET *thermostat_pagination_list[] = {
    (GX_WIDGET *)&thermostat_screen.thermostat_screen_pagination_1,
    (GX_WIDGET *)&thermostat_screen.thermostat_screen_pagination_2,
    (GX_WIDGET *)&thermostat_screen.thermostat_screen_pagination_3,
    (GX_WIDGET *)&thermostat_screen.thermostat_screen_pagination_4,
    GX_NULL
};

/* This is a list of screens and page/pagination lists that applied to those screens. */
static SCREEN_INFO screen_info_list[] = {
    { &lights_screen.base, lights_win_list, lights_pagination_list},
    { &thermostat_screen.base, thermostat_win_list, thermostat_pagination_list},
    { &locks_screen.base, locks_win_list, locks_pagination_list},
    { &weather_screen.base, weather_win_list, weather_pagination_list},
    {GX_NULL, GX_NULL, GX_NULL}
};

/* This is a list of footer menu ids, normal and selected pixelmaps that maps to one menu. */
static FOOTER_MENU_INFO footer_menu_info_list[] = {
    { ID_FOOTER_LIGHTS, GX_PIXELMAP_ID_FOOTER_MENU_ICON_LIGHT, GX_PIXELMAP_ID_FOOTER_MENU_ICON_LIGHT_ACTIVE },
    { ID_FOOTER_THERMOSTAT, GX_PIXELMAP_ID_FOOTER_MENU_ICON_THERMOSTAT, GX_PIXELMAP_ID_FOOTER_MENU_ICON_THERMOSTAT_ACTIVE },
    { ID_FOOTER_LOCKS, GX_PIXELMAP_ID_FOOTER_MENU_ICON_LOCK, GX_PIXELMAP_ID_FOOTER_MENU_ICON_LOCK_ACTIVE },
    { ID_FOOTER_WEATHER, GX_PIXELMAP_ID_FOOTER_MENU_ICON_WEATHER, GX_PIXELMAP_ID_FOOTER_MENU_ICON_WEATHER_ACTIVE },
    { 0, 0, 0}
};

/* This variable is used to count the number of in process slide animations. */
int page_slide_animation_count = 0;

/* This variable is used to count the number of in process child animations in current screen. */
int screen_animation_count = 0;

/******************************************************************************************/
/* Test if there is animation in process.                                                 */
/******************************************************************************************/
static GX_BOOL is_animation_active()
{
    if (page_slide_animation_count ||
        slide_animation.gx_animation_status != GX_ANIMATION_IDLE)
    {
        return GX_TRUE;
    }

    return GX_FALSE;
}

/******************************************************************************************/
/* Reterieve window list for screen slide animation.                                      */
/******************************************************************************************/
static SCREEN_INFO *screen_info_get(GX_WIDGET *screen)
{
    SCREEN_INFO *info = screen_info_list;

    while (info->widget)
    {
        if ((GX_WIDGET *)info->widget == screen)
        {
            return info;
        }
        info++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Start animations for footer icons.                                                     */
/******************************************************************************************/
static VOID footer_icons_animation_start(SCREEN_BASE_CONTROL_BLOCK *base)
{
    GX_ANIMATION *animation;
    GX_ANIMATION_INFO info;
    GX_WIDGET *target;

    memset(&info, 0, sizeof(GX_ANIMATION_INFO));
    info.gx_animation_style = GX_ANIMATION_ELASTIC_EASE_OUT;
    info.gx_animation_parent = (GX_WIDGET *)base;
    info.gx_animation_start_alpha = 255;
    info.gx_animation_end_alpha = 255;
    info.gx_animation_steps = 400 / GX_SYSTEM_TIMER_MS;
    info.gx_animation_frame_interval = 2;

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        /* start home icon animation. */
        target = (GX_WIDGET *)&base->screen_base_home;;

        info.gx_animation_id = ANIMATION_ID_FOOTER_HOME;
        info.gx_animation_target = target;
        info.gx_animation_start_position.gx_point_x = target->gx_widget_size.gx_rectangle_left;
        info.gx_animation_start_position.gx_point_y = 0;
        info.gx_animation_end_position.gx_point_x = target->gx_widget_size.gx_rectangle_left;
        info.gx_animation_end_position.gx_point_y = DISPLAY_1_Y_RESOLUTION - 45;

        gx_widget_style_remove(target, GX_STYLE_ENABLED);
        gx_animation_start(animation, &info);
    }

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        /* start setting icon animation. */
        target = (GX_WIDGET *)&base->screen_base_setting;;
        info.gx_animation_id = ANIMATION_ID_FOOTER_SETTING;
        info.gx_animation_target = target;
        info.gx_animation_start_position.gx_point_x = target->gx_widget_size.gx_rectangle_left;
        info.gx_animation_start_position.gx_point_y = DISPLAY_1_Y_RESOLUTION + 55;
        info.gx_animation_end_position.gx_point_x = target->gx_widget_size.gx_rectangle_left;
        info.gx_animation_end_position.gx_point_y = DISPLAY_1_Y_RESOLUTION - 45;

        gx_widget_style_remove(target, GX_STYLE_ENABLED);
        gx_animation_start(animation, &info);
    }

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        /* start add window animation. */
        target = (GX_WIDGET *)&base->screen_base_add;

        info.gx_animation_id = ANIMATION_ID_FOOTER_ADD;
        info.gx_animation_target = target;
        info.gx_animation_start_position.gx_point_x = DISPLAY_1_X_RESOLUTION;
        info.gx_animation_start_position.gx_point_y = target->gx_widget_size.gx_rectangle_top;
        info.gx_animation_end_position.gx_point_x = DISPLAY_1_X_RESOLUTION - 115;
        info.gx_animation_end_position.gx_point_y = target->gx_widget_size.gx_rectangle_top;

        gx_widget_style_remove(target, GX_STYLE_ENABLED);
        gx_animation_start(animation, &info);
    }
}

/******************************************************************************************/
/* Start title animation for widgets that based on "controler_base".                      */
/******************************************************************************************/
VOID title_animation_start(CONTROLLER_BASE_CONTROL_BLOCK *base)
{
    GX_ANIMATION *animation;
    GX_ANIMATION_INFO info;
    GX_WIDGET *target;

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        memset(&info, 0, sizeof(GX_ANIMATION_INFO));

        target = (GX_WIDGET *)&base->controller_base_title;
        info.gx_animation_style = 0;
        info.gx_animation_id = ANIMATION_ID_SCREEN_CHILDREN;
        info.gx_animation_parent = (GX_WIDGET *)base;
        info.gx_animation_target = target;
        info.gx_animation_start_alpha = 255;
        info.gx_animation_end_alpha = 255;
        info.gx_animation_steps = 100 / GX_SYSTEM_TIMER_MS;
        info.gx_animation_frame_interval = 2;
        info.gx_animation_start_position.gx_point_x = base->gx_widget_size.gx_rectangle_left + 22;
        info.gx_animation_start_position.gx_point_y = base->gx_widget_size.gx_rectangle_top - 18;
        info.gx_animation_end_position.gx_point_x = base->gx_widget_size.gx_rectangle_left + 22;
        info.gx_animation_end_position.gx_point_y = base->gx_widget_size.gx_rectangle_top + 14;

        gx_animation_start(animation, &info);

        screen_animation_count++;
    }
}

/******************************************************************************************/
/* Start screen slide animation for specified window.                                     */
/******************************************************************************************/
static VOID slide_animation_start(GX_WINDOW *window)
{
    GX_ANIMATION_INFO slide_animation_info;
    SCREEN_INFO *info = screen_info_get(window->gx_widget_parent);
 
    if (info)
    {
        if (!window->gx_widget_first_child)
        {
            gx_widget_attach((GX_WIDGET *)window, info->window_list[0]);
        }

        memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
        slide_animation_info.gx_animation_parent = (GX_WIDGET *)window;
        slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL | GX_ANIMATION_WRAP | GX_ANIMATION_SINE_EASE_OUT;
        slide_animation_info.gx_animation_id = ANIMATION_ID_DRAG_SLIDE;
        slide_animation_info.gx_animation_frame_interval = 1;
        slide_animation_info.gx_animation_steps = 400 / GX_SYSTEM_TIMER_MS;
        slide_animation_info.gx_animation_slide_screen_list = info->window_list;

        gx_animation_drag_enable(&slide_animation, (GX_WIDGET *)window, &slide_animation_info);
    }
}

/******************************************************************************************/
/* Retrieve page index with the specified page id.                                        */
/******************************************************************************************/
static INT get_page_index(INT page_id)
{
    INT page_index = 0;

    switch (page_id)
    {
    case ID_PAGINATION_1:
        page_index = 0;
        break;

    case ID_PAGINATION_2:
        page_index = 1;
        break;

    case ID_PAGINATION_3:
        page_index = 2;
        break;

    case ID_PAGINATION_4:
        page_index = 3;
        break;
    }

    return page_index;
}

/******************************************************************************************/
/* Enable/Disable pagination buttons.                                                     */
/******************************************************************************************/
VOID pagination_button_enable_disable(GX_WINDOW *window, GX_BOOL enabled)
{
    SCREEN_INFO *info = screen_info_get((GX_WIDGET *)window);
    GX_WIDGET *pagination;
    INT index = 0;

    if (!info)
    {
        return;
    }

    while (1)
    {
        pagination = info->pagination_list[index];

        if (pagination)
        {
            if (enabled)
            {
                gx_widget_style_add(pagination, GX_STYLE_ENABLED);
            }
            else
            {
                gx_widget_style_remove(pagination, GX_STYLE_ENABLED);
            }

            index++;
        }
        else
        {
            break;
        }
    };
}

/******************************************************************************************/
/* Update status of pagination buttons.                                                   */
/******************************************************************************************/
static VOID pagination_button_update(GX_WINDOW *window)
{
    INT         index;
    GX_WIDGET  *screen;
    GX_WIDGET  *pagination;
    SCREEN_INFO *info = screen_info_get((GX_WIDGET *)window);

    /* Search current visible menu index. */
    index = 0;

    if (!info)
    {
        return;
    }

    switch (info->widget->gx_widget_id)
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
        /* Start weather children animations. */
        weather_screen_animation_start();
        break;
    }

    while (1)
    {
        screen = info->window_list[index];
        pagination = info->pagination_list[index];

        if (screen && pagination)
        {
            if (screen->gx_widget_status & GX_STATUS_VISIBLE)
            {
                gx_widget_style_add(pagination, GX_STYLE_BUTTON_PUSHED);
            }
            else
            {
                gx_widget_style_remove(pagination, GX_STYLE_BUTTON_PUSHED);
            }

            index++;
        }
        else
        {
            break;
        }
    }
}

/******************************************************************************************/
/* Start animation to slide from current page to the specified page.                      */
/******************************************************************************************/
static VOID screen_list_slide(GX_WINDOW *window, INT page_id)
{
    GX_WIDGET        *target;
    INT               current_index = -1;
    INT               index;
    INT               screen_width;
    INT               animation_dis;
    GX_ANIMATION     *animation;
    GX_ANIMATION_INFO animation_info;
    GX_WIDGET        *parent = GX_NULL;
    SCREEN_INFO      *info = screen_info_get((GX_WIDGET *)window);
    INT               page_index = get_page_index(page_id);

    if (!info)
    {
        return;
    }

    index = 0;

    /* Search for visible page. */
    while (1)
    {
        target = info->window_list[index];

        if (target)
        {
            if (target->gx_widget_status & GX_STATUS_VISIBLE)
            {
                current_index = index;
                parent = target->gx_widget_parent;
                break;
            }

            index++;
        }
        else
        {
            break;
        }
    }

    if (!parent)
    {
        return;
    }

    screen_width = (target->gx_widget_size.gx_rectangle_right - target->gx_widget_size.gx_rectangle_left + 1);
    animation_dis = screen_width * (current_index - page_index);

    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    animation_info.gx_animation_frame_interval = 1;
    animation_info.gx_animation_id = ANIMATION_ID_PAGE_SLIDE;
    animation_info.gx_animation_steps = 600 / GX_SYSTEM_TIMER_MS;
    animation_info.gx_animation_start_alpha = 255;
    animation_info.gx_animation_end_alpha = 255;
    animation_info.gx_animation_parent = parent;
    animation_info.gx_animation_start_position.gx_point_x = info->window_list[current_index]->gx_widget_size.gx_rectangle_left;
    animation_info.gx_animation_start_position.gx_point_y = info->window_list[current_index]->gx_widget_size.gx_rectangle_top;
    animation_info.gx_animation_end_position.gx_point_x = info->window_list[current_index]->gx_widget_size.gx_rectangle_left + animation_dis;
    animation_info.gx_animation_end_position.gx_point_y = info->window_list[current_index]->gx_widget_size.gx_rectangle_top;
    animation_info.gx_animation_target = info->window_list[current_index];
    animation_info.gx_animation_style = GX_ANIMATION_TRANSLATE | GX_ANIMATION_DETACH;

    if (gx_system_animation_get(&animation) == GX_SUCCESS)
    {
        /* start animation to slide current page to target position. */
        gx_animation_start(animation, &animation_info);
        page_slide_animation_count++;
    }

    while (current_index != page_index)
    {
        if (current_index < page_index)
        {
            animation_info.gx_animation_start_position.gx_point_x += screen_width;
            current_index++;
        }
        else
        {
            animation_info.gx_animation_start_position.gx_point_x -= screen_width;
            current_index--;
        }

        if (current_index == page_index)
        {
            animation_info.gx_animation_style = GX_ANIMATION_TRANSLATE;
        }

        animation_info.gx_animation_end_position.gx_point_x = animation_info.gx_animation_start_position.gx_point_x + animation_dis;

        animation_info.gx_animation_target = info->window_list[current_index];

        if (gx_system_animation_get(&animation) == GX_SUCCESS)
        {
            /* Start animation to slide following pages to target position until
               the target page is on animation. */
            gx_animation_start(animation, &animation_info);
            page_slide_animation_count++;
        }
    }
}

/******************************************************************************************/
/* Event handler for GX_EVENT_ANIMATION_COMPLETE.                                         */
/******************************************************************************************/
static VOID on_animation_complete(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    SCREEN_BASE_CONTROL_BLOCK *base = (SCREEN_BASE_CONTROL_BLOCK*)window;

    switch (event_ptr->gx_event_sender)
    {
    case ANIMATION_ID_DRAG_SLIDE:
        /* Update pagination status. */
        pagination_button_update(window);
        break;

    case ANIMATION_ID_PAGE_SLIDE:
        page_slide_animation_count--;

        if (page_slide_animation_count == 0)
        {
            /* Update pagination status. */
            pagination_button_update(window);
        }
        break;

    case ANIMATION_ID_FOOTER_HOME:
        gx_widget_style_add(&base->screen_base_home, GX_STYLE_ENABLED);
        break;

    case ANIMATION_ID_FOOTER_ADD:
        gx_widget_style_add(&base->screen_base_add, GX_STYLE_ENABLED);
        break;

    case ANIMATION_ID_FOOTER_SETTING:
        gx_widget_style_add(&base->screen_base_setting, GX_STYLE_ENABLED);
        break;

    case ANIMATION_ID_SCREEN_CHILDREN:
        screen_animation_count--;

        if (screen_animation_count == 0)
        {
            /* Enable drag slide animation. */
            slide_animation_start(&base->screen_base_slide_win);
            pagination_button_enable_disable(window, GX_TRUE);
        }
        break;
    }
}

/******************************************************************************************/
/* Event handler for footer menu button click.                                            */
/******************************************************************************************/
VOID on_footer_menu_clicked(INT button_id)
{
    SCREEN_BASE_CONTROL_BLOCK *base;
    FOOTER_MENU_INFO *info = footer_menu_info_list;
    GX_PIXELMAP_BUTTON *button;
    GX_WIDGET *child;
    INT map_id;

    switch (button_id)
    {
    case ID_FOOTER_LIGHTS:
        base = &lights_screen.base;
        break;

    case ID_FOOTER_THERMOSTAT:
        base = &thermostat_screen.base;
        break;

    case ID_FOOTER_LOCKS:
        base = &locks_screen.base;
        break;

    case ID_FOOTER_WEATHER:
        base = &weather_screen.base;
        break;

    default:
        return;
    }

    while (info->footer_menu_id)
    {
        gx_widget_find(base, info->footer_menu_id, GX_SEARCH_DEPTH_INFINITE, &button);

        if (info->footer_menu_id == button_id)
        {
            map_id = info->footer_selected_icon;
        }
        else
        {
            map_id = info->footer_normal_icon;
        }

        child = button->gx_widget_first_child;

        while (child)
        {
            switch (child->gx_widget_type)
            {
            case GX_TYPE_PIXELMAP_BUTTON:
                gx_pixelmap_button_pixelmap_set((GX_PIXELMAP_BUTTON *)child, map_id, map_id, map_id);
                break;

            case GX_TYPE_PROMPT:
                if (info->footer_menu_id == button_id)
                {
                    gx_widget_hide(child);
                }
                else
                {
                    gx_widget_show(child);
                }

                break;
            }

            child = child->gx_widget_next;
        }

        info++;
    }

    toggle_screen((GX_WIDGET *)base);
}

/******************************************************************************************/
/* Override the default event processing of "slide_win" to handle signals from my child   */
/* widgets.                                                                               */
/******************************************************************************************/
UINT slide_win_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        slide_animation_start(window);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)window);
        return gx_widget_event_process(window, event_ptr);

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override the default event processing of templates based on "screen_base" to handle    */
/* signals from my child widgets.                                                         */
/******************************************************************************************/
UINT screen_base_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Start animation of footer icons. */
        footer_icons_animation_start((SCREEN_BASE_CONTROL_BLOCK *)window);
        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_HIDE:
        switch (window->gx_widget_id)
        {
        case ID_LIGHTS_SCREEN:
            lights_screen_animation_stop();
            break;

        case ID_THERMOSTAT_SCREEN:
            thermostat_screen_animation_stop();
            break;

        case ID_LOCKS_SCREEN:
            locks_screen_animation_stop();
            break;

        case ID_WEATHER_SCREEN:
            weather_screen_animation_stop();
            break;
        }

        /* Delete footer animations.  */
        gx_animation_delete(GX_NULL, (GX_WIDGET *)window);

        /* Disable drag slide animation. */
        gx_animation_drag_disable(&slide_animation, (GX_WIDGET*)&((SCREEN_BASE_CONTROL_BLOCK*)window)->screen_base_slide_win);

        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_HOME, GX_EVENT_CLICKED):
        if (!is_animation_active())
        {
            /* Toggle to home screen. */
            toggle_screen((GX_WIDGET *)&main_screen.main_screen_home_window);
        }
        break;

    case GX_SIGNAL(ID_ADD, GX_EVENT_CLICKED):
        if (!is_animation_active())
        {
            /* Toggle to passcode screen. */
            toggle_screen((GX_WIDGET *)&passcode_screen);
        }
        break;

    case GX_SIGNAL(ID_FOOTER_LIGHTS, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_FOOTER_THERMOSTAT, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_FOOTER_LOCKS, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_FOOTER_WEATHER, GX_EVENT_CLICKED):
        if (!is_animation_active())
        {
            on_footer_menu_clicked(event_ptr->gx_event_type >> 8);
        }
        break;

    case GX_SIGNAL(ID_PAGINATION_1, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_PAGINATION_2, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_PAGINATION_3, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_PAGINATION_4, GX_EVENT_RADIO_SELECT):
        if (!is_animation_active())
        {
            /* Disable drag slide animation. */
            gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)&((SCREEN_BASE_CONTROL_BLOCK *)window)->screen_base_slide_win);
            pagination_button_enable_disable(window, GX_FALSE);

            screen_list_slide(window, event_ptr->gx_event_type >> 8);
        }
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        if (window -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            on_animation_complete(window, event_ptr);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
