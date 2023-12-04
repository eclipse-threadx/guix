#pragma once
#include "afxwin.h"
#include "studiox_includes.h"


// edit_pixelmap_dlg dialog

typedef struct PIXELMAP_INFO_STRUCT {
    int compress;
    int keep_alpha;
    int dither;
    int raw;
    int output_color_format;
    int output_file_enabled;
    int palette_type;
    CString output_file;
    BOOL binary_mode;
}pixelmap_info;

class edit_pixelmap_dlg : public express_dialog
{
    DECLARE_DYNAMIC(edit_pixelmap_dlg)

public:
    edit_pixelmap_dlg(res_info *info, CWnd* pParent = NULL);   // standard constructor
    virtual ~edit_pixelmap_dlg();

    afx_msg void OnBnClickedBrowse();
    afx_msg void OnEditImagePath();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedAlpha();
    afx_msg void OnBnClickedCustomOutput();
    afx_msg void OnEnChangeOutputFile();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedPathType();
    afx_msg void SetPathProjectRelative();
    afx_msg void SetPathStudioRelative();
    afx_msg void SetPathAbsolute();
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnBnClickedRawFormat();
    afx_msg void OnCbnSelchangeOutputFormat();
    afx_msg void OnCbangePaletteDef();
    //afx_msg void OnEnChangeIdName();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    static INT *GetAllowedFormats(int display_format);
    static BOOL IsOutputFormatSupported(int display_format, int output_format);

// Dialog Data
    enum { IDD = IDD_EDIT_PIXELMAP };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()
    CEdit mCustomFile;
    CButton mBinaryMode;
    res_info *mpInfo;
    int     mDisplayColorFormat;
    int     mDisplay;
    PATHINFO m_pathinfo;
    
private:
    void SetPathPrefix();
    BOOL CheckPathname(CString pathname);
    void EnableDisableRawFields();
    void ShowHidePaletteType(int selected_format);
    BOOL HasAlpha(res_info* info);
    void CheckAlphaStatus(BOOL Initialize = FALSE);
    void InitPixelmapDlgConfig();
    void SavePixelmapDlgConfig();
    BOOL IsPixelmapFolder();
    void SavePixelmapInfo(res_info *info);
    BOOL SavePixelmapInfo(int& val, int input_val);
    void GetPixelmapInfo(res_info *info);
    void GetPixelmapInfo(int &current_val, int input_val);
    void SetCheck(int control_id, int check);
    void GetCheck(int control_id, int &check);

    CButton mPathTypeButton;
    CBitmap mPathBitmap;

    express_combobox mOutputFormatCombo;
    CStatic mPaletteLabel;
    express_combobox mPaletteDefList;

    pixelmap_info mPixelmapInfo;
};
