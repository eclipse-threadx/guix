#ifndef _MENU_SERVICE_PROVIDER_
#define _MENU_SERVICE_PROVIDER_

class screen_generator; // forward reference

class menu_service_provider : public pixelmap_prompt_service_provider
{
public:
    menu_service_provider();

    virtual int GetType() { return GX_TYPE_MENU; }
    virtual CString GetTypeName(widget_info *) { return CString("GX_TYPE_MENU"); }
    virtual CString GetShortName() { return CString("menu"); }
    virtual CString GetVarDeclaration();
    virtual CString GetControlBlockName() { return CString("GX_MENU"); }

    virtual CString DeclarePropertiesStruct();
    virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
    virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);

    virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
    virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

    virtual void AssignTextOffset(widget_info *info, int text_x_offset, int text_y_offset);
    virtual void AssignStyle(widget_info *info, int style);

    static GX_WIDGET *TopLevelMenuContainerFind(GX_WIDGET *start);
    void Autosize(widget_info *info);
};

#endif
