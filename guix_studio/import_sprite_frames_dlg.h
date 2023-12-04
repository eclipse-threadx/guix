#ifndef _IMPORT_SPRITE_FRAMES_DLG_
#define _IMPORT_SPRITE_FRAMES_DLG_

// import_project_dlg dialog

class import_sprite_frames_dlg : public express_dialog
{
    DECLARE_DYNAMIC(import_sprite_frames_dlg)

public:
    import_sprite_frames_dlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~import_sprite_frames_dlg();

    // Dialog Data
    enum { IDD = IDD_IMPORT_SPRITE_FRAMES };

    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK();
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

public:
    CArray<CString>* GetFrameNameList() { return &mFrameNameList; }

private:
    void OnSelectPixFolder();
    void InitPixFolderList();
    void UpdateFrameList(res_info *folder_info);
    void UpdateListBoxHorizontalExtend(CListBox* pList);
    res_info *mPixGroupInfo;
    CListBox mPixFolderList;
    CListBox mFrameList;
    CArray<CString> mFrameNameList;
};

#endif