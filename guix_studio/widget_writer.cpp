
#include "studiox_includes.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void widget_writer::WriteWidgets(xml_writer &writer, studiox_project *project, int display_index, widget_info *start, BOOL DoSiblings)
{
    while(start)
    {
        widget_service_provider *provider = widget_factory::GetServiceProvider(start->basetype);
        writer.OpenTag(CT2A(widget_factory::WidgetTypeToString(GX_TYPE_WIDGET)));

        if (provider)
        {
            provider->SaveToProject(writer, project, display_index, start);
        }
        else
        {
            ErrorMsg("Unknown widget type");
        }
        if (start ->GetChildWidgetInfo())
        {
            WriteWidgets(writer, project, display_index, start->GetChildWidgetInfo());
        }
        writer.CloseTag(CT2A(widget_factory::WidgetTypeToString(GX_TYPE_WIDGET)));

        if (DoSiblings)
        {
            start = start->GetNextWidgetInfo();
        }
        else
        {
            return;
        }
    }
}

void widget_writer::WriteWidgetFolders(xml_writer &writer, studiox_project *project, int display_index, folder_info *start, BOOL DoSiblings)
{
    while (start)
    {
        writer.OpenTag("widget_folder");
        writer.WriteString("folder_name", start->folder_name);
        writer.WriteString("specified_output_name", start->output_filename);
        WriteWidgets(writer, project, display_index, start->GetFirstChildWidget(), TRUE);
        writer.CloseTag("widget_folder");

        if (DoSiblings)
        {
            start = start->GetNextFolder();
        }
        else
        {
            return;
        }
    }
}
