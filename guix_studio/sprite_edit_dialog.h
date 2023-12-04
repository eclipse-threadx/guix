#pragma once

#include "studiox_includes.h"
#include "afxwin.h"
#include "afxcmn.h"

// sprite_edit_dialog dialog

class sprite_edit_dialog : public express_dialog
{

public:
    sprite_edit_dialog(widget_info *info, CWnd* pParent = NULL);   // standard constructor
    virtual ~sprite_edit_dialog();

// Dialog Data
    enum { IDD = IDD_SPRITE_EDIT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg void OnDeltaposSpinNumFrames(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinEditFrame(NMHDR *pNMHDR, LRESULT *pResult);
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()

private:
    void SelectDropListItem(CComboBox* box, long val);
    void SetFrameFields();
    void ReadFrameFields();
    void SetFrameCount(int frame_count);

    widget_info *mpInfo;
    widget_info mTempInfo;
    CSpinButtonCtrl mSpinNumFrames;
    CSpinButtonCtrl mSpinEditFrame;
    CEdit mNumFramesField;
    CEdit mEditFrameField;
    express_combobox mBackgroundCombo;
    express_combobox mPixelmapCombo;
    custom_edit mXOffsetEdit;
    custom_edit mYOffsetEdit;
    int mEditFrame;

public:
    afx_msg void OnEnKillfocusNumFrames();
    afx_msg void OnEnKillfocusEditFrame();
    afx_msg void OnEnKillfocusAlphaValue();
    afx_msg void OnEnKillfocusXOffset();
    afx_msg void OnEnKillfocusYOffset();
    afx_msg void OnEnKillfocusDelay();
    afx_msg void OnClickApplyToAllFrames();
    afx_msg void OnImportFrames();
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
};
