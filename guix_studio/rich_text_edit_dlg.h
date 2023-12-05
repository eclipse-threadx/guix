
#ifndef _RICH_TEXT_EDIT_DLG_
#define _RICH_TEXT_EDIT_DLG_
#include "afxcmn.h"

// rich_text_edit_dlg dialog

class custom_rich_edit: public CRichEditCtrl
{
public:
    custom_rich_edit();
    ~custom_rich_edit() {};


public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnSetFocus(CWnd* pOldWnd);

private:
    long mSelStart;
    long mSelEnd;
};

class text_edit_button_frame : public CWnd
{
public:
    text_edit_button_frame(widget_info *info, CWnd *pParent);
    ~text_edit_button_frame();

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnClickBold();
    afx_msg void OnClickItalic();
    afx_msg void OnClickUnderline();
    afx_msg void OnClickLeftAlign();
    afx_msg void OnClickCenterAlign();
    afx_msg void OnClickRightAlign();
    afx_msg void OnClickHighlightColor();
    afx_msg void OnClickTextColor();
    afx_msg void OnChangeFont();
    afx_msg void OnChangeTextColor();
    afx_msg void OnChangeHighlightColor();

protected:
    DECLARE_MESSAGE_MAP()

private:
    CString InitResList(CComboBox *box, int res_type, int sel_res_id);

private:
    custom_bitmap_button mBtnBold;
    custom_bitmap_button mBtnItalic;
    custom_bitmap_button mBtnUnderline;
    custom_bitmap_button mBtnLeftAlign;
    custom_bitmap_button mBtnCenterAlign;
    custom_bitmap_button mBtnRightAlign;
    express_combobox mFontBox;
    express_combobox mHighlightColorBox;
    express_combobox mTextColorBox;
    CStatic mFontLable;
    CStatic mHighlightColorLable;
    CStatic mTextColorLable;
    color_box_button mHighlightColorButton;
    color_box_button mTextColorButton;
    CString mHighlightColorName;
    CString mTextColorName;
    CString mFontName;

    int mLableWidth;
    int mColorButtonWidth;
    widget_info* mpInfo;
};

class rich_text_preview_win : public CWnd
{
public:
    rich_text_preview_win(widget_info *info, CWnd *parent);
    ~rich_text_preview_win();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnPaint();

public:
    void CreateDisplay();
    void MakeWidgetPreview();
    void DeleteWidgetString();
    void UpdateRichText(CString &richtext);

protected:
    CScrollHelper *m_scroll_helper;
    GX_DISPLAY  *mpDisplay;
    GX_WIDGET *mpWidget;
    GX_CANVAS   *mpCanvas;
    GX_WINDOW_ROOT *mpRoot;
    UCHAR *mpMem;
    widget_info *mpInfo;
    GX_BMP_INFO  m_bitmap;
};

class rich_text_edit_dlg : public express_dialog
{
    DECLARE_DYNAMIC(rich_text_edit_dlg)

public:
    rich_text_edit_dlg(widget_info *info, CWnd* pParent = NULL);   // standard constructor
    virtual ~rich_text_edit_dlg();

// Dialog Data
    enum { IDD = IDD_RICH_TEXT_EDIT_DLG };

protected:
    void PositionChildren();

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

public:
    void RichEditInit();
    void OnChangeRichText();
    void InsertTag(CString& tag_start, CString& tag_end);
    void SetSelectedBold();
    void SetSelectedItalic();
    void SetSelectedUnderline();
    void SetSelectedLeftAlign();
    void SetSelectedCenterAlign();
    void SetSelectedRightAlign();
    void SetSelectedHighlightColor(CString &id_name);
    void SetSelectedTextColor(CString &id_name);
    void SetSelectedFont(CString &id_name);
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
#if 0
    void FormatRichText();
#endif
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnOK();
    COLORREF MakeColorRef(int color_id);

    CString GetText();

private:
    custom_rich_edit mRichEdit;

    text_edit_button_frame *mpButtonFrame;
    rich_text_preview_win  *mpPreview;
    widget_info *mpInfo;
    studiox_project *mpProject;
    int mDisplay;
    palette_info mPalette;

    LOGFONT lf;

    int mMenubarHeight;
    int mMinDlgWidth;
    CString mText;

    IRichEditOle *mpIRich;
    ITextDocument *mpIDoc;
};

#endif
