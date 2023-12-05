
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

drop_list_service_provider::drop_list_service_provider()
{
}

CString drop_list_service_provider::GetVarDeclaration()
{
    return CString("GX_DROP_LIST_MEMBERS_DECLARE");
}

CString drop_list_service_provider::DeclarePropertiesStruct()
{
    CString out(""
   "typedef struct\n"
    "{\n"
    "    GX_RESOURCE_ID pixelmap_id;\n"
    "    GX_RESOURCE_ID wallpaper_id;\n"
    "    VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT);\n"
    "    int total_rows;\n"
    "    int open_height;\n"
    "} GX_DROP_LIST_PROPERTIES;\n\n");
    return out;
}

CString drop_list_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
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

    out.Format(_T("GX_DROP_LIST_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s, /* widget pixelmap id */\n")
        _T("    %s, /* popup list wallpaper pixelmap id */\n")
        _T("    %s, /* callback function */\n")
        _T("    %d, /* total rows */\n")
        _T("    %d  /* open height */\n};\n"),
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]),
        callback,
        info->ewi.drop_list.total_rows,
        info->ewi.drop_list.open_height);
    return out;
}


CString drop_list_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_drop_list_create");
}

CString drop_list_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("drop_list", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_DROP_LIST *list = (GX_DROP_LIST *) control_block;\n"
        "    GX_DROP_LIST_PROPERTIES *props = (GX_DROP_LIST_PROPERTIES *) info->properties;\n"   
        "    status = gx_drop_list_create(list, info->widget_name, parent,\n"
        "                                 props->total_rows, props->open_height,\n"
        "                                 props->callback, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        if (props->pixelmap_id)\n"
        "        {\n"
        "            gx_drop_list_pixelmap_set(list, props->pixelmap_id);\n"
        "        }\n"
        "        if (props->wallpaper_id)\n"
        "        {\n"
        "            gx_window_wallpaper_set((GX_WINDOW *)&list->gx_drop_list_popup.gx_popup_list_list, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);\n"
        "        }\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}


widget_info *drop_list_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;

    gx_utility_rectangle_define(&size, 0, 0, 100, 23);

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_DROP_LIST *list = new GX_DROP_LIST;
    gx_drop_list_create(list, "drop_list", parent,
        10, 100, NULL, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) list);

    // keep the list box size in the info struture, along with open height
    info->size = list -> gx_widget_size;
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX] = 0;
    info->callback_func = CString("callback_name");
    info->ewi.drop_list.total_rows  = 10;
    info->ewi.drop_list.open_height = 100;
    info->ewi.drop_list.seperation = 0;

    return info;
}

GX_WIDGET *drop_list_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_DROP_LIST *list = new GX_DROP_LIST;
    gx_drop_list_create(list,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->ewi.drop_list.total_rows, info->ewi.drop_list.open_height, NULL,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set(list,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    if (info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX] > 0)
    {
        gx_drop_list_pixelmap_set(list, info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX]);
    }

    if (info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] > 0)
    {
        BOOL tile = FALSE;

        if (info->style & GX_STYLE_TILE_WALLPAPER)
        {
            tile = TRUE;
        }
        gx_window_wallpaper_set((GX_WINDOW *)&list->gx_drop_list_popup.gx_popup_list_list, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX], tile);
    }

    return ((GX_WIDGET *) list);
}

void drop_list_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    window_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("seperation", info->ewi.drop_list.seperation);
    writer.WriteInt("total_rows", info->ewi.drop_list.total_rows);
    writer.WriteInt("open_height", info->ewi.drop_list.open_height);
    WritePixelmapId(writer, project, display, "pixelmap_id", info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX]);
    writer.WriteString("callback", info->callback_func);
}

void drop_list_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TILE_BACKGROUND|GX_STYLE_CENTER_SELECTED|GX_STYLE_WRAP|GX_STYLE_FLICKABLE|GX_STYLE_REPEAT_SELECT;

    window_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "pixelmap_id");
    reader.ReadInt("seperation", info->ewi.drop_list.seperation, 0);
    reader.ReadInt("total_rows", info->ewi.drop_list.total_rows, 10);
    reader.ReadInt("open_height", info->ewi.drop_list.open_height, 100);
    reader.ReadString("callback", info->callback_func);
}


void drop_list_service_provider::AssignPixelmap(widget_info *info,
    int index, GX_RESOURCE_ID pixmap_id)
{
    GX_DROP_LIST *list = (GX_DROP_LIST *) (info->widget);
    BOOL tile = GX_FALSE;

    switch (index)
    {
    case DROP_LIST_PIXELMAP_INDEX:
        info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX] = pixmap_id;

        if (list)
        {
            if (info->style & GX_STYLE_TILE_BACKGROUND)
            {
                gx_widget_style_add((GX_WIDGET *)list, GX_STYLE_TILE_BACKGROUND);
            }
            else
            {
                gx_widget_style_remove((GX_WIDGET *)list, GX_STYLE_TILE_BACKGROUND);
            }
            gx_drop_list_pixelmap_set(list, info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX]);
        }
        break;

    case WALLPAPER_PIXELMAP_INDEX:
        info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = pixmap_id;

        if (list)
        {
            if (info->style & GX_STYLE_TILE_WALLPAPER)
            {
                tile = GX_TRUE;
            }
            gx_window_wallpaper_set((GX_WINDOW *)&list->gx_drop_list_popup.gx_popup_list_list, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX], tile);
        }
        break;

    default:
        break;
    }
}
        

