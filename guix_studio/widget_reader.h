

class widget_reader {
    public:
        static widget_info *ReadOneWidget(xml_reader &reader, studiox_project *project, int display);
        static void ReadChildWidgets(xml_reader &reader, studiox_project *project, int display, widget_info *parent);
        static void ReadWidgets(xml_reader &reader, studiox_project *project, int display, folder_info *widgets_folder);
        static void ReadWidgetFolders(xml_reader &reader, studiox_project *project, int display);
};
