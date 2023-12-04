
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

menu_service_provider::menu_service_provider()
{
}

CString menu_service_provider::GetVarDeclaration()
{
    return CString("GX_MENU_MEMBERS_DECLARE");
}

CString menu_service_provider::DeclarePropertiesStruct()
{
    CString out;
    
    out.Format(_T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("    GX_VALUE       text_x_offset;\n")
        _T("    GX_VALUE       text_y_offset;\n")
        _T("    GX_VALUE       list_total_count;\n")
        _T("} GX_MENU_PROPERTIES;\n\n"),
        pixelmap_prompt_service_provider::DeclarePropertiesStructContent());
    return out;
}

CString menu_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_MENU_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("    %d, /* text display x offset*/\n")
        _T("    %d, /* text display y offset*/\n")
        _T("    %d, /* list_total_count*/\n")
        _T("};\n"),
        propname,
        pixelmap_prompt_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
        info->ewi.menu.text_x_offset,
        info->ewi.menu.text_y_offset,
        info->ewi.menu.list_total_count);
    return out;
}

CString menu_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_menu_create");
}

CString menu_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("menu", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_MENU *menu = (GX_MENU *) control_block;\n"
        "    GX_MENU_PROPERTIES *props = (GX_MENU_PROPERTIES *) info->properties;\n"
        "    status = gx_menu_create(menu, info->widget_name, parent,\n"
        "               props->string_id,\n"
        "               props->fill_map_id,\n"
        "               info->style, info->widget_id, &info->size);\n"
        "\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        menu->gx_menu_list_total_count = props->list_total_count;\n"
        "        gx_menu_text_offset_set(menu, props->text_x_offset, props->text_y_offset);\n"
        "        gx_pixelmap_prompt_pixelmap_set((GX_PIXELMAP_PROMPT *)menu,\n"
        "                                        props->left_map_id,\n"
        "                                        props->fill_map_id,\n"
        "                                        props->right_map_id,\n"
        "                                        props->selected_left_map_id,\n"
        "                                        props->selected_fill_map_id,\n"
        "                                        props->selected_right_map_id);\n"
        "        gx_prompt_font_set((GX_PROMPT *)menu, props->font_id);\n";

    if (version > 50401)
    {
        out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_prompt_text_color_set((GX_PROMPT *)menu, props->normal_text_color_id, props->selected_text_color_id);\n"
            "#else\n"
            "        gx_prompt_text_color_set((GX_PROMPT *)menu, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
            "#endif\n";
    }
    else
    {
        out += "        gx_prompt_text_color_set((GX_PROMPT *)menu, props->normal_text_color_id, props->selected_text_color_id);\n";
    }
    out +=  "    }\n"
            "    return status;\n"
            "}\n";

    return out;
}

widget_info *menu_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 99, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_MENU *menu = new GX_MENU;
    GX_RESOURCE_ID string_id = GetStringId("menu");

    gx_menu_create(menu, "menu", parent, string_id, 0,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_LEFT, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)menu);
    info->pixelmap_id[FILL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[LEFT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[RIGHT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX] = 0;
    info->string_id[0] = string_id;
    info->font_id[0] = menu->gx_prompt_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = menu->gx_prompt_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = menu->gx_prompt_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = menu->gx_prompt_disabled_text_color;
    info->ewi.menu.text_x_offset = menu->gx_menu_text_x_offset;
    info->ewi.menu.text_y_offset = menu->gx_menu_text_y_offset;
    info->ewi.menu.list_total_count = menu->gx_menu_list_total_count;

    // if insert mode is true, every time we insert a widget to the menu,
    // the inserted widget will be treat as a menu item
    info->ewi.menu.insert_as_menu_item = TRUE;

    return info;
}

GX_WIDGET *menu_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_MENU *menu = new GX_MENU;

    gx_menu_create(menu,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->pixelmap_id[FILL_PIXELMAP_INDEX],
        info->style, 0, &info->size);

    gx_menu_text_offset_set(menu, info->ewi.menu.text_x_offset, info->ewi.menu.text_y_offset);

    gx_pixelmap_prompt_pixelmap_set((GX_PIXELMAP_PROMPT *)menu,
        info->pixelmap_id[LEFT_PIXELMAP_INDEX],
        info->pixelmap_id[FILL_PIXELMAP_INDEX],
        info->pixelmap_id[RIGHT_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]);

    gx_widget_fill_color_set((GX_PROMPT *)menu,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    gx_prompt_text_color_set((GX_PROMPT *)menu,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_prompt_font_set((GX_PROMPT *)menu, info->font_id[0]);

    menu->gx_menu_list_total_count = info->ewi.menu.list_total_count;

    return (GX_WIDGET *)menu;
}

void menu_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    pixelmap_prompt_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("text_x_offset", info->ewi.menu.text_x_offset);
    writer.WriteInt("text_y_offset", info->ewi.menu.text_y_offset);
    writer.WriteInt("list_total_count", info->ewi.menu.list_total_count);
    writer.WriteBool("insert_as_menu_item", info->ewi.menu.insert_as_menu_item);
}

void menu_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_MENU_EXPANDED;
    pixelmap_prompt_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadInt("text_x_offset", info->ewi.menu.text_x_offset);
    reader.ReadInt("text_y_offset", info->ewi.menu.text_y_offset);
    reader.ReadInt("list_total_count", info->ewi.menu.list_total_count);
    reader.ReadBool("insert_as_menu_item", info->ewi.menu.insert_as_menu_item);
}

void menu_service_provider::AssignTextOffset(widget_info *info, int text_x_offset, int text_y_offset)
{
    gx_menu_text_offset_set((GX_MENU *)info->widget, text_x_offset, text_y_offset);
    info->ewi.menu.text_x_offset = text_x_offset;
    info->ewi.menu.text_y_offset = text_y_offset;
}

void menu_service_provider::AssignStyle(widget_info *info, int style)
{
    GX_WIDGET *parent = info->widget->gx_widget_parent;
    GX_WIDGET *top_level_container;
    GX_WIDGET *child;
    widget_info *child_info;
    studiox_project *project = GetOpenProject();
    project_view *project_view = GetProjectView();
    int old_style = info->style;
    BOOL update_size = FALSE;
    GX_RECTANGLE dirty;

    gx_widget_style_set(info->widget, style);
    info->style = style;

    top_level_container = menu_service_provider::TopLevelMenuContainerFind(info->widget);

    if (!top_level_container)
    {
        //the menu is not a member of accordion menu or tree view,just return
        return;
    }

    if ((old_style & GX_STYLE_MENU_EXPANDED) !=
        (style & GX_STYLE_MENU_EXPANDED))
    {
        if (project && parent && parent->gx_widget_type == GX_TYPE_ACCORDION_MENU)
        {
            if (style & GX_STYLE_MENU_EXPANDED)
            {
                // Remove expanded style from neighbor widgets.
                child = parent->gx_widget_first_child;

                while (child)
                {
                    if ((child != info->widget) &&
                        (child->gx_widget_type == GX_TYPE_MENU))
                    {
                        child_info = project->FindWidgetInfo(child);
                        gx_widget_style_remove(child, GX_STYLE_MENU_EXPANDED);
                        child_info->style = child->gx_widget_style;
                    }

                    child = child->gx_widget_next;
                }
            }
        }

        update_size = TRUE;
    }

    if (top_level_container != parent)
    {
        // If new style have GX_STYLE_MENU_EXPANDED, 
        // check if we need to add GX_STYLE_MENU_EXPANDED to the parent.
        if (parent->gx_widget_type != GX_TYPE_MENU_LIST)
        {
            parent = parent->gx_widget_parent;
        }

        if ((style & GX_STYLE_MENU_EXPANDED) &&
            (parent->gx_widget_type == GX_TYPE_MENU_LIST))
        {
            child = ((GX_MENU_LIST *)parent)->gx_menu_list_owner;

            child_info = project->FindWidgetInfo(child);
            AssignStyle(child_info, child->gx_widget_style | GX_STYLE_MENU_EXPANDED);
        }
    }

    if (update_size)
    {
        //Reposition accordion menu.
        switch (top_level_container->gx_widget_type)
        {
        case GX_TYPE_ACCORDION_MENU:
            gx_accordion_menu_position((GX_ACCORDION_MENU *)top_level_container);
            break;
        case GX_TYPE_TREE_VIEW:
            gx_tree_view_position((GX_TREE_VIEW *)top_level_container);
            dirty = top_level_container->gx_widget_size;
            dirty.gx_rectangle_top = info->size.gx_rectangle_top;
            gx_system_dirty_partial_add(top_level_container, &dirty);
            break;
        }

        if (project_view)
        {
            project_view->UpdateChildPositions(project->FindWidgetInfo(parent));
        }
    }
}

GX_WIDGET *menu_service_provider::TopLevelMenuContainerFind(GX_WIDGET *start)
{
    GX_WIDGET *parent;
    GX_WIDGET *found = GX_NULL;
    GX_WIDGET *temp;

    if ((start->gx_widget_type == GX_TYPE_ACCORDION_MENU) ||
        (start->gx_widget_type == GX_TYPE_TREE_VIEW))
    {
        found = start;
    }

    parent = start->gx_widget_parent;
    if (parent)
    {
        switch (parent->gx_widget_type)
        {
        case GX_TYPE_TREE_VIEW:
            found = parent;
            break;

        case GX_TYPE_ACCORDION_MENU:
            //the accordion menu is the child of another accordion menu
            temp = TopLevelMenuContainerFind(parent);
            if (temp)
            {
                found = temp;
            }
            break;

        case GX_TYPE_MENU_LIST:
            //the accordion menu is the child of a menu list
            parent = ((GX_MENU_LIST *)parent)->gx_menu_list_owner;
            parent = parent->gx_widget_parent;
            if (parent)
            {
                //the menu list owner is the child of another accordion menu
                temp = TopLevelMenuContainerFind(parent);
                if (temp)
                {
                    found = temp;
                }
            }
            break;
        }
    }

    return found;
}

void menu_service_provider::Autosize(widget_info *info)
{
    CheckResizeToText(info);
    CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT);
}

