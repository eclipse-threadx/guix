#ifndef _TRIGGER_LIST_EDIT_DLG_
#define _TRIGGER_LIST_EDIT_DLG_

#define VISIBLE_TRIGGER_ROWS 8
#define TRIGGER_ROW_HEIGHT 40

class trigger_list_edit_dlg;
class trigger_frame;

class trigger_row : public CWnd
{
public:
    trigger_row(CWnd *parent, int Id, CArray<trigger_info *> *trigger_list);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    
private:
    int mRowId;
    CArray<trigger_info *> *mpTriggerList;
    trigger_frame *frame;
    DECLARE_MESSAGE_MAP()
};

class trigger_frame : public CWnd
{
public:
    trigger_frame(CWnd *parent, CArray<trigger_info *> *trigger_list);
    ~trigger_frame();
    afx_msg void OnDestroy();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void AssignFocus(int rowId);
    void SetSelectedRow(int rowId, bool assign_focus = FALSE);
    int GetSelectedRow(){ return mSelectedRow; };
    void SelectedVisible();
    void CreateTriggerRows();
    void PositionChildren();
    DECLARE_MESSAGE_MAP()

private:
    CArray<trigger_info *> *mpTriggerList;
    int mSelectedRow;
    int mRowHeight;
    CScrollHelper *m_scroll_helper;
};

// trigger_list_edit_dlg dialog

class trigger_list_edit_dlg : public express_dialog
{
    DECLARE_DYNAMIC(trigger_list_edit_dlg)

public:
    trigger_list_edit_dlg(int display_index, screen_flow *flow, flow_item *item, CWnd* pParent = NULL);   // standard constructor
    virtual ~trigger_list_edit_dlg();
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnDestroy();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnAddTrigger();
    afx_msg void OnDeleteTrigger();
    afx_msg void OnEditTrigger();
    afx_msg void OnEditTriggerAction();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    int GetRowCount(){ return mTriggerList.GetCount(); };

protected:
    void EnableDisableTriggerEditControls();

// Dialog Data
    enum { IDD = IDD_TRIGGER_LIST_EDIT_DLG };

protected:
    screen_flow        *mpOldFlow;
    //trigger_connection *mpOldConnection;
    flow_item mItem;
    CArray<trigger_info *> mTriggerList;
    trigger_frame *mpTriggerFrame;
    int mDisplayIndex;

    DECLARE_MESSAGE_MAP()
};

#endif