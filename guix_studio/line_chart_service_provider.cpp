
#include "studiox_includes.h"

extern "C" {
#include "gx_line_chart.h"
#include "gx_utility.h"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

line_chart_service_provider::line_chart_service_provider()
{
}

CString line_chart_service_provider::GetVarDeclaration()
{
    return CString("GX_LINE_CHART_MEMBERS_DECLARE");
}

CString line_chart_service_provider::DeclarePropertiesStruct()
{
    CString out("");
/*
    "typedef struct\n"
    "{\n"
    "    INT min_value;\n"
    "    INT max_value;\n"
    "    GX_VALUE left_margin;\n"
    "    GX_VALUE top_margin;\n"
    "    GX_VALUE right_margin;\n"
    "    GX_VALUE bottom_margin;\n"
    "    GX_VALUE max_data_count;\n"
    "    GX_VALUE axis_line_width;\n"
    "    GX_VALUE data_line_width;\n"
    "    GX_RESOURCE_ID axis_color_id;\n"
    "    GX_RESOURCE_ID line_color_id;\n"
    "} GX_LINE_CHART_PROPERTIES;\n\n");
*/
    return out;
}

CString line_chart_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString props;
    CString out;
    CString propname = prefix + info->app_name;

    out = "";

    props.Format(_T("GX_LINE_CHART_INFO %s_properties =\n")
        _T("{\n")
        _T("    %d, /* min data value */\n")
        _T("    %d, /* max data value */\n")
        _T("    GX_NULL, /* data pointer */\n")
        _T("    %d, /* left margin */\n")
        _T("    %d, /* top margin */\n") 
        _T("    %d, /* right margin */\n") 
        _T("    %d, /* bottom margin */\n") 
        _T("    %d, /* max data items */\n")
        _T("    0,  /* active data items */\n")
        _T("    %d, /* axis line width */\n")
        _T("    %d, /* data line width */\n")
        _T("    %s, /* axis line color */\n")
        _T("    %s  /* data line color */\n};\n"),

        propname,
        info->ewi.line_chart_info.gx_line_chart_min_val,
        info->ewi.line_chart_info.gx_line_chart_max_val,
        info->ewi.line_chart_info.gx_line_chart_left_margin,
        info->ewi.line_chart_info.gx_line_chart_top_margin,
        info->ewi.line_chart_info.gx_line_chart_right_margin,
        info->ewi.line_chart_info.gx_line_chart_bottom_margin,
        info->ewi.line_chart_info.gx_line_chart_max_data_count,
        info->ewi.line_chart_info.gx_line_chart_axis_line_width,
        info->ewi.line_chart_info.gx_line_chart_data_line_width,
        gen->GetColorIdName(info->ewi.line_chart_info.gx_line_chart_axis_color),
        gen->GetColorIdName(info->ewi.line_chart_info.gx_line_chart_line_color));
    
    out += props;
    return out;
}

CString line_chart_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_line_chart_create");
}

CString line_chart_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("line_chart", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_LINE_CHART *chart = (GX_LINE_CHART *) control_block;\n"
        "    GX_LINE_CHART_INFO *chart_info = (GX_LINE_CHART_INFO *) info->properties;\n\n"   
        "    status = gx_line_chart_create(chart, info->widget_name, parent, chart_info, info->style, info->widget_id, &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *line_chart_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    int parent_size;
    GX_LINE_CHART_INFO chart_info;

    GX_RECTANGLE size = parent->gx_widget_size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    parent_size = (parent->gx_widget_size.gx_rectangle_right -
                   parent->gx_widget_size.gx_rectangle_left) / 2;
    size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

    parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
                   parent->gx_widget_size.gx_rectangle_top) / 2;
    size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_LINE_CHART *chart = new GX_LINE_CHART;

    chart_info.gx_line_chart_min_val = 0;
    chart_info.gx_line_chart_max_val = 100;
    chart_info.gx_line_chart_data = NULL;
    chart_info.gx_line_chart_left_margin = 0;
    chart_info.gx_line_chart_top_margin = 0;
    chart_info.gx_line_chart_right_margin = 0;
    chart_info.gx_line_chart_bottom_margin = 0;
    chart_info.gx_line_chart_max_data_count = 100;
    chart_info.gx_line_chart_active_data_count = 0;
    chart_info.gx_line_chart_axis_line_width = 3;
    chart_info.gx_line_chart_data_line_width = 2;
    chart_info.gx_line_chart_axis_color = GX_COLOR_ID_DEFAULT_BORDER;
    chart_info.gx_line_chart_line_color = GX_COLOR_ID_SHINE;
    
    _gx_line_chart_create(chart, "line_chart", parent, &chart_info,
        GX_STYLE_BORDER_THIN, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) chart);

    info->ewi.line_chart_info = chart_info;
    return info;
}

GX_WIDGET *line_chart_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_LINE_CHART *chart = new GX_LINE_CHART;
    
    gx_line_chart_create(chart,
        (CHAR *) info->app_name.GetString(),
        parent,
        &info->ewi.line_chart_info,
        info->style, 0,
        &info->size);

    gx_widget_fill_color_set(chart,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return ((GX_WIDGET *) chart);
}
        
void line_chart_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    GX_LINE_CHART_INFO *chart_info = &info->ewi.line_chart_info;
    window_service_provider::SaveToProject(writer, project, display, info);
    
    writer.WriteInt("min_val", chart_info->gx_line_chart_min_val);
    writer.WriteInt("max_val", chart_info->gx_line_chart_max_val);
    writer.WriteInt("left_margin", chart_info->gx_line_chart_left_margin);
    writer.WriteInt("top_margin", chart_info->gx_line_chart_top_margin);
    writer.WriteInt("right_margin", chart_info->gx_line_chart_right_margin);
    writer.WriteInt("bottom_margin", chart_info->gx_line_chart_bottom_margin);
    writer.WriteInt("max_count", chart_info->gx_line_chart_max_data_count);
    writer.WriteInt("axis_line_width", chart_info->gx_line_chart_axis_line_width);
    writer.WriteInt("data_line_width", chart_info->gx_line_chart_data_line_width);
    WriteColorId(writer, project, display, "axis_color", chart_info->gx_line_chart_axis_color);
    WriteColorId(writer, project, display, "data_line_color", chart_info->gx_line_chart_line_color);
}

void line_chart_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    GX_LINE_CHART_INFO *chart_info = &info->ewi.line_chart_info;

    window_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadInt("min_val", chart_info->gx_line_chart_min_val);
    reader.ReadInt("max_val", chart_info->gx_line_chart_max_val);
    reader.ReadValue("left_margin", chart_info->gx_line_chart_left_margin);
    reader.ReadValue("top_margin", chart_info->gx_line_chart_top_margin);
    reader.ReadValue("right_margin", chart_info->gx_line_chart_right_margin);
    reader.ReadValue("bottom_margin", chart_info->gx_line_chart_bottom_margin);
    reader.ReadValue("max_count", chart_info->gx_line_chart_max_data_count);
    reader.ReadValue("axis_line_width", chart_info->gx_line_chart_axis_line_width);
    reader.ReadValue("data_line_width", chart_info->gx_line_chart_data_line_width);
    chart_info->gx_line_chart_axis_color = ReadColorId(reader, project, display, "axis_color");
    chart_info->gx_line_chart_line_color = ReadColorId(reader, project, display, "data_line_color");
}
        
