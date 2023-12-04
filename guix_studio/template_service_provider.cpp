
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//CArray<template_info *> template_service_provider::base_info_list;

extern "C" {
extern GX_DISPLAY  target_win_display;
}

///////////////////////////////////////////////////////////////////////////////
template_service_provider::template_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
template_service_provider::~template_service_provider()
{
    Reset();
}

///////////////////////////////////////////////////////////////////////////////
CString template_service_provider::GetVarDeclaration()
{
    /* For derived widgets, the screen_generator installs and instance
       of the base widget control block. Nothing to do here.
    */
    return CString("");
}


///////////////////////////////////////////////////////////////////////////////
CString template_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "   GX_CONST GX_STUDIO_WIDGET *base_info;\n"
    "   UINT (*base_create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);\n"
    "   GX_RECTANGLE size;\n"
    "} GX_TEMPLATE_PROPERTIES;\n\n");
    return out;   
}

///////////////////////////////////////////////////////////////////////////////
CString template_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    /* derived class should write extended properties of it's base */
    CString out;
    widget_service_provider *bp = GetBaseProvider(info);
    widget_info *base = GetBaseInfo(info);
    studiox_project *project = GetOpenProject();

    if (project && base && bp)
    {
        widget_info *parent = project->FindParentInfo(base);

        CString parent_name = _T("");

        if (parent)
        {
            parent_name = parent->app_name;
        }

        CString temp = prefix + info->app_name;
        out.Format(_T("GX_TEMPLATE_PROPERTIES %s_properties =\n{\n"), temp);

        if (parent_name.IsEmpty())
        {
            temp.Format(_T("    &%s%s_define,  /* base info */\n"), gen->GetScreenNamePrefix(), base->app_name);
        }
        else
        {
            temp.Format(_T("    %s_%s_define,  /* base info */\n"), parent_name, base->app_name);
        }
        out += temp;
        temp.Format(_T("    %s,  /* base create function */\n"), bp->GetCreateFromDefFunctionName());
        out += temp;

        temp.Format(_T("    {%d, %d, %d, %d}   /* widget size */\n};\n"),
            info->size.gx_rectangle_left,
            info->size.gx_rectangle_top,
            info->size.gx_rectangle_right,
            info->size.gx_rectangle_bottom);
        out += temp;
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString template_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_template_create");
}

///////////////////////////////////////////////////////////////////////////////
CString template_service_provider::GetCreateFromDefFunction(int version)
{
    studiox_project *project = GetOpenProject();
    CString out("");
    MakeCreatePreamble("template", version, out);

    out += "{\n"
        "    UINT status = GX_SUCCESS;\n"
        "    GX_STUDIO_WIDGET derived;\n"
        "    GX_TEMPLATE_PROPERTIES *props = (GX_TEMPLATE_PROPERTIES *) info->properties;\n\n"
        "    /* create base widget and children */ \n"
        "    derived =            *props->base_info;\n"
        "    derived.widget_id =   info->widget_id;\n"
        "    derived.widget_name = info->widget_name;\n"
        "\n"
        "    if (info->style & GX_STYLE_ENABLED)\n"
        "    {\n"
        "        derived.style |= GX_STYLE_ENABLED;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        derived.style &= ~GX_STYLE_ENABLED;\n"
        "    }\n"
        "    if (info->status & GX_STATUS_ACCEPTS_FOCUS)\n"
        "    {\n"
        "        derived.status |= GX_STATUS_ACCEPTS_FOCUS;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        derived.status &= ~GX_STATUS_ACCEPTS_FOCUS;\n"
        "    }\n\n";

    if (project->mHeader.guix_version > 50206)
    {

        out += "    #if defined(GX_WIDGET_USER_DATA)\n"
               "    derived.user_data =   info->user_data;\n"
               "    #endif\n\n";
    }

    out += "    control_block = gx_studio_widget_create((GX_BYTE *) control_block, &derived, parent);\n"
        "    if (control_block)\n"
        "    {\n"
        "        if (info->style & GX_STYLE_DYNAMICALLY_ALLOCATED)\n"
        "        {\n"
        "            control_block->gx_widget_status |= GX_STATUS_DYNAMICALLY_ALLOCATED;\n"
        "        }\n"
        "        if (derived.size.gx_rectangle_left != info->size.gx_rectangle_left ||\n"
        "            derived.size.gx_rectangle_top != info->size.gx_rectangle_top)\n"
        "        {\n"
        "            gx_widget_shift(control_block,\n"
        "                info->size.gx_rectangle_left - derived.size.gx_rectangle_left,\n"
        "                info->size.gx_rectangle_top - derived.size.gx_rectangle_top, GX_FALSE);\n"
        "        }\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        status = GX_FAILURE;\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out; 
}


///////////////////////////////////////////////////////////////////////////////
widget_info *template_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *template_service_provider::CreateNewInstance(GX_WIDGET *parent, widget_info *base)
{
    GX_RECTANGLE size = base->size;

    widget_info *derived_info = new widget_info;
    *derived_info = *base;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    derived_info->size = size;

    derived_info->app_name = CString("derived");
    widget_factory::CreateUniqueAppNames(derived_info);

    derived_info->widget = NULL;
    derived_info->SetChildWidgetInfo(NULL);
    derived_info->SetNextWidgetInfo(NULL);
    derived_info->basetype = GX_TYPE_TEMPLATE;
    derived_info->is_template = FALSE;
    derived_info->ewi.template_display = GetProjectView()->GetActiveDisplay();
    derived_info->base_name = base->app_name;

    GX_WIDGET *derived = GenerateFromInfo(parent, derived_info);
    derived_info->widget = derived;
    return derived_info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *template_service_provider::BuildBaseWidget(GX_WIDGET *parent, widget_info *info, GX_BOOL add_children)
{
    GX_WIDGET *template_instance = NULL;

    if (widget_factory::FindBaseName(info->base_name))
    {

        widget_service_provider *pp = widget_factory::GetServiceProvider(info->basetype);

        if (pp)
        {
            /* generate an instance of this template base */
            template_instance = pp->GenerateFromInfo(parent, info);
        }
    }
    else
    {
        widget_info* base_info = GetBaseInfo(info);

        if (base_info)
        {
            // this template is based on another, start by building the base for this
            // template:
            template_instance = BuildBaseWidget(parent, base_info, TRUE);
        }
    }

    if (template_instance)
    {
        /* move the template instance to the correct position */
        if (template_instance->gx_widget_size.gx_rectangle_left != info->size.gx_rectangle_left ||
            template_instance->gx_widget_size.gx_rectangle_top != info->size.gx_rectangle_top)
        {
            _gx_widget_shift(template_instance,
                info->size.gx_rectangle_left - template_instance->gx_widget_size.gx_rectangle_left,
                info->size.gx_rectangle_top - template_instance->gx_widget_size.gx_rectangle_top, GX_TRUE);

            info->size = template_instance->gx_widget_size;
        }

        if (add_children)
        {
            /* generate children of this template instance */
            if (info->GetChildWidgetInfo())
            {
                widget_factory::GenerateWidgets(template_instance, info->GetChildWidgetInfo(), FALSE);
            }
        }
            
        if (info->style & GX_STYLE_ENABLED)
        {
            if (!(template_instance->gx_widget_style & GX_STYLE_ENABLED))
            {
                _gx_widget_style_add(template_instance, GX_STYLE_ENABLED);
            }
        }
        else
        {
            if (template_instance->gx_widget_style & GX_STYLE_ENABLED)
            {
                _gx_widget_style_remove(template_instance, GX_STYLE_ENABLED);
            }
        }       
    }
    return template_instance;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *template_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_WIDGET *instance;

    instance = BuildBaseWidget(parent, info, FALSE);

    if (instance)
    {
        instance->gx_widget_name = (CHAR*)info->app_name.GetString();
    }

    return instance;
}

        
///////////////////////////////////////////////////////////////////////////////
void template_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    widget_info *base_info = GetBaseInfo(info, project);

    if (base_info)
    {    
        if (base_info->app_name)
        {
            writer.WriteString("template_base", base_info->app_name);
        }
        else
        {
            ErrorMsg("Internal Error: Template base has no name");
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void template_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= 0x7fffffff;

    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadString("template_base", info->base_name);
    info->ewi.template_display = display;
}
        
///////////////////////////////////////////////////////////////////////////////
void template_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
}

///////////////////////////////////////////////////////////////////////////////
void template_service_provider::Autosize(widget_info *info)
{
}


///////////////////////////////////////////////////////////////////////////////
widget_info *template_service_provider::GetBaseInfo(widget_info *derived, studiox_project *project)
{
    widget_info *base = NULL;

    if (!project)
    {
        project = GetOpenProject();
    }

    if (derived->app_name == derived->base_name)
    {
        if (derived->is_template)
        {
            ErrorMsg("Internal error: Template is based on itself");

            return NULL;
        }
    }

    if (project)
    {
        folder_info *folder = project->mDisplays[derived->ewi.template_display].GetFirstChildFolder();
        base = project->FindWidgetInfo(folder, derived->base_name, FALSE);
    }

    if (base)
    {
        if (!base->is_template)
        {
            return NULL;
        }
    }

    return base;
}

///////////////////////////////////////////////////////////////////////////////
widget_service_provider *template_service_provider::GetBaseProvider(widget_info *derived)
{
    widget_service_provider *bp = NULL;
    widget_info *base_info = GetBaseInfo(derived);

    if (base_info)
    {
        bp = widget_factory::GetServiceProvider(base_info->basetype);
    }
    return bp;
}

///////////////////////////////////////////////////////////////////////////////
/* This function is called only when a folder is being deleted. Check to see
   if any widgets outside of this folder are derived from a template
   within this folder
*/
BOOL template_service_provider::CheckExternalTemplateDerivations(folder_info *folder)
{
    int active_display;
    folder_info *folder_item;;
    int derived_widgets;

    widget_info *info = folder->GetFirstChildWidget();
    active_display = GetProjectView()->GetActiveDisplay();

    /* Go through the child list to see there's template or not. */
    while (info)
    {
        if (info->is_template)
        {
            derived_widgets = 0;

            /* Count total derived_widgets.*/
            folder_item = GetOpenProject()->mDisplays[active_display].GetFirstChildFolder();
            while (folder_item)
            {
                if (folder_item != folder)
                {
                    //count derived widgets outside the current folder
                    CountDerivedWidgets(folder_item->GetFirstChildWidget(), info, derived_widgets);
                }
                folder_item = folder_item->GetNextFolder();
            }

            /* go to next child if total derived_widgets is 0. */
            if (derived_widgets != 0)
            {
                return FALSE;
            }
        }
        info = info->GetNextWidgetInfo();
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Count widgets that are derived from a given base
int template_service_provider::CountDerivedWidgets(widget_info *base_info)
{
    int active_display;
    folder_info *folder;
    int derived_widgets = 0;

    active_display = GetProjectView()->GetActiveDisplay();
    folder = GetOpenProject()->mDisplays[active_display].GetFirstChildFolder();

    while (folder)
    {
        CountDerivedWidgets(folder->GetFirstChildWidget(), base_info, derived_widgets);
        folder = folder->GetNextFolder();
    }

    return derived_widgets;
}

///////////////////////////////////////////////////////////////////////////////
void template_service_provider::CountDerivedWidgets(widget_info *start, widget_info *base_info, int &count)
{
    widget_info *child;
    widget_info *next;

    while (start)
    {
        next = start->GetNextWidgetInfo();

        if (start->base_name == base_info->app_name)
        {
            count++;
        }
        else
        {
            child = start->GetChildWidgetInfo();

            if (child)
            {
                CountDerivedWidgets(child, base_info, count);
            }
        }

        start = next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void template_service_provider::CheckRenameTemplate(CString &old_name, CString *new_name)
{
    int active_display;
    folder_info *folder;

    active_display = GetProjectView()->GetActiveDisplay();
    folder = GetOpenProject()->mDisplays[active_display].GetFirstChildFolder();
    while (folder)
    {
        CheckRenameTemplate(folder->GetFirstChildWidget(), old_name, new_name);
        folder = folder->GetNextFolder();
    }
    
}

///////////////////////////////////////////////////////////////////////////////
void template_service_provider::CheckRenameTemplate(widget_info *start, CString &old_name, CString *new_name)
{
    widget_info *child;

    while (start)
    {
        if (start->base_name == old_name)
        {
            start->base_name = *new_name;
        }

        child = start->GetChildWidgetInfo();

        if (child)
        {
            CheckRenameTemplate(child, old_name, new_name);
        }

        start = start->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void template_service_provider::RebuildDerivedWidgets(widget_info *start, CString &base_name)
{
    GX_WIDGET *parent;

    while (start)
    {
        if ((start->base_name == base_name) && start->widget)
        {
            parent = start->widget->gx_widget_parent;
            _gx_widget_detach(start->widget);

            widget_factory::CleanupWidgets(start, TRUE, FALSE);
            widget_factory::GenerateWidgets(parent, start, TRUE, FALSE);

            // if this widget is also a template, check for any higher level derivations:
            if (start->is_template)
            {
                RebuildDerivedWidgets(start);
            }
        }
        if (start->GetChildWidgetInfo())
        {
            RebuildDerivedWidgets(start->GetChildWidgetInfo(), base_name);
        }
        start = start->GetNextWidgetInfo();
    }
}


///////////////////////////////////////////////////////////////////////////////
void template_service_provider::RebuildDerivedWidgets(widget_info *base_info)
{
    int active_display;
    folder_info *folder;
    widget_info *info;
    
    active_display = GetProjectView()->GetActiveDisplay();

    folder = GetOpenProject()->mDisplays[active_display].GetFirstChildFolder();
    while (folder)
    {
        info = folder->GetFirstChildWidget();
        RebuildDerivedWidgets(info, base_info->app_name);
        folder = folder->GetNextFolder();
    }
}
