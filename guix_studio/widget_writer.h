

class widget_writer {
    public:
        static void WriteWidgets(xml_writer &writer, studiox_project *project, int display_index, widget_info *start, BOOL DoSiblings = TRUE);
        static void WriteWidgetFolders(xml_writer &writer, studiox_project *project, int display_index, folder_info *start, BOOL DoSiblings = TRUE);
};