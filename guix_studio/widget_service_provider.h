#ifndef _WIDGET_SERVICE_PROVIDER_
#define _WIDGET_SERVICE_PROVIDER_

class screen_generator; // forward reference

#define RESIZE_MODE_ALL    1
#define RESIZE_MODE_HEIGHT 2
#define RESIZE_MODE_WIDTH  3


class widget_service_provider
{
    public:
        widget_service_provider();
        virtual ~widget_service_provider() {}
        widget_info *InitWidgetInfo(GX_WIDGET *child);

        virtual int GetType() {return GX_TYPE_WIDGET;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_WIDGET");}
        virtual CString GetShortName() {return CString("widget");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_WIDGET");}
        static  CString GetDefaultEventProcess(int widget_type);

        virtual CString DeclarePropertiesStruct();
        virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info) {return CString("");}
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);
        GX_RESOURCE_ID GetStringId(char *string);
        void GetStringText(CHAR *put, int limit, widget_info *info);
        static void GetTextDimension(int base_type, GX_CONST GX_CHAR *string, GX_FONT *normal_font, GX_FONT *selected_font, INT &max_width, INT &max_height);
        static void GetTextDrawDimension(widget_info *info, GX_DISPLAY *display, INT &draw_width, INT &draw_height);
        static GX_RECTANGLE CalculateTextFitSize(widget_info *info, int display_index);
        BOOL CheckResizeToText(widget_info *info, int resize_mode = RESIZE_MODE_ALL);
        BOOL CheckResizeToPixelmap(widget_info *info, int resize_mode = RESIZE_MODE_ALL, BOOL DontShrink = FALSE);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);

        static void WriteStringId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid);
        static void WriteFontId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid);
        static void WritePixelmapId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid);
        static void WriteColorId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid);

        static GX_RESOURCE_ID ReadStringId(xml_reader &reader, studiox_project *project, int display, char *tag);
        static GX_RESOURCE_ID ReadFontId(xml_reader &reader, studiox_project *project, int display, char *tag);
        static GX_RESOURCE_ID ReadPixelmapId(xml_reader &reader, studiox_project *project, int display, char *tag);
        static GX_RESOURCE_ID ReadColorId(xml_reader &reader, studiox_project *project, int display, char *tag);

        virtual void SaveToProject(xml_writer &, studiox_project *project, int display, widget_info *);
        virtual void ReadFromProject(xml_reader &, studiox_project *projet, int display, widget_info *info, ULONG valid_styles);
        virtual void Reset() {};

        void MakeCreatePreamble(char *name, int version, CString &out);

        virtual void AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id) {}
        virtual void AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id) {}
        virtual void AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id);
        virtual void AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id) {}
        virtual void Autosize(widget_info *info) {}

        static int GetBorderWidth(DWORD style);
};

#endif
