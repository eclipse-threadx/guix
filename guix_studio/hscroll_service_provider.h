#ifndef _HSCROLL_SERVICE_PROVIDER_
#define _HSCROLL_SERVICE_PROVIDER_

class hscroll_service_provider : public widget_service_provider
{
    public:
        hscroll_service_provider();

        virtual int GetType() {return GX_TYPE_HORIZONTAL_SCROLL;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_HORIZONTAL_SCROLL");}
        virtual CString GetShortName() {return CString("hscroll");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_SCROLLBAR");}

        virtual CString DeclarePropertiesStruct() {return CString("");}
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info);

        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);

        virtual void AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id);
        virtual void Autosize(widget_info *info);
};

#endif
