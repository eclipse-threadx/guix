#include "studiox_includes.h"
#include "trigger_list_edit_dlg.h"

extern CString target_class_name;
extern CFont NormalFont;

enum trigger_list_edit_dlg_test_commands{
    TEST_ADD_TRIGGER = 1,
    TEST_DELETE_TRIGGER,
    TEST_EDIT_TRIGGER,
    TEST_EDIT_ACTION,
    TEST_SELETE_ROW,
    TEST_SAVE_TRIGGER_LIST_EDIT,
    TEST_CANCEL_TRIGGER_LIST_EDIT
};

IMPLEMENT_DYNAMIC(trigger_list_edit_dlg, express_dialog)

BEGIN_MESSAGE_MAP(trigger_list_edit_dlg, express_dialog)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_TRIGGER_ADD_BTN, &trigger_list_edit_dlg::OnAddTrigger)
    ON_BN_CLICKED(IDC_TRIGGER_DEL_BTN, &trigger_list_edit_dlg::OnDeleteTrigger)
    ON_BN_CLICKED(IDC_TRIGGER_EDIT_BTN, &trigger_list_edit_dlg::OnEditTrigger)
    ON_BN_CLICKED(IDC_TRIGGER_ACTION_EDIT_BTN, &trigger_list_edit_dlg::OnEditTriggerAction)
    ON_MESSAGE(STUDIO_TEST, &trigger_list_edit_dlg::OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
trigger_list_edit_dlg::trigger_list_edit_dlg(int display_index, screen_flow *flow, flow_item *item, CWnd* pParent /*=NULL*/)
    : express_dialog(trigger_list_edit_dlg::IDD, pParent)
{
    SetTitleText("Edit Trigger List");
    IconId = IDB_MACRO_PLAYBACK_ICON;
    mpTriggerFrame = NULL;
    mpOldFlow = flow;
    mDisplayIndex = display_index;

    mItem = *item;

    trigger_info *trigger = mItem.trigger_list;
    while (trigger)
    {
        mTriggerList.Add(trigger);
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
trigger_list_edit_dlg::~trigger_list_edit_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnDestroy()
{
    express_dialog::OnDestroy();

    mpTriggerFrame->DestroyWindow();
    delete mpTriggerFrame;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnOK()
{
    flow_item *item = mpOldFlow->GetFlowItem(mItem.screen_name);

    if (item->trigger_list)
    {
        delete item->trigger_list;
    }
    item->trigger_list = mItem.trigger_list;
    mItem.trigger_list = NULL;

    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnCancel()
{
    mItem.RemovedAnimationIdFromDictionary(mDisplayIndex);

    flow_item *item = mpOldFlow->GetFlowItem(mItem.screen_name);
    if (item)
    {
        item->AddAnimationIdToDictionary(mDisplayIndex);
    }

    express_dialog::OnCancel();
}

int bad_wnd_pointers = 0;

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_list_edit_dlg::OnInitDialog()
{
    CString List_Prompt;
    CWnd *wnd;

    express_dialog::OnInitDialog();

    AddCancelButton();
    AddSaveButton();

    CRect rect;
    CRect list_prompt_rect;
    GetClientRect(&rect);
    rect.left += 10;
    rect.right -= 10;
    rect.bottom -= (m_status_bar_height + 10);

    wnd = GetDlgItem(IDC_TRIGGER_LIST_PROMPT);
    if (wnd)
    {
        wnd->GetWindowRect(&list_prompt_rect);
        ScreenToClient(&list_prompt_rect);
        rect.top = list_prompt_rect.bottom + 1;
    }
    else
    {
        bad_wnd_pointers++;
    }

    // create trigger frame
    mpTriggerFrame = new trigger_frame(this, &mTriggerList);
    mpTriggerFrame->Create(target_class_name, _T("trigger_frame"),
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_TABSTOP, rect, this, 0);
    mpTriggerFrame->SetWindowPos(GetDlgItem(IDC_TRIGGER_ACTION_EDIT_BTN), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    //set source screen name
    List_Prompt.Format(_T("List of defined triggers for \"%s\":"), mItem.screen_name);
    SetDlgItemText(IDC_TRIGGER_LIST_PROMPT, List_Prompt);

    EnableDisableTriggerEditControls();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnAddTrigger()
{
    trigger_info *trigger = new trigger_info;
    trigger->event_type = 0;
    trigger_edit_dlg dlg(mDisplayIndex, &mItem, trigger, this);

    if ((dlg.DoModal() == IDOK) && trigger->event_type)
    {
        mTriggerList.Add(trigger);

        mpTriggerFrame->CreateTriggerRows();
        mpTriggerFrame->PositionChildren();

        //select new added trigger
        mpTriggerFrame->SetSelectedRow(mTriggerList.GetCount() - 1);

        EnableDisableTriggerEditControls();
    }
    else
    {
        delete trigger;
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnDeleteTrigger()
{
    int index = mpTriggerFrame->GetSelectedRow();

    if (index < mTriggerList.GetCount())
    {
        trigger_info *trigger = mTriggerList.GetAt(index);
        trigger->RemoveAnimationIdFromDictionary(mDisplayIndex);
        screen_flow::RemoveTrigger(&mItem, trigger);
        mTriggerList.RemoveAt(index);

        mpTriggerFrame->CreateTriggerRows();
        mpTriggerFrame->PositionChildren();

        if (index > 0)
        {
            mpTriggerFrame->SetSelectedRow(index - 1);
        }
    }

    EnableDisableTriggerEditControls();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnEditTrigger()
{
    int index = mpTriggerFrame->GetSelectedRow();

    if (mTriggerList.GetCount())
    {
        trigger_info *trigger = mTriggerList.GetAt(index);
        trigger_edit_dlg dlg(mDisplayIndex, &mItem, trigger, this);

        if (dlg.DoModal() == IDOK)
        {
            if (trigger->event_type == 0)
            {
                trigger = mTriggerList.GetAt(index);
                screen_flow::RemoveTrigger(&mItem, trigger);
                mTriggerList.RemoveAt(index);
            }

            mpTriggerFrame->CreateTriggerRows();
            mpTriggerFrame->PositionChildren();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::OnEditTriggerAction()
{
    int index = mpTriggerFrame->GetSelectedRow();

    if (mTriggerList.GetCount())
    {
        trigger_info *trigger = mTriggerList.GetAt(index);
        trigger_action_edit_dlg dlg(mDisplayIndex, &mItem, trigger, this);

        if (dlg.DoModal() == IDOK)
        {
            mpTriggerFrame->CreateTriggerRows();
            mpTriggerFrame->PositionChildren();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_list_edit_dlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
    {
        CWnd* focus_owner = GetFocus();

        if (focus_owner)
        {
            switch (focus_owner->GetDlgCtrlID())
            {
            case IDC_TRIGGER_ADD_BTN:
                OnAddTrigger();
                return TRUE;

            case IDC_TRIGGER_DEL_BTN:
                OnDeleteTrigger();
                return TRUE;

            case IDC_TRIGGER_EDIT_BTN:
                OnEditTrigger();
                return TRUE;

            case IDC_TRIGGER_ACTION_EDIT_BTN:
                OnEditTriggerAction();
                return TRUE;

            default:
                break;
            }
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////
void trigger_list_edit_dlg::EnableDisableTriggerEditControls()
{
    BOOL enable = TRUE;
    if (mTriggerList.GetCount() == 0)
    {
        enable = FALSE;

        if (GetFocus() == GetDlgItem(IDC_TRIGGER_DEL_BTN))
        {
            // Move focus to trigger add button.
            GetDlgItem(IDC_TRIGGER_ADD_BTN)->SetFocus();
        }
    }

    GetDlgItem(IDC_TRIGGER_DEL_BTN)->EnableWindow(enable);
    GetDlgItem(IDC_TRIGGER_EDIT_BTN)->EnableWindow(enable);
    GetDlgItem(IDC_TRIGGER_ACTION_EDIT_BTN)->EnableWindow(enable);
    mpTriggerFrame->EnableWindow(enable);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT trigger_list_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_ADD_TRIGGER:
        OnAddTrigger();
        break;

    case TEST_DELETE_TRIGGER:
        OnDeleteTrigger();
        break;

    case TEST_EDIT_TRIGGER:
        OnEditTrigger();
        break;

    case TEST_EDIT_ACTION:
        OnEditTriggerAction();
        break;

    case TEST_SELETE_ROW:
        mpTriggerFrame->SetSelectedRow(lParam);
        break;

    case TEST_SAVE_TRIGGER_LIST_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_TRIGGER_LIST_EDIT:
        OnCancel();
        break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(trigger_frame, CWnd)
    ON_WM_DESTROY()
    ON_WM_CREATE()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
trigger_frame::trigger_frame(CWnd *parent, CArray<trigger_info *> *trigger_list)
{
    mpTriggerList = trigger_list;
    mSelectedRow = 0;
    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);

    mRowHeight = GetScaledValue(TRIGGER_ROW_HEIGHT, GetSystemDPI(), GetTextScaler());
}

trigger_frame::~trigger_frame()
{
    if (m_scroll_helper)
    {
        delete m_scroll_helper;
    }
}

///////////////////////////////////////////////////////////////////////////////
int trigger_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CreateTriggerRows();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::OnDestroy()
{
    CWnd::OnDestroy();

    CWnd *child = GetWindow(GW_CHILD);

    while (child)
    {
        child->DestroyWindow();
        delete child;
        child = GetWindow(GW_CHILD);
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    PositionChildren();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Compute the desired change or delta in scroll position.
    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_frame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    BOOL wasScrolled = m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
    return wasScrolled;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    CBrush fill_brush(GetSysColor(COLOR_3DFACE));
    GetClientRect(&rect);

    dc.FillRect(rect, &fill_brush);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    // Move focus to the selected row
    AssignFocus(mSelectedRow);
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_frame::PreTranslateMessage(MSG* pMsg)
{
    int pagesize;
    CRect rect;

    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_UP:
            if (mSelectedRow > 0)
            {
                SetSelectedRow(mSelectedRow - 1, TRUE);
            }
            return TRUE;

        case VK_DOWN:
            if (mSelectedRow < mpTriggerList->GetCount() - 1)
            {
                SetSelectedRow(mSelectedRow + 1, TRUE);
            }
            return TRUE;

        case VK_HOME:
            if (mSelectedRow > 0)
            {
                SetSelectedRow(0, TRUE);
            }
            break;

        case VK_END:
            if (mSelectedRow < mpTriggerList->GetCount() - 1)
            {
                SetSelectedRow(mpTriggerList->GetCount() - 1, TRUE);
            }
            break;

        case VK_PRIOR:
            GetClientRect(&rect);
            if (m_scroll_helper->Scroll(SB_VERT, -rect.Height()))
            {
                pagesize = rect.Height() / mRowHeight;
                SetSelectedRow(max(0, mSelectedRow - pagesize), TRUE);
            }
            break;

        case VK_NEXT:
            GetClientRect(&rect);
            if (m_scroll_helper->Scroll(SB_VERT, rect.Height()))
            {
                pagesize = rect.Height() / mRowHeight;
                SetSelectedRow(min(mpTriggerList->GetCount() - 1, mSelectedRow + pagesize), TRUE);
            }
            break;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::AssignFocus(int rowId)
{
    CWnd* child = GetWindow(GW_CHILD);

    while (child && rowId)
    {
        child = child->GetWindow(GW_HWNDNEXT);
        rowId--;
    }

    // move focus to selected row
    if (child)
    {
        child->SetFocus();
        child->Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::SetSelectedRow(int rowId, bool assign_focus)
{
    if (mSelectedRow != rowId)
    {
        mSelectedRow = rowId;
        SelectedVisible();
        Invalidate();

        if (assign_focus)
        {
            AssignFocus(rowId);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::SelectedVisible()
{
    if (mSelectedRow >= 0)
    {
        int min_scroll_value = ((mSelectedRow + 1) * mRowHeight) - m_scroll_helper->GetPageSize().cy;
        int max_scroll_value = min_scroll_value + m_scroll_helper->GetPageSize().cy - mRowHeight;

        if (min_scroll_value < 0)
        {
            min_scroll_value = 0;
        }

        if (max_scroll_value > m_scroll_helper->GetDisplaySize().cy - m_scroll_helper->GetPageSize().cy)
        {
            max_scroll_value = m_scroll_helper->GetDisplaySize().cy - m_scroll_helper->GetPageSize().cy;
        }

        int current_scroll_value = m_scroll_helper->GetScrollPos().cy;
        if ((current_scroll_value < min_scroll_value) || (current_scroll_value > max_scroll_value))
        {
            // scroll so that selected item is visible
            m_scroll_helper->SetScrollPos(SB_VERT, min_scroll_value);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::CreateTriggerRows()
{
    int index;
    trigger_row *row;
    CWnd *child = GetWindow(GW_CHILD);

    while (child)
    {
        child->DestroyWindow();
        delete child;
        child = GetWindow(GW_CHILD);
    }

    int rowcount = mpTriggerList->GetCount();
    trigger_info* trigger;
    CString row_name("");
    for (index = 0; index < rowcount; index++)
    {
        row = new trigger_row(this, index, mpTriggerList);
        trigger = mpTriggerList->GetAt(index);
        if (trigger)
        {
            row_name = trigger->trigger_name;
        }
        row->Create(target_class_name, row_name, WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 0);
        SetControlAccessibleRole(row->GetSafeHwnd(), ROLE_SYSTEM_LISTITEM);

    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_frame::PositionChildren()
{
    int rowheight;
    CRect client;
    GetClientRect(&client);
    CRect childsize = client;
    trigger_row *row = (trigger_row *)GetWindow(GW_CHILD);

    if (row)
    {
        rowheight = mRowHeight;

        childsize.top -= (m_scroll_helper->GetScrollPos()).cy;
        childsize.bottom = childsize.top + rowheight;

        while (row)
        {
            row->MoveWindow(&childsize);
            childsize.top += rowheight;
            childsize.bottom += rowheight;
            row = (trigger_row *)row->GetWindow(GW_HWNDNEXT);
        }
    }

    m_scroll_helper->SetDisplaySize(0, mpTriggerList->GetCount() * mRowHeight);

    Invalidate();
}


///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(trigger_row, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
trigger_row::trigger_row(CWnd *parent, int Id, CArray<trigger_info *> *trigger_list)
{
    mRowId = Id;
    frame = (trigger_frame *)parent;
    mpTriggerList = trigger_list;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_row::OnPaint()
{
    CPaintDC dc(this);
    CFont *old_font;
    CRect boxrect;
    CBrush brush;

    GetClientRect(&boxrect);

    dc.SetBkMode(TRANSPARENT);
    old_font = dc.SelectObject(&NormalFont);
    if (mRowId == frame->GetSelectedRow())
    {

        //highlight selected item
        if (GetFocus() == this)
        {
            brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
            dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
        }
        else
        {
            brush.CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
            dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
        }

        dc.FillRect(&boxrect, &brush);
    }
    else
    {
        brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
        dc.FillRect(&boxrect, &brush);
    }

    CPen line_pen;
    line_pen.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));
    CPen* old_pen = dc.SelectObject(&line_pen);

    // Draw control border
    if (mRowId == 0)
    {
        dc.MoveTo(boxrect.left, boxrect.top);
        dc.LineTo(boxrect.right, boxrect.top);
    }

    dc.MoveTo(boxrect.left, boxrect.top);
    dc.LineTo(boxrect.left, boxrect.bottom - 1);
    dc.MoveTo(boxrect.right - 1, boxrect.top);
    dc.LineTo(boxrect.right - 1, boxrect.bottom - 1);
    dc.MoveTo(boxrect.left, boxrect.bottom - 1);
    dc.LineTo(boxrect.right, boxrect.bottom - 1);
    dc.SelectObject(old_pen);

    trigger_info* trigger = mpTriggerList->GetAt(mRowId);
    if (trigger)
    {
        boxrect.left += 4;
        dc.DrawText(trigger->trigger_name, boxrect, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_VCENTER);
    }

    dc.SelectObject(old_font);
}


///////////////////////////////////////////////////////////////////////////////
void trigger_row::OnLButtonDown(UINT nFlags, CPoint point)
{
    frame->SetSelectedRow(mRowId);
    frame->SetFocus();
    CWnd::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_row::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    Invalidate();
}
