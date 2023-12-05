#ifndef _TEXT_SCROLL_WHEEL_EDIT_DLG_
#define _TEXT_SCROLL_WHEEL_EDIT_DLG_
#include "afxwin.h"
#include "express_combobox.h"

#define VISIBLE_VALUE_ROWS 7

// scroll_wheel_edit_dlg dialog

class value_list_row : public express_table_row
{
public:
    value_list_row(int row_id);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    int GetRowId() { return mRowId; }
    void SetData(CString id, CString string);
    void InsertStringId(CString id_name, GX_RESOURCE_ID res_id);
    CWnd *GetTextEdit(){ return (CWnd *)&mStringTextEdit; };
    CWnd *GetIdCombo(){ return (CWnd *)&mStringIdCombo; };

protected:
    void OnChangeStringText();
    void OnChangeStringId();
    BOOL PreTranslateKeyDown(WPARAM keyvalue);

private:
    int           mRowId;
    CStatic       mIndexPrompt;
    express_combobox mStringIdCombo;
    CRichEditCtrl mStringTextEdit;
    CString       mStringId;
    CString       mStringText;
    DECLARE_MESSAGE_MAP()
};

class value_list_frame :public CWnd
{
public:
    value_list_frame(CArray<GX_RESOURCE_ID> *string_id_list, string_table *table);
    ~value_list_frame();

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* bar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnDestroy();
    afx_msg void OnSetFocus(CWnd* pOldWnd);

public:
    void NotifyStringIdAdded(CString id_name, GX_RESOURCE_ID res_id);
    void NotifyStringIdChanged(int index, GX_RESOURCE_ID id);
    string_table* GetStringTable() { return mpTable; }
    BOOL Scroll(int delta);
    int GetRowHeight() { return mRowHeight; }

private:
    CArray<GX_RESOURCE_ID>* mpStringIdList;
    string_table* mpTable;
    CScrollHelper* m_scroll_helper;
    int mRowHeight;
};

class string_scroll_wheel_edit_dlg : public express_dialog
{
    DECLARE_DYNAMIC(string_scroll_wheel_edit_dlg)

public:
    string_scroll_wheel_edit_dlg(int display_index, CArray<GX_RESOURCE_ID> *list_array, CWnd* pParent = NULL);   // standard constructor
    virtual ~string_scroll_wheel_edit_dlg();

// Dialog Data
    enum { IDD = IDD_TEXT_SCROLL_WHEEL_EDIT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    void TestSetRowData(int row_id, CString string);
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

private:
    CArray<GX_RESOURCE_ID>  mOldStringIdList;
    CArray<GX_RESOURCE_ID>* mpStringIdList;
    string_table* mpTable;
    studiox_project *mpProject;
    int              mDisplayIndex;
    value_list_frame *mpValueListFrame;

    DECLARE_MESSAGE_MAP()
};

#endif
