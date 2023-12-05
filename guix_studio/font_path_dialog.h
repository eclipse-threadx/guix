#pragma once

#include "express_table_row.h"
#include "studiox_includes.h"
#include "afxwin.h"

class font_range_header : public CWnd
{
public:
    font_range_header(CFont* font) { mpFont = font; };
    ~font_range_header() {};
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

private:
    CFont* mpFont;
};


class font_range_row : public express_table_row
{
    public:
        font_range_row(CString &label, font_page_info *page_info, BOOL extended, INT row_id, CFont *font);
        void OnEditRangeField();
        void OnChangeEnabled();

        afx_msg int OnCreate(LPCREATESTRUCT lpcs);

        virtual BOOL PreTranslateMessage(MSG *pMsg);
        virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
        virtual void OnPaint();

    private:
        BOOL PreTranslateKeyDown(WPARAM keyvalue);

        DECLARE_MESSAGE_MAP()

    private:
        CString mNameText;
        font_page_info *mpInfo;

        CEdit   mStartField;
        CEdit   mEndField;
        CButton mEnabledBox;

        BOOL    mExtended;
        INT     mRowId;
        CFont* mpFont;
};

class range_list_win : public CWnd
{
    public:
        range_list_win(res_info *info, CFont *mpFont);
        ~range_list_win();
        void SaveRangeInfo(void);
        void CreateExtendedRangeEntry();
        void DeleteExtendedRangeEntry();
        BOOL Scroll(int delta);

        DECLARE_MESSAGE_MAP()
        afx_msg int OnCreate(LPCREATESTRUCT lpcs);
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
        afx_msg void OnSetFocus(CWnd* pOldWnd);

    private:
        CScrollHelper *m_scroll_helper;
        font_range_row *range_entry[NUM_FONT_CHAR_RANGES + NUM_FONT_EXTENDED_CHAR_RANGES];
        font_page_info mPageInfo[NUM_FONT_CHAR_RANGES + NUM_FONT_EXTENDED_CHAR_RANGES];
        res_info *m_info;
        CFont* mpFont;
        int page_count;
        int m_range_row_height;
};

// font_path_dialog dialog

class font_path_dialog : public express_dialog
{

public:
    font_path_dialog(int display_index, int color_format, res_info *info, CWnd* pParent = NULL);   // standard constructor
    virtual ~font_path_dialog();

    static font_page_info *CreateDefaultFontPages(BOOL include_extended_unicode = FALSE);
    static font_page_info *CreateExtendedFontPages();

// Dialog Data
    enum { IDD = IDD_EDIT_FONT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedBrowse();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedPathType();
    afx_msg void OnBnClickedCustomOutput();
    afx_msg void OnBnClickedSupportExtendedUnicode();
    afx_msg void OnEnChangeFontHeight();
    afx_msg void SetPathProjectRelative();
    afx_msg void SetPathStudioRelative();
    afx_msg void SetPathAbsolute();
    afx_msg void OnKillFontPathEdit();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();

    void OnPathnameChange();

private:
    res_info *m_info;
    int m_color_format;
    int m_display_index;
    int m_range_row_height;

public:
    range_list_win *mpRangeListWin;
    font_range_header *mpRangeListHeader;
    void SetPathPrefix();

private:
    CEdit mCustomFile;
    CButton mBinaryMode;
    CButton mPathTypeButton;
    CBitmap mPathBitmap;
    CStatic mPagesPrompt;
    PATHINFO pathinfo;
    INT      mHeight;
};
