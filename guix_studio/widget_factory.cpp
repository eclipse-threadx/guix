
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern WIDGET_TABLE_ENTRY widget_table[];
#define PROVIDER_TABLE_SIZE 64
static widget_service_provider *service_providers[PROVIDER_TABLE_SIZE];

///////////////////////////////////////////////////////////////////////////////
widget_factory::widget_factory()
{
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::InitServiceProviders()
{
    memset(service_providers, 0, PROVIDER_TABLE_SIZE * sizeof(widget_service_provider *));

    int index = 0;
    service_providers[index] = new widget_service_provider();
    index++;
    service_providers[index] = new button_service_provider();
    index++;
    service_providers[index] = new text_button_service_provider();
    index++;
    service_providers[index] = new mlt_button_service_provider();
    index++;
    service_providers[index] = new checkbox_service_provider();
    index++;
    service_providers[index] = new radio_button_service_provider();
    index++;
    service_providers[index] = new icon_button_service_provider();
    index++;
    service_providers[index] = new pixelmap_button_service_provider();
    index++;
    service_providers[index] = new icon_service_provider();
    index++;
    service_providers[index] = new slider_service_provider();
    index++;
    service_providers[index] = new pixelmap_slider_service_provider();
    index++;
    service_providers[index] = new progress_bar_service_provider();
    index++;
    service_providers[index] = new radial_progress_bar_service_provider();
    index++;
    service_providers[index] = new radial_slider_service_provider();
    index++;
    service_providers[index] = new sprite_service_provider();
    index++;
    service_providers[index] = new prompt_service_provider();
    index++;
    service_providers[index] = new numeric_prompt_service_provider();
    index++;
    service_providers[index] = new pixelmap_prompt_service_provider();
    index++;
    service_providers[index] = new numeric_pixelmap_prompt_service_provider();
    index++;
    service_providers[index] = new window_service_provider();
    index++;
    service_providers[index] = new vertical_list_service_provider();
    index++;
    service_providers[index] = new horizontal_list_service_provider();
    index++;
    service_providers[index] = new drop_list_service_provider();
    index++;
    service_providers[index] = new scroll_wheel_service_provider();
    index++;
    service_providers[index] = new text_scroll_wheel_service_provider();
    index++;
    service_providers[index] = new string_scroll_wheel_service_provider();
    index++;
    service_providers[index] = new numeric_scroll_wheel_service_provider();
    index++;
    service_providers[index] = new text_input_service_provider();
    index++;
    service_providers[index] = new ml_text_view_service_provider();
    index++;
    service_providers[index] = new ml_text_input_service_provider();
    index++;
    service_providers[index] = new hscroll_service_provider();
    index++;
    service_providers[index] = new vscroll_service_provider();
    index++;
    service_providers[index] = new circular_gauge_service_provider();
    index++;
    service_providers[index] = new line_chart_service_provider();
    index++;
    service_providers[index] = new template_service_provider();
    index++;
    service_providers[index] = new menu_service_provider();
    index++;
    service_providers[index] = new accordion_menu_service_provider();
    index++;
    service_providers[index] = new tree_view_service_provider();
    index++;
    service_providers[index] = new rich_text_view_service_provider();
    index++;
    service_providers[index] = new generic_scroll_wheel_service_provider();
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::DeleteServiceProviders()
{
    int index = 0;

    for (index = 0; index < PROVIDER_TABLE_SIZE; index++)
    {
        if (service_providers[index])
        {
            delete service_providers[index];
            service_providers[index] = NULL;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::ResetServiceProviders(int provider_type)
{
    int index = 0;

    for (index = 0; index < PROVIDER_TABLE_SIZE; index++)
    {
        if (service_providers[index])
        {
            if (provider_type >= 0)
            {
                if (service_providers[index]->GetType() == provider_type)
                {
                    service_providers[index]->Reset();
                    break;
                }
            }
            else
            {
                service_providers[index]->Reset();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int widget_factory::GetWidgetType(int index)
{
    if (index >= 0 && index < PROVIDER_TABLE_SIZE)
    {
        if (service_providers[index])
        {
            return(service_providers[index]->GetType());
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
widget_service_provider *widget_factory::GetServiceProvider(int type)
{
    int index = 0;

    for (index = 0; index < PROVIDER_TABLE_SIZE; index++)
    {
        if (service_providers[index])
        {
            if (service_providers[index]->GetType() == type)
            {
                return service_providers[index];
            }
        }
        else
        {
            break;
        }
    }
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
CString widget_factory::WidgetTypeToString(int type)
{
    widget_service_provider *provider = widget_factory::GetServiceProvider(type);
    if (provider)
    {
        return provider->GetShortName();
    }
    return CString("unknown");
}

///////////////////////////////////////////////////////////////////////////////
int widget_factory::WidgetStringToType(CString &name)
{
    int index = 0;
    widget_service_provider *provider;

    while(service_providers[index])
    {
        provider = service_providers[index];
        if (name.Compare(provider->GetShortName()) == 0)
        {
            return provider->GetType();
        }
        index++;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
#if 0
BOOL widget_factory::GenerateWidgets(folder_info *folder, BOOL save_ptrs)
{
    studiox_project *project = GetOpenProject();
    BOOL status = TRUE;
    if (!project)
    {
        return FALSE;
    }

    while (folder)
    {
        status |= GenerateWidgets(NULL, folder->GetFirstChildWidget(), save_ptrs);
        folder = folder->GetNextFolder();
    }

    return status;
}
#endif

///////////////////////////////////////////////////////////////////////////////
BOOL widget_factory::GenerateWidgets(GX_WIDGET *parent, widget_info *child, BOOL save_ptrs, BOOL do_siblings)
{
    BOOL status = TRUE;
    GX_WIDGET *attach = parent;
    GX_WIDGET *created;
    studiox_project *project = GetOpenProject();
    int display_index;
    GX_VALUE   list_count = 0;
    GX_VALUE   list_total_count = 0;
    GX_WIDGET *list_child;

    if (!project)
    {
        return FALSE;
    }

    if (parent)
    {
        if ((parent ->gx_widget_type == GX_TYPE_DROP_LIST) && (child->basetype == GX_TYPE_VERTICAL_LIST))
        {
            GX_DROP_LIST *drop = (GX_DROP_LIST *) parent;
            GX_VERTICAL_LIST *list;
            gx_drop_list_popup_get(drop, &list);
            attach = (GX_WIDGET *) list;
        }
        else if (parent->gx_widget_type == GX_TYPE_MENU)
        {
            list_total_count = ((GX_MENU *)parent)->gx_menu_list_total_count;

            list_child = ((GX_MENU *)parent)->gx_menu_list.gx_widget_first_child;
            while (list_child)
            {
                list_count++;
                list_child = list_child->gx_widget_next;
            }
        }
    }

    while(child && status)
    {
        widget_service_provider *provider = GetServiceProvider(child->basetype);

        if (provider)
        {
            if (list_count < list_total_count)
            {
                created = provider->GenerateFromInfo(GX_NULL, child);
                gx_menu_insert((GX_MENU *)attach, created);
                ((GX_MENU *)attach)->gx_menu_list_total_count--;
                list_count++;
            }
            else
            {
                created = provider->GenerateFromInfo(attach, child);
            }

            if (!save_ptrs && created)
            {
                if (!child->accepts_focus)
                {
                    gx_widget_status_remove(created, GX_STATUS_ACCEPTS_FOCUS);
                }

                if (!child->id_name.IsEmpty())
                {
                    //create app screen. 
                    display_index = project->GetDisplayIndex(child);

                    if (display_index >= 0)
                    {
                        created->gx_widget_id = project->GetIdIndex(display_index, ID_TYPE_WIDGET, child->id_name);
                    }
                }
            }

            /* If we are creating the widget(s) for the target view, we want to save the widget
               pointers in the WIDGET_INFO structures. If we are creating the widget(s) for the
               runtime preview, we just create the widgets and they will be deleted when the
               application window is closed
            */

            if (save_ptrs)
            {
                child->widget = created;

                if (created)
                {
                    if (created->gx_widget_type == GX_TYPE_SPRITE)
                    {
                        // don't pass auto or loop styles, we don't want the sprite running
                        // inside the target view
                        created->gx_widget_style &= ~(GX_STYLE_SPRITE_AUTO|GX_STYLE_SPRITE_LOOP);
                    }
                }
            }
        }
        else
        {
            ErrorMsg("Project contains invalid widget type.");
            status = FALSE;
        }
        if (created && child->GetChildWidgetInfo() && status)
        {
            GenerateWidgets(created, child->GetChildWidgetInfo(), save_ptrs);
        }

        if (do_siblings)
        {
            child = child->GetNextWidgetInfo();
        }
        else
        {
            child = NULL;
        }
    }
    return status;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *widget_factory::GenerateAppScreen(GX_WIDGET *root, widget_info *child)
{
    GX_WIDGET *created = GX_NULL;

    widget_service_provider *provider = GetServiceProvider(child->basetype);
    studiox_project *project = GetOpenProject();
    int display_index;

    if (!project)
    {
        return FALSE;
    }

    if (provider)
    {
        created = provider->GenerateFromInfo(root, child);

        if (!child->id_name.IsEmpty() && created)
        {
            display_index = project->GetDisplayIndex(child);
            created->gx_widget_id = project->GetIdIndex(display_index, ID_TYPE_WIDGET, child->id_name);
        }

        if (!created)
        {
            ErrorMsg("Unable to create application widget instance.");
        }
    }
    else
    {
        ErrorMsg("Project contains invalid widget type.");
    }
    if (child->GetChildWidgetInfo() && created)
    {
        GenerateWidgets(created, child->GetChildWidgetInfo(), FALSE);
    }
    return created;
}


///////////////////////////////////////////////////////////////////////////////
// When a widget is moved, either by dragging it or by typing new values into
// the properties window, this function is called to update the widget_info
// structures for the moved widget and all of it's children.
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void widget_factory::PositionPopupList(widget_info *info, GX_DROP_LIST *drop)
{
    GX_VERTICAL_LIST *list = &drop->gx_drop_list_popup.gx_popup_list_list;

    GX_RECTANGLE size;
    size.gx_rectangle_top = drop->gx_widget_size.gx_rectangle_bottom + 1;
    size.gx_rectangle_bottom = size.gx_rectangle_top + info->ewi.drop_list.open_height - 1;
    size.gx_rectangle_left = drop ->gx_widget_size.gx_rectangle_left;
    size.gx_rectangle_right = drop ->gx_widget_size.gx_rectangle_right;
    gx_widget_resize(list, &size);
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::UpdateWidgetInfoPositions(widget_info *info, int xshift, int yshift, BOOL do_next)
{
    while (info)
    {
        if (info->basetype == GX_TYPE_DROP_LIST)
        {
            if (info->widget)
            {   
                GX_DROP_LIST *list = (GX_DROP_LIST *) info->widget;
                PositionPopupList(info, list);
            }
        }
        info->size.gx_rectangle_left += xshift;
        info->size.gx_rectangle_right += xshift;
        info->size.gx_rectangle_top += yshift;
        info->size.gx_rectangle_bottom += yshift;

        //info->size = info->widget->gx_widget_size;

        if (info->GetChildWidgetInfo())
        {
            UpdateWidgetInfoPositions(info->GetChildWidgetInfo(), xshift, yshift, TRUE);
        }
        
        if (do_next)
        {
            info = info->GetNextWidgetInfo();
        }
        else
        {
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::MoveWidget(widget_info *info, GX_RECTANGLE &size)
{
    int width;
    int height;
    int xshift;
    int yshift;

    if (info->basetype == GX_TYPE_TEMPLATE)
    {
        int new_width = size.gx_rectangle_right - size.gx_rectangle_left + 1;
        int new_height = size.gx_rectangle_bottom - size.gx_rectangle_top + 1;
        width = info->size.gx_rectangle_right - info->size.gx_rectangle_left + 1;
        height = info->size.gx_rectangle_bottom - info->size.gx_rectangle_top + 1;

        if ((new_width != width) || (new_height != height))
        {
            Notify("This widget is derived from a template, to resize the widget you must resize the template on which this widget is based.");
            return;
        }
    }
    if (info->widget)
    {
        gx_widget_resize(info->widget, &size);
    }

    xshift = size.gx_rectangle_left - info->size.gx_rectangle_left;
    yshift = size.gx_rectangle_top - info->size.gx_rectangle_top;
    info->size = size;

    if (info->basetype == GX_TYPE_RADIAL_PROGRESS_BAR)
    {
        info->ewi.radial_progress = ((GX_RADIAL_PROGRESS_BAR *)info->widget)->gx_radial_progress_bar_info;
    }

    if (xshift || yshift)
    {
        if (info->GetChildWidgetInfo())
        {
            UpdateWidgetInfoPositions(info->GetChildWidgetInfo(), xshift, yshift, TRUE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::DeleteWidgets(GX_WIDGET *start)
{
    GX_WIDGET *next;
    GX_MENU_LIST *list;

    while (start)
    {
        if (start->gx_widget_type == GX_TYPE_MENU_LIST)
        {
            /* Skip menu list type widgets. */
            start = start->gx_widget_next;
            continue;
        }

        next = start->gx_widget_next;
        while (next && next->gx_widget_type == GX_TYPE_MENU_LIST)
        {
            /* Skip menu list type widgets. */
            next = next->gx_widget_next;
        }

        if (start->gx_widget_first_child)
        {
            if (start->gx_widget_type != GX_TYPE_HORIZONTAL_SCROLL &&
                start->gx_widget_type != GX_TYPE_VERTICAL_SCROLL)
            {
                DeleteWidgets(start->gx_widget_first_child);
            }
        }

        if (start->gx_widget_type == GX_TYPE_MENU)
        {
            /* Delete menu list of menu widget. */
            list = &((GX_MENU *)start)->gx_menu_list;
            if (list->gx_widget_first_child)
            {
                DeleteWidgets(list->gx_widget_first_child);
            }
        }

        DeleteOneWidget(start);
        start = next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::DeleteOneWidget(GX_WIDGET *widget)
{
    if (!widget)
    {
        return;
    }

    GX_WIDGET *parent = widget->gx_widget_parent;

    if (parent && parent->gx_widget_type == GX_TYPE_MENU_LIST)
    {
        parent = ((GX_MENU_LIST *)parent)->gx_menu_list_owner;
        gx_menu_remove((GX_MENU *)parent, widget);
    }

    // for the text input widget types, check to see if
    // we need to delete the text buffer:

    switch(widget->gx_widget_type)
    {
    case GX_TYPE_DROP_LIST:
        {
        GX_DROP_LIST *drop = (GX_DROP_LIST *) widget;
        gx_widget_delete(&drop->gx_drop_list_popup.gx_popup_list_list);
        }
        break;

    case GX_TYPE_MENU:
        {
            GX_MENU *menu = (GX_MENU *)widget;
            gx_widget_delete(&menu->gx_menu_list);
        }
        break;

    case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
        {
        GX_SINGLE_LINE_TEXT_INPUT *pi = (GX_SINGLE_LINE_TEXT_INPUT *) widget;
        if (pi->gx_single_line_text_input_buffer)
        {
            delete [] pi->gx_single_line_text_input_buffer;
            pi->gx_single_line_text_input_buffer = NULL;
        }
        }
        break;

    case GX_TYPE_MULTI_LINE_TEXT_INPUT:
        {
        GX_MULTI_LINE_TEXT_INPUT *mi = (GX_MULTI_LINE_TEXT_INPUT *) widget;
        if (mi->gx_multi_line_text_view_text.gx_string_ptr)
        {
            delete [] mi->gx_multi_line_text_view_text.gx_string_ptr;
            mi->gx_multi_line_text_view_text.gx_string_ptr = NULL;
            mi->gx_multi_line_text_view_text.gx_string_length = 0;
        }
        }
        break;

    default:
        break;
    }

    gx_widget_delete(widget);
    delete widget;
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::CleanupWidgets(folder_info *start)
{
    while (start)
    {
        if (start->GetFirstChildWidget())
        {
            CleanupWidgets(start->GetFirstChildWidget(), TRUE, TRUE);
        }
        start = start->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::CleanupWidgets(widget_info *start, BOOL recursive, BOOL do_siblings)
{
    GX_WIDGET *template_child;

    while(start)
    {
        if (recursive && start->GetChildWidgetInfo())
        {
            CleanupWidgets(start->GetChildWidgetInfo(), TRUE, TRUE);
        }

        if (start->basetype == GX_TYPE_TEMPLATE)
        {
            /* We delete the widget_info structures and associated GX_WIDGETs "bottom_up",
               meaning we delete the child widget_info before deleting the parent. So if we
               get to this stage a template widget still has chidren, those children do not
               have their own widget_info, they were created as part of creating a template
               instance. Delete them here.
            */

            if (start->widget)
            {
                while(start->widget->gx_widget_first_child)
                {
                    template_child = start->widget->gx_widget_first_child;
                    DeleteWidgets(template_child);
                }
            }
        }

        if (start->widget)
        {
            DeleteOneWidget(start->widget);
            start->widget = NULL;
        }
        if (do_siblings)
        {
            start = start->GetNextWidgetInfo();
        }
        else
        {
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL widget_factory::FindAppName(widget_info *start, widget_info *find, BOOL search_child)
{
    while(start)
    {
        if ((find != start) &&
            (find->app_name == start->app_name))
        {
            return TRUE;
        }
        if (search_child && start->GetChildWidgetInfo())
        {
            if (FindAppName(start->GetChildWidgetInfo(), find, search_child))
            {
                return TRUE;
            }
        }
        start = start->GetNextWidgetInfo();
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL widget_factory::FindAppName(widget_info *start, CString &find_name, BOOL search_child)
{
    while (start)
    {
        if (find_name == start->app_name)
        {
            return TRUE;
        }
        if (search_child && start->GetChildWidgetInfo())
        {
            if (FindAppName(start->GetChildWidgetInfo(), find_name, search_child))
            {
                return TRUE;
            }
        }
        start = start->GetNextWidgetInfo();
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL widget_factory::FindAppName(folder_info *folder, CString &find_name, BOOL search_child)
{
    while (folder)
    {
        if (FindAppName(folder->GetFirstChildWidget(), find_name, search_child))
        {
            return TRUE;
        }
        folder = folder->GetNextFolder();
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL widget_factory::FindAppName(folder_info *folder, widget_info *find, BOOL search_child)
{
    while (folder)
    {
        if (FindAppName(folder->GetFirstChildWidget(), find, search_child))
        {
            return TRUE;
        }
        folder = folder->GetNextFolder();
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL widget_factory::FindBaseName(CString &find_name)
{
    widget_service_provider *provider = NULL;

    for (int index = 0; index < PROVIDER_TABLE_SIZE; index++)
    {
        provider = service_providers[index];

        if (provider)
        {
            if (find_name == provider->GetShortName())
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::CreateUniqueAppNames(widget_info *start, widget_info *search_also, widget_info *search_start, BOOL is_top_level_widget)
{
    CString out;
    BOOL search_child;
    widget_info *new_search_also = search_also;

    if (is_top_level_widget)
    {
        search_child = FALSE;
    }
    else
    {
        search_child = TRUE;
    }

    while(start)
    {
        CString seed = start->app_name;
        folder_info *root_folder = NULL;
        widget_info *root_widget = search_start;

        if (!root_widget)
        {
            root_folder = GetProjectView()->GetRootFolder();
        }

        if (FindAppName(root_folder, start, search_child) || FindAppName(root_widget, start, search_child) ||
            FindAppName(search_also, start, search_child))
        {
            int underscore = seed.ReverseFind('_');

            if (underscore > 0 && underscore > (seed.GetLength() - 3))
            {
                if (seed.GetAt(underscore + 1) >= '0' &&
                    seed.GetAt(underscore + 1) <= '9')
                {
                    seed = seed.Left(underscore);
                }
            }

            if (root_folder || root_widget)
            {
                start->app_name = seed;

                if (FindAppName(root_folder, start, search_child) || FindAppName(root_widget, start, search_child) ||
                    FindAppName(search_also, start, search_child))
                {
                    int index = 1;
                    out.Format(_T("%s_%d"), seed, index);

                    start->app_name = out;
                    while (FindAppName(root_folder, start, search_child) || FindAppName(root_widget, start, search_child) ||
                           FindAppName(search_also, start, search_child))
                    {
                        index++;
                        out.Format(_T("%s_%d"), seed, index);
                        start->app_name = out;
                    }
                }
            }
        }

        if (start->GetChildWidgetInfo())
        {
            if (is_top_level_widget)
            {
                root_widget = start->GetChildWidgetInfo();
                new_search_also = start->GetChildWidgetInfo();
            }
            CreateUniqueAppNames(start->GetChildWidgetInfo(), new_search_also, root_widget, FALSE);
        }
        start = start->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::DecrementWidgetResourceIds(int res_type, folder_info *folder, GX_RESOURCE_ID resource_id)
{
    CArray<GX_RESOURCE_ID> resource_id_array;

    resource_id_array.Add(resource_id);
    while (folder)
    {
        DecrementWidgetResourceIds(res_type, folder->GetFirstChildWidget(), resource_id_array);
        folder = folder->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::DecrementWidgetResourceIds(int res_type, folder_info* folder, CArray<GX_RESOURCE_ID> &resource_id_array)
{
    while (folder)
    {
        DecrementWidgetResourceIds(res_type, folder->GetFirstChildWidget(), resource_id_array);
        folder = folder->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::UpdateSpriteFrameInfos(widget_info* info, GX_RESOURCE_ID map_id)
{
    sprite_service_provider* provider;
    BOOL updated = FALSE;

    while (info)
    {
        if(info->basetype == GX_TYPE_SPRITE)
        {
            if (info->pixelmap_id[NORMAL_PIXELMAP_INDEX] == map_id)
            {
                provider = (sprite_service_provider *)GetServiceProvider(info->basetype);
                if (provider && info)
                {
                    provider->UpdateSpriteFrameInfo(info);
                    updated = TRUE;
                }
            }
        }

        if (info->GetChildWidgetInfo())
        {
            UpdateSpriteFrameInfos(info->GetChildWidgetInfo(), map_id);
        }

        if (updated)
        {
            widget_info* screen_info = GetProjectView()->FindTopLevelWidget(info);

            if (screen_info && screen_info->is_template)
            {
                template_service_provider::RebuildDerivedWidgets(screen_info);
            }
        }
        info = info->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::UpdateSpriteFrameInfos(folder_info *folder, GX_RESOURCE_ID map_id)
{
    while (folder)
    {
        UpdateSpriteFrameInfos(folder->GetFirstChildWidget(), map_id);
        folder = folder->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
int widget_factory::CalculateResourceIdDecrementCount(CArray<GX_RESOURCE_ID> &resource_id_array, GX_RESOURCE_ID resource_id)
{
    int count = 0;
    GX_RESOURCE_ID deleted_id;

    for (int index = 0; index < resource_id_array.GetCount(); index++)
    {
        deleted_id = resource_id_array.GetAt(index);

        if (resource_id == deleted_id)
        {
            return -1;
        }
        else if (resource_id > deleted_id)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::DecrementWidgetResourceIds(int res_type, widget_info *info, CArray<GX_RESOURCE_ID> &resource_id_array)
{
    int index;
    int count;
    GX_RESOURCE_ID *id_list;
    widget_service_provider *provider;
    BOOL updated = FALSE;

    while(info)
    {
        switch(res_type)  
        {
        case RES_TYPE_STRING:
            if (info->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
            {
                count = info->ewi.string_scroll_wheel.base.total_rows;
                id_list = info->ewi.string_scroll_wheel.string_id_list;
            }
            else
            {
                count = NUM_WIDGET_STRINGS;
                id_list = info->string_id;
            }

            if (id_list)
            {
                provider = GetServiceProvider(info->basetype);
                if (provider)
                {
                    for (index = 0; index < count; index++)
                    {
                        if (id_list[index] == resource_id_array.GetAt(0))
                        {
                            provider->AssignText(info, index, 0);

                            updated = TRUE;
                        }
                        else
                        {
                            if (id_list[index] > resource_id_array.GetAt(0))
                            {
                                provider->AssignText(info, index, id_list[index] - 1);

                                updated = TRUE;
                            }
                        }
                    }
                }
            }
            break;

        case RES_TYPE_COLOR:
            for (index = 0; index < NUM_WIDGET_COLORS; index++)
            {
                if (info->color_id[index] == resource_id_array.GetAt(0))
                {
                    provider = GetServiceProvider(info->basetype);
                    if (provider && info)
                    {
                        provider->AssignColor(info, index, 0);

                        updated = TRUE;
                    }
                }
                else
                {
                    if (info->color_id[index] > resource_id_array.GetAt(0))
                    {
                        provider = GetServiceProvider(info->basetype);
                        if (provider && info)
                        {
                            provider->AssignColor(info, index, info->color_id[index] - 1);

                            updated = TRUE;
                        }
                    }
                }
            }
            break;

        case RES_TYPE_PIXELMAP:
            for (index = 0; index < NUM_WIDGET_PIXELMAPS; index++)
            {
                if (!info->pixelmap_id[index])
                {
                    continue;
                }

                count = CalculateResourceIdDecrementCount(resource_id_array, info->pixelmap_id[index]);
                if (count == -1)
                {
                    provider = GetServiceProvider(info->basetype);
                    if (provider && info)
                    {
                        provider->AssignPixelmap(info, index, 0);

                        updated = TRUE;
                    }
                }
                else
                {
                    if (count > 0)
                    {
                        provider = GetServiceProvider(info->basetype);
                        if (provider && info)
                        {
                            provider->AssignPixelmap(info, index, info->pixelmap_id[index] - count);

                            updated = TRUE;
                        }
                    }
                }
            }

            if (info->basetype == GX_TYPE_SPRITE)
            {
                CheckReplaceSpritePixelmaps(info, resource_id_array);
            }
            break;

        case RES_TYPE_FONT:
            for (index = 0; index < NUM_WIDGET_FONTS; index++)
            {
                if (info->font_id[index] == resource_id_array.GetAt(0))
                {
                    provider = GetServiceProvider(info->basetype);
                    if (provider && info)
                    {
                        provider->AssignFont(info, index, 0);

                        updated = TRUE;
                    }
                }
                else
                {
                    if (info->font_id[index] > resource_id_array.GetAt(0))
                    {
                        provider = GetServiceProvider(info->basetype);
                        if (provider && info)
                        {
                            provider->AssignFont(info, index, info->font_id[index] - 1);

                            updated = TRUE;
                        }
                    }
                }
            }
            break;
        }
        if (info->GetChildWidgetInfo())
        {
            DecrementWidgetResourceIds(res_type, info->GetChildWidgetInfo(), resource_id_array);
        }

        if (updated)
        {
            widget_info *screen_info = GetProjectView()->FindTopLevelWidget(info);

            if (screen_info && screen_info->is_template)
            {
                template_service_provider::RebuildDerivedWidgets(screen_info);
            }
        }
        info = info->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::StringDeleted(studiox_project *project, GX_RESOURCE_ID string_id)
{
    int display = GetProjectView()->GetActiveDisplay();

    folder_info *info = project->mDisplays[display].GetFirstChildFolder();

    if (info)
    {
        DecrementWidgetResourceIds(RES_TYPE_STRING, info, string_id);
    }

}

///////////////////////////////////////////////////////////////////////////////
int widget_factory::CountReferences(int res_type, widget_info *info, GX_RESOURCE_ID resource_id, int count, bool skip_template)
{
    int index;
    GX_RESOURCE_ID *string_list;
    int string_count;

    while(info)
    {

        if ((!skip_template) || (info->basetype != GX_TYPE_TEMPLATE))
        {
            switch (res_type)
            {
            case RES_TYPE_STRING:
                if (info->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
                {
                    string_list = info->ewi.string_scroll_wheel.string_id_list;
                    string_count = info->ewi.string_scroll_wheel.base.total_rows;
                }
                else
                {
                    string_list = info->string_id;
                    string_count = NUM_WIDGET_STRINGS;
                }
                if (string_list)
                {
                    for (index = 0; index < string_count; index++)
                    {
                        if (string_list[index] == resource_id)
                        {
                            count++;
                        }
                    }
                }
                break;

            case RES_TYPE_COLOR:
                for (index = 0; index < NUM_WIDGET_COLORS; index++)
                {
                    if (info->color_id[index] == resource_id)
                    {
                        count++;
                    }
                }
                break;

            case RES_TYPE_PIXELMAP:
                for (index = 0; index < NUM_WIDGET_PIXELMAPS; index++)
                {
                    if (info->pixelmap_id[index] == resource_id)
                    {
                        count++;
                    }
                }
                break;

            case RES_TYPE_FONT:
                for (index = 0; index < 2; index++)
                {
                    if (info->font_id[index] == resource_id)
                    {
                        count++;
                    }
                }
                break;
            }
        }

        if (info->GetChildWidgetInfo())
        {
            count = CountReferences(res_type, info->GetChildWidgetInfo(), resource_id, count);
        }
        info = info->GetNextWidgetInfo();
    }
    return count;
}

///////////////////////////////////////////////////////////////////////////////
int widget_factory::CountReferences(studiox_project *project, int res_type, GX_RESOURCE_ID resource_id)
{
    int count = 0;

    for (int Display = 0; Display < MAX_DISPLAYS; Display++)
    {
        folder_info *info = project->mDisplays[Display].GetFirstChildFolder();

        while (info)
        {
            count = CountReferences(res_type, info->GetFirstChildWidget(), resource_id, count);
            info = info->GetNextFolder();
        }
    }
    return count;
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::UpdateInputWidgetText(widget_info *info, GX_RESOURCE_ID resource_id)
{
    while (info)
    {
        switch (info->basetype)
        {
        case GX_TYPE_MULTI_LINE_TEXT_INPUT:
        case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
            if ((info->string_id[0] == resource_id) || (resource_id == -1))
            {
                widget_service_provider * provider = widget_factory::GetServiceProvider(info->basetype);

                if (provider && info->widget)
                {
                    provider->AssignText(info, 0, info->string_id[0]);
                }
            }
            break;
        }

        if (info->GetChildWidgetInfo())
        {
            UpdateInputWidgetText(info->GetChildWidgetInfo(), resource_id);
        }

        info = info->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::UpdateInputWidgetText(studiox_project *project, GX_RESOURCE_ID resource_id)
{
    for (int Display = 0; Display < MAX_DISPLAYS; Display++)
    {
        folder_info *info = project->mDisplays[Display].GetFirstChildFolder();

        while (info)
        {
            UpdateInputWidgetText(info->GetFirstChildWidget(), resource_id);
            info = info->GetNextFolder();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::ReplaceScrollbar(widget_info *info, GX_SCROLLBAR_APPEARANCE *appearance)
{
    info->ewi.scroll = *appearance;

    if (info->widget)
    {
        GX_WIDGET *new_scroll = NULL;
        GX_WIDGET *old_scroll = info->widget;
        GX_WINDOW *parent = (GX_WINDOW *)old_scroll->gx_widget_parent;
        gx_widget_detach(old_scroll);
        gx_widget_delete(old_scroll);

        widget_service_provider *provider = widget_factory::GetServiceProvider(info->basetype);
        new_scroll = provider->GenerateFromInfo((GX_WIDGET *)parent, info);
        info->widget = new_scroll;

        // GetTargetScreen()->WidgetWasReplaced(old_scroll, new_scroll);
        delete old_scroll;
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_factory::CheckReplaceSpritePixelmaps(widget_info *info, CArray<GX_RESOURCE_ID> &resource_id_array)
{
    if ((info->basetype != GX_TYPE_SPRITE) || (info->pixelmap_id[0] != 0))
    {
        return;
    }
     
    GX_SPRITE_FRAME *frame = info->ewi.sprite.framelist;
    int index;
    int decrement_count;

    for (index = 0; index < info->ewi.sprite.frame_count; index++)
    {
        decrement_count = CalculateResourceIdDecrementCount(resource_id_array, frame->gx_sprite_frame_pixelmap);

        if (decrement_count < 0)
        {
            frame ->gx_sprite_frame_pixelmap = 0;
        }
        else
        {
            if (decrement_count > 0)
            {
                frame->gx_sprite_frame_pixelmap -= decrement_count;
            }
        }
        frame++;
    }

    widget_service_provider *provider;
    provider = GetServiceProvider(info->basetype);
    provider->AssignPixelmap(info, 0, 0);
}