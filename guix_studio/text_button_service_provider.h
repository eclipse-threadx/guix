#ifndef _TEXT_BUTTON_SERVICE_PROVIDER_
#define _TEXT_BUTTON_SERVICE_PROVIDER_

class screen_generator; // forward reference

class text_button_service_provider : public button_service_provider
{
    public:
        text_button_service_provider();

        virtual int GetType() {return GX_TYPE_TEXT_BUTTON;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_TEXT_BUTTON");}
        virtual CString GetShortName() {return CString("text_button");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_TEXT_BUTTON");}

        virtual CString DeclaraPropertiesStructContent();
        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedPropertiesContent(screen_generator *gen, widget_info *info, BOOL add_last_comma = FALSE);
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);
        
        virtual void AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id);
        virtual void AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id);
        virtual void AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id);
        virtual void Autosize(widget_info *info);
};

#endif
