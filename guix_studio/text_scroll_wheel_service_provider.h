#ifndef _TEXT_SCROLL_WHEEL_SERVICE_PROVIDER_
#define _TEXT_SCROLL_WHEEL_SERVICE_PROVIDER_

class screen_generator; // forward reference

class text_scroll_wheel_service_provider : public scroll_wheel_service_provider
{
    public:
        text_scroll_wheel_service_provider();

        virtual int GetType() {return GX_TYPE_TEXT_SCROLL_WHEEL;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_TEXT_SCROLL_WHEEL");}
        virtual CString GetShortName() {return CString("text_scroll_wheel");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_TEXT_SCROLL_WHEEL");}

        virtual CString DeclarePropertiesStructContent();
        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedPropertiesContent(screen_generator *gen, widget_info *info, BOOL add_last_comma = FALSE);
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        
        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

        virtual void AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id);
        virtual void AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id);
};

#endif
