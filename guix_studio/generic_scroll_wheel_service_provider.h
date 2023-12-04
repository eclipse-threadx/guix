#ifndef _GENERIC_SCROLL_WHEEL_SERVICE_PROVIDER_
#define _GENERIC_SCROLL_WHEEL_SERVICE_PROVIDER_

class screen_generator; // forward reference

class generic_scroll_wheel_service_provider : public scroll_wheel_service_provider
{
public:
    generic_scroll_wheel_service_provider();

    virtual int GetType() { return GX_TYPE_GENERIC_SCROLL_WHEEL; }
    virtual CString GetTypeName(widget_info*) { return CString("GX_TYPE_GENERIC_SCROLL_WHEEL"); }
    virtual CString GetShortName() { return CString("generic_scroll_wheel"); }
    virtual CString GetVarDeclaration();
    virtual CString GetControlBlockName() { return CString("GX_GENERIC_SCROLL_WHEEL"); }

    virtual CString DeclarePropertiesStructContent();
    virtual CString DeclarePropertiesStruct();
    virtual CString WriteExtendedPropertiesContent(screen_generator* gen, widget_info* info, BOOL add_last_comma = FALSE);
    virtual CString WriteExtendedProperties(screen_generator* gen, CString& scope, widget_info* info);
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info* CreateNewInstance(GX_WIDGET* parent);
    virtual GX_WIDGET* GenerateFromInfo(GX_WIDGET* parent, widget_info* info);

    virtual void SaveToProject(xml_writer&, studiox_project* project, int display, widget_info *);
    virtual void ReadFromProject(xml_reader&, studiox_project* project, int display, widget_info *info, ULONG valid_styles);

    virtual void AssignScrollWheelInfo(widget_info* info, scroll_wheel_info* wheel_info);
};

#endif
