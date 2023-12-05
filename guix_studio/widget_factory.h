

#ifndef _WIDGET_FACTORY_
#define _WIDGET_FACTORY_

class widget_factory {
    public:
        static BOOL GenerateWidgets(GX_WIDGET *parent, widget_info *start, BOOL save_ptrs = TRUE, BOOL do_siblings = TRUE);
        //static BOOL GenerateWidgets(folder_info *start, BOOL save_ptrs = TRUE);
        static GX_WIDGET *GenerateAppScreen(GX_WIDGET *root, widget_info *child);

        static void CleanupWidgets(widget_info *start, BOOL recursive = FALSE, BOOL do_siblings = FALSE);
        static void CleanupWidgets(folder_info *folder);
        static void DeleteWidgets(GX_WIDGET *widget);
        static void DeleteOneWidget(GX_WIDGET *widget);

        //static void MoveWidget(GX_WIDGET *widget, GX_RECTANGLE &size);
        static void MoveWidget(widget_info *info, GX_RECTANGLE &size);

        static void CreateUniqueAppNames(widget_info *start, widget_info *search_also = NULL, widget_info *search_start = NULL, BOOL is_top_level_widget = FALSE);
        static BOOL FindAppName(widget_info *start, widget_info *find, BOOL search_child = TRUE);
        static BOOL FindAppName(widget_info *start, CString &find_name, BOOL search_child = TRUE);
        static BOOL FindAppName(folder_info *folder, CString &find_name, BOOL search_child = TRUE);
        static BOOL FindAppName(folder_info *folder, widget_info *find, BOOL search_child = TRUE);
        static BOOL FindBaseName(CString &find_name);

        static CString WidgetTypeToString(int type);
        static int WidgetStringToType(CString &name);

        static void InitServiceProviders();     // called once at startup
        static void DeleteServiceProviders();   // called once at shutdown
        static void ResetServiceProviders(int provider_type = -1);        // called when project is closed

        static widget_service_provider *GetServiceProvider(int type);
        static int GetWidgetType(int index);
        static void ReplaceScrollbar(widget_info *info, GX_SCROLLBAR_APPEARANCE *appearance);
        static void CheckReplaceSpritePixelmaps(widget_info *info, CArray<GX_RESOURCE_ID> &resource_id_array);

        static void UpdateSpriteFrameInfos(folder_info *folder, GX_RESOURCE_ID map_id);
        static void UpdateSpriteFrameInfos(widget_info *info, GX_RESOURCE_ID map_id);
        static int  CalculateResourceIdDecrementCount(CArray<GX_RESOURCE_ID>& resource_id_array, GX_RESOURCE_ID resource_id);
        static void DecrementWidgetResourceIds(int res_type, widget_info *info, CArray<GX_RESOURCE_ID> &resource_id_array);
        static void DecrementWidgetResourceIds(int res_type, folder_info *folder, GX_RESOURCE_ID resource_id);
        static void DecrementWidgetResourceIds(int res_type, folder_info* folder, CArray<GX_RESOURCE_ID> &resource_id_array);
        static int CountReferences(int res_type, widget_info *info, GX_RESOURCE_ID resource_id, int seed_count, bool skip_template = false);

        static int CountReferences(studiox_project *project, int res_type, GX_RESOURCE_ID resource_id);

        static void UpdateInputWidgetText(widget_info *info, GX_RESOURCE_ID resource_id);
        static void UpdateInputWidgetText(studiox_project *project, GX_RESOURCE_ID resource_id = -1);
        static void StringDeleted(studiox_project *project, GX_RESOURCE_ID string_id);

    private:
        widget_factory();
        static void UpdateWidgetInfoPositions(widget_info *info, int xshift,int yshift, BOOL do_next);
        static void PositionPopupList(widget_info *info, GX_DROP_LIST *drop);
};

#endif