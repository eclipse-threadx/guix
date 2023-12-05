#ifndef _NUMERIC_PIXELMAP_PROMPT_SERVICE_PROVIDER_
#define _NUMERIC_PIXELMAP_PROMPT_SERVICE_PROVIDER_

class screen_generator; // forward reference

class numeric_pixelmap_prompt_service_provider : public pixelmap_prompt_service_provider
{
public:
    numeric_pixelmap_prompt_service_provider();

    virtual int GetType() { return GX_TYPE_NUMERIC_PIXELMAP_PROMPT; }
    virtual CString GetTypeName(widget_info *) { return CString("GX_TYPE_NUMERIC_PIXELMAP_PROMPT"); }
    virtual CString GetShortName() { return CString("numeric_pixelmap_prompt"); }
    virtual CString GetVarDeclaration();
    virtual CString GetControlBlockName() { return CString("GX_NUMERIC_PIXELMAP_PROMPT"); }

    virtual CString DeclarePropertiesStruct();
    virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
    virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);

    virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
    virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);
    virtual void AssignNumericValue(widget_info *info, INT value);
};

#endif
