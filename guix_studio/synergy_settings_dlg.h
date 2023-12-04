#pragma once
#include "afxwin.h"


// synergy_settings_dlg dialog

class synergy_settings_dlg : public express_dialog
{
    DECLARE_DYNAMIC(synergy_settings_dlg)

public:
    synergy_settings_dlg(CWnd* pParent, int current_display);   // standard constructor
    virtual ~synergy_settings_dlg();

    // Dialog Data
    enum { IDD = IDD_SYNERGY_SETTINGS };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void AddComboData(int control_id, STRING_VAL_PAIR *types, int current_val);
    virtual BOOL OnInitDialog();
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

protected:
    int m_active_display;
    CButton mD2DCheck;
    express_combobox mJpegDecoder;
    express_combobox mPngDecoder;
};
