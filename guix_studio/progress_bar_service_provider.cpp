
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

progress_bar_service_provider::progress_bar_service_provider()
{
}

CString progress_bar_service_provider::GetVarDeclaration()
{
    return CString("GX_PROGRESS_BAR_MEMBERS_DECLARE");
}

CString progress_bar_service_provider::DeclarePropertiesStruct()
{
    CString out("");
    return out;
}

CString progress_bar_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    if (project_lib_version() > 50401)
    {
        out.Format(_T("GX_PROGRESS_BAR_INFO %s_properties =\n")
            _T("{\n")
            _T("    %d, /* mimimun value */\n")
            _T("    %d, /* maximum value */\n")
            _T("    %d, /* current value */\n")
            _T("    %s, /* font_id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s, /* disabled text color*/\n")
            _T("    %s  /* fill pixelmap */\n};\n"),
            propname,
            info->ewi.progress.min_val,
            info->ewi.progress.max_val,
            info->ewi.progress.current_val,
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX]));
    }
    else
    {
        out.Format(_T("GX_PROGRESS_BAR_INFO %s_properties =\n")
            _T("{\n")
            _T("    %d, /* mimimun value */\n")
            _T("    %d, /* maximum value */\n")
            _T("    %d, /* current value */\n")
            _T("    %s, /* font_id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s  /* fill pixelmap */\n};\n"),
            propname,
            info->ewi.progress.min_val,
            info->ewi.progress.max_val,
            info->ewi.progress.current_val,
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX]));
    }
    return out;
}

CString progress_bar_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_progress_bar_create");
}

CString progress_bar_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("progress_bar", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_PROGRESS_BAR *bar = (GX_PROGRESS_BAR *) control_block;\n"
        "    GX_PROGRESS_BAR_INFO *bar_info = (GX_PROGRESS_BAR_INFO *) info->properties;\n"   
        "    status = gx_progress_bar_create(bar,\n"
        "                    info->widget_name,\n"
        "                    parent,\n"
        "                    bar_info,\n"
        "                    info->style,\n"
        "                    info->widget_id,\n"
        "                    &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;

}

widget_info *progress_bar_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    GX_PROGRESS_BAR_INFO props;

    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_PROGRESS_BAR *bar = new GX_PROGRESS_BAR;
    props.gx_progress_bar_info_current_val = 50;
    props.gx_progress_bar_info_min_val = 0;
    props.gx_progress_bar_info_max_val = 100;
    props.gx_progress_bar_font_id = GX_FONT_ID_DEFAULT;
    props.gx_progress_bar_normal_text_color = GX_COLOR_ID_TEXT;
    props.gx_progress_bar_selected_text_color = GX_COLOR_ID_SELECTED_TEXT;
    props.gx_progress_bar_disabled_text_color = GX_COLOR_ID_DISABLED_TEXT;
    props.gx_progress_bar_fill_pixelmap = GX_PIXELMAP_NULL;

    gx_progress_bar_create(bar,
                    "progress_bar",
                    parent, &props,
                    GX_STYLE_ENABLED | GX_STYLE_BORDER_RAISED | GX_STYLE_PROGRESS_PERCENT | GX_STYLE_PROGRESS_TEXT_DRAW,
                    0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) bar);

    info->ewi.progress.current_val = props.gx_progress_bar_info_current_val;
    info->ewi.progress.min_val = props.gx_progress_bar_info_min_val;
    info->ewi.progress.max_val = props.gx_progress_bar_info_max_val;
    info->font_id[0] =  props.gx_progress_bar_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = props.gx_progress_bar_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = props.gx_progress_bar_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = props.gx_progress_bar_disabled_text_color;
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = props.gx_progress_bar_fill_pixelmap;
    return info;
}

GX_WIDGET *progress_bar_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_PROGRESS_BAR *bar = new GX_PROGRESS_BAR;
    GX_PROGRESS_BAR_INFO props;

    props.gx_progress_bar_info_current_val = info->ewi.progress.current_val;
    props.gx_progress_bar_info_min_val = info->ewi.progress.min_val;
    props.gx_progress_bar_info_max_val = info->ewi.progress.max_val;
    props.gx_progress_bar_font_id = info->font_id[0];
    props.gx_progress_bar_normal_text_color = info->color_id[NORMAL_TEXT_COLOR_INDEX];
    props.gx_progress_bar_selected_text_color = info->color_id[SELECTED_TEXT_COLOR_INDEX];
    props.gx_progress_bar_disabled_text_color = info->color_id[DISABLED_TEXT_COLOR_INDEX];
    props.gx_progress_bar_fill_pixelmap = info->pixelmap_id[NORMAL_PIXELMAP_INDEX];

    gx_progress_bar_create(bar,
        (CHAR *) info->app_name.GetString(),
        parent, &props,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set((GX_WIDGET *) bar,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return ((GX_WIDGET *) bar);
}

void progress_bar_service_provider::AssignFont(widget_info *info, int index,
    GX_RESOURCE_ID font_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_PROGRESS_BAR *bar = (GX_PROGRESS_BAR *)widget;
        gx_progress_bar_font_set(bar, font_id);
    }

    info->font_id[0] = font_id;
}

void progress_bar_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_PROGRESS_BAR *bar = (GX_PROGRESS_BAR *) (info->widget);
    GX_PROGRESS_BAR_INFO *bar_info;

    if (bar)
    {
        bar_info = &bar->gx_progress_bar_info;
    }

    switch(index)
    {
    case NORMAL_TEXT_COLOR_INDEX:
        if (bar)
        {
            gx_progress_bar_text_color_set(bar, color_id, bar_info->gx_progress_bar_selected_text_color,
                bar_info->gx_progress_bar_disabled_text_color);
        }

        info->color_id[NORMAL_TEXT_COLOR_INDEX] = color_id;
        break;
        
    case SELECTED_TEXT_COLOR_INDEX:
        if (bar)
        {
            gx_progress_bar_text_color_set(bar, bar_info->gx_progress_bar_normal_text_color, color_id,
                bar_info->gx_progress_bar_disabled_text_color);
        }

        info->color_id[SELECTED_TEXT_COLOR_INDEX] = color_id;
        break;

    case DISABLED_TEXT_COLOR_INDEX:
        if (bar)
        {
            gx_progress_bar_text_color_set(bar, bar_info->gx_progress_bar_normal_text_color,
                bar_info->gx_progress_bar_selected_text_color, color_id);
        }

        info->color_id[DISABLED_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        widget_service_provider::AssignColor(info, index, color_id);
    }
}

void progress_bar_service_provider::AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id)
{
    GX_WIDGET *widget = info->widget;

    if (index == NORMAL_PIXELMAP_INDEX)
    {
        info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = pixmap_id;

        if (widget)
        {
            GX_PROGRESS_BAR *pb = (GX_PROGRESS_BAR *)widget;
            gx_progress_bar_pixelmap_set(pb, pixmap_id);
        }
    }
}


void progress_bar_service_provider::Autosize(widget_info *info)
{
    GX_PROGRESS_BAR *bar = (GX_PROGRESS_BAR *) info->widget;

    if (bar)
    {
        if (info->pixelmap_id[0])
        {
            if (bar->gx_widget_style & GX_STYLE_PROGRESS_VERTICAL)
            {
                CheckResizeToPixelmap(info, RESIZE_MODE_WIDTH);
            }
            else
            {
                CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT);
            }
        }
    }
}
        
void progress_bar_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("minval", info->ewi.progress.min_val);
    writer.WriteInt("maxval", info->ewi.progress.max_val);
    writer.WriteInt("current_val", info->ewi.progress.current_val);
    WriteFontId(writer, project, display, "font_id", info->font_id[0]);
    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    WritePixelmapId(writer, project, display, "fill_pixelmap", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
}

void progress_bar_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_PROGRESS_PERCENT|GX_STYLE_PROGRESS_TEXT_DRAW|GX_STYLE_PROGRESS_VERTICAL|GX_STYLE_PROGRESS_SEGMENTED_FILL;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadInt("minval", info->ewi.progress.min_val);
    reader.ReadInt("maxval", info->ewi.progress.max_val);
    reader.ReadInt("current_val", info->ewi.progress.current_val);
    info->font_id[0] = ReadFontId(reader, project, display, "font_id");
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_text_color");
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_text_color");

    if (project->mHeader.project_version > 54)
    {
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_text_color");
    }
    else
    {
        //selected text color is start been used after guix version 5.4.0
        //set selected text color to normal text color incase broken old projects
        info->color_id[SELECTED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
    }
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "fill_pixelmap");
}
