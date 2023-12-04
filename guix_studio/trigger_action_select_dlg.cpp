#include "studiox_includes.h"
#include "trigger_action_select_dlg.h"

// trigger_action_select_dlg dialog
enum trigger_action_select_dlg_test_commands{
    TEST_SELECT_ACTION = 1,
    TEST_SAVE_TRIGGER_ACTION_SELECT,
    TEST_CANCEL_TRIGGER_ACTION_SELECT
};

// Define common actions
STRING_VAL_PAIR ActionTypes[] = {
    { _T("Attach"), GX_ACTION_TYPE_ATTACH },
    { _T("Detach"), GX_ACTION_TYPE_DETACH },
    { _T("Toggle"), GX_ACTION_TYPE_TOGGLE },
    { _T("Show"), GX_ACTION_TYPE_SHOW },
    { _T("Hide"), GX_ACTION_TYPE_HIDE },
    { _T("Animation"), GX_ACTION_TYPE_ANIMATION },
    { _T("Window Execute"), GX_ACTION_TYPE_WINDOW_EXECUTE },
    { _T("Window Execute Stop"), GX_ACTION_TYPE_WINDOW_EXECUTE_STOP },
    { _T("Screen Stack Push"), GX_ACTION_TYPE_SCREEN_STACK_PUSH },
    { _T("Screen Stack Pop"), GX_ACTION_TYPE_SCREEN_STACK_POP },
    { _T("Screen Stack Reset"), GX_ACTION_TYPE_SCREEN_STACK_RESET },
    { _T(""), 0 }
};

IMPLEMENT_DYNAMIC(trigger_action_select_dlg, express_dialog)

BEGIN_MESSAGE_MAP(trigger_action_select_dlg, express_dialog)
    ON_MESSAGE(STUDIO_TEST, &trigger_action_select_dlg::OnTestMessage)
    ON_LBN_SELCHANGE(IDC_ACTION_LIST, &trigger_action_select_dlg::OnSelectActionList)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
trigger_action_select_dlg::trigger_action_select_dlg(CWnd* pParent /*=NULL*/)
: express_dialog(trigger_action_select_dlg::IDD, pParent)
{
    IconId = IDB_MACRO_PLAYBACK_ICON;
    SetTitleText("Select Action");

    mSelectedActionName = "";
    mSelectedActionType = 0;
}

///////////////////////////////////////////////////////////////////////////////
trigger_action_select_dlg::~trigger_action_select_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
int trigger_action_select_dlg::GetActionType(CString type_name)
{
    return FindPairVal(ActionTypes, type_name);
}

///////////////////////////////////////////////////////////////////////////////
CString trigger_action_select_dlg::GetActionTypeName(int type)
{
    return FindPairString(ActionTypes, type);
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_action_select_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    AddCancelButton();
    AddSaveButton();

    //initiate action list
    InitActionLists();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_select_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_ACTION_LIST, mActionList);
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_select_dlg::UpdateListBoxHorizontalExtend(CListBox *pList)
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
void trigger_action_select_dlg::InitActionLists()
{
    STRING_VAL_PAIR *current = ActionTypes;
    CListBox *pBox = &mActionList;
    int index;

    while (current->name.GetLength())
    {
        index = pBox->AddString(current->name);
        pBox->SetItemData(index, current->val);
        current++;
    }
    UpdateListBoxHorizontalExtend(pBox);
    pBox->SetCurSel(0);

    OnSelectActionList();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_action_select_dlg::OnSelectActionList()
{
    int sel = mActionList.GetCurSel();

    mActionList.GetText(sel, mSelectedActionName);
    mSelectedActionType = mActionList.GetItemData(sel);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT trigger_action_select_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_SELECT_ACTION:
        mActionList.SelectString(0, GetTestingParam(0));
        OnSelectActionList();
        break;

    case TEST_SAVE_TRIGGER_ACTION_SELECT:
        OnOK();
        break;

    case TEST_CANCEL_TRIGGER_ACTION_SELECT:
        OnCancel();
        break;
    }
    return 0;
}

