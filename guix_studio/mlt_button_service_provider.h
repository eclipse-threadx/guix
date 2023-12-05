#ifndef _MLT_BUTTON_SERVICE_PROVIDER_
#define _MLT_BUTTON_SERVICE_PROVIDER_

class screen_generator; // forward reference

class mlt_button_service_provider : public text_button_service_provider
{
    public:
        mlt_button_service_provider();

        virtual int GetType() {return GX_TYPE_MULTI_LINE_TEXT_BUTTON;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_MULTI_LINE_TEXT_BUTTON");}
        virtual CString GetShortName() {return CString("multi_line_text_button");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_MULTI_LINE_TEXT_BUTTON");}

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);

        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id);
};

#endif
