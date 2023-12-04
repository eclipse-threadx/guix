#include "studiox_includes.h"
#include "trigger_edit_dlg.h"

// trigger_edit_dlg dialog

enum trigger_edit_dlg_test_commands{
    TEST_SET_TRIGGER_TYPE = 1,
    TEST_SET_EVENT_TYPE,
    TEST_SAVE_TRIGGER_EDIT,
    TEST_CANCEL_TRIGGER_EDIT
};

// Define trigger types
STRING_VAL_PAIR TriggerTypes[] = {
    { _T("system_event"), TRIGGER_TYPE_SYSTEM_EVENT },
    { _T("child_signal"), TRIGGER_TYPE_CHILD_SIGNAL },
    { _T("user_event"), TRIGGER_TYPE_USER_EVENT },
    { _T(""), 0 }
};

// Define common system events
STRING_VAL_PAIR SystemEventTypes[] = {
    { _T("GX_EVENT_SHOW"), GX_EVENT_SHOW },
    { _T("GX_EVENT_HIDE"), GX_EVENT_HIDE },
    { _T("GX_EVENT_FOCUS_GAINED"), GX_EVENT_FOCUS_GAINED },
    { _T("GX_EVENT_FOCUS_LOST"), GX_EVENT_FOCUS_LOST },
    { _T("GX_EVENT_ANIMATION_COMPLETE"), GX_EVENT_ANIMATION_COMPLETE },
    { _T("GX_EVENT_PEN_DOWN"), GX_EVENT_PEN_DOWN },
    { _T("GX_EVENT_PEN_DRAG"), GX_EVENT_PEN_DRAG },
    { _T("GX_EVENT_PEN_UP"), GX_EVENT_PEN_UP },
    { _T("GX_EVENT_KEY_DOWN"), GX_EVENT_KEY_DOWN },
    { _T("GX_EVENT_KEY_UP"), GX_EVENT_KEY_UP },
    { _T("GX_EVENT_CLOSE_POPUP"), GX_EVENT_CLOSE_POPUP },
    { _T("GX_EVENT_LIST_SELECT"), GX_EVENT_LIST_SELECT },
    { _T("GX_EVENT_HORIZONTAL_FLICK"), GX_EVENT_HORIZONTAL_FLICK },
    { _T("GX_EVENT_VERTICAL_FLICK"), GX_EVENT_VERTICAL_FLICK},
    { _T(""), 0 }
};

STRING_VAL_PAIR SignalEventTypes[] = {
    { _T("GX_EVENT_FOCUS_GAINED"), GX_EVENT_FOCUS_GAINED },
    { _T("GX_EVENT_FOCUS_LOST"), GX_EVENT_FOCUS_LOST },
    { _T("GX_EVENT_CLICKED"), GX_EVENT_CLICKED },
    { _T("GX_EVENT_RADIO_SELECT"), GX_EVENT_RADIO_SELECT },
    { _T("GX_EVENT_RADIO_DESELECT"), GX_EVENT_RADIO_DESELECT },
    { _T("GX_EVENT_TOGGLE_ON"), GX_EVENT_TOGGLE_ON },
    { _T("GX_EVENT_TOGGLE_OFF"), GX_EVENT_TOGGLE_OFF },
    { _T("GX_EVENT_SELECT"), GX_EVENT_TOGGLE_OFF },
    { _T("GX_EVENT_DESELECT"), GX_EVENT_DESELECT },
    { _T("GX_EVENT_SLIDER_VALUE"), GX_EVENT_SLIDER_VALUE },
    { _T("GX_EVENT_LIST_SELECT"), GX_EVENT_LIST_SELECT },
    { _T("GX_EVENT_ANIMATION_COMPLETE"), GX_EVENT_ANIMATION_COMPLETE },
    { _T("GX_EVENT_TEXT_EDITED"), GX_EVENT_TEXT_EDITED },
    { _T("GX_EVENT_SPRITE_COMPLETE"), GX_EVENT_SPRITE_COMPLETE },
    { _T(""), 0 }
};

IMPLEMENT_DYNAMIC(trigger_edit_dlg, express_dialog)

BEGIN_MESSAGE_MAP(trigger_edit_dlg, express_dialog)
    ON_CBN_SELCHANGE(IDC_SYSTEM_EVENTS_COMBO, &trigger_edit_dlg::OnChangeSystemEvent)
    ON_MESSAGE(STUDIO_TEST, &trigger_edit_dlg::OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
trigger_edit_dlg::trigger_edit_dlg(int display_index, flow_item *item, trigger_info *trigger, CWnd* pParent /*=NULL*/)
    : express_dialog(trigger_edit_dlg::IDD, pParent)
{
    CString title;

    //set dialog icon
    IconId = IDB_MACRO_PLAYBACK_ICON;

    //set dialog title
    if (trigger->event_type)
    {
        title.Format(_T("Edit Trigger for [%s]"), item->screen_name);
    }
    else
    {
        title.Format(_T("Add Trigger for [%s]"), item->screen_name);
    }
    SetTitleText(title);
    studiox_project *project = GetOpenProject();
    widget_info *info = NULL;
    if (project)
    {
        info = project->FindWidgetInfo(project->mDisplays[display_index].GetFirstChildFolder(), item->screen_name, FALSE);
    }

    mDisplayIndex = display_index;
    mpFlowItem = item;
    mpTrigger = trigger;
    mpInfo = info;

    mOldTrigger = *trigger;
}

///////////////////////////////////////////////////////////////////////////////
trigger_edit_dlg::~trigger_edit_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
int trigger_edit_dlg::GetTriggerType(CString type_name)
{
    return FindPairVal(TriggerTypes, type_name);
}

///////////////////////////////////////////////////////////////////////////////
CString trigger_edit_dlg::GetTriggerTypeName(int type)
{
    return FindPairString(TriggerTypes, type);
}

///////////////////////////////////////////////////////////////////////////////
int trigger_edit_dlg::GetEventType(CString type_name)
{
    int type = FindPairVal(SystemEventTypes, type_name);

    if (!type)
    {
        type = FindPairVal(SignalEventTypes, type_name);
    }

    return type;
}

///////////////////////////////////////////////////////////////////////////////
CString trigger_edit_dlg::GetEventTypeName(int type)
{
    CString name = FindPairString(SystemEventTypes, type);

    if (name.IsEmpty())
    {
        name = FindPairString(SignalEventTypes, type);
    }

    return name;
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_edit_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    CRect rect;
    CRect size;

    AddCancelButton();
    AddSaveButton();

    //SetWindowText(title_text);

    LONG system_event = 0, child_signal = 0;
    switch (mpTrigger->trigger_type)
    {
    case TRIGGER_TYPE_CHILD_SIGNAL:
        ((CButton *)GetDlgItem(IDC_CHILD_SIGNAL_RADIO))->SetCheck(TRUE);
        child_signal = mpTrigger->event_type;
        break;

    case TRIGGER_TYPE_USER_EVENT:
        ((CButton *)GetDlgItem(IDC_USER_EVENT_RADIO))->SetCheck(TRUE);
        mUserEventName.SetWindowText(mpTrigger->user_event_id_name);
        break;

    case TRIGGER_TYPE_SYSTEM_EVENT:
    default:
        ((CButton *)GetDlgItem(IDC_SYSTEM_EVENT_RADIO))->SetCheck(TRUE);
        system_event = mpTrigger->event_type;
        break;
    }

    AddComboList(&mSystemEventsCombo, SystemEventTypes, system_event);
    AddChildSignals(mpTrigger->signal_id_name, child_signal);
    AddAnimationIds(mpTrigger->system_event_animat_id_name);
    EnableDisableAnimationIdsCombo();

    SetControlAccessibleName(mSystemEventsCombo.GetSafeHwnd(), _T("system_event_select"));
    SetControlAccessibleName(mChildSignalsCombo.GetSafeHwnd(), _T("child_signal_select"));
    SetControlAccessibleName(mUserEventName.GetSafeHwnd(), _T("user_event_name"));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SYSTEM_EVENTS_COMBO, mSystemEventsCombo);
    DDX_Control(pDX, IDC_CHILD_SIGNALS_COMBO, mChildSignalsCombo);
    DDX_Control(pDX, IDC_ANIMATION_IDS_COMBO, mAnimationIdsCombo);
    DDX_Control(pDX, IDC_ANIMATION_IDS_LABEL, mAnimationIdsLabel);
    DDX_Control(pDX, IDC_USER_EVENT_NAME, mUserEventName);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::OnOK()
{
    if (!SaveTriggerInfo())
    {
        return;
    }

    if (!screen_flow::CheckAddTrigger(mpFlowItem, mpTrigger, this))
    {
        return;
    }
    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::OnCancel()
{
    mpTrigger->trigger_name = mOldTrigger.trigger_name;
    mpTrigger->trigger_type = mOldTrigger.trigger_type;
    mpTrigger->system_event_animat_id_name = mOldTrigger.system_event_animat_id_name;
    mpTrigger->signal_id_name = mOldTrigger.signal_id_name;
    mpTrigger->user_event_id_name = mOldTrigger.user_event_id_name;
    mpTrigger->event_type = mOldTrigger.event_type;

    express_dialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::OnChangeSystemEvent()
{
    EnableDisableAnimationIdsCombo();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::AddComboList(CComboBox *pBox, STRING_VAL_PAIR *list, LONG current_val)
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

void trigger_edit_dlg::SelectComboListItem(CComboBox *box, LONG val)
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
void trigger_edit_dlg::SelectComboListItem(CComboBox *box, CString string)
{
    CString get_string;
    int sel_index = 0;

    for (int box_index = 0; box_index < box->GetCount(); box_index++)
    {
        box->GetLBText(box_index, get_string);
        if(get_string == string)
        {
            sel_index = box_index;
            break;
        }
    }

    box->SetCurSel(sel_index);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::AddChildSignals(CComboBox *pBox, CArray<CString> *pNameList, widget_info *child, CString signal_id_name, LONG current_val)
{
CArray<INT> event_list;
CString signal_string;
CString selected_string;
int event_type;
int pos;

    while (child)
    {
        if ((!child->id_name.IsEmpty()))
        {
            GetWidgetEventList(child, &event_list);

            if (!event_list.IsEmpty() && !IsItemInArray<CString>(*pNameList, child->id_name))
            {
                for (int index = 0; index < event_list.GetCount(); index++)
                {
                    event_type = event_list.GetAt(index);
                    signal_string.Format(_T("(%s, %s)"), child->id_name, GetEventTypeName(event_type));
                    pos = pBox->AddString(signal_string);
                    pBox->SetItemData(pos, event_type);

                    if (child->id_name == signal_id_name &&
                        event_type == current_val)
                    {
                        selected_string = signal_string;
                    }
                }
                pNameList->Add(child->id_name);
            }
        }

        if (child->GetChildWidgetInfo())
        {
            AddChildSignals(pBox, pNameList, child->GetChildWidgetInfo(), signal_id_name, current_val);
        }

        child = child->GetNextWidgetInfo();
    }

    if (!selected_string.IsEmpty())
    {
        pBox->SelectString(0, selected_string);
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::AddChildSignals(CString signal_id_name, LONG current_val)
{
    mNameList.RemoveAll();
    AddChildSignals(&mChildSignalsCombo, &mNameList, mpInfo->GetChildWidgetInfo(), signal_id_name, current_val);

    widget_info *base = mpInfo;
    while (base && base->basetype == GX_TYPE_TEMPLATE)
    {
        base = template_service_provider::GetBaseInfo(base);
        if (base)
        {
            AddChildSignals(&mChildSignalsCombo, &mNameList, base->GetChildWidgetInfo(), signal_id_name, current_val);
        }
    }

    mChildSignalsCombo.InsertString(0, _T("None"));

    int cursel = mChildSignalsCombo.GetCurSel();

    if (cursel < 0)
    {
        mChildSignalsCombo.SetCurSel(0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::AddAnimationIds(CString selected_name)
{
    studiox_project *project = GetOpenProject();
    CString id_name;
    INT id_index = 1;
    INT index;

    while (1)
    {
        id_name = project->GetIdName(mDisplayIndex, ID_TYPE_ANIMATION, id_index++);
        if (id_name.IsEmpty())
        {
            break;
        }
        else
        {
            index = mAnimationIdsCombo.AddString(id_name);
            mAnimationIdsCombo.SetItemData(index, id_index);
        }
    }

    mAnimationIdsCombo.InsertString(0, _T("None"));
    if (selected_name.IsEmpty())
    {
        mAnimationIdsCombo.SetCurSel(0);
    }
    else
    {
        mAnimationIdsCombo.SelectString(0, selected_name);
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::EnableDisableAnimationIdsCombo()
{
    int cur_sel = mSystemEventsCombo.GetCurSel();
    int event_type = mSystemEventsCombo.GetItemData(cur_sel);
    BOOL enable;

    if (event_type == GX_EVENT_ANIMATION_COMPLETE)
    {
        enable = TRUE;
    }
    else
    {
        enable = FALSE;
    }

    mAnimationIdsCombo.EnableWindow(enable);
    mAnimationIdsLabel.EnableWindow(enable);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_edit_dlg::GetWidgetEventList(widget_info *info, CArray<INT> *list)
{
    list->RemoveAll();

    if (!(info->style & GX_STYLE_ENABLED))
    {
        return;
    }

    list->Add(GX_EVENT_FOCUS_GAINED);
    list->Add(GX_EVENT_FOCUS_LOST);

    switch (info->basetype)
    {
    case GX_TYPE_BUTTON:
    case GX_TYPE_TEXT_BUTTON:
    case GX_TYPE_MULTI_LINE_TEXT_BUTTON:
    case GX_TYPE_PIXELMAP_BUTTON:
    case GX_TYPE_ICON_BUTTON:
        if (info->style & GX_STYLE_BUTTON_TOGGLE)
        {
            list->Add(GX_EVENT_TOGGLE_ON);
            list->Add(GX_EVENT_TOGGLE_OFF);
        }
        else
        {
            list->Add(GX_EVENT_CLICKED);
        }
        break;

    case GX_TYPE_RADIO_BUTTON:
        list->Add(GX_EVENT_RADIO_SELECT);
        list->Add(GX_EVENT_RADIO_DESELECT);
        break;

    case GX_TYPE_CHECKBOX:
        list->Add(GX_EVENT_TOGGLE_ON);
        list->Add(GX_EVENT_TOGGLE_OFF);
        break;

    case GX_TYPE_SLIDER:
    case GX_TYPE_PIXELMAP_SLIDER:
        list->Add(GX_EVENT_SLIDER_VALUE);
        break;

    case GX_TYPE_DROP_LIST:
    case GX_TYPE_VERTICAL_LIST:
    case GX_TYPE_HORIZONTAL_LIST:
        list->Add(GX_EVENT_LIST_SELECT);
        break;

    case GX_TYPE_CIRCULAR_GAUGE:
        list->Add(GX_EVENT_ANIMATION_COMPLETE);
        break;

    case GX_TYPE_MULTI_LINE_TEXT_INPUT:
    case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
        list->Add(GX_EVENT_TEXT_EDITED);
        break;

    case GX_TYPE_SPRITE:
        list->Add(GX_EVENT_SPRITE_COMPLETE);
        break;

    default:
        list->Add(GX_EVENT_CLICKED);
        break;
    }
}

BOOL trigger_edit_dlg::SaveEventType()
{
    CButton *radio = ((CButton *)GetDlgItem(IDC_SYSTEM_EVENT_RADIO));

    if (radio->GetCheck() == TRUE)
    {
        mpTrigger->trigger_type = TRIGGER_TYPE_SYSTEM_EVENT;
    }
    else
    {
        radio = ((CButton *)GetDlgItem(IDC_CHILD_SIGNAL_RADIO));

        if (radio->GetCheck() == TRUE)
        {
            mpTrigger->trigger_type = TRIGGER_TYPE_CHILD_SIGNAL;
        }
        else
        {
            mpTrigger->trigger_type = TRIGGER_TYPE_USER_EVENT;
        }
    }

    int index;
    CString string;

    switch (mpTrigger->trigger_type)
    {
    case TRIGGER_TYPE_SYSTEM_EVENT:
        index = mSystemEventsCombo.GetCurSel();
        mpTrigger->event_type = mSystemEventsCombo.GetItemData(index);
        if (!mpTrigger->event_type)
        {
            ErrorMsg("Please set event type!", this);
            return FALSE;
        }

        mSystemEventsCombo.GetLBText(index, string);
        mpTrigger->trigger_name.Format(_T("ON_%s"), string);

        if (mpTrigger->event_type == GX_EVENT_ANIMATION_COMPLETE)
        {
            index = mAnimationIdsCombo.GetCurSel();
            if (index)
            {
                mAnimationIdsCombo.GetWindowText(mpTrigger->system_event_animat_id_name);
                mpTrigger->trigger_name.Format(_T("ON_%s(%s)"), string, mpTrigger->system_event_animat_id_name);
            }
        }
        break;

    case TRIGGER_TYPE_CHILD_SIGNAL:
        index = mChildSignalsCombo.GetCurSel();
        mpTrigger->event_type = mChildSignalsCombo.GetItemData(index);
        if (!mpTrigger->event_type)
        {
            ErrorMsg("Please set event type!", this);
            return FALSE;
        }

        mChildSignalsCombo.GetLBText(index, string);
        mpTrigger->trigger_name.Format(_T("ON_GX_SIGNAL%s"), string);
        
        //Get signal id name
        index = string.Find('(');
        string = string.Mid(index+1);
        index = string.Find(',');
        string = string.Left(index);
        mpTrigger->signal_id_name = string;
        break;

    case TRIGGER_TYPE_USER_EVENT:
        string = mpTrigger->user_event_id_name;
        if (TestInputName(&mUserEventName, "User Event", mpTrigger->user_event_id_name, this))
        {
            mpTrigger->event_type = GX_FIRST_USER_EVENT;
            mpTrigger->trigger_name.Format(_T("ON_%s"), mpTrigger->user_event_id_name);
        }
        else
        {
            return FALSE;
        }
        
        break;
    }

    mpTrigger->trigger_name.MakeLower();
    return TRUE;
}

BOOL trigger_edit_dlg::SaveTriggerInfo()
{
    if (!SaveEventType())
    {
        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT trigger_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    int ctrl_id;
    CWnd *pWnd;
    CString str;
    CStringArray param_list;

    switch(wParam)
    {
    case TEST_SET_TRIGGER_TYPE:
        switch (lParam)
        {
        case TRIGGER_TYPE_SYSTEM_EVENT:
            ctrl_id = IDC_SYSTEM_EVENT_RADIO;
            break;

        case TRIGGER_TYPE_CHILD_SIGNAL:
            ctrl_id = IDC_CHILD_SIGNAL_RADIO;
            break;

        case TRIGGER_TYPE_USER_EVENT:
            ctrl_id = IDC_USER_EVENT_RADIO;
            break;
        }
        CheckRadioButton(IDC_SYSTEM_EVENT_RADIO, IDC_USER_EVENT_RADIO, ctrl_id);
        break;

    case TEST_SET_EVENT_TYPE:
        pWnd = (GetDlgItem(IDC_SYSTEM_EVENT_RADIO));
        str = GetTestingParam(0);
        if (((CButton *)pWnd)->GetCheck() == TRUE)
        {
            mSystemEventsCombo.SelectString(0, str);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SYSTEM_EVENTS_COMBO, CBN_SELCHANGE), (LPARAM)mSystemEventsCombo.m_hWnd);
        }
        else
        {
            pWnd = GetDlgItem(IDC_CHILD_SIGNAL_RADIO);

            if (((CButton *)pWnd)->GetCheck() == TRUE)
            {
                mChildSignalsCombo.SelectString(0, str);
                SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CHILD_SIGNALS_COMBO, CBN_SELCHANGE), (LPARAM)mChildSignalsCombo.m_hWnd);
            }
            else
            {
                mUserEventName.SetWindowText(str);
                SendMessage(WM_COMMAND, MAKEWPARAM(IDC_USER_EVENT_NAME, EN_KILLFOCUS), (LPARAM)mUserEventName.m_hWnd);
            }
        }
        break;

    case TEST_SAVE_TRIGGER_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_TRIGGER_EDIT:
        OnCancel();
        break;
    }

    return 0;
}