
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
tree_view_service_provider::tree_view_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
CString tree_view_service_provider::GetVarDeclaration()
{
    return CString("GX_TREE_VIEW_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString tree_view_service_provider::DeclarePropertiesStruct()
{
    CString out(""
        "typedef struct\n"
        "{\n"
        "    GX_RESOURCE_ID expand_map_id;\n"
        "    GX_RESOURCE_ID collapse_map_id;\n"
        "    GX_RESOURCE_ID root_line_color_id;\n"
        "    GX_VALUE       indentation;\n"
        "} GX_TREE_VIEW_PROPERTIES;\n\n");
    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString tree_view_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    out.Format(_T("GX_TREE_VIEW_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s, /* expand map id */\n")
        _T("    %s, /* collapse map id */\n")
        _T("    %s, /* root line color id */\n")
        _T("    %d  /* indentation */\n};\n"),
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX]),
        gen->GetColorIdName(info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX]),
        info->ewi.tree_view_indentation);

    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString tree_view_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_tree_view_create");
}

///////////////////////////////////////////////////////////////////////////////
CString tree_view_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("tree_view", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_TREE_VIEW *tree = (GX_TREE_VIEW *) control_block;\n"
        "    GX_TREE_VIEW_PROPERTIES *props = (GX_TREE_VIEW_PROPERTIES *) info->properties;\n"
        "    status = gx_tree_view_create(tree, info->widget_name, parent,\n"
        "                                 info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        if (props->expand_map_id || props->collapse_map_id)\n"
        "        {\n"
        "            gx_tree_view_root_pixelmap_set(tree, props->expand_map_id, props->collapse_map_id);\n"
        "        }\n"
        "        gx_tree_view_indentation_set(tree, props->indentation);\n"
        "        gx_tree_view_root_line_color_set(tree, props->root_line_color_id);\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *tree_view_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    int parent_size;

    GX_RECTANGLE size = parent->gx_widget_size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    parent_size = (parent->gx_widget_size.gx_rectangle_right -
        parent->gx_widget_size.gx_rectangle_left) / 2;
    size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

    parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
        parent->gx_widget_size.gx_rectangle_top) / 2;
    size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_TREE_VIEW *tree = new GX_TREE_VIEW;
    gx_tree_view_create(tree, "tree_view", parent,
        GX_STYLE_BORDER_THIN | GX_STYLE_ENABLED | GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES, 0, &size);
    gx_widget_style_remove(tree, GX_STYLE_DRAW_SELECTED);
    widget_info *info = InitWidgetInfo((GX_WIDGET *)tree);
    info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX] = 0;
    info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX] = tree->gx_tree_view_root_line_color;
    info->ewi.tree_view_indentation = tree->gx_tree_view_indentation;
    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *tree_view_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_TREE_VIEW *tree = new GX_TREE_VIEW;

    //create a tree view widget
    gx_tree_view_create(tree,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->style,
        0,
        &info->size);

    //set fill color
    gx_widget_fill_color_set((GX_WIDGET *)tree,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    if (info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX] ||
        info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX])
    {
        //set tree view root pixelmap
        gx_tree_view_root_pixelmap_set(tree, info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX],
            info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX]);
    }

    //set tree view indentation
    gx_tree_view_indentation_set(tree, info->ewi.tree_view_indentation);
    gx_tree_view_root_line_color_set(tree, info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX]);

    return (GX_WIDGET*)tree;
}

///////////////////////////////////////////////////////////////////////////////
void tree_view_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    window_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "expand_map_id", info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "collapse_map_id", info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX]);
    WriteColorId(writer, project, display, "root_line_color_id", info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX]);
    writer.WriteInt("indentation", info->ewi.tree_view_indentation);
}

///////////////////////////////////////////////////////////////////////////////
void tree_view_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES;
    window_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "expand_map_id");
    info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "collapse_map_id");
    info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX] = ReadColorId(reader, project, display, "root_line_color_id");
    reader.ReadInt("indentation", info->ewi.tree_view_indentation);
}

///////////////////////////////////////////////////////////////////////////////
void tree_view_service_provider::AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id)
{
    GX_TREE_VIEW *tree = (GX_TREE_VIEW *) (info->widget);

    switch (index)
    {
    case TREE_VIEW_EXPAND_PIXELMAP_INDEX:
        info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX] = pixmap_id;

        if (tree)
        {
            gx_tree_view_root_pixelmap_set(tree, pixmap_id, tree->gx_tree_view_collapse_pixelmap_id);
        }
        break;

    case TREE_VIEW_COLLAPSE_PIXELMAP_INDEX:
        info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX] = pixmap_id;

        if (tree)
        {
            gx_tree_view_root_pixelmap_set(tree, tree->gx_tree_view_expand_pixelmap_id, pixmap_id);
        }
        break;

    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void tree_view_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_WIDGET *widget = info->widget;

    switch (index)
    {
    case TREE_VIEW_ROOT_LINE_COLOR_INDEX:
        if (widget)
        {
            gx_tree_view_root_line_color_set((GX_TREE_VIEW *)widget, color_id);
        }

        info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX] = color_id;
        break;

    default:
        widget_service_provider::AssignColor(info, index, color_id);
    }
}

