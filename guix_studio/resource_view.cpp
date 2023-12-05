
#include "studiox_includes.h"
#include "font_path_dialog.h"
#include "edit_pixelmap_dlg.h"
#include "folder_name_dlg.h"
#include "string_table_edit_dlg.h"
#include "config_languages_dlg.h"
#include "color_edit_dialog.h"
#include "resource_view.h"
#include "resource_view_provider.h"
#include "resource_item_provider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define VALID_PROJECT                            \
    studiox_project *project = GetOpenProject(); \
    if (!project) return;

extern CString target_class_name;

#define DRAG_MOVE_TIMER_ID   1

enum resource_view_test_commands {
    TEST_CLICK_RESOURCE_GROUP = 1,
    TEST_CLICK_PIXELMAP_FOLDER,
    TEST_CLICK_RESOURCE_ITEM,
    TEST_ADD_COLOR,
    TEST_EDIT_COLOR,
    TEST_DELETE_COLOR,
    TEST_ADD_FONT,
    TEST_EDIT_FONT,
    TEST_DELETE_FONT,
    TEST_ADD_PIXELMAPS,
    TEST_EDIT_PIXELMAP,
    TEST_EDIT_PIXELMAPS,
    TEST_DELETE_PIXELMAP,
    TEST_ENABLE_PIXELMAP,
    TEST_DISABLE_PIXELMAP,
    TEST_ADD_PIXELMAP_FOLDER,
    TEST_REMOVE_PIXELMAP_FOLDER,
    TEST_ENABLE_PIXELMAP_FOLDER,
    TEST_DISABLE_PIXELMAP_FOLDER,
    TEST_SET_FOLDER_NAME,
    TEST_SAVE_FOLDER_NAME_EDIT,
    TEST_EDIT_STRING,
    TEST_INCREMENT_ACTIVE_LANGUAGE_INDEX,
    TEST_DECREMENT_ACTIVE_LANGUAGE_INDEX,
    TEST_GENERATE_XML
};

STRING_VAL_PAIR paste_conflict_options[] = {
    { _T("Use Source Resource"), PASTE_CONFLICT_OPTION_USE_SOURCE },
    { _T("Use Destination Resource"), PASTE_CONFLICT_OPTION_USE_DESTINATION },
    { _T("Auto Rename The Resource ID"), PASTE_CONFLICT_OPTION_RENAME },
    { _T("Abort the paste operation"), PASTE_CONFLICT_OPTION_ABORT },
    { _T(""), 0 }
};

IMPLEMENT_DYNCREATE(resource_view, CScrollView)

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(resource_view, CScrollView)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()

    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()

    ON_COMMAND(ID_ADD_CUSTOM_COLOR, OnAddCustomColor)
    ON_COMMAND(ID_EDIT_SYSTEM_COLOR, OnEditSystemColor)
    ON_COMMAND(ID_EDITCOLOR, OnEditCustomColor)
    ON_COMMAND(ID_DELETE_COLOR, OnDeleteCustomColor)
    ON_COMMAND(ID_EDITFONT, OnEditFont)
    ON_COMMAND(ID_DELETEFONT, OnDeleteFont)

    ON_COMMAND(ID_ADD_PIXELMAP, OnAddPixelmaps)
    ON_COMMAND(ID_EDIT_PIXELMAPS, OnEditPixelmaps)
    ON_COMMAND(ID_ADD_PIXELMAP_FOLDER, OnAddPixelmapFolder)
    ON_COMMAND(ID_REMOVE_PIXELMAP_FOLDER, OnRemovePixelmapFolder)
    ON_COMMAND(ID_RENAME_PIXELMAP_FOLDER, OnRenamePixelmapFolder)
    ON_COMMAND(ID_ENABLE_PIXELMAP_FOLDER, OnEnablePixelmapFolder)
    ON_COMMAND(ID_DISABLE_PIXELMAP_FOLDER, OnDisablePixelmapFolder)
    ON_COMMAND(ID_PIXELMAP_EDIT, OnEditPixelmap)
    ON_COMMAND(ID_PIXELMAP_DELETE, OnDeletePixelmap)
    ON_COMMAND(ID_PIXELMAP_ENABLE, OnEnablePixelmap)
    ON_COMMAND(ID_PIXELMAP_DISABLE, OnDisablePixelmap)
    ON_COMMAND(ID_GENERATE_XML, OnGenerateXML)

    ON_COMMAND(ID_EDIT_STRING_TABLE, OnEditStringTable)
    ON_COMMAND(ID_EDIT_LANGUAGES, OnEditLanguages)
    //ON_COMMAND(ID_SET_ACTIVE_LANGUAGE, OnSetActiveLanguage)
    ON_MESSAGE(USR_MSG_REBUILD_STIRNG_ITEMS, OnRebuildStringItems)
    ON_MESSAGE(USR_MSG_UPDATE_STRING_TABLE_FONTS, OnUpdateStringTableFonts)
    ON_MESSAGE(USR_MSG_OPEN_RESOURCE_ITEM, OnOpenResourceItem)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
    ON_WM_TIMER()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CONTEXTMENU()
    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
resource_view::resource_view()
{
    mpTree = NULL;
    mDisplayIndex = -1;
    mpCurrentItem = NULL;
    mpDragItem = NULL;
    mpPalette = NULL;
    mNewResourcePasted = FALSE;

    m_pResViewProvider = NULL;
}

///////////////////////////////////////////////////////////////////////////////
resource_view::~resource_view()
{
    if (mpTree)
    {
        delete mpTree;
    }

    if (m_pResViewProvider)
    {
        UiaDisconnectProvider(m_pResViewProvider);
        m_pResViewProvider->Release();
        m_pResViewProvider = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CScrollView::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int resource_view::OnCreate(LPCREATESTRUCT lpcs)
{
    CScrollView::OnCreate(lpcs);
//    p_tree = &GetTreeCtrl();

    CMainFrame *frame = (CMainFrame *) GetParentFrame();
    frame ->SetResourceView(this);
    SetWindowText(_T("Resource View"));
    mpTree = new resource_tree(this);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnInitialUpdate()
{
    OnTreeSizeChange();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnDraw(CDC *pDC)
{
    CView::OnDraw(pDC);
    mpTree->Paint(pDC);
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::OnEraseBkgnd(CDC *pDC)
{
    /*
    CBrush redbrush(RGB(88, 88, 88));
    CBrush *pOldBrush = pDC->SelectObject(&redbrush);

    RECT size;
    GetClientRect(&size);
    pDC->Rectangle(&size);
    pDC->SelectObject(pOldBrush);
    */
    return CScrollView::OnEraseBkgnd(pDC);
    //return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnSize(UINT nType, int cx, int cy)
{
    CScrollView::OnSize(nType, cx, cy);
    mpTree->PositionItems();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnLButtonDown(UINT di, CPoint cp)
{
    cp.y += GetScrollPos(SB_VERT);
    resource_item *item = mpTree->FindItem(mpTree->GetRoot(), cp);
    int command = 0;

    if (item)
    {
        SetCurrentItem(item);

        int item_type = item->GetResType();

        switch(item_type)
        {
        case RES_TYPE_HEADER:
            if (item->mpRes->folder_id == THEME_HEADER)
            {
                command = item->CheckClickCommand(cp);

                switch (command)
                {
                case CMD_INC_THEME:
                    OnSetActiveTheme(1);
                    break;

                case CMD_DEC_THEME:
                    OnSetActiveTheme(-1);
                    break;
                }
            }
            break;

        case RES_TYPE_GROUP:
            if (item->IsOpen())
            {
                if (item->mpRes->folder_id == STRING_GROUP)
                {
                    command = item->CheckClickCommand(cp);
                    
                    if (command)
                    {
                        switch(command)
                        {
                        case CMD_INC_LANGUAGE:
                            OnSetActiveLanguage(1);
                            break;

                        case CMD_DEC_LANGUAGE:
                            OnSetActiveLanguage(-1);
                            break;
                        }
                        break;
                    }
                }
                item->Close();
            }
            else
            {
                item->Open();
            }
            break;

        case RES_TYPE_FOLDER:
            if (item->IsOpen())
            {
                item->Close();
            }
            else
            {
                item->Open();
            }

            mpTree->PositionItems();
            break;

        case RES_TYPE_COLOR:
        case RES_TYPE_FONT:
        case RES_TYPE_PIXELMAP:
        case RES_TYPE_STRING:
            mpDragItem = item;
            SetTimer(DRAG_MOVE_TIMER_ID, 75, NULL);
            mTimerTicks = 0;

            SetCapture();
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
            break;

        case RES_TYPE_ADD_FONT:
            OnAddFont();
            break;

        case RES_TYPE_ADD_COLOR:
            OnAddCustomColor();
            break;

        case RES_TYPE_ADD_PIXELMAP:
            OnAddPixelmaps();
            break;

        case RES_TYPE_ADD_STRING:
            OnEditStringTable();
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnLButtonUp(UINT nFlags, CPoint point)
{
    BOOL Shifted = FALSE;

    if (nFlags & MK_SHIFT)
    {
        Shifted = TRUE;
    }
    VALID_PROJECT
   
    if (GetCapture() == this)
    {
        ReleaseCapture();

        if (mpDragItem)
        {
            resource_item *new_parent = mpTree->FindItem(mpTree->GetRoot(), CPoint(point.x, point.y + GetScrollPos(SB_VERT)));
            INT parent_id = DEFAULT_PIXELMAP_FOLDER;

            if (mpDragItem->Parent())
            {
                parent_id = mpDragItem->Parent()->mpRes->folder_id;
            }

            if (new_parent && (parent_id != DEFAULT_PIXELMAP_FOLDER))
            {
                /* Draw resource file from one folder to another. */

                if (new_parent->mpRes)
                {
                    if (new_parent->mpRes->type == RES_TYPE_PIXELMAP)
                    {
                        new_parent = new_parent->Parent();
                    }

                    if ((new_parent != mpDragItem->Parent()) &&
                        (new_parent->mpRes->type == RES_TYPE_FOLDER) &&
                        (new_parent->mpRes->folder_id == CUSTOM_PIXELMAP_FOLDER))
                    {
                        ChangeItemParent(mpDragItem, new_parent);

                        project->SetModified();
                    }
                }
            }
            else
            {
                /* Drag resource file to target screen. */
                ClientToScreen(&point);
                GetTargetScreen()->TestDropItem(mpDragItem, point, Shifted);
            }
        }
    }
    mpDragItem = NULL;
    KillTimer(DRAG_MOVE_TIMER_ID);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::PopupMenu(int MenuId, CPoint &pos)
{
    VALID_PROJECT
    resource_item *item = mpCurrentItem;
    res_info *info = item->mpRes;
	CMenu menu;
	CMenu* pPopup;

    menu.LoadMenu(MenuId);

    if (MenuId == IDR_PIX_SUBFOLDER_MENU)
    {
        if (info->folder_id == PIXELMAP_GROUP)
        {
            menu.EnableMenuItem(ID_ADD_PIXELMAP, MF_DISABLED);
            menu.EnableMenuItem(ID_ADD_PIXELMAP_FOLDER, MF_ENABLED);
            menu.EnableMenuItem(ID_REMOVE_PIXELMAP_FOLDER, MF_DISABLED);
            menu.EnableMenuItem(ID_RENAME_PIXELMAP_FOLDER, MF_DISABLED);
        }
        else if (info->folder_id == DEFAULT_PIXELMAP_FOLDER)
        {
            menu.EnableMenuItem(ID_ADD_PIXELMAP, MF_DISABLED);
            menu.EnableMenuItem(ID_EDIT_PIXELMAPS, MF_ENABLED);
            menu.EnableMenuItem(ID_ADD_PIXELMAP_FOLDER, MF_DISABLED);
            menu.EnableMenuItem(ID_REMOVE_PIXELMAP_FOLDER, MF_DISABLED);
            menu.EnableMenuItem(ID_RENAME_PIXELMAP_FOLDER, MF_DISABLED);
        }
        else
        {
            menu.EnableMenuItem(ID_ADD_PIXELMAP, MF_ENABLED);
            menu.EnableMenuItem(ID_ADD_PIXELMAP_FOLDER, MF_DISABLED);
            menu.EnableMenuItem(ID_REMOVE_PIXELMAP_FOLDER, MF_ENABLED);
            menu.EnableMenuItem(ID_RENAME_PIXELMAP_FOLDER, MF_ENABLED);
        }

        int enable_menu_state;
        int disable_menu_state;

        switch (GetFolderEnableDisableState(info))
        {
        case RES_STATE_ENABLED:
            enable_menu_state = MF_CHECKED;
            disable_menu_state = MF_UNCHECKED;
            break;

        case RES_STATE_DISABLED:
            enable_menu_state = MF_UNCHECKED;
            disable_menu_state = MF_CHECKED;
            break;

        case RES_STATE_UNDETERMINED:
        default:
            enable_menu_state = MF_UNCHECKED;
            disable_menu_state = MF_UNCHECKED;
            break;

        }

        menu.CheckMenuItem(ID_ENABLE_PIXELMAP_FOLDER, enable_menu_state);
        menu.CheckMenuItem(ID_DISABLE_PIXELMAP_FOLDER, disable_menu_state);
    }
    else
    {
        if (info->enabled)
        {
            menu.CheckMenuItem(ID_PIXELMAP_ENABLE, MF_CHECKED);
            menu.CheckMenuItem(ID_PIXELMAP_DISABLE, MF_UNCHECKED);
        }
        else
        {
            menu.CheckMenuItem(ID_PIXELMAP_ENABLE, MF_UNCHECKED);
            menu.CheckMenuItem(ID_PIXELMAP_DISABLE, MF_CHECKED);
        }
    }


	pPopup = menu.GetSubMenu(0);
	ClientToScreen(&pos);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    VALID_PROJECT
    CPoint treepoint = point;
    treepoint.y += GetScrollPos(SB_VERT);
    resource_item *item = mpTree->FindItem(mpTree->GetRoot(), treepoint);
    int item_type = RES_TYPE_STRING;

    if (!item)
    {
        return;
    }

    SetCurrentItem(item);

    if (item->mpRes)
    {
        item_type = item->mpRes->type;
    }

    switch(item_type)
    {
    case RES_TYPE_COLOR:
        if (project->GetResourceId(mDisplayIndex, item->mpRes) >= GX_COLOR_ID_CANVAS)
        {
            if (item->mpRes->is_default)
            {
                OnEditSystemColor();
            }
            else
            {
                OnEditCustomColor();
            }
        }
        break;

    case RES_TYPE_FONT:
        OnEditFont();
        break;

    case RES_TYPE_PIXELMAP:
        OnEditPixelmap();
        break;

    case RES_TYPE_STRING:
        OnEditStringTable();
        break;      
           
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnContextMenu(CWnd* pWnd, CPoint point)
{
    // TODO: Add your message handler code here
    VALID_PROJECT

    if (!mpCurrentItem)
    {
        return;
    }

    CPoint pt;

    if (point.x == -1 && point.y == -1)
    {
        // The contet menu is generated from the keyboard, for example the user types Shift+F10
        // Pop up menu under selected item
        CRect size = mpCurrentItem->GetPos();
        pt.x = size.left;
        pt.y = size.bottom - GetScrollPos(SB_VERT);;
    }
    else
    {
        pt.x = point.x;
        pt.y = point.y;
    }

    switch (mpCurrentItem->GetResType())
    {
    case RES_TYPE_COLOR:
        if (project->GetResourceId(mDisplayIndex, mpCurrentItem->mpRes) >= GX_COLOR_ID_CANVAS)
        {
            if (mpCurrentItem->mpRes->is_default)
            {
                PopupMenu(IDR_SYSTEM_COLOR_MENU, pt);
            }
            else
            {
                PopupMenu(IDR_CUSTOM_COLOR_MENU, pt);
            }
        }
        break;

    case RES_TYPE_FONT:
        if (mpCurrentItem->mpRes->is_default)
        {
            PopupMenu(IDR_DEFAULT_FONT_MENU, pt);
        }
        else
        {
            PopupMenu(IDR_FONT_SUBMENU, pt);
        }
        break;

    case RES_TYPE_PIXELMAP:
        PopupMenu(IDR_PIXELMAP_MENU, pt);
        break;

    case RES_TYPE_FOLDER:
    case RES_TYPE_GROUP:
        if (mpCurrentItem->mpRes->folder_id == STRING_GROUP)
        {
            PopupMenu(IDR_STRING_MENU, pt);
        }
        else if ((mpCurrentItem->mpRes->folder_id == PIXELMAP_GROUP) ||
                 (mpCurrentItem->mpRes->folder_id == CUSTOM_PIXELMAP_FOLDER) ||
                 (mpCurrentItem->mpRes->folder_id == DEFAULT_PIXELMAP_FOLDER))
        {
            PopupMenu(IDR_PIX_SUBFOLDER_MENU, pt);
        }
        break;

    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnRButtonDown(UINT nFlags, CPoint point)
{
    CPoint treepoint = point;
    treepoint.y += GetScrollPos(SB_VERT);
    resource_item *item = mpTree->FindItem(mpTree->GetRoot(), treepoint);

    if (!item)
    {
        return;
    }

    SetCurrentItem(item);

    SendMessage(WM_CONTEXTMENU, (WPARAM)m_hWnd, point.x | (point.y << 16));
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnOpenProject()
{
    mpTree->DeleteAllItems();
    mDisplayIndex = -1;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::AddStringItems(resource_item *parent)
{
    VALID_PROJECT

    if (mDisplayIndex < 0)
    {
        return;
    }

    int StringId;
    int NumStrings;
    int language = 0;

    string_table *pt = project->mDisplays[mDisplayIndex].stable;

    if (pt)
    {
        language = pt->GetActiveLanguage();
        NumStrings = pt->CountStrings();

        for (StringId = 1; StringId < NumStrings; StringId++)
        {
            mpTree->InsertItem(parent, pt->GetStringId(StringId), pt->GetString(StringId, language));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::RebuildStringItems(BOOL update_GUIX)
{
    VALID_PROJECT
    string_table *table = project->mDisplays[mDisplayIndex].stable;

    if (table)
    {
        // first, get rid of the old string items:
        resource_item *string_group = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_GROUP, STRING_GROUP);

        if (string_group)
        {
            CRect size = string_group->GetPos();

            if (mpCurrentItem)
            {
                //get position of current item
                resource_item* item = string_group->First();

                while (item)
                {
                    if (mpCurrentItem == item)
                    {
                        size = mpCurrentItem->GetPos();
                        SetCurrentItem(NULL);
                        break;
                    }

                    item = item->Next();
                }
            }

            int scrollpos = GetScrollPos(SB_VERT);

            while(string_group->First())
            {
                mpTree->DeleteItem(string_group->First(), FALSE);
            }

            // now re-add all the string items
            AddStringItems(string_group);

            // Add string add item
            res_info* pStringAdd = new res_info(RES_TYPE_ADD_STRING);
            string_group->mpRes->Attach(pStringAdd);
            mpTree->InsertItem(string_group, pStringAdd);

            mpTree->PositionItems();
            SetScrollPos(SB_VERT, scrollpos, TRUE);

            if (mpCurrentItem == NULL)
            {
                CPoint cp;
                cp.x = size.left;
                cp.y = ((size.top + size.bottom) >> 1);
                resource_item *item = mpTree->FindItem(mpTree->GetRoot(), cp);

                while (!item)
                {
                    item = mpTree->FindItem(mpTree->GetRoot(), cp);
                    cp.y -= size.Height();
                }

                if (item)
                {
                    SetCurrentItem(item);
                }
            }
        }
        else
        {
            ErrorMsg("Internal Error: resource view string group not found");
        }

        if (update_GUIX)
        {
            // our tree view is now updated, update the GUIX side of things
            table->UpdateGuixLanguageTable();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::InsertTreeResources(resource_item *parent, res_info *start)
{
    while(start)
    {
        resource_item *current = mpTree->InsertItem(parent, start);

        switch(start->type)
        {
        case RES_TYPE_HEADER:
            if (current->mpRes->folder_id == THEME_HEADER)
            {
                current->SetIcon(IDB_CONFIGURE_THEMES, IDB_CONFIGURE_THEMES_HIGHLIGHT);
            }
            break;

        case RES_TYPE_GROUP:
            switch(current->mpRes->folder_id)
            {
            case COLOR_GROUP:
                current->SetIcon(IDB_COLORS, IDB_COLORS_HIGHLIGHT);
                break;

            case FONT_GROUP:
                current->SetIcon(IDB_FONTS, IDB_FONTS_HIGHLIGHT);
                break;

            case PIXELMAP_GROUP:
                current->SetIcon(IDB_PIXELMAPS, IDB_PIXELMAPS_HIGHLIGHT);
                break;

            case STRING_GROUP:
                current->SetIcon(IDB_STRINGS, IDB_STRINGS_HIGHLIGHT);

                if (mDisplayIndex >= 0)
                {
                    AddStringItems(current);
                }
                break;
            }
            break;

        case RES_TYPE_FOLDER:
            current->SetIcon(IDB_FOLDER_CLOSE, 0);
            break;

        case RES_TYPE_FONT:
            break;

        case RES_TYPE_COLOR:
            current->SetColorValue(start->colorval);
            break;

        case RES_TYPE_PIXELMAP:
        case RES_TYPE_STRING:
        default:
            break;
        }
        if (start->child)
        {
            InsertTreeResources(current, start->child);
        }
        start = start->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_COLOR resource_view::GetNativeColor(GX_COLOR rgb_color, int color_format)
{
    GX_COLOR native_color = 0;

    switch (color_format)
    {
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        // the incoming color is really just a palette index, not rgb color
        native_color = rgb_color;
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        image_reader::ConvertRGBToGray(rgb_color, (GX_UBYTE *)&native_color);
        if ((native_color & 0xff) < 128)
        {
            native_color = 0x00;
        }
        else
        {
            native_color = 0x01;
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        image_reader::ConvertRGBToGray(rgb_color, (GX_UBYTE *)&native_color);
        native_color >>= 4;
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        native_color = ((rgb_color & 0xe00000) >> 16) | ((rgb_color & 0x00e000) >> 11) | ((rgb_color & 0x0000c0) >> 6);
        break;

    case GX_COLOR_FORMAT_5551BGRX:
        native_color = (((rgb_color & 0xf8) << 8) | ((rgb_color & 0xf800) >> 5) | ((rgb_color & 0xf80000) >> 15));
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        native_color = (((rgb_color & 0xf80000) >> 9) | ((rgb_color & 0xf800) >> 6) | ((rgb_color & 0xf8) >> 3));
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        native_color = ((rgb_color & 0xf00000) >> 12) | ((rgb_color & 0x00f000) >> 8) | ((rgb_color & 0x0000f0) >> 4) | 0xf000;
        break;

    case GX_COLOR_FORMAT_4444BGRA:
        native_color = ((rgb_color & 0xf00000) >> 20) | ((rgb_color & 0x00f000) >> 4) | ((rgb_color & 0x0000f0) << 8) | 0x000f;
        break;

    case GX_COLOR_FORMAT_565RGB:
        native_color = ((rgb_color & 0xf80000) >> 8) | ((rgb_color & 0x00fc00) >> 5) | ((rgb_color & 0x0000ff) >> 3);
        break;

    case GX_COLOR_FORMAT_565BGR:
        native_color = ((rgb_color & 0x0000f8) << 8) | ((rgb_color & 0x00fc00) >> 5) | ((rgb_color & 0xf80000) >> 19);
        break;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32RGBA:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32BGRA:
        native_color = rgb_color;
        break;
    }
    return native_color;
}

///////////////////////////////////////////////////////////////////////////////
COLORREF resource_view::GetColorRef(GX_COLOR gx_color, int color_format, GX_COLOR *palette, int palsize)
{
    COLORREF color = 0;
    int red = 0;
    int green = 0;
    int blue = 0;

    switch (color_format)
    {
    case GX_COLOR_FORMAT_8BIT_PALETTE:
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (palette && ((int)gx_color < palsize))
        {
            color = palette[gx_color];
            red = (color & 0xff0000) >> 16;
            green = (color & 0xff00) >> 8;
            blue = (color & 0xff);
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        red = (gx_color & 0xe0) >> 5;
        red = (red << 5) | (red << 2) || (red >> 1);

        green = (gx_color & 0x18) >> 3;
        green = (green << 5) | (green << 2) | (green >> 1);

        blue = (gx_color & 0x1f);
        blue = (blue << 5) | (blue << 2) | (blue >> 1);
        break;

    case GX_COLOR_FORMAT_5551BGRX:
        blue = (gx_color & 0x7c) >> 10;
        blue = (blue << 3) | (blue >> 2);

        green = (gx_color & 0x3e) >> 5;
        green = (green << 3) | (red >> 2);

        red = (gx_color & 0x1f);
        red = (red << 3) | (red >> 2);
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        red = (gx_color & 0x7c) >> 10;
        red = (red << 3) | (red >> 2);
        
        green = (gx_color & 0x3e) >> 5;
        green = (green << 3) | (red >> 2);

        blue = (gx_color & 0x1f);
        blue = (blue << 3) | (blue >> 2);
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        red = (gx_color & 0xf00) >> 16;
        red = (red << 8) | red;

        green = (gx_color & 0xf0) >> 8;
        green = (green << 8) | green;

        blue = (gx_color & 0xf);
        blue = (blue << 8) | blue;
        break;

    case GX_COLOR_FORMAT_4444BGRA:
        blue = (gx_color & 0xf000) >> 24;
        blue = (blue << 8) | blue;
        
        green = (gx_color & 0xf00) >> 16;
        green = (green << 8) | green;

        red = (gx_color & 0xf0) >> 8;
        red = (red << 8) | red;
        break;

    case GX_COLOR_FORMAT_565RGB:
        red = (gx_color & 0xf100) >> 11;
        red = (red << 3) | (red >> 2);

        green = (gx_color & 0x7e0) >> 5;
        green = (green << 2) | (green >> 4);

        blue = (gx_color & 0x1f);
        blue = (blue << 3) | (blue >> 2);
        break;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_32ABGR:
    default:
        red = (gx_color >> 16) & 0xff;
        green = (gx_color >> 8) & 0xff;
        blue = (gx_color) & 0xff;
        break;
    }

    color = RGB(red, green, blue);

    return color;
}


///////////////////////////////////////////////////////////////////////////////
int resource_view::BuildColorTable(int DisplayIndex, int ThemeIndex, int color_format, color_table *table)
{
    GX_COLOR color_val;
    GX_COLOR native_color;
    studiox_project *project = GetOpenProject();
    int color_id;
    CString color_name;

    res_info *res = project->mDisplays[DisplayIndex].themes[ThemeIndex].GetFirstResourceInfo();
    res_info *color_res;
    int numcolors = project->CountResources(DisplayIndex, RES_TYPE_COLOR);

    table->colors = NULL;
    table->num_colors = numcolors;

    if (numcolors)
    {
        table->colors = new GX_COLOR[numcolors];
        memset(table->colors, 0, (numcolors * sizeof(GX_COLOR)));

        for (color_id = 0; color_id < numcolors; color_id++)
        {
            project->GetResourceName(DisplayIndex, RES_TYPE_COLOR, color_id, color_name);
            color_res = project->FindResource(res, RES_TYPE_COLOR, color_name);

            if (color_res)
            {
                color_val = color_res->colorval;
                native_color = GetNativeColor(color_val, color_format);
                table->colors[color_id] = native_color;
            }
        }
    }
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
int resource_view::BuildFontTable(int DisplayIndex, int ThemeIndex, font_table *table)
{
    studiox_project *project = GetOpenProject();
    int font_id;
    res_info *font_res;
    CString font_name;

    res_info *res = project->mDisplays[DisplayIndex].themes[ThemeIndex].GetFirstResourceInfo();
    int numfonts = project->CountResources(DisplayIndex, RES_TYPE_FONT);

    table->fonts = NULL;
    table->num_fonts = numfonts;

    if (numfonts)
    {
        table->fonts = new GX_FONT *[numfonts];
        memset(table->fonts, 0, (numfonts * sizeof(GX_FONT *)));

        for (font_id = 0; font_id < numfonts; font_id++)
        {
            project->GetResourceName(DisplayIndex, RES_TYPE_FONT, font_id, font_name);
            font_res = project->FindResource(res, RES_TYPE_FONT, font_name);

            if (font_res)
            {
                table->fonts[font_id] = font_res->font;
            }
        }
    }
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
int resource_view::BuildPixelmapTable(int DisplayIndex, int ThemeIndex, pixelmap_table *table)
{
    studiox_project *project = GetOpenProject();
    int pixelmap_id;
    res_info *pixelmap_res;
    CString pixelmap_name;

    res_info *res = project->mDisplays[DisplayIndex].themes[ThemeIndex].GetFirstResourceInfo();
    int num_pixelmaps = project->CountResources(DisplayIndex, RES_TYPE_PIXELMAP);

    table->maps = NULL;
    table->num_maps = num_pixelmaps;

    if (num_pixelmaps)
    {
        table->maps = new GX_PIXELMAP *[num_pixelmaps];
        memset(table->maps, 0, (num_pixelmaps * sizeof(GX_PIXELMAP *)));
        table->maps[0] = GX_NULL;

        for (pixelmap_id = 1; pixelmap_id < num_pixelmaps; pixelmap_id++)
        {
            project->GetResourceName(DisplayIndex, RES_TYPE_PIXELMAP, pixelmap_id, pixelmap_name);
            pixelmap_res = project->FindResource(res, RES_TYPE_PIXELMAP, pixelmap_name);

            if (pixelmap_res)
            {
                table->maps[pixelmap_id] = pixelmap_res->GetPixelmap();
            }
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::CheckInstallPalette(int DisplayIndex, GX_DISPLAY *display)
{
    VALID_PROJECT

    if (!display)
    {
        return;
    }

    if (project->IsPaletteMode(DisplayIndex) &&
        display->gx_display_driver_palette_set)
    {
        // if this is a palette mode driver, install the palette:
        int theme_index = project->mDisplays[DisplayIndex].active_theme;
        GX_COLOR *pPalette = project->mDisplays[DisplayIndex].themes[theme_index].palette;

        if (pPalette)
        {
            display->gx_display_driver_palette_set(display, pPalette, 
                project->mDisplays[DisplayIndex].themes[theme_index].palette_total_size);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::InstallColorTable(int DisplayIndex, GX_DISPLAY *display)
{
    VALID_PROJECT

    color_table table;

    if (DisplayIndex < 0)
    {
        return;
    }

    int ThemeIndex = GetOpenProject()->mDisplays[DisplayIndex].active_theme;

    if (!display)
    {
         display = get_target_view_display();
    }

    CheckInstallPalette(DisplayIndex, display);

    BuildColorTable(DisplayIndex, ThemeIndex, project->mDisplays[DisplayIndex].colorformat, &table);

    if (display)
    {
        GX_COLOR *old_color_table = display->gx_display_color_table;
        gx_display_color_table_set(display, table.colors, table.num_colors);

        if (old_color_table)
        {
            delete [] old_color_table;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::InstallFontTable(int DisplayIndex, GX_DISPLAY *display)
{
    VALID_PROJECT
    font_table table;

    if (DisplayIndex < 0)
    {
        return;
    }

    int ThemeIndex = GetOpenProject()->mDisplays[DisplayIndex].active_theme;

    BuildFontTable(DisplayIndex, ThemeIndex, &table);

    if (!display)
    {
        display = get_target_view_display();
    }
    if (display)
    {
        GX_FONT **old_font_table = display->gx_display_font_table;
        gx_display_font_table_set(display, table.fonts, table.num_fonts);
        if (old_font_table)
        {
            delete [] old_font_table;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::InstallPixelmapTable(int DisplayIndex, GX_DISPLAY *display)
{
    VALID_PROJECT
    pixelmap_table table;

    if (DisplayIndex < 0)
    {
        return;
    }

    if (!display)
    {
        display = get_target_view_display();
    }

    int ThemeIndex = GetOpenProject()->mDisplays[DisplayIndex].active_theme;

    if (display)
    {
        CheckInstallPalette(DisplayIndex, display);

        GX_PIXELMAP **old_map_table = display->gx_display_pixelmap_table;
        BuildPixelmapTable(DisplayIndex, ThemeIndex, &table);
        gx_display_pixelmap_table_set(display, table.maps, table.num_maps);

        if (old_map_table)
        {
            delete [] old_map_table;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void resource_view::BuildResourceTables(int DisplayIndex, GX_DISPLAY *display, BOOL update_resource_view)
{
    VALID_PROJECT

    // Install these resources with GUIX:
    InstallColorTable(DisplayIndex, display);
    InstallFontTable(DisplayIndex, display);
    InstallPixelmapTable(DisplayIndex, display);
            
    string_table *table = project->mDisplays[DisplayIndex].stable;
    if (table)
    {
        table->UpdateGuixLanguageTable(display, update_resource_view);
    }
}

/////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI InstallResourcesThreadEntry(LPVOID thread_input)
{
    resource_view *res_view = (resource_view*)GetResourceView();

    if (res_view)
    {
        int display_index = (int)thread_input;
        res_view->BuildResourceTables(display_index, NULL);
    }

    EndBusyMsg();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void resource_view::InstallResources(int DisplayIndex)
{
    VALID_PROJECT

    int ThemeIndex = GetOpenProject()->mDisplays[DisplayIndex].active_theme;

    // Delete resource tree
    mpTree->DeleteAllItems();

    // populate the tree with the selected display's resources
    res_info *info = project->mDisplays[DisplayIndex].themes[ThemeIndex].GetFirstResourceInfo();
    InsertTreeResources(NULL, info);

    resource_item *root = mpTree->GetRoot();
    if (root)
    {
        // Set current item
        SetCurrentItem(root->First());
    }

    StartWorkThread(InstallResourcesThreadEntry, (LPVOID)DisplayIndex, "Installing Resources.", TRUE);
}

/////////////////////////////////////////////////////////////////////////////
void resource_view::CreateUniqueResourceName(res_info *info)
{
    CString base_name = info->name;
    CString name;
    CString num;
    int val = 1;

    while (1)
    {
        num.Format(_T("%d"), val);

        name = base_name + num;
        if (!NameExists(mDisplayIndex, info->type, name))
        {
            info->name = name;
            return;
        }
        val++;
    }
}

/////////////////////////////////////////////////////////////////////////////
void resource_view::OnDisplaySelect(int DisplayIndex, BOOL bReload)
{
    VALID_PROJECT
    if (mDisplayIndex != DisplayIndex || bReload)
    {
        mNewResourcePasted = FALSE;
        mpTree->DeleteAllItems();
        mDisplayIndex = DisplayIndex;
        
        if (DisplayIndex >= 0)
        {
            InstallResources(DisplayIndex);
        }

        mpTree->PositionItems();
    }
}

/////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI InitializeThemePixelmapsThreadEntry(LPVOID thread_input)
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        int display_index = (int)thread_input;
        project->InitializeThemePixelmaps(display_index, project->mDisplays[display_index].active_theme);
    }

    EndBusyMsg();
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void resource_view::FinalizePasteResources()
{
    studiox_project *project = GetOpenProject();

    if (mNewResourcePasted && project)
    {
        string_table *table = project->mDisplays[mDisplayIndex].stable;
        if (table)
        {
            table->Sort();
        }

        if (project->IsPaletteMode(mDisplayIndex))
        {
            StartWorkThread(InitializeThemePixelmapsThreadEntry, (LPVOID)mDisplayIndex, "Creating Optimal Palette.", TRUE);
        }

        OnDisplaySelect(mDisplayIndex, TRUE);
        mNewResourcePasted = FALSE;
    }
}

void resource_view::CleanupDisplayResources(GX_DISPLAY *display)
{
    GX_PIXELMAP **old_map_table;
    GX_COLOR    *old_color_table;
    GX_FONT     **old_font_table;

    old_map_table = display->gx_display_pixelmap_table;
    gx_display_pixelmap_table_set(display, NULL, 0);

    old_font_table = display->gx_display_font_table;
    gx_display_font_table_set(display, NULL, 0);

    old_color_table = display->gx_display_color_table;
    gx_display_color_table_set(display, NULL, 0);

    if (old_map_table)
    {
        delete [] old_map_table;
    }

    if (old_font_table)
    {
        delete [] old_font_table;
    }

    if (old_color_table)
    {
        delete [] old_color_table;
    }

    // if we installed a string table with GUIX, delete it now:
    GX_STRING **pOldTable = NULL;
    GX_UBYTE language_count = 0;
    UINT string_count = 0;
    gx_display_language_table_get_ext(display, &pOldTable, &language_count, &string_count);

    string_table::DeleteGuixLanguageDirectionTable(display);

    if (pOldTable)
    {
        string_table::DeleteGuixLanguageTable(pOldTable, language_count, string_count);
        gx_display_language_table_set_ext(display, NULL, 0, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnCloseProject()
{
    mpTree->DeleteAllItems();

    CleanupDisplayResources(get_target_view_display());

    Invalidate();
    mDisplayIndex = -1;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnTreeSizeChange()
{
    if (mpTree)
    {
        CRect childsize;
        GetClientRect(&childsize);
        childsize.bottom = childsize.top + mpTree->GetHeight();
        childsize.right = childsize.left + mpTree->GetWidth();

        SIZE total;
        total.cx = childsize.Width();
        total.cy = childsize.Height() + 20;

        SetScrollSizes(MM_TEXT, total);
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnAddCustomColor()
{
    VALID_PROJECT
    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *parent;

    if (mpCurrentItem->GetResType() == RES_TYPE_ADD_COLOR)
    {
        parent = mpCurrentItem->Parent();
    }
    else
    {
        parent = mpCurrentItem;
    }


    // double check to insure we have the correct parent:
    if (mDisplayIndex == -1)
    {
        return;
    }
    if (parent->mpRes->name != "Custom")
    {
        return;
    }

    color_edit_dialog dlg(mDisplayIndex, this);

    if (dlg.DoModal() == IDOK)
    {
        GX_COLOR color = dlg.GetColor();
        CString name = dlg.GetName();

        // Add a res_info element to the project:
        AddColor(color, name, parent);
    }
}

///////////////////////////////////////////////////////////////////////////////
// KGM Note:
// This function does NOT update the GUIX tables, because we could be adding
// several resources so just update the tables once. The copy/paste engine will
// force a table update after all resources are added.
///////////////////////////////////////////////////////////////////////////////

int resource_view::PasteResource(res_info *info, int conflict_option)
{
    studiox_project *project = GetOpenProject();
    if (!project || !info)
    {
        return conflict_option;
    }
    resource_item *parent;
    resource_item *existing;
    res_info *new_info;
    display_info *display;
    int rgb_color = 0;

    switch(info->type)
    {
    case RES_TYPE_COLOR:
        parent = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_FOLDER, CUSTOM_COLOR_FOLDER);
        existing = mpTree->FindItem(parent, RES_TYPE_COLOR, info->name);

        display = &project->mDisplays[mDisplayIndex];

        if (display)
        {
            int active_theme = display->active_theme;

            //color convert
            info->colorval = ProjectConfigDlg::GetColorVal(info->colorval,
                display->themes[active_theme].palette,
                display->themes[active_theme].palette_predefined,
                display->colorformat);

            rgb_color = ProjectConfigDlg::GetRGBColor(info->colorval,
                display->themes[active_theme].palette,
                display->themes[active_theme].palette_total_size,
                display->colorformat);
        }

        if (existing)
        {
            if (existing->GetRGBColor() != rgb_color)
            {
                //conflict
                if (conflict_option == 0)
                {
                    CString msg;
                    msg.Format(_T("Source and Destination projects have duplicate Color IDs \"%s\"."), info->name);

                    options_dialog dlg(_T("Warning"), msg, paste_conflict_options, this);
                    if (dlg.DoModal() == IDOK)
                    {
                        conflict_option = dlg.GetSelectedOption();
                    }
                }

                switch (conflict_option)
                {
                case PASTE_CONFLICT_OPTION_USE_SOURCE:
                    //Use string from source
                    existing->SetColorValue(info->colorval);
                    mNewResourcePasted = TRUE;
                    break;

                case PASTE_CONFLICT_OPTION_RENAME:
                    //Rename pasted string id
                    CreateUniqueResourceName(info);

                    AddColor(info->colorval, info->name, parent);
                    mNewResourcePasted = TRUE;
                    break;

                case PASTE_CONFLICT_OPTION_USE_DESTINATION:
                case PASTE_CONFLICT_OPTION_ABORT:
                default:
                    break;
                }
            }
        }
        else
        {
            mNewResourcePasted = TRUE;
            AddColor(info->colorval, info->name, parent);
        }
        break;

    case RES_TYPE_FONT:
        parent = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_FOLDER, CUSTOM_FONT_FOLDER);
        existing = mpTree->FindItem(parent, RES_TYPE_FONT, info->name);

        if (!existing)
        {
            mNewResourcePasted = TRUE;
            new_info = new res_info(parent->mpRes, *info, FALSE);
            AddFont(parent, new_info);
        }
        break;

    case RES_TYPE_PIXELMAP:
        parent = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_FOLDER, CUSTOM_PIXELMAP_FOLDER);

        if (!parent)
        {
            // create a custom pixelmap folder   
            parent = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_GROUP, PIXELMAP_GROUP); 
            
            if (!parent)
            {
                // this should NOT happen, but just in case
                return conflict_option;
            }
            res_info *info = new res_info(RES_TYPE_FOLDER);
            info->name = _T("Custom");
            info->folder_id = CUSTOM_PIXELMAP_FOLDER;
            mpTree->CreateUniqueFolderName(info->name, info->folder_id);

            // insert new folder into project:
            parent->mpRes->Attach(info);
            InsertTreeResources(parent, info);

            SyncResourceAdd(info);

            // we created custom pixelmap folder, so now find it again:
            parent = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_FOLDER, CUSTOM_PIXELMAP_FOLDER);

            if (!parent)
            {
                // This should NOT happen
                return conflict_option;
            }
        }

        existing = mpTree->FindItem(parent, RES_TYPE_PIXELMAP, info->name);

        if (!existing)
        {
            mNewResourcePasted = TRUE;
            new_info = new res_info(parent->mpRes, *info, FALSE);
            int display_format = project->mDisplays[mDisplayIndex].colorformat;
            if (!edit_pixelmap_dlg::IsOutputFormatSupported(display_format, new_info->output_color_format) || new_info->raw)
            {
                //if the output pixelmap format is not supported, reset output color format as default
                new_info->output_color_format = 0;
            }
            if (display_format == GX_COLOR_FORMAT_8BIT_PALETTE)
            {
                new_info->palette_type = PALETTE_TYPE_SHARED;
            }

            AddPixelmap(parent, new_info, project->IsPaletteMode(mDisplayIndex));
            
            for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
            {

                if (project->GetDisplayColorFormat(new_info) == GX_COLOR_FORMAT_8BIT_PALETTE ||
                    project->TestForPixelmapsUsingGlobalPalette(project->mDisplays[mDisplayIndex].themes[theme].GetFirstResourceInfo()))
                {
                    project->InitializeThemePixelmaps(mDisplayIndex, theme);
                }
            }
        }
        break;    
    }
    return conflict_option;
}

///////////////////////////////////////////////////////////////////////////////
// KGM Note:
// This function does NOT update the GUIX tables, because we could be adding
// several resources so just update the tables once. The copy/paste engine will
// force a table update after all resources are added.
///////////////////////////////////////////////////////////////////////////////

int resource_view::PasteResource(string_table_record &record, int conflict_option)
{
    int language;
    BOOL conflict_string_content = FALSE;

    studiox_project *project = GetOpenProject();
    if (!project || mDisplayIndex < 0)
    {
        return PASTE_CONFLICT_OPTION_ABORT;
    }

    string_table *table = project->mDisplays[mDisplayIndex].stable;

    if (table)
    {
        int string_index = table->FindStringIndex(record.id_name);
  
        if (string_index)
        {
            for (language = 0; language < table->CountLanguages(); language++)
            {
                if (table->GetString(string_index, language) != record.strings[language])
                {
                    conflict_string_content = TRUE;
                    break;
                }
            }

            if (conflict_string_content)
            {
                if (conflict_option == 0)
                {
                    CString msg;
                    msg.Format(_T("Source and Destination projects have duplicate String IDs \"%s\"."), record.id_name);

                    options_dialog dlg(_T("Warning"), msg, paste_conflict_options, this);
                    if (dlg.DoModal() == IDOK)
                    {
                        conflict_option = dlg.GetSelectedOption();
                    }
                }

                switch (conflict_option)
                {
                case PASTE_CONFLICT_OPTION_USE_SOURCE:
                    //Use string from source
                    for (language = 0; language < table->CountLanguages(); language++)
                    {
                        table->SetString(string_index, language, record.strings[language], FALSE);
                    }
                    mNewResourcePasted = TRUE;
                    break;

                case PASTE_CONFLICT_OPTION_USE_DESTINATION:
                    //use string from destination
                    break;

                case PASTE_CONFLICT_OPTION_RENAME:
                    //Rename pasted string id
                    for (language = 0; language < table->CountLanguages(); language++)
                    {
                        int index = table->CheckAddString(record.strings[language], record.id_name);
                        record.id_name = table->GetResourceIdName(index);
                    }
                    mNewResourcePasted = TRUE;
                    break;

                case PASTE_CONFLICT_OPTION_ABORT:
                default:
                    //abort pasting
                    break;
                }
            }
        }
        else
        {
            string_index = table->AddString(record.id_name, record.strings[0]);
            for (language = 0; language < table->CountLanguages(); language++)
            {
                table->SetString(string_index, language, record.strings[language], FALSE);
            }
            mNewResourcePasted = TRUE;
        }
    }
    return conflict_option;
}


///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditSystemColor()
{
    VALID_PROJECT

    if (!mpCurrentItem)
    {
        return;
    }
    resource_item *item = mpCurrentItem;
    GX_DISPLAY *display = get_target_view_display();

    color_edit_dialog dlg(item, mDisplayIndex, this);

    if (project && dlg.DoModal() == IDOK)
    {
        GX_COLOR color = dlg.GetColor();

        item->SetColorValue(color);
        project->SetModified();
        Invalidate();
        InstallColorTable(mDisplayIndex);
    }
}


///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditCustomColor()
{
    VALID_PROJECT
    resource_item *item = mpCurrentItem;
    GX_DISPLAY *display = get_target_view_display();

    GX_COLOR color;

    if (!item || !item->mpRes)
    {
        return;
    }

    CString old_color_name = item->mpRes->name;

    color_edit_dialog dlg(item, mDisplayIndex, this);

    if (dlg.DoModal() == IDOK)
    {
        color = dlg.GetColor();

        item->SetColorValue(color);
        item->mpRes->name = dlg.GetName();

        if (old_color_name != item->mpRes->name)
        {
            SyncResourceName(old_color_name, item->mpRes);

            project->UpdateDictionaryResourceName(mDisplayIndex, old_color_name, item->mpRes);
            GetPropsWin()->ResourcesChanged();

            if (project->mDisplays[mDisplayIndex].stable)
            {
                project->mDisplays[mDisplayIndex].stable->UpdateStringTableRichTextResourceName(RES_TYPE_COLOR, old_color_name, item->mpRes->name);
            }
        }

        InstallColorTable(mDisplayIndex);
        project->SetModified();
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnAddFont()
{
    VALID_PROJECT
    resource_item *parent;

    if (!mpCurrentItem)
    {
        return;
    }

    if (mpCurrentItem->GetResType() == RES_TYPE_ADD_FONT)
    {
        parent = mpCurrentItem->Parent();
    }
    else
    {
        parent = mpCurrentItem;
    }

    if (!parent || !parent->mpRes)
    {
        return;
    }

    res_info * info = InitFontInfo();

    font_path_dialog dlg(mDisplayIndex, project->mDisplays[mDisplayIndex].colorformat, info, this);

    if (dlg.DoModal() == IDOK)
    {
        AddFont(parent,info);
    }
    else
	{
        delete info;
    }
}
///////////////////////////////////////////////////////////////////////////////
res_info * resource_view::InitFontInfo()
{
    studiox_project *project = GetOpenProject();
    if (!project)
    {
        return NULL;
    }
    res_info *info = new res_info(RES_TYPE_FONT);
    info->pathinfo.pathname = _T("");
    info->pathinfo.pathtype = PATH_TYPE_PROJECT_RELATIVE;
    info->name = _T("NewFont");
    info->font_height = 16;
    info->font_pages = font_path_dialog::CreateDefaultFontPages();
    info->font_bits = 1;
    info->compress = FALSE;
    info->font_kerning = FALSE;

    return info;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::AddFont(resource_item *parent, res_info *info)
{
    VALID_PROJECT
    BOOL kerning = info->font_kerning;
    GX_FONT *font = MakeFont(info, mDisplayIndex, TRUE);

    info->font = font;
    if (kerning)
    {
        if (font)
        {
            if ((font->gx_font_format & GX_FONT_FORMAT_KERNING) == 0)
            {
                info->font_kerning = GX_FALSE;

                Notify("The selected font file doesn't have kerning info. The kerning checkbox will be set to false.");
            }
        }
    }

    parent->mpRes->Attach(info);
    project->AddToResourceDictionary(mDisplayIndex, info);

    /* Get output font size. */
    info->storage_size = GetFontStorage(info, project, mDisplayIndex);

    // insert this item into the display tree:
    InsertTreeResources(parent, info);
    if (!parent->IsOpen())
    {
        parent->Open();
    }
    mpTree->PositionItems();

    SyncResourceAdd(info);

    InstallFontTable(mDisplayIndex);
    project->SetModified();
    GetPropsWin()->ResourcesChanged();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditFont()
{
    VALID_PROJECT

    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *item = mpCurrentItem;
    res_info *info = item->mpRes;

    if (!info || info->type != RES_TYPE_FONT)
    {
        return;
    }

    CString old_font_name = info->name;
    CString old_font_path = info->pathinfo.pathname;
    font_path_dialog dlg(mDisplayIndex, project->mDisplays[mDisplayIndex].colorformat, info, this);

    if (dlg.DoModal() == IDOK)
    {
        if (old_font_name != info->name)
        {
            // Reset font name in all active themes
            SyncResourceName(old_font_name, info);

            project->UpdateDictionaryResourceName(mDisplayIndex, old_font_name, info);

            if (project->mDisplays[mDisplayIndex].stable)
            {
                project->mDisplays[mDisplayIndex].stable->UpdateStringTableRichTextResourceName(RES_TYPE_FONT, old_font_name, info->name);
            }
        }

        EditFont(item, old_font_path);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::EditFont(resource_item *item, CString &old_font_path)
{
    VALID_PROJECT
    res_info *info = item->mpRes;
    BOOL     kerning = info->font_kerning;
    if (info->font && (!info->is_default || !old_font_path.IsEmpty()))
    {
        DestroyFont(info->font);
        info->font = NULL;
    }

    GX_FONT *font = NULL;

    if (info->is_default && info->pathinfo.pathname.IsEmpty())
    {
        studiox_project::ConfigureDefaultFont(info, mDisplayIndex);
    }
    else
    {
        font = MakeFont(info, mDisplayIndex, TRUE);
        if (font)
        {
            if (kerning)
            {
                if ((font->gx_font_format & GX_FONT_FORMAT_KERNING) == 0)
                {
                    info->font_kerning = GX_FALSE;
                    Notify("The selected font file doesn't have kerning info. The kerning checkbox will be set to false.");
                }
            }

            info->font = font;

            /* Get output font size. */
            info->storage_size = GetFontStorage(info, project, mDisplayIndex);
        }
    }

    project->SetModified();
    InstallFontTable(mDisplayIndex);
    GetPropsWin()->ResourcesChanged();
    item->RebuildPreview();
    mpTree->PositionItems();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::Scroll(int delta, BOOL redraw)
{
    SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + delta, redraw);
    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::ScrollIntoView(resource_item *item)
{
    if (item)
    {
        int scrollpos = GetScrollPos(SB_VERT);

        CRect client;
        GetClientRect(client);

        CRect rect = item->GetPos();
        rect.top -= scrollpos;
        rect.bottom -= scrollpos;

        if (rect.top < client.top)
        {
            Scroll(rect.top - client.top);
        }
        else if (rect.bottom > client.bottom)
        {
            Scroll(rect.bottom - client.bottom);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::SetCurrentItem(resource_item* item)
{
    CRect rect;
    int scrollpos = GetScrollPos(SB_VERT);


    if (mpCurrentItem)
    {
        rect = mpCurrentItem->GetPos();
        rect.top -= scrollpos;
        rect.bottom -= scrollpos;

        InvalidateRect(rect);
    }

    mpCurrentItem = item;

    if (mpCurrentItem)
    {
        rect = mpCurrentItem->GetPos();
        rect.top -= scrollpos;
        rect.bottom -= scrollpos;

        InvalidateRect(rect);

        ScrollIntoView(mpCurrentItem);

        // Raise UI Automation event.
        resource_item_provider* itemProvider = mpCurrentItem->GetResItemProvider();
        if (itemProvider)
        {
            itemProvider->RaiseSelectElementEvent();
            itemProvider->RaiseChangeFocusEvent();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void resource_view::UpdateStringTableFonts(FontCharMap *map, res_info *start)
{
    VALID_PROJECT

    if (!start)
    {
        int active_theme = project->mDisplays[mDisplayIndex].active_theme;
        start = project->mDisplays[mDisplayIndex].themes[active_theme].GetFirstResourceInfo();
    }

    while(start)
    {
        if (start->type == RES_TYPE_FONT)
        {
            if (start->font_charset_include_string_table)
            {
                if (start->font)
                {
                    if (!start->is_default || !start->pathinfo.pathname.IsEmpty())
                    {
                        // try to just add needed glyphs. If that doesn't work,
                        // then we have to re-make the font
                        if (!InsertStringTableGlyphs(map, start, mDisplayIndex))
                        {
                            DestroyFont(start->font);
                            start->font = MakeFont(start, mDisplayIndex, FALSE);
                            start->storage_size = GetFontStorage(start, project, mDisplayIndex);
                        }
                        else
                        {
                            if (start->font_charset_include_string_table)
                            {
                                start->storage_size = GetFontStorage(start, project, mDisplayIndex);
                            }
                        }
                    }
                }
                else
                {
                    start->font = MakeFont(start, mDisplayIndex, FALSE);
                    start->storage_size = GetFontStorage(start, project, mDisplayIndex);
                }
            }
        }

        if (start->child)
        {
            UpdateStringTableFonts(map, start->child);
        }
        start = start->next;
    }
    InstallFontTable(mDisplayIndex);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnDeleteCustomColor()
{
    VALID_PROJECT
    resource_item *pItem = mpCurrentItem;

    if (pItem && pItem->mpRes)
    {
        if (pItem->mpRes->type == RES_TYPE_COLOR)
        {
            if (pItem->mpRes->is_default == FALSE)
            {
                int res_id = project->GetResourceId(mDisplayIndex, pItem->mpRes);

                //select next item
                SelectNextItem();

                SyncResourceDelete(pItem->mpRes);

                project->DeleteResource(pItem->mpRes);
                pItem->mpRes = NULL;
                mpTree->DeleteItem(pItem);
                InstallColorTable(mDisplayIndex);

                // clean up all widget references to this resource
                folder_info *info = project->mDisplays[mDisplayIndex].GetFirstChildFolder();
                widget_factory::DecrementWidgetResourceIds(RES_TYPE_COLOR, info, res_id);
                Invalidate();
                GetPropsWin()->ResourcesChanged();
                project->SetModified();


                if (project->mDisplays[mDisplayIndex].stable)
                {
                    // update string table.
                    project->mDisplays[mDisplayIndex].stable->UpdateGuixLanguageTable();
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnDeleteFont()
{
    VALID_PROJECT

    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *item = mpCurrentItem;

    res_info *info = item->mpRes;

    if (!info || info->type != RES_TYPE_FONT ||
        info->is_default)
    {
        return;
    }
    int res_id = project->GetResourceId(mDisplayIndex, info);

    //select next item
    SelectNextItem();

    SyncResourceDelete(info);

    project->DeleteResource(info);
    item->mpRes = NULL;
    mpTree->DeleteItem(item);
    InstallFontTable(mDisplayIndex);

    // clean up all widget references to this resource
    folder_info *finfo = project->mDisplays[mDisplayIndex].GetFirstChildFolder();
    widget_factory::DecrementWidgetResourceIds(RES_TYPE_FONT, finfo, res_id);
    GetPropsWin()->ResourcesChanged();

    if (project->mDisplays[mDisplayIndex].stable)
    {
        // update string table.
        project->mDisplays[mDisplayIndex].stable->UpdateGuixLanguageTable();
    }

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnAddPixelmaps()
{
    TCHAR *path;
    TCHAR *name;
    TCHAR *pGet;
    CString pathname;
    VALID_PROJECT
    BOOL warn_on_error = TRUE;

    int NameLen;
    resource_item *parent;

    if (!mpCurrentItem)
    {
        return;
    }

    if (mpCurrentItem->GetResType() == RES_TYPE_ADD_PIXELMAP)
    {
        parent = mpCurrentItem->Parent();
    }
    else
    {
        parent = mpCurrentItem;
    }

    if (!parent || !parent->mpRes)
    {
        return;
    }

    project->mHeader.warn_missing_image = TRUE;

    int filecount = BrowseForMultipleFiles(_T("Select Image File(s)"),
                                           _T("Image Files\0*.png;*.jpg;*.bmp;*.gif\0"), NULL,
                                           &path, &name, this);

    CStringArray patharray;

    if (filecount > 0)
    {
        pGet = name;

        while(*pGet)
        {
            if (filecount > 1)
            {
                pathname = CString(path) + CString("\\") + CString(pGet);
            }
            else
            {
                pathname = CString(path);
            }

            patharray.Add(pathname);

            NameLen = (int)_tcslen(pGet) + 1;
            pGet += NameLen;
        }
        delete [] path;
    }

    AddPixelmaps(parent, &patharray);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditPixelmaps()
{
    if (!mpCurrentItem)
    {
        return;
    }

    edit_pixelmap_dlg dlg(mpCurrentItem->mpRes, this);

    if (dlg.DoModal() == IDOK)
    {
        EditPixelmaps(mpCurrentItem);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnAddPixelmapFolder()
{
    VALID_PROJECT
    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *parent = mpCurrentItem;

    if (!parent->mpRes)
    {
        return;
    }

    res_info *info = new res_info(RES_TYPE_FOLDER);
    info->name = _T("New Folder");
    info->folder_id = CUSTOM_PIXELMAP_FOLDER;

    mpTree->CreateUniqueFolderName(info->name, info->folder_id);

    res_info* add = new res_info(RES_TYPE_ADD_PIXELMAP);
    info->Attach(add);

    // insert new folder into project:
    parent->mpRes->Attach(info);

    // insert this item into the display tree:
    InsertTreeResources(parent, info);

    SyncResourceAdd(info);

    if (!parent->IsOpen())
    {
        parent->Open();
    }
    else
    {
        mpTree->PositionItems(); 
    }

    resource_item *child = parent->First();
    while(child->Next())
    {
        child = child->Next();
    }
    SetCurrentItem(child);
    OnRenamePixelmapFolder();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnRemovePixelmapFolder()
{
    VALID_PROJECT
    if (mpCurrentItem && mpCurrentItem->mpRes)
    {
        if (mpCurrentItem->mpRes->type == RES_TYPE_FOLDER)
        {
            // remove child resources under the folder
            res_info *child;
            res_info *next;
            resource_item *childItem;
            resource_item *delItem = mpCurrentItem;
            int res_id;
            child = delItem->mpRes->child;

            delItem->Close();

            //select next item
            SelectNextItem();

            if (mpCurrentItem && mpCurrentItem->GetResType() != RES_TYPE_FOLDER)
            {
                //select previous item
                SetCurrentItem(delItem);
                SelectPreviousItem();
            }

            CArray<GX_RESOURCE_ID> res_id_array;

            while (child)
            {
                next = child->next;

                if (child->type == RES_TYPE_PIXELMAP)
                {
                    res_id = project->GetResourceId(GetProjectView()->GetActiveDisplay(), child);

                    if (res_id)
                    {
                        res_id_array.Add(res_id);
                    }
                }

                child = next;
            }

            child = delItem->mpRes->child;

            while(child)
            {
                next = child->next;

                childItem = mpTree->FindItem(delItem, child->type, child->name);

                project->DeleteResource(child);
                mpTree->DeleteItem(childItem);

                child = next;
            }

            if (project->IsPaletteMode(mDisplayIndex))
            {
                StartWorkThread(InitializeThemePixelmapsThreadEntry, (LPVOID)mDisplayIndex, "Creating Optimal Palette.", TRUE);
            }
            InstallPixelmapTable(mDisplayIndex);

            SyncResourceDelete(delItem->mpRes);

            // remove this resource from project:
            project->DeleteResource(delItem->mpRes);
            delItem->mpRes = NULL;
         
            // remove this item from display tree:
            mpTree->DeleteItem(delItem);

            // clean up all widget references to this resource
            folder_info* finfo = project->mDisplays[mDisplayIndex].GetFirstChildFolder();
            widget_factory::DecrementWidgetResourceIds(RES_TYPE_PIXELMAP, finfo, res_id_array);

            GetPropsWin()->ResourcesChanged();
            Invalidate();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnRenamePixelmapFolder()
{
    VALID_PROJECT
    resource_item *item = mpCurrentItem;

    if (!mpCurrentItem)
    {
        return;
    }

    res_info *info = item->mpRes;

    if (!info || info->type != RES_TYPE_FOLDER)
    {
        return;
    }

    CString old_folder_name = info->name;

    folder_name_dlg dlg(info, this);

    if (dlg.DoModal() == IDOK)
    {
        dlg.GetEditFolderName(info->name);

        SyncResourceName(old_folder_name, info);

        Invalidate();
        project->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEnablePixelmapFolder()
{
    VALID_PROJECT
    resource_item* item = mpCurrentItem;

    if (!mpCurrentItem)
    {
        return;
    }

    res_info* info = item->mpRes;

    info->enabled = TRUE;

    //Sychronous child properties with parent group. If pixelmap folder is enabled, its child should be all enabled.
    EnableDisableChild(info, RES_TYPE_PIXELMAP, info->enabled);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnDisablePixelmapFolder()
{
    VALID_PROJECT
    resource_item *item = mpCurrentItem;

    if (!mpCurrentItem)
    {
        return;
    }

    res_info *info = item->mpRes;

    info->enabled = FALSE;

    //Sychronous child properties with parent group. If pixelmap folder is enabled, its child should be all enabled.
    EnableDisableChild(info, RES_TYPE_PIXELMAP, info->enabled);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::EnableDisableChild(res_info *parent, int target_child_type, GX_BOOL enable)
{
    res_info *child = parent->child;

    while (child)
    {
        if (child->type == target_child_type)
        {
            child->enabled = enable;
        }

        if (child->child)
        {
            EnableDisableChild(child, target_child_type, enable);
        }

        child = child->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
int resource_view::GetFolderEnableDisableState(res_info *parent)
{
    int current_state = parent->enabled;
    BOOL first_child = TRUE;

    if (parent->type == RES_TYPE_GROUP ||
        parent->type == RES_TYPE_FOLDER)
    {
        int next_state;
        res_info* child = parent->child;

        while (child)
        {
            if (child->type == RES_TYPE_GROUP ||
                child->type == RES_TYPE_FOLDER ||
                child->type == RES_TYPE_PIXELMAP)
            {
                if (child->type == RES_TYPE_PIXELMAP)
                {
                    next_state = child->enabled;
                }
                else
                {
                    next_state = GetFolderEnableDisableState(child);
                }

                if (first_child)
                {
                    current_state = next_state;
                    first_child = FALSE;
                }

                if (current_state != next_state)
                {
                    return RES_STATE_UNDETERMINED;
                }
            }

            child = child->next;
        }
    }

    return current_state;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditPixelmap()
{
    VALID_PROJECT
    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *item = mpCurrentItem;
    res_info *info = item->mpRes;

    if (!info || info->type != RES_TYPE_PIXELMAP)
    {
        return;
    }

    CString old_pixelmap_name = info->name;
    int old_frame_count = info->GetPixelmapFrameCount();

    edit_pixelmap_dlg dlg(info, this);

    if (dlg.DoModal() == IDOK)
    {
        if (old_pixelmap_name != info->name)
        {
            // Reset pixelmap name in all active themes
            SyncResourceName(old_pixelmap_name, info);

            project->UpdateDictionaryResourceName(mDisplayIndex, old_pixelmap_name, info);
        }

        EditPixelmaps(item);

        if (old_frame_count != info->GetPixelmapFrameCount())
        {
            // Need to update frame information of the sprite that reference this pixemlap.
            folder_info* finfo = project->mDisplays[mDisplayIndex].GetFirstChildFolder();

            if (finfo)
            {
                widget_factory::UpdateSpriteFrameInfos(finfo, project->GetResourceId(mDisplayIndex, RES_TYPE_PIXELMAP, info->name));
            }

            SyncResourcePath(info);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnDeletePixelmap()
{
    VALID_PROJECT

    if (!mpCurrentItem)
    {
        return;
    }
    resource_item *item = mpCurrentItem;
    res_info *info = item->mpRes;

    if (!info || info->type != RES_TYPE_PIXELMAP ||
        info->is_default)
    {
        return;
    }
    int res_id = project->GetResourceId(mDisplayIndex, info);

    //select next item
    SelectNextItem();

    SyncResourceDelete(info);

    project->DeleteResource(info);
    item->mpRes = NULL;
    mpTree->DeleteItem(item);

    if (project->IsPaletteMode(mDisplayIndex))
    {
        StartWorkThread(InitializeThemePixelmapsThreadEntry, (LPVOID)mDisplayIndex, "Creating Optimal Palette.", TRUE);
    }
    InstallPixelmapTable(mDisplayIndex);

    // clean up all widget references to this resource
    folder_info *finfo = project->mDisplays[mDisplayIndex].GetFirstChildFolder();
    widget_factory::DecrementWidgetResourceIds(RES_TYPE_PIXELMAP, finfo, res_id);
    GetPropsWin()->ResourcesChanged();
    Invalidate();
}

////////////////////////////////////////////////////////////////////////////////
void resource_view::OnEnablePixelmap()
{
    VALID_PROJECT

    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *item = mpCurrentItem;
    res_info *info;
    res_info *parent;

    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            info = item->mpRes;
        }
        else
        {
            info = project->FindResource(mDisplayIndex, theme, item->mpRes->type, item->mpRes->name);
        }

        if (info)
        {
            info->enabled = TRUE;

            parent = info->parent;
            if (parent->type == RES_TYPE_FOLDER && parent->folder_id == CUSTOM_PIXELMAP_FOLDER)
            {
                //enable the folder
                parent->enabled = TRUE;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void resource_view::OnDisablePixelmap()
{
    VALID_PROJECT
    if (!mpCurrentItem)
    {
        return;
    }

    resource_item *item = mpCurrentItem;
    res_info *info;

    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            info = item->mpRes;
        }
        else
        {
            info = project->FindResource(mDisplayIndex, theme, item->mpRes->type, item->mpRes->name);
        }

        if (info)
        {
            info->enabled = FALSE;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void resource_view::OnGenerateXML()
{
    VALID_PROJECT
    if (!mpCurrentItem)
    {
        return;
    }

    if (mpCurrentItem->mpRes)
    {
        CFileDialog dlg(FALSE, _T(".xml"), mpCurrentItem->mpRes->name,
            OFN_SHOWHELP | OFN_OVERWRITEPROMPT,
            _T("XML Files|*.xml||"),
            this);

        if (dlg.DoModal() == IDOK)
        {
            CString filename = dlg.GetFileName();
            int index = filename.ReverseFind('.');
            if (index >= 0)
            {
                // Remove extension.
                filename = filename.Left(index);
            }
            project->GenerateResourceXml(dlg.GetPathName(), filename, mpCurrentItem->mpRes);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void resource_view::RestoreStringIdsFromBackup(folder_info *write, folder_info *read)
{
    while (write && read)
    {
        RestoreStringIdsFromBackup(write->GetFirstChildWidget(), read->GetFirstChildWidget());
        write = write->GetNextFolder();
        read = read->GetNextFolder();
    }
}

////////////////////////////////////////////////////////////////////////////////
void resource_view::RestoreStringIdsFromBackup(widget_info *write, widget_info *read)
{
    widget_service_provider *provider;
    GX_RESOURCE_ID *read_id_list;
    GX_RESOURCE_ID *write_id_list;
    int count;
    int index;

    while(write && read)
    {
        if (write->GetChildWidgetInfo() && read->GetChildWidgetInfo())
        {
            RestoreStringIdsFromBackup(write->GetChildWidgetInfo(), read->GetChildWidgetInfo());
        }

        if (read->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
        {
            read_id_list = read->ewi.string_scroll_wheel.string_id_list;
            write_id_list = write->ewi.string_scroll_wheel.string_id_list;
            count = read->ewi.string_scroll_wheel.base.total_rows;
        }
        else
        {
            read_id_list = read->string_id;
            write_id_list = write->string_id;
            count = NUM_WIDGET_STRINGS;
        }

        if (write_id_list && read_id_list)
        {
            provider = widget_factory::GetServiceProvider(write->basetype);
            if (provider)
            {
                for (index = 0; index < count; index++)
                {
                    if (write_id_list[index] != read_id_list[index])
                    {
                        provider->AssignText(write, index, read_id_list[index]);
                    }
                }
            }
        }

        write = write->GetNextWidgetInfo();
        read = read->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditStringTable()
{
    VALID_PROJECT
    folder_info *widget_backup = NULL;

    // clone the string table and the widget information in case the user cancels:
    string_table *table = project->mDisplays[mDisplayIndex].stable;

    if (!table)
    {
        return;
    }
    string_table *clone_table = new string_table(*table);
 
    // clone the widget info
    if (project->mDisplays[mDisplayIndex].GetFirstChildFolder())
    {
        widget_backup = new folder_info(*project->mDisplays[mDisplayIndex].GetFirstChildFolder(), TRUE);
    }

    CRect size;

    string_table_edit_dlg dlg(table, this);
    
    if (dlg.DoModal() == IDOK)
    {
        delete clone_table;     // delete the table backup
        delete widget_backup;   // delete backup widget info
        GetPropsWin()->ResourcesChanged();
        project->SetModified();
    }
    else
    {
        // delete the modified project and replace it with our backup and reload the resource tree
        delete project->mDisplays[mDisplayIndex].stable;
        project->mDisplays[mDisplayIndex].stable = clone_table;
        project->mHeader.num_languages = clone_table->CountLanguages();
        RebuildStringItems();
        RestoreStringIdsFromBackup(project->mDisplays[mDisplayIndex].GetFirstChildFolder(), widget_backup);
        delete widget_backup;

        widget_factory::UpdateInputWidgetText(project);

        GetProjectView()->DeSelect();
        GetPropsWin()->ResourcesChanged();
    }

    string_table::EnableDisableRuntimeBidiText(project->mDisplays[mDisplayIndex].stable->GetActiveLanguage());
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEditLanguages()
{
    VALID_PROJECT
    config_languages_dlg dlg;
    dlg.DoModal();
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnSetActiveLanguage(int delta)
{
    VALID_PROJECT

    string_table *table = project->mDisplays[mDisplayIndex].stable;

    int num_languages = table->CountLanguages();
    int current_language = table->GetActiveLanguage();

    if (delta == 1)
    {
        if (current_language + 1 < num_languages)
        {
            table->SetActiveLanguage(current_language + 1);
            RebuildStringItems(FALSE);
        }
    }
    else
    {
        if (current_language > 0)
        {
            table->SetActiveLanguage(current_language - 1);
            RebuildStringItems(FALSE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnSetActiveTheme(int delta)
{
    VALID_PROJECT

    int total_themes = project->mDisplays[mDisplayIndex].num_themes;
    int active_theme = project->mDisplays[mDisplayIndex].active_theme;

    active_theme += delta;

    if ((active_theme >= 0) && (active_theme < total_themes))
    {
        project->mDisplays[mDisplayIndex].active_theme = active_theme;

        /* Font resource can be different for different theme,
           clear string table char_map so font with include_string_table_c style can be recalculate. */
        project->mDisplays[mDisplayIndex].stable->ClearStringTableCharMap();

        // install active theme resources
        InstallResources(mDisplayIndex);
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::IsOpen(int folder_id)
{
    resource_item *item = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_GROUP, folder_id);
    
    if (item)
    {
        return item->IsOpen();
    }
    else
    {
        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::SetResFolderStatus(int folder_id, int open_status)
{
    resource_item *item = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_GROUP, folder_id);

    if (!item)
    {
        return;
    }

    if (open_status)
    {
        if (!item->IsOpen())
        {
            item->Open();
        }
    }
    else
    {
        if (item->IsOpen())
        {
            item->Close();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    mTimerTicks++;

    if (nIDEvent != DRAG_MOVE_TIMER_ID)
    {
        CScrollView::OnTimer(nIDEvent);
    }

    POINT pt;
    GetCursorPos(&pt);
    RECT rect;
    GetClientRect(&rect);
    ClientToScreen(&rect);

    if (pt.y < rect.top + 10)
    {
        //scroll up
        int slowscroll = 6 - (rect.top + 10 - pt.y) / 10;
        if (0 == (mTimerTicks % (slowscroll > 0 ? slowscroll : 1)))
        {
            SendMessage(WM_VSCROLL, SB_LINEUP);
        }
    }
    else if (pt.y > rect.bottom - 10)
    {
        //scroll down
        int slowscroll = 6 - (pt.y - rect.bottom + 10) / 10;
        if (0 == (mTimerTicks % (slowscroll > 0 ? slowscroll : 1)))
        {
            SendMessage(WM_VSCROLL, SB_LINEDOWN);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::AddColor(GX_COLOR color, CString name, resource_item *parent)
{
    // Add a res_info element to the project:
    VALID_PROJECT

    res_info *info = project->AddCustomColor(color, name, parent->mpRes);

    // insert this item into the display tree:
    InsertTreeResources(parent, info);

    SyncResourceAdd(info);

    if (!parent->IsOpen())
    {
        parent->Open();
    }
    mpTree->PositionItems();

    // make a new color table for GUIX:
    InstallColorTable(mDisplayIndex);
    GetPropsWin()->ResourcesChanged();
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::AddPixelmap(resource_item *parent, res_info *info, BOOL palette_mode)
{
    VALID_PROJECT

    //palette_info *pal = NULL;
    parent->mpRes->Attach(info);
    project->AddToResourceDictionary(mDisplayIndex, info);

    if (!palette_mode)
    {
        project->InitializeOnePixelmap(info, NULL);
    }

    // insert this item into the display tree:
    InsertTreeResources(parent, info);
    SyncResourceAdd(info);
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::TaskAddPixelmaps(resource_item *parent, CStringArray *patharray)
{
    VALID_PROJECT
    CString pathname;
    BOOL    add_pixelmap;
    BOOL  palette_mode = FALSE;

    int count = patharray->GetCount();

    palette_mode = project->IsPaletteMode(mDisplayIndex);

    while (count--)
    {
        add_pixelmap = FALSE;

        pathname = patharray->GetAt(count);

        FormatPath(pathname);

        CString res_name;
        int index = pathname.ReverseFind('\\');

        if (index >= 0)
        {
            res_name = pathname.Mid(index + 1);
        }
        else
        {
            res_name = pathname;
        }

        res_name = RemoveFileExtension(res_name);

        /*Check file exists in all pixelmaps forlder?*/
        if (!FileExists(mDisplayIndex, RES_TYPE_PIXELMAP, pathname))
        {
            add_pixelmap = TRUE;
        }
        else
        {
            CString message;
            CString temp_str("already exists. Do you still want to add it?");
            message = res_name + " " + temp_str;
            if (AskUser(CW2A(message)) == TRUE)
            {
                add_pixelmap = TRUE;
            }
        }

        if (add_pixelmap)
        {
            res_name.MakeUpper();
            /*Set available Id name for import pixelmap file.*/
            SetImportIdName(mDisplayIndex, res_name);

            res_info *info = new res_info(RES_TYPE_PIXELMAP);
            info->name = res_name;
            info->pathinfo.pathtype = PATH_TYPE_PROJECT_RELATIVE;
            SaveToPathInfo(info->pathinfo, pathname);

            if (!parent->mpRes->enabled)
            {
                info->enabled = GX_FALSE;
            }

            if (palette_mode)
            {
                info->palette_type = PALETTE_TYPE_SHARED;
            }

            // if the source has an alpha channel, default to keeping it
            CString abspath = MakeAbsolutePathname(info->pathinfo);
            image_reader* pReader = image_reader::CreateProperReader(abspath);

            if (pReader)
            {
                info->keep_alpha = pReader->CheckImageHasAlphaChannel(abspath);
                delete pReader;
            }

            if ((image_reader::GetImageType(abspath) == IMAGE_TYPE_GIF) &&
                (project->mDisplays[mDisplayIndex].colorformat > GX_COLOR_FORMAT_8BIT_PALETTE))
            {
                info->output_color_format = GX_COLOR_FORMAT_8BIT_PALETTE;
                info->palette_type = PALETTE_TYPE_PRIVATE;
            }

            AddPixelmap(parent, info, palette_mode);
        }
    }

    if (!parent->IsOpen())
    {
        this->SendMessage(USR_MSG_OPEN_RESOURCE_ITEM, (WPARAM)parent, 0);
    }

    if (palette_mode)
    {
        project->InitializeThemePixelmaps(mDisplayIndex, project->mDisplays[mDisplayIndex].active_theme);
    }
}

static DWORD WINAPI TaskAddPixelmapsThreadEntry(LPVOID thread_input)
{
    resource_view *res_view = (resource_view *) GetResourceView();

    if (res_view)
    {
        void **pointer_array = (void **)thread_input;
        resource_item *parent = (resource_item *) pointer_array[0];
        CStringArray *patharray = (CStringArray *) pointer_array[1];

        res_view->TaskAddPixelmaps(parent, patharray);
    }

    EndBusyMsg();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::AddPixelmaps(resource_item *parent, CStringArray *patharray)
{
    VALID_PROJECT

    void *param_array[2] = { (void*)parent, (void*)patharray };
    StartWorkThread(TaskAddPixelmapsThreadEntry, param_array, "Initializing Pixelmaps.", TRUE);

    mpTree->PositionItems();
    InstallPixelmapTable(mDisplayIndex);
    GetPropsWin()->ResourcesChanged();
    project->SetModified();
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::InitializePixelmaps(resource_item *item)
{
    VALID_PROJECT

    resource_item *child = item;
    res_info *info;

    info = child->mpRes;

    if (!info)
    {
        return;
    }

    switch (info->type)
    {
    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        child = child->First();
        while (child)
        {
            InitializePixelmaps(child);

            child = child->Next();
        }
        break;

    case RES_TYPE_PIXELMAP:
        if (info && info->type == RES_TYPE_PIXELMAP && info->is_modified)
        {
            int display = project->GetDisplayIndex(info);
            int theme = project->mDisplays[display].active_theme;

            if (project->GetDisplayColorFormat(info) == GX_COLOR_FORMAT_8BIT_PALETTE ||
                project->TestForPixelmapsUsingGlobalPalette(project->mDisplays[display].themes[theme].GetFirstResourceInfo()))
            {
                project->InitializeThemePixelmaps(display, theme);
            }
            else
            {
                project->InitializeOnePixelmap(info, NULL);
            }

            // Rebuild preview
            child->RebuildPreview();
            info->is_modified = FALSE;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::TaskInitializePixelmaps(resource_item *item)
{
    InitializePixelmaps(item);
    InstallPixelmapTable(mDisplayIndex);
}

//////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI InitializePixelmapsThreadEntry(LPVOID thread_input)
{
    resource_view *res_view = GetResourceView();

    if (res_view)
    {
        res_view->TaskInitializePixelmaps((resource_item *)thread_input);
    }

    EndBusyMsg();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::EditPixelmaps(resource_item *item)
{
    VALID_PROJECT

    StartWorkThread(InitializePixelmapsThreadEntry, (LPVOID)item, "Initializing Pixelmap.", TRUE);

    mpTree->PositionItems();
    project->SetModified();
    GetPropsWin()->ResourcesChanged();
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::SyncResourceName(CString &old_name, res_info *info)
{
    VALID_PROJECT

    res_info *current_info;
    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            continue;
        }

        current_info = project->FindResource(mDisplayIndex, theme, info->type, old_name);

        if (current_info)
        {
            current_info->name = info->name;
        }
        else
        {
            CString msg;

            msg.Format(_T("Internal Error: resource item \"%s\" in theme \"%s\" not found"),info->name,
                project->mDisplays[mDisplayIndex].themes[theme].theme_name);
            ErrorMsg(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::SyncResourceAdd(res_info *info)
{
    VALID_PROJECT

    res_info *parent;
    res_info *current_info;

    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            continue;
        }
        else
        {
            if (info->parent)
            {
                if (info->parent->type == RES_TYPE_FOLDER)
                {
                    parent = project->FindResourceFolder(mDisplayIndex, theme, info->parent->type, info->parent->folder_id, info->parent->name);
                }
                else
                {
                    parent = project->FindResource(mDisplayIndex, theme, info->parent->type, info->parent->name);
                }
            }
            else
            {
                parent = NULL;
            }
        }

        if (parent)
        {
            current_info = new res_info(NULL, *info, TRUE);
            parent->Attach(current_info);

            switch (current_info->type)
            {
            case RES_TYPE_FONT:
            {
                GX_FONT *font = MakeFont(current_info, mDisplayIndex, TRUE);

                if (font)
                {
                    current_info->font = font;

                    /* Get output font size. */
                    current_info->storage_size = GetFontStorage(current_info, project, mDisplayIndex);
                }

                break;
            }

            case RES_TYPE_PIXELMAP:
                /* KGM_PALETTE_MODE
                   Need to update theme palette if running in
                   8bit palette mode
                */
                project->InitializeOnePixelmap(current_info, NULL);
                break;
            }
        }
        else
        {
            CString msg;
            msg.Format(_T("Internal Error: the parent of resource item \"%s\" in theme \"%s\" not found"), info->name,
                project->mDisplays[mDisplayIndex].themes[theme].theme_name);
            ErrorMsg(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::SyncResourcePath(res_info* info)
{
    VALID_PROJECT

    res_info* current_info;
    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            continue;
        }

        current_info = project->FindResource(mDisplayIndex, theme, info->type, info->name);

        if (current_info)
        {
            current_info->pathinfo = info->pathinfo;

            int theme = project->mDisplays[mDisplayIndex].active_theme;

            if (project->GetDisplayColorFormat(info) == GX_COLOR_FORMAT_8BIT_PALETTE ||
                project->TestForPixelmapsUsingGlobalPalette(project->mDisplays[mDisplayIndex].themes[theme].GetFirstResourceInfo()))
            {
                project->InitializeThemePixelmaps(mDisplayIndex, theme);
            }
            else
            {
                project->InitializeOnePixelmap(current_info, NULL);
            }
        }
        else
        {
            CString msg;

            msg.Format(_T("Internal Error: resource item \"%s\" in theme \"%s\" not found"), info->name,
                project->mDisplays[mDisplayIndex].themes[theme].theme_name);
            ErrorMsg(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::DeleteResourceChildren(res_info* parent)
{
    VALID_PROJECT

    res_info *child = parent->child;
    res_info *next;

    while (child)
    {
        next = child->next;

        project->DeleteResource(child);

        child = next;
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::SyncResourceDelete(res_info *info)
{
    VALID_PROJECT

    res_info *current_info;

    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            continue;
        }
        else
        {
            if (info->type == RES_TYPE_FOLDER)
            {
                current_info = project->FindResourceFolder(mDisplayIndex, theme, info->type, info->folder_id, info->name);

                if (current_info)
                {
                    DeleteResourceChildren(current_info);
                }
            }
            else
            {
                current_info = project->FindResource(mDisplayIndex, theme, info->type, info->name);
            }
        }

        if (current_info)
        {
            project->DeleteResource(current_info);
        }
        else
        {
            CString msg;

            msg.Format(_T("Internal Error: resource item \"%s\" in theme \"%s\" not found"), info->name,
                project->mDisplays[mDisplayIndex].themes[theme].theme_name);
            ErrorMsg(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void resource_view::ChangeItemParent(resource_item *item, resource_item *new_parent)
{
    VALID_PROJECT

    res_info *info;
    res_info *parent_info;

    for (int theme = 0; theme < project->mDisplays[mDisplayIndex].num_themes; theme++)
    {
        if (theme == project->mDisplays[mDisplayIndex].active_theme)
        {
            info = item->mpRes;
            parent_info = new_parent->mpRes;

            item->Detach();
            new_parent->Attach(item);
            item->SetPos(CRect(0, 0, 0, 0));
            mpTree->PositionItems();
        }
        else
        {
            info = project->FindResource(mDisplayIndex, theme, item->mpRes->type, item->mpRes->name);
            parent_info = project->FindResource(mDisplayIndex, theme, new_parent->mpRes->type, new_parent->mpRes->name);
        }

        info->Detach();
        parent_info->Attach(info);
    }
}

//////////////////////////////////////////////////////////////////////////////
LRESULT resource_view::OnRebuildStringItems(WPARAM wParam, LPARAM lParam)
{
    RebuildStringItems((BOOL)wParam);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
LRESULT resource_view::OnUpdateStringTableFonts(WPARAM wParam, LPARAM lParam)
{
    FontCharMap *charmap = (FontCharMap *)wParam;
    UpdateStringTableFonts(charmap);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
LRESULT resource_view::OnOpenResourceItem(WPARAM wParam, LPARAM lParam)
{
    resource_item *res_item = (resource_item *)wParam;
    if (!res_item->IsOpen())
    {
        res_item->Open();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
LRESULT resource_view::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    resource_item *item;
    res_info *info = NULL;
    int index;
    CStringArray param;
    CStringArray path_array;
    CString folder_name;
    CString path;
    CString name;
    int res_type;
    int res_id;
    studiox_project *project;

    switch (wParam)
    {
    case TEST_ADD_PIXELMAPS:
        //parameter = folder_name,path,name1,name2,...
        SplitString(GetTestingParam(1), ',', &param);

        folder_name = param.GetAt(0);
        path = param.GetAt(1);

        for (index = 2; index < param.GetCount(); index++)
        {
            path_array.Add(path + param.GetAt(index));
        }

        // Find parent item
        item = mpTree->FindFolderNamed(mpTree->GetRoot(), RES_TYPE_FOLDER, CUSTOM_PIXELMAP_FOLDER, folder_name);

        if (item)
        {
            // for testing, don't flag missing image file
            project = GetOpenProject();
            project->mHeader.warn_missing_image = FALSE;

            // Add pixelmaps to parent
            AddPixelmaps(item, &path_array);
        }
        break;

    case TEST_CLICK_RESOURCE_GROUP:
        item = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_GROUP, lParam);
        if (item)
        {
            SetCurrentItem(item);

            if (item->IsOpen())
            {
                item->Close();
            }
            else
            {
                item->Open();
            }
        }
        break;

    case TEST_CLICK_PIXELMAP_FOLDER:
        SplitString(GetTestingParam(0), ',', &param);

        res_id = _tstoi(param.GetAt(0));
        name = param.GetAt(1);

        item = mpTree->FindFolderNamed(mpTree->GetRoot(), RES_TYPE_FOLDER, res_id, name);

        if (item)
        {
            SetCurrentItem(item);

            if (item->IsOpen())
            {
                item->Close();
            }
            else
            {
                item->Open();
            }
        }
        break;

    case TEST_CLICK_RESOURCE_ITEM:
        project = GetOpenProject();
        SplitString(GetTestingParam(0), ',', &param);

        res_type = _tstoi(param.GetAt(0));
        name = param.GetAt(1);

        item = mpTree->FindItem(mpTree->GetRoot(), res_type, name);

        if (item)
        {
            SetCurrentItem(item);
        }
        break;

    case TEST_ADD_COLOR:
        name = _T("Add New Color");
        item = mpTree->FindItem(mpTree->GetRoot(), RES_TYPE_ADD_COLOR, name);
        if (item)
        {
            SetCurrentItem(item);
            OnAddCustomColor();
        }
        break;

    case TEST_EDIT_COLOR:
        OnEditCustomColor();
        break;

    case TEST_DELETE_COLOR:
        OnDeleteCustomColor();
        break;

    case TEST_ADD_FONT:
        name = _T("Add New Font");
        item = mpTree->FindItem(mpTree->GetRoot(), RES_TYPE_ADD_FONT, name);
        if (item)
        {
            SetCurrentItem(item);
            OnAddFont();
        }
        break;

    case TEST_EDIT_FONT:
        OnEditFont();
        break;

    case TEST_DELETE_FONT:
        OnDeleteFont();
        break;

    case TEST_EDIT_STRING:
        OnEditStringTable();
        break;

    case TEST_EDIT_PIXELMAP:
        OnEditPixelmap();
        break;

    case TEST_EDIT_PIXELMAPS:
        OnEditPixelmaps();
        break;

    case TEST_DELETE_PIXELMAP:
        OnDeletePixelmap();
        break;

    case TEST_ENABLE_PIXELMAP:
        OnEnablePixelmap();
        break;

    case TEST_DISABLE_PIXELMAP:
        OnDisablePixelmap();
        break;

    case TEST_ADD_PIXELMAP_FOLDER:
        OnAddPixelmapFolder();
        break;

    case TEST_REMOVE_PIXELMAP_FOLDER:
        OnRemovePixelmapFolder();
        break;

    case TEST_ENABLE_PIXELMAP_FOLDER:
        OnEnablePixelmapFolder();
        break;

    case TEST_DISABLE_PIXELMAP_FOLDER:
        OnDisablePixelmapFolder();
        break;

    case TEST_INCREMENT_ACTIVE_LANGUAGE_INDEX:
        OnSetActiveLanguage(1);
        break;

    case TEST_DECREMENT_ACTIVE_LANGUAGE_INDEX:
        OnSetActiveLanguage(-1);
        break;

    case TEST_GENERATE_XML:
        project = GetOpenProject();
        if (project && mpCurrentItem)
        {
            path = GetTestingParam(0);
            index = path.ReverseFind('\\');
            if (index < 0)
            {
                index = path.ReverseFind('/');
            }

            if (index >= 0)
            {
                name = path.Mid(index + 1);
                name = RemoveFileExtension(name);
            }
            project->GenerateResourceXml(path, name, mpCurrentItem->mpRes);
        }
        break;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
void resource_view::OnSetFocus(CWnd* pOldWnd)
{
    CScrollView::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    if (mpCurrentItem)
    {
        CRect rect = mpCurrentItem->GetPos();
        int scrollpos = GetScrollPos(SB_VERT);
        rect.top -= scrollpos;
        rect.bottom -= scrollpos;
        InvalidateRect(rect);

        if (mpCurrentItem->GetResItemProvider())
        {
            mpCurrentItem->GetResItemProvider()->RaiseChangeFocusEvent();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnKillFocus(CWnd* pNewWnd)
{
    CScrollView::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
    if (mpCurrentItem)
    {
        CRect rect = mpCurrentItem->GetPos();
        int scrollpos = GetScrollPos(SB_VERT);
        rect.top -= scrollpos;
        rect.bottom -= scrollpos;
        InvalidateRect(rect);
    }
}

///////////////////////////////////////////////////////////////////////////////
resource_view_provider* resource_view::GetResViewProvider()
{
    if (m_pResViewProvider == NULL)
    {
        m_pResViewProvider = new resource_view_provider(this);
    }
    return m_pResViewProvider;
}

///////////////////////////////////////////////////////////////////////////////
int resource_view::CreateUniqueId()
{
    static int uniqueId;
    return uniqueId++;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::SelectNextItem()
{
    if (!mpCurrentItem)
    {
        return;
    }

    CRect size = mpCurrentItem->GetPos();
    CPoint cp(size.left, size.bottom + (mpCurrentItem->GetItemHeight() / 2));

    resource_item *next = mpTree->FindItem(mpTree->GetRoot(), cp);

    if (next)
    {
        SetCurrentItem(next);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::SelectPreviousItem()
{
    if (!mpCurrentItem)
    {
        return;
    }

    CRect size = mpCurrentItem->GetPos();
    CPoint cp(size.left, size.top - (mpCurrentItem->GetItemHeight() / 2));

    resource_item *previous = mpTree->FindItem(mpTree->GetRoot(), cp);

    if (previous)
    {
        SetCurrentItem(previous);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::OnReturnKey(int item_type)
{
    switch (item_type)
    {
    case RES_TYPE_COLOR:
    {
        if (!GetCurrentItem())
        {
            return FALSE;
        }

        resource_item* parent = GetCurrentItem()->Parent();

        if (parent && (parent->GetResType() == RES_TYPE_FOLDER))
        {
            if (parent->mpRes->folder_id == DEFAULT_COLOR_FOLDER)
            {
                OnEditSystemColor();
            }
            else
            {
                OnEditCustomColor();
            }

            return TRUE;
        }
    }
        break;

    case RES_TYPE_ADD_COLOR:
        OnAddCustomColor();
        return TRUE;

    case RES_TYPE_FONT:
        OnEditFont();
        return TRUE;

    case RES_TYPE_ADD_FONT:
        OnAddFont();
        return TRUE;

    case RES_TYPE_PIXELMAP:
        OnEditPixelmap();
        return TRUE;

    case RES_TYPE_ADD_PIXELMAP:
        OnAddPixelmaps();
        return TRUE;

    case RES_TYPE_ADD_STRING:
    case RES_TYPE_STRING:
        OnEditStringTable();
        return TRUE;

    default:
        break;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::OnTabKey(int item_type)
{
    int nShiftKey = GetKeyState(VK_SHIFT);

    switch (item_type)
    {
    case RES_TYPE_HEADER:
    case RES_TYPE_GROUP:
        if (nShiftKey & KEY_STATE_DOWN)
        {
            if (mpCurrentItem->Previous())
            {
                SetCurrentItem(mpCurrentItem->Previous());
            }
        }
        else
        {
            if (mpCurrentItem->Next())
            {
                SetCurrentItem(mpCurrentItem->Next());
            }
        }
        return TRUE;

    default:
        break;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::OnDirectionKey(int item_type, int key)
{
    switch (key)
    {
    case VK_DOWN:
        SelectNextItem();
        return TRUE;

    case VK_UP:
        SelectPreviousItem();
        return TRUE;

    default:
        break;
    }

    switch (item_type)
    {
    case RES_TYPE_HEADER:
        if (key == VK_LEFT)
        {
            OnSetActiveTheme(-1);
        }
        else if (key == VK_RIGHT)
        {
            OnSetActiveTheme(1);
        }
        return TRUE;

    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        if (key == VK_LEFT)
        {
            if (mpCurrentItem->IsOpen())
            {
                //collapse group or folder
                mpCurrentItem->Close();
                mpTree->PositionItems();
            }
        }
        else if (key == VK_RIGHT)
        {
            if (!mpCurrentItem->IsOpen())
            {
                //expand group or folder
                mpCurrentItem->Open();
                mpTree->PositionItems();
            }
        }
        return TRUE;

    case RES_TYPE_STRING:
        if (key == VK_LEFT)
        {
            OnSetActiveLanguage(-1);
        }
        else if (key == VK_RIGHT)
        {
            OnSetActiveLanguage(1);
        }
        return TRUE;

    default:
        break;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnPageDonw(int item_type)
{
    CRect client;
    CRect size = mpCurrentItem->GetPos();

    GetClientRect(&client);
    CPoint cp(size.left, size.top + client.Height());

    Scroll(client.Height());

    resource_item* next = mpTree->FindItem(mpTree->GetRoot(), cp);

    if (next)
    {
        SetCurrentItem(next);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnPageUp(int item_type)
{
    CRect client;
    CRect size = mpCurrentItem->GetPos();

    GetClientRect(&client);
    CPoint cp(size.left, size.top - client.Height());

    resource_item* previous = mpTree->FindItem(mpTree->GetRoot(), cp);

    Scroll(-client.Height());

    if (previous)
    {
        SetCurrentItem(previous);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnHome(int item_type)
{
    resource_item *item = mpTree->GetRoot()->First();
    if (item)
    {
        SetCurrentItem(item);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnEnd(int item_type)
{
    resource_item *item;


    item = mpTree->FindFolder(mpTree->GetRoot(), RES_TYPE_GROUP, STRING_GROUP);

    if (item && item->IsOpen())
    {
        CString res_name("Add New String");

        item = mpTree->FindItem(item, RES_TYPE_ADD_STRING, res_name);
    }

    if (item)
    {
        SetCurrentItem(item);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_view::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_SYSKEYDOWN)
    {
        int nAltKey = GetKeyState(VK_MENU);

        switch (pMsg->wParam)
        {
        case VK_UP:
        case VK_LEFT:
            if (nAltKey & KEY_STATE_DOWN)
            {
                //move focus to target view
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
    else if (pMsg->message == WM_KEYDOWN)
    {
        int item_type = -1;
        int key = pMsg->wParam;

        if (mpCurrentItem)
        {
            item_type = mpCurrentItem->GetResType();
        }

        switch (key)
        {
        case VK_TAB:
            if (OnTabKey(item_type))
            {
                return TRUE;
            }
            break;

        case VK_RETURN:
            if (OnReturnKey(item_type))
            {
                return TRUE;
            }
            break;

        case VK_LEFT:
        case VK_RIGHT:
        case VK_DOWN:
        case VK_UP:
            if (OnDirectionKey(item_type, key))
            {
                return TRUE;
            }
            break;

        case VK_PRIOR:
            OnPageUp(item_type);
            return TRUE;

        case VK_NEXT:
            OnPageDonw(item_type);
            return TRUE;

        case VK_HOME:
            OnHome(item_type);
            return TRUE;

        case VK_END:
            OnEnd(item_type);
            return TRUE;

        default:
            break;
        }
    }

    return CScrollView::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT resource_view::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_GETOBJECT:
        // Register the control with UI Automation.
        // If the lParam matches the RootObjectId, send back the tree view provider.
        if (static_cast<long>(lParam) == static_cast<long>(UiaRootObjectId))
        {
            // Return its associated UI Automation provider.
            LRESULT lresult = UiaReturnRawElementProvider(
                GetSafeHwnd(), wParam, lParam, GetResViewProvider());
            return lresult;
        }
        return 0;

    case SBM_GETSCROLLINFO:
    {
        SCROLLINFO* si = (SCROLLINFO*)lParam;
        if (si->fMask == SIF_POS)
        {
            si->nPos = GetScrollPos(SB_VERT);
        }
        return 0;
    }

    default:
        // Do nothing.
        break;
    }

    return CScrollView::WindowProc(message, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
void resource_view::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CScrollView::OnSettingChange(uFlags, lpszSection);

    if (mDisplayIndex >= 0)
    {
        InstallResources(mDisplayIndex);
    }
}
