

class screen_generator : public studio_source_writer {
    public:
        screen_generator(studiox_project *project);
        ~screen_generator();

        BOOL GenerateScreens(int display);
        void WriteErrorDirectives();

        CString GetColorIdName(int color_id) const;
        static CString GetColorIdName(studiox_project *proj, int display, int color_id);
        CString GetFontIdName(int font_id) const;
        static CString GetFontIdName(studiox_project *proj, int display, int font_id);
        CString GetStringIdName(int string_id, widget_info *info = NULL) const;
        static CString GetStringIdName(studiox_project *proj, int display, int string_id, widget_info *info = NULL);
        CString GetPixelmapIdName(int pixelmap_id, int frame_id = 0) const;
        static CString GetPixelmapIdName(studiox_project *proj, int display, int pixelmap_id, int frame_id = 0);

        CString GetString(int string_id, int language = 0) const;

        // style flag writers

        static BOOL IsTextType(widget_info *info);
        static void AddButtonStyles(widget_info *info, CString &out);
        static void AddButtonAlignmentStyles(widget_info *info, CString &out);
        static void AddListStyles(widget_info *info, CString &out);
        static void AddSliderStyles(widget_info *info, CString &out);
        static void AddProgressBarStyles(widget_info *info, CString &out);
        static void AddSpriteStyles(widget_info *info, CString &out);
        static void AddScrollbarStyles(widget_info *info, CString &out);
        static void AddScrollbarStyles(ULONG style, CString &out);
        static void AddPixelmapSliderStyles(widget_info *info, CString &out);
        static void AddDropListStyles(widget_info *info, CString &out);
        static void AddTextAlignStyle(widget_info *info, CString &out);
        static void AddWindowStyles(widget_info *info, CString &out);
        static void AddTextScrollWheelStyles(widget_info *info, CString &out);
        static void AddWidgetScrollWheelStyles(widget_info* info, CString& out);
        static void AddTreeViewStyles(widget_info *info, CString &out);
        static void AddTextInputStyles(widget_info *info, CString &out);
        static void GetAnimationStyles(GX_ANIMATION_INFO *info, CString &out);

        CString GetScreenNamePrefix(){ return m_screen_name_prefix; };

    private:
        studiox_project *m_project;
        int m_display;
        int m_widget_id;
        CString m_screen_name_prefix;
        CString m_output_filepath;
        CArray<CString> name_list;
        CArray<CFile *> output_file_list;
        CString m_strings_fit_msg;

        CString GetWidgetTypeName(widget_info *info);

        BOOL    IsWidgetUsed(int widget_type) const;
        BOOL    IsWidgetUsed(widget_info *start, int widget_type) const;

        // functions for generating header file
        void GenerateScreenHeader();
        BOOL TypedefControlBlock(folder_info *folder);
        void PrototypeCallbacks(widget_info *info);
        BOOL CheckDependencies(widget_info *info, BOOL top_level = FALSE) const;
        void DeclareScreenFlowStructures();

        void DeclareBaseMembers(widget_info *info);
        void DeclareChildMembers(CString &parent_name, widget_info *info);

        // functions for generating source file
        void GenerateDisplayConfigBlock();
        void DeclareDisplayControlBlock();
        void GenerateScreenSource();
        void WriteWidgetsInfo();
        void DeclareControlBlocks(widget_info *info);
        void DeclareControlBlocks();
        void WriteExternWidget(widget_info *&info);
        void WriteExternWidgetsUsedByScreenFlow(folder_info *folder, int display_index);
        void WriteExternWidgetsToDefaultFile();
        void WriteExternWidgetsUsedByScreenFlow();
        void DeclareWidgetTable();
        void DeclareDisplayTable();
        void WriteExternBaseDefines();
        void WriteExternBaseDefines(folder_info *parent_folder);
        ULONG CalculateCanvasSizeBytes(int display);
        bool SetOutFile(CString &outfile);
        BOOL ProjectHasScreenFlow();
        CString GetTempateEventFuncName(widget_info *info);
        CString GetWidgetReferenceName(widget_info *info);
        BOOL CheckWriteScreenFlow(int display, widget_info *info);
        void GenerateActionListName(flow_item *item, trigger_info *trigger, CString &name);
        void GenerateAnimationInfo(flow_item *item, trigger_info *trigger, action_info *action, int index);
        void GenerateAnimationInfoName(flow_item *item, CString &name, int index);
        void WriteEventTable(flow_item *item);
        void WriteAutoEventHandler();

        void WriteWidgetType(widget_info *info);
        void WriteWidgetId(widget_info *info);
        void WriteWidgetUserData(widget_info *info);
        void WriteWidgetStyleFlags(widget_info *info);
        void WriteWidgetStatusFlags(widget_info *info);
        void WriteWidgetControlBlockSize(widget_info *info, BOOL bTop = FALSE);
        void WriteWidgetColors(widget_info *info);
        void WriteWidgetSize(widget_info *info);
        void WriteCreateFunctionName(widget_info *info);
        void WriteWidgetOverrides(widget_info *info);
        void WriteCreateFunctions();


        void WriteChildDefine(CString &parent_name, widget_info *info);
        void WriteParentDefine(widget_info *info);
        void WriteWidgetIds(widget_info *info);
        void WriteWidgetIds(folder_info *folder);
        void WriteWidgetProperties(CString &prefix, widget_info *info, BOOL TopLevel = FALSE);

        void WriteCreateFromResourceFunctions();
        void WriteDisplayConfigure();

        // functions specifically for Synergy targets:
        void WriteDave2D8bppPaletteSetup();
        void WriteDave2D8bppPaletteRotatedSetup();

        void DeclareSynergyDriverFunctions(void);
        void WriteDave2D16_32_Setup(UINT color_format);
        void WriteDave2D16_32_RotatedSetup(UINT color_format);
        void WriteDave2DSetupFunction(int display);
};