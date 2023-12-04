

#pragma once

#include <afxcview.h>

#define PROJECT_VIEW_SEARCH_DEPTH 65535

class copy_paste_info;

class project_view : public CTreeView
{
    protected:
        DECLARE_DYNCREATE(project_view)

    public:
        project_view();
        ~project_view();
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        virtual void OnInitialUpdate();
        void NewProject(CWnd *parent = NULL);
        void ProjectRenamed();
        void TerminateAppExecutions();
        void CloseProject(BOOL DisplayRecent = FALSE, BOOL CheckSaveChanges = TRUE);
        void OpenProject();
        BOOL OpenProject(CString &path, CString &name);
        void MergeProject();
        void MergeProject(CString &path, CString &name);
        void OpenHistoryProject(int Index);
        void DeSelect();
        void OnProjectReset();
        void NotifyNameChange(widget_info *info);
        void SelectFirstScreen();
        void SelectDisplay(int display);

        void RunApplication(CWnd *parent = NULL);

        int GetActiveDisplay() {return m_active_display;}
        template <class T>
        BOOL check_set_active_display(T *info);
        folder_info *GetRootFolder();
        BOOL IsDisplaySelected();
        BOOL IsFolderSelected();
        INT GetSelectedNodeLevel();
        folder_info *GetSelectedFolder();

        // Insert a top-level widget
        void InsertTopLevelWidget(widget_info *info);    // for top level widget
        void InsertFolder(folder_info *info);

        void DeleteWidget(widget_info *info);
        void DeleteFolder(folder_info *info);


        // this version is used to add a child widget to a specified node
        widget_info *InsertTreeChild(HTREEITEM hParent, widget_info *info, BOOL do_siblings = TRUE);

        // this version is used to add a child widget to selected node
        widget_info *InsertTreeChild(widget_info *parent, widget_info *info, BOOL do_siblings = TRUE);
        folder_info *InsertTreeChild(folder_info *folder, widget_info *info, BOOL do_sibling = TRUE);

        
        // this function is called by project view or target screen to see
        // if we need to do anything special after modifying a widget.
        static void CheckParentRefresh(GX_WIDGET *parent);
        static void CheckParentRefresh(widget_info *parent_info);
        static void UpdateChildPositions(widget_info *parent);

        void WidgetSelected(widget_info *widget);
        void FolderSelected(INT display_index,  CString folder_name);
        void FolderSelected(folder_info * folder);

        afx_msg void OnDeleteWidget();
        afx_msg void OnInsertFolder();
        afx_msg void OnEditFolderProperties();
        afx_msg void OnCutWidget();
        afx_msg void OnCopyWidget();
        afx_msg void OnPasteWidget();
        afx_msg void OnConfigureDisplays();
        afx_msg void OnConfigureThemes();
        afx_msg void OnGenerateAll();
        afx_msg BOOL OnGenResources(GX_BOOL show_notify = TRUE);
        afx_msg void OnGenApplication(GX_BOOL show_notify = TRUE);
        afx_msg BOOL OnGenBinary(GX_BOOL show_notify = TRUE);

        BOOL CheckRemoveDuplicateTemplates(folder_info *start, folder_info *search_start = NULL, BOOL notify_duplicates = FALSE);
        void AddWidgetsToFolder(folder_info *target_folder, folder_info *src_folder);
        folder_info *CombineCopyPasteWidgets(copy_paste_info *pasted_info);
        void PasteFolder(copy_paste_info *pasted_info);
        void PasteWidgets(folder_info *parent_folder, copy_paste_info *pasted_info);
        void PasteWidgets(widget_info *parent_widget_info, copy_paste_info *pasted_info);

        void MoveSelectedToFront();
        void MoveSelectedToBack();

        /* Retrieve project tree status, used for macro record and playback.  */
        void GetTreeState(CArray<int> *state_array);
        void GetTreeState(HTREEITEM hParent, CArray<int> *state_array);

        /* Set project tree status, used for macro record and playback.  */
        void SetTreeState(CArray<int> *state_array);
        void SetTreeState(HTREEITEM hParent, CArray<int> *state_array);

        widget_info *GetTemplateInfo(int menu_index);
        void UpdateBaseMenu(CMenu *parent);
        BOOL CreateBaseMenu(CMenu *pBaseMenu);
        GX_BOOL IsTopLevelWidget(widget_info *widget_info);//check if a widget is in top level
        void GetTopLevelWidgetList(int display_index, CArray<widget_info *> *list);
        widget_info *FindTopLevelWidget(widget_info *child);
        GX_BOOL IsFolderNameExist(CString name);
        GX_BOOL IsFolderNameExist(folder_info *folder, CString name);
        void CreateUniqeFolderName(folder_info *info);
        BOOL FindWidgetIdName(widget_info *start, widget_info *except, CString id_name);
        BOOL CheckTriggerReference(screen_flow *flow, flow_item *item, widget_info *screen_info, widget_info *info, widget_info *search);
        VOID RemoveWidgetIdFromDictionary(studiox_project *project, int display, widget_info *info);
        BOOL CheckRemoveWidget(studiox_project *project, int display, widget_info *info);
        VOID OnLockUlockWidgetPositions();

    // Generated message map functions
    protected:

        afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC *pDC);
        afx_msg void OnRClick(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnContextMenu(CWnd *pWnd, CPoint pos);
        afx_msg LRESULT OnTestMessage(WPARAM, LPARAM);

        afx_msg void OnNodeSelect(NMHDR *, LRESULT *);
        afx_msg void OnItemExpanded(NMHDR *ph, LRESULT *pr);
        virtual BOOL PreTranslateMessage(MSG* pMsg);

        DECLARE_MESSAGE_MAP()

    private:
        HTREEITEM AddTreeRoot(CString &rootname);
        void PopulateProjectTree(studiox_project *pProj);
        void SetFrameTitle(CString &title);
        void AssignTreeImage(HTREEITEM item, widget_info *info);
        void AssignTreeImage(HTREEITEM item, folder_info *info, BOOL open = FALSE);

        void AddTreeWidgets(HTREEITEM hParent, widget_info *start);
        void AddTreeFolders(HTREEITEM hParent, folder_info *start);
        int GetTreeNodeLevel(HTREEITEM item);
        HTREEITEM FindWidget(widget_info *widget);
        HTREEITEM FindWidget(HTREEITEM start, widget_info *widget);
        HTREEITEM FindProjectTreeNode(HTREEITEM start, CString name, int search_depth = PROJECT_VIEW_SEARCH_DEPTH);
        HTREEITEM FindProjectTreeNode(CString name, CString display_name = _T(""), CString screen_name = _T(""));
        void DisplaySelected();

        int        m_active_display;
        CTreeCtrl *mpTree;
        widget_info *m_widget_copy;
        CImageList *mpImageList;
};

