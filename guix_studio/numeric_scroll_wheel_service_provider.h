#ifndef _NUMERIC_SCROLL_WHEEL_SERVICE_PROVIDER_
#define _NUMERIC_SCROLL_WHEEL_SERVICE_PROVIDER_

class numeric_scroll_wheel_service_provider : public text_scroll_wheel_service_provider
{
public:
    numeric_scroll_wheel_service_provider();

    virtual int GetType() { return GX_TYPE_NUMERIC_SCROLL_WHEEL; }
    virtual CString GetTypeName(widget_info *) { return CString("GX_TYPE_NUMERIC_SCROLL_WHEEL"); }
    virtual CString GetShortName() { return CString("numeric_scroll_wheel"); }
    virtual CString GetVarDeclaration();
    virtual CString GetControlBlockName() { return CString("GX_NUMERIC_SCROLL_WHEEL"); }

    virtual CString DeclarePropertiesStruct();
    virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
    virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);

    virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
    virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);
    void AssignValueRange(widget_info *info, int start_val, int end_val);
};
#endif
