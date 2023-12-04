#ifndef _IMPORT_PROJECT_DLG_
#define _IMPORT_PROJECT_DLG_

// import_project_dlg dialog

class import_project_dlg : public express_dialog
{
    DECLARE_DYNAMIC(import_project_dlg)

public:
    import_project_dlg(studiox_project *merge_project, CString project_path, CWnd* pParent = NULL);   // standard constructor
    virtual ~import_project_dlg();

// Dialog Data
    enum { IDD = IDD_IMPORT_PROJECT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg LRESULT OnTestMessage(WPARAM, LPARAM);
    DECLARE_MESSAGE_MAP()

protected:
    void BuildProjectTree();
    studiox_project *mpMergeProject;
    CString          mProjectPath;
    CustomTreeCtrl   mProjectTree;
};

#endif
