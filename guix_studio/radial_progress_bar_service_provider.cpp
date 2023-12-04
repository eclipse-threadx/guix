
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

radial_progress_bar_service_provider::radial_progress_bar_service_provider()
{
}

CString radial_progress_bar_service_provider::GetVarDeclaration()
{
    return CString("GX_RADIAL_PROGRESS_BAR_MEMBERS_DECLARE");
}

CString radial_progress_bar_service_provider::DeclarePropertiesStruct()
{
    CString out("");
    return out;
}

CString radial_progress_bar_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    if (project_lib_version() > 50401)
    {
        out.Format(_T("GX_RADIAL_PROGRESS_BAR_INFO %s_properties =\n")
            _T("{\n")
            _T("    %d, /* xcenter */\n")
            _T("    %d, /* ycenter */\n")
            _T("    %d, /* radius */\n")
            _T("    %d, /* current val */\n")
            _T("    %d, /* anchor val*/\n")
            _T("    %s, /* font_id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s, /* disabled text color*/\n")
            _T("    %d, /* normal brush width */\n")
            _T("    %d, /* selected brush width */\n")
            _T("    %s, /* normal brush color */\n")
            _T("    %s, /* selected brush color */\n")
            _T("};\n"),
            propname,
            info->ewi.radial_progress.gx_radial_progress_bar_info_xcenter,
            info->ewi.radial_progress.gx_radial_progress_bar_info_ycenter,
            info->ewi.radial_progress.gx_radial_progress_bar_info_radius,
            info->ewi.radial_progress.gx_radial_progress_bar_info_current_val,
            info->ewi.radial_progress.gx_radial_progress_bar_info_anchor_val,
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
            info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_width,
            info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_width,
            gen->GetColorIdName(info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_color),
            gen->GetColorIdName(info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_color));
    }
    else
    {
        out.Format(_T("GX_RADIAL_PROGRESS_BAR_INFO %s_properties =\n")
            _T("{\n")
            _T("    %d, /* xcenter */\n")
            _T("    %d, /* ycenter */\n")
            _T("    %d, /* radius */\n")
            _T("    %d, /* current val */\n")
            _T("    %d, /* anchor val*/\n")
            _T("    %s, /* font_id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %d, /* normal brush width */\n")
            _T("    %d, /* selected brush width */\n")
            _T("    %s, /* normal brush color */\n")
            _T("    %s, /* selected brush color */\n")
            _T("};\n"),
            propname,
            info->ewi.radial_progress.gx_radial_progress_bar_info_xcenter,
            info->ewi.radial_progress.gx_radial_progress_bar_info_ycenter,
            info->ewi.radial_progress.gx_radial_progress_bar_info_radius,
            info->ewi.radial_progress.gx_radial_progress_bar_info_current_val,
            info->ewi.radial_progress.gx_radial_progress_bar_info_anchor_val,
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_width,
            info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_width,
            gen->GetColorIdName(info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_color),
            gen->GetColorIdName(info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_color));
    }

    return out;
}

CString radial_progress_bar_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_radial_progress_bar_create");
}

CString radial_progress_bar_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("radial_progress_bar", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_RADIAL_PROGRESS_BAR *bar = (GX_RADIAL_PROGRESS_BAR *) control_block;\n"
        "    GX_RADIAL_PROGRESS_BAR_INFO *bar_info = (GX_RADIAL_PROGRESS_BAR_INFO *) info->properties;\n"   
        "    status = gx_radial_progress_bar_create(bar,\n"
        "                    info->widget_name,\n"
        "                    parent,\n"
        "                    bar_info,\n"
        "                    info->style,\n"
        "                    info->widget_id);\n"
        "    return status;\n"
        "}\n";
    return out;

}

widget_info *radial_progress_bar_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RADIAL_PROGRESS_BAR_INFO props;
    int xcenter;
    int ycenter;

    xcenter = (parent->gx_widget_size.gx_rectangle_left + parent->gx_widget_size.gx_rectangle_right) >> 1;
    ycenter = (parent->gx_widget_size.gx_rectangle_top + parent->gx_widget_size.gx_rectangle_bottom) >> 1;

    GX_RADIAL_PROGRESS_BAR *bar = new GX_RADIAL_PROGRESS_BAR;
    props.gx_radial_progress_bar_info_xcenter = xcenter;
    props.gx_radial_progress_bar_info_ycenter = ycenter;
    props.gx_radial_progress_bar_info_radius = 100;
    props.gx_radial_progress_bar_info_anchor_val = 90;
    props.gx_radial_progress_bar_info_current_val = -180;
    props.gx_radial_progress_bar_info_font_id = GX_FONT_ID_DEFAULT;
    props.gx_radial_progress_bar_info_normal_text_color = GX_COLOR_ID_TEXT;
    props.gx_radial_progress_bar_info_selected_text_color = GX_COLOR_ID_SELECTED_TEXT;
    props.gx_radial_progress_bar_info_disabled_text_color = GX_COLOR_ID_DISABLED_TEXT;
    props.gx_radial_progress_bar_info_normal_brush_color = GX_COLOR_ID_SLIDER_NEEDLE_FILL;
    props.gx_radial_progress_bar_info_selected_brush_color = GX_COLOR_ID_SELECTED_FILL;
    props.gx_radial_progress_bar_info_normal_brush_width = 20;
    props.gx_radial_progress_bar_info_selected_brush_width = 20;

    gx_radial_progress_bar_create(bar,
                    "radial_progress_bar",
                    parent, &props,
                    GX_STYLE_ENABLED | GX_STYLE_BORDER_NONE | GX_STYLE_TRANSPARENT | GX_STYLE_PROGRESS_PERCENT |
                    GX_STYLE_PROGRESS_TEXT_DRAW | GX_STYLE_RADIAL_PROGRESS_ALIAS | GX_STYLE_RADIAL_PROGRESS_ROUND, 0);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) bar);

    info->ewi.radial_progress = props;
    info->font_id[0] =  props.gx_radial_progress_bar_info_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = props.gx_radial_progress_bar_info_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = props.gx_radial_progress_bar_info_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = props.gx_radial_progress_bar_info_disabled_text_color;

    return info;
}

GX_WIDGET *radial_progress_bar_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_RADIAL_PROGRESS_BAR *bar = new GX_RADIAL_PROGRESS_BAR;
    GX_RADIAL_PROGRESS_BAR_INFO *props = &info->ewi.radial_progress;

    props->gx_radial_progress_bar_info_xcenter = info->ewi.radial_progress.gx_radial_progress_bar_info_xcenter;
    props->gx_radial_progress_bar_info_ycenter = info->ewi.radial_progress.gx_radial_progress_bar_info_ycenter;
    props->gx_radial_progress_bar_info_radius = info->ewi.radial_progress.gx_radial_progress_bar_info_radius;
    props->gx_radial_progress_bar_info_anchor_val = info->ewi.radial_progress.gx_radial_progress_bar_info_anchor_val;
    props->gx_radial_progress_bar_info_current_val = info->ewi.radial_progress.gx_radial_progress_bar_info_current_val;
    props->gx_radial_progress_bar_info_normal_brush_width = info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_width;
    props->gx_radial_progress_bar_info_selected_brush_width = info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_width;
    props->gx_radial_progress_bar_info_normal_brush_color = info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_color;
    props->gx_radial_progress_bar_info_selected_brush_color = info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_color;
    props->gx_radial_progress_bar_info_font_id = info->font_id[0];
    props->gx_radial_progress_bar_info_normal_text_color = info->color_id[NORMAL_TEXT_COLOR_INDEX];
    props->gx_radial_progress_bar_info_selected_text_color = info->color_id[SELECTED_TEXT_COLOR_INDEX];
    props->gx_radial_progress_bar_info_disabled_text_color = info->color_id[DISABLED_TEXT_COLOR_INDEX];

    gx_radial_progress_bar_create(bar,
        (CHAR *) info->app_name.GetString(),
        parent, props,
        info->style,
        0);

    gx_widget_fill_color_set((GX_WIDGET *) bar,
                             info->color_id[NORMAL_FILL_COLOR_INDEX],
                             info->color_id[SELECTED_FILL_COLOR_INDEX], 
                             info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return ((GX_WIDGET *) bar);
}

void radial_progress_bar_service_provider::AssignFont(widget_info *info, int index, 
    GX_RESOURCE_ID font_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_RADIAL_PROGRESS_BAR *bar = (GX_RADIAL_PROGRESS_BAR *)widget;
        gx_radial_progress_bar_font_set(bar, font_id);
    }

    info->font_id[0] = font_id;
    info->ewi.radial_progress.gx_radial_progress_bar_info_font_id = font_id;
}

void radial_progress_bar_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_RADIAL_PROGRESS_BAR *bar = (GX_RADIAL_PROGRESS_BAR *) (info->widget);
    GX_RADIAL_PROGRESS_BAR_INFO *progress_info;

    if (bar)
    {
        progress_info = &bar->gx_radial_progress_bar_info;
    }

    switch(index)
    {
    case NORMAL_TEXT_COLOR_INDEX:
        if (bar)
        {
            gx_radial_progress_bar_text_color_set(bar, color_id, progress_info->gx_radial_progress_bar_info_selected_text_color,
                progress_info->gx_radial_progress_bar_info_disabled_text_color);
        }

        info->ewi.radial_progress.gx_radial_progress_bar_info_normal_text_color = color_id;
        info->color_id[NORMAL_TEXT_COLOR_INDEX] = color_id;
        break;
        
    case SELECTED_TEXT_COLOR_INDEX:
        if (bar)
        {
            gx_radial_progress_bar_text_color_set(bar, progress_info->gx_radial_progress_bar_info_normal_text_color, color_id,
                progress_info->gx_radial_progress_bar_info_disabled_text_color);
        }

        info->ewi.radial_progress.gx_radial_progress_bar_info_selected_text_color = color_id;
        info->color_id[SELECTED_TEXT_COLOR_INDEX] = color_id;
        break;

    case DISABLED_TEXT_COLOR_INDEX:
        if (bar)
        {
            gx_radial_progress_bar_text_color_set(bar, progress_info->gx_radial_progress_bar_info_normal_text_color,
                progress_info->gx_radial_progress_bar_info_selected_text_color, color_id);
        }

        info->ewi.radial_progress.gx_radial_progress_bar_info_disabled_text_color = color_id;
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        widget_service_provider::AssignColor(info, index, color_id);
    }
}

        
void radial_progress_bar_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("xcenter", info->ewi.radial_progress.gx_radial_progress_bar_info_xcenter);
    writer.WriteInt("ycenter", info->ewi.radial_progress.gx_radial_progress_bar_info_ycenter);
    writer.WriteInt("radius", info->ewi.radial_progress.gx_radial_progress_bar_info_radius);
    writer.WriteInt("anchor_val", info->ewi.radial_progress.gx_radial_progress_bar_info_anchor_val);
    writer.WriteInt("current_val", info->ewi.radial_progress.gx_radial_progress_bar_info_current_val);
    writer.WriteInt("normal_brush_width", info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_width);
    writer.WriteInt("selected_brush_width", info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_width);
    WriteColorId(writer, project, display, "normal_brush_color", info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_color);
    WriteColorId(writer, project, display, "selected_brush_color", info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_color);
    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    WriteFontId(writer, project, display, "font_id", info->font_id[0]);
}

void radial_progress_bar_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_PROGRESS_PERCENT|GX_STYLE_PROGRESS_TEXT_DRAW|GX_STYLE_PROGRESS_VERTICAL;
    valid_styles |= GX_STYLE_RADIAL_PROGRESS_ALIAS|GX_STYLE_RADIAL_PROGRESS_ROUND|GX_STYLE_RADIAL_PROGRESS_NO_BACKTRACK;

    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadValue("xcenter", info->ewi.radial_progress.gx_radial_progress_bar_info_xcenter);
    reader.ReadValue("ycenter", info->ewi.radial_progress.gx_radial_progress_bar_info_ycenter);
    reader.ReadValue("radius", info->ewi.radial_progress.gx_radial_progress_bar_info_radius);
    reader.ReadValue("anchor_val", info->ewi.radial_progress.gx_radial_progress_bar_info_anchor_val);
    reader.ReadValue("current_val", info->ewi.radial_progress.gx_radial_progress_bar_info_current_val);
    reader.ReadValue("normal_brush_width", info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_width);
    reader.ReadValue("selected_brush_width", info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_width);
    info->ewi.radial_progress.gx_radial_progress_bar_info_normal_brush_color = ReadColorId(reader, project, display, "normal_brush_color");
    info->ewi.radial_progress.gx_radial_progress_bar_info_selected_brush_color = ReadColorId(reader, project, display, "selected_brush_color");
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_text_color");
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_text_color");
    if (project->mHeader.project_version > 54)
    {
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_text_color");
    }
    else
    {
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
    }
    info->font_id[0] = ReadFontId(reader, project, display, "font_id");
}
