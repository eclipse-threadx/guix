#ifndef _TRIGGER_ACTION_EDIT_DLG_
#define _TRIGGER_ACTION_EDIT_DLG_
#include "afxwin.h"
#include "custom_edit.h"

enum TRIGGER_ACTION_EDIT_CONTROL_IDS {
    ID_EASING_FUNCTION_SELECT = 4096,
    ID_ANIMATION_ID,
    ID_ANIMATION_START_X,
    ID_ANIMATION_START_Y,
    ID_ANIMATION_END_X,
    ID_ANIMATION_END_Y,
    ID_ANIMATION_STEPS,
    ID_ANIMATION_TICK_RATE,
    ID_ANIMATION_DELAY_BEFORE,
    ID_ANIMATION_START_ALPHA,
    ID_ANIMATION_END_ALPHA,
    ID_ON_ANIMATION_COMPLETE_WND
};

#define ANIMATION_PARAMS_COUNT (ID_ANIMATION_END_ALPHA - ID_ANIMATION_ID + 1)

class trigger_action_edit_dlg : public express_dialog
{
    DECLARE_DYNAMIC(trigger_action_edit_dlg)

public:
    trigger_action_edit_dlg(int display_index, flow_item *item, trigger_info *trigger, CWnd* pParent = NULL);   // standard constructor
    virtual ~trigger_action_edit_dlg();

    // Dialog Data
    enum { IDD = IDD_TRIGGER_ACTION_EDIT_DLG };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnAddAction();
    afx_msg void OnDeleteAction();
    afx_msg void OnSelectAddedActionList();
    afx_msg void OnChangeActionName();
    afx_msg void OnSelectTargetCombo();
    afx_msg void OnSelectParentCombo();
    afx_msg void OnClickEasingFunction();
    afx_msg void OnClickTargetShowChildWidgets();
    afx_msg void OnClickParentShowChildWidgets();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    action_info *SelectedActionGet();

    void SetEasingFunction();
    void UpdateListBoxHorizontalExtend(CListBox *pList);

    void UpdateTargetCombo(CComboBox *pBox, CString &string);
    void OnSelectTargetCombo(CComboBox *pBox);

    void AddComboList(CComboBox *pBox, STRING_VAL_PAIR *list, LONG current_val);
    void SelectComboListItem(CComboBox *pBox, LONG val);
    void SelectComboListItem(CComboBox *pBox, CString string);
    void InitActionLists();
    void AddChildSignals(CString signal_id_name, LONG current_val);
    void AddAnimationIds(CString selected_name);
    void ResetAnimationIds(CString selected_name);
    void EnableDisableAnimationIdsCombo();
    void AddChildTargets(CComboBox *pBox, widget_info *info);
    void AddTargets(CComboBox *pBox);
    void ResetTargets(CComboBox *pBox);
    void ShowHideAnimationParams();
    void EnableDisableActionDeleteButton();

    BOOL SaveActionInfo();
    BOOL SaveAnimationInfo();

    DECLARE_MESSAGE_MAP()
protected:
    trigger_info         *mpTrigger;
    widget_info          *mpInfo;
    CArray<action_info *> mActionList;
    CArray<CString>       mNameList;
    int                   mAddedActionListSel;
    int                   mDisplayIndex;
    int                   mAnimationEasingFuncId;

    express_combobox mTargetCombo;
    express_combobox mParentCombo;
    CButton   mTargetShowChildWidgets;
    CButton   mParentShowChildWidgets;
    CStatic   mAnimationParams;
    CStatic   mOnAnimationCompleteGroup;
    CStatic   mAnimationParamLabels[ANIMATION_PARAMS_COUNT];
    custom_edit mAnimationParamEdits[ANIMATION_PARAMS_COUNT];
    CButton   mDetachTargetBox;
    CButton   mPushTargetBox;
    CEdit     mEasingFunctionEdit;
    CButton   mEasingFunctionButton;
    CListBox  mAddedActionList;
    CStatic   mStatusMsg;
};
#endif