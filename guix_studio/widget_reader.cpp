
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
widget_info *widget_reader::ReadOneWidget(xml_reader &reader, studiox_project *project, int display)
{
    CString widget_type_name;
    widget_info *info = NULL;

    reader.ReadString("type", widget_type_name);
    int widget_type = widget_factory::WidgetStringToType(widget_type_name);

    widget_service_provider *provider = widget_factory::GetServiceProvider(widget_type);

    if (provider)
    {
        info = new widget_info(widget_type);
        provider->ReadFromProject(reader, project, display, info, 0);

        if (!info->id_name.IsEmpty())
        {
            /* Add id name to id dictionary. */
            project->AddToIdDictionary(display, ID_TYPE_WIDGET, info->id_name);
        }
    }
    else
    {
        ErrorMsg("Unknown widget type in project");
    }

    return info;
}

///////////////////////////////////////////////////////////////////////////////
void widget_reader::ReadChildWidgets(xml_reader &reader, studiox_project *project, int display, widget_info *parent)
{
    widget_info *widget = NULL;
    widget_info *previous = NULL;

    while (reader.EnterSection(CT2A(widget_factory::WidgetTypeToString(GX_TYPE_WIDGET))))
    {
        widget = ReadOneWidget(reader, project, display);

        if (widget)
        {
            if (previous)
            {
                previous->SetNextWidgetInfo(widget);
            }
            else
            {
                parent->SetChildWidgetInfo(widget);
            }
            ReadChildWidgets(reader, project, display, widget);
            previous = widget;
        }
        
        reader.CloseSection(TRUE, TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_reader::ReadWidgets(xml_reader &reader, studiox_project *project, int display, folder_info *widgets_folder)
{
    CString widget_type_name;
    widget_info *widget = NULL;
    widget_info *previous = NULL;

    while (reader.EnterSection(CT2A(widget_factory::WidgetTypeToString(GX_TYPE_WIDGET))))
    {
        widget = ReadOneWidget(reader, project, display);

        if (widget)
        {
            if (previous)
            {
                previous->SetNextWidgetInfo(widget);
            }
            else
            {
                widgets_folder->SetFirstChildWidget(widget);
            }
            ReadChildWidgets(reader, project, display, widget);
            previous = widget;

            /* This is a check to repair templates that were previously allowed to be marked as
               dynamically allocated
            */

            if (widget->is_template)
            {
                if (widget->allocation != STATICALLY_ALLOCATED)
                {
                    widget->allocation = STATICALLY_ALLOCATED;
                    properties_win::SetChildAllocation(widget->GetChildWidgetInfo(), STATICALLY_ALLOCATED);
                }
            }
        }
        reader.CloseSection(TRUE, TRUE);
    }
}



///////////////////////////////////////////////////////////////////////////////
void widget_reader::ReadWidgetFolders(xml_reader &reader, studiox_project *project, int display)
{
    CString widget_type_name;
    folder_info *folder = NULL;
    folder_info *previous = NULL;
    display_info *info = &project->mDisplays[display];

    while (reader.EnterSection("widget_folder"))
    {
        CString name;
        reader.ReadString("folder_name", name);
        folder = new folder_info(name);

        if (previous)
        {
            previous->SetNextFolder(folder);
        }
        else
        {
            info->SetFirstChildFolder(folder);
        }
        previous = folder;
        reader.ReadString("specified_output_name", name);
        folder->output_filename = name;
        ReadWidgets(reader, project, display, folder);
        reader.CloseSection(TRUE, TRUE);
    }
}