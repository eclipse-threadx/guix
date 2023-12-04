#pragma once
#include "studiox_includes.h"
#include "afxcmn.h"

#define MIN_GRID_SNAP_SPACE 2
#define MAX_GRID_SNAP_SPACE 160

class grid_setting_dlg : public express_dialog
{
    DECLARE_DYNAMIC(grid_setting_dlg)
public:
    grid_setting_dlg(CWnd* pParent);
    virtual ~grid_setting_dlg();

    // Dialog Data
    enum { IDD = IDD_GRID_SETTINGS };
    virtual BOOL OnInitDialog();

protected:
    afx_msg void DoDataExchange(CDataExchange* pDX);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    void OnEditGridSpacing();
    void OnEditSnapSpacing();

    DECLARE_MESSAGE_MAP()

private:
    int mGridSpacingVal;
    int mSnapSpacingVal;

public:
    CSpinButtonCtrl mSpinGridSpacing;
    CSpinButtonCtrl mSpinSnapSpacing;
};

