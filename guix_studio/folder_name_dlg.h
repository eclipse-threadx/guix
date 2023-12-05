#pragma once


// folder_name_dlg dialog

class folder_name_dlg : public express_dialog
{
    DECLARE_DYNAMIC(folder_name_dlg)

public:
    folder_name_dlg::folder_name_dlg(res_info *info, CWnd* pParent =NULL);// standard constructor
    folder_name_dlg::folder_name_dlg(folder_info *info, CWnd* pParent = NULL);// standard constructor

    virtual ~folder_name_dlg();
    void GetEditFolderName(CString &new_folder_name) const;
    void GetSpecifiedOutputFileName(CString &new_folder_name) const;

// Dialog Data
    enum { IDD = IDD_PIX_FOLDER_NAME };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    

private:
    CString folder_name;
    CString output_filename;
    INT     input_item_type;

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEnChangeFolderName();
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedCustomOutput();

    virtual void OnOK();

};
