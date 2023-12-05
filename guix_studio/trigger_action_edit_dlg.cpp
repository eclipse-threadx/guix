#include "studiox_includes.h"
#include "trigger_action_edit_dlg.h"

// trigger_action_edit_dlg dialog
enum trigger_action_edit_dlg_test_commands {
    TEST_ADD_ACTION = 1,
    TEST_SELECT_ACTION,
    TEST_DELETE_ACTION,
    TEST_SELECT_TARGET,
    TEST_SELECT_PARENT,
    TEST_TARGET_SHOW_CHILD_WIDGETS,
    TEST_PARENT_SHOW_CHILD_WIDGETS,
    TEST_EDIT_ACTION_NAME,
    TEST_EDIT_ANIMATION_INFO,
    TEST_CHECK_DETACH_TARGET,
    TEST_CONFIGURE_EASING_FUNCTION,
    TEST_SAVE_TRIGGER_ACTION_EDIT,
    TEST_CANCEL_TRIGGER_ACTION_EDIT
};

// Define animation parameter names
STRING_VAL_PAIR AnimationParamNames[] = {
    { _T("Animation ID"), ID_ANIMATION_ID },
    { _T("Start X"), ID_ANIMATION_START_X },
    { _T("Start Y"), ID_ANIMATION_START_Y },
    { _T("End X"), ID_ANIMATION_END_X },
    { _T("End Y"), ID_ANIMATION_END_Y },
    { _T("Steps"), ID_ANIMATION_STEPS },
    { _T("Tick Rate"), ID_ANIMATION_TICK_RATE },
    { _T("Delay Before"), ID_ANIMATION_DELAY_BEFORE },
    { _T("Start Alpha"), ID_ANIMATION_START_ALPHA },
    { _T("End Alpha"),ID_ANIMATION_END_ALPHA },
    { _T(""), 0 }
};

CString SCREEN_STACK_POP_STRING(_T("gx_system_screen_stack_pop"));

extern CFont NormalFont;
extern CString target_class_name;


IMPLEMENT_DYNAMIC(trigger_action_edit_dlg, express_dialog)

BEGIN_MESSAGE_MAP(trigger_action_edit_dlg, express_dialog)
    ON_BN_CLICKED(IDC_ADD_ACTION, &trigger_action_edit_dlg::OnAddAction)
    ON_BN_CLICKED(IDC_DELETE_ACTION, &trigger_action_edit_dlg::OnDeleteAction)
    ON_LBN_SELCHANGE(IDC_ADDED_ACTION_LIST, &trigger_action_edit_dlg::OnSelectAddedActionList)
    ON_CBN_SELCHANGE(IDC_TARGET_COMBO, &trigger_action_edit_dlg::OnSelectTargetCombo)
    ON_CBN_SELCHANGE(IDC_PARENT_COMBO, &trigger_action_edit_dlg::OnSelectParentCombo)
    ON_EN_KILLFOCUS(IDC_ACTION_NAME, &trigger_action_edit_dlg::OnChangeActionName)
    ON_BN_CLICKED(ID_EASING_FUNCTION_SELECT, OnClickEasingFunction)
    ON_BN_CLICKED(IDC_TARGET_SHOW_CHILD_WIDGETS, &trigger_action_edit_dlg::OnClickTargetShowChildWidgets)
    ON_BN_CLICKED(IDC_PARENT_SHOW_CHILD_WIDGETS, &trigger_action_edit_dlg::OnClickParentShowChildWidgets)
    ON_MESSAGE(STUDIO_TEST, &trigger_action_edit_dlg::OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
trigger_action_edit_dlg::trigger_action_edit_dlg(int display_index, flow_item *item, trigger_info *trigger, CWnd* pParent /*=NULL*/)
: express_dialog(trigger_action_edit_dlg::IDD, pParent)
{
    CString title;

    IconId = IDB_MACRO_PLAYBACK_ICON;
    if (trigger->event_type)
    {
        title.Format(_T("Edit Actions for Trigger [%s]"), trigger->trigger_name);
    }
    else
    {
        title.Format(_T("Add Actions for Trigger [%s]"), trigger->trigger_name);
    }
    SetTitleText(title);

    mDisplayIndex = display_index;
    mpTrigger = trigger;

    action_info *action;
    mActionList.RemoveAll();
    for (int index = 0; index < trigger->action_list.GetCount(); index++)
    {
        action = new action_info(*trigger->action_list.GetAt(index));

        mActionList.Add(action);
    }

    studiox_project *project = GetOpenProject();
    widget_info *info = NULL;
    if (project)
    {
        info = project->FindWidgetInfo(project->mDisplays[display_index].GetFirstChildFolder(), item->screen_name, FALSE);
    }

    mpInfo = info;
    mAddedActionListSel = -1;
    mAnimationEasingFuncId = 0;
}

///////////////////////////////////////////////////////////////////////////////
trigger_action_edit_dlg::~trigger_action_edit_dlg()
{
    trigger_info::CleanActionList(mActionList);
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_action_edit_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CRect rect;
    CRect size;
    int item_height;
    int item_width;
    int index;

    // Create animation parameter controls.
    mAnimationParams.GetWindowRect(&rect);
    ScreenToClient(&rect);

    // get animation parameter group box text size
    CString text;
    mAnimationParams.GetWindowTextW(text);
    CDC* pDC = mAnimationParams.GetDC();
    CFont* pFont = mAnimationParams.GetFont();
    CFont* pOldFont = pDC->SelectObject(pFont);
    CSize sz = pDC->GetTextExtent(text);
    pDC->SelectObject(pOldFont);

    item_height = (rect.Height() - 30) / (ANIMATION_PARAMS_COUNT + 1) - 3;
    item_width = (rect.Width() - 20) / 2 - 10;

    size.top = rect.top + sz.cy + 10;

    CFont *font = mAnimationParams.GetFont();

    for (index = 0; index < ANIMATION_PARAMS_COUNT; index++)
    {
        size.left = rect.left + 5;

        size.right = size.left + item_width * 2 / 3;
        size.bottom = size.top + item_height;
        mAnimationParamLabels[index].Create(AnimationParamNames[index].name, WS_CHILD | WS_VISIBLE, size, this);
        mAnimationParamLabels[index].SetFont(font);
        size.left = size.right + 5;
        size.right = rect.right - 5;
        mAnimationParamEdits[index].Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_TABSTOP | ES_AUTOHSCROLL,
            size, this, AnimationParamNames[index].val);
        mAnimationParamEdits[index].SetFont(font);

        switch (AnimationParamNames[index].val)
        {
        case ID_ANIMATION_START_X:
        case ID_ANIMATION_START_Y:
        case ID_ANIMATION_END_X:
        case ID_ANIMATION_END_Y:
            mAnimationParamEdits[index].EnableNegativeValue();
            mAnimationParamEdits[index].EnableNumberCheck();
            break;

        case ID_ANIMATION_STEPS:
        case ID_ANIMATION_TICK_RATE:
        case ID_ANIMATION_DELAY_BEFORE:
        case ID_ANIMATION_START_ALPHA:
        case ID_ANIMATION_END_ALPHA:
            mAnimationParamEdits[index].EnableNumberCheck();
            break;
        }

        size.top = size.bottom + 3;
    }

    size.left = rect.left + 5;
    size.top = size.bottom + 8;
    size.bottom = rect.bottom - 5;
    size.right = size.left + item_width;
    mEasingFunctionEdit.Create(WS_CHILD | SS_LEFT | WS_VISIBLE | WS_BORDER | ES_READONLY, size, this, 0);
    mEasingFunctionEdit.SetFont(font);
    SetEasingFunction();

    size.left = size.right + 5;
    size.right = rect.right - 5;
    mEasingFunctionButton.Create(_T("Easing Func Select"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, size, this, ID_EASING_FUNCTION_SELECT);
    mEasingFunctionButton.SetFont(font);

    mOnAnimationCompleteGroup.GetWindowRect(&rect);
    ScreenToClient(&rect);

    size.left = rect.left + 5;
    size.top = rect.top + sz.cy + 10;
    size.bottom = size.top + item_height;
    size.right = size.left + item_width * 2;
    mDetachTargetBox.Create(_T("Detach Target"), WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX, size, this, IDC_DETACH_TARGET);
    mDetachTargetBox.SetFont(font);

    size.top += item_height + 3;
    size.bottom += item_height + 3;
    mPushTargetBox.Create(_T("Push Target to Screen Stack"), WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX, size, this, IDC_TARGET_TO_SCREEN_STACK);
    mPushTargetBox.SetFont(font);

    InitActionLists();
    ShowHideAnimationParams();

    SetControlAccessibleName(mTargetShowChildWidgets.GetSafeHwnd(), _T("show target's child widget"));
    SetControlAccessibleName(mParentShowChildWidgets.GetSafeHwnd(), _T("show parent's child widget"));
    SetControlAccessibleName(mEasingFunctionEdit.GetSafeHwnd(), _T("easing function name"));

    AddCancelButton();
    AddSaveButton();

    mStatusMsg.Create(L"", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    SetLiveRegion(mStatusMsg.GetSafeHwnd());

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ANIMATION_PARAMS, mAnimationParams);
    DDX_Control(pDX, IDC_ADDED_ACTION_LIST, mAddedActionList);
    DDX_Control(pDX, IDC_TARGET_COMBO, mTargetCombo);
    DDX_Control(pDX, IDC_PARENT_COMBO, mParentCombo);
    DDX_Control(pDX, IDC_TARGET_SHOW_CHILD_WIDGETS, mTargetShowChildWidgets);
    DDX_Control(pDX, IDC_PARENT_SHOW_CHILD_WIDGETS, mParentShowChildWidgets);

    DDX_Control(pDX, IDC_ON_ANIMATION_COMPLETE, mOnAnimationCompleteGroup);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnOK()
{
    if (!SaveActionInfo())
    {
        return;
    }

    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnCancel()
{
    action_info *action;
    for (int index = 0; index < mActionList.GetCount(); index++)
    {
        action = mActionList.GetAt(index);
        action->RemoveAnimationIdFromDictionary(mDisplayIndex);
    }

    mpTrigger->AddAnimationIdToDictionary(mDisplayIndex);

    express_dialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_action_edit_dlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {

        CWnd* focus_owner = GetFocus();
        int ctrl_id = -1;

        if (focus_owner)
        {
            ctrl_id = focus_owner->GetDlgCtrlID();
        }
        if ((pMsg->wParam == VK_RETURN) &&
            (ctrl_id == IDC_ACTION_NAME))
        {
            OnChangeActionName();
            return TRUE;
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnAddAction()
{
    trigger_action_select_dlg dlg(this);

    if (dlg.DoModal() == IDOK)
    {
        CString action_name("");
        int action_type = 0;

        action_name = dlg.GetSelectedActionName();
        action_type = dlg.GetSelectedActionType();

        if ((action_type > 0) && SaveAnimationInfo())
        {
            //add new action to the action list
            action_info *action = new action_info;
            action->action_type = action_type;
            action->action_name = action_name;
            action->target_widget_name = _T("");
            action->parent_widget_name = _T("");
            trigger_info::CreateUniqueActionName(mActionList, action);

            mActionList.Add(action);

            int sel = mAddedActionList.AddString(action->action_name);
            mAddedActionList.SetItemData(sel, action_type);
            mAddedActionList.SetCurSel(sel);

            mAddedActionListSel = sel;

            mTargetShowChildWidgets.SetCheck(FALSE);
            mParentShowChildWidgets.SetCheck(FALSE);
            OnClickTargetShowChildWidgets();
            OnClickParentShowChildWidgets();

            SetDlgItemText(IDC_ACTION_TYPE, trigger_action_select_dlg::GetActionTypeName(action->action_type));
            SetDlgItemText(IDC_ACTION_NAME, action->action_name);

            //if the new action type is animation, show animation params
            ShowHideAnimationParams();

            EnableDisableActionDeleteButton();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnDeleteAction()
{
    int sel = mAddedActionList.GetCurSel();

    if (sel >= 0)
    {
        int type = mAddedActionList.GetItemData(sel);
        CString action_name;

        mAddedActionList.GetText(sel, action_name);
        mAddedActionList.DeleteString(sel);

        action_info *action;
        for (int index = 0; index < mActionList.GetCount(); index++)
        {
            action = mActionList.GetAt(index);

            if (action->action_name == action_name)
            {
                action->RemoveAnimationIdFromDictionary(mDisplayIndex);

                delete mActionList.GetAt(index);
                mActionList.RemoveAt(index);
                break;
            }
        }

        if (sel < mAddedActionList.GetCount())
        {
            mAddedActionList.SetCurSel(sel);
        }
        else if (sel > 0)
        {
            mAddedActionList.SetCurSel(sel - 1);
        }

        mAddedActionListSel = -1;
        OnSelectAddedActionList();

        CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

        if (pMain)
        {
            CString msg;
            msg.Format(L"%s action is deleted.", action_name);
            mStatusMsg.SetWindowText(msg);
            mStatusMsg.NotifyWinEvent(
                EVENT_OBJECT_LIVEREGIONCHANGED,
                OBJID_CLIENT,
                CHILDID_SELF);
        }

        EnableDisableActionDeleteButton();
    }
}


///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnSelectAddedActionList()
{
    // TODO: Add your control notification handler code here

    if ((mAddedActionListSel >= 0) &&
        (mAddedActionListSel < mAddedActionList.GetCount()))
    {
        int previous_sel_type = mAddedActionList.GetItemData(mAddedActionListSel);

        if (previous_sel_type == GX_ACTION_TYPE_ANIMATION)
        {
            //save animation information for previous selected action
            if (!SaveAnimationInfo())
            {
                mAddedActionList.SetCurSel(mAddedActionListSel);
                return;
            }
        }
    }

    action_info *action = SelectedActionGet();
    if (action)
    {
        BOOL check = mTargetShowChildWidgets.GetCheck();
        if (check != action->target_show_child_widgets)
        {
            mTargetShowChildWidgets.SetCheck(action->target_show_child_widgets);
            ResetTargets(&mTargetCombo);
        }

        check = mParentShowChildWidgets.GetCheck();
        if (check != action->target_show_child_widgets)
        {
            mParentShowChildWidgets.SetCheck(action->parent_show_child_widgets);
            ResetTargets(&mParentCombo);
        }

        // set target screen name for selected action
        UpdateTargetCombo(&mTargetCombo, action->target_widget_name);
        UpdateTargetCombo(&mParentCombo, action->parent_widget_name);

        // set action type/name for selected action
        SetDlgItemText(IDC_ACTION_TYPE, trigger_action_select_dlg::GetActionTypeName(action->action_type));
        SetDlgItemText(IDC_ACTION_NAME, action->action_name);
    }

    mAddedActionListSel = mAddedActionList.GetCurSel();
    ShowHideAnimationParams();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnChangeActionName()
{
    action_info *action = SelectedActionGet();
    if (action)
    {
        CString new_name;
        GetDlgItemText(IDC_ACTION_NAME, new_name);

        if (new_name.IsEmpty())
        {
            SetDlgItemText(IDC_ACTION_NAME, action->action_name);
            ErrorMsg(_T("Invalid Action Name!"), this);
        }
        else if ((new_name != action->action_name) &&
            (trigger_info::FindActionName(mActionList, new_name)))
        {
            SetDlgItemText(IDC_ACTION_NAME, action->action_name);
            ErrorMsg(_T("Action Name Already Exist!"), this);
        }
        else
        {
            int sel = mAddedActionList.GetCurSel();
            mAddedActionList.DeleteString(sel);

            action->action_name = new_name;

            sel = mAddedActionList.AddString(action->action_name);
            mAddedActionList.SetItemData(sel, action->action_type);
            mAddedActionList.SetCurSel(sel);

            mAddedActionListSel = sel;


            //Update horizontal extend
            UpdateListBoxHorizontalExtend(&mAddedActionList);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnSelectTargetCombo()
{
    OnSelectTargetCombo(&mTargetCombo);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnSelectParentCombo()
{
    OnSelectTargetCombo(&mParentCombo);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnClickEasingFunction()
{
    easing_function_select_dlg dlg(mAnimationEasingFuncId, this);
    if (dlg.DoModal() == IDOK)
    {
        mAnimationEasingFuncId = dlg.GetEasingFuncId();
        SetEasingFunction();
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnClickTargetShowChildWidgets()
{
    //reset target combolist with child widgets
    ResetTargets(&mTargetCombo);

    BOOL check = mTargetShowChildWidgets.GetCheck();

    action_info *action = SelectedActionGet();

    if (action)
    {
        UpdateTargetCombo(&mTargetCombo, action->target_widget_name);
        action->target_show_child_widgets = check;
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnClickParentShowChildWidgets()
{
    //reset target and parent combolist with child widgets
    ResetTargets(&mParentCombo);

    BOOL check = mParentShowChildWidgets.GetCheck();

    action_info *action = SelectedActionGet();

    if (action)
    {
        UpdateTargetCombo(&mParentCombo, action->parent_widget_name);
        action->parent_show_child_widgets = check;
    }
}


///////////////////////////////////////////////////////////////////////////////
action_info *trigger_action_edit_dlg::SelectedActionGet()
{
    int action_sel = mAddedActionList.GetCurSel();

    if (action_sel >= 0)
    {
        CString action_name;
        mAddedActionList.GetText(action_sel, action_name);

        action_info *action;
        for (int index = 0; index < mActionList.GetCount(); index++)
        {
            action = mActionList.GetAt(index);

            if (action_name == action->action_name)
            {
                return action;
            }
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::SetEasingFunction()
{

    CString easing_func = easing_function_select_dlg::GetEasingFuncIdName(mAnimationEasingFuncId);
    if (easing_func.IsEmpty())
    {
        easing_func = _T("None");
    }
    mEasingFunctionEdit.SetWindowText(easing_func);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::UpdateListBoxHorizontalExtend(CListBox *pList)
{
    // Find the longest string in the list box.
    CString    str;
    CSize      sz;
    int        dx = 0;
    TEXTMETRIC tm;
    CDC*       pDC = pList->GetDC();
    CFont*     pFont = pList->GetFont();

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

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::OnSelectTargetCombo(CComboBox *pBox)
{
    action_info *action = SelectedActionGet();

    if (action)
    {
        int     target_sel = pBox->GetCurSel();
        CString target_name;

        if (target_sel == 0)
        {
            target_name = "";
        }
        else
        {
            pBox->GetLBText(target_sel, target_name);
        }

        if (pBox == &mTargetCombo)
        {
            action->target_widget_name = target_name;
        }
        else
        {
            action->parent_widget_name = target_name;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::UpdateTargetCombo(CComboBox *pBox, CString &string)
{
    if (string.IsEmpty())
    {
        pBox->SetCurSel(0);
    }
    else
    {
        pBox->SelectString(0, string);
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::AddComboList(CComboBox *pBox, STRING_VAL_PAIR *list, LONG current_val)
{
    STRING_VAL_PAIR *current = list;
    int index;

    while (current->name.GetLength())
    {
        index = pBox->AddString(current->name);
        pBox->SetItemData(index, current->val);
        current++;
    }

    pBox->InsertString(0, _T("None"));
    SelectComboListItem(pBox, current_val);
}

void trigger_action_edit_dlg::SelectComboListItem(CComboBox *box, LONG val)
{
    int sel_index = 0;

    for (int box_index = 0; box_index < box->GetCount(); box_index++)
    {
        if (box->GetItemData(box_index) == val)
        {
            sel_index = box_index;
            break;
        }
    }

    box->SetCurSel(sel_index);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::SelectComboListItem(CComboBox *box, CString string)
{
    CString get_string;
    int sel_index = 0;

    for (int box_index = 0; box_index < box->GetCount(); box_index++)
    {
        box->GetLBText(box_index, get_string);
        if (get_string == string)
        {
            sel_index = box_index;
            break;
        }
    }

    box->SetCurSel(sel_index);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::InitActionLists()
{
    CListBox *pBox= &mAddedActionList;

    action_info *action;

    if (mActionList.IsEmpty())
    {
        AddTargets(&mTargetCombo);
        AddTargets(&mParentCombo);
    }
    else
    {
        for (int index = 0; index < mActionList.GetCount(); index++)
        {
            action = mActionList.GetAt(index);
            int sel = pBox->AddString(action->action_name);
            pBox->SetItemData(sel, action->action_type);
        }
    }

    if (pBox->GetCount())
    {
        pBox->SetCurSel(0);
        mAddedActionListSel = 0;
        action = SelectedActionGet();

        if (action)
        {
            mTargetShowChildWidgets.SetCheck(action->target_show_child_widgets);
            mParentShowChildWidgets.SetCheck(action->parent_show_child_widgets);
            AddTargets(&mTargetCombo);
            AddTargets(&mParentCombo);

            UpdateTargetCombo(&mTargetCombo, action->target_widget_name);
            UpdateTargetCombo(&mParentCombo, action->parent_widget_name);
            SetDlgItemText(IDC_ACTION_TYPE, trigger_action_select_dlg::GetActionTypeName(action->action_type));
            SetDlgItemText(IDC_ACTION_NAME, action->action_name);
        }
    }

    UpdateListBoxHorizontalExtend(pBox);
    EnableDisableActionDeleteButton();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::AddChildTargets(CComboBox *pBox, widget_info *info)
{
    widget_info *child = info->GetChildWidgetInfo();
    while (child)
    {
        pBox->AddString(child->app_name);

        if (child->GetChildWidgetInfo())
        {
            AddChildTargets(pBox, child);
        }
        child = child->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::AddTargets(CComboBox *pBox)
{
    int pos;
    widget_info *info;
    CButton *pButton;

    if (pBox == &mTargetCombo)
    {
        pButton = &mTargetShowChildWidgets;
    }
    else
    {
        pButton = &mParentShowChildWidgets;
    }

    if (pButton->GetCheck())
    {
        //add current screen
        //pBox->AddString(mpInfo->app_name);

        //add child widgets
        studiox_project *project = GetOpenProject();
        if (project)
        {
            AddChildTargets(pBox, mpInfo);
        }
    }
    else
    {
        //add top level widgets
        CArray<widget_info *> screen_list;
        GetProjectView()->GetTopLevelWidgetList(mDisplayIndex, &screen_list);

        for (int index = 0; index < screen_list.GetCount(); index++)
        {
            info = screen_list.GetAt(index);

            if (!info->is_template)
            {
                pos = pBox->AddString(info->app_name);
            }
        }
    }

    pBox->InsertString(0, _T("None"));
    pBox->InsertString(1, SCREEN_STACK_POP_STRING);
    pBox->SetCurSel(0);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::ResetTargets(CComboBox *pBox)
{
    pBox->ResetContent();
    AddTargets(pBox);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::ShowHideAnimationParams()
{
    BOOL show = FALSE;
    action_info *action = NULL;
    GX_ANIMATION_INFO *animation = NULL;

    if (mAddedActionListSel >= 0)
    {
        int action_type = mAddedActionList.GetItemData(mAddedActionListSel);
        if (action_type == GX_ACTION_TYPE_ANIMATION)
        {
            show = TRUE;
        }

        CString action_name;
        mAddedActionList.GetText(mAddedActionListSel, action_name);

        action = trigger_info::GetActionInfo(mActionList, action_name);
        animation = action->animation;
    }

    mAnimationParams.ShowWindow(show);


    if (animation)
    {
        if (animation->gx_animation_style & GX_ANIMATION_DETACH)
        {
            mDetachTargetBox.SetCheck(TRUE);
        }
        else
        {
            mDetachTargetBox.SetCheck(FALSE);
        }

        if (animation->gx_animation_style & GX_ANIMATION_PUSH_STACK)
        {
            mPushTargetBox.SetCheck(TRUE);
        }
        else
        {
            mPushTargetBox.SetCheck(FALSE);
        }

        mAnimationEasingFuncId = animation->gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK;
        SetEasingFunction();
    }
    else
    {
        mAnimationEasingFuncId = 0;
        SetEasingFunction();
    }

    mEasingFunctionButton.ShowWindow(show);
    mEasingFunctionEdit.ShowWindow(show);

    CString string;
    int index;

    for (index = 0; index < ANIMATION_PARAMS_COUNT; index++)
    {
        mAnimationParamLabels[index].ShowWindow(show);
        mAnimationParamEdits[index].ShowWindow(show);

        switch (AnimationParamNames[index].val)
        {
        case ID_ANIMATION_ID:
            if (animation)
            {
                string = action->animation_id_name;
            }
            else
            {
                string = _T("");
            }
            break;

        case ID_ANIMATION_START_X:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_start_position.gx_point_x);
            }
            else
            {
                string = _T("0");
            }
            break;

        case ID_ANIMATION_START_Y:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_start_position.gx_point_y);
            }
            else
            {
                string = _T("0");
            }
            break;

        case ID_ANIMATION_END_X:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_end_position.gx_point_x);
            }
            else
            {
                string = _T("0");
            }
            break;

        case ID_ANIMATION_END_Y:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_end_position.gx_point_y);
            }
            else
            {
                string = _T("0");
            }
            break;

        case ID_ANIMATION_STEPS:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_steps);
            }
            else
            {
                string = _T("10");
            }
            break;

        case ID_ANIMATION_TICK_RATE:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_frame_interval);
            }
            else
            {
                string = _T("1");
            }
            break;

        case ID_ANIMATION_DELAY_BEFORE:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_start_delay);
            }
            else
            {
                string = _T("0");
            }
            break;

        case ID_ANIMATION_START_ALPHA:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_start_alpha);
            }
            else
            {
                string = _T("255");
            }
            break;

        case ID_ANIMATION_END_ALPHA:
            if (animation)
            {
                string.Format(_T("%d"), animation->gx_animation_end_alpha);
            }
            else
            {
                string = _T("255");
            }
            break;
        }

        mAnimationParamEdits[index].SetWindowText(string);
    }


    mOnAnimationCompleteGroup.ShowWindow(show);

    mDetachTargetBox.ShowWindow(show);
    mPushTargetBox.ShowWindow(show);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_edit_dlg::EnableDisableActionDeleteButton()
{
    CWnd *delete_button = GetDlgItem(IDC_DELETE_ACTION);

    if (!delete_button)
    {
        return;
    }

    if (mActionList.GetCount() == 0)
    {
        if (GetFocus() == delete_button)
        {
            // Move focus to add button.
            CWnd *add_button = GetDlgItem(IDC_ADD_ACTION);
            if (add_button)
            {
                add_button->SetFocus();
            }
        }

        delete_button->EnableWindow(FALSE);
    }
    else
    {
        delete_button->EnableWindow(TRUE);
    }
}

BOOL trigger_action_edit_dlg::SaveActionInfo()
{
    if (!SaveAnimationInfo())
    {
        return FALSE;
    }

    //save action list
    mpTrigger->CleanActionList(mpTrigger->action_list);

    action_info *action;
    for (int index = 0; index < mActionList.GetCount(); index++)
    {
        action = mActionList.GetAt(index);
        mpTrigger->action_list.Add(action);
    }

    mActionList.RemoveAll();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_action_edit_dlg::SaveAnimationInfo()
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return FALSE;
    }

    if (mAddedActionListSel < 0)
    {
        // no aciton been selected, just return
        return TRUE;
    }

    int type = mAddedActionList.GetItemData(mAddedActionListSel);

    if (type != GX_ACTION_TYPE_ANIMATION)
    {
        // current selected action type is not animation, just return
        return TRUE;
    }

    CString action_name;
    mAddedActionList.GetText(mAddedActionListSel, action_name);

    action_info *action = trigger_info::GetActionInfo(mActionList, action_name);

    //int cursel = mAnimationTypesCombo.GetCurSel();

    if (!action->animation)
    {
        action->animation = new GX_ANIMATION_INFO;
        memset(action->animation, 0, sizeof(GX_ANIMATION_INFO));
    }

    for (int index = 0; index < ANIMATION_PARAMS_COUNT; index++)
    {
        CString text;
        mAnimationParamEdits[index].GetWindowText(text);
        int value = _ttoi(text);

        switch (AnimationParamNames[index].val)
        {
        case ID_ANIMATION_ID:
            if (action->animation_id_name != text)
            {
                CString old_text = action->animation_id_name;
                if (text.IsEmpty())
                {
                    screen_flow *flow = project->mDisplays[mDisplayIndex].screenflow;
                    if (flow && flow->IsAnimationIdBeReferenced(old_text))
                    {
                        mAnimationParamEdits[index].SetWindowText(old_text);
                        Notify("Animation id cannot be empty, it is referened by another trigger!", this);
                        return FALSE;
                    }
                    else
                    {
                        //remove old animation id name
                        action->RemoveAnimationIdFromDictionary(mDisplayIndex);
                        action->animation_id_name = text;
                    }
                }
                else if (TestInputName(&mAnimationParamEdits[index], "Animation ID", old_text, this))
                {
                    //remove old animation id name
                    action->RemoveAnimationIdFromDictionary(mDisplayIndex);

                    action->animation_id_name = text;

                    //add new animation id name
                    action->AddAnimationIdToDictionary(mDisplayIndex);
                }
                else
                {
                    return FALSE;
                }
            }
            break;

        case ID_ANIMATION_START_X:
            action->animation->gx_animation_start_position.gx_point_x = value;
            break;

        case ID_ANIMATION_START_Y:
            action->animation->gx_animation_start_position.gx_point_y = value;
            break;

        case ID_ANIMATION_END_X:
            action->animation->gx_animation_end_position.gx_point_x = value;
            break;

        case ID_ANIMATION_END_Y:
            action->animation->gx_animation_end_position.gx_point_y = value;
            break;

        case ID_ANIMATION_STEPS:
            action->animation->gx_animation_steps = value;
            break;

        case ID_ANIMATION_TICK_RATE:
            action->animation->gx_animation_frame_interval = value;
            break;

        case ID_ANIMATION_DELAY_BEFORE:
            action->animation->gx_animation_start_delay = value;
            break;

        case ID_ANIMATION_START_ALPHA:
            action->animation->gx_animation_start_alpha = value;
            break;

        case ID_ANIMATION_END_ALPHA:
            action->animation->gx_animation_end_alpha = value;
            break;
        }
    }

    if (mDetachTargetBox.GetCheck())
    {
        action->animation->gx_animation_style |= GX_ANIMATION_DETACH;
    }
    else
    {
        action->animation->gx_animation_style &= ~GX_ANIMATION_DETACH;
    }

    if (mPushTargetBox.GetCheck())
    {
        action->animation->gx_animation_style |= GX_ANIMATION_PUSH_STACK;
    }
    else
    {
        action->animation->gx_animation_style &= ~GX_ANIMATION_PUSH_STACK;
    }

    CString easingfunc;
    mEasingFunctionEdit.GetWindowText(easingfunc);
    mAnimationEasingFuncId = easing_function_select_dlg::GetEasingFuncId(easingfunc);
    action->animation->gx_animation_style &= ~GX_ANIMATION_EASING_FUNC_MASK;
    action->animation->gx_animation_style |= mAnimationEasingFuncId;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT trigger_action_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    int ctrl_id;
    CWnd *pWnd;
    CString str;
    CStringArray param_list;

    switch (wParam)
    {
    case TEST_ADD_ACTION:
        OnAddAction();
        break;

    case TEST_SELECT_ACTION:
        mAddedActionList.SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ADDED_ACTION_LIST, LBN_SELCHANGE), (LPARAM)mAddedActionList.m_hWnd);
        break;

    case TEST_DELETE_ACTION:
        OnDeleteAction();
        break;

    case TEST_SELECT_TARGET:
        mTargetCombo.SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_TARGET_COMBO, CBN_SELCHANGE), (LPARAM)mTargetCombo.m_hWnd);
        break;

    case TEST_SELECT_PARENT:
        mParentCombo.SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_PARENT_COMBO, CBN_SELCHANGE), (LPARAM)mParentCombo.m_hWnd);
        break;

    case TEST_EDIT_ACTION_NAME:
        ctrl_id = IDC_ACTION_NAME;
        pWnd = GetDlgItem(ctrl_id);
        if (pWnd)
        {
            ((CEdit *)pWnd)->SetWindowText(GetTestingParam(0));
            SendMessage(WM_COMMAND, MAKEWPARAM(ctrl_id, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_EDIT_ANIMATION_INFO:
        SplitString(GetTestingParam(0), ',', &param_list);
        if (param_list.GetCount() == 2)
        {
            ctrl_id = _tstoi(param_list.GetAt(0)) - ID_ANIMATION_ID;
            str = param_list.GetAt(1);

            mAnimationParamEdits[ctrl_id].SetWindowText(str);
        }
        break;

    case TEST_CHECK_DETACH_TARGET:
        mDetachTargetBox.SetCheck(lParam);
        break;

    case TEST_CONFIGURE_EASING_FUNCTION:
        OnClickEasingFunction();
        break;

    case TEST_TARGET_SHOW_CHILD_WIDGETS:
        mTargetShowChildWidgets.SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_TARGET_SHOW_CHILD_WIDGETS, BN_CLICKED), (LPARAM)mTargetShowChildWidgets.m_hWnd);
        break;

    case TEST_PARENT_SHOW_CHILD_WIDGETS:
        mParentShowChildWidgets.SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_PARENT_SHOW_CHILD_WIDGETS, BN_CLICKED), (LPARAM)mParentShowChildWidgets.m_hWnd);
        break;

    case TEST_SAVE_TRIGGER_ACTION_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_TRIGGER_ACTION_EDIT:
        OnCancel();
        break;
    }

    return 0;
}
