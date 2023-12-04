
#include "studiox_includes.h"
#include "copy_paste_engine.h"
#include "app_runner.h"
#include "folder_name_dlg.h"
#include "gx_win32_studio_display_driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern INI_INFO        StudioXIni;
extern CString         studiox_version_string;
extern CArray<WIDGET_SELECT_ENTRY> SelectedWidgets;

IMPLEMENT_DYNCREATE(project_view, CTreeView)

enum project_view_test_commands{
    TEST_OPEN_PROJECT = 1,
    TEST_CLOSE_PROJECT,
    TEST_IMPORT_PROJECT,
    TEST_SELECT_PROJECT_TREE_NODE,
    TEST_SELECT_PROJECT_FOLDER,
    TEST_SELECT_CHILD_WIDGET,
    TEST_SELECT_MULTI_WIDGETS,
    TEST_DELETE_WIDGET,
    TEST_GET_WIDGET_LEFT,
    TEST_GET_WIDGET_TOP,
    TEST_GET_WIDGET_RIGHT,
    TEST_GET_WIDGET_BOTTOM,
    TEST_INSERT_FOLDER,
    TEST_EDIT_FOLDER_PROPERTIES,
    TEST_DELETE_FOLDER,
    TEST_TERMINATE_APP_EXECUTION
};

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(project_view, CTreeView)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CONTEXTMENU()
    ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnNodeSelect)
    ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)
    ON_COMMAND(ID_EDIT_FOLDER_PROPERTIES, OnEditFolderProperties)
    ON_COMMAND(ID_LOCK_WIDGET_POSITIONS, OnLockUlockWidgetPositions)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

typedef struct {
    int widget_type;
    int bmp_id;
} WIDGET_TYPE_IMAGE_MAP ;

WIDGET_TYPE_IMAGE_MAP ProjectViewImageMap[] = {
    { -1,   IDB_PROJECT},
    { -1,   IDB_DISPLAY},
    { -1,   IDB_FOLDER_CLOSE},
    { -1,   IDB_FOLDER_OPEN},
    { GX_TYPE_WINDOW, IDB_WINDOW},
    { GX_TYPE_DIALOG, IDB_DIALOG},
    { GX_TYPE_HORIZONTAL_LIST, IDB_HORZ_LIST},
    { GX_TYPE_VERTICAL_LIST,   IDB_VERT_LIST},
    { GX_TYPE_DROP_LIST,       IDB_DROP_LIST},
    { GX_TYPE_BUTTON, IDB_BUTTON},
    { GX_TYPE_CHECKBOX, IDB_CHECKBOX},
    { GX_TYPE_RADIO_BUTTON, IDB_RADIO_BUTTON},
    { GX_TYPE_ICON, IDB_ICON },
    { GX_TYPE_ICON_BUTTON, IDB_ICON_BUTTON},
    { GX_TYPE_TEXT_BUTTON, IDB_TEXT_BUTTON},
    { GX_TYPE_MULTI_LINE_TEXT_BUTTON, IDB_MULTI_LINE_TEXT_BUTTON},
    { GX_TYPE_PIXELMAP_BUTTON, IDB_PIXELMAP_BUTTON},
    { GX_TYPE_SHADOW_BUTTON, IDB_SHADOW_BUTTON},
    { GX_TYPE_SPRITE, IDB_SPRITE },
    { GX_TYPE_PROMPT, IDB_PROMPT},
    { GX_TYPE_MULTI_LINE_TEXT_VIEW, IDB_MULTILINE_PROMPT},
    { GX_TYPE_MULTI_LINE_TEXT_INPUT, IDB_MULTILINE_TEXT_INPUT },
    { GX_TYPE_PIXELMAP_PROMPT, IDB_PIXELMAP_PROMPT},
    { GX_TYPE_PIXELMAP_TEXT_INPUT, IDB_PIXELMAP_TEXT_INPUT},
    { GX_TYPE_SINGLE_LINE_TEXT_INPUT, IDB_TEXT_INPUT},  
    { GX_TYPE_HORIZONTAL_SCROLL, IDB_HORZ_SCROLL},
    { GX_TYPE_VERTICAL_SCROLL, IDB_VERT_SCROLL},
    { GX_TYPE_SLIDER, IDB_SLIDER},
    { GX_TYPE_PIXELMAP_SLIDER, IDB_PIXELMAP_SLIDER},
    { GX_TYPE_PROGRESS_BAR, IDB_PROGRESS_BAR},
    { GX_TYPE_RADIAL_PROGRESS_BAR, IDB_RADIAL_PROGRESS_BAR},
    { GX_TYPE_CIRCULAR_GAUGE, IDB_GAUGE},
    { 0, 0}
};

///////////////////////////////////////////////////////////////////////////////
project_view::project_view()
{
    mpImageList = NULL;
    m_active_display = -1;
    m_widget_copy = NULL;
}

///////////////////////////////////////////////////////////////////////////////
project_view::~project_view()
{
    studiox_project *project = GetOpenProject();
    if (project)
    {
        delete project;
        SetOpenProject(NULL);
    }


    if (mpImageList)
    {
        delete mpImageList;
    }

    if (m_widget_copy)
    {
        delete m_widget_copy;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::PreCreateWindow(CREATESTRUCT& cs)
{
    
    if( !CTreeView::PreCreateWindow(cs) )
        return FALSE;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
HTREEITEM project_view::AddTreeRoot(CString &rootname)
{
   /* setup initial tree view */

    TVINSERTSTRUCT tvInsert;
    CString fullname = rootname;
    TCHAR *ps = fullname.GetBuffer(1);

    tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = NULL;
    tvInsert.item.mask = TVIF_TEXT;
    tvInsert.item.pszText = ps;
    tvInsert.item.iImage = 0;
    HTREEITEM hroot = mpTree->InsertItem(&tvInsert);
    fullname.ReleaseBuffer();
    return hroot;
}

///////////////////////////////////////////////////////////////////////////////
CImageList *CreateImageList(int width, int height)
{
    CImageList *list = new CImageList();
    list->Create(width, height, ILC_COLOR24, 0, 0);
    
    WIDGET_TYPE_IMAGE_MAP *entry = ProjectViewImageMap;

    while(entry->bmp_id)
    {
        CBitmap *bmp = new CBitmap();
        bmp->LoadBitmap(entry->bmp_id);
        list->Add(bmp, (CBitmap *) NULL);
        delete bmp;
        entry++;
    }
    return list;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnInitialUpdate()
{
    /* inform the parent frame of my instance for later access */
    CMainFrame *frame = (CMainFrame *) GetParentFrame();
    frame ->SetProjectView(this);
    frame->SetActiveView(this);
    SetWindowText(_T("Project View"));

    mpTree = &GetTreeCtrl();
    mpTree->DeleteAllItems();

    SetControlAccessibleName(mpTree->GetSafeHwnd(), _T("project_view"));

    SetFrameTitle(CString(_T("No Project")));
    HTREEITEM hroot = AddTreeRoot(CString(_T("No Project")));
//    mpTree->ModifyStyle(0, TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS);
    mpTree->ModifyStyle(0, TVS_HASLINES|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);

    // create an image list:
    if (mpImageList)
    {
        delete mpImageList;
        mpImageList = NULL;
    }
    mpImageList = CreateImageList(16, 16);
    mpTree->SetImageList(mpImageList, TVSIL_NORMAL);
    mpTree->SetIndent(0);
}

///////////////////////////////////////////////////////////////////////////////
void project_view::AssignTreeImage(HTREEITEM item, widget_info *info)
{
    WIDGET_TYPE_IMAGE_MAP *entry = ProjectViewImageMap;
    int index = 0;

    while(entry->bmp_id)
    {
        if (entry->widget_type == info->basetype)
        {
            mpTree->SetItemImage(item, index, index);
            break;
        }
        index++;
        entry++;
    }
}


///////////////////////////////////////////////////////////////////////////////
void project_view::AssignTreeImage(HTREEITEM item, folder_info *info , BOOL expaned)
{
    WIDGET_TYPE_IMAGE_MAP *entry = ProjectViewImageMap;
    int index = 0;
    if (expaned)
    {
        while (entry->bmp_id)
        {
            if (entry->bmp_id == IDB_FOLDER_OPEN)
            {
                mpTree->SetItemImage(item, index, index);
                break;
            }
            index++;
            entry++;
        }
    }
    else
    {
        while (entry->bmp_id)
        {
            if (entry->bmp_id == IDB_FOLDER_CLOSE)
            {
                mpTree->SetItemImage(item, index, index);
                break;
            }

            index++;
            entry++;
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
void project_view::AddTreeFolders(HTREEITEM hParent, folder_info *start)
{
    while (start)
    {
        HTREEITEM hCurrent = mpTree->InsertItem(start->folder_name,
            hParent, TVI_SORT);
        mpTree->SetItemData(hCurrent, (DWORD_PTR)start);
        AssignTreeImage(hCurrent, start);

        AddTreeWidgets(hCurrent, start->GetFirstChildWidget());

        start = start->GetNextFolder();
    }
}


///////////////////////////////////////////////////////////////////////////////
void project_view::AddTreeWidgets(HTREEITEM hParent, widget_info *start)
{
    CString tree_name;

    while (start)
    {
        if (!start->app_name.IsEmpty())
        {
            tree_name = start->app_name;
        }
        else
        {
            tree_name = start->base_name;
        }
        HTREEITEM hCurrent = mpTree->InsertItem(tree_name,
            hParent, TVI_SORT);
        mpTree->SetItemData(hCurrent, (DWORD_PTR) start);
        AssignTreeImage(hCurrent, start);

        if (start->GetChildWidgetInfo())
        {
            AddTreeWidgets(hCurrent, start->GetChildWidgetInfo());
        }
        start = start->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::SelectFirstScreen()
{
    HTREEITEM hroot = mpTree->GetRootItem();

    if (hroot)
    {
        // Select the first display:
        HTREEITEM hdisplay = mpTree->GetChildItem(hroot);

        if (hdisplay)
        {
            HTREEITEM hfolder = mpTree->GetChildItem(hdisplay);
            if (hfolder)
            {
                HTREEITEM hscreen = mpTree->GetChildItem(hfolder);
                if (hscreen)
                {
                    mpTree->SelectItem(hscreen);
                }
                else
                {
                    mpTree->SelectItem(hfolder);
                }
            }
            else
            {
                mpTree->SelectItem(hdisplay);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::RunApplication(CWnd *parent)
{
    studiox_project *project = GetOpenProject();
    display_info *display;

    if (project)
    {
        int display_index = GetActiveDisplay();
        display = &project->mDisplays[display_index];
        if (display->screenflow)
        {
            app_runner runner;
            runner.RunApplication(display_index, parent);
        }
        else
        {
            ErrorMsg("Screen flow diagram is not been configured, cannot run application", parent);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::SelectDisplay(int display)
{
    HTREEITEM hroot = mpTree->GetRootItem();

    if (hroot)
    {
        // Select the first display:
        HTREEITEM hdisplay = mpTree->GetChildItem(hroot);

        while (hdisplay)
        {
            int index = mpTree->GetItemData(hdisplay);

            if (index == display)
            {
                mpTree->SelectItem(hdisplay);
                break;
            }
            hdisplay = mpTree->GetNextSiblingItem(hdisplay);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::PopulateProjectTree(studiox_project *pProj)
{
    HTREEITEM hCurrent;
    int DisplayIndex = 0;
    mpTree->DeleteAllItems();
   
    if (pProj == NULL)
    {
        return;
    }

    for (DisplayIndex = 0; DisplayIndex < pProj->mHeader.max_displays; DisplayIndex++)
    {
        widget_factory::CleanupWidgets(pProj->mDisplays[DisplayIndex].GetFirstChildFolder());
    }

    // Add the root node:
    HTREEITEM hroot = AddTreeRoot(pProj->mHeader.project_name);

    // Add the display nodes:

    for (DisplayIndex = 0; DisplayIndex < pProj->mHeader.num_displays; DisplayIndex++)
    {
        hCurrent = mpTree->InsertItem(pProj->mDisplays[DisplayIndex].name,
            hroot, TVI_SORT);
        mpTree->SetItemData(hCurrent, DisplayIndex);
        mpTree->SetItemImage(hCurrent, 1, 1);

        AddTreeFolders(hCurrent, pProj->mDisplays[DisplayIndex].GetFirstChildFolder());
        //widget_factory::GenerateWidgets(pProj->mDisplays[DisplayIndex].GetFirstChildFolder());
    }
    mpTree->Expand(hroot, TVE_EXPAND);
}   

///////////////////////////////////////////////////////////////////////////////
void project_view::OnPaint()
{
    CTreeView::OnPaint();
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::OnEraseBkgnd(CDC *pDC)
{
     return CTreeView::OnEraseBkgnd(pDC);
}
template <class T>
///////////////////////////////////////////////////////////////////////////////
BOOL project_view::check_set_active_display(T *info)
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        int display_index = project->GetDisplayIndex(info);

        if (display_index >= 0)
        {
            if (display_index != m_active_display)
            {
                //Cleanup current display resources
                GetResourceView()->CleanupDisplayResources(get_target_view_display());

                m_active_display = display_index;
                GetTargetScreen()->ConfigureDisplay(display_index);
                GetResourceView()->OnDisplaySelect(display_index);
                return TRUE;
            }
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnItemExpanded(NMHDR *pNotifyStruct, LRESULT *pr)
{
    NM_TREEVIEW *pTree = (NM_TREEVIEW *)pNotifyStruct;
    TV_ITEM tItem = pTree->itemNew;
    HTREEITEM hItem = tItem.hItem;

    if (hItem)
    {
        if (GetTreeNodeLevel(hItem) == FOLDER_NODE_LEVEL)
        {
            folder_info *folder = (folder_info *)mpTree->GetItemData(hItem);
            UINT style = mpTree->GetItemState(hItem, TVIS_EXPANDED);
            /* */ 
            if (style & TVIS_EXPANDED)
            {
                AssignTreeImage(hItem, folder, TRUE);
            }
            else 
            {
                AssignTreeImage(hItem, folder);
            }

            /* Force project view to redraw here to make the "+" shown.
            Sometime the "+" isn't shown when generally running,
            but it is shown when debug. So force it redraw here.*/
            Invalidate();
            UpdateWindow();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnNodeSelect(NMHDR *ph, LRESULT *pr)
{
    studiox_project *project = GetOpenProject();
    if (!project)
    {
        return;
    }
    HTREEITEM hItem = mpTree->GetSelectedItem();
    widget_info *info;
    folder_info *folder;
    GX_WINDOW_ROOT *root;
    int display_index;
    
    switch(GetTreeNodeLevel(hItem))
    {
    case PROJECT_NODE_LEVEL:
        GetTargetScreen()->SelectWidget(NULL, FALSE, FALSE);
        GetPropsWin()->OnWidgetSelect(NULL);
        break;

    case DISPLAY_NODE_LEVEL:
        GetTargetScreen()->SelectWidget(NULL, FALSE, FALSE);
        GetPropsWin()->OnWidgetSelect(NULL);
        display_index = (int) mpTree->GetItemData(hItem);
        if (display_index != m_active_display)
        {
            m_active_display = display_index;
            GetResourceView()->CleanupDisplayResources(get_target_view_display());
            GetTargetScreen()->ConfigureDisplay(display_index);
            GetResourceView()->OnDisplaySelect(display_index);
        }
        break;

    case FOLDER_NODE_LEVEL:
        folder = (folder_info *)mpTree->GetItemData(hItem);
        check_set_active_display<folder_info>(folder);
        root = get_root_window();
        info = folder->GetFirstChildWidget();
        if (root && !root->gx_widget_first_child && info)
        {
            GetTargetScreen()->SelectWidget(info, FALSE, FALSE);
            GetPropsWin()->OnWidgetSelect(info);
        }
        else
        {
            GetTargetScreen()->SelectWidget(NULL, FALSE, FALSE);
            GetPropsWin()->OnWidgetSelect(NULL);
        }

        if (root)
        {
            gx_widget_show((GX_WIDGET *)get_root_window());
        }
        break;

    case SCREEN_NODE_LEVEL:
        info = (widget_info *) mpTree->GetItemData(hItem);
        check_set_active_display<widget_info>(info);
        GetTargetScreen()->SwitchTopWidget(info);
        GetTargetScreen()->SelectWidget(info, FALSE, FALSE);
        GetPropsWin()->OnWidgetSelect(info);
        if (get_root_window())
        {
            gx_widget_show((GX_WIDGET *)get_root_window());
        }
        break;

    default:
        info = (widget_info *) mpTree->GetItemData(hItem);
        check_set_active_display(info);            
        GetTargetScreen()->SelectWidget(info, FALSE, FALSE);
        GetPropsWin()->OnWidgetSelect(info);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos());
    *pResult = 1;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnContextMenu(CWnd *pWnd, CPoint pos)
{
    HTREEITEM htItem;

    if (pos.x == -1 && pos.y == -1)
    {
        // The contet menu is generated from the keyboard, for example the user types Shift+F10
        // Pop up menu under selected item
        htItem = mpTree->GetSelectedItem();

        if (htItem == NULL)
            return;

        CRect rect;
        mpTree->GetItemRect(htItem, &rect, TRUE);
        pos.x = rect.left;
        pos.y = rect.bottom;
    }
    else
    {
        ScreenToClient(&pos);

        UINT uFlags;

        htItem = mpTree->HitTest(pos, &uFlags);

        if (htItem == NULL)
            return;

        if (htItem != mpTree->GetSelectedItem())
        {
            mpTree->Select(htItem, TVGN_CARET);
        }
    }

    //m_hActiveItem = htItem;

    CMenu       menu;
    CMenu       insert_menu;
    CMenu       base_menu;
    CMenu      *pPopup;
    CMenu      *pInsertMenu = GX_NULL;
    INT         node_level;

    node_level = GetTreeNodeLevel(htItem);

    switch(node_level)
    {
    case PROJECT_NODE_LEVEL:
    {
        menu.LoadMenu(IDR_PROJECT_OPTIONS);
        pPopup = menu.GetSubMenu(0);

        studiox_project *project = GetOpenProject();

        if (project)
        {
            if (project->IsWidgetPositionLocked())
            {
                pPopup->CheckMenuItem(ID_LOCK_WIDGET_POSITIONS, MF_CHECKED);
            }
            else
            {
                pPopup->CheckMenuItem(ID_LOCK_WIDGET_POSITIONS, MF_UNCHECKED);
            }

            ClientToScreen(&pos);
            pPopup->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, (CMainFrame*)AfxGetMainWnd());
        }
    }
        break;

    case DISPLAY_NODE_LEVEL:
        menu.LoadMenu(IDR_DISPLAY_MENU);
        pPopup = menu.GetSubMenu(0);

        insert_menu.LoadMenu(IDR_INSERT_MENU);
        pInsertMenu = insert_menu.GetSubMenu(0);

        if (pInsertMenu)
        {
            if (CreateBaseMenu(&base_menu))
            {
                pInsertMenu->AppendMenu(MF_POPUP, (UINT)base_menu.m_hMenu, _T("Template"));
            }
            pPopup->AppendMenu(MF_POPUP, (UINT)pInsertMenu->m_hMenu, _T("Insert"));

            /* Disable items except folder. Only folder can be added to display node. */
            int count = pInsertMenu->GetMenuItemCount();
            int id;
            for (int pos = 0; pos < count; pos++)
            {
                id = pInsertMenu->GetMenuItemID(pos);

                if (id != ID_INSERT_FOLDER)
                {
                    pInsertMenu->EnableMenuItem(pos, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                }
            }
        }

        ClientToScreen(&pos);
        pPopup->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, (CMainFrame *)AfxGetMainWnd());
        break;

    case FOLDER_NODE_LEVEL:
    case SCREEN_NODE_LEVEL:
    default:
        menu.LoadMenu(IDR_WIDGET_MENU);
        pPopup = menu.GetSubMenu(0);

        insert_menu.LoadMenu(IDR_INSERT_MENU);
        pInsertMenu = insert_menu.GetSubMenu(0);

        if (pInsertMenu)
        {
            if (node_level == FOLDER_NODE_LEVEL)
            {
                /* Insert rename item to rename folder. */
                pPopup->InsertMenu(0, MF_BYPOSITION, ID_EDIT_FOLDER_PROPERTIES, _T("Edit Folder Properties"));
            }

            if (CreateBaseMenu(&base_menu))
            {
                pInsertMenu->AppendMenu(MF_POPUP, (UINT)base_menu.m_hMenu, _T("Template"));
            }

            pPopup->AppendMenu(MF_POPUP, (UINT)pInsertMenu->m_hMenu, _T("Insert"));

            /* Disable "folder" item */
            pInsertMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        }

        ClientToScreen(&pos);
        pPopup->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, (CMainFrame *)AfxGetMainWnd());
        break;
    }
}


///////////////////////////////////////////////////////////////////////////////
// This function MUST be called when the active project pointer is changed,
// like if it is backed up (cloned) and restored
void project_view::DeSelect()
{
    mpTree->SelectItem(mpTree->GetRootItem());

    GetTargetScreen()->SelectWidget(NULL, FALSE, FALSE);
    GetPropsWin()->OnWidgetSelect(NULL);   
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnProjectReset()
{
    GetTargetScreen()->SwitchTopWidget(NULL);
    GetTargetScreen()->ConfigureDisplay(m_active_display);
    PopulateProjectTree(GetOpenProject());
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnConfigureDisplays()
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    CString name;
    BOOL is_root_selected = FALSE;

    HTREEITEM item = mpTree->GetSelectedItem();
    int selected_node_level = 0;

    name = mpTree->GetItemText(item);
    selected_node_level = GetTreeNodeLevel(item);

    GetProjectView()->DeSelect();
    GetTargetScreen()->SwitchTopWidget(NULL);
    GetTargetScreen()->ConfigureDisplay(-1);

    ProjectConfigDlg dlg(NULL, m_active_display);
    dlg.SetOwner(AfxGetMainWnd());
    if (dlg.DoModal() == IDOK)
    {
        if (project)
        {
            project->mHeader.guix_version = dlg.GetGuixVersion();
            project->SetModified();
            project->InitializeAllPixelmaps();
            project->InitializeFonts();
            PopulateProjectTree(project);
        }
    }
    GetResourceView()->CleanupDisplayResources(get_target_view_display());
    GetTargetScreen()->ConfigureDisplay(m_active_display, TRUE);
    GetResourceView()->OnDisplaySelect(m_active_display, TRUE);

    switch (selected_node_level)
    {
    case PROJECT_NODE_LEVEL:
        item = mpTree->GetRootItem();
        break;

    case DISPLAY_NODE_LEVEL:
        item = FindProjectTreeNode(mpTree->GetRootItem(), name, DISPLAY_NODE_LEVEL);
        break;

    default:
        if (m_active_display >= 0)
        {
            item = FindProjectTreeNode(name, project->mDisplays[m_active_display].name);
        }
        break;
    }

    if (item)
    {
        mpTree->SelectItem(item);
    }
    else
    {
        SelectFirstScreen();
    }
    
}

///////////////////////////////////////////////////////////////////////////////
void project_view::TerminateAppExecutions()
{
    GX_WIN32_DISPLAY_DRIVER_DATA *data = gx_win32_get_data_instance_head();
    for (int index = 0; index < GX_MAX_WIN32_DISPLAYS; index++)
    {
        if (data->win32_driver_type && data->win32_guix_ThreadId)
        {
            /* Send window close message. */
            ::SendMessage(data->win32_driver_winhandle, WM_CLOSE, 0, 0);

            HANDLE guix_thread = OpenThread(SYNCHRONIZE, FALSE, data->win32_guix_ThreadId);
            WaitForSingleObject(guix_thread, INFINITE);
            CloseHandle(guix_thread);
        }
        data++;
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::CloseProject(BOOL DisplayRecent, BOOL CheckSaveChanges)
{
    /* Destory app execution window. */
    TerminateAppExecutions();

    GetResourceView()->OnCloseProject();
    GetTargetScreen()->OnCloseProject();
    m_active_display = -1;
    GetPropsWin()->OnWidgetSelect(NULL);
    widget_factory::ResetServiceProviders();

    studiox_project *project = GetOpenProject();

    if (project)
    {
        if (CheckSaveChanges && project->IsModified())
        {
            if (AskUser("The current project has been modified. Do you want to save your changes?"))
            {
                project->Save();
            }
        }
        delete project;
        SetOpenProject(NULL);
    }
    mpTree->DeleteAllItems();
    SetFrameTitle(CString(_T("No Project")));
    AddTreeRoot(CString(_T("No Project")));

    if (DisplayRecent)
    {
        GetTargetView()->DisplayRecentProjects();
    }

    ((CMainFrame *)AfxGetMainWnd())->DisableMenus();
}

///////////////////////////////////////////////////////////////////////////////
void project_view::SetFrameTitle(CString &title)
{
    CString fulltitle = _T("Azure RTOS GUIX Studio ");
    fulltitle += studiox_version_string;
    fulltitle += " - ";
    fulltitle += title;
    AfxGetMainWnd()->SetWindowText(fulltitle);
}

///////////////////////////////////////////////////////////////////////////////
void project_view::NewProject(CWnd *parent)
{
    if (parent == NULL)
    {
        parent = AfxGetMainWnd();
    }

    CString title;
    CString pathname;
    NewProjectDlg dlg(parent);

    if (dlg.DoModal() == IDOK)
    {
        CloseProject();

        title = dlg.Getproject_name();
        pathname = dlg.Getproject_path();
        SetProjectDirectory(pathname);

        studiox_project *project = new studiox_project(pathname, title, TRUE);
        SetOpenProject(project);

        if (project->Save())
        {
            pathname += '\\';
            pathname += title;
            pathname += ".gxp";
            AddRecentProject(pathname);
            SetFrameTitle(title);
            AddTreeRoot(project->mHeader.project_name);
            OnConfigureDisplays();
            PopulateProjectTree(project);
            GetTargetView()->DisplayTarget();
            GetResourceView()->OnOpenProject();

            GetTargetScreen()->ConfigureDisplay(0);
            GetResourceView()->OnDisplaySelect(0, TRUE);

            //Add default folder and default window to project to help user start in Studio when create new project
            SelectDisplay(0);
            /* Add default folder to display 0*/
            OnInsertFolder();
            /* Add default window */
            GetTargetScreen()->OnAddWidget(GX_TYPE_WINDOW);
            /* Show notify message. */
            Notify("To begin, right click in the target view and begin inserting child widgets.");

            project->mHeader.b_new_project = FALSE;
        }
        else
        {
            CloseProject();
        }

        ((CMainFrame *)AfxGetMainWnd())->EnableMenus();
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::ProjectRenamed()
{
    // this function is called when the user uses "Save As". We have to reset
    // the title and reset the root tree node:
    studiox_project *project = GetOpenProject();
    if (project)
    {
        SetFrameTitle(project->mHeader.project_name);
        mpTree->SetItemText(mpTree->GetRootItem(), project->mHeader.project_name);
    }
}

///////////////////////////////////////////////////////////////////////////////

BOOL project_view::OpenProject(CString &path, CString &name)
{
    CMainFrame *pMain = (CMainFrame *) AfxGetMainWnd();
    CWnd* pCwnd = AfxGetApp()->m_pMainWnd;
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();
    BOOL failure = FALSE;

    if (path == ".")
    {
        TCHAR tpath[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, tpath);
        path = tpath;
    }

    studiox_project *project = new studiox_project(path, name, FALSE);
    SetOpenProject(project);

    CString pathname = path;
    pathname += '\\';
    pathname += name;
    SetProjectDirectory(path);
    GotoProjectDirectory();

    if (pCmdInfo->IsXmlMode())
    {
        if (!project->ReadXMLFile(pathname))
        {
            failure = TRUE;
        }
    }
    else if (project->Read(pathname))
    {
        SetFrameTitle(name);
        AddRecentProject(pathname);
        PopulateProjectTree(project);
        GetTargetView()->DisplayTarget();
        GetResourceView()->OnOpenProject();

        m_active_display = 0;
        
        if (GetTargetScreen()->ConfigureDisplay(0))
        {
            GetResourceView()->OnDisplaySelect(0, TRUE);
            SelectFirstScreen();
            ((CMainFrame *)AfxGetMainWnd())->EnableMenus();
        }
        else
        {
            failure = TRUE;
        }
    }
    else
    {
        failure = TRUE;
    }
     
    if (failure)
    {
        CloseProject();

        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OpenProject()
{
    CFileDialog dlg(TRUE, _T(".gxp"), NULL,
        OFN_FILEMUSTEXIST|OFN_NOREADONLYRETURN|OFN_PATHMUSTEXIST,
        _T("GUIX project files|*.gxp||"),
        this);

    if (dlg.DoModal() == IDOK)
    {
        CloseProject();

        CString pathname = dlg.GetPathName();
        CString name = dlg.GetFileName();

        CString path = pathname.Left(pathname.GetLength() - name.GetLength() - 1);
        OpenProject(path, name);
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::MergeProject()
{
    studiox_project *current = GetOpenProject();

    if (current)
    {
        CFileDialog dlg(TRUE, _T(".gxp"), NULL,
            OFN_FILEMUSTEXIST|OFN_NOREADONLYRETURN|OFN_PATHMUSTEXIST,
            _T("GUIX project files|*.gxp||"),
            this);

        if (dlg.DoModal() == IDOK)
        {
            CString pathname = dlg.GetPathName();
            CString name = dlg.GetFileName();

            CString path = pathname.Left(pathname.GetLength() - name.GetLength() - 1);
            MergeProject(path, name);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::MergeProject(CString &path, CString &name)
{
    extern CString gOpenProjectDir;

    studiox_project *merge_project = new studiox_project(path, name, FALSE);

    CString oldpathname = gOpenProjectDir;
    SetProjectDirectory(path);

    CString pathname;
    pathname.Format(_T("%s\\%s"), path, name);

    if (merge_project->Read(pathname))
    {
        SetProjectDirectory(oldpathname);

        import_project_dlg import(merge_project, path, this);

        import.DoModal();
    }
    delete merge_project;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OpenHistoryProject(int index)
{
    CloseProject();
    CString pathname = StudioXIni.recent_project_paths[index];
    if (pathname.IsEmpty())
    {
        return;
    }
    CString path = pathname;
    CString name = path.Mid(path.ReverseFind('\\') + 1);
    path = path.Left(path.GetLength() - name.GetLength() - 1);
    OpenProject(path, name);
}

///////////////////////////////////////////////////////////////////////////////
int project_view::GetTreeNodeLevel(HTREEITEM item)
{
    if (!item)
    {
        return -1;
    }

    int Level = PROJECT_NODE_LEVEL;
    HTREEITEM root = mpTree->GetRootItem();

    while (item != root)
    {
        Level++;
        item = mpTree->GetParentItem(item);
    }
    return Level;
}


///////////////////////////////////////////////////////////////////////////////
void project_view::OnConfigureThemes()
{
    studiox_project *project = GetOpenProject();
    if (project)
    {
        int old_theme = project->mDisplays[m_active_display].active_theme;

        configure_theme_dlg dlg(m_active_display);
        dlg.DoModal();
        project->SetModified();
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL project_view::IsDisplaySelected()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();

        if (GetTreeNodeLevel(hItem) == DISPLAY_NODE_LEVEL)
        {
            LONG data = (LONG) mpTree->GetItemData(hItem);
    
            // this should be a display index, 0 to MAX_DISPLAYS

            if (data >= 0 && data < MAX_DISPLAYS)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}  

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::IsFolderSelected()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();

        if (GetTreeNodeLevel(hItem) == FOLDER_NODE_LEVEL)
        {
            return TRUE;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
INT project_view::GetSelectedNodeLevel()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();

        return GetTreeNodeLevel(hItem);
    }
    return PROJECT_NODE_LEVEL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info *project_view::GetSelectedFolder() 
{
    if (IsFolderSelected())
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();
        if (hItem)
        {
            return ((folder_info *)mpTree->GetItemData(hItem));
        }
    }
    return GX_NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info *project_view::GetRootFolder()
{
    folder_info *root = NULL;
    studiox_project *project = GetOpenProject();

    if (project && m_active_display >= 0)
    {
        root = project->mDisplays[m_active_display].GetFirstChildFolder();
    }
    return root;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::UpdateChildPositions(widget_info *parent)
{
    if (parent)
    {
        widget_info *child = parent->GetChildWidgetInfo();

        while(child)
        {
            if (child->widget)
            {
                child->size = child->widget->gx_widget_size;
            }
            UpdateChildPositions(child);
            child = child->GetNextWidgetInfo();
        }
    }    
}

///////////////////////////////////////////////////////////////////////////////
void project_view::CheckParentRefresh(GX_WIDGET *parent)
{
    studiox_project *project = GetOpenProject();
    if (project && parent)
    {
        if (parent->gx_widget_type == GX_TYPE_MENU_LIST)
        {
            parent = ((GX_MENU_LIST *)parent)->gx_menu_list_owner;
        }

        widget_info *info = project->FindWidgetInfo(parent);
        CheckParentRefresh(info);
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::CheckParentRefresh(widget_info *parent_info)
{
    BOOL update_positions = FALSE;
    GX_WIDGET *parent;
    studiox_project *project = GetOpenProject();

    if (parent_info && project)
    {
        switch(parent_info->basetype)
        {
        case GX_TYPE_VERTICAL_LIST:
            {
            GX_VERTICAL_LIST *list = (GX_VERTICAL_LIST *) parent_info->widget;
            gx_vertical_list_children_position(list);
            update_positions = TRUE;
            }
            break;
            
        case GX_TYPE_HORIZONTAL_LIST:
            {
            GX_HORIZONTAL_LIST *list = (GX_HORIZONTAL_LIST *) parent_info->widget;
            gx_horizontal_list_children_position(list);
            update_positions = TRUE;
            }
            break;

        case GX_TYPE_ACCORDION_MENU:
            parent = menu_service_provider::TopLevelMenuContainerFind(parent_info->widget);
            if (parent && parent->gx_widget_type == GX_TYPE_ACCORDION_MENU)
            {
                parent_info = project->FindWidgetInfo(parent);
                gx_accordion_menu_position((GX_ACCORDION_MENU *)parent);
                memcpy_s(&parent_info->size, sizeof(parent_info->size), &parent_info->widget->gx_widget_size, sizeof(GX_RECTANGLE));
                update_positions = TRUE;
            }
            break;

        case GX_TYPE_MENU:
            parent = parent_info->widget->gx_widget_parent;
            parent = menu_service_provider::TopLevelMenuContainerFind(parent);

            if (parent)
            {
                parent_info = project->FindWidgetInfo(parent);

                if (parent->gx_widget_type == GX_TYPE_ACCORDION_MENU)
                {
                    gx_accordion_menu_position((GX_ACCORDION_MENU *)parent);
                }
                else if (parent->gx_widget_type == GX_TYPE_TREE_VIEW)
                {
                    gx_tree_view_position((GX_TREE_VIEW *)parent);
                }
                update_positions = TRUE;
            }
            break;

        case GX_TYPE_TREE_VIEW:
            gx_tree_view_position((GX_TREE_VIEW *)parent_info->widget);
            update_positions = TRUE;
            break;

        case GX_TYPE_GENERIC_SCROLL_WHEEL:
            gx_generic_scroll_wheel_children_position((GX_GENERIC_SCROLL_WHEEL *)parent_info->widget);
            update_positions = TRUE;
            break;

        default:
            break;
        }

        if (update_positions)
        {
            UpdateChildPositions(parent_info);
        }
    }
    
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnDeleteWidget()
{
    studiox_project *project = GetOpenProject();
    if (project)
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();
        int       node_level = GetTreeNodeLevel(hItem);

        if (node_level >= SCREEN_NODE_LEVEL)
        {
            widget_info *info = (widget_info *) mpTree->GetItemData(hItem);

            DeleteWidget(info);
        }
        else if (node_level == FOLDER_NODE_LEVEL)
        {
            folder_info *folder = (folder_info *)mpTree->GetItemData(hItem);

            DeleteFolder(folder);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// called when user do delete (and also by Undo manager)
void project_view::DeleteFolder(folder_info *folder)
{
    studiox_project *project = GetOpenProject();
    if (project)
    {
        int display_index = project->GetDisplayIndex(folder);

        /* Check all derived widgets of template (if there's one) are removed before delete any item. */
        if (!template_service_provider::CheckExternalTemplateDerivations(folder))
        {
            Notify("At least one template within this folder is referenced outside this folder. This folder cannot be deleted until all template references are first removed.");
            return;
        }

        screen_flow *screenflow = project->mDisplays[display_index].screenflow;
        widget_info *info = folder->GetFirstChildWidget();

        /* delete screen flow*/
        while (info)
        {
            if (screenflow)
            {
                flow_item *item = screenflow->GetFlowItem(info->app_name);
                if (item && item->trigger_list)
                {
                    CString msg;
                    msg.Format(_T("Triggers that defined for \"%s\" will be deleted as well, continue?"), info->app_name);
                    if (!AskUser(CT2A(msg)))
                    {
                        return;
                    }
                }
                screenflow->DeleteFlowItem(info->app_name, display_index);
            }

            info = info->GetNextWidgetInfo();
        }

        HTREEITEM hItem = FindProjectTreeNode(folder->folder_name, project->mDisplays[display_index].name);

        UndoManager()->AddEntry(UNDO_TYPE_DELETE_FOLDER, folder, display_index);

        GetTargetScreen()->SelectWidget(NULL, FALSE, FALSE);
        GetPropsWin()->OnWidgetSelect(NULL);

        project->DeleteFolder(folder);

        if (hItem)
        {
            mpTree->DeleteItem(hItem);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::FindWidgetIdName(widget_info *start, widget_info *except, CString id_name)
{
    //search widget id name under "start" info
    //do not search under "except" info
    if (start != except)
    {
        if (start->id_name == id_name)
        {
            return TRUE;
        }

        widget_info *child = start->GetChildWidgetInfo();
        while (child)
        {
            if (FindWidgetIdName(child, except, id_name))
            {
                return TRUE;
            }
            child = child->GetNextWidgetInfo();
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::CheckTriggerReference(screen_flow *flow, flow_item *item, widget_info *screen_info, widget_info *info, widget_info *search)
{
    if (item)
    {
        if (!search->id_name.IsEmpty())
        {
            // check if the id name is referenced by a trigger
            // if yes, check if there are duplicate ID name under the same top level widget
            if (flow->FindTrigger(item, search->id_name) && !FindWidgetIdName(screen_info, info, search->id_name))
            {
                CString  msg;
                msg.Format(_T("ID name \"%s\" is referenced by a trigger, remove the id name will invalid the trigger, continue?"), search->id_name);
                if (!AskUser(CT2A(msg)))
                {
                    return FALSE;
                }
            }
        }

        widget_info *child = search->GetChildWidgetInfo();
        while (child)
        {
            if (!CheckTriggerReference(flow, item, screen_info, info, child))
            {
                return FALSE;
            }

            child = child->GetNextWidgetInfo();
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
VOID project_view::RemoveWidgetIdFromDictionary(studiox_project *project, int display, widget_info *info)
{
    screen_flow *flow = project->mDisplays[display].screenflow;

    if (!info->id_name.IsEmpty())
    {
        project->RemoveFromIdDictionary(display, ID_TYPE_WIDGET, info->id_name);
    }

    widget_info *child = info->GetChildWidgetInfo();

    while (child)
    {
        RemoveWidgetIdFromDictionary(project, display, child);
        child = child->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::CheckRemoveWidget(studiox_project *project, int display, widget_info *info)
{
    screen_flow *flow = project->mDisplays[display].screenflow;
    widget_info *screen_info = NULL;
    flow_item *item = NULL;

    if (flow)
    {
        screen_info = GetProjectView()->FindTopLevelWidget(info);

        if (screen_info)
        {
            item = flow->GetFlowItem(screen_info->app_name);
        }

        if (item)
        {
            if (item->screen_name == info->app_name)
            {
                CString  msg;
                msg.Format(_T("Triggers that defined for \"%s\" will be deleted as well, continue?"), info->app_name);
                if (!AskUser(CT2A(msg)))
                {
                    return FALSE;
                }
            }
            else if (!CheckTriggerReference(flow, item, screen_info, info, info))
            {
                return FALSE;
            }
        }
    }

    RemoveWidgetIdFromDictionary(project, display, info);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// called when user delete, and also by Undo manager
void project_view::DeleteWidget(widget_info *info)
{
    HTREEITEM hItem = 0;
    GX_WIDGET *parent = NULL;
    studiox_project *project = GetOpenProject();

    if (!info)
    {
        return;
    }

    if (project)
    {
        if (info->is_template)
        {
            if (template_service_provider::CountDerivedWidgets(info)) 
            {
                Notify("This template cannot be deleted until all references are first removed.");
                return;
            }
        }

        int display_index = project->GetDisplayIndex(info);

        if (!CheckRemoveWidget(project, display_index, info))
        {
            return;
        }

        if (IsTopLevelWidget(info))
        {
            screen_flow *screenflow = project->mDisplays[m_active_display].screenflow;

            if (screenflow)
            {
                screenflow->DeleteFlowItem(info->app_name, m_active_display);
            }
        }

        UndoManager()->AddEntry(UNDO_TYPE_DELETE_WIDGET, info, display_index);

        GetTargetScreen()->SelectWidget(NULL, FALSE, FALSE);
        GetPropsWin()->OnWidgetSelect(NULL);

        if (info->widget)
        {
            parent = info->widget->gx_widget_parent;
        }

        hItem = FindWidget(info);
        project->DeleteWidget(info);

        if (hItem)
        {
            mpTree->DeleteItem(hItem);
        }

        if (parent)
        {
            CheckParentRefresh(parent);

            if (parent->gx_widget_type == GX_TYPE_MENU_LIST)
            {
                //deleted widget is a child of menu list, decrease the list count
                widget_info *parent_info = project->FindWidgetInfo(parent);
                if (parent_info)
                {
                    parent_info->ewi.menu.list_total_count--;
                }
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void project_view::OnCutWidget()
{
    OnCopyWidget();
    OnDeleteWidget();
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnCopyWidget()
{
    studiox_project *project = GetOpenProject();
    if (project)
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();
        int selected_level = GetTreeNodeLevel(hItem);
        if (selected_level == FOLDER_NODE_LEVEL)
        {
            folder_info *folder = (folder_info *)mpTree->GetItemData(hItem);

            /* Copy Folder. */
            copy_paste_engine cpe;
            cpe.CopyToClipboard(folder, m_hWnd);
        }
        else if (selected_level >= SCREEN_NODE_LEVEL)
        {
            //GetTargetScreen()->SelectWidget(NULL);
            widget_info *info = (widget_info *) mpTree->GetItemData(hItem);

            if (info)
            {
                copy_paste_engine cpe;
                cpe.CopyToClipboard(info, m_hWnd);
            }
        }
        else
        {
            //There maybe more than one widgets been selected
            if (GetTargetScreen())
            {
                GetTargetScreen()->CopySelectedWidgets();
            }
        }
    }  
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::CheckRemoveDuplicateTemplates(folder_info *start, folder_info *search_start, BOOL notify_duplicates)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return FALSE;
    }

    if (!search_start)
    {
        search_start = project->mDisplays[m_active_display].GetFirstChildFolder();
    }

    widget_info *info = start->GetFirstChildWidget();
    widget_info *find;
    widget_info *pre = NULL;
    widget_info *next;
    while (info)
    {
        next = info->GetNextWidgetInfo();

        if (info->is_template)
        {
            find = project->FindWidgetInfo(search_start, info->app_name, FALSE);

            if (find)
            {
                if (!find->is_template)
                {
                    CString msg;
                    msg.Format(L"Template name \"%s\" already exists! Aborting.", info->app_name);
                    Notify(CT2A(msg));
                    return FALSE;
                }

                if (notify_duplicates)
                {
                    CString msg;
                    msg.Format(L"Template \"%s\" already exists! Skipping.", info->app_name);
                    Notify(CT2A(msg));
                }

                if (pre)
                {
                    pre->SetNextWidgetInfo(info->GetNextWidgetInfo());
                }
                else
                {
                    start->SetFirstChildWidget(info->GetNextWidgetInfo());
                }
                info->SetNextWidgetInfo(NULL);
                delete info;
            }
            else
            {
                pre = info;
            }
        }
        else
        {
            pre = info;
        }
        info = next;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::AddWidgetsToFolder(folder_info *target_folder, folder_info *src_folder)
{
    widget_info *child = src_folder->GetFirstChildWidget();
    widget_info *next;

    if (!child)
    {
        return;
    }

    next = child->GetNextWidgetInfo();

    while (next)
    {
        child = next;
        next = child->GetNextWidgetInfo();
    }

    child->SetNextWidgetInfo(target_folder->GetFirstChildWidget());
    target_folder->SetFirstChildWidget(src_folder->GetFirstChildWidget());
}


///////////////////////////////////////////////////////////////////////////////
folder_info *project_view::CombineCopyPasteWidgets(copy_paste_info *pasted_info)
{
    folder_info *copied_widgets = pasted_info->copied_widgets;
    folder_info *template_dependencies = pasted_info->template_dependencies;

    if (template_dependencies)
    {
        // Remove templates that duplicate the target project template.
        if (!CheckRemoveDuplicateTemplates(template_dependencies))
        {
            delete pasted_info;
            return NULL;
        }

        // Remove copied widgets that duplicate template dependencies.
        CheckRemoveDuplicateTemplates(copied_widgets, template_dependencies);

        // Add the remaining template widgets to copied widgets fodler.
        AddWidgetsToFolder(copied_widgets, template_dependencies);
        template_dependencies->SetFirstChildWidget(NULL);
    }

    if (!CheckRemoveDuplicateTemplates(copied_widgets, NULL, TRUE))
    {
        delete pasted_info;
        return NULL;
    }

    pasted_info->copied_widgets = NULL;
    delete pasted_info;

    return copied_widgets;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::PasteFolder(copy_paste_info *pasted_info)
{
    studiox_project *project = GetOpenProject();

    if (!project || !pasted_info)
    {
        return;
    }

    folder_info *copied_widgets = CombineCopyPasteWidgets(pasted_info);
    if (!copied_widgets)
    {
        return;
    }

    //create uniqute folder name
    CreateUniqeFolderName(copied_widgets);

    widget_factory::CreateUniqueAppNames(copied_widgets->GetFirstChildWidget(), copied_widgets->GetFirstChildWidget(), NULL, TRUE);

    //add folder to active display
    project->AddFolderToDisplay(m_active_display, copied_widgets);

    if (copied_widgets->GetFirstChildWidget())
    {
        //select the first widget of the folder
        HTREEITEM select = FindWidget(copied_widgets->GetFirstChildWidget());
        if (select)
        {
            mpTree->SelectItem(select);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::PasteWidgets(folder_info *parent_folder, copy_paste_info *pasted_info)
{
    studiox_project *project = GetOpenProject();
    
    if (!project || !pasted_info)
    {
        return;
    }

    folder_info *copied_widgets = CombineCopyPasteWidgets(pasted_info);
    if (!copied_widgets)
    {
        return;
    }

    if (copied_widgets->GetFirstChildWidget())
    {
        // Find last child of the copied widgets.
        widget_info* last_child = copied_widgets->GetFirstChildWidget();
        while (last_child && last_child->GetNextWidgetInfo())
        {
            last_child = last_child->GetNextWidgetInfo();
        }

        copied_widgets->folder_name = parent_folder->folder_name;

        widget_factory::CreateUniqueAppNames(copied_widgets->GetFirstChildWidget(), copied_widgets->GetFirstChildWidget(), NULL, TRUE);

        UndoManager()->AddEntry(UNDO_TYPE_INSERT_TOP_LEVEL_WIDGETS, copied_widgets, m_active_display);

        project->AddWidgetToFolder(parent_folder, copied_widgets->GetFirstChildWidget());

        //select the last widget of the folder
        HTREEITEM select = FindWidget(last_child);
        if (select)
        {
            mpTree->SelectItem(select);
        }
    }

    copied_widgets->SetFirstChildWidget(NULL);
    delete copied_widgets;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::PasteWidgets(widget_info *parent_info, copy_paste_info *pasted_info)
{
    studiox_project *project = GetOpenProject();

    if (!project || !pasted_info)
    {
        return;
    }

    folder_info *copied_widgets = pasted_info->copied_widgets;

    if (!copied_widgets)
    {
        return;
    }

    widget_info* pasted_widget = copied_widgets->GetFirstChildWidget();

    // Check if copied widgets contain templates.
    while (pasted_widget)
    {
        if (pasted_widget->is_template)
        {
            // Template is not allowed to be pasted to widget node.
            CString msg;
            msg.Format(L"Template \"%s\" must be pasted to folder node.", pasted_widget->app_name);
            Notify(CT2A(msg));
            delete pasted_info;
            return;
        }

        pasted_widget = pasted_widget->GetNextWidgetInfo();
    }

    folder_info *template_dependencies = pasted_info->template_dependencies;
    int display = GetActiveDisplay();

    if (template_dependencies)
    {
        // Remove templates that duplicate the target project template.
        if (!CheckRemoveDuplicateTemplates(template_dependencies))
        {
            delete pasted_info;
            return;
        }
        
        // Paste templates to top level folder.
        if (template_dependencies->GetFirstChildWidget())
        {
            folder_info *parent_folder = project->FindParentFolderInfo(parent_info);

            template_dependencies->folder_name = parent_folder->folder_name;
            UndoManager()->AddEntry(UNDO_TYPE_INSERT_TOP_LEVEL_WIDGETS, template_dependencies, display);

            project->AddWidgetToFolder(parent_folder, template_dependencies->GetFirstChildWidget());
            template_dependencies->SetFirstChildWidget(NULL);

            HTREEITEM select = FindWidget(parent_info);
            if (select)
            {
                mpTree->SelectItem(select);
            }
        }
    }

    const widget_info *pasted_widget_tail;
    HTREEITEM hItem = mpTree->GetSelectedItem();

    GX_RECTANGLE combine_size;
    GX_RECTANGLE newsize;
    int x_shift, y_shift;

    pasted_widget = copied_widgets->GetFirstChildWidget();

    if (!pasted_widget)
    {
        delete pasted_info;
        return;
    }

    combine_size = pasted_widget->size;
    while (pasted_widget)
    {
        gx_utility_rectangle_combine(&combine_size, &pasted_widget->size);
        pasted_widget = pasted_widget->GetNextWidgetInfo();
    }
    newsize = combine_size;
    gx_utility_rectangle_center(&newsize, &parent_info->size);

    x_shift = newsize.gx_rectangle_left - combine_size.gx_rectangle_left;
    y_shift = newsize.gx_rectangle_top - combine_size.gx_rectangle_top;

    widget_info *search_start = GetProjectView()->FindTopLevelWidget(parent_info);

    widget_factory::CreateUniqueAppNames(copied_widgets->GetFirstChildWidget(),
        copied_widgets->GetFirstChildWidget(), search_start->GetChildWidgetInfo(), FALSE);

    if (parent_info->widget)
    {
        widget_factory::GenerateWidgets(parent_info->widget, copied_widgets->GetFirstChildWidget());
    }

    pasted_widget = copied_widgets->GetFirstChildWidget();
    while (pasted_widget)
    {
        //adjust size to center pasted widgets within the selected parent:
        newsize = pasted_widget->size;
        gx_utility_rectangle_shift(&newsize, x_shift, y_shift);
        widget_factory::MoveWidget(pasted_widget, newsize);

        pasted_widget = pasted_widget->GetNextWidgetInfo();
    }

    pasted_widget_tail = project->GetTailInfo(copied_widgets->GetFirstChildWidget());
    project->AddWidgetToParent(parent_info, copied_widgets->GetFirstChildWidget());

    // now make the widget visible:
    CheckParentRefresh(parent_info);

    GetTargetScreen()->DeSelect();
    pasted_widget = copied_widgets->GetFirstChildWidget();
    while(pasted_widget)
    {
        if (pasted_widget->basetype == GX_TYPE_ACCORDION_MENU ||
            pasted_widget->basetype == GX_TYPE_TREE_VIEW ||
            pasted_widget->basetype == GX_TYPE_GENERIC_SCROLL_WHEEL)
        {
            CheckParentRefresh(pasted_widget);
        }

        GetTargetScreen()->SelectWidget(pasted_widget, TRUE, TRUE);

        if (pasted_widget == pasted_widget_tail)
        {
            break;
        }

        pasted_widget = pasted_widget->GetNextWidgetInfo();
    }

    UndoManager()->AddEntry(UNDO_TYPE_INSERT_WIDGET, SelectedWidgets);

    copied_widgets->SetFirstChildWidget(NULL);
    delete pasted_info;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnPasteWidget()
{
    studiox_project *project = GetOpenProject();
    widget_info *parent_widget_info = NULL;
    folder_info *parent_folder_info = NULL;
    copy_paste_info *pasted_info = NULL;
    int display;

    if (project)
    {
        HTREEITEM hItem = mpTree->GetSelectedItem();
        UINT selected_item_level = GetTreeNodeLevel(hItem);
        display = GetActiveDisplay();

        copy_paste_engine cpe;
        pasted_info = cpe.PasteFromClipboard(m_hWnd, display);

        if (!pasted_info || !pasted_info->copied_widgets)
        {
            Notify("Failed to read clipboard data.");
        }
        else
        {
            if (pasted_info->copied_widgets->folder_name.IsEmpty())
            {
                //copy widgets
                if (selected_item_level < FOLDER_NODE_LEVEL)
                {
                    Notify("Copied widgets must be pasted to folder or widgets node.");
                    delete pasted_info;
                }
                else
                {
                    if (selected_item_level == FOLDER_NODE_LEVEL)
                    {
                        //copy widgets to a folder
                        parent_folder_info = (folder_info *)mpTree->GetItemData(hItem);
                        PasteWidgets(parent_folder_info, pasted_info);
                    }
                    else
                    {
                        //copy widgets to a widget
                        parent_widget_info = (widget_info *)mpTree->GetItemData(hItem);
                        PasteWidgets(parent_widget_info, pasted_info);
                    }
                }
                return;
            }
            else
            {
                //copy folder
                if (selected_item_level != DISPLAY_NODE_LEVEL)
                {
                    Notify("Screen folder must be pasted to display node.");
                    delete pasted_info;
                }
                else
                {
                    PasteFolder(pasted_info);
                }
            }
        }
    }   
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnInsertFolder()
{
    studiox_project *project = GetOpenProject();

    folder_info *folder = new folder_info(_T("default_folder"));
    CreateUniqeFolderName(folder);
    project->AddFolderToDisplay(m_active_display, folder);
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnEditFolderProperties()
{
    HTREEITEM hItem = mpTree->GetSelectedItem();
    folder_info *folder = (folder_info *)mpTree->GetItemData(hItem);

    folder_name_dlg dlg(folder, this);
    if (dlg.DoModal() == IDOK)
    {
        CString new_folder_name;
        dlg.GetEditFolderName(new_folder_name);

        /* Set output file name. */
        dlg.GetSpecifiedOutputFileName(folder->output_filename);

        if (new_folder_name != folder->folder_name)
        {
            if (IsFolderNameExist(new_folder_name))
            {
                Notify("Folder name already exists. Please check it.");
                return;
            }

            folder->folder_name = new_folder_name;
            mpTree->SetItemText(hItem, folder->folder_name);
            HTREEITEM parent = mpTree->GetParentItem(hItem);
            if (parent)
            {
                mpTree->SortChildren(parent);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_BOOL project_view::IsFolderNameExist(folder_info *folder, CString name)
{
    while (folder)
    {
        if (folder->folder_name == name)
        {
            return TRUE;
        }

        folder = folder->GetNextFolder();
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
GX_BOOL project_view::IsFolderNameExist(CString name)
{
    int display = GetActiveDisplay();
    studiox_project *project = GetOpenProject();

    folder_info  *info = project->mDisplays[display].GetFirstChildFolder();

    return IsFolderNameExist(info, name);
}

///////////////////////////////////////////////////////////////////////////////
void project_view::CreateUniqeFolderName(folder_info *folder)
{
    CString name = folder->folder_name;
    if (IsFolderNameExist(name))
    {
        /* Create unique folder name. */
        CString seed = name;
        int underscore = seed.ReverseFind('_');

        if (underscore > 0 && underscore > (seed.GetLength() - 3))
        {
            if (seed.GetAt(underscore + 1) >= '0' &&
                seed.GetAt(underscore + 1) <= '9')
            {
                seed = seed.Left(underscore);
            }
        }

        int index = 1;
        name.Format(seed + _T("_%d"), index++);
        while (IsFolderNameExist(name))
        {
            name.Format(seed + _T("_%d"), index++);
        }
        folder->folder_name = name;
    }
}

///////////////////////////////////////////////////////////////////////////////
widget_info *project_view::GetTemplateInfo(int menu_index)
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        HTREEITEM     root = mpTree->GetRootItem();
        HTREEITEM     hItem = mpTree->GetChildItem(root);
        HTREEITEM     folder;
        CString       name;
        widget_info  *info;

        // Get first item under screen node
        folder = mpTree->GetChildItem(hItem);
        while (folder)
        {
            hItem = mpTree->GetChildItem(folder);
            while (hItem)
            {
                info = (widget_info  *)mpTree->GetItemData(hItem);

                if (info->is_template)
                {
                    if (menu_index == 0)
                    {
                        return info;
                    }
                    menu_index--;
                }
                hItem = mpTree->GetNextSiblingItem(hItem);
            }
            folder = mpTree->GetNextSiblingItem(folder);
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::MoveSelectedToFront()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        widget_info *selected_info = GetTargetScreen()->GetSingleSelect();

        if (!selected_info)
        {
            return;
        }
        HTREEITEM hItem = mpTree->GetSelectedItem();

        if (GetTreeNodeLevel(hItem) < DISPLAY_NODE_LEVEL)
        {
            return;
        }
        widget_info *info = (widget_info *) mpTree->GetItemData(hItem);

        // sanity check
        if (!info || info != selected_info)
        {
            return;
        }

        // if we get to here, we have a valid widget and valid widget_info

        project->MoveInfoToFront(info);
        gx_widget_front_move(selected_info->widget, NULL);
        CheckParentRefresh(selected_info->widget->gx_widget_parent);
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::MoveSelectedToBack()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        widget_info *selected_info = GetTargetScreen()->GetSingleSelect();
        if (!selected_info)
        {
            return;
        }
        HTREEITEM hItem = mpTree->GetSelectedItem();

        if (GetTreeNodeLevel(hItem) < DISPLAY_NODE_LEVEL)
        {
            return;
        }
        widget_info *info = (widget_info *) mpTree->GetItemData(hItem);

        // sanity check
        if (!info || info != selected_info)
        {
            return;
        }

        // if we get to here, we have a valid widget and valid widget_info

        project->MoveInfoToBack(info);
        gx_widget_back_move(selected_info->widget, NULL);
        CheckParentRefresh(selected_info->widget->gx_widget_parent);
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnGenerateAll()
{
    studiox_project *project = GetOpenProject();
    BOOL result;

    if (project)
    {
        if (project->mHeader.gen_binary)
        {
            result = OnGenBinary(FALSE);
        }
        else
        {
            result = OnGenResources(FALSE);
        }

        if (result == TRUE)
        {
            OnGenApplication(FALSE);

            Notify("All Output files have been updated.");
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI GenerateResourceFileThreadEntry(LPVOID thread_input)
{
    studiox_project* project = GetOpenProject();
    BOOL result = TRUE;

    if (project)
    {
        resource_gen generator(project);
        CCommandInfo* pCmdInfo = GetCmdInfo();

        for (int display = 0; display < project->mHeader.num_displays; display++)
        {
            BOOL enabled;

            if (pCmdInfo->IsNoGui())
            {
                enabled = pCmdInfo->IsDisplayEnabled(project->mDisplays[display].name);
            }
            else
            {
                enabled = project->mDisplays[display].enabled;
            }

            if (enabled)
            {
                project->mDisplays[display].stable->MakeMLViewReferenceRecord(display);
                if (generator.GenerateResourceFile(display) == FALSE)
                {
                    /* Should return if generation fail.*/
                    result = FALSE;
                    break;
                }
            }
        }
    }
    
    EndBusyMsg();
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////
BOOL project_view::OnGenResources(GX_BOOL show_notify)
{
    if (!StartWorkThread(GenerateResourceFileThreadEntry, 0, "Generating Resource File."))
    {
        return FALSE;
    }

    if (show_notify)
    {
        Notify("Resource File Generation Completed.");
    }

    return TRUE;
}

static DWORD WINAPI GenerateScreensThreadEntry(LPVOID thread_input)
{
    studiox_project* project = GetOpenProject();
    BOOL result = FALSE;

    if (project)
    {
        screen_generator generator(project);

        result = generator.GenerateScreens(0);
    }

    EndBusyMsg();
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
void project_view::OnGenApplication(GX_BOOL show_notify)
{
    if (!StartWorkThread(GenerateScreensThreadEntry, 0, "Generating Specifications File."))
    {
        return;
    }
    
    if (show_notify)
    {
        Notify("Application Screen Generation Completed.");
    }
}

///////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI GenerateBinaryFileThreadEntry(LPVOID thread_input)
{
    studiox_project* project = GetOpenProject();
    BOOL result = TRUE;

    if (project)
    {
        binary_resource_gen generator(project, project->mHeader.binary_file_format);
        CCommandInfo *pCmdInfo = GetCmdInfo();

        for (int display = 0; display < project->mHeader.num_displays; display++)
        {
            BOOL enabled;

            if (pCmdInfo->IsNoGui())
            {
                enabled = pCmdInfo->IsDisplayEnabled(project->mDisplays[display].name);
            }
            else
            {
                enabled = project->mDisplays[display].enabled;
            }

            if (enabled)
            {
                project->mDisplays[display].stable->MakeMLViewReferenceRecord(display);
                result = generator.GenerateBinaryFile(display);

                if(result == FALSE)
                {
                    break;
                }
            }
        }
    }

    EndBusyMsg();
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::OnGenBinary(GX_BOOL show_notify)
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        if (!StartWorkThread(GenerateBinaryFileThreadEntry, 0, "Generating Binary Resource File."))
        {
            return FALSE;
        }

        if (show_notify)
        {
            Notify("Binary Resource File Generation Completed.");
        }

        return TRUE;
    }

    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// add a child widget to the selected screen tree node:
void project_view::InsertTopLevelWidget(widget_info *info)
{
    // KGM- this needs to be fixed. We can't use the tree view selected item
    // to determine where to insert a top level widget when we are doing an Undo.
    // this causes breakage......

    // we should locate the widget root window, and use that to find the 
    // tree root.

    HTREEITEM hParent = mpTree->GetSelectedItem();

    while(GetTreeNodeLevel(hParent) > FOLDER_NODE_LEVEL)
    {
        hParent = mpTree->GetParentItem(hParent);
    }
        
    HTREEITEM hCurrent = mpTree->InsertItem(info->app_name,
            hParent, TVI_SORT);
    mpTree->SetItemData(hCurrent, (DWORD_PTR) info);
    AssignTreeImage(hCurrent, info);

    /* Insert child widgets.  */
    if (info->GetChildWidgetInfo())
    {
        InsertTreeChild(hCurrent, info->GetChildWidgetInfo());
    }
    
    mpTree->SelectItem(hCurrent);
    GetPropsWin()->OnWidgetSelect(info);
}

///////////////////////////////////////////////////////////////////////////////
// add a folder to display:
void project_view::InsertFolder(folder_info *info)
{
    HTREEITEM hItem = mpTree->GetSelectedItem();

    HTREEITEM hCurrent = mpTree->InsertItem(info->folder_name,
        hItem, TVI_SORT);
    mpTree->SetItemData(hCurrent, (DWORD_PTR)info);
    AssignTreeImage(hCurrent, info);

    UndoManager()->AddEntry(UNDO_TYPE_INSERT_FOLDER, info, m_active_display);

    /* Insert child widgets.  */
    mpTree->SelectItem(hCurrent);

    if (info->GetFirstChildWidget())
    {
        InsertTreeChild(hCurrent, info->GetFirstChildWidget());
        GetPropsWin()->OnWidgetSelect(info->GetFirstChildWidget());
    }

    /* Force project view to redraw here to make the "+" shown.
       Sometime the "+" isn't shown when generally running,
       but it is shown when debug. So force it redraw here.*/
    Invalidate();
    UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////
// add a child widget to the indicated parent:
widget_info *project_view::InsertTreeChild(HTREEITEM hParent, widget_info *info, BOOL do_siblings)
{
    widget_info *parent_info = (widget_info *)mpTree->GetItemData(hParent);
    widget_info *sib;

    HTREEITEM hCurrent = mpTree->InsertItem(info->app_name,
        hParent, TVI_SORT);
    AssignTreeImage(hCurrent, info);
    mpTree->SetItemData(hCurrent, (DWORD_PTR)info);

    if (info->GetChildWidgetInfo())
    {
        InsertTreeChild(hCurrent, info->GetChildWidgetInfo());
    }

    if (do_siblings)
    {
        sib = info->GetNextWidgetInfo();

        while (sib)
        {
            InsertTreeChild(hParent, sib, FALSE);
            sib = sib->GetNextWidgetInfo();
        }
    }

    return parent_info;
}

///////////////////////////////////////////////////////////////////////////////
// add a child widget to the selected tree node:
widget_info *project_view::InsertTreeChild(widget_info *parent, widget_info *info, BOOL do_siblings)
{
    HTREEITEM hParent = mpTree->GetSelectedItem();
    widget_info *parent_info = (widget_info *) mpTree->GetItemData(hParent);
    widget_info *sib;

    // make sure we are in sync
    if (parent_info == parent)
    {
        HTREEITEM hCurrent = mpTree->InsertItem(info->app_name,
            hParent, TVI_SORT);
        AssignTreeImage(hCurrent, info);
        mpTree->SetItemData(hCurrent, (DWORD_PTR) info);

        if (info->GetChildWidgetInfo())
        {
            InsertTreeChild(hCurrent, info->GetChildWidgetInfo());
        }

        if (do_siblings)
        {
            sib = info->GetNextWidgetInfo();

            while(sib)
            {
                InsertTreeChild(hParent, sib, FALSE);
                sib = sib->GetNextWidgetInfo();
            }
        }
    
        mpTree->SelectItem(hCurrent);
        GetPropsWin()->OnWidgetSelect(info);
        return parent_info;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info *project_view::InsertTreeChild(folder_info *folder, widget_info *info, BOOL do_siblings)
{
    HTREEITEM hParent = mpTree->GetSelectedItem();
    folder_info *parent_folder = (folder_info *)mpTree->GetItemData(hParent);
    widget_info *sib;

    // make sure we are in sync
    if (parent_folder == folder)
    {
        HTREEITEM hCurrent = mpTree->InsertItem(info->app_name,
            hParent, TVI_SORT);
        AssignTreeImage(hCurrent, info);
        mpTree->SetItemData(hCurrent, (DWORD_PTR)info);

        if (info->GetChildWidgetInfo())
        {
            InsertTreeChild(hCurrent, info->GetChildWidgetInfo());
        }

        if (do_siblings)
        {
            sib = info->GetNextWidgetInfo();

            while (sib)
            {
                InsertTreeChild(hParent, sib, FALSE);
                sib = sib->GetNextWidgetInfo();
            }
        }

        mpTree->SelectItem(hCurrent);
        GetPropsWin()->OnWidgetSelect(info);
        return parent_folder;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
HTREEITEM project_view::FindWidget(HTREEITEM start, widget_info *widget)
{
    HTREEITEM result;

    while(start)
    {
        widget_info *info = (widget_info *) mpTree->GetItemData(start);
        if (info == widget)
        {
            return start;
        }

        HTREEITEM child = mpTree->GetChildItem(start);
        if (child)
        {
            result = FindWidget(child, widget);
            if (result)
            {
                return result;
            }
        }
        start = mpTree->GetNextSiblingItem(start);
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
HTREEITEM project_view::FindWidget(widget_info *widget)
{
    HTREEITEM root = mpTree->GetRootItem();
    HTREEITEM display = mpTree->GetChildItem(root); //display node
    HTREEITEM found;
    while (display)
    {
        HTREEITEM folder = mpTree->GetChildItem(display); //folder item
        while (folder)
        {
            HTREEITEM child = mpTree->GetChildItem(folder); //screen item
            found = FindWidget(child, widget);
            if (found)
            {
                return found;
            }
            folder = mpTree->GetNextSiblingItem(folder);
        }
        display = mpTree->GetNextSiblingItem(display);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
HTREEITEM project_view::FindProjectTreeNode(HTREEITEM start, CString name, int search_depth)
{
    HTREEITEM result;
    HTREEITEM child;
    CString   text;

    while (start)
    {
        text = mpTree->GetItemText(start);
        if (text.Compare(name) == 0)
        {
            return start;
        }

        child = mpTree->GetChildItem(start);
        if (child && search_depth)
        {
            search_depth--;
            result = FindProjectTreeNode(child, name, search_depth);
            if (result)
            {
                return result;
            }
        }

        start = mpTree->GetNextSiblingItem(start);
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
HTREEITEM project_view::FindProjectTreeNode(CString name, CString display_name, CString screen_name)
{
    if (!name)
    {
        return 0;
    }
    HTREEITEM root = mpTree->GetRootItem();
    HTREEITEM start = mpTree->GetChildItem(root); //display level
    HTREEITEM screen;
    BOOL found = FALSE;

    if (!display_name.IsEmpty())
    {
        while (start)
        {
            if (display_name == mpTree->GetItemText(start))
            {
                start = mpTree->GetChildItem(start);
                break;
            }
            start = mpTree->GetNextSiblingItem(start);
        }
    }

    if (start && !screen_name.IsEmpty())
    {
        while (start)
        {
            //loop through folders
            screen = mpTree->GetChildItem(start);
            while (screen)
            {
                //loop through screens
                if (screen_name == mpTree->GetItemText(screen))
                {
                    start = mpTree->GetChildItem(screen);
                    found = TRUE;
                    break;
                }
                screen = mpTree->GetNextSiblingItem(screen);
            }

            if (found)
            {
                break;
            }
            start = mpTree->GetNextSiblingItem(start);
        }
    }

    return FindProjectTreeNode(start, name);
}

///////////////////////////////////////////////////////////////////////////////
// this is called by WidgetSelected, when the target widget is NULl. This 
// means the user clicked on the root window.
void project_view::DisplaySelected()
{
    if (m_active_display < 0)
    {
        return;
    }
    HTREEITEM root = mpTree->GetRootItem();
    HTREEITEM display = mpTree->GetChildItem(root);
    int index = 0;

    while(display && index < m_active_display)
    {
        display = mpTree->GetNextSiblingItem(display);
        index++;
    }
    mpTree->SelectItem(display);
}


///////////////////////////////////////////////////////////////////////////////
// this is called by target screen when the user clicks on a widget.
// update tree view to match
void project_view::WidgetSelected(widget_info *info)
{
    if (info && info->widget && info->widget->gx_widget_type != GX_TYPE_ROOT_WINDOW)
    {
        HTREEITEM hSelected;
        hSelected = FindWidget(info);

        if (hSelected)
        {
            mpTree->SelectItem(hSelected);
            GetPropsWin()->OnWidgetSelect(info);
        }
    }
    else
    {
        GetPropsWin()->OnWidgetSelect(NULL);   
        DisplaySelected();
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::FolderSelected(INT display_index, CString folder_name)
{
    HTREEITEM item = mpTree->GetRootItem(); //root
    HTREEITEM display = mpTree->GetChildItem(item); //display

    int index = 0;
    /* Get target display. */
    while (display && index < display_index)
    {
        display = mpTree->GetNextSiblingItem(display);
        index++;
    }
    mpTree->SelectItem(display);

    /* Select target folder. */
    HTREEITEM folder = mpTree->GetChildItem(display); //folder
    while (folder)
    {
        if (mpTree->GetItemText(folder) == folder_name)
        {
            mpTree->SelectItem(folder);
            return;
        }
        folder = mpTree->GetNextSiblingItem(folder);
    }

}

///////////////////////////////////////////////////////////////////////////////
void project_view::FolderSelected(folder_info * folder)
{
    if (folder)
    {
        int display_index = GetOpenProject()->GetDisplayIndex(folder);
        FolderSelected(display_index, folder->folder_name);
    }
    else
    {
        GetPropsWin()->OnWidgetSelect(NULL);
        DisplaySelected();
    }
}

///////////////////////////////////////////////////////////////////////////////
void project_view::NotifyNameChange(widget_info *info)
{
    HTREEITEM hitem = mpTree->GetSelectedItem();
    widget_info *selinfo = (widget_info *) mpTree->GetItemData(hitem);
    CString name;

    if (info == selinfo)
    {
        if (!info->app_name.IsEmpty())
        {
            mpTree->SetItemText(hitem, info->app_name);
            name = info->app_name;
        }
        else
        {
            mpTree -> SetItemText(hitem, info->base_name);
            name = info->base_name;
        }

        HTREEITEM hParent = mpTree->GetParentItem(hitem);
        if (hParent)
        {
            /* Sort child to be alphabetly displayed. */
            mpTree->SortChildren(hParent);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void project_view::GetTreeState(CArray<int> *state_array)
{
    HTREEITEM root = mpTree->GetRootItem();
    HTREEITEM screen = mpTree->GetChildItem(root); // display node
    UINT      state = 0;

    while (screen)
    {
        HTREEITEM child = mpTree->GetChildItem(screen);

        state = mpTree->GetItemState(screen, TVIS_EXPANDED);

        state_array->Add(state);

        if(child)
        {
            GetTreeState(child, state_array);
        }

        screen = mpTree->GetNextSiblingItem(screen);
    }
}

///////////////////////////////////////////////////////////////////////////
void project_view::GetTreeState(HTREEITEM hParent, CArray<int> *state_array)
{
    UINT      state = 0;

    while (hParent)
    {
        HTREEITEM child = mpTree->GetChildItem(hParent);

        state = mpTree->GetItemState(hParent, TVIS_EXPANDED);

        state_array->Add(state);

        if (child)
        {
            GetTreeState(child, state_array);
        }

        hParent = mpTree->GetNextSiblingItem(hParent);
    }
}


/////////////////////////////////////////////////////////////////////////////
void project_view::SetTreeState(CArray<int> *state_array)
{
    HTREEITEM root = mpTree->GetRootItem();
    HTREEITEM screen = mpTree->GetChildItem(root); // display node

    if (!state_array)
    {
        return;
    }

    while (screen)
    {
        if (state_array->GetCount() == 0)
        {
            break;
        }

        HTREEITEM child = mpTree->GetChildItem(screen);

        if (state_array->GetAt(0))
        {
            mpTree->Expand(screen, TVE_EXPAND);
        }
        else
        {
            mpTree->Expand(screen, TVE_COLLAPSE);
        }

        state_array->RemoveAt(0);

        if (state_array->GetCount() && child)
        {
            SetTreeState(child, state_array);
        }

        screen = mpTree->GetNextSiblingItem(screen);
    }
}

///////////////////////////////////////////////////////////////////////////
void project_view::SetTreeState(HTREEITEM hParent, CArray<int> *state_array)
{
    if (!state_array)
    {
        return;
    }

    while (hParent)
    {
        if (state_array->GetCount() == 0)
        {
            break;
        }

        HTREEITEM child = mpTree->GetChildItem(hParent);

        if (state_array->GetAt(0))
        {
            mpTree->Expand(hParent, TVE_EXPAND);
        }
        else
        {
            mpTree->Expand(hParent, TVE_COLLAPSE);
        }

        state_array->RemoveAt(0);

        if (state_array->GetCount() && child)
        {
            SetTreeState(child, state_array);
        }

        hParent = mpTree->GetNextSiblingItem(hParent);
    }
}

///////////////////////////////////////////////////////////////////////////////
LRESULT project_view::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CStringArray param_list;
    LRESULT result = 0;
    CString pathname;
    CString filename;
    HTREEITEM select;
    CString path;
    CString title;
    widget_info *info;

    switch(wParam)
    {
    case TEST_OPEN_PROJECT:
        CloseProject(FALSE, FALSE);
        pathname = GetTestingParam(0);
        filename = pathname.Right(pathname.GetLength() - pathname.ReverseFind('\\') - 1);
        pathname = pathname.Left(pathname.GetLength() - filename.GetLength() - 1);
        OpenProject(pathname, filename);
        break;

    case TEST_CLOSE_PROJECT:
        CloseProject(TRUE, lParam);
        break;

    case TEST_IMPORT_PROJECT:
        pathname = GetTestingParam(1);
        filename = pathname.Right(pathname.GetLength() - pathname.ReverseFind('\\') - 1);
        pathname = pathname.Left(pathname.GetLength() - filename.GetLength() - 1);
        MergeProject(pathname, filename);
        break;

    case TEST_SELECT_PROJECT_TREE_NODE:
        select = FindProjectTreeNode(GetTestingParam(0));
        if (select)
        {
            mpTree->SelectItem(select);
            result = 1;
        }
        else
        {
            result = 0;
        }
        break;

    case TEST_SELECT_PROJECT_FOLDER:
        // parameters are display name, folder name:
        SplitString(GetTestingParam(0), ',', &param_list);
        select = FindProjectTreeNode(param_list[1], param_list[0]);
        if (select)
        {
            mpTree->SelectItem(select);
            result = 1;
        }
        else
        {
            result = 0;
        }
        break;

    case TEST_SELECT_CHILD_WIDGET:
        select = mpTree->GetSelectedItem();

        if (select)
        {
            mpTree->SelectItem(mpTree->GetChildItem(select));
            result = 1;
        }
        else
        {
            result = 0;
        }
        break;


    case TEST_SELECT_MULTI_WIDGETS:
        select = FindProjectTreeNode(GetTestingParam(0));
        if (select)
        {
            info = (widget_info *)mpTree->GetItemData(select);

            if (info)
            {
                GetTargetScreen()->MakeTopWindowVisible(info);
                GetTargetScreen()->SelectWidget(info, TRUE, TRUE);
            }
        }
        break;

    case TEST_DELETE_FOLDER:
    case TEST_DELETE_WIDGET:
        OnDeleteWidget();
        break;

    case TEST_GET_WIDGET_LEFT:
        select = FindProjectTreeNode(GetTestingParam(0));
        if (select)
        {
            info = (widget_info *)mpTree->GetItemData(select);

            if (info)
            {
                return info->size.gx_rectangle_left;
            }
        }
        break;

    case TEST_GET_WIDGET_TOP:
        select = FindProjectTreeNode(GetTestingParam(0));
        if (select)
        {
            info = (widget_info *)mpTree->GetItemData(select);

            if (info)
            {
                return info->size.gx_rectangle_top;
            }
        }
        break;

    case TEST_GET_WIDGET_RIGHT:
        select = FindProjectTreeNode(GetTestingParam(0));
        if (select)
        {
            info = (widget_info *)mpTree->GetItemData(select);

            if (info)
            {
                return info->size.gx_rectangle_right;
            }
        }
        break;

    case TEST_GET_WIDGET_BOTTOM:
        select = FindProjectTreeNode(GetTestingParam(0));
        if (select)
        {
            info = (widget_info *)mpTree->GetItemData(select);

            if (info)
            {
                return info->size.gx_rectangle_bottom;
            }
        }
        break;

    case TEST_INSERT_FOLDER:
        OnInsertFolder();
        break;

    case TEST_EDIT_FOLDER_PROPERTIES:
        select = mpTree->GetSelectedItem();
        if (GetTreeNodeLevel(select) == FOLDER_NODE_LEVEL)
        {
            OnEditFolderProperties();
        }
        break;

    case TEST_TERMINATE_APP_EXECUTION:
        TerminateAppExecutions();
        break;
    }
    
    return result;
}

///////////////////////////////////////////////////////////////////////////
void project_view::UpdateBaseMenu(CMenu *parent)
{
    HTREEITEM    root = mpTree->GetRootItem();
    HTREEITEM    display = mpTree->GetChildItem(root);
    HTREEITEM    folder;
    HTREEITEM    child;
    widget_info *info;

    CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
    CMenu      *pBaseMenu;
    CString     screen_name;
    int         index = 0;
    widget_info *selected = GetTargetScreen()->GetSingleSelect();
    int          flags;
    int          template_menu_pos = -1;

    pBaseMenu = pMain->GetBaseMenu();

    index = pBaseMenu->GetMenuItemCount();

    // clear menu items of container menu
    if (index)
    {
        template_menu_pos = parent->GetSubMenu(0)->GetMenuItemCount() - 1;

        while (index)
        {
            pBaseMenu->RemoveMenu(index - 1, MF_BYPOSITION);
            index--;
        }
    }

    index = m_active_display;
    while (index)
    {
        display = mpTree->GetNextSiblingItem(display);
        index--;
    }

    folder = mpTree->GetChildItem(display);
    while (folder)
    {
        child = mpTree->GetChildItem(folder);
        while (child)
        {
            info = (widget_info *)mpTree->GetItemData(child);

            if (info && info->is_template)
            {
                flags = MF_STRING;
                if (selected == info)
                {
                    flags |= MF_DISABLED;
                }
                pBaseMenu->AppendMenu(flags, ID_ADD_TEMPLATE_BASE + index, info->app_name);
                index++;
            }

            child = mpTree->GetNextSiblingItem(child);
        }
        folder = mpTree->GetNextSiblingItem(folder);
    }

    if (index)
    {
        if (template_menu_pos < 0)
        {
            //insert template submebu
            parent->GetSubMenu(0)->AppendMenu(MF_POPUP, (UINT)pBaseMenu->m_hMenu, _T("Template"));
        }
    }
    else
    {
        if (template_menu_pos >= 0)
        {
            //remove template submenu
            parent->GetSubMenu(0)->RemoveMenu(template_menu_pos, MF_BYPOSITION);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
BOOL project_view::CreateBaseMenu(CMenu *pBaseMenu)
{
    HTREEITEM    root = mpTree->GetRootItem();
    HTREEITEM    display = mpTree->GetChildItem(root);
    HTREEITEM    folder = mpTree->GetChildItem(display);
    HTREEITEM    child;
    widget_info *info;
    int          index;
    widget_info *selected = GetTargetScreen()->GetSingleSelect();
    int          flags;

    index = m_active_display;
    while (index)
    {
        display = mpTree->GetNextSiblingItem(display);
        index--;
    }

    folder = mpTree->GetChildItem(display);
    while (folder)
    {
        child = mpTree->GetChildItem(folder);
        while (child)
        {
            info = (widget_info *)mpTree->GetItemData(child);
            if (info->is_template)
            {
                if (index == 0)
                {
                    pBaseMenu->CreatePopupMenu();
                }

                flags = MF_STRING;
                if (selected == info)
                {
                    flags |= MF_DISABLED;
                }
                pBaseMenu->AppendMenu(flags, ID_ADD_TEMPLATE_BASE + index, info->app_name);
                index++;
            }
            child = mpTree->GetNextSiblingItem(child);
        }

        folder = mpTree->GetNextSiblingItem(folder);
    }

    if (index)
    {
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////
GX_BOOL project_view::IsTopLevelWidget(widget_info *widgetinfo)
{
    //Check if a widget is in top level
    HTREEITEM       root = mpTree->GetRootItem();
    HTREEITEM       display = mpTree->GetChildItem(root);
    HTREEITEM       child;
    HTREEITEM       folder;
    widget_info    *info;

    while (display)
    {
        folder = mpTree->GetChildItem(display);
        while (folder)
        {
            child = mpTree->GetChildItem(folder);
            while (child)
            {
                info = (widget_info *)mpTree->GetItemData(child);

                if (info == widgetinfo)
                {
                    return TRUE;
                }

                child = mpTree->GetNextSiblingItem(child);
            }
            folder = mpTree->GetNextSiblingItem(folder);
        }
        display = mpTree->GetNextSiblingItem(display);
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////
void project_view::GetTopLevelWidgetList(int display_index, CArray<widget_info *> *list)
{
    HTREEITEM       root = mpTree->GetRootItem();
    HTREEITEM       screen = mpTree->GetChildItem(root); // display level
    HTREEITEM       child;
    widget_info    *info;

    list->RemoveAll();

    while (display_index)
    {
        screen = mpTree->GetNextSiblingItem(screen);
        display_index--;
    }

    screen = mpTree->GetChildItem(screen);  // folder level
    while (screen)
    {
        child = mpTree->GetChildItem(screen);
        while (child)
        {
            info = (widget_info *)mpTree->GetItemData(child);

            list->Add(info);

            child = mpTree->GetNextSiblingItem(child);
        }
        screen = mpTree->GetNextSiblingItem(screen);
    }

}

///////////////////////////////////////////////////////////////////////////
widget_info *project_view::FindTopLevelWidget(widget_info *child)
{
    HTREEITEM       root = mpTree->GetRootItem();
    HTREEITEM       display = mpTree->GetChildItem(root);
    HTREEITEM       folder;
    HTREEITEM       screen;
    HTREEITEM       child_screen;
    widget_info    *found = NULL;
    widget_info    *info;

    while(display)
    {
        folder = mpTree->GetChildItem(display);
        while (folder)
        {
            screen = mpTree->GetChildItem(folder);
            while (screen)
            {
                info = (widget_info *)mpTree->GetItemData(screen);

                if (info == child)
                {
                    found = info;
                    break;
                }

                child_screen = mpTree->GetChildItem(screen);

                while (child_screen)
                {
                    if (FindWidget(child_screen, child))
                    {
                        found = info;
                        break;
                    }

                    child_screen = mpTree->GetNextSiblingItem(child_screen);
                }

                screen = mpTree->GetNextSiblingItem(screen);
            }
            if (found)
            {
                break;
            }
            folder = mpTree->GetNextSiblingItem(folder);
        }
        display = mpTree->GetNextSiblingItem(display);
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
VOID project_view::OnLockUlockWidgetPositions()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        if (project->IsWidgetPositionLocked())
        {
            project->LockUlockWidgetPositions(FALSE);
        }
        else
        {
            project->LockUlockWidgetPositions(TRUE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL project_view::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_SYSKEYDOWN)
    {
        int nAltKey = GetKeyState(VK_MENU);

        switch (pMsg->wParam)
        {
        case VK_DOWN:
            if (nAltKey & KEY_STATE_DOWN)
            {
                //Move focus to property window
                properties_win* props = GetPropsWin();
                if (props)
                {
                    props->SetFocus();
                    return TRUE;
                }
            }
            break;

        case VK_RIGHT:
            if (nAltKey & KEY_STATE_DOWN)
            {
                target_view* view = GetTargetView();
                if (view)
                {
                    CMainFrame* frame = (CMainFrame*)GetParentFrame();
                    frame->SetActiveView(view);
                    return TRUE;
                }
            }
            break;

        default:
            break;
        }
    }

    return CTreeView::PreTranslateMessage(pMsg);
}
