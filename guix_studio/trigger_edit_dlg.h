#ifndef _TRIGGER_EDIT_DLG_
#define _TRIGGER_EDIT_DLG_
#include "afxwin.h"

// Define trigger types
enum TRIGGER_TYPE{
    TRIGGER_TYPE_SYSTEM_EVENT = 1,
    TRIGGER_TYPE_CHILD_SIGNAL,
    TRIGGER_TYPE_USER_EVENT
};

class trigger_edit_dlg : public express_dialog
{
    DECLARE_DYNAMIC(trigger_edit_dlg)

public:
    trigger_edit_dlg(int display_index, flow_item *item, trigger_info *trigger, CWnd* pParent = NULL);   // standard constructor
    virtual ~trigger_edit_dlg();

    static int GetTriggerType(CString type_name);
    static CString GetTriggerTypeName(int type);
    static int GetEventType(CString type_name);
    static CString GetEventTypeName(int type);
    static void GetWidgetEventList(widget_info *info, CArray<INT> *list);
    static void AddChildSignals(CComboBox *pBox, CArray<CString> *pNameList, widget_info *info, CString signal_id_name, LONG current_val);

// Dialog Data
    enum { IDD = IDD_TRIGGER_EDIT_DLG };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnChangeSystemEvent();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    void AddComboList(CComboBox *pBox, STRING_VAL_PAIR *list, LONG current_val);
    void SelectComboListItem(CComboBox *pBox, LONG val);
    void SelectComboListItem(CComboBox *pBox, CString string);

    void AddChildSignals(CString signal_id_name, LONG current_val);
    void AddAnimationIds(CString selected_name);
    void EnableDisableAnimationIdsCombo();

    BOOL SaveEventType();
    BOOL SaveTriggerInfo();

    DECLARE_MESSAGE_MAP()
protected:
    flow_item           *mpFlowItem;
    trigger_info        *mpTrigger;
    widget_info         *mpInfo;
    trigger_info         mOldTrigger;
    int                  mDisplayIndex;
    CArray<CString>      mNameList;

    express_combobox mSystemEventsCombo;
    express_combobox mChildSignalsCombo;
    CEdit     mUserEventName;
    express_combobox mAnimationIdsCombo;
    CStatic   mAnimationIdsLabel;
};
#endif