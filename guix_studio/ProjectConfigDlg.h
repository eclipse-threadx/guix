#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "express_combobox.h"

// ProjectConfigWin dialog

class ProjectConfigWin : public CDialog
{
    DECLARE_DYNAMIC(ProjectConfigWin)

public:
    ProjectConfigWin(CWnd* pParent = NULL, int current_display = 0);   // standard constructor
    virtual ~ProjectConfigWin();

    int GetGuixVersion();
    BOOL Save();
    BOOL Cancel();

// Dialog Data
    enum { IDD = IDD_CONFIGURE_PROJECT_WIN };

protected:
    virtual void OnOK() {};
    virtual void OnCancel() {};
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    void InitDisplayConfig(int display_num);
    void SaveDisplayConfig(int display_index);

    int  GetBitsSelected();
    void BitDepthChanged();
    void Format4444Clicked();
    void Format1555Clicked();
    void Format332Clicked();
    void EnableDisable(int bits_per_pix, studiox_project *project);
    void OffAndDisable(int id);
    void EnableDisableAdvancedSettings(int cpu_type);
    void EnableDisableAATextColorsSetting();
    void EnableDisableCCRX(int cpu_type);
    void InitAATextColorsCombobox();
    void InitRotationAnglesCombobox(int display);
    void EnableDisableFlipRotation(int cpu_type);

    CWnd *mpParent;
    CScrollHelper *m_scroll_helper;
    int m_current_display;
    studiox_project *mpProject;
    int mGuixVersion;
    express_combobox mCpuCombo;
    express_combobox mToolsCombo;
    express_combobox mAATextColorsCombo;
    project_header mOldHeader;

    CSpinButtonCtrl m_spin_current_screen;
    CSpinButtonCtrl m_spin_num_screens;

    CSpinButtonCtrl mSpinMajorVersion;
    CSpinButtonCtrl mSpinMinorVersion;
    CSpinButtonCtrl mSpinServicePack;

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnDeltaposSpinNumScreens(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinScreenSelect(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnEnEditNumScreens();
    afx_msg void OnEnEditSelectedDisplay();
    afx_msg void OnEnEditMajorVersion();
    afx_msg void OnEnEditMinorVersion();
    afx_msg void OnEnEditServicePack();

    afx_msg void OnBnClickedBrowseSource();
    afx_msg void OnBnClickedBrowseHeader();
    afx_msg void OnBnClickedBrowseResource();

    afx_msg void OnAdvancedSettings();
    afx_msg void OnChangeSelectedCPU();
    afx_msg void OnEditXRes();
    afx_msg void OnEditYRes();
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
};

// ProjectConfigDlg dialog

class ProjectConfigDlg : public express_dialog
{
    DECLARE_DYNAMIC(ProjectConfigDlg)

public:
    ProjectConfigDlg(CWnd* pParent = NULL, int current_display = 0);   // standard constructor
    virtual ~ProjectConfigDlg();

    static void CreateDefault1BppPalette(palette_info& info);
    static void CreateDefault4BppPalette(palette_info& info);
    static void CreateDefault332RGBPalette(palette_info& info);
    static void CreateDefault8BppPalette(palette_info& info);
    static void CreateDefaultPalette(studiox_project* project, int display, int theme);
    static COLOR_RECORD* GetDefaultColorTable(int color_format);
    static void ColorTableConvert(res_info* info, GX_COLOR* old_pal, int old_palsize, int old_color_format, GX_COLOR* new_pal, int new_palsize, int new_color_format);
    static GX_COLOR GetRGBColor(GX_COLOR colorval, GX_COLOR* old_pal, int old_palsize, int old_color_format);
    static GX_COLOR GetColorVal(GX_COLOR rgb_color, GX_COLOR* new_pal, int new_palsize, int new_color_format);
    static void CheckFontFormat(res_info* start, int m_display);
    static void DisableTextScrollWheelRoundStyle(widget_info* info);
    static void ResetPixelmapFormat(res_info* start, int m_display);
    static CString FindTargetCPUName(int val);
    static int FindTargetCPUVal(CString name);
    static CString FindTargetCompilerName(int val);
    static int FindTargetCompilerVal(CString name);
    static CString FindScreenRotationName(int val);
    static int FindScreenRotationVal(CString name);

    // Dialog Data
    enum { IDD = IDD_CONFIGURE_PROJECT };

public:
    int GetGuixVersion();
    void PositionChildren();

protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    ProjectConfigWin* mpProjectConfigWin;
    int mCurrentDisplay;
};
