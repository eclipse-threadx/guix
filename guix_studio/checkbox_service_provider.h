#ifndef _CHECKBOX_SERVICE_PROVIDER_
#define _CHECLBOX_SERVICE_PROVIDER_

class screen_generator; // forward reference

class checkbox_service_provider : public text_button_service_provider
{
    public:
        checkbox_service_provider();

        virtual int GetType() {return GX_TYPE_CHECKBOX;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_CHECKBOX");}
        virtual CString GetShortName() {return CString("checkbox");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_CHECKBOX");}

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);
        
        virtual void AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id);
        virtual void AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id);
        virtual void AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id);
        virtual void Autosize(widget_info *info);
};

#endif
