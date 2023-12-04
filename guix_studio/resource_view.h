

#pragma once

#include <afxcview.h>

class resource_view_provider;
struct IMAGE_INFO;

struct COLOR_RECORD {
    char *name;
    int color_id;
    ULONG rgb_val;
};

struct FONT_RECORD {
    char *name;
    int  font_id;
    GX_FONT *font;
};

struct PIXELMAP_RECORD {
    char *name;
    int  pixelmap_id;
    BOOL include_alpha;
    IMAGE_INFO *image_info;
};

struct font_table {
    GX_FONT **fonts;
    int     num_fonts;
};

struct color_table {
    GX_COLOR *colors;
    int      num_colors;
};

struct pixelmap_table {
    GX_PIXELMAP **maps;
    int     num_maps;
};

class resource_view : public CScrollView
{
    protected:
        DECLARE_DYNCREATE(resource_view)

    public:
        resource_view();
        ~resource_view();
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	    virtual void OnDraw(CDC* pDC);

        void OnCloseProject();
        void OnOpenProject();
        void OnDisplaySelect(int DisplayIndex, BOOL bReload = FALSE);
        
        void OnTreeSizeChange();
        virtual void OnInitialUpdate();
        void RebuildStringItems(BOOL update_GUIX = TRUE);
        void UpdateStringTableFonts(FontCharMap *map, res_info *start = NULL);
        static GX_COLOR GetNativeColor(GX_COLOR rgb_color, int color_format);
        static COLORREF GetColorRef(GX_COLOR gx_color, int color_format, GX_COLOR *palette, int palsize);

        //these functions are used in macro record and macro playback
        //to save and reload the layout of resource view
        BOOL IsOpen(int res_id);
        void SetResFolderStatus(int res_id, int open_status);

        int  PasteResource(res_info *info, int conflict_option = 0);
        int  PasteResource(string_table_record &record, int conflict_option = 0);
        void FinalizePasteResources();
        void InstallResources(int DisplayIndex);
        void BuildResourceTables(int DisplayIndex, GX_DISPLAY *display, BOOL update_resource_view = GX_TRUE);
        void CreateUniqueResourceName(res_info *info);
        void SetCurrentItem(resource_item* item);
        resource_item* GetCurrentItem() { return mpCurrentItem; }

        static void CleanupDisplayResources(GX_DISPLAY *display);

        resource_tree* GetResTree() { return mpTree; }
        resource_view_provider* GetResViewProvider();
        static int CreateUniqueId();
        void ScrollIntoView(resource_item *item);
        void TaskAddPixelmaps(resource_item *parent, CStringArray *patharray);
        void TaskInitializePixelmaps(resource_item *item);

    // Generated message map functions
    protected:
        void Scroll(int delta, BOOL redraw = TRUE);
        void InsertTreeResources(resource_item *parent, res_info *start);
        void AddStringItems(resource_item *parent);

        void InstallColorTable(int DisplayIndex, GX_DISPLAY *display = NULL);
        void InstallFontTable(int DisplayIndex, GX_DISPLAY *display = NULL);
        void InstallPixelmapTable(int DisplayIndex, GX_DISPLAY *display = NULL);
        void CheckInstallPalette(int DisplayIndex, GX_DISPLAY *display);


        void OnSetActiveLanguage(int delta);
        void OnSetActiveTheme(int delta);

        int BuildColorTable(int DisplayIndex, int ThemeIndex, int color_format, color_table *table);
        int BuildFontTable(int DisplayIndex, int ThemeIndex, font_table *table);
        int BuildPixelmapTable(int DisplayIndex, int ThemeIndex, pixelmap_table *table);
        void RestoreStringIdsFromBackup(widget_info *write, widget_info *read);
        void RestoreStringIdsFromBackup(folder_info *write, folder_info *read);

        void PopupMenu(int MenuId, CPoint &pos);

        afx_msg BOOL OnEraseBkgnd(CDC *pDC);
        afx_msg int OnCreate(LPCREATESTRUCT lpcs);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnLButtonDown(UINT di, CPoint cp);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

        afx_msg void OnAddCustomColor();
        afx_msg void OnEditCustomColor();
        afx_msg void OnEditSystemColor();

        afx_msg void OnAddFont();
        afx_msg void OnEditFont();
        afx_msg void OnEditStringTable();
        afx_msg void OnEditLanguages();
        afx_msg void OnDeleteCustomColor();
        afx_msg void OnDeleteFont();

        afx_msg void OnAddPixelmaps();
        afx_msg void OnEditPixelmaps();
        afx_msg void OnAddPixelmapFolder();
        afx_msg void OnRemovePixelmapFolder();
        afx_msg void OnRenamePixelmapFolder();
        afx_msg void OnEnablePixelmapFolder();
        afx_msg void OnDisablePixelmapFolder();
        afx_msg void OnEditPixelmap();
        afx_msg void OnDeletePixelmap();
        afx_msg void OnEnablePixelmap();
        afx_msg void OnDisablePixelmap();
        afx_msg void OnGenerateXML();
        afx_msg void OnTimer(UINT_PTR nIDEvent);
        afx_msg LRESULT OnRebuildStringItems(WPARAM, LPARAM);
        afx_msg LRESULT OnUpdateStringTableFonts(WPARAM, LPARAM);
        afx_msg LRESULT OnOpenResourceItem(WPARAM, LPARAM);
        afx_msg LRESULT OnTestMessage(WPARAM, LPARAM);
        afx_msg void OnSetFocus(CWnd* pOldWnd);
        afx_msg void OnKillFocus(CWnd* pNewWnd);
        afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

        BOOL OnReturnKey(int item_type);
        BOOL OnTabKey(int item_type);
        BOOL OnDirectionKey(int item_type, int key);
        void OnPageDonw(int item_type);
        void OnPageUp(int item_type);
        void OnHome(int item_type);
        void OnEnd(int item_type);
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

        void SelectNextItem();
        void SelectPreviousItem();

        void AddColor(GX_COLOR color, CString name, resource_item *parent);

        void AddPixelmap(resource_item *parent, res_info *info, BOOL palette_mode);
        void AddPixelmaps(resource_item *parent, CStringArray *patharray);
        void InitializePixelmaps(resource_item *item);
        void EditPixelmaps(resource_item *item);
        void AddFont(resource_item *parent, res_info *info);
        void EditFont(resource_item *item, CString &old_font_path);

        res_info *InitFontInfo();

        DECLARE_MESSAGE_MAP()

        int mDisplayIndex;

        resource_tree *mpTree;
        resource_item *mpCurrentItem;
        resource_item *mpDragItem;
        GX_COLOR      *mpPalette;
        int            mTimerTicks;
        BOOL           mNewResourcePasted;

        resource_view_provider* m_pResViewProvider;

    private:
        void DeleteResourceChildren(res_info *parent);
        void SyncResourceName(CString &old_name, res_info *info);
        void SyncResourceAdd(res_info *info);
        void SyncResourceDelete(res_info *info);
        void SyncResourcePath(res_info *info);
        void ChangeItemParent(resource_item *item, resource_item *new_parent);
        void EnableDisableChild(res_info *parent, int target_child_type, GX_BOOL enable);
        int  GetFolderEnableDisableState(res_info* parent);
};