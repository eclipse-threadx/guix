#ifndef _STRING_SCROLL_WHEEL_SERVICE_PROVIDER_
#define _STRING_SCROLL_WHEEL_SERVICE_PROVIDER_

class string_scroll_wheel_service_provider : public text_scroll_wheel_service_provider
{
public:
    string_scroll_wheel_service_provider();

    virtual int GetType() { return GX_TYPE_STRING_SCROLL_WHEEL; }
    virtual CString GetTypeName(widget_info *) { return CString("GX_TYPE_STRING_SCROLL_WHEEL"); }
    virtual CString GetShortName() { return CString("string_scroll_wheel"); }
    virtual CString GetVarDeclaration();
    virtual CString GetControlBlockName() { return CString("GX_STRING_SCROLL_WHEEL"); }

    virtual CString DeclarePropertiesStruct();
    virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
    virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);

    virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
    virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

    virtual void AssignScrollWheelInfo(widget_info *info, scroll_wheel_info *wheel_info);
    void StringEditEnableDisable(widget_info *info, BOOL enabled);
    static void InitStringIdListArray(GX_RESOURCE_ID *string_id_list, int total_rows, CArray<GX_RESOURCE_ID> &string_id_list_array);
    void CreateStringIdList(widget_info *info, CArray<GX_RESOURCE_ID> &string_id_list_array);

    virtual void AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id);
    virtual void Autosize(widget_info *info);
};

#endif
