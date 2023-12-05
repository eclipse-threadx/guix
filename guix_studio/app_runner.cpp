/* Logic to run the application in a standalone window */

#include "studiox_includes.h"
#include "app_runner.h"
#include "gx_win32_studio_display_driver.h"
#include "gx_animation.h"

#include "gx_api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct APP_ANIMATION_STRUCT{
    GX_ANIMATION           *app_animation;
    APP_ANIMATION_STRUCT   *app_animation_next;
    INT                     app_animation_win_thread_id;
}APP_ANIMATION;

APP_ANIMATION *app_animation_created_list;
UINT           app_animation_id;
extern CString SCREEN_STACK_POP_STRING;

/**************************************************************************/
GX_WIDGET *app_child_widget_find(GX_WIDGET *parent, CString widget_name)
{
    GX_WIDGET *child;
    CString name;
    GX_WIDGET *found = GX_NULL;

    /* Is there a widget?  */
    if (parent)
    {
        child = parent->gx_widget_first_child;

        while (child)
        {
            name = (TCHAR *)child->gx_widget_name;
            if (name == widget_name)
            {
                found = child;
            }
            else if (child->gx_widget_first_child)
            {
                found = app_child_widget_find(child, widget_name);
            }

            if (found)
            {
                break;
            }

            child = child->gx_widget_next;
        }
    }

    if ((!found) && (parent->gx_widget_type == GX_TYPE_MENU))
    {
        GX_MENU *menu = (GX_MENU *)parent;
        found = app_child_widget_find((GX_WIDGET *)&menu->gx_menu_list, widget_name);
    }

    return found;
}

/**************************************************************************/
GX_WIDGET *app_widget_find(GX_WINDOW *root, CString widget_name)
{
    int loop;
    GX_WIDGET *screen;
    GX_WIDGET *found = GX_NULL;
    GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;
    GX_DISPLAY *display;
    CString name;

    display = ((GX_WINDOW_ROOT *)root)->gx_window_root_canvas->gx_canvas_display;
    driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display->gx_display_driver_data;

    if (widget_name.IsEmpty())
    {
        return GX_NULL;
    }

    for (loop = 0; loop < driver_instance->win32_screen_count; loop++)
    {
        screen = driver_instance->win32_screens[loop];

        if (screen)
        {
            name = (TCHAR *)screen->gx_widget_name;
            if (name == widget_name)
            {
                found = screen;
            }
            else
            {
                found = app_child_widget_find(screen, widget_name);
            }

            if (found)
            {
                break;
            }
        }
    }

    return found;
}

/**************************************************************************/
flow_item *app_flow_item_get(studiox_project *project, int display_index, GX_WIDGET *screen)
{
    flow_item *flow_item = GX_NULL;
    screen_flow *screen_flow = project->mDisplays[display_index].screenflow;

    if (screen_flow && screen)
    {
        CString widget_name = (TCHAR *)screen->gx_widget_name;
        flow_item = screen_flow->GetFlowItem(widget_name);
    }

    return flow_item;
}

/**************************************************************************/
void app_animation_clean_up(GX_WIN32_DISPLAY_DRIVER_DATA *data)
{
    APP_ANIMATION *entry = app_animation_created_list;
    APP_ANIMATION *pre = GX_NULL;
    APP_ANIMATION *next;

    while (entry)
    {
        next = entry->app_animation_next;
        if (entry->app_animation_win_thread_id == data->win32_window_ThreadId)
        {
            if (pre)
            {
                pre->app_animation_next = entry->app_animation_next;
            }
            else
            {
                app_animation_created_list = entry->app_animation_next;
            }

            if (entry->app_animation->gx_animation_status != GX_ANIMATION_IDLE)
            {
                gx_animation_stop(entry->app_animation);
            }

            delete entry->app_animation;
            delete entry;
        }
        else
        {
            pre = entry;
        }
        entry = next;
    }
}

/**************************************************************************/
GX_ANIMATION *app_animation_create(GX_WINDOW *root, GX_WIDGET *target)
{
    //Create a animation
    APP_ANIMATION *animation;
    GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;
    GX_DISPLAY *display;

    display = ((GX_WINDOW_ROOT *)root)->gx_window_root_canvas->gx_canvas_display;
    driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display->gx_display_driver_data;

    animation = new APP_ANIMATION;
    animation->app_animation = new GX_ANIMATION;
    gx_animation_create(animation->app_animation);
    animation->app_animation_win_thread_id = driver_instance->win32_window_ThreadId;

    GX_ENTER_CRITICAL
    animation->app_animation_next = app_animation_created_list;
    app_animation_created_list = animation;
    GX_EXIT_CRITICAL

    return animation->app_animation;
}

/**************************************************************************/
void app_animation_delete(UINT animation_id)
{
    if (!animation_id)
    {
        return;
    }

    GX_ENTER_CRITICAL
    // Delete a animation
    APP_ANIMATION *entry = app_animation_created_list;
    APP_ANIMATION *pre = GX_NULL;

    while (entry)
    {
        if (entry->app_animation->gx_animation_info.gx_animation_id == animation_id)
        {
            if (pre)
            {
                pre->app_animation_next = entry->app_animation_next;
            }
            else
            {
                app_animation_created_list = entry->app_animation_next;
            }

            delete entry->app_animation;
            delete entry;
            break;
        }
        pre = entry;
        entry = entry->app_animation_next;
    }
    GX_EXIT_CRITICAL
}

/**************************************************************************/
void app_animation_process(GX_ANIMATION_INFO *animation, GX_WINDOW *root, GX_WIDGET *parent, GX_WIDGET *target)
{
    GX_ANIMATION *app_animation = app_animation_create(root, target);

    if (app_animation)
    {
        GX_ANIMATION_INFO info;
        info = *animation;
        info.gx_animation_parent = parent;
        info.gx_animation_target = target;

        if (!info.gx_animation_id)
        {
            GX_ENTER_CRITICAL
            info.gx_animation_id = app_animation_id;
            app_animation_id++;
            GX_EXIT_CRITICAL
        }

        gx_animation_start(app_animation, &info);
    }
}

/**************************************************************************/
UINT app_trigger_action_process(studiox_project *project, int display_index, GX_WINDOW *root, CArray<action_info *> *action_list, GX_WIDGET *screen, GX_EVENT *event_ptr)
{
    action_info *action;
    GX_WIDGET *target;
    GX_WIDGET *parent;
    screen_flow *flow = project->mDisplays[display_index].screenflow;

    //handle child signals
    for (int index = 0; index < action_list->GetCount(); index++)
    {
        action = action_list->GetAt(index);

        switch (action->action_type)
        {
        case GX_ACTION_TYPE_ATTACH:
            if ((action->target_widget_name == SCREEN_STACK_POP_STRING) ||
                (action->parent_widget_name == SCREEN_STACK_POP_STRING))
            {
                gx_system_screen_stack_get(&parent, &target);
            }

            if (action->parent_widget_name != SCREEN_STACK_POP_STRING)
            {
                parent = app_widget_find(root, action->parent_widget_name);
            }

            if (action->target_widget_name != SCREEN_STACK_POP_STRING)
            {
                target = app_widget_find(root, action->target_widget_name);
            }

            if (!parent)
            {
                parent = (GX_WIDGET *)root;
            }

            if (parent && target)
            {
                gx_widget_attach(parent, target);
            }
            break;

        case GX_ACTION_TYPE_WINDOW_EXECUTE:
            if ((action->target_widget_name == SCREEN_STACK_POP_STRING) ||
                (action->parent_widget_name == SCREEN_STACK_POP_STRING))
            {
                gx_system_screen_stack_get(&parent, &target);
            }

            if (action->parent_widget_name != SCREEN_STACK_POP_STRING)
            {
                parent = app_widget_find(root, action->parent_widget_name);
            }

            if (action->target_widget_name != SCREEN_STACK_POP_STRING)
            {
                target = app_widget_find(root, action->target_widget_name);
            }

            if (!parent)
            {
                parent = (GX_WIDGET *)root;
            }

            if (parent && target && target->gx_widget_type == GX_TYPE_WINDOW)
            {
                gx_widget_attach(parent, target);
                gx_window_execute((GX_WINDOW *)target, GX_NULL);
            }
            break;

        case GX_ACTION_TYPE_WINDOW_EXECUTE_STOP:
            return event_ptr->gx_event_sender;
            break;

        case GX_ACTION_TYPE_DETACH:
            if (!action->target_widget_name.IsEmpty())
            {
                target = app_widget_find(root, action->target_widget_name);
            }
            else
            {
                target = screen;
            }

            if (target)
            {
                gx_widget_hide(target);
            }
            break;

        case GX_ACTION_TYPE_TOGGLE:
            if (action->target_widget_name == SCREEN_STACK_POP_STRING)
            {
                gx_system_screen_stack_get(&parent, &target);
            }
            else
            {
                target = app_widget_find(root, action->target_widget_name);
            }

            if (target)
            {
                gx_widget_attach(screen->gx_widget_parent, target);
                gx_widget_hide(screen);
            }
            break;

        case GX_ACTION_TYPE_SHOW:
            target = app_widget_find(root, action->target_widget_name);
            if (target)
            {
                gx_widget_show(target);
            }
            break;

        case GX_ACTION_TYPE_HIDE:
            target = app_widget_find(root, action->target_widget_name);
            if (target)
            {
                gx_widget_hide(target);
            }
            break;

        case GX_ACTION_TYPE_ANIMATION:
            if ((action->target_widget_name == SCREEN_STACK_POP_STRING) ||
                (action->parent_widget_name == SCREEN_STACK_POP_STRING))
            {
                gx_system_screen_stack_get(&parent, &target);
            }

            if (action->target_widget_name != SCREEN_STACK_POP_STRING)
            {
                target = app_widget_find(root, action->target_widget_name);
            }

            if (target && action->animation)
            {
                action->animation->gx_animation_id = project->GetIdIndex(display_index, ID_TYPE_ANIMATION, action->animation_id_name);
                if (action->parent_widget_name != SCREEN_STACK_POP_STRING)
                {
                    parent = app_widget_find(root, action->parent_widget_name);
                }

                if (!parent)
                {
                    //animation parent is not set
                    if (target->gx_widget_parent)
                    {
                        //set animation parent to widget's parent
                        parent = target->gx_widget_parent;
                    }
                    else
                    {
                        //set animation parent to root window
                        parent = (GX_WIDGET *)root;
                    }
                }

                app_animation_process(action->animation, (GX_WINDOW *)root, parent, target);
            }
            break;

        case GX_ACTION_TYPE_SCREEN_STACK_PUSH:
            target = app_widget_find(root, action->target_widget_name);
            if (target)
            {
                gx_system_screen_stack_push(target);
            }
            break;

        case GX_ACTION_TYPE_SCREEN_STACK_POP:
            gx_system_screen_stack_pop();
            break;

        case GX_ACTION_TYPE_SCREEN_STACK_RESET:
            gx_system_screen_stack_reset();
            break;
        }
    }

    return 0;
}

/**************************************************************************/
UINT app_root_event_process(GX_WINDOW *root, GX_EVENT *event_ptr)
{
    studiox_project *project = GetOpenProject();
    UINT status;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_ANIMATION_COMPLETE:
        app_animation_delete(event_ptr->gx_event_sender);
        break;
    }

    status = gx_window_event_process(root, event_ptr);

    if (project && root->gx_widget_first_child)
    {
        BOOL process = GX_FALSE;
        BOOL child_detect;
        int display_index;
        GX_WIDGET *screen = GX_NULL;
        flow_item *flow_item = NULL;
        trigger_info *trigger = GX_NULL;
        GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;
        GX_DISPLAY *display;

        display = ((GX_WINDOW_ROOT *)root)->gx_window_root_canvas->gx_canvas_display;
        driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display->gx_display_driver_data;
        display_index = driver_instance->win32_display_index;

        // Loop through top level screens
        for (int loop = 0; loop < driver_instance->win32_screen_count; loop++)
        {
            screen = driver_instance->win32_screens[loop];

            if (event_ptr->gx_event_target)
            {
                if (screen == event_ptr->gx_event_target)
                {
                    //the target is current screen
                    child_detect = GX_TRUE;
                }
                else
                {
                    //is event target a child of current screen?
                    gx_widget_child_detect(screen, event_ptr->gx_event_target, &child_detect);
                }
            }
            else
            {
                //no target specified
                child_detect = GX_TRUE;
            }

            if ((!(screen->gx_widget_status | GX_STATUS_VISIBLE)) ||
                (!child_detect))
            {
                screen = screen->gx_widget_next;
                continue;
            }

            //app_widget_animation_process(project, display_index, root, screen, event_ptr);
            
            // Find top level screen
            flow_item = app_flow_item_get(project, display_index, screen);

            if (flow_item)
            {
                trigger = flow_item->trigger_list;
            }

            while (trigger)
            {
                CArray<action_info *> *action_list;
                ULONG signal = 0;
                INT id;

                action_list = &trigger->action_list;

                switch (trigger->trigger_type)
                {
                case TRIGGER_TYPE_SYSTEM_EVENT:
                    if (trigger->event_type == event_ptr->gx_event_type)
                    {
                        if (event_ptr->gx_event_type == GX_EVENT_ANIMATION_COMPLETE)
                        {
                            id = project->GetIdIndex(display_index, ID_TYPE_ANIMATION, trigger->system_event_animat_id_name);
                            if (event_ptr->gx_event_sender == id)
                            {
                                process = TRUE;
                            }
                        }
                        else
                        {
                            process = TRUE;
                        }
                    }
                    break;

                case TRIGGER_TYPE_CHILD_SIGNAL:
                    id = project->GetIdIndex(display_index, ID_TYPE_WIDGET, trigger->signal_id_name);
                    signal = GX_SIGNAL(id, trigger->event_type);

                    if (signal == event_ptr->gx_event_type)
                    {
                        process = TRUE;
                    }
                    break;

                case TRIGGER_TYPE_USER_EVENT:
                    break;
                }

                if (process)
                {
                    return app_trigger_action_process(project, display_index, root, action_list, screen, event_ptr);
                }
                trigger = trigger->next;
            }

            screen = screen->gx_widget_next;
        }
    }

    return status;
}

extern "C" {

/**************************************************************************/
// This function is called by the application execution thread when the
// Windows window into which the application is rendering is closed. Cleanup
// the canvas, display, and root window
/**************************************************************************/

VOID guix_cleanup_win32_canvas(GX_WIN32_DISPLAY_DRIVER_DATA *driver)
{
    GX_WIDGET *screen;
    int loop;

    if (driver->win32_canvas)
    {
        gx_widget_hide((GX_WIDGET *)driver->win32_root_win);
        if (driver->win32_canvas->gx_canvas_memory)
        {
            delete (driver->win32_canvas->gx_canvas_memory);
        }
        
        resource_view::CleanupDisplayResources(driver->win32_display);
       
        guix_studio_delete_display(driver->win32_display);
        delete(driver->win32_display);

        while(driver->win32_root_win->gx_widget_first_child)
        {
            screen = driver->win32_root_win->gx_widget_first_child;
            gx_widget_detach(screen);
        }
        gx_window_root_delete(driver->win32_root_win);
        delete(driver->win32_root_win);

        // make sure none of the top level screen are attached to each other:
        for (loop = 0; loop < driver->win32_screen_count; loop++)
        {
            screen = driver->win32_screens[loop];

            if (screen)
            {
                if (screen->gx_widget_parent)
                {
                    gx_widget_detach(screen);
                }
            }
        }

        // now delete each of the top level screens:
        for (loop = 0; loop < driver->win32_screen_count; loop++)
        {
            screen = driver->win32_screens[loop];

            if (screen)
            {
                widget_factory::DeleteWidgets(screen);
            }
        }
        if (driver->win32_screens)
        {
            delete [] driver->win32_screens;
        }

        gx_canvas_delete(driver->win32_canvas);
        delete driver->win32_canvas;

        memset(driver, 0, sizeof(GX_WIN32_DISPLAY_DRIVER_DATA));
    }
}

/**************************************************************************/
void guix_execute_thread(ULONG thread_input)
{
    GX_WIN32_DISPLAY_DRIVER_DATA *data;

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    data = (GX_WIN32_DISPLAY_DRIVER_DATA *)thread_input;

    screen_flow *flow = project->mDisplays[data->win32_display_index].screenflow;
    int size = flow->GetFlowListCount() * 2;
    GX_WIDGET **memory = new GX_WIDGET*[size];
    _gx_system_screen_stack_create(memory, sizeof(GX_WIDGET *)* size);

    _gx_system_thread_entry(0);

    app_animation_clean_up(data);

    //save current win32 window position
    project->mHeader.app_execute_xpos = data->win32_window_xpos;
    project->mHeader.app_execute_ypos = data->win32_window_ypos;

    //delete system screen stack memory
    delete _gx_system_screen_stack.gx_screen_stack_control_memory;

    guix_cleanup_win32_canvas(data);

    ExitThread(0);
}
}

/**************************************************************************/
app_runner::app_runner()
{

}

/**************************************************************************/
app_runner::~app_runner()
{

}


/**************************************************************************/
BOOL app_runner::HaveStartupScreens(const CArray<widget_info *> *screen_list) const
{
    widget_info *screen;

    for (int index = 0; index < screen_list->GetCount(); index++)
    {
        screen = screen_list->GetAt(index);

        if (screen->visible_at_startup)
        {
            return TRUE;
        }
    }
    return FALSE;
}


/**************************************************************************/
void app_runner::RunApplication(int display_index, CWnd *parent)
{
GX_WIN32_DISPLAY_DRIVER_DATA *data;

int          memsize;
UCHAR        *canvas_memory;
GX_DISPLAY   *app_display;
GX_CANVAS    *app_canvas;
GX_WINDOW_ROOT *app_root;
GX_BOOL      status = GX_FALSE;
GX_RECTANGLE size;
CArray<widget_info *> screen_list;
widget_info *screen;
display_info *info;

    studiox_project *project = GetOpenProject();

    if (project)
    {
        info = &project->mDisplays[display_index];

        if (!info)
        {
            ErrorMsg("Internal Error: Invalid display", parent);
            return;
        }

        GetProjectView()->GetTopLevelWidgetList(display_index, &screen_list);

        if (!HaveStartupScreens(&screen_list))
        {
            ErrorMsg("No startup screen(s) have been defined, cannot run application", parent);
            return;
        }

        int canvas_xres = info->xres;
        int canvas_yres = info->yres;

        if (info->rotation_angle == GX_SCREEN_ROTATION_CCW ||
            info->rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            GX_SWAP_VALS(canvas_xres, canvas_yres);
        }

        app_display = new GX_DISPLAY;
        int active_theme = project->mDisplays[display_index].active_theme;

        memsize = guix_create_app_display(app_display, "GUIX App Display", 
                                         canvas_xres, canvas_yres,
                                         info->colorformat,
                                         project->mHeader.target_cpu,
                                         IsRenesasDave2D(project),
                                         IsDave2dFontFormat(project, display_index),
                                         info->themes[active_theme].palette,
                                         info->themes[active_theme].palette_total_size,
                                         project->mHeader.palette_mode_aa_text_colors);

        if (memsize)
        {
            data = (GX_WIN32_DISPLAY_DRIVER_DATA *) app_display->gx_display_driver_data;

            if (!data)
            {
                ErrorMsg("Internal Error: Invalid display driver data", parent);
                delete app_display;
                return;
            }

            GetResourceView()->BuildResourceTables(display_index, app_display, FALSE);

            canvas_memory = new UCHAR[memsize];
            app_canvas = new GX_CANVAS;
            app_root   = new GX_WINDOW_ROOT;

            app_animation_created_list = GX_NULL;
            app_animation_id = 1024;

            // save the dynamically allocated control block pointers, so that we can clean up
            // when the Win32 window is closed:

            data->win32_canvas = app_canvas;
            data->win32_root_win = app_root;
            data->win32_display = app_display;

            data->win32_window_xpos = project->mHeader.app_execute_xpos;
            data->win32_window_ypos = project->mHeader.app_execute_ypos;

            gx_canvas_create(app_canvas, "GUIX App Canvas",
                          app_display, GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                          canvas_xres, canvas_yres, (GX_COLOR *) canvas_memory, memsize);

            /* Create a background root window and attach to the background canvas.  */

            gx_utility_rectangle_define(&size, 0, 0, canvas_xres - 1, canvas_yres - 1);
            gx_window_root_create(app_root, "GUIX App Root Window", app_canvas,
                                   GX_STYLE_BORDER_NONE, GX_ID_NONE, &size);

            gx_widget_event_process_set(app_root, app_root_event_process);

            int screen_count = 0;
            for (int index = 0; index < screen_list.GetCount(); index++)
            {
                screen = screen_list.GetAt(index);
                if (screen->is_template)
                {
                    continue;
                }
                screen_count++;
            }

            data->win32_screen_count = screen_count;
            data->win32_screens = new GX_WIDGET *[screen_count];
            data->win32_display_index = display_index;

            screen_count = 0;
            
            for (int index = 0; index < screen_list.GetCount(); index++)
            {
                screen = screen_list.GetAt(index);

                if (screen->is_template)
                {
                    continue;
                }

                data->win32_screens[screen_count] = widget_factory::GenerateAppScreen(NULL, screen);
                if (screen->visible_at_startup)
                {
                    gx_widget_attach(app_root, data->win32_screens[screen_count]);
                }

                screen_count++;
            }

            /* create a Windows thread, hand off the configured canvas/display/root window */
            CreateThread(NULL, GX_WIN32_STACK_SIZE, (LPTHREAD_START_ROUTINE) gx_win32_studio_driver_thread_entry, data, 0, (LPDWORD)&data->win32_window_ThreadId);

            /* create a Windows thread to execute the GUIX event loop */
            CreateThread(NULL, GX_WIN32_STACK_SIZE, (LPTHREAD_START_ROUTINE) guix_execute_thread, data, 0, (LPDWORD)&data->win32_guix_ThreadId);

            gx_widget_show((GX_WIDGET *)app_root);
        }
        else
        {
            delete app_display;
        }
    }
}


