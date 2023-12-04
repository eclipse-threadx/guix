#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum resource_export_dlg_test_commands{
    TEST_CHECK_BINARY_MODE = 1,
    TEST_CHECK_RES_HEADER_GEN,
    TEST_CHECK_RESOURCE,
    TEST_SET_CUSTOM_RESOURCE_NAME,
    TEST_GENERATE,
    TEST_CANCEL
};

BEGIN_MESSAGE_MAP(resource_export_dlg, express_dialog)
    ON_BN_CLICKED(IDOK, &resource_export_dlg::OnBnClickedOk)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
    ON_BN_CLICKED(IDC_CUSTOM_RESOURCE, &resource_export_dlg::OnBnClickedCustomResource)
    ON_BN_CLICKED(IDC_BINARY_FORMAT_SREC, &resource_export_dlg::OnSelectSRecord)
    ON_BN_CLICKED(IDC_BINARY_FORMAT_BIN, &resource_export_dlg::OnSelectBinary)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(resource_export_dlg, express_dialog)


/////////////////////////////////////////////////////////////////////////////////////
resource_export_dlg::resource_export_dlg(CWnd *pParent)
    : express_dialog(resource_export_dlg::IDD, pParent)
{
    IconId = IDB_CHECK;
    SetTitleText("Select Export Resources");
    b_modified = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////
resource_export_dlg::~resource_export_dlg()
{
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::DoDataExchange(CDataExchange* pDX)
{
    // TODO: Add your specialized code here and/or call the base class
    DDX_Control(pDX, IDC_RES_VIEW, mResView);

    studiox_project *project = GetOpenProject();
    CString text;

    if (pDX->m_bSaveAndValidate)
    {
        CButton *pCheck = (CButton *)GetDlgItem(IDC_BINARY_MODE);
        if(pCheck->GetCheck() !=project->mHeader.gen_binary)
        {
            project->mHeader.gen_binary = pCheck->GetCheck();
            b_modified = TRUE;
        }

        ULONG binary_format;
        ULONG memory_offset;
        pCheck = (CButton *)GetDlgItem(IDC_BINARY_FORMAT_SREC);
        if (pCheck->GetCheck())
        {
            binary_format = BINARY_FILE_FORMAT_SREC;
        }
        else
        {
            binary_format = BINARY_FILE_FORMAT_BIN;
        }

        GetDlgItemText(IDC_MEM_OFFSET_EDIT, text);
        memory_offset = _tcstoul(text.GetBuffer(), NULL, 16);

        if (binary_format != project->mHeader.binary_file_format ||
            memory_offset != project->mHeader.memory_offset)
        {
            project->mHeader.binary_file_format = binary_format;
            project->mHeader.memory_offset = memory_offset;
            b_modified = TRUE;
        }

        pCheck = (CButton *)GetDlgItem(IDC_GEN_RES_HEADER);
        if (pCheck->GetCheck() != project->mHeader.gen_res_header)
        {
            project->mHeader.gen_res_header = pCheck->GetCheck();
            b_modified = TRUE;
        }

        pCheck = (CButton *)GetDlgItem(IDC_CUSTOM_RESOURCE);
        if (pCheck->GetCheck() != project->mHeader.custom_resource_enabled)
        {
            project->mHeader.custom_resource_enabled = pCheck->GetCheck();
            b_modified = TRUE;
        }

        if (project->mHeader.custom_resource_enabled)
        {
            CString custom_resource_file_name;
            GetDlgItemText(IDC_CUSTOM_RESOURCE_FILE_NAME, custom_resource_file_name);

            if (custom_resource_file_name.Compare(project->mHeader.custom_resource_file_name))
            {
                project->mHeader.custom_resource_file_name = custom_resource_file_name;
                b_modified = TRUE;
            }
        }
    }
    else
    {
        DDX_Check(pDX, IDC_BINARY_MODE, project->mHeader.gen_binary);
        DDX_Check(pDX, IDC_GEN_RES_HEADER, project->mHeader.gen_res_header);
        DDX_Check(pDX, IDC_CUSTOM_RESOURCE, project->mHeader.custom_resource_enabled);

        int check;
        check = project->mHeader.binary_file_format & BINARY_FILE_FORMAT_BIN;
        ((CButton *)GetDlgItem(IDC_BINARY_FORMAT_BIN))->SetCheck(check);
        check = project->mHeader.binary_file_format & BINARY_FILE_FORMAT_SREC;
        ((CButton *)GetDlgItem(IDC_BINARY_FORMAT_SREC))->SetCheck(check);

        if (project->mHeader.memory_offset <= 0xffff)
        {
            text.Format(_T("%04x"), project->mHeader.memory_offset);
        }
        else if (project->mHeader.memory_offset < 0xffffff)
        {
            text.Format(_T("%06x"), project->mHeader.memory_offset);
        }
        else
        {
            text.Format(_T("%08x"), project->mHeader.memory_offset);
        }
        SetDlgItemText(IDC_MEM_OFFSET_EDIT, text);
        ShowHideMemoryOffset();

        if (project->mHeader.custom_resource_enabled)
        {
            GetDlgItem(IDC_CUSTOM_RESOURCE_FILE_NAME)->ShowWindow(TRUE);
            SetDlgItemText(IDC_CUSTOM_RESOURCE_FILE_NAME, project->mHeader.custom_resource_file_name);
        }
        else
        {
            GetDlgItem(IDC_CUSTOM_RESOURCE_FILE_NAME)->ShowWindow(FALSE);
        }
    }

    express_dialog::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::OnBnClickedCustomResource()
{
    // TODO: Add your control notification handler code here
    CButton *binary = (CButton *)GetDlgItem(IDC_CUSTOM_RESOURCE);

    if (binary->GetCheck())
    {
        GetDlgItem(IDC_CUSTOM_RESOURCE_FILE_NAME)->ShowWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_CUSTOM_RESOURCE_FILE_NAME)->ShowWindow(FALSE);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::OnSelectSRecord()
{
    ShowHideMemoryOffset();
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::OnSelectBinary()
{
    ShowHideMemoryOffset();
}

/////////////////////////////////////////////////////////////////////////////////////
BOOL resource_export_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    AddCancelButton();
    AddSaveButton(_T("Generate"));

    SetControlAccessibleName(mResView.GetSafeHwnd(), _T("select export resource"));
    SetControlAccessibleName(GetDlgItem(IDC_CUSTOM_RESOURCE_FILE_NAME)->GetSafeHwnd(), _T("resource file name"));
    mResView.InitImageList();
    PopulateResourceTree();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::OnBnClickedOk()
{
    express_dialog::OnOK();

    SaveChecks();

    if (b_modified)
    {
        studiox_project *project = GetOpenProject();

        project->SetModified();
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::ShowHideMemoryOffset()
{
    int check = ((CButton *)GetDlgItem(IDC_BINARY_FORMAT_SREC))->GetCheck();

    GetDlgItem(IDC_MEM_OFFSET_LABEL)->ShowWindow(check);
    GetDlgItem(IDC_MEM_OFFSET_EDIT)->ShowWindow(check);
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::PopulateResourceTree()
{
    HTREEITEM hDisplays;

    hDisplays = mResView.InsertItem(_T("Displays"));
    AddDisplays(hDisplays);
    mResView.Expand(hDisplays, TVE_EXPAND);

    mResView.SelectSetFirstVisible(mResView.GetRootItem());
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::SaveChecks()
{
    HTREEITEM hRoot = mResView.GetRootItem();
    HTREEITEM hCurrent;

    hCurrent = mResView.FindItem(hRoot, _T("Displays"));
    SaveDisplayChecks(hCurrent);
}


/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::AddDisplays(HTREEITEM hParent)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    int state;

    for (int display = 0; display < project->mHeader.num_displays; display++)
    {
        HTREEITEM hDisplay = mResView.InsertItem(project->mDisplays[display].name, hParent);

        for (int theme = 0; theme < project->mDisplays[display].num_themes; theme++)
        {
            HTREEITEM hTheme = mResView.InsertItem(project->mDisplays[display].themes[theme].theme_name, hDisplay);

            HTREEITEM hThemeResource = mResView.InsertItem(_T("color table"), hTheme);
            if (project->mDisplays[display].themes[theme].gen_color_table)
            {
                mResView.SetItemState(hThemeResource, CUSTOM_TV_STATE_CHECKED);
            }
            else
            {
                mResView.SetItemState(hThemeResource, CUSTOM_TV_STATE_UNCHECKED);
            }

            hThemeResource = mResView.InsertItem(_T("font table"), hTheme);
            if (project->mDisplays[display].themes[theme].gen_font_table)
            {
                mResView.SetItemState(hThemeResource, CUSTOM_TV_STATE_CHECKED);
            }
            else
            {
                mResView.SetItemState(hThemeResource, CUSTOM_TV_STATE_UNCHECKED);
            }

            hThemeResource = mResView.InsertItem(_T("pixelmap table"), hTheme);
            if (project->mDisplays[display].themes[theme].gen_pixelmap_table)
            {
                mResView.SetItemState(hThemeResource, CUSTOM_TV_STATE_CHECKED);
            }
            else
            {
                mResView.SetItemState(hThemeResource, CUSTOM_TV_STATE_UNCHECKED);
            }

            state = mResView.GetChildrenState(hTheme);
            mResView.SetItemState(hTheme, state);

            mResView.Expand(hTheme, TVE_EXPAND);
        }

        HTREEITEM hLanguage = mResView.InsertItem(_T("Languages"), hDisplay);
        AddLanguages(hLanguage, display);
        mResView.Expand(hLanguage, TVE_EXPAND);

        state = mResView.GetChildrenState(hDisplay);
        mResView.SetItemState(hDisplay, state);

        mResView.Expand(hDisplay, TVE_EXPAND);
    }

    state = mResView.GetChildrenState(hParent);
    mResView.SetItemState(hParent, state);
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::AddLanguages(HTREEITEM hParent, int display_index)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }


    for (int index = 0; index < project->mHeader.num_languages; index++)
    {
        HTREEITEM hLanguage = mResView.InsertItem(project->mHeader.languages[index].name, hParent);

        if (project->mDisplays[display_index].gen_string_table[index])
        {
            mResView.SetItemState(hLanguage, CUSTOM_TV_STATE_CHECKED);
        }
        else
        {
            mResView.SetItemState(hLanguage, CUSTOM_TV_STATE_UNCHECKED);
        }
    }

    int state = mResView.GetChildrenState(hParent);
    mResView.SetItemState(hParent, state);
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::SaveDisplayChecks(HTREEITEM hParent)
{
    studiox_project *project = GetOpenProject();

    if ((!project) || (!hParent))
    {
        return;
    }

    HTREEITEM hDisplay = mResView.GetChildItem(hParent);
    UINT state;
    BOOL enabled = TRUE;

    for (int display = 0; display < project->mHeader.num_displays; display++)
    {
        if (hDisplay)
        {
            state = mResView.GetItemCheckState(hDisplay);

            if (state == CUSTOM_TV_STATE_UNCHECKED)
            {
                enabled = FALSE;
            }
            else
            {
                enabled = TRUE;
            }

            if (project->mDisplays[display].enabled != enabled)
            {
                project->mDisplays[display].enabled = enabled;
                b_modified = TRUE;
            }

            HTREEITEM hTheme = mResView.GetChildItem(hDisplay);

            for (int theme = 0; theme < project->mDisplays[display].num_themes; theme++)
            {
                state = mResView.GetItemCheckState(hTheme);

                if (state == CUSTOM_TV_STATE_UNCHECKED)
                {
                    enabled = FALSE;
                }
                else
                {
                    enabled = TRUE;
                }

                if (project->mDisplays[display].themes[theme].enabled != enabled)
                {
                    project->mDisplays[display].themes[theme].enabled = enabled;
                    b_modified = TRUE;
                }

                HTREEITEM hThemeResource = mResView.FindItem(hTheme, _T("color table"));
                state = mResView.GetItemCheckState(hThemeResource);

                if (state == CUSTOM_TV_STATE_UNCHECKED)
                {
                    enabled = FALSE;
                }
                else
                {
                    enabled = TRUE;
                }

                if (project->mDisplays[display].themes[theme].gen_color_table != enabled)
                {
                    project->mDisplays[display].themes[theme].gen_color_table = enabled;
                    b_modified = TRUE;
                }

                hThemeResource = mResView.FindItem(hTheme, _T("font table"));

                state = mResView.GetItemCheckState(hThemeResource);

                if (state == CUSTOM_TV_STATE_UNCHECKED)
                {
                    enabled = FALSE;
                }
                else
                {
                    enabled = TRUE;
                }

                project->mDisplays[display].themes[theme].gen_font_table = enabled;

                hThemeResource = mResView.FindItem(hTheme, _T("pixelmap table"));

                state = mResView.GetItemCheckState(hThemeResource);

                if (state == CUSTOM_TV_STATE_UNCHECKED)
                {
                    enabled = FALSE;
                }
                else
                {
                    enabled = TRUE;
                }

                if (project->mDisplays[display].themes[theme].gen_pixelmap_table != enabled)
                {
                    project->mDisplays[display].themes[theme].gen_pixelmap_table = enabled;
                    b_modified = TRUE;
                }

                hTheme = mResView.GetNextSiblingItem(hTheme);
            }

            HTREEITEM hLanguage = mResView.FindItem(hDisplay, _T("Languages"));
            SaveLanguageChecks(hLanguage, display);
        }

        hDisplay = mResView.GetNextSiblingItem(hDisplay);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void resource_export_dlg::SaveLanguageChecks(HTREEITEM hParent, int display_index)
{
    studiox_project *project = GetOpenProject();

    if ((!project) || (!hParent))
    {
        return;
    }

    HTREEITEM hChild = mResView.GetChildItem(hParent);
    UINT state;
    BOOL enabled;

    for (int index = 0; index < project->mHeader.num_languages; index++)
    {
        if (hChild)
        {
            state = mResView.GetItemCheckState(hChild);

            if (state == CUSTOM_TV_STATE_UNCHECKED)
            {
                enabled = FALSE;
            }
            else
            {
                enabled = TRUE;
            }

            if (project->mDisplays[display_index].gen_string_table[index] != enabled)
            {
                project->mDisplays[display_index].gen_string_table[index] = enabled;
                b_modified = TRUE;
            }
        }

        hChild = mResView.GetNextSiblingItem(hChild);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
BOOL resource_export_dlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    CWnd* focus_owner = GetFocus();

    if (focus_owner &&
        (focus_owner->GetDlgCtrlID() == IDC_MEM_OFFSET_EDIT) &&
        (pMsg->message == WM_CHAR) &&
        isprint(pMsg->wParam))
    {
        if (pMsg->wParam < '0' || pMsg->wParam > '9')
        {
            ErrorMsg("Unaccetable Character. You can only type a number here.", this);
            return TRUE;
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////////////
LRESULT resource_export_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd = NULL;
    HTREEITEM item;
    CStringArray param;
    CString parent_name;
    CString resource_name;
    int check;

    switch (wParam)
    {
    case TEST_CHECK_BINARY_MODE:
        pWnd = (CButton *)GetDlgItem(IDC_BINARY_MODE);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BINARY_MODE, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CHECK_RES_HEADER_GEN:
        pWnd = (CButton *)GetDlgItem(IDC_GEN_RES_HEADER);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GEN_RES_HEADER, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_CHECK_RESOURCE:
        SplitString(GetTestingParam(0), ',', &param);
        parent_name = param.GetAt(0);
        resource_name = param.GetAt(1);
        check = _tstoi(param.GetAt(2));
        item = mResView.FindItem(mResView.GetRootItem(), parent_name);
        if (item)
        {
            item = mResView.FindItem(item, resource_name);

            if (check)
            {
                mResView.SetItemState(item, CUSTOM_TV_STATE_CHECKED);
            }
            else
            {
                mResView.SetItemState(item, CUSTOM_TV_STATE_UNCHECKED);
            }

            mResView.UpdateChildrenState(item);
        }
        break;

    case TEST_SET_CUSTOM_RESOURCE_NAME:
        // check on "specify reousrce file name"
        pWnd = (CButton *)GetDlgItem(IDC_CUSTOM_RESOURCE);
        ((CButton *)pWnd)->SetCheck(TRUE);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CUSTOM_RESOURCE, BN_CLICKED), (LPARAM)pWnd->m_hWnd);

        // set custom resource file name
        SetDlgItemText(IDC_CUSTOM_RESOURCE_FILE_NAME, GetTestingParam(0));
        break;

    case TEST_GENERATE:
        OnBnClickedOk();
        break;

    case TEST_CANCEL:
        OnCancel();
        break;
    }

    return 0;
}

BEGIN_MESSAGE_MAP(CustomTreeCtrl, CTreeCtrl)
    ON_NOTIFY_REFLECT(NM_CLICK, &CustomTreeCtrl::OnNMClick)
    ON_NOTIFY_REFLECT(TVN_KEYDOWN, &CustomTreeCtrl::OnTvnKeydown)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CustomTreeCtrl::OnNodeSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    TVHITTESTINFO hti;
    hti.pt.x = pt.x;
    hti.pt.y = pt.y;
    HitTest(&hti);

    if (hti.flags & TVHT_ONITEMSTATEICON)
    {
        SelectItem(hti.hItem);
        OnSelectItem(hti.hItem);
    }

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::OnTvnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    if (pTVKeyDown->wVKey == VK_SPACE)
    {
        HTREEITEM item = GetSelectedItem();

        if (item != NULL)

        {
            OnSelectItem(item);
        }
    }

    *pResult = 0;
}

void CustomTreeCtrl::OnSelectItem(HTREEITEM hItem)
{
    UINT state = GetItemCheckState(hItem);

    switch (state)
    {
    case CUSTOM_TV_STATE_UNCHECKED:
        break;
    case CUSTOM_TV_STATE_CHECKED:
        state = CUSTOM_TV_STATE_PARTIAL_CHECKED;
        break;
    case CUSTOM_TV_STATE_PARTIAL_CHECKED:
        break;
    }

    // temporarily change item status 
    SetItemState(hItem, 1 + (state % 3));

    // update parent state
    UpdateParentState(hItem);

    // update children state
    UpdateChildrenState(hItem);

    // revert item status
    SetItemState(hItem, state);

    CWnd* parent = GetParent();

    if (parent)
    {
        parent->PostMessage(USR_MSG_CHECK_STATE_CHANGE, 1 + (state % 3), (LPARAM)hItem);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::OnNodeSelect(NMHDR* ph, LRESULT* pr)
{
    HTREEITEM hItem = GetSelectedItem();

    CWnd* parent = GetParent();

    if (parent)
    {
        parent->PostMessage(USR_MSG_NODE_SELECT, 0, (LPARAM)hItem);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::InitImageList()
{
    mImageList.Create(16, 16, ILC_COLOR24, 0, 4);
    CBitmap image;
    BITMAP  bitmap;

    image.LoadBitmap(IDB_UNCHECK);
    image.GetBitmap(&bitmap);

    //Set item height
    int height = MulDiv(bitmap.bmHeight, GetSystemDPI(), DEFAULT_DPI_96);
    SetItemHeight((SHORT)(height + 3));

    mImageList.Add(&image, RGB(255, 55, 255));
    mImageList.Add(&image, RGB(255, 55, 255));
    image.DeleteObject();

    image.LoadBitmap(IDB_CHECKBOX);
    mImageList.Add(&image, RGB(255, 55, 255));
    image.DeleteObject();

    image.LoadBitmap(IDB_PARTIAL_CHECK);
    mImageList.Add(&image, RGB(255, 55, 255));

    //Set image list for the tree view
    SetImageList(&mImageList, TVSIL_STATE);
}

/////////////////////////////////////////////////////////////////////////////////////
UINT CustomTreeCtrl::GetItemCheckState(HTREEITEM hItem)
{
    UINT state = CTreeCtrl::GetItemState(hItem, LVIS_STATEIMAGEMASK);

    state &= LVIS_STATEIMAGEMASK;

    switch (state)
    {
    case INDEXTOSTATEIMAGEMASK(2):
        return CUSTOM_TV_STATE_CHECKED;

    case INDEXTOSTATEIMAGEMASK(3):
        return CUSTOM_TV_STATE_PARTIAL_CHECKED;

    default:
        return CUSTOM_TV_STATE_UNCHECKED;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::SelectAllChildren(HTREEITEM hParent, BOOL bFlag)
{
    HTREEITEM hChild;

    hChild = GetChildItem(hParent);

    while (hChild)
    {
        SetItemState(hChild, CUSTOM_TV_STATE_CHECKED);

        SelectAllChildren(hChild, bFlag);

        hChild = GetNextSiblingItem(hChild);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
UINT CustomTreeCtrl::GetChildrenState(HTREEITEM hParent)
{
    HTREEITEM hChild;

    hChild = GetChildItem(hParent);

    int check_count = 0;
    int uncheck_count = 0;

    while (hChild)
    {
        UINT state = GetItemCheckState(hChild);

        switch (state)
        {
        case CUSTOM_TV_STATE_UNCHECKED:
            uncheck_count++;
            break;

        case CUSTOM_TV_STATE_CHECKED:
            check_count++;
            break;

        case CUSTOM_TV_STATE_PARTIAL_CHECKED:
            return CUSTOM_TV_STATE_PARTIAL_CHECKED;

        default:
            break;
        }

        hChild = GetNextSiblingItem(hChild);
    }

    if (check_count && uncheck_count)
    {
        return CUSTOM_TV_STATE_PARTIAL_CHECKED;
    }
    else if (check_count == 0)
    {
        return CUSTOM_TV_STATE_UNCHECKED;
    }
    else
    {
        return CUSTOM_TV_STATE_CHECKED;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::SetItemState(HTREEITEM hItem, int state)
{
    switch (state)
    {
    case CUSTOM_TV_STATE_UNCHECKED:
        state = INDEXTOSTATEIMAGEMASK(1);
        break;

    case CUSTOM_TV_STATE_CHECKED:
        state = INDEXTOSTATEIMAGEMASK(2);
        break;

    case CUSTOM_TV_STATE_PARTIAL_CHECKED:
        state = INDEXTOSTATEIMAGEMASK(3);
        break;

    default:
        break;
    }

    CTreeCtrl::SetItemState(hItem, state, TVIS_STATEIMAGEMASK);
}


/////////////////////////////////////////////////////////////////////////////////////
HTREEITEM CustomTreeCtrl::FindItem(HTREEITEM hStart, CString text)
{
    HTREEITEM hResult = NULL;

    while (hStart)
    {
        CString item_text = GetItemText(hStart);
        if (item_text.Compare(text) == 0)
        {
            hResult = hStart;
            break;
        }

        HTREEITEM hChild = GetChildItem(hStart);
        if (hChild)
        {
            hResult = FindItem(hChild, text);

            if (hResult)
            {
                break;
            }
        }

        hStart = GetNextSiblingItem(hStart);
    }

    return hResult;
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::UpdateParentState(HTREEITEM hChild)
{
    HTREEITEM hParent;

    hParent = GetParentItem(hChild);

    if (hParent)
    {
        UINT state = GetChildrenState(hParent);
        SetItemState(hParent, state);

        UpdateParentState(hParent);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
void CustomTreeCtrl::UpdateChildrenState(HTREEITEM hParent)
{
    HTREEITEM hChild;

    hChild = GetChildItem(hParent);

    UINT state = GetItemCheckState(hParent);

    while (hChild)
    {
        SetItemState(hChild, state);

        UpdateChildrenState(hChild);

        hChild = GetNextSiblingItem(hChild);
    }
}
