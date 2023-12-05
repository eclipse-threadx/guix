#pragma once
#include "afxwin.h"
#include <string>
#include "resource.h"
#include "express_dialog.h"
#include "studiox_includes.h"
// PaletteLayoutDlg dialog

#define VISIBLE_PALETTE_ROWS 6

class palette_row : public express_table_row
{
    public:
        palette_row();
        void SetData(int index, GX_COLOR color, BOOL bEnabled);
        GX_COLOR GetData(void);
        afx_msg void OnPaint();
        virtual BOOL PreTranslateMessage(MSG *pMsg);

    protected:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnChar(UINT nChar, UINT repeat, UINT nFlags);
	    DECLARE_MESSAGE_MAP()
        virtual BOOL OnEraseBkgnd(CDC* pDC);
        void ColorChanged();
        
    private:

        CStatic mIndexPrompt;
        CEdit   mAlphaField;
        CEdit   mRedField;
        CEdit   mBlueField;
        CEdit   mGreenField;
        int     mIndex;
        BOOL    mEnabled;
        unsigned char mAlpha;
        unsigned char mRed;
        unsigned char mGreen;
        unsigned char mBlue;
        GX_COLOR mColor;
};

class palette_frame : public CWnd
{
public:
    palette_frame(theme_info *theme);
    ~palette_frame();

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd* pOldWnd);

public:
    void SetRowData(void);
    void SetTopIndex(int index) { mTopIndex = index; }
    INT  GetTopIndex() { return mTopIndex; }
    void NotifyColorChanged(int index, GX_COLOR color);
    void Scroll(int deltaPos);
    void UpdateStatusMsg(CString msg);

private:
    CStatic mStatusMsg;
    palette_row mPaletteRows[VISIBLE_PALETTE_ROWS];
    theme_info *mpTheme;
    int mTopIndex;
};

class PaletteLayoutDlg : public express_dialog
{
public:
	PaletteLayoutDlg(int display, int theme, CWnd* pParent = NULL);   // standard constructor
	virtual ~PaletteLayoutDlg();
    void NotifyColorChanged(int index, GX_COLOR color);
    void Scroll(int delta);

// Dialog Data
	enum { IDD = IDD_PALETTE_LAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnPaint();
    afx_msg void OnCancel();
    afx_msg void OnReturnKey();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *bar);
    afx_msg void OnBnClickedOk();
    afx_msg void OnEnChangeTotalPaletteEntries();
    afx_msg void OnEnChangeUserPaletteEntries();
    afx_msg void OnBnClickedImportPalette();
    afx_msg void OnBnClickedExportPalette();
    afx_msg void OnBnClickedDefaultPalette();
    afx_msg void OnGenGradient();

	DECLARE_MESSAGE_MAP()

private:
    int mDisplay;
    int mTheme;
    BOOL mPaletteResetToDefault;

    int old_pal_total_entries;
    int old_pal_predefined;
    GX_COLOR OldPalette[256];
    palette_frame *mpPaletteFrame;

    CScrollBar mPalIndexScroll;
    studiox_project *mpProject;
    void CheckResetColorIndexes(res_info *info, GX_COLOR *new_palette, USHORT new_pal_size);
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

public:
    afx_msg void OnEnChangeAutoPaletteEntries();
    virtual BOOL OnInitDialog();
};
