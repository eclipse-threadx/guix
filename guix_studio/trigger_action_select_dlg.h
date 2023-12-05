#ifndef _TRIGGER_ACTION_SELECT_DLG_
#define _TRIGGER_ACTION_SELECT_DLG_
#include "afxwin.h"

class trigger_action_select_dlg : public express_dialog
{
    DECLARE_DYNAMIC(trigger_action_select_dlg)

public:
    trigger_action_select_dlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~trigger_action_select_dlg();

    // Dialog Data
    enum { IDD = IDD_TRIGGER_ACTION_SELECT_DLG };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnSelectActionList();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

public:
    static int GetActionType(CString type_name);
    static CString GetActionTypeName(int type);
    CString GetSelectedActionName(){ return mSelectedActionName; };
    int     GetSelectedActionType(){ return mSelectedActionType; };

protected:
    void UpdateListBoxHorizontalExtend(CListBox *pList);
    void InitActionLists();

    CListBox  mActionList;
    CString   mSelectedActionName;
    int       mSelectedActionType;
};
#endif