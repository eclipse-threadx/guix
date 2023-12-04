// import_project_dlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "copy_paste_engine.h"

// import_project_dlg dialog

IMPLEMENT_DYNAMIC(import_project_dlg, express_dialog)

BEGIN_MESSAGE_MAP(import_project_dlg, express_dialog)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

enum import_project_dlg_test_commands{
    TEST_IMPORT = 1,
    TEST_CANCEL,
    TEST_CHECK_SCREEN
};

///////////////////////////////////////////////////////////////////////////////
import_project_dlg::import_project_dlg(studiox_project *merge_project, CString project_path, CWnd* pParent /*=NULL*/)
: express_dialog(import_project_dlg::IDD, pParent)
{
    IconId = IDB_PIXELMAPS;
    SetTitleText("Import Project");

    mpMergeProject = merge_project;
    mProjectPath = project_path;
}

///////////////////////////////////////////////////////////////////////////////
import_project_dlg::~import_project_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
void import_project_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROJECT_TREE, mProjectTree);

    if (pDX->m_bSaveAndValidate)
    {

    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL import_project_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    SetDlgItemText(IDC_MERGE_PROJECT_NAME, mpMergeProject->mHeader.project_name);

    AddCancelButton();
    AddSaveButton(_T("Import"));
    SetSavedMsg(L"Imported");

    BuildProjectTree();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// import project must specify the target folder
void import_project_dlg::OnOK()
{
    HTREEITEM hRoot = mProjectTree.GetRootItem();
    HTREEITEM hParent;
    HTREEITEM hDisplay;
    HTREEITEM hItem;
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    hParent = mProjectTree.FindItem(hRoot, _T("Displays"));
    int state = mProjectTree.GetItemCheckState(hParent);

    if (state == CUSTOM_TV_STATE_UNCHECKED)
    {
        return express_dialog::OnOK();
    }

    copy_paste_engine cpe;
    extern CString gOpenProjectDir;
    CString oldpathname = gOpenProjectDir;
    folder_info *selected = GetProjectView()->GetSelectedFolder();
    if (selected)
    {
        for (int display = 0; display < mpMergeProject->mHeader.num_displays; display++)
        {
            hDisplay = mProjectTree.FindItem(hParent, mpMergeProject->mDisplays[display].name);
            state = mProjectTree.GetItemCheckState(hDisplay);

            if (state == CUSTOM_TV_STATE_UNCHECKED)
            {
                continue;
            }

            folder_info *folder = mpMergeProject->mDisplays[display].GetFirstChildFolder();
            CArray<widget_info *> info_list;

            while (folder)
            {
                widget_info *info = folder->GetFirstChildWidget();
                while (info)
                {
                    hItem = mProjectTree.FindItem(hDisplay, info->app_name);
                    state = mProjectTree.GetItemCheckState(hItem);

                    if (state == CUSTOM_TV_STATE_CHECKED)
                    {
                        info_list.Add(info);
                    }
                    info = info->GetNextWidgetInfo();
                }
                folder = folder->GetNextFolder();
            }

            /* Should not do copy-paste while no income screen. */
            if (info_list.GetCount())
            {
                SetProjectDirectory(mProjectPath);
                cpe.SetProject(mpMergeProject);
                cpe.CopyToClipboard(info_list, m_hWnd);

                SetProjectDirectory(oldpathname);
                cpe.SetProject(project);
                // Should select the target folder here.
                GetProjectView()->FolderSelected(selected);
                GetProjectView()->OnPasteWidget();
            }
        }
    }
    express_dialog::OnOK();
}


///////////////////////////////////////////////////////////////////////////////
void import_project_dlg::BuildProjectTree()
{
    mProjectTree.InitImageList();

    HTREEITEM hParent;
    HTREEITEM hDisplay;
    HTREEITEM hItem;
    HTREEITEM screen;

    hParent = mProjectTree.InsertItem(_T("Displays"));
    mProjectTree.SetItemState(hParent, CUSTOM_TV_STATE_CHECKED);

    for (int display = 0; display < mpMergeProject->mHeader.num_displays; display++)
    {
        hDisplay = mProjectTree.InsertItem(mpMergeProject->mDisplays[display].name, hParent);
        mProjectTree.SetItemState(hDisplay, CUSTOM_TV_STATE_CHECKED);

        folder_info *folder = mpMergeProject->mDisplays[display].GetFirstChildFolder();

        while (folder)
        {
            hItem = mProjectTree.InsertItem(folder->folder_name, hDisplay, TVI_SORT);
            mProjectTree.SetItemState(hItem, CUSTOM_TV_STATE_CHECKED);

            widget_info *info = folder->GetFirstChildWidget();

            while (info)
            {
                screen = mProjectTree.InsertItem(info->app_name, hItem);
                mProjectTree.SetItemState(screen, CUSTOM_TV_STATE_CHECKED);
                info = info->GetNextWidgetInfo();
            }
            mProjectTree.Expand(hItem, TVE_EXPAND);
            folder = folder->GetNextFolder();
        }
        

        mProjectTree.Expand(hDisplay, TVE_EXPAND);
    }
    mProjectTree.Expand(hParent, TVE_EXPAND);

    mProjectTree.SelectSetFirstVisible(mProjectTree.GetRootItem());
}

///////////////////////////////////////////////////////////////////////////////
LRESULT import_project_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    HTREEITEM item;
    CStringArray param;
    CString parent_name;
    CString resource_name;
    int check;

    switch (wParam)
    {
    case TEST_IMPORT:
        OnOK();
        break;

    case TEST_CANCEL:
        OnCancel();
        break;

    case TEST_CHECK_SCREEN:
        SplitString(GetTestingParam(0), ',', &param);
        parent_name = param.GetAt(0);
        resource_name = param.GetAt(1);
        check = _tstoi(param.GetAt(2));
        item = mProjectTree.FindItem(mProjectTree.GetRootItem(), parent_name);
        if (item)
        {
            item = mProjectTree.FindItem(item, resource_name);

            if (check)
            {
                mProjectTree.SetItemState(item, CUSTOM_TV_STATE_CHECKED);
            }
            else
            {
                mProjectTree.SetItemState(item, CUSTOM_TV_STATE_UNCHECKED);
            }

            mProjectTree.UpdateChildrenState(item);
        }
        break;
    }

    return 0;
}