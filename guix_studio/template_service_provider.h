#ifndef _BASE_WIDGET_SERVICE_PROVIDER_
#define _BASE_WIDGET_SERVICE_PROVIDER_

#define GX_TYPE_TEMPLATE 200

class screen_generator; // forward reference

class template_service_provider : public widget_service_provider
{
    public:
        template_service_provider();
        virtual ~template_service_provider();

        virtual int GetType()
        {
             return GX_TYPE_TEMPLATE;
        }
        virtual CString GetTypeName(widget_info *info)
        {
            return CString("GX_TYPE_TEMPLATE");
        }

        virtual CString GetShortName()
        {
            return CString("TEMPLATE");
        }

        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName()
        {
            /* the control block name for template derived is the template base
               app name. Handled by screen_generator
            */
            return CString("");
        }

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        widget_info *CreateNewInstance(GX_WIDGET *parent, widget_info *base);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

        virtual void AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id);
        virtual void Autosize(widget_info *info);

        static widget_info *GetBaseInfo(widget_info *derived, studiox_project *project = NULL);

        static void RebuildDerivedWidgets(widget_info *base_info);
        static int  CountDerivedWidgets(widget_info *base_info);
        static BOOL CheckExternalTemplateDerivations(folder_info *folder);
        static void CountDerivedWidgets(widget_info *start, widget_info *base_info, int &count);
        static void CheckRenameTemplate(CString &old_name, CString *new_name);
        static void CheckRenameTemplate(widget_info *start, CString &old_name, CString *new_name);

    private:
        widget_service_provider *GetBaseProvider(widget_info *derived);
        GX_WIDGET *BuildBaseWidget(GX_WIDGET *parent, widget_info *base, GX_BOOL add_children = FALSE);
        static void RebuildDerivedWidgets(widget_info *start, CString &base_name);

};

#endif
