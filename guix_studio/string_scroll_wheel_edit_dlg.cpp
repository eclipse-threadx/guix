#include "studiox_includes.h"
#include "express_table_row.h"
#include "string_scroll_wheel_edit_dlg.h"

extern CFont NormalFont;

enum string_scroll_wheel_edit_dlg_test_commands{
    TEST_SET_STRING = 1,
    TEST_SAVE,
    TEST_CANCEL
};

enum CONTROL_IDS{
    ID_STRING_INDEX = 4096,
    ID_STRING_VAL,
    ID_STRING_ID
};

extern CString target_class_name;
// string_scroll_wheel_edit_dlg dialog

IMPLEMENT_DYNAMIC(string_scroll_wheel_edit_dlg, express_dialog)

BEGIN_MESSAGE_MAP(string_scroll_wheel_edit_dlg, express_dialog)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
string_scroll_wheel_edit_dlg::string_scroll_wheel_edit_dlg(int display_index, CArray<GX_RESOURCE_ID> *value_list, CWnd* pParent /*=NULL*/)
    : express_dialog(string_scroll_wheel_edit_dlg::IDD, pParent)
{
    IconId = IDB_PROPS_VIEW;
    SetTitleText("String Scroll Wheel Edit");

    mpStringIdList = value_list;
    mOldStringIdList.Copy(*value_list);
    mDisplayIndex = display_index;

    studiox_project *project = GetOpenProject();

    if (project)
    {
        mpProject = project;

        if (project->mDisplays[mDisplayIndex].stable)
        {
            mpTable = new string_table(*project->mDisplays[mDisplayIndex].stable);
        }
        else
        {
            mpTable = new string_table;
        }
    }
    else
    {
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////
string_scroll_wheel_edit_dlg::~string_scroll_wheel_edit_dlg()
{
    if (mpTable)
    {
        delete mpTable;
    }

    if (mpValueListFrame)
    {
        delete mpValueListFrame;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_edit_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);

    if (pDX->m_bSaveAndValidate)
    {
        if (mpProject->mDisplays[mDisplayIndex].stable)
        {
            delete mpProject->mDisplays[mDisplayIndex].stable;
            mpTable->Sort();
            mpProject->mDisplays[mDisplayIndex].stable = mpTable;
            mpTable = NULL;

            if (GetResourceView())
            {
                GetResourceView()->RebuildStringItems();
            }
        }
    }
    else
    {
        mpStringIdList->Copy(mOldStringIdList);
    }
}

BOOL string_scroll_wheel_edit_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    CRect frame_size;
    CRect size;

    GetDlgItem(IDC_INDEX_LABEL)->GetWindowRect(&size);
    ScreenToClient(&size);

    GetClientRect(&frame_size);

    frame_size.left += 5;
    frame_size.top = size.bottom + 5;
    frame_size.right -= 5;
    frame_size.bottom -= m_status_bar_height + 10;

    // Create value list frame
    mpValueListFrame = new value_list_frame(mpStringIdList, mpTable);
    mpValueListFrame->Create(target_class_name, _T("ValueListFrame"),
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | WS_TABSTOP,
        frame_size, this, 0);

    AddCancelButton();
    AddSaveButton();

    return TRUE;
}

void string_scroll_wheel_edit_dlg::TestSetRowData(int row_id, CString string)
{
    CWnd* child = mpValueListFrame->GetWindow(GW_CHILD);
    value_list_row* row;
    

    while (child)
    {
        row = (value_list_row*)child;

        if (row->GetRowId() == row_id)
        {
            CRect list_client;
            CRect rect;

            mpValueListFrame->GetClientRect(&list_client);
            mpValueListFrame->ClientToScreen(&list_client);

            row->GetWindowRect(&rect);

            if (rect.top < list_client.top)
            {
                mpValueListFrame->Scroll(rect.top - list_client.top);
            }
            else if (rect.bottom > list_client.bottom)
            {
                mpValueListFrame->Scroll(rect.bottom - list_client.bottom);
            }

            row->SetData(_T(""), string);
            row->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRING_VAL, EN_KILLFOCUS),
                (LPARAM)(row->GetTextEdit()->m_hWnd));
        }
        child = child->GetWindow(GW_HWNDNEXT);
    }
}

//////////////////////////////////////////////////////////////////////////////
LRESULT string_scroll_wheel_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CStringArray param;
    INT id;
    CString string;

    switch (wParam)
    {
    case TEST_SET_STRING:
        SplitString(GetTestingParam(0), ',', &param);
        id = _tstoi(param.GetAt(0));
        string = param.GetAt(1);

        TestSetRowData(id, string);
        break;

    case TEST_SAVE:
        OnOK();
        break;

    case TEST_CANCEL:
        OnCancel();
        break;
    }

    return 0;
}

BEGIN_MESSAGE_MAP(value_list_row, express_table_row)
    ON_WM_CREATE()
    ON_EN_KILLFOCUS(ID_STRING_VAL, OnChangeStringText)
    ON_CBN_SELCHANGE(ID_STRING_ID, OnChangeStringId)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
value_list_row::value_list_row(int row_id)
{
    mRowId = row_id;
}

///////////////////////////////////////////////////////////////////////////////
int value_list_row::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (express_table_row::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect client;
    CRect size;

    // TODO:  Add your specialized creation code here
    GetClientRect(&client);

    CFont* font = &NormalFont;

    size = client;
    size.top += 5;
    size.bottom -= 5;
    size.left += 3;
    size.right = client.Width() / 3 - 35;
    CString text;
    text.Format(_T("%d"), mRowId);
    mIndexPrompt.Create(text, WS_CHILD | SS_CENTER | WS_VISIBLE, size, this, ID_STRING_INDEX);
    mIndexPrompt.SetFont(font);

    size.left = size.right + 15;
    size.right = client.right - 5;

    int width = size.right - size.left + 1;
    size.right = size.left + width / 2 - 5;
    mStringIdCombo.Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_SORT | WS_VISIBLE | WS_VSCROLL, size, this, ID_STRING_ID);
    mStringIdCombo.SetFont(font);
    SetControlAccessibleName(mStringIdCombo.GetSafeHwnd(), _T("Index ") + text + _T(" String ID"));

    size.left = size.right + 5;
    size.right = client.right - 5;

    mStringTextEdit.Create(WS_CHILD | SS_RIGHT | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, size, this, ID_STRING_VAL);
    mStringTextEdit.SetFont(font);
    SetControlAccessibleName(mStringTextEdit.GetSafeHwnd(), _T("Index ") + text + _T(" String Value"));

    value_list_frame *frame = (value_list_frame*)GetParent();
    if (frame)
    {
        string_table *table = frame->GetStringTable();
        if (table)
        {
            int string_id = 1;
            int box_index;

            while (string_id < table->CountStrings())
            {
                box_index = mStringIdCombo.AddString(table->GetResourceIdName(string_id));
                mStringIdCombo.SetItemData(box_index, string_id);
                string_id++;
            }

            mStringIdCombo.InsertString(0, _T("None"));
            mStringIdCombo.SetItemData(0, 0);
            mStringIdCombo.SetCurSel(0);
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
BOOL value_list_row::OnEraseBkgnd(CDC* pDC)
{
    CBrush newBrush(RGB(210, 210, 210));
    CBrush *pOldBrush;

    pOldBrush = pDC->SelectObject(&newBrush);

    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed

    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
        PATCOPY);
    pDC->SelectObject(pOldBrush);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL value_list_row::PreTranslateKeyDown(WPARAM keyvalue)
{
    CWnd* focus_owner = GetFocus();
    CWnd* next_focus_owner = NULL;
    value_list_frame* list = (value_list_frame*)GetParent();
    CRect list_client;

    list->GetClientRect(&list_client);
    list->ClientToScreen(&list_client);

    switch (keyvalue)
    {
    case VK_LEFT:
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
    case VK_HOME:
    case VK_END:
        next_focus_owner = GetNextTabStopChild(focus_owner, keyvalue);
        break;

    case VK_PRIOR:
        if (list->Scroll(-list_client.Height()))
        {
            next_focus_owner = GetNextTabStopChild(focus_owner, keyvalue, list_client.Height() / list->GetRowHeight());
        }
        break;

    case VK_NEXT:
        if (list->Scroll(list_client.Height()))
        {
            next_focus_owner = GetNextTabStopChild(focus_owner, keyvalue, list_client.Height() / list->GetRowHeight());
        }
        break;

    default:
        return FALSE;
    }

    if (next_focus_owner)
    {
        CRect rect;

        next_focus_owner->GetWindowRect(&rect);

        if (rect.top < list_client.top)
        {
            list->Scroll(rect.top - list_client.top);
        }
        else if (rect.bottom > list_client.bottom)
        {
            list->Scroll(rect.bottom - list_client.bottom);
        }

        AssignFocus(next_focus_owner);
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL value_list_row::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_TAB)
        {
            //handle tab key
            value_list_frame* frame = (value_list_frame*)GetParent();
            string_scroll_wheel_edit_dlg* dlg = (string_scroll_wheel_edit_dlg*)frame->GetParent();
            CWnd* child = dlg->GetNextDlgTabItem(frame, FALSE);
            if (child)
            {
                child->SetFocus();
                return TRUE;
            }
        }

        CWnd* child = GetFocus();
        CComboBox* box = NULL;
        TCHAR  class_name[MAX_PATH];
        GetClassName(child->GetSafeHwnd(), class_name, MAX_PATH - 1);
        if (class_name[0] == 'C')
        {
            //"ComboBox"
            box = (CComboBox*)child;
        }

        if (!box || !box->GetDroppedState())
        {
            if (PreTranslateKeyDown(pMsg->wParam))
            {
                return TRUE;
            }
        }
    }

    return express_table_row::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void value_list_row::OnChangeStringId()
{
    value_list_frame *frame = (value_list_frame*)GetParent();
    string_table *table = frame->GetStringTable();

    int res_id = mStringIdCombo.GetItemData(mStringIdCombo.GetCurSel());
    CString id_name = table->GetResourceIdName(res_id);

    if (mStringId != id_name)
    {
        mStringId = id_name;

        SetUtf8Text(&mStringTextEdit, table->GetString(id_name));
    }

    frame->NotifyStringIdChanged(mRowId, res_id);
}

///////////////////////////////////////////////////////////////////////////////
void value_list_row::OnChangeStringText()
{
    value_list_frame *frame = (value_list_frame*)GetParent();
    string_table *table = frame->GetStringTable();
    CString string;
    
    mStringTextEdit.GetWindowText(string);

    int   res_id = mStringIdCombo.GetItemData(mStringIdCombo.GetCurSel());
    CString id_name;

    if (res_id)
    {
        id_name = table->GetResourceIdName(res_id);

        if (table->GetString(id_name) == string)
        {
            // no change, just return
            return;
        }
    }

    if (string.IsEmpty())
    {
        id_name = "";
        res_id = 0;
        mStringIdCombo.SetCurSel(0);
    }
    else
    {
        int string_index = table->CheckAddString(string);
        id_name = table->GetStringId(string_index);
        res_id = table->GetResourceId(id_name);

        if (mStringIdCombo.GetCount() == res_id)
        {
            frame->NotifyStringIdAdded(id_name, res_id);
            mStringIdCombo.SelectString(0, id_name);
        }
    }
    frame->NotifyStringIdChanged(mRowId, res_id);
}

///////////////////////////////////////////////////////////////////////////////
void value_list_row::SetData(CString id, CString text)
{
    mStringId = id;
    mStringText = text;

    if (id.IsEmpty())
    {
        mStringIdCombo.SetCurSel(0);
    }
    else
    {
        mStringIdCombo.SelectString(0, id);
    }
    SetDlgItemText(ID_STRING_VAL, text);

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void value_list_row::InsertStringId(CString id_name, GX_RESOURCE_ID res_id)
{
    int index;
    index = mStringIdCombo.AddString(id_name);
    mStringIdCombo.SetItemData(index, res_id);
}


BEGIN_MESSAGE_MAP(value_list_frame, CWnd)
ON_WM_CREATE()
ON_WM_VSCROLL()
ON_WM_MOUSEWHEEL()
ON_WM_DESTROY()
ON_WM_SETFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
value_list_frame::value_list_frame(CArray<GX_RESOURCE_ID>* string_id_list, string_table *stable)
{
    mpStringIdList = string_id_list;
    mpTable = stable;
    mRowHeight = 0;

    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);
}

///////////////////////////////////////////////////////////////////////////////
value_list_frame::~value_list_frame()
{

    if (m_scroll_helper)
    {
        delete m_scroll_helper;
    }
}

void value_list_frame::OnDestroy()
{
    CWnd::OnDestroy();

    // TODO: Add your message handler code here
    CWnd* child = GetWindow(GW_CHILD);

    while (child)
    {
        child->DestroyWindow();
        delete child;
        child = GetWindow(GW_CHILD);
    }
}


///////////////////////////////////////////////////////////////////////////////
int value_list_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    CRect client;

    GetClientRect(&client);
    mRowHeight = (client.Height() + VISIBLE_VALUE_ROWS - 1) / VISIBLE_VALUE_ROWS;

    client.bottom = client.top + mRowHeight;

    value_list_row *row;
    int row_count = mpStringIdList->GetCount();
    int res_id;
    CString id_name;
    CString text;

    //add row window
    for (int index = 0; index < row_count; index++)
    {
        row = new value_list_row(index);
        row->Create(target_class_name, _T("ValueListRow"), WS_CHILD | WS_VISIBLE, client, this, 0, NULL);

        res_id = mpStringIdList->GetAt(index);
        id_name = mpTable->GetResourceIdName(res_id);
        text = mpTable->GetString(id_name);

        row->SetData(id_name, text);

        client.OffsetRect(0, mRowHeight);
    }

    m_scroll_helper->SetDisplaySize(0, row_count * mRowHeight);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void value_list_frame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* bar)
{
    // Compute the desired change or delta in scroll position.
    m_scroll_helper->OnVScroll(nSBCode, nPos, bar);
}

///////////////////////////////////////////////////////////////////////////////
BOOL value_list_frame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    return m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
}

///////////////////////////////////////////////////////////////////////////////
void value_list_frame::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    //pick first row
    CWnd* child = GetWindow(GW_CHILD);
    CRect list_client;
    CRect rect;

    GetClientRect(&list_client);
    ClientToScreen(&list_client);

    // set focus to the first visible child
    while (child)
    {
        child->GetWindowRect(&rect);

        if (rect.top < list_client.top)
        {
            child = child->GetWindow(GW_HWNDNEXT);
        }
        else
        {
            //pick first child of the row
            child = child->GetWindow(GW_CHILD);

            while (child)
            {
                if (child->GetStyle() & WS_TABSTOP)
                {
                    child->SetFocus();
                    break;
                }

                child = child->GetWindow(GW_HWNDNEXT);
            }

            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void value_list_frame::NotifyStringIdAdded(CString id_name, GX_RESOURCE_ID res_id)
{
    CWnd* child = GetWindow(GW_CHILD);

    while (child)
    {
        ((value_list_row*)child)->InsertStringId(id_name, res_id);
        child = child->GetWindow(GW_HWNDNEXT);
    }
}

///////////////////////////////////////////////////////////////////////////////
void value_list_frame::NotifyStringIdChanged(int index, GX_RESOURCE_ID id)
{
    if (index < mpStringIdList->GetCount())
    {
        mpStringIdList->SetAt(index, id);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL value_list_frame::Scroll(int delta)
{
    CSize pos = m_scroll_helper->GetScrollPos();
    int new_cy;

    new_cy = pos.cy + delta;

    if (delta > 0)
    {
        CSize display_size = m_scroll_helper->GetDisplaySize();
        CSize page_size = m_scroll_helper->GetPageSize();

        int maxScrollPos = display_size.cy - page_size.cy;

        if (new_cy > maxScrollPos)
        {
            new_cy = maxScrollPos;
        }
    }
    else
    {
        if (new_cy < 0)
        {
            new_cy = 0;
        }
    }

    if (new_cy != pos.cy)
    {
        m_scroll_helper->SetScrollPos(SB_VERT, new_cy, TRUE);
        return TRUE;
    }
    
    return FALSE;
}
