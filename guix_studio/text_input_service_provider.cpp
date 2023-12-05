
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

text_input_service_provider::text_input_service_provider()
{
}

CString text_input_service_provider::GetVarDeclaration()
{
    return CString("GX_SINGLE_LINE_TEXT_INPUT_MEMBERS_DECLARE");
}

CString text_input_service_provider::DeclarePropertiesStruct()
{
    CString out;

    if (project_lib_version() > 50401)
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
            "    GX_CHAR *buffer;\n"
            "    UINT buffer_size;\n"
            "} GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES;\n\n");
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
            "} GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES;\n\n");
    }
    return out;
}

CString text_input_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString props;
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
        props.Format(_T("GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s, /* disabled text color */\n")
            _T("    %s, /* readonly fill color */\n")
            _T("    %s, /* readonly text color */\n")
            _T("    %s, /* buffer */\n")
            _T("    %d, /* buffer size */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[READONLY_FILL_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[READONLY_TEXT_COLOR_INDEX]),
            buffer,
            info->ewi.text_info.buffer_size);
    }
    else
    {
        props.Format(_T("GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s, /* buffer */\n")
            _T("    %d, /* buffer size */\n};\n"),
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

CString text_input_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_text_input_create");
}

#if 0
UINT  _gx_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr,
                                        CHAR *name,
                                        GX_WIDGET *parent,
                                        CHAR *input_buffer,
                                        UINT buffer_size,
                                        UINT style,
                                        USHORT text_input_id,
                                        GX_RECTANGLE *size)
#endif


CString text_input_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("text_input", version, out);
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return out;
    }

    if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
    {
        CString string;
        string.Format(_T("{\n")
            _T("    UINT status;\n")
            _T("    GX_STRING text;\n")
            _T("    GX_SINGLE_LINE_TEXT_INPUT *input = (GX_SINGLE_LINE_TEXT_INPUT *) control_block;\n")
            _T("    GX_PROMPT *prompt = (GX_PROMPT *) input;\n")
            _T("    GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES *props = (GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n")
            _T("    status = gx_single_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n")
            _T("    if (status == GX_SUCCESS)\n")
            _T("    {\n")
            _T("        gx_prompt_font_set(prompt, props->font_id);\n")
            _T("        gx_single_line_text_input_text_color_set(input, props->normal_text_color_id, props->selected_text_color_id,\n")
            _T("                                                 props->disabled_text_color_id, props->readonly_text_color_id);\n")
            _T("        gx_single_line_text_input_fill_color_set(input, input->gx_widget_normal_fill_color, input->gx_widget_selected_fill_color,\n")
            _T("                                                 input->gx_widget_disabled_fill_color, props->readonly_fill_color_id);\n")
            _T("        if (props->buffer && props->buffer_size > 0 && props->string_id)\n")
            _T("        {\n")
            _T("             gx_display_string_get_ext(%s_display_table[0].display, props->string_id, &text);\n")// FIXME: string set issue
            _T("\n")
            _T("             if (text.gx_string_ptr)\n")
            _T("             {\n")
            _T("                 gx_single_line_text_input_text_set_ext(input, &text);\n")
            _T("             }\n")
            _T("        }\n")
            _T("    }\n")
            _T("    return status;\n")
            _T("}\n"),
            project->mHeader.project_name);

        out += string;
    }
    else if (project_lib_version() > 50401)
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_CONST GX_CHAR *text;\n"
            "    GX_SINGLE_LINE_TEXT_INPUT *input = (GX_SINGLE_LINE_TEXT_INPUT *) control_block;\n"
            "    GX_PROMPT *prompt = (GX_PROMPT *) input;\n"
            "    GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES *props = (GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n"
            "    status = gx_single_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_prompt_font_set(prompt, props->font_id);\n"
            "        gx_single_line_text_input_text_color_set(input, props->normal_text_color_id, props->selected_text_color_id,\n"
            "                                                 props->disabled_text_color_id, props->readonly_text_color_id);\n"
            "        gx_single_line_text_input_fill_color_set(input, input->gx_widget_normal_fill_color, input->gx_widget_selected_fill_color,\n"
            "                                                 input->gx_widget_disabled_fill_color, props->readonly_fill_color_id);\n"
            "        if (props->buffer && props->buffer_size > 0 && props->string_id)\n"
            "        {\n"
            "             gx_system_string_get(props->string_id, &text);\n"
            "\n"
            "             if (text)\n"
            "             {\n"
            "                 gx_single_line_text_input_text_set(input, text);\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }
    else
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_SINGLE_LINE_TEXT_INPUT *input = (GX_SINGLE_LINE_TEXT_INPUT *) control_block;\n"
            "    GX_PROMPT *prompt = (GX_PROMPT *) input;\n"
            "    GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES *props = (GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES *) info->properties;\n"
            "    status = gx_single_line_text_input_create(input, info->widget_name, parent, props->buffer, props->buffer_size, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_prompt_font_set(prompt, props->font_id);\n"
            "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }
    return out;
}

widget_info *text_input_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_SINGLE_LINE_TEXT_INPUT *input = new GX_SINGLE_LINE_TEXT_INPUT;
    GX_CHAR *buffer = new GX_CHAR[100];
    memset(buffer, 0, 100);

    gx_single_line_text_input_create(input, "text_input", parent, buffer, 100,
        GX_STYLE_ENABLED|GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_LEFT, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) input);

    info->string_id[0] = 0;
    info->font_id[0] = input->gx_prompt_font_id;
    info->ewi.text_info.buffer_size = 100;
    info->ewi.text_info.dynamic_buffer = false;
    info->callback_func = GetMallocName();
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = input->gx_prompt_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = input->gx_prompt_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = input->gx_prompt_disabled_text_color;
    info->color_id[READONLY_FILL_COLOR_INDEX] = input->gx_single_line_text_input_readonly_fill_color;
    info->color_id[READONLY_TEXT_COLOR_INDEX] = input->gx_single_line_text_input_readonly_text_color;
    return info;
}

GX_WIDGET *text_input_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_SINGLE_LINE_TEXT_INPUT *input = new GX_SINGLE_LINE_TEXT_INPUT;
    
    GX_CHAR *text = NULL;

    if (info->ewi.text_info.buffer_size > 1)
    {
        text = new GX_CHAR[info->ewi.text_info.buffer_size + 1];
    }
    
    if (text && info->string_id[0])
    {
        GetStringText((char*)text, info->ewi.text_info.buffer_size, info);
    }
    else
    {
        text[0] = '\0';
    }

    gx_single_line_text_input_create(input,
        (CHAR*)info->app_name.GetString(),
        parent,
        text, info->ewi.text_info.buffer_size,
        info->style, 0,
        &info->size);

    gx_single_line_text_input_fill_color_set(input,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX], info->color_id[READONLY_FILL_COLOR_INDEX]);
    gx_single_line_text_input_text_color_set(input,
        info->color_id[NORMAL_TEXT_COLOR_INDEX], info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX], info->color_id[READONLY_TEXT_COLOR_INDEX]);
    gx_prompt_font_set((GX_PROMPT *) input, info->font_id[0]);

    return ((GX_WIDGET *) input);
}
        
void text_input_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WriteStringId(writer, project, display, "string_id", info->string_id[0]);
    WriteFontId(writer, project, display, "font_id", info->font_id[0]);
    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "readonly_fill_color", info->color_id[READONLY_FILL_COLOR_INDEX]);
    WriteColorId(writer, project, display, "readonly_text_color", info->color_id[READONLY_TEXT_COLOR_INDEX]);
    writer.WriteString("buffer_allocator", info->callback_func);
    writer.WriteInt("buffer_size", info->ewi.text_info.buffer_size);
    writer.WriteBool("dynamic_buffer", info->ewi.text_info.dynamic_buffer);
}

void text_input_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TEXT_ALIGNMENT_MASK|GX_STYLE_CURSOR_BLINK|GX_STYLE_CURSOR_ALWAYS|GX_STYLE_TEXT_INPUT_NOTIFY_ALL|GX_STYLE_TEXT_INPUT_READONLY;
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
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
        info->color_id[READONLY_FILL_COLOR_INDEX] = GX_COLOR_ID_READONLY_FILL;
        info->color_id[READONLY_TEXT_COLOR_INDEX] = GX_COLOR_ID_READONLY_TEXT;
    }
    reader.ReadString("buffer_allocator", info->callback_func);
    reader.ReadInt("buffer_size", info->ewi.text_info.buffer_size, 100);
    reader.ReadBool("dynamic_buffer", info->ewi.text_info.dynamic_buffer);
}
        
void text_input_service_provider::AssignText(widget_info *info, int index,
    GX_RESOURCE_ID string_id)
{
    info->string_id[0] = string_id;
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_SINGLE_LINE_TEXT_INPUT *input = (GX_SINGLE_LINE_TEXT_INPUT *)widget;

        if (input->gx_single_line_text_input_buffer &&
            info->ewi.text_info.buffer_size > 1)
        {
            CHAR *text = new CHAR[info->ewi.text_info.buffer_size];
            GetStringText(text, info->ewi.text_info.buffer_size, info);
            gx_single_line_text_input_buffer_clear(input);
            gx_single_line_text_input_character_insert(input, (GX_UBYTE *)text, strlen(text));
            delete text;
        }
        gx_system_dirty_mark(widget);
    }
}

void text_input_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_SINGLE_LINE_TEXT_INPUT *input = (GX_SINGLE_LINE_TEXT_INPUT *) (info->widget);

    switch (index)
    {
    case READONLY_FILL_COLOR_INDEX:
        if (input)
        {
            gx_single_line_text_input_fill_color_set(input, input->gx_widget_normal_fill_color, input->gx_widget_selected_fill_color,
                input->gx_prompt_disabled_text_color, color_id);
        }
        info->color_id[READONLY_FILL_COLOR_INDEX] = color_id;
        break;

    case READONLY_TEXT_COLOR_INDEX:
        if (input)
        {
            gx_single_line_text_input_text_color_set(input, input->gx_prompt_normal_text_color,
                input->gx_prompt_selected_text_color, input->gx_prompt_disabled_text_color, color_id);
        }
        info->color_id[READONLY_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        prompt_service_provider::AssignColor(info, index, color_id);
    }
}

void text_input_service_provider::Autosize(widget_info *info)
{
    CheckResizeToText(info, RESIZE_MODE_HEIGHT);
}
