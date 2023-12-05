#ifndef _RADIO_BUTTON_SERVICE_PROVIDER_
#define _RADIO_BUTTON_SERVICE_PROVIDER_

class screen_generator; // forward reference

class radio_button_service_provider : public text_button_service_provider
{
    public:
        radio_button_service_provider();

        virtual int GetType() {return GX_TYPE_RADIO_BUTTON;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_RADIO_BUTTON");}
        virtual CString GetShortName() {return CString("radio_button");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_RADIO_BUTTON");}

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

        virtual void AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id);
        virtual void Autosize(widget_info *info);
};

#endif
