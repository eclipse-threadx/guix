#ifndef _ML_TEXT_INPUT_SERVICE_PROVIDER_
#define _ML_TEXT_INPUT_SERVICE_PROVIDER_

class screen_generator; // forward reference

class ml_text_input_service_provider : public ml_text_view_service_provider
{
    public:
        ml_text_input_service_provider();

        virtual int GetType() {return GX_TYPE_MULTI_LINE_TEXT_INPUT;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_MULTI_LINE_TEXT_INPUT");}
        virtual CString GetShortName() {return CString("ml_text_input");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_MULTI_LINE_TEXT_INPUT");}

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

        virtual void AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id);
        virtual void AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id);
};

#endif
