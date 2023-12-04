#ifndef _RESOURCE_EXPORT_DLG_
#define _RESOURCE_EXPORT_DLG_

#define CUSTOM_TV_STATE_UNCHECKED          1
#define CUSTOM_TV_STATE_CHECKED            2
#define CUSTOM_TV_STATE_PARTIAL_CHECKED    3

class CustomTreeCtrl : public CTreeCtrl
{
public:
    CustomTreeCtrl(){};
    ~CustomTreeCtrl(){};
    afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNodeSelect(NMHDR* ph, LRESULT* pr);
    UINT GetItemCheckState(HTREEITEM hItem);
    DECLARE_MESSAGE_MAP()

public:
    CImageList mImageList;

public:
    void InitImageList();
    void SelectAllChildren(HTREEITEM hParent, BOOL bFlag);
    UINT GetChildrenState(HTREEITEM hParent);
    void SetItemState(HTREEITEM hItem, int state);
    HTREEITEM FindItem(HTREEITEM hStart, CString text);
    void UpdateParentState(HTREEITEM hItem);
    void UpdateChildrenState(HTREEITEM hItem);
    void OnSelectItem(HTREEITEM hItem);
};


//resource export dialog

class resource_export_dlg : public express_dialog
{
    DECLARE_DYNAMIC(resource_export_dlg)
    DECLARE_MESSAGE_MAP()
public:
    resource_export_dlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~resource_export_dlg();
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCustomResource();
    afx_msg void OnSelectSRecord();
    afx_msg void OnSelectBinary();
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    enum { IDD = IDD_RES_EXPORT };

protected:
    void ShowHideMemoryOffset();
    void PopulateResourceTree();
    void SaveChecks();
    void AddDisplays(HTREEITEM hParent);
    void AddLanguages(HTREEITEM hParent, int display_index);
    void SaveDisplayChecks(HTREEITEM hParent);
    void SaveLanguageChecks(HTREEITEM hParent, int display_index);
    BOOL b_modified;

protected:
    CustomTreeCtrl  mResView;
};
#endif