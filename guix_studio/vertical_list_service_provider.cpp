
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vertical_list_service_provider::vertical_list_service_provider()
{
}

CString vertical_list_service_provider::GetVarDeclaration()
{
    return CString("GX_VERTICAL_LIST_MEMBERS_DECLARE");
}

CString vertical_list_service_provider::DeclarePropertiesStruct()
{
    CString out(""
   "typedef struct\n"
    "{\n"
    "    GX_RESOURCE_ID wallpaper_id;\n"
    "    VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT);\n"
    "    int total_rows;\n"
    "} GX_VERTICAL_LIST_PROPERTIES;\n\n");
    return out;
}

CString vertical_list_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    CString callback;

    if (info->callback_func.IsEmpty())
    {
        callback = "GX_NULL";
    }
    else
    {
        callback = info->callback_func;
    }
    out.Format(_T("GX_VERTICAL_LIST_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s, /* wallpaper id */\n")
        _T("    %s, /* callback function */\n")
        _T("    %d  /* total rows */\n};\n"),
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]),
        callback,
        info->ewi.vlist.total_rows);

    return out;
}


CString vertical_list_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_vertical_list_create");
}

CString vertical_list_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("vertical_list", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_VERTICAL_LIST *list = (GX_VERTICAL_LIST *) control_block;\n"
        "    GX_VERTICAL_LIST_PROPERTIES *props = (GX_VERTICAL_LIST_PROPERTIES *) info->properties;\n"   
        "    status = gx_vertical_list_create(list, info->widget_name, parent, props->total_rows,\n"
        "                                     props->callback, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        if (props->wallpaper_id)\n"
        "        {\n"
        "            gx_window_wallpaper_set((GX_WINDOW *) list, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);\n"
        "        }\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}


widget_info *vertical_list_service_provider::CreateNewInstance(GX_WIDGET *parent)
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
    
    GX_VERTICAL_LIST *list = new GX_VERTICAL_LIST;
    gx_vertical_list_create(list, "vertical_list", parent,
        10, NULL, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) list);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;
    info->callback_func = CString("callback_name");
    info->ewi.vlist.total_rows  = 10;

    return info;
}

GX_WIDGET *vertical_list_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_VERTICAL_LIST *list = new GX_VERTICAL_LIST;
    gx_vertical_list_create(list,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->ewi.vlist.total_rows, NULL,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set(list,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    if (info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] > 0)
    {
        BOOL tile = FALSE;

        if (info->style & GX_STYLE_TILE_WALLPAPER)
        {
            tile = TRUE;
        }
        gx_window_wallpaper_set((GX_WINDOW *) list, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX], tile);
    }
    return ((GX_WIDGET *) list);
}

void vertical_list_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    window_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("seperation", info->ewi.vlist.seperation);
    writer.WriteInt("total_rows", info->ewi.vlist.total_rows);
    writer.WriteString("callback", info->callback_func);
}

void vertical_list_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_CENTER_SELECTED|GX_STYLE_WRAP|GX_STYLE_FLICKABLE|GX_STYLE_REPEAT_SELECT;   

    window_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadInt("seperation", info->ewi.vlist.seperation, 0);
    reader.ReadInt("total_rows", info->ewi.vlist.total_rows, 10);
    reader.ReadString("callback", info->callback_func);
}
        

