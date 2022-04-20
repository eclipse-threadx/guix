
#include "demo_guix_home_automation.h"

/* Define lock information struture. */
typedef struct LOCK_INFO_STRUCT{
    GX_BOOL lock_enabled; /* Mark if lock function is enabled. */
    GX_BOOL lock_all; /* Mark if all locks are enabled. */
}LOCK_INFO;

/* Define door information structure. */
typedef struct DOOR_INFO_STRUCT{
    LOCKS_DOOR_BASE_CONTROL_BLOCK *door; /* Pointer to locks controller widget. */
    GX_BOOL locked; /* Locks status. */
}DOOR_INFO;

/* Lock controller widget is based on "locks door base" widgets.
   This is a list of lock controller widget and the lock status of those widgets. */
static DOOR_INFO door_widget_list[] = {
    { &locks_page_1.locks_page_1_front_door, GX_TRUE },
    { &locks_page_2.locks_page_2_patio_door, GX_TRUE},
    { &locks_page_2.locks_page_2_upper_deck_door, GX_TRUE },
    { &locks_page_3.locks_page_3_lower_deck_door, GX_TRUE },
    { GX_NULL, 0}
};

/* Lock controller widget is based on "locks door base" widgets.
   This is a list of all lock controller widgets. */
static CONTROLLER_BASE_CONTROL_BLOCK *lock_controller_list[] = {
    &locks_page_1.locks_page_1_security_alarm,
    &locks_page_1.locks_page_1_front_door.base,
    &locks_page_2.locks_page_2_patio_door.base,
    &locks_page_2.locks_page_2_upper_deck_door.base,
    &locks_page_3.locks_page_3_lower_deck_door.base,
    GX_NULL
};

/* Initialize lock information. */
static LOCK_INFO lock_info = { GX_TRUE, GX_TRUE };

extern GX_ANIMATION slide_animation;
extern int screen_animation_count;

/******************************************************************************************/
/* Custom button select function.                                                         */
/******************************************************************************************/
static VOID lock_button_select(GX_BUTTON *button)
{
    GX_WIDGET *child = button->gx_widget_first_child;

    while (child)
    {
        gx_widget_style_add(child, GX_STYLE_DRAW_SELECTED);

        child = child->gx_widget_next;
    }

    gx_button_select(button);
}

/******************************************************************************************/
/* Custom button deselect function.                                                       */
/******************************************************************************************/
static VOID lock_button_deselect(GX_BUTTON *button, GX_BOOL generate)
{
    GX_WIDGET *child = button->gx_widget_first_child;

    while (child)
    {
        gx_widget_style_remove(child, GX_STYLE_DRAW_SELECTED);

        child = child->gx_widget_next;
    }

    gx_button_deselect(button, generate);
}

/******************************************************************************************/
/* Update text of "Enabled/Disabled" button.                                              */
/******************************************************************************************/
static VOID update_enable_disable_button_text()
{
    GX_RESOURCE_ID text_id;

    if (lock_info.lock_enabled)
    {
        text_id = GX_STRING_ID_ENABLED;
    }
    else
    {
        text_id = GX_STRING_ID_DISABLED;
    }

    gx_prompt_text_id_set(&locks_page_1.locks_page_1_enable_disable_text, text_id);
}

/******************************************************************************************/
/* Update appearance of "Lock All/Unlock All" button.                                     */
/******************************************************************************************/
static VOID update_lock_unlock_button_appearance()
{
    GX_RESOURCE_ID text_id;
    GX_RESOURCE_ID lock_normal_bg;
    GX_RESOURCE_ID lock_selecgted_bg;
    GX_RESOURCE_ID lock_normal_icon;
    GX_RESOURCE_ID lock_selected_icon;
    GX_RESOURCE_ID color_id;

    /* Set appearance properties with regard to different lock status. */
    if (lock_info.lock_enabled)
    {
        if (lock_info.lock_all)
        {
            text_id = GX_STRING_ID_UNLOCK_ALL;
            lock_normal_bg = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_SMALL_ACTIVE;
            lock_selecgted_bg = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_SMALL_ACTIVE_HOVER;
            lock_normal_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_SMALL_CLOSED_LOCK;
            lock_selected_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_SMALL_CLOSED_LOCK_HOVER;
            color_id = GX_COLOR_ID_BLUE;
        }
        else
        {
            text_id = GX_STRING_ID_LOCK_ALL;
            lock_normal_bg = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_SMALL;
            lock_selecgted_bg = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_SMALL_HOVER;
            lock_normal_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_SMALL_OPEN_LOCK;
            lock_selected_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_SMALL_OPEN_LOCK_HOVER;
            color_id = GX_COLOR_ID_GREEN;
        }
    }
    else
    {
        text_id = GX_STRING_ID_ACTIVE;
        lock_normal_bg = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_SMALL;
        lock_selecgted_bg = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_SMALL_HOVER;
        lock_normal_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_SMALL_CAMERA;
        lock_selected_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_SMALL_CAMERA_HOVER;
        color_id = GX_COLOR_ID_GREEN;
    }

    /* Set child prompt text. */
    gx_prompt_text_id_set(&locks_page_1.locks_page_1_lock_unlock_text, text_id);

    /* Set child prompt text color. */
    gx_prompt_text_color_set(&locks_page_1.locks_page_1_lock_unlock_text, color_id, color_id, color_id);

    /* Set child prompt fill color. */
    gx_widget_fill_color_set(&locks_page_1.locks_page_1_lock_unlock_text, color_id, color_id, color_id);

    /* Set child icon pixelmaps. */
    gx_icon_pixelmap_set(&locks_page_1.locks_page_1_lock_unlock_icon, lock_normal_icon, lock_selected_icon);

    /* Set child icon fill colors. */
    gx_widget_fill_color_set(&locks_page_1.locks_page_1_lock_unlock_icon, color_id, color_id, color_id);

    /* Set "Lock All/Unlock All" button pixelmaps. */
    gx_pixelmap_button_pixelmap_set(&locks_page_1.locks_page_1_button_lock, lock_normal_bg, lock_selecgted_bg, lock_normal_bg);
}

/******************************************************************************************/
/* lock/unlock one doors.                                                                 */
/******************************************************************************************/
static VOID lock_unlock_one_door(LOCKS_DOOR_BASE_CONTROL_BLOCK *base, GX_BOOL lock)
{
    GX_RESOURCE_ID button_normal_icon;
    GX_RESOURCE_ID button_selected_icon;
    GX_RESOURCE_ID lock_normal_icon;
    GX_RESOURCE_ID lock_selected_icon;
    GX_RESOURCE_ID lock_text_id;
    GX_WIDGET *child;
    GX_RESOURCE_ID color_id;
    GX_PIXELMAP_BUTTON *button = &base->locks_door_base_button_lock;

    /* Set appearance properties with regard different lock status. */
    if (lock)
    {
        button_normal_icon = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_BIG_ACTIVE;
        button_selected_icon = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_BIG_ACTIVE_HOVER;
        lock_normal_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_CLOSED_LOCK;
        lock_selected_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_CLOSED_LOCK_HOVER;
        lock_text_id = GX_STRING_ID_LOCKED;
        color_id = GX_COLOR_ID_BLUE;
    }
    else
    {
        button_normal_icon = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_BIG;
        button_selected_icon = GX_PIXELMAP_ID_LOCKS_ROUND_BTN_BIG_HOVER;
        lock_normal_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_OPEN_LOCK;
        lock_selected_icon = GX_PIXELMAP_ID_LOCKS_ICON_BTN_OPEN_LOCK_HOVER;
        lock_text_id = GX_STRING_ID_UNLOCKED;
        color_id = GX_COLOR_ID_GREEN;
    }

    child = button->gx_widget_first_child;

    while (child)
    {
        switch (child->gx_widget_type)
        {
        case GX_TYPE_ICON:
            /* Set child icon background. */
            gx_icon_pixelmap_set((GX_ICON *)child, lock_normal_icon, lock_selected_icon);
            break;

        case GX_TYPE_PROMPT:
            /* Set child prompt text. */
            gx_prompt_text_id_set((GX_PROMPT *)child, lock_text_id);

            /* Set child prompt text color. */
            gx_prompt_text_color_set((GX_PROMPT *)child, color_id, color_id, color_id);
            break;
        }

        /* Set child widget fill color. */
        gx_widget_fill_color_set(child, color_id, color_id, color_id);

        child = child->gx_widget_next;
    }

    gx_pixelmap_button_pixelmap_set(button, button_normal_icon, button_selected_icon, button_normal_icon);
}

/******************************************************************************************/
/* lock/unlock all doors.                                                                 */
/******************************************************************************************/
static VOID lock_unlock_all_doors(GX_BOOL lock)
{
    DOOR_INFO *info = door_widget_list;

    while (info->door)
    {
        /* Lock/Ulock one door. */
        lock_unlock_one_door(info->door, lock);

        info++;
    }
}

/******************************************************************************************/
/* Event handler for "Enabled/Disabled" button click.                                     */
/******************************************************************************************/
static VOID on_enabled_disabled_security_alarm()
{
    if (lock_info.lock_enabled)
    {
        lock_info.lock_enabled = GX_FALSE;
    }
    else
    {
        lock_info.lock_enabled = GX_TRUE;
    }

    /* Update button text. */
    update_enable_disable_button_text();

    /* Update button appearance. */
    update_lock_unlock_button_appearance();
}

/******************************************************************************************/
/* Event handler for "Lock All"/"Unlock All" button click.                                */
/******************************************************************************************/
static VOID on_lock_unlock_all()
{
    if (lock_info.lock_enabled)
    {
        if (lock_info.lock_all)
        {
            lock_info.lock_all = GX_FALSE;
        }
        else
        {
            lock_info.lock_all = GX_TRUE;
        }

        lock_unlock_all_doors(lock_info.lock_all);
    }
    else
    {
        lock_info.lock_enabled = GX_TRUE;
    }

    /* Update button text. */
    update_enable_disable_button_text();

    /* Update button appearance. */
    update_lock_unlock_button_appearance();
}

/******************************************************************************************/
/* Event handler for "Lock" button click.                                                 */
/******************************************************************************************/
static VOID on_lock_button_clicked(LOCKS_DOOR_BASE_CONTROL_BLOCK *base)
{
    DOOR_INFO *info = door_widget_list;

    while (info->door)
    {
        if (info->door == base)
        {
            /* Switch lock status. */
            if (info->locked)
            {
                info->locked = GX_FALSE;
            }
            else
            {
                info->locked = GX_TRUE;
            }

            lock_unlock_one_door(base, info->locked);
            break;
        }

        info++;
    }
}

/******************************************************************************************/
/* Event handler for lock close button click.                                             */
/******************************************************************************************/
static VOID on_close_button_clicked(LOCKS_DOOR_BASE_CONTROL_BLOCK *base)
{
    /* detach lock button. */
    gx_widget_detach(&base->locks_door_base_button_lock);

    /* detach close button. */
    gx_widget_detach(&base->locks_door_base_button_close);

    /* attach open button. */
    gx_widget_attach((GX_WIDGET *)base, &base->locks_door_base_button_open);
}

/******************************************************************************************/
/* Event handler for lock open button click.                                              */
/******************************************************************************************/
static VOID on_open_button_clicked(LOCKS_DOOR_BASE_CONTROL_BLOCK *base)
{
    /* attach lock button. */
    gx_widget_attach((GX_WIDGET *)base, &base->locks_door_base_button_lock);

    /* attach close button. */
    gx_widget_attach((GX_WIDGET *)base, &base->locks_door_base_button_close);

    /* detach open button. */
    gx_widget_detach(&base->locks_door_base_button_open);
}

/******************************************************************************************/
/* Override the default event processing of "locks_page_1" to handle signals from my      */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT locks_page_1_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTON_ENABLE_DISABLE, GX_EVENT_CLICKED):
        on_enabled_disabled_security_alarm();
        break;

    case GX_SIGNAL(ID_BUTTON_LOCK_UNLOCK_ALL, GX_EVENT_CLICKED):
        on_lock_unlock_all();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override the default event processing of templates based "locks_door_base" to handle   */
/* signals from my child widgets.                                                         */
/******************************************************************************************/
UINT locks_door_base_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_LOCK_BUTTON, GX_EVENT_CLICKED):
        on_lock_button_clicked((LOCKS_DOOR_BASE_CONTROL_BLOCK *)window);
        break;

    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        on_close_button_clicked((LOCKS_DOOR_BASE_CONTROL_BLOCK *)window);
        break;

    case GX_SIGNAL(ID_OPEN, GX_EVENT_CLICKED):
        on_open_button_clicked((LOCKS_DOOR_BASE_CONTROL_BLOCK *)window);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Hide some children of lock widget.                                                     */
/******************************************************************************************/
static VOID hide_locks_children(CONTROLLER_BASE_CONTROL_BLOCK *widget)
{
    GX_WIDGET *child = widget->gx_widget_first_child;

    while (child)
    {
        if (child->gx_widget_id != ID_STATUS)
        {
            gx_widget_hide(child);
        }

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Start animation for lock widget.                                                       */
/******************************************************************************************/
static VOID start_lock_children_animation(CONTROLLER_BASE_CONTROL_BLOCK *widget)
{
    GX_WIDGET *child = widget->gx_widget_first_child;

    while (child)
    {
        if (child->gx_widget_id != ID_STATUS)
        {
            gx_widget_show(child);
        }

        child = child->gx_widget_next;
    }

    /* Disable drag slide animation before screen item animations complete. */
    gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)&locks_screen.base.screen_base_slide_win);

    /* Disabled pagination buttons before screen item animations complete. */
    pagination_button_enable_disable((GX_WINDOW *)&locks_screen.base, GX_FALSE);

    /* Slide in controller title. */
    title_animation_start(widget);
}

/******************************************************************************************/
/* Reset locks screen.                                                                    */
/******************************************************************************************/
VOID locks_screen_reset()
{
    DOOR_INFO *info = door_widget_list;

    while (info->door)
    {
        if (info->door->base.gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* Hide child widgets for lock controller. */
            hide_locks_children(&info->door->base);
        }

        info++;
    }

    /* Hide child widgets for security alarm controller. */
    hide_locks_children(&locks_page_1.locks_page_1_security_alarm);
}

/******************************************************************************************/
/* Start animation for locks screen      .                                                */
/******************************************************************************************/
VOID locks_screen_animation_start()
{
    CONTROLLER_BASE_CONTROL_BLOCK *lock_controller;
    INT index = 0;

    while (1)
    {
        lock_controller = lock_controller_list[index];

        if (lock_controller)
        {
            if (lock_controller->gx_widget_status & GX_STATUS_VISIBLE)
            {
                /* lock controller is get visible. */
                if (!(lock_controller->controller_base_title.gx_widget_status & GX_STATUS_VISIBLE))
                {
                    /* Title of the lock controller is not visible, which means lock page switched.
                       start children animations for the lock controller. */
                    start_lock_children_animation(lock_controller);
                }
            }
            else
            {
                /* Hide some child widgets for invisible lock controllers.
                   Next time the lock controller get visible, those widgets will show with animations. */
                hide_locks_children(lock_controller);
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
/* Stop all animations in locks screen.                                                   */
/******************************************************************************************/
VOID locks_screen_animation_stop()
{
    CONTROLLER_BASE_CONTROL_BLOCK* lock_controller;
    INT index = 0;

    while (1)
    {
        lock_controller = lock_controller_list[index];

        if (lock_controller)
        {
            if (lock_controller->gx_widget_status & GX_STATUS_VISIBLE)
            {
                gx_animation_delete(GX_NULL, (GX_WIDGET*)lock_controller);
            }

            index++;
        }
        else
        {
            break;
        }
    }
    
    screen_animation_count = 0;
}

/******************************************************************************************/
/* Initiate lock screen information.                                                      */
/******************************************************************************************/
VOID locks_screen_init()
{
    DOOR_INFO *info = door_widget_list;
    LOCKS_DOOR_BASE_CONTROL_BLOCK *door;
    while (info->door)
    {
        door = info->door;

        /* Detach lock open button, this button will be attached when
           lock is closed. */
        gx_widget_detach((GX_WIDGET *)&door->locks_door_base_button_open);

        /* Set custom button select/deselect handler for lock buttons. */
        door->locks_door_base_button_lock.gx_button_select_handler = (VOID(*)(GX_WIDGET *))lock_button_select;
        door->locks_door_base_button_lock.gx_button_deselect_handler = (VOID(*)(GX_WIDGET *, GX_BOOL))lock_button_deselect;
        
        /* Hide some child widgets for invisible lock controllers.
           Next time the lock controller get visible, those widgets will show with animations. */
        hide_locks_children(&door->base);

        info++;
    }

    /* Hide some child widgets for invisible security alarm controller.
       Next time the security alarm controller is visible, those widgets will show with animations. */
    hide_locks_children(&locks_page_1.locks_page_1_security_alarm);

    /* Set text for lock controllers. */
    locks_title_init();

    /* Set custom button select/deselect handler for enabled/disabled and lock/unlock buttons. */
    locks_page_1.locks_page_1_button_enable_diable.gx_button_select_handler = (VOID(*)(GX_WIDGET *))lock_button_select;
    locks_page_1.locks_page_1_button_enable_diable.gx_button_deselect_handler = (VOID(*)(GX_WIDGET *, GX_BOOL))lock_button_deselect;
    locks_page_1.locks_page_1_button_lock.gx_button_select_handler = (VOID(*)(GX_WIDGET *))lock_button_select;
    locks_page_1.locks_page_1_button_lock.gx_button_deselect_handler = (VOID(*)(GX_WIDGET *, GX_BOOL))lock_button_deselect;

    /* Set "Add Lock" widget text. */
    gx_prompt_text_id_set(&locks_screen.base.screen_base_add_text, GX_STRING_ID_ADD_LOCK);
}
