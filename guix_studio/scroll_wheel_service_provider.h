#ifndef _SCROLL_WHEEL_SERVICE_PROVIDER_
#define _SCROLL_WHEEL_SERVICE_PROVIDER_

class scroll_wheel_service_provider: public window_service_provider
{
public:
    scroll_wheel_service_provider();

    virtual int GetType() { return GX_TYPE_SCROLL_WHEEL; }
    virtual CString GetTypeName(widget_info *) { return CString("GX_TYPE_SCROLL_WHEEL"); }
    virtual CString GetShortName() { return CString("scroll_wheel"); }
    virtual CString GetVarDeclaration();
    virtual CString GetControlBlockName() { return CString("GX_SCROLL_WHEEL"); }

    virtual CString DeclarePropertiesStruct();
    virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
    virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
                         
    virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
    virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

    virtual void AssignScrollWheelInfo(widget_info *info, scroll_wheel_info *wheel_info);
    virtual void AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id);
};

#endif

