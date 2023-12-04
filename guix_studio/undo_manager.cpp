
#include "studiox_includes.h"
#include "undo_manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
undo_manager::undo_manager()
{
    current = NULL;

    tail = undo_list;
    tail += MAX_UNDO_ENTRIES - 1;
    entries = 0;

    is_add_entry_locked = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
undo_manager::~undo_manager()
{
    Reset();
}

///////////////////////////////////////////////////////////////////////////////
void undo_manager::AddEntry(int type, folder_info *folder, int display_index)
{
    if (!folder || IsAddEntryLocked())
    {
        return;
    }

    if (current)
    {
        current++;
        if (current > tail)
        {
            current = undo_list;
        }
        if (entries < MAX_UNDO_ENTRIES)
        {
            entries++;
        }
    }
    else
    {
        current = undo_list;
        entries = 1;
    }

    ClearUndoRecord(current);
    current->type = type;

    undo_widget new_entry;

    switch (type)
    {
    case UNDO_TYPE_DELETE_FOLDER:
    case UNDO_TYPE_INSERT_TOP_LEVEL_WIDGETS:
        new_entry.folder = new folder_info(*folder);
        break;

    case UNDO_TYPE_INSERT_FOLDER:
        new_entry.folder_name = folder->folder_name;
        break;
    }

    new_entry.res_index = display_index;
    current->widget_list.Add(new_entry);
}

///////////////////////////////////////////////////////////////////////////////
void undo_manager::AddEntry(int type, widget_info *info, int res_index, BOOL fold)
{
    if (!info || !info->widget || IsAddEntryLocked())
    {
        return;
    }

    if (current)
    {
        // in a few cases we don't need to make another entry
        //   repeast position/size changes can be folded into one
        //   repeated name changes are folded into one
        //   repeated style changes are foled into one
        switch(type)
        {
        case UNDO_TYPE_SIZE:
        case UNDO_TYPE_POSITION:
        
            if (current->type == type && fold)
            {
                if (current->widget_list[0].project_info == info)
                {
                    // no need to make a new entry
                    return;
                }
            }
            break;

        case UNDO_TYPE_STYLE:
        case UNDO_TYPE_NAMES:
        case UNDO_TYPE_USER_DATA:
            if (current->type == type &&
                current->widget_list[0].project_info == info)
            {
                // same widget getting name or style change, don't save new record
                return;
            }
            break;

        default:
            break;
        }            
        current++;
        if (current > tail)
        {
            current = undo_list;
        }
        if (entries < MAX_UNDO_ENTRIES)
        {
            entries++;
        }
    }
    else
    {
        current = undo_list;
        entries = 1;
    }

    ClearUndoRecord(current);
    current->type = type;

    undo_widget new_entry;

    if (type == UNDO_TYPE_DELETE_WIDGET)
    {
        // copy the entire info tree, including children
        new_entry.copy_info = new widget_info(*info);
        new_entry.copy_info->widget = NULL;

        // When a widget is deleted, the corresponding widget_info is also deleted.
        // So if we un-delete the widget, we need to fixup any undo_manager entries
        // that are pointing at the old widget_info structures:
        UpdateUndoRecordInfoPointers(new_entry.copy_info, info);
    }
    else
    {
        // copy just this one record
        new_entry.copy_info = new widget_info(info->basetype);
        *new_entry.copy_info = *info;
        new_entry.copy_info->widget = NULL;
    }

    if (info)
    {
        // save pointer to original widget_info
        new_entry.project_info = info;
    }
    else
    {
        ErrorMsg("Internal Error: Invalid project information.");
    }

    studiox_project *project = GetOpenProject();

    widget_info *parent_info = project->FindParentInfo(info);
    if (parent_info) 
    {
        new_entry.parent_info = parent_info;
    }
    else
    {
        new_entry.parent_info = NULL;

        if (project)
        {
            folder_info *folder = project->FindParentFolderInfo(info);

            if (folder)
            {
                //new_entry.folder = folder;
                new_entry.folder_name = folder->folder_name;
            }
        }
    }
    new_entry.res_index = res_index;
    current->widget_list.Add(new_entry);
}

///////////////////////////////////////////////////////////////////////////////
void undo_manager::AddEntry(int type, CArray<WIDGET_SELECT_ENTRY> &widget_list, BOOL fold)
{
    int widget_count;
    int widget_index;
    BOOL same_set = TRUE;

    if (IsAddEntryLocked())
    {
        return;
    }

    if (current)
    {
        // see if we just need to update most recent undo entry
        
        if (current->type == type && fold &&
            (type == UNDO_TYPE_POSITION || type == UNDO_TYPE_SIZE))
        {
            widget_count = current->widget_list.GetSize();
            if (widget_count != widget_list.GetCount())
            {
                same_set = FALSE;
            }
            else
            {
                for (widget_index = 0; widget_index < widget_count; widget_index++)
                {
                    if (current->widget_list[widget_index].project_info !=
                        widget_list[widget_index].info)
                    {
                        same_set = FALSE;
                        break;
                    }
                }
            }
             
            if (same_set)
            {
                // same set of widgets being positioned, so no need to make new copy
                // of the widget info
                return;
            }       
        }

        current++;
        if (current > tail)
        {
            current = undo_list;
        }
        if (entries < MAX_UNDO_ENTRIES)
        {
            entries++;
        }
    }
    else
    {
        current = undo_list;
        entries = 1;
    }

    ClearUndoRecord(current);
    current->type = type;

    widget_count = widget_list.GetCount();
    current->widget_list.SetSize(widget_count);

    for (widget_index = 0; widget_index < widget_count; widget_index++)
    {
        widget_info *info = widget_list[widget_index].info;
        
        current->widget_list[widget_index].project_info = info;
        current->widget_list[widget_index].copy_info = new widget_info(info->basetype);
        *current->widget_list[widget_index].copy_info = *info;
        current->widget_list[widget_index].copy_info->widget = NULL;
        current->widget_list[widget_index].parent_info = GetOpenProject()->FindParentInfo(info);
        current->widget_list[widget_index].res_index = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
/* Count how many nodes are in a widget_info tree.
   This is done as part of UpdateUndoEntryInfoPointers()
*/

int undo_manager::CountInfoNodes(widget_info *start, int count)
{
    while(start)
    {
        count++;
        if (start->GetChildWidgetInfo())
        {
            count += CountInfoNodes(start->GetChildWidgetInfo(), count);
        }
        start = start->GetNextWidgetInfo();
    }
    return count;
}

///////////////////////////////////////////////////////////////////////////////
/* Write an array of old_info and new_info pointers.
   This is done as part of UpdateUndoEntryInfoPointers()
*/
widget_info **undo_manager::BuildInfoNodeArray(widget_info **write, widget_info *new_tree, widget_info *old_tree)
{
    while(new_tree)
    {
        *write++ = old_tree;
        *write++ = new_tree;

        if (new_tree->GetChildWidgetInfo())
        {
            write = BuildInfoNodeArray(write, new_tree->GetChildWidgetInfo(), old_tree->GetChildWidgetInfo());
        }
        new_tree = new_tree->GetNextWidgetInfo();
        old_tree = old_tree->GetNextWidgetInfo();
    }
    return write;
}

///////////////////////////////////////////////////////////////////////////////
/* Called by UpdateUndoEntryInfoPointers.
   For each undo entry, go through the list of project_info pointers. If any
   of them are in the node_list array, replace them with the new pointers.
*/
void undo_manager::SearchAndReplaceInfoPointers(undo_record *search, widget_info **node_list, int node_list_size)
{
    int widget_index;
    int list_index;

    int rows = search->widget_list.GetCount();
    widget_info *old_pointer;

    for (widget_index = 0; widget_index < rows; widget_index++)
    {
        old_pointer = search->widget_list[widget_index].project_info;

        for (list_index = 0; list_index < node_list_size * 2; list_index += 2)
        {
            if (node_list[list_index] == old_pointer)
            {
                search->widget_list[widget_index].project_info = node_list[list_index + 1];
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/* If we Delete a widget, then undo the Delete, it's possible that the undo
   list will hold records pointing at the old widget_info structure tree. So if
   we un-delete a widget (tree), we need to update the undo list records to
   convert the old project_info pointers to the new project_info pointers.
*/
void undo_manager::UpdateUndoRecordInfoPointers(widget_info *new_info, widget_info *old_info)
{
    undo_record *search = current;
    int rows = entries;

    int nodes = CountInfoNodes(new_info, 0);
    widget_info **info_list = new widget_info*[nodes * 2];
    BuildInfoNodeArray(info_list, new_info, old_info);

    while(rows--)
    {
        SearchAndReplaceInfoPointers(search, info_list, nodes);
        search--;
        if (search < undo_list)
        {
            search = tail;
        }
    }
    delete [] info_list;
}

///////////////////////////////////////////////////////////////////////////////
void undo_manager::Undo()
{
    int widget_list_size;
    int widget_list_index;
    int resource_index;
    int display;
    widget_service_provider *provider;
    widget_info *current_info;
    widget_info *parent_info;
    GX_WIDGET *target;

    studiox_project *pProject = GetOpenProject();

    if (entries <= 0 || pProject == NULL)
    {
        return;
    }

    LockAddEntry();

    undo_record *entry = current;
    current--;
    if (current < undo_list)
    {
        current = tail;
    }
    entries--;

    if (!entries)
    {
        current = NULL;
    }

    widget_list_size = entry->widget_list.GetSize();

    if (widget_list_size)
    {
        if (entry->type == UNDO_TYPE_DELETE_WIDGET)
        {
            parent_info = entry->widget_list[0].parent_info;
        }
        else
        {
            parent_info = entry->widget_list[0].project_info;
        }
    }

    if (parent_info)
    {
         GetTargetScreen()->MakeTopWindowVisible(parent_info);
    }

    switch(entry->type)
    {
    case UNDO_TYPE_POSITION:
    case UNDO_TYPE_SIZE:
        for (widget_list_index = 0; widget_list_index < widget_list_size; widget_list_index++)
        {
            widget_factory::MoveWidget(entry->widget_list[widget_list_index].project_info,
                                       entry->widget_list[widget_list_index].copy_info->size);
            
            current_info = entry->widget_list[widget_list_index].project_info;
            if (current_info)
            {
                GetTargetScreen()->WidgetWasMoved(current_info);
            }
        }
        GetPropsWin()->WidgetWasMoved();
        break;

    case UNDO_TYPE_STYLE:
        current_info = entry->widget_list[0].project_info;
        target = current_info->widget;

        if (current_info)
        {
            current_info->style = entry->widget_list[0].copy_info->style;
            if (target)
            {
                gx_widget_style_set(target, current_info->style);
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_TEMPLATE:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->is_template = entry->widget_list[0].copy_info->is_template;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_SLIDER_INFO:
        current_info = entry->widget_list[0].project_info;
        target = current_info->widget;

        if (current_info)
        {
            current_info->ewi = entry->widget_list[0].copy_info->ewi;
            current_info->misc_value = entry->widget_list[0].copy_info->misc_value;

            if (target)
            {
                GX_SLIDER *slider = (GX_SLIDER *) target;
                if (current_info->basetype == GX_TYPE_SLIDER)
                {
                    slider->gx_slider_tick_count = current_info->misc_value;
                }
                gx_slider_info_set(slider, &current_info->ewi.slider);
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_PROGRESS_BAR_INFO:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi = entry->widget_list[0].copy_info->ewi;
            properties_win::AssignProgressBarInfo(current_info);
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_LIST_ROWS:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            switch(current_info->basetype)
            {
            case GX_TYPE_VERTICAL_LIST:
            case GX_TYPE_HORIZONTAL_LIST:
                current_info->ewi.vlist.total_rows = entry->widget_list[0].copy_info->ewi.vlist.total_rows;
                break;

            case GX_TYPE_DROP_LIST:
                current_info->ewi.drop_list.total_rows = entry->widget_list[0].copy_info->ewi.drop_list.total_rows;
                break;

            default:
                break;
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_OPEN_HEIGHT:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi.drop_list.open_height = entry->widget_list[0].copy_info->ewi.drop_list.open_height;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_DYNAMIC_TEXT_BUFFER:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi.text_info.dynamic_buffer = entry->widget_list[0].copy_info->ewi.text_info.dynamic_buffer;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_TEXT_VIEW_WHITESPACE:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi.text_info.whitespace = entry->widget_list[0].copy_info->ewi.text_info.whitespace;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_TEXT_VIEW_LINE_SPACE:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi.text_info.line_space = entry->widget_list[0].copy_info->ewi.text_info.line_space;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_TEXT_BUFFER_SIZE:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi.text_info.buffer_size = entry->widget_list[0].copy_info->ewi.text_info.buffer_size;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_SCROLL_APPEARANCE:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            GX_SCROLLBAR_APPEARANCE appearance = entry->widget_list[0].copy_info->ewi.scroll;
            widget_factory::ReplaceScrollbar(current_info, &appearance);
            GetPropsWin()->WidgetWasModified(current_info);
         }
         break;

    case UNDO_TYPE_SCROLL_STYLE:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            GX_SCROLLBAR_APPEARANCE appearance = current_info->ewi.scroll;
            current_info->style = entry->widget_list[0].copy_info->style;
            widget_factory::ReplaceScrollbar(current_info, &appearance);
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_INSERT_WIDGET:
        if (widget_list_size > 1)
        {
            GetTargetScreen()->DeSelect();
        }

        for (widget_list_index = 0; widget_list_index < widget_list_size; widget_list_index++)
        {
            current_info = entry->widget_list[widget_list_index].project_info;
            if (current_info)
            {
                GetProjectView()->DeleteWidget(current_info);
            }
            else
            {
                ErrorMsg("Internal Error: Invalid Undo information.");
            }
        }
        break;

    case UNDO_TYPE_DELETE_WIDGET:
        {
        // re-insert the deleted widget
        // copy the widget info structures
        parent_info = entry->widget_list[0].parent_info;

        // select the parent widget so that we don't get
        // an out-of-sync verification error:
        GetTargetScreen()->SelectWidget(parent_info, TRUE, FALSE);

        widget_info *pTemp = entry->widget_list[0].copy_info;
        entry->widget_list[0].copy_info = NULL;

        widget_info *search_start;
        BOOL is_top_level_widget;

        if (parent_info == NULL)
        {
            search_start = NULL;
            is_top_level_widget = TRUE;
        }
        else
        {
            search_start = GetProjectView()->FindTopLevelWidget(parent_info);
            if (!search_start)
            {
                break;
            }
            search_start = search_start->GetChildWidgetInfo();
            is_top_level_widget = FALSE;
        }

        widget_factory::CreateUniqueAppNames(pTemp, pTemp, search_start, is_top_level_widget);          // handy name passed to create

        // now make the widget visible:
        if (parent_info)
        {
            pProject->AddWidgetToParent(parent_info, pTemp);
        }
        else
        {
            //selected parent folder in case paste wrong place.
            GetProjectView()->FolderSelected(entry->widget_list[0].res_index, entry->widget_list[0].folder_name);

            folder_info *parent_folder = pProject->FindFolderInfo(entry->widget_list[0].res_index, entry->widget_list[0].folder_name);
            pProject->AddWidgetToFolder(parent_folder, pTemp);
        }

        if (!pTemp->id_name.IsEmpty())
        {
            display = pProject->GetDisplayIndex(pTemp);
            pProject->AddToIdDictionary(display, ID_TYPE_WIDGET, pTemp->id_name);
        }
        GetProjectView()->CheckParentRefresh(parent_info);
        }
        break;

    case UNDO_TYPE_DELETE_FOLDER:
        {
            folder_info *folder = entry->widget_list[0].folder;
            entry->widget_list[0].folder = NULL;

            project_view *pview = GetProjectView();

            if (pview)
            {
                pview->SelectDisplay(entry->widget_list[0].res_index);
                pProject->AddFolderToDisplay(entry->widget_list[0].res_index, folder);
                //widget_factory::GenerateWidgets(NULL, folder->GetFirstChildWidget());
                pview->SelectDisplay(entry->widget_list[0].res_index);

                if (folder->GetFirstChildWidget())
                {
                    pview->WidgetSelected(folder->GetFirstChildWidget());
                }
            }
        }
        break;

    case UNDO_TYPE_INSERT_FOLDER:
        {
             project_view *pview = GetProjectView();
             if (pview)
             {
                 undo_widget *test = &(entry->widget_list[0]);
                 folder_info *folder = pProject->FindFolderInfo(entry->widget_list[0].res_index, entry->widget_list[0].folder_name);

                 if (folder)
                 {
                     pview->DeleteFolder(folder);
                 }
             }
        }
        break;

    case UNDO_TYPE_INSERT_TOP_LEVEL_WIDGETS:
        {
            project_view *pview = GetProjectView();

            if (pview)
            {
                folder_info *folder = entry->widget_list[0].folder;
                CArray<widget_info *> widget_list;
                folder_info *parent_folder = pProject->FindFolderInfo(entry->widget_list[0].res_index, folder->folder_name);
                widget_info *info = folder->GetFirstChildWidget();

                pview->FolderSelected(parent_folder);

                while (info)
                {
                    widget_list.Add(info);
                    info = info->GetNextWidgetInfo();
                }

                for (int count = widget_list.GetCount() - 1; count >= 0; count--)
                {
                    info = widget_list.GetAt(count);
                    info = pProject->FindWidgetInfo(parent_folder, info->app_name, FALSE);
                    pview->DeleteWidget(info);
                }
            }
        }
        break;

    case UNDO_TYPE_COLOR:
        current_info = entry->widget_list[0].project_info;
        resource_index = entry->widget_list[0].res_index;
        provider = widget_factory::GetServiceProvider(current_info->basetype);

        if (provider && current_info)
        {
            provider->AssignColor(current_info,
                resource_index,
                entry->widget_list[0].copy_info->color_id[resource_index]);
        }
        GetPropsWin()->WidgetWasModified(current_info);
        break;           

    case UNDO_TYPE_FONT:
        current_info = entry->widget_list[0].project_info;
        resource_index = entry->widget_list[0].res_index;
        provider = widget_factory::GetServiceProvider(current_info->basetype);

        if (provider && current_info)
        {
            provider->AssignFont(current_info,
                resource_index, entry->widget_list[0].copy_info->font_id[resource_index]);
        }
        GetPropsWin()->WidgetWasModified(current_info);
        break;

    case UNDO_TYPE_PIXELMAP:
        current_info = entry->widget_list[0].project_info;
        resource_index = entry->widget_list[0].res_index;
        provider = widget_factory::GetServiceProvider(current_info->basetype);

        if (provider && current_info)
        {
            provider->AssignPixelmap(current_info,
                resource_index,
                entry->widget_list[0].copy_info->pixelmap_id[resource_index]);
        }
        GetPropsWin()->WidgetWasModified(current_info);
        break;

    case UNDO_TYPE_STRING:
        current_info = entry->widget_list[0].project_info;
        resource_index = entry->widget_list[0].res_index;
        display = GetProjectView()->GetActiveDisplay();
       
        provider = widget_factory::GetServiceProvider(current_info->basetype);

        if (display >= 0 && current_info && provider)
        {
            provider->AssignText(current_info, 0,
                entry->widget_list[0].copy_info->string_id[resource_index]);
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_NAMES:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            current_info->app_name = entry->widget_list[0].copy_info->app_name;
            current_info->callback_func = entry->widget_list[0].copy_info->callback_func;

            if (current_info->id_name != entry->widget_list[0].copy_info->id_name)
            {
                display = pProject->GetDisplayIndex(current_info);

                if (!current_info->id_name.IsEmpty() &&
                    pProject->mDisplays[display].screenflow)
                {
                    pProject->mDisplays[display].screenflow->UpdateIdName(current_info, current_info->id_name, entry->widget_list[0].copy_info->id_name);
                }
                current_info->id_name = entry->widget_list[0].copy_info->id_name;
            }
            current_info->event_func = entry->widget_list[0].copy_info->event_func;
            current_info->draw_func = entry->widget_list[0].copy_info->draw_func;
            GetPropsWin()->WidgetWasModified(current_info);
        }        
        break;

    case UNDO_TYPE_USER_DATA:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            current_info->user_data = entry->widget_list[0].copy_info->user_data;
            GetPropsWin()->WidgetWasModified(current_info);
        }        
        break;

    case UNDO_TYPE_FOCUS:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->accepts_focus = entry->widget_list[0].copy_info->accepts_focus;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;      

    case UNDO_TYPE_CIRCULAR_GAUGE_INFO:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->ewi.gauge.info = entry->widget_list[0].copy_info->ewi.gauge.info;
            current_info->ewi.gauge.start_angle = entry->widget_list[0].copy_info->ewi.gauge.start_angle;

            if (current_info->widget)
            {
                GX_CIRCULAR_GAUGE *gauge = (GX_CIRCULAR_GAUGE *) current_info->widget;
                gauge->gx_circular_gauge_info = current_info->ewi.gauge.info;
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_CHART_INFO:
        current_info = entry->widget_list[0].project_info;
        
        if (current_info)
        {
            current_info->ewi.line_chart_info = entry->widget_list[0].copy_info->ewi.line_chart_info;

            if (current_info->widget)
            {
                GX_LINE_CHART *chart = (GX_LINE_CHART *) current_info->widget;
                chart->gx_line_chart_info = current_info->ewi.line_chart_info;
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_NUMERIC_PROMPT_INFO:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            provider = widget_factory::GetServiceProvider(current_info->basetype);
            
            ((prompt_service_provider *)provider)->AssignNumericValue(current_info,
                entry->widget_list[0].copy_info->ewi.numeric_prompt_value);

            current_info->ewi.numeric_prompt_value = entry->widget_list[0].copy_info->ewi.numeric_prompt_value;
            current_info->format_func = entry->widget_list[0].copy_info->format_func;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_SCROLL_WHEEL_INFO:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            scroll_wheel_service_provider *provider = (scroll_wheel_service_provider *)widget_factory::GetServiceProvider(current_info->basetype);
            if (provider)
            {
                provider->AssignScrollWheelInfo(current_info, &entry->widget_list[0].copy_info->ewi.scroll_wheel);
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_TEXT_SCROLL_WHEEL_INFO:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            int normal_font = entry->widget_list[0].copy_info->font_id[NORMAL_FONT_INDEX];
            int selected_font = entry->widget_list[0].copy_info->font_id[SELECTED_FONT_INDEX];
            current_info->font_id[NORMAL_FONT_INDEX] = normal_font;
            current_info->font_id[SELECTED_FONT_INDEX] = selected_font;

            text_scroll_wheel_service_provider *provider = (text_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(current_info->basetype);

            if (provider)
            {
                provider->AssignFont(current_info, NORMAL_FONT_INDEX, normal_font);
                provider->AssignFont(current_info, SELECTED_FONT_INDEX, selected_font);
            }

            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_STRING_SCROLL_WHEEL_INFO:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            string_scroll_wheel_service_provider *provider = (string_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(current_info->basetype);

            if (provider)
            {
                widget_info *entry_info = entry->widget_list[0].copy_info;
                current_info->callback_func = entry_info->callback_func;

                CArray<GX_RESOURCE_ID> list_array;
                provider->InitStringIdListArray(entry_info->ewi.string_scroll_wheel.string_id_list, entry_info->ewi.string_scroll_wheel.base.total_rows, list_array);
                if (current_info->ewi.string_scroll_wheel.string_id_list)
                {
                    delete current_info->ewi.string_scroll_wheel.string_id_list;
                    current_info->ewi.string_scroll_wheel.string_id_list = NULL;
                }
                provider->CreateStringIdList(current_info, list_array);
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_NUMRIC_SCROLL_WHEEL_INFO:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            numeric_scroll_wheel_info *wheel_info = &entry->widget_list[0].copy_info->ewi.numeric_scroll_wheel;
            current_info->ewi.numeric_scroll_wheel = *wheel_info;

            numeric_scroll_wheel_service_provider *provider = (numeric_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(current_info->basetype);
            target = current_info->widget;

            if (provider)
            {
                provider->AssignScrollWheelInfo(current_info, &wheel_info->base);
                provider->AssignValueRange(current_info, wheel_info->start_val, wheel_info->end_val);
                GetPropsWin()->WidgetWasModified(current_info);
            }
        }
        break;

    case UNDO_TYPE_MENU_INFO:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            widget_info *copy = entry->widget_list[0].copy_info;
            target = current_info->widget;
            provider = widget_factory::GetServiceProvider(current_info->basetype);

            ((menu_service_provider *)provider)->AssignTextOffset(current_info,
                copy->ewi.menu.text_x_offset, copy->ewi.menu.text_y_offset);
            ((menu_service_provider *)provider)->AssignStyle(current_info, copy->style);
            current_info->ewi.menu.insert_as_menu_item = copy->ewi.menu.insert_as_menu_item;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_TREE_VIEW_INFO:
        current_info = entry->widget_list[0].project_info;
        target = current_info->widget;

        if (current_info)
        {
            current_info->ewi.tree_view_indentation = entry->widget_list[0].copy_info->ewi.tree_view_indentation;
            if (target)
            {
                gx_tree_view_indentation_set((GX_TREE_VIEW *)target, current_info->ewi.tree_view_indentation);
            }
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_VISIBLE_AT_STARTUP:
        current_info = entry->widget_list[0].project_info;
        if (current_info)
        {
            current_info->visible_at_startup = entry->widget_list[0].copy_info->visible_at_startup;
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    case UNDO_TYPE_RADIAL_SLIDER_INFO:
        current_info = entry->widget_list[0].project_info;

        if (current_info)
        {
            provider = widget_factory::GetServiceProvider(current_info->basetype);
            ((radial_slider_service_provider *)provider)->SetRadialSliderInfo(current_info, &entry->widget_list[0].copy_info->ewi.radial_slider);
            GetPropsWin()->WidgetWasModified(current_info);
        }
        break;

    default:
        break;
    }
    ClearUndoRecord(entry);
    FreeAddEntry();
}

///////////////////////////////////////////////////////////////////////////////
void undo_manager::ClearUndoRecord(undo_record *record)
{
    // delete the copied widget_info structures associate with this undo record:
    int widget_list_size = record->widget_list.GetSize();
    int widget_list_index;

    for (widget_list_index = 0; widget_list_index < widget_list_size; widget_list_index++)
    {
        switch (record->type)
        {
        case UNDO_TYPE_INSERT_FOLDER:
        case UNDO_TYPE_INSERT_TOP_LEVEL_WIDGETS:
        case UNDO_TYPE_DELETE_FOLDER:
            if (record->widget_list[widget_list_index].folder)
            {
                delete record->widget_list[widget_list_index].folder;
                record->widget_list[widget_list_index].folder = NULL;
            }
            break;

        default:
            if (record->widget_list[widget_list_index].copy_info)
            {
                delete record->widget_list[widget_list_index].copy_info;
                record->widget_list[widget_list_index].copy_info = NULL;
            }
            break;
        }
    }

    // now empty out the undo record widget list array
    record->widget_list.RemoveAll();
    record->type = 0;
}

///////////////////////////////////////////////////////////////////////////////
void undo_manager::Reset()
{
    while(entries)
    {
        undo_record *entry = current;
        current--;
        if (current < undo_list)
        {
            current = tail;
        }
        ClearUndoRecord(entry);
        entries--;
    }
    current = NULL;
}

