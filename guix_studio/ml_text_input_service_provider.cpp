
#include "studiox_includes.h"

extern "C" {
#include "gx_multi_line_text_view.h"
#include "gx_utility.h"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ml_text_input_service_provider::ml_text_input_service_provider()
{
}

CString ml_text_input_service_provider::GetVarDeclaration()
{
    return CString("GX_MULTI_LINE_TEXT_INPUT_MEMBERS_DECLARE");
}


CString ml_text_input_service_provider::DeclarePropertiesStruct()
{
CString out;

    if (project_lib_version() >= GX_WHITESPACE_IS_UBYTE)
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_RESOURCE_ID disabled_text_color_id;\n"
            "    GX_RESOURCE_ID readonly_fill_color_id;\n"
            "    GX_RESOURCE_ID readonly_text_color_id;\n"
            "    GX_UBYTE whitespace;\n"
            "    GX_BYTE  line_space;\n"
            "    GX_CHAR *buffer;\n"
            "    UINT buffer_size;\n"
            "} GX_MULTI_LINE_TEXT_INPUT_PROPERTIES;\n\n");
    }
    else if (project_lib_version() > 50401)
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_RESOURCE_ID disabled_text_color_id;\n"
            "    GX_RESOURCE_ID readonly_fill_color_id;\n"
            "    GX_RESOURCE_ID readonly_text_color_id;\n"
            "    GX_BYTE  whitespace;\n"
            "    GX_BYTE  line_space;\n"
            "    GX_CHAR *buffer;\n"
            "    UINT buffer_size;\n"
            "} GX_MULTI_LINE_TEXT_INPUT_PROPERTIES;\n\n");
    }
    else if (project_lib_version() >= 50303)
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_BYTE  whitespace;\n"
            "    GX_BYTE  line_space;\n"
            "    GX_CHAR *buffer;\n"
            "    UINT buffer_size;\n"
            "} GX_MULTI_LINE_TEXT_INPUT_PROPERTIES;\n\n");
    }
    else
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_CHAR *buffer;\n"
            "    UINT buffer_size;\n"
            "} GX_MULTI_LINE_TEXT_INPUT_PROPERTIES;\n\n");
    }

    return out;
}

CString ml_text_input_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString props;
    CString out;
    CString propname = prefix + info->app_name;
    CString buffer;

    if (!info->ewi.text_info.dynamic_buffer)
    {
        buffer.Format(_T("%s_buffer"), propname);

        out.Format(_T("GX_CHAR %s_buffer[%d];\n"),
            propname, info->ewi.text_info.buffer_size);
    }
    else
    {
        buffer = "GX_NULL";

        out = "";
    }

    if (project_lib_version() > 50401)
    {
        props.Format(_T("GX_MULTI_LINE_TEXT_INPUT_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s, /* disabled text color */\n")
            _T("    %s, /* readonly fill color */\n")
            _T("    %s, /* readonly text color */\n")
            _T("    %d, /* whitespace */\n")
            _T("    %d, /* line_space */\n")
            _T("    %s,  /* buffer */\n")
            _T("    %d /* buffer size */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[READONLY_FILL_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[READONLY_TEXT_COLOR_INDEX]),
            info->ewi.text_info.whitespace,
            info->ewi.text_info.line_space,
            buffer,
            info->ewi.text_info.buffer_size);
    }
    else if (project_lib_version() >= 50303)
    {
        props.Format(_T("GX_MULTI_LINE_TEXT_INPUT_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %d, /* whitespace */\n")
            _T("    %d, /* line_space */\n")
            _T("    %s,  /* buffer */\n")
            _T("    %d /* buffer size */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            info->ewi.text_info.whitespace,
            info->ewi.text_info.line_space,
            buffer,
            info->ewi.text_info.buffer_size);
    }
    else
    {
        props.Format(_T("GX_MULTI_LINE_TEXT_INPUT_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s,  /* buffer */\n")
            _T("    %d /* buffer size */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            buffer,
            info->ewi.text_info.buffer_size);
    }

    out += props;
    return out;
}

CString ml_text_input_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_multi_line_text_input_create");
}

CString ml_text_input_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("multi_line_text_input", version, out);
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return out;
    }

    if (version >= GX_VERSION_STRING_LENGTH_FIX)
    {
        CString string;
        string.Format(_T("{\n")
            _T("    UINT status;\n")
            _T("    GX_STRING text;\n")
            _T("    GX_MULTI_LINE_TEXT_INPUT *input = (GX_MULTI_LINE_TEXT_INPUT *) control_block;\n")
            _T("    GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *props = (GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n")
            _T("    status = gx_multi_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n")
            _T("    if (status == GX_SUCCESS)\n")
            _T("    {\n")
            _T("        gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->font_id);\n")
            _T("        gx_multi_line_text_input_fill_color_set(input, info->normal_fill_color_id, info->selected_fill_color_id,\n")
            _T("                                                info->disabled_fill_color_id, props->readonly_fill_color_id);\n")
            _T("        gx_multi_line_text_input_text_color_set(input, props->normal_text_color_id, props->selected_text_color_id,\n")
            _T("                                                props->disabled_text_color_id, props->readonly_text_color_id);\n")
            _T("        gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->whitespace);\n")
            _T("        gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->line_space);\n")
            _T("        if(props->buffer && (props->buffer_size > 0) && props->string_id)\n")
            _T("        {\n")
            _T("            gx_display_string_get_ext(%s_display_table[0].display, props->string_id, &text);\n")// FIXME: string set issue
            _T("            if(text.gx_string_ptr)\n")
            _T("            {\n")
            _T("                gx_multi_line_text_input_text_set_ext(input, &text);\n")
            _T("            }\n")
            _T("        }\n")
            _T("    }\n")
            _T("    return status;\n")
            _T("}\n"),
            project->mHeader.project_name);

        out += string;
    }
    else if (version > 50401)
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_CONST GX_CHAR *text;\n"
            "    GX_MULTI_LINE_TEXT_INPUT *input = (GX_MULTI_LINE_TEXT_INPUT *) control_block;\n"
            "    GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *props = (GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n"
            "    status = gx_multi_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->font_id);\n"
            "        gx_multi_line_text_input_fill_color_set(input, info->normal_fill_color_id, info->selected_fill_color_id,\n"
            "                                                info->disabled_fill_color_id, props->readonly_fill_color_id);\n"
            "        gx_multi_line_text_input_text_color_set(input, props->normal_text_color_id, props->selected_text_color_id,\n"
            "                                                props->disabled_text_color_id, props->readonly_text_color_id);\n"
            "        gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->whitespace);\n"
            "        gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->line_space);\n"
            "        if(props->buffer && (props->buffer_size > 0) && props->string_id)\n"
            "        {\n"
            "            gx_system_string_get(props->string_id, &text);\n"
            "            if(text)\n"
            "            {\n"
            "                gx_multi_line_text_input_text_set(input, text);\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }
    else if (version >= 50303)
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_MULTI_LINE_TEXT_INPUT *input = (GX_MULTI_LINE_TEXT_INPUT *) control_block;\n"
            "    GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *props = (GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n"
            "    status = gx_multi_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->font_id);\n"
            "        gx_multi_line_text_view_text_color_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->normal_text_color_id, props->selected_text_color_id);\n"
            "        gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->whitespace);\n"
            "        gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->line_space);\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }
    else
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_MULTI_LINE_TEXT_INPUT *input = (GX_MULTI_LINE_TEXT_INPUT *) control_block;\n"
            "    GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *props = (GX_MULTI_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n"
            "    status = gx_multi_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->font_id);\n"
            "        gx_multi_line_text_view_text_color_set((GX_MULTI_LINE_TEXT_VIEW *) input, props->normal_text_color_id, props->selected_text_color_id);\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }

    return out;
}

widget_info *ml_text_input_service_provider::CreateNewInstance(GX_WIDGET *parent)
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

    GX_MULTI_LINE_TEXT_INPUT *input = new GX_MULTI_LINE_TEXT_INPUT;

    GX_CHAR *buffer = new GX_CHAR[100];
    memset(buffer, 0, 100);

    gx_multi_line_text_input_create(input, "text_input", parent, buffer, 100,
        GX_STYLE_ENABLED|GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_CENTER, 0, &size);
    gx_widget_style_remove(input, GX_STYLE_DRAW_SELECTED);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) input);

    info->font_id[0] = input->gx_multi_line_text_view_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = input->gx_multi_line_text_view_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = input->gx_multi_line_text_view_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = input->gx_multi_line_text_view_disabled_text_color;
    info->color_id[READONLY_FILL_COLOR_INDEX] = input->gx_multi_line_text_input_readonly_fill_color;
    info->color_id[READONLY_TEXT_COLOR_INDEX] = input->gx_multi_line_text_input_readonly_text_color;
    info->string_id[0] = 0;
    info->ewi.text_info.whitespace = 0;
    info->ewi.text_info.line_space = 0;
    info->ewi.text_info.buffer_size = 100;
    info->ewi.text_info.dynamic_buffer = FALSE;
    return info;
}

GX_WIDGET *ml_text_input_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_MULTI_LINE_TEXT_INPUT *input = new GX_MULTI_LINE_TEXT_INPUT;
    GX_CHAR *text = NULL;

    if (info ->ewi.text_info.buffer_size > 1)
    {
        text = new GX_CHAR[info->ewi.text_info.buffer_size + 1];
        memset(text, 0, info->ewi.text_info.buffer_size + 1);
    }
    
    if (text && info->string_id[0])
    {
        GetStringText(text, info->ewi.text_info.buffer_size, info);
    }

    gx_multi_line_text_input_create(input,
        (CHAR *) info->app_name.GetString(),
        parent,
        text, info->ewi.text_info.buffer_size,
        info->style, 0,
        &info->size);

    gx_multi_line_text_input_fill_color_set(input,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX], info->color_id[READONLY_FILL_COLOR_INDEX]);

    gx_multi_line_text_input_text_color_set(input,
        info->color_id[NORMAL_TEXT_COLOR_INDEX], info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX], info->color_id[READONLY_TEXT_COLOR_INDEX]);

    GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)input;
    gx_multi_line_text_view_font_set(view, info->font_id[0]);

    gx_multi_line_text_view_whitespace_set(view, info->ewi.text_info.whitespace);
    gx_multi_line_text_view_line_space_set(view, info->ewi.text_info.line_space);

    return ((GX_WIDGET *) input);
}
        
void ml_text_input_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WriteStringId(writer, project, display, "string_id", info->string_id[0]);
    WriteFontId(writer, project, display, "font_id", info->font_id[0]);
    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_fill_color", info->color_id[DISABLED_FILL_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "readonly_fill_color", info->color_id[READONLY_FILL_COLOR_INDEX]);
    WriteColorId(writer, project, display, "readonly_text_color", info->color_id[READONLY_TEXT_COLOR_INDEX]);
    writer.WriteInt("whitespace", info->ewi.text_info.whitespace);
    writer.WriteInt("line_space", info->ewi.text_info.line_space);

    writer.WriteString("buffer_allocator", info->callback_func);
    writer.WriteInt("buffer_size", info->ewi.text_info.buffer_size);
    writer.WriteBool("dynamic_buffer", info->ewi.text_info.dynamic_buffer);
}

void ml_text_input_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TEXT_ALIGNMENT_MASK|GX_STYLE_TEXT_COPY|GX_STYLE_CURSOR_BLINK|GX_STYLE_CURSOR_ALWAYS|GX_STYLE_TEXT_INPUT_NOTIFY_ALL|GX_STYLE_TEXT_INPUT_READONLY;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->string_id[0] = ReadStringId(reader, project, display, "string_id");
    if (info->string_id[0] < 0 || info->string_id[0] > 100000)
    {
        info->string_id[0] = 0;
    }
    info->font_id[0] = ReadFontId(reader, project, display, "font_id");
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_text_color");
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_text_color");

    if (project->mHeader.project_version > 54)
    {
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_text_color");
        info->color_id[READONLY_FILL_COLOR_INDEX] = ReadColorId(reader, project, display, "readonly_fill_color");
        info->color_id[READONLY_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "readonly_text_color");
    }
    else
    {
        //selected color is start been used after project version 54
        //set selected color to normal color for old projects
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
        info->color_id[READONLY_FILL_COLOR_INDEX] = GX_COLOR_ID_READONLY_FILL;
        info->color_id[READONLY_TEXT_COLOR_INDEX] = GX_COLOR_ID_READONLY_TEXT;
    }

    reader.ReadInt("whitespace", info->ewi.text_info.whitespace, 0);
    reader.ReadInt("line_space", info->ewi.text_info.line_space, 0);

    reader.ReadString("buffer_allocator", info->callback_func);
    reader.ReadInt("buffer_size", info->ewi.text_info.buffer_size, 100);
    reader.ReadBool("dynamic_buffer", info->ewi.text_info.dynamic_buffer);
}
        
void ml_text_input_service_provider::AssignText(widget_info *info, int index,
    GX_RESOURCE_ID string_id)
{
    info->string_id[0] = string_id;
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_MULTI_LINE_TEXT_INPUT *input = (GX_MULTI_LINE_TEXT_INPUT *)widget;

        GX_CHAR *text = (GX_CHAR *) input->gx_multi_line_text_view_text.gx_string_ptr;
        GX_STRING string;

        if (text && info->ewi.text_info.buffer_size > 1)
        {
            GetStringText(text, info->ewi.text_info.buffer_size, info);
            input->gx_multi_line_text_view_text_id = string_id;
            string.gx_string_ptr = text;
            string.gx_string_length = strlen(text);
            _gx_multi_line_text_input_text_set_ext(input, &string);
        }
        gx_system_dirty_mark(widget);
    }
}

void ml_text_input_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_MULTI_LINE_TEXT_INPUT *input = (GX_MULTI_LINE_TEXT_INPUT *) (info->widget);

    switch (index)
    {
    case READONLY_FILL_COLOR_INDEX:
        if (input)
        {
            gx_multi_line_text_input_fill_color_set(input, input->gx_widget_normal_fill_color, input->gx_widget_selected_fill_color,
                input->gx_widget_disabled_fill_color, color_id);
        }

        info->color_id[READONLY_FILL_COLOR_INDEX] = color_id;
        break;

    case READONLY_TEXT_COLOR_INDEX:
        if (input)
        {
            gx_multi_line_text_input_text_color_set(input, input->gx_multi_line_text_view_normal_text_color,
                input->gx_multi_line_text_view_selected_text_color,
                input->gx_multi_line_text_view_disabled_text_color, color_id);
        }

        info->color_id[READONLY_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        ml_text_view_service_provider::AssignColor(info, index, color_id);
    }
}
