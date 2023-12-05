#include "studiox_includes.h"
#include "import_sprite_frames_dlg.h"

enum import_sprite_frames_dlg_test_commands {
    TEST_SAVE = 1,
    TEST_CANCEL
};

IMPLEMENT_DYNAMIC(import_sprite_frames_dlg, express_dialog)

BEGIN_MESSAGE_MAP(import_sprite_frames_dlg, express_dialog)
    ON_LBN_SELCHANGE(IDC_PIX_FOLDER_LIST, OnSelectPixFolder)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
import_sprite_frames_dlg::import_sprite_frames_dlg(CWnd *pParent)
    : express_dialog(import_sprite_frames_dlg::IDD, pParent)
{
    IconId = IDB_PIXELMAPS;
    SetTitleText("Import Sprite Frames");
    mPixGroupInfo = NULL;
}

///////////////////////////////////////////////////////////////////////////////
import_sprite_frames_dlg::~import_sprite_frames_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
BOOL import_sprite_frames_dlg::OnInitDialog()
{
	express_dialog::OnInitDialog();

    AddCancelButton();
    AddSaveButton(_T("Import"));
    SetSavedMsg(L"Imported");

    InitPixFolderList();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void import_sprite_frames_dlg::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_PIX_FOLDER_LIST, mPixFolderList);
    DDX_Control(pDX, IDC_FRAME_LIST, mFrameList);

    express_dialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
void import_sprite_frames_dlg::OnOK()
{
    bool select_all = false;

    if (mFrameList.GetSelCount() <= 0)
    {
        select_all = true;
    }

    CString name;

    for (int index = 0; index < mFrameList.GetCount(); index++)
    {
        if ((mFrameList.GetSel(index) > 0) || select_all)
        {
            mFrameList.GetText(index, name);
            mFrameNameList.Add(name);
        }
    }

    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
void import_sprite_frames_dlg::OnSelectPixFolder()
{
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    int sel = mPixFolderList.GetCurSel();
    int folder_id = mPixFolderList.GetItemData(sel);
    CString folder_name;
    mPixFolderList.GetText(sel, folder_name);

    res_info *info = project->FindResourceFolder(mPixGroupInfo, RES_TYPE_FOLDER, folder_id, folder_name);
    UpdateFrameList(info);
}

///////////////////////////////////////////////////////////////////////////////
void import_sprite_frames_dlg::UpdateFrameList(res_info *folder_info)
{
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    res_info* child = folder_info->child;

    mFrameList.ResetContent();

    while (child)
    {
        if (child->type == RES_TYPE_PIXELMAP)
        {
            mFrameList.AddString(child->name);
        }
        child = child->next;
    }

    UpdateListBoxHorizontalExtend(&mFrameList);
    mFrameList.SelItemRange(true, 0, mFrameList.GetCount() - 1);
}

///////////////////////////////////////////////////////////////////////////////
void import_sprite_frames_dlg::InitPixFolderList()
{
    studiox_project* project = GetOpenProject();

    if (!project || !GetProjectView())
    {
        return;
    }

    int display = GetProjectView()->GetActiveDisplay();
    int theme = project->mDisplays[display].active_theme;

    mPixGroupInfo = project->FindResource(display, theme, RES_TYPE_GROUP, L"Pixelmaps");

    INT sel;

    res_info *info = mPixGroupInfo->child;

    while(info)
    {
        if (info->type == RES_TYPE_FOLDER)
        {
            sel = mPixFolderList.AddString(info->name);
            mPixFolderList.SetItemData(sel, info->folder_id);
        }

        info = info->next;
    }
    UpdateListBoxHorizontalExtend(&mPixFolderList);
    mPixFolderList.SetCurSel(0);
    OnSelectPixFolder();
}

///////////////////////////////////////////////////////////////////////////////
void import_sprite_frames_dlg::UpdateListBoxHorizontalExtend(CListBox* pList)
{
    // Find the longest string in the list box.
    CString    str;
    CSize      sz;
    int        dx = 0;
    TEXTMETRIC tm;
    CDC* pDC = pList->GetDC();
    CFont* pFont = pList->GetFont();

    // Select the listbox font, save the old font
    CFont* pOldFont = pDC->SelectObject(pFont);
    // Get the text metrics for avg char width
    pDC->GetTextMetrics(&tm);

    for (int index = 0; index < pList->GetCount(); index++)
    {
        pList->GetText(index, str);
        sz = pDC->GetTextExtent(str);

        // Add the avg width to prevent clipping
        sz.cx += tm.tmAveCharWidth;

        if (sz.cx > dx)
            dx = sz.cx;
    }
    // Select the old font back into the DC
    pDC->SelectObject(pOldFont);
    pList->ReleaseDC(pDC);

    // Set the horizontal extent so every character of all strings 
    // can be scrolled to.
    pList->SetHorizontalExtent(dx);
}

//////////////////////////////////////////////////////////////////////////////
LRESULT import_sprite_frames_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{

    switch (wParam)
    {
    case TEST_SAVE:
        OnOK();
        break;

    case TEST_CANCEL:
        OnCancel();
        break;
    }

    return 0;
}

