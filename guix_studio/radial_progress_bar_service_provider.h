
#ifndef _RADIAL_PROGRESS_BAR_SERVICE_PROVIDER_
#define _RADIAL_PROGRESS_BAR_SERVICE_PROVIDER_

class screen_generator; // forward reference

class radial_progress_bar_service_provider : public widget_service_provider
{
    public:
        radial_progress_bar_service_provider();

        virtual int GetType() { return GX_TYPE_RADIAL_PROGRESS_BAR; }
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_RADIAL_PROGRESS_BAR");}
        virtual CString GetShortName() {return CString("radial_progress_bar");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_RADIAL_PROGRESS_BAR");}

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

        virtual void AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id);
        virtual void AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id);
};

#endif
