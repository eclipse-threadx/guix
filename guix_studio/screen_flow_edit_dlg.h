#ifndef _SCREEN_FLOW_EDIT_DLG_
#define _SCREEN_FLOW_EDIT_DLG_

#define SCREEN_FLOW_MIN_SCALE 25
#define SCREEN_FLOW_MAX_SCALE 500

// screen_flow_edit_dlg dialog

struct trigger_connection{
    CString source_screen;
    CString target_screen;
    CPoint line_start;
    CPoint line_end;
};

class screen_flow_button_frame : public CWnd
{
public:
    screen_flow_button_frame(screen_flow *flow, CWnd *pParent = NULL);
    ~screen_flow_button_frame();

protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()

    void OnFitContent();
    void OnZoomOut();
    void OnZoomIn();
    void OnRunProject();
    void UpdateZoomValue();

private:
    screen_flow *mpScreenFlow;
    custom_bitmap_button mZoomInButton;
    custom_bitmap_button mZoomOutButton;
    custom_bitmap_button mFitContent;
    CStatic mZoomValueLable;
    CButton mRunButton;

    CFont mFont;
};

class screen_flow_diagram_win : public CWnd
{
public:
    screen_flow_diagram_win(int display, screen_flow *flow, CWnd* pParent = NULL);   // standard constructor
    ~screen_flow_diagram_win();

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void SelectFlowItem(flow_item* item);
    void HideFlowItem(flow_item *item);
    DECLARE_MESSAGE_MAP()

public:
    void InitConnectionList();
    void RemoveConnection(flow_item *source);
    void RemoveConnection(flow_item *source, flow_item *target);
    void CheckAddConnection(flow_item *source);
    void CheckAddConnection(flow_item *source, flow_item *target);
    void CalculateConnectionPos(trigger_connection *connection, flow_item *source, flow_item *target);
    void UpdateConnectionPos(flow_item *source);
    int CheckResizeCursor(CPoint point);
    void UpdateDisplaySize();
    void SelectedVisible();
    BOOL HandleKeydown(UINT message);
    VOID Zoom(int type);
    CRect GetScaledRect(CRect rect);
    CPoint GetScaledPoint(CPoint point);
    void ShiftLeft(CRect scaled_rect, CRect client, INT scaled_shift_x, INT shift_x);
    void ShiftTop(CRect scaled_rect, CRect client, INT scaled_shift_y, INT shift_y);
    void ShiftRight(CRect scaled_rect, CRect client, INT scaled_shift_x, INT shift_x);
    void ShiftBottom(CRect scaled_rect, CRect client, INT scaled_shift_y, INT shift_y);
    flow_item *FindFirstVisibleItem();
    BOOL EditFlowItem(flow_item* item);
    int GetScale() { return mScale; }

private:
    screen_flow *mpScreenFlow;
    CArray<trigger_connection *> mConnectionList;
    flow_item *mpDragItem;
    CPoint mDragStart;
    int    mDragMode;
    int    mActiveDisplay;
    CScrollHelper *m_scroll_helper;
    int mDisplayWidth;
    int mDisplayHeight;
    int mDiagramScale;
    int mSystemDpi;
    int mScale;
};

class screen_flow_edit_dlg : public express_dialog
{
    DECLARE_DYNAMIC(screen_flow_edit_dlg)

public:
    screen_flow_edit_dlg(int display, CWnd* pParent = NULL);   // standard constructor
    virtual ~screen_flow_edit_dlg();

// Dialog Data
    enum { IDD = IDD_SCREEN_FLOW_EDIT_DLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg LRESULT  OnShowHideScreenItem(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT  OnNodeSelect(WPARAM wParam, LPARAM lParam);
    BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

public:
    void PositionChildren();
    void InitScreenFlow(int diagram_width, int diagram_height);
    void PopulateScreenItems();
    screen_flow *GetOldFlow(){ return mpOldScreenFlow; };
    screen_flow *GetFlow(){ return mpScreenFlow; };
    void Zoom(int type){ mpScreenFlowDiagram->Zoom(type); };

private:
    int mActiveDisplay;
    screen_flow *mpScreenFlow;
    screen_flow *mpOldScreenFlow;
    screen_flow_diagram_win *mpScreenFlowDiagram;
    screen_flow_button_frame *mpButtonFrame;
    CustomTreeCtrl mScreenItemView;
};

#endif